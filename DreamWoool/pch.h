#pragma once
// 包括 SDKDDKVer.h 将定义可用的最高版本的 Windows 平台。
// 如果要为以前的 Windows 平台生成应用程序，请包括 WinSDKVer.h，并
// 将 _WIN32_WINNT 宏设置为要支持的平台，然后再包括 SDKDDKVer.h。
#include <WinSDKVer.h>
#define _WIN32_WINNT 0x0601
#include <SDKDDKVer.h>
//=====================================================================================
// Use the C++ standard templated min/max
#define NOMINMAX
// DirectX apps don't need GDI //direct 2d 中有使用这个的地方 所以注释掉
//#define NODRAWTEXT
//#define NOGDI
#define NOBITMAP
// Include <mcx.h> if you need this
#define NOMCX
// Include <winsvc.h> if you need this
#define NOSERVICE
// WinHelp is deprecated
#define NOHELP
//Exclude rarely used content from Windows header files
#define WIN32_LEAN_AND_MEAN
//for enable algned storge
#define _ENABLE_EXTENDED_ALIGNED_STORAGE
//=====================================================================================
// C 
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
//=====================================================================================
//windows and directX
#include <windows.h>
#include <strsafe.h>
#include <wrl/client.h>
#include <d3d11_1.h>
#if defined(NTDDI_WIN10_RS2)
#include <dxgi1_6.h>
#else
#include <dxgi1_5.h>
#endif
#include <DirectXMath.h>
#include <DirectXColors.h>
#ifdef _DEBUG
#include <dxgidebug.h>
#endif
#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>
#include <stdint.h>
//=====================================================================================
//C++
#include <algorithm>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <memory>
#include <set>
#include <thread>
#include <utility>
#include <vector>
#include <queue>
#include <map>
#include <unordered_map>
#include <list>
#include <functional>
#include <mutex>

//=====================================================================================
//tools
#include "srcs/tools/StringUtil.h"
#include "srcs/tools/Logger.h"
#include "srcs/tools/DXtrace.h"
#include "srcs/tools/Utils.h"
//=====================================================================================
// Helper class for COM exceptions
namespace DX
{
	class com_exception : public std::exception
	{
	public:
		com_exception(HRESULT hr) : result(hr) {}

		virtual const char* what() const override
		{
			static char s_str[64] = {};
			sprintf_s(s_str, "Failure with HRESULT of %08X", static_cast<unsigned int>(result));
			return s_str;
		}

	private:
		HRESULT result;
	};

	// Helper utility converts D3D API failures into exceptions.
	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			throw com_exception(hr);
		}
	}
}
//=====================================================================================
// SafeRelease inline function.
template <class T> inline void SafeRelease(T **ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}
using namespace DirectX;