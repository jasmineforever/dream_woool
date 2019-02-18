#pragma once
#include <string>

class StringUtil
{
public:
	~StringUtil();
	static void InitLocale(const std::string& loc = "");

	static std::unique_ptr<wchar_t[]> CstrToWstr(const char* in);
	static std::wstring CstrToWstr(const std::string& in);

	static std::unique_ptr<char[]> WstrToCstr(const wchar_t* in);
	static std::string WstrToCstr(const std::wstring& in);

	static std::unique_ptr<char[]> Utf8ToAnsi(const char* in);
	static std::string Utf8ToAnsi(const std::string& in);

	static std::unique_ptr<char[]> AnsiToUtf8(const char* in);
	static std::string AnsiToUtf8(const std::string& in);
private:
	static std::unique_ptr<char[]> ConvertStringCode(int fromeCode, int toCode, const char* in);
private:
	StringUtil();
};

