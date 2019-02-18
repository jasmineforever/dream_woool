#include "pch.h"
#include "StringUtil.h"
#include <iostream>
#include <stdlib.h>
#include <Windows.h>

StringUtil::StringUtil()
{

}
StringUtil::~StringUtil()
{

}
void StringUtil::InitLocale(const std::string& loc)
{
	std::locale::global(std::locale(loc));
}
std::unique_ptr<wchar_t[]> StringUtil::CstrToWstr(const char* in)
{
	if (in)
	{
		size_t new_size = strlen(in) + 1;
		std::unique_ptr<wchar_t[]> wcstring(new wchar_t[new_size]);
		size_t converted_num = 0;
		mbstowcs_s(&converted_num, wcstring.get(), new_size, in, _TRUNCATE);
		return wcstring;
	}
	return nullptr;
}

std::wstring StringUtil::CstrToWstr(const std::string& in)
{
	auto ptr = CstrToWstr(in.c_str());
	if (ptr)
	{
		return std::wstring(ptr.get());
	}
	return L"";
}

std::unique_ptr<char[]> StringUtil::WstrToCstr(const wchar_t* in)
{
	if (in)
	{
		size_t in_size = wcslen(in) + 1;
		size_t converted_num = 0;

		const size_t new_size = in_size * 2;
		std::unique_ptr<char[]> cstring(new char[new_size]);

		wcstombs_s(&converted_num, cstring.get(), new_size, in, _TRUNCATE);
		return cstring;
	}
	return nullptr;
}

std::string StringUtil::WstrToCstr(const std::wstring& in)
{
	auto ptr = WstrToCstr(in.c_str());
	if (ptr)
	{
		return std::string(ptr.get());
	}
	return "";
}

std::unique_ptr<char[]> StringUtil::Utf8ToAnsi(const char* in)
{
	return ConvertStringCode(CP_UTF8, CP_ACP, in);
}

std::string StringUtil::Utf8ToAnsi(const std::string& in)
{
	auto ptr = ConvertStringCode(CP_UTF8, CP_ACP, in.c_str());
	if (ptr)
	{
		return std::string(ptr.get());
	}
	return "";
}

std::unique_ptr<char[]> StringUtil::AnsiToUtf8(const char* in)
{
	return ConvertStringCode(CP_ACP, CP_UTF8, in);
}

std::string StringUtil::AnsiToUtf8(const std::string& in)
{
	auto ptr = ConvertStringCode(CP_ACP, CP_UTF8, in.c_str());
	if (ptr)
	{
		return std::string(ptr.get());
	}
	return "";
}

std::unique_ptr<char[]> StringUtil::ConvertStringCode(int fromeCode, int toCode, const char* in)
{
	if (in)
	{
		if ((fromeCode == CP_UTF8 && toCode == CP_ACP) || (fromeCode == CP_ACP && toCode == CP_UTF8))
		{
			int len = MultiByteToWideChar(fromeCode, 0, (LPCCH)in, -1, NULL, 0);
			auto unicode_buf = std::unique_ptr<wchar_t[]>(new wchar_t[len + 1]);
			MultiByteToWideChar(fromeCode, 0, (LPCCH)in, -1, (LPWSTR)unicode_buf.get(), len);
			len = WideCharToMultiByte(toCode, 0, (LPCWCH)unicode_buf.get(), -1, NULL, 0, NULL, NULL);
			auto dest_buf = std::unique_ptr<char[]>(new char[len + 1]);
			WideCharToMultiByte(toCode, 0, (LPCWCH)unicode_buf.get(), -1, dest_buf.get(), len, NULL, NULL);
			return dest_buf;
		}
		else
		{
			std::cerr << "in ConvertStringCode: Unsupported format conversion, from "
				<< fromeCode << " to " << toCode << std::endl;
		}
	}
	return nullptr;
}