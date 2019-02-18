#include "pch.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "Logger.h"
#include "StringUtil.h"

Mutex::Mutex()
{
#if defined(_WIN32)
	_mutex = ::CreateMutex(NULL, FALSE, NULL);
#elif defined(linux)
	pthread_mutex_init(&_mutex, NULL);
#endif
}
Mutex::~Mutex()
{
#if defined(_WIN32)
	::CloseHandle(_mutex);
#elif defined(linux)
	pthread_mutex_destroy(&_mutex);
#endif
}
Mutex::scoped_lock::scoped_lock(Mutex& mutex):
_mutex(mutex)
{
#if defined(_WIN32)
	::WaitForSingleObject(_mutex._mutex, INFINITE);
#elif defined(linux)
	pthread_mutex_lock(&_mutex._mutex);
#endif
}
Mutex::scoped_lock::~scoped_lock()
{
#if defined(_WIN32)
	::ReleaseMutex(_mutex._mutex);
#elif defined(linux)
	pthread_mutex_unlock(&_mutex._mutex);
#endif
}
const char* Logger::_logTyeString[] = { "[fatal]", "[error]", "[warning]", "[info]", "[debug]", "[trace]"};

Logger::Logger():
	_logToStdout(true),
	_fp(NULL),
	_fileMaxSize(1024 * 400),
	_logLevel(6),
	_writeBuf(NULL),
	_isShowProcess(false),
	_isShowThread(false),
	_logToDebug(false),
	_isShowTimestamp(true),
	_writeBufSize(2048)
#ifdef ENABLE_REMOTE_LOG
    ,_socketFd(-1)
