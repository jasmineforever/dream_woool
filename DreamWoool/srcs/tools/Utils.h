#pragma once
#include <string>
#pragma comment(lib,"d3dcompiler.lib")
class Utils
{
public:
	static bool IsFileExist(const std::string& file_path);

	static bool CompileD3DShader(const char* filePath, const char* entry, const char*
		shaderModel, ID3DBlob** buffer);
private:
	Utils();
	~Utils() = default;
};

