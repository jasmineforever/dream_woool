#include "pch.h"
#include "Utils.h"
#include  <io.h>
#include  <stdio.h>
#include <D3DCompiler.h>
#include <istream>
#include <codecvt>
#include <locale>

bool Utils::IsFileExist(const std::string& file_path)
{
	return (_access(file_path.c_str(), 0) == 0);
}
bool Utils::CompileD3DShader(const char* filePath, const char* entry, const char*
	shaderModel, ID3DBlob** buffer)
{
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined( DEBUG ) || defined( _DEBUG )

	shaderFlags |= D3DCOMPILE_DEBUG;

#endif
	ID3DBlob* errorBuffer = 0;
	HRESULT result;
	result = D3DCompileFromFile(StringUtil::CstrToWstr(filePath).get(), 0, 0, entry, shaderModel,
		shaderFlags, 0, buffer, &errorBuffer);

	if (FAILED(result))
	{
		if (errorBuffer != 0)
		{

			OutputDebugStringA((char*)errorBuffer->GetBufferPointer());
			errorBuffer->Release();
		}
		return false;
	}
	if (errorBuffer != 0)
		errorBuffer->Release();
	return true;
}
std::string Utils::GetFileNameInPath(const std::string& path)
{
	int point_index = -1;
	int split_index = -1;

	for (int i = path.size() - 1; i >= 0; i--)
	{
		if ('.' == path[i] && point_index == -1)
		{
			point_index = i;
		}
		else if ('/' == path[i] || '\\' == path[i])
		{
			split_index = i;
			break;
		}
	}
	if (split_index > 0 && point_index > split_index)
	{
		return path.substr(split_index + 1, point_index - split_index - 1);
	}
	return path;
}
std::string Utils::GetParentPath(const std::string& path, int level)
{
	int count = 0;;
	int index = -1;
	for (int i = path.size() - 1; i >= 0; i--)
	{
		if ('/' == path[i] || '\\' == path[i])
		{
			count++;
			if (count == level)
			{
				index = i; 
				break;
			}
		}
	}
	if (index >= 0 && index <= path.size())
	{
		return path.substr(0, index);
	}
	return path;
}