#endif
{
#if (defined(LOG_FILE_NAME) && defined(_WIN32))
	FILE* fp = NULL;
	fopen_s(&fp, LOG_FILE_NAME, "r+");
	if (!fp)
	{
		fopen_s(&fp, LOG_FILE_NAME, "w+");
		if (!fp)
		{
			return;
		}
	}
	fseek(fp, 0L, SEEK_END);
	_fp = fp;
#elif (defined(LOG_FILE_NAME) && defined(linux))
	FILE* fp = NULL;
	fopen(&fp, LOG_FILE_NAME, "r+");
	if (!fp)
	{
		fopen(&fp, LOG_FILE_NAME, "w+");
		if (!fp)
		{
			return;
		}
	}
	fseek(fp, 0L, SEEK_END);
	_fp = fp;
#endif

	_writeBuf = new char[_writeBufSize + 1];
#ifdef ENABLE_REMOTE_LOG
    _socketFd = createNonBlockSocket();
    bzero(&_socketAddr, sizeof(_socketAddr));
    _socketAddr.sin_family = AF_INET;
    _socketAddr.sin_port = htons(REMOTE_LOG_SERVER_PORT);
    _socketAddr.sin_addr.s_addr = inet_addr(REMOTE_LOG_SERVER_IP);
#endif

}
Logger::~Logger()
{
	if (_fp != NULL)
	{
		fclose(_fp);
	}
	if (_writeBuf)
	{
		delete[] _writeBuf;
	}

}
void Logger::setLogToDebug(bool enable)
{
	_logToDebug = enable;
}
void Logger::disableLogToStdout()
{
	_logToStdout = false;
}
void Logger::showTimestamp(bool enabled)
{
	_isShowTimestamp = enabled;
}
void Logger::showProcessID(bool enabled)
{
	_isShowProcess = enabled;
}
void Logger::showThreadID(bool enabled)
{
	_isShowThread = enabled;
}
void Logger::setLogLevel(int level)
{
	if (level > -1 && level < 6)
	{
		_logLevel = level;
	}
}
void Logger::setLogFileMaxSize(int sizeInKb)
{
	if (sizeInKb > 0)
	{
		//max is 1GB
		if (sizeInKb < 1024 * 1024)
		{
			_fileMaxSize = sizeInKb * 1024;
		}
		else
		{
			_fileMaxSize = 1024 * 1024 * 1024;
		}
	}
}
#ifdef ENABLE_REMOTE_LOG
int Logger::createNonBlockSocket()
{
    int fd = -1;
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd < 0)
    {
        return fd;
    }
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    return fd;
}
void Logger::sendToRemote(char* pData, int size)
{
    if(_socketFd < 0)
    {
        _socketFd = createNonBlockSocket();
        if(_socketFd < 0)
        {
            perror("can't create log socket");
            return;
        }
    }
    if(pData && size > 0)
    {
        if (sendto(_socketFd, pData, size , 0, (struct sockaddr*)&_socketAddr, sizeof(_socketAddr)) < 0)
        {
            PRINTF("in send log to remote err: %d\n", errno);
            close(_socketFd);
            _socketFd = -1;
        }
    }
}
#endif
void Logger::writeLog(LogType type, const char* szFormat, ...)
{
#ifndef DISABLE_LOG

	if ((int)type > _logLevel)
	{
		return;
	}
#if defined (LOG_USING_LOCK)
	Mutex::scoped_lock lock(_mutex);
#endif
	int nowSize = 0;
	if (_isShowTimestamp)
	{
#if defined(_WIN32)
		SYSTEMTIME start;
		GetLocalTime(&start);
		nowSize = SPRINTF(_writeBuf, _writeBufSize, "[%4d-%.2d-%.2d %.2d:%.2d:%.2d:%.3d]", start.wYear, start.wMonth, start.wDay, start.wHour, start.wMinute, start.wSecond, start.wMilliseconds);
#elif defined(linux)
		struct timeval   tv;
		gettimeofday(&tv, NULL);
		struct tm *start = localtime(&tv.tv_sec);
		nowSize = SPRINTF(_writeBuf, _writeBufSize, "[%4d-%.2d-%.2d %.2d:%.2d:%.2d:%.3d]", start->tm_year + 1900, start->tm_mon + 1, start->tm_mday, start->tm_hour, start->tm_min, start->tm_sec, (int)tv.tv_usec / 1000);
#endif
	}
	if (nowSize < 0)
		return;
	if (_isShowProcess)
	{
#if defined(_WIN32)
		DWORD processId = GetCurrentProcessId();
#elif defined(linux)
		pid_t processId = getpid();
#endif
		int nLen = SPRINTF(_writeBuf + nowSize, _writeBufSize - nowSize, "[p-%5d]", (int)processId);
		if (nLen < 0)
			return;
		nowSize += nLen;
	}
	if (_isShowThread)
	{
#if defined(_WIN32)
		DWORD threadId = GetCurrentThreadId();
#elif defined(linux)
		long threadId = syscall(SYS_gettid);
#endif
		int nLen = SPRINTF(_writeBuf + nowSize, _writeBufSize - nowSize, "[t-%5d]", (int)threadId);
		if (nLen < 0)
			return;
		nowSize += nLen;
	}

	int nLen = SPRINTF(_writeBuf + nowSize, _writeBufSize - nowSize, "%s\t", _logTyeString[type]);
	if (nLen < 0)
		return;
	nowSize += nLen;

	va_list arglist;
	va_start(arglist, szFormat);
	size_t strLen = strlen(szFormat);
	char* pFormat = (char *)malloc((strLen + 2) * sizeof(char));
	memcpy(pFormat, szFormat, strLen);
	pFormat[strLen] = '\n';
	pFormat[strLen + 1] = '\0';
	int appendLength = VSPRINTF(_writeBuf + nowSize, _writeBufSize - nowSize, pFormat, arglist);

	if (appendLength < 0)
	{
		free(pFormat);
		return;
	}
	int totalSize = nowSize + appendLength;
	_writeBuf[totalSize] = '\0';
	if (_logToStdout)
		PRINTF("%s", _writeBuf);
	if (_logToDebug)
	{
#if defined(_WIN32)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED);
		OutputDebugString(StringUtil::CstrToWstr(_writeBuf).get());
#endif
	}
    free(pFormat);
#ifdef ENABLE_REMOTE_LOG
    sendToRemote(_writeBuf, totalSize);
#endif
	if (!_fp)
	{
		return ;
	}
	long nowFileSize = ftell(_fp);
	if (nowFileSize >= _fileMaxSize)
	{
		fseek(_fp, 0L, SEEK_SET);
	}
	fwrite(_writeBuf, 1, totalSize, _fp);
	fflush(_fp);

	va_end(arglist);
#endif
}
Logger& Logger::getLogger()
{
	static Logger logger;
	return logger;
}