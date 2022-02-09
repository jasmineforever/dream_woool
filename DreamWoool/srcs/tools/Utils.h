#pragma once
#include <string>
#pragma comment(lib,"d3dcompiler.lib")
class Utils
{
public:
	static bool IsFileExist(const std::string& file_path);

	static bool CompileD3DShader(const char* filePath, const char* entry, const char*
		shaderModel, ID3DBlob** buffer);

	static std::string GetParentPath(const std::string& path, int level = 1);

	static std::string GetFileNameInPath(const std::string& path);
private:
	Utils();
	~Utils() = default;
};

