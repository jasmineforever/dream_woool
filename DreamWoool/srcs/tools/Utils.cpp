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