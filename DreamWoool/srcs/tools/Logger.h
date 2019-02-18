#pragma once
//
// Created by xiaofeng on 6/12/17.
// Work on windows and linux 
//TODO add log to netWork
//#define ENABLE_REMOTE_LOG
//#define REMOTE_LOG_SERVER_IP "127.0.0.1"
//#define REMOTE_LOG_SERVER_PORT 10036

#include <string>
#include <stdarg.h>
#if defined(_WIN32)
#include <windows.h>
#elif defined(linux)
#include <pthread.h>
#include <unistd.h>
#include <syscall.h>
#include <time.h>
#include <sys/time.h>
#endif

#ifdef ENABLE_REMOTE_LOG
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#endif

class Logger;
//#define DISABLE_LOG
//#define LOG_FILE_NAME "E:\\Looger\\test.txt"
#define LOG_USING_LOCK
#if defined (_WIN32)
#define SPRINTF sprintf_s
#define VSPRINTF vsprintf_s
#define PRINTF printf_s
#define LOG_TRACE(format, ...) Logger::getLogger().writeLog(Logger::LOG_TYPE_TRACE, format, __VA_ARGS__)
#define LOG_DEBUG(format, ...) Logger::getLogger().writeLog(Logger::LOG_TYPE_DEBUG, format, __VA_ARGS__)
#define LOG_INFO(format, ...) Logger::getLogger().writeLog(Logger::LOG_TYPE_INFO, format, __VA_ARGS__)
#define LOG_WARNING(format, ...) Logger::getLogger().writeLog(Logger::LOG_TYPE_WARNING, format, __VA_ARGS__)
#define LOG_ERROR(format, ...) Logger::getLogger().writeLog(Logger::LOG_TYPE_ERROR, format, __VA_ARGS__)
#define LOG_FATAL(format, ...) Logger::getLogger().writeLog(Logger::LOG_TYPE_FATAL, format, __VA_ARGS__)
#elif defined(linux)
#define SPRINTF snprintf
#define VSPRINTF vsnprintf
#define PRINTF printf
#define LOG_TRACE(format, ...) Logger::getLogger().writeLog(Logger::LOG_TYPE_TRACE, format, ##__VA_ARGS__)
#define LOG_DEBUG(format, ...) Logger::getLogger().writeLog(Logger::LOG_TYPE_DEBUG, format, ##__VA_ARGS__)
#define LOG_INFO(format, ...) Logger::getLogger().writeLog(Logger::LOG_TYPE_INFO, format, ##__VA_ARGS__)
#define LOG_WARNING(format, ...) Logger::getLogger().writeLog(Logger::LOG_TYPE_WARNING, format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) Logger::getLogger().writeLog(Logger::LOG_TYPE_ERROR, format, ##__VA_ARGS__)
#define LOG_FATAL(format, ...) Logger::getLogger().writeLog(Logger::LOG_TYPE_FATAL, format, ##__VA_ARGS__)
#endif

class Mutex
{
public:
	Mutex();
	~Mutex();
private:
#if defined(_WIN32)
	HANDLE			_mutex;
#elif defined(linux)
	pthread_mutex_t	_mutex;
#endif
public:
	class scoped_lock
	{
	public:
		scoped_lock(Mutex& mutex);
		~scoped_lock();
	private:
		Mutex& _mutex;
	};
};

class Logger
{
public:
	typedef enum
	{
		LOG_TYPE_FATAL = 0,
		LOG_TYPE_ERROR,
		LOG_TYPE_WARNING,
		LOG_TYPE_INFO,
		LOG_TYPE_DEBUG,
		LOG_TYPE_TRACE
	} LogType;

	static Logger& getLogger();
	
	void disableLogToStdout();

	void setLogToDebug(bool enable);
	//default is 400KB
	void setLogFileMaxSize(int sizeInKb);

	void writeLog(LogType type, const char* szFormat, ...);
	//log level 0-2, default is 2
	void setLogLevel(int level);

	void showTimestamp(bool enabled);

	void showProcessID(bool enabled);

	void showThreadID(bool enabled);
private:
	Logger();

	~Logger();
#ifdef ENABLE_REMOTE_LOG
    void sendToRemote(char* pData, int size);
    int createNonBlockSocket();
#endif
private:
	static const char*		_logTyeString[];
	bool					_logToStdout;
	Mutex					_mutex;
	FILE*					_fp;
	long					_fileMaxSize;
	int						_logLevel;
	char*					_writeBuf;
	int						_writeBufSize;
	bool					_isShowThread;
	bool					_isShowProcess;
	bool					_isShowTimestamp;
	bool					_logToDebug;
#ifdef ENABLE_REMOTE_LOG
    int                     _socketFd;
    struct sockaddr_in      _socketAddr;
#endif
};