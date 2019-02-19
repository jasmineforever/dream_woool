#include "pch.h"
#include <sstream>
#include "ID3DApp.h"

using namespace DirectX;
using namespace DX;
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return DW::ID3DApp::GetInstance()->MsgProc(hwnd, msg, wParam, lParam);
}
#if defined(_DEBUG)
// Check for SDK Layer support.
inline bool SdkLayersAvailable()
{
	HRESULT hr = D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_NULL,       // There is no need to create a real hardware device.
		0,
		D3D11_CREATE_DEVICE_DEBUG,  // Check for the SDK layers.
		nullptr,                    // Any feature level will do.
		0,
		D3D11_SDK_VERSION,
		nullptr,                    // No need to keep the D3D device reference.
		nullptr,                    // No need to know the feature level.
		nullptr                     // No need to keep the D3D device context reference.
	);
	return SUCCEEDED(hr);
}
#endif
namespace DW
{
	ID3DApp* ID3DApp::instance_ = nullptr;
	ID3DApp::ID3DApp(HINSTANCE h_instance, 
		size_t width, size_t height, 
		const std::string& window_name,
		unsigned int flags):
		h_instance_(h_instance),
		window_width_(width),
		window_height_(height),
		window_name_(StringUtil::CstrToWstr(window_name)),
		d3d_options_(flags | C_FLIP_PRESENT),
		hwnd_(nullptr),
		is_paused_(false)
	{
		instance_ = this;
		timer_.SetFixedTimeStep(true);
	}

	ID3DApp::~ID3DApp()
	{
		CoUninitialize();
	}
	SimpleMath::Size ID3DApp::GetWindowSize()
	{
		return SimpleMath::Size(window_width_, window_height_);
	}
	bool ID3DApp::Init()
	{
		bool ret = false;
		do
		{
			if (!InitEnv())
				break;

			if (!InitWindow())
				break;

			if (!InitD3D())
				break;

			if (!InitD2D())
				break;

			ret = true;
		} while (0);

		return ret;
	}
	bool ID3DApp::InitEnv()
	{
		bool ret = false;
		StringUtil::InitLocale();
		Logger::getLogger().disableLogToStdout();
		Logger::getLogger().setLogToDebug(true);
		
		do
		{
			if (!XMVerifyCPUSupport())
				break;

			HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
			if (FAILED(hr))
				break;
			ret = true;
		} while (0);

		return ret;
	}
	bool ID3DApp::InitWindow()
	{
		bool ret = false;
		do
		{
			WNDCLASSEX wcex = {};
			wcex.cbSize = sizeof(WNDCLASSEX);
			wcex.style = CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc = MainWndProc;
			wcex.hInstance = h_instance_;
			wcex.hIcon = LoadIcon(h_instance_, L"MAIN_ICO");
			wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
			wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			wcex.lpszClassName = window_name_.c_str();
			wcex.hIconSm = LoadIcon(h_instance_, L"MAIN_ICO");

			if (!RegisterClassEx(&wcex))
				break;
			// Create window
			
			RECT rc = { 0, 0, static_cast<LONG>(window_width_), static_cast<LONG>(window_height_) };

			AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

			hwnd_ = CreateWindowEx(0, window_name_.c_str(), window_name_.c_str(), WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,
				CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, h_instance_,
				nullptr);

			if (!hwnd_)
				break;

			ShowWindow(hwnd_, SW_SHOW);
			ret = true;
		}
		while (0);
		return ret;
	}

	bool ID3DApp::InitD2D()
	{
		HR(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, d2d_factory_.GetAddressOf()));
		HR(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(dwrite_factory_.GetAddressOf())));

		// 为D2D创建DXGI表面渲染目标
		ComPtr<IDXGISurface> surface;
		HR(swap_chain_->GetBuffer(0, __uuidof(IDXGISurface), reinterpret_cast<void**>(surface.GetAddressOf())));
		D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));
		HR(d2d_factory_->CreateDxgiSurfaceRenderTarget(surface.Get(), &props, d2d_render_target_.GetAddressOf()));
		surface.Reset();

		return true;
	}

	bool ID3DApp::InitD3D()
	{
		bool ret = false;
		do
		{
			UINT create_flag = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
			if (SdkLayersAvailable())
			{
				// If the project is in a debug build, enable debugging via SDK Layers with this flag.
				create_flag |= D3D11_CREATE_DEVICE_DEBUG;
			}
			else
			{
				LOG_WARNING("Direct3D Debug Device is not available");
			}
#endif
			ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(dxgi_factory_.ReleaseAndGetAddressOf())));

			// Determines whether tearing support is available for fullscreen borderless windows.
			if (d3d_options_ & C_ALLOW_TEARING)
			{
				BOOL allow = FALSE;
				ComPtr<IDXGIFactory5> factory5;
				HRESULT hr = dxgi_factory_.As(&factory5);
				if (SUCCEEDED(hr))
				{
					hr = factory5->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allow, sizeof(allow));
				}
				if (FAILED(hr) || !allow)
				{
					d3d_options_ &= ~C_ALLOW_TEARING;
					LOG_WARNING("Variable refresh rate displays not supported");
				}
			}
			// Disable FLIP if not on a supporting OS
			if (d3d_options_ & C_FLIP_PRESENT)
			{
				ComPtr<IDXGIFactory4> factory4;
				if (FAILED(dxgi_factory_.As(&factory4)))
				{
					d3d_options_ &= ~C_FLIP_PRESENT;
					LOG_WARNING("Flip swap effects not supported");
				}
			}
			// Determine DirectX hardware feature levels this app will support.
			static const D3D_FEATURE_LEVEL s_feature_levels[] =
			{
				D3D_FEATURE_LEVEL_11_1,
				D3D_FEATURE_LEVEL_11_0,
				D3D_FEATURE_LEVEL_10_1,
				D3D_FEATURE_LEVEL_10_0,
				D3D_FEATURE_LEVEL_9_3,
				D3D_FEATURE_LEVEL_9_2,
				D3D_FEATURE_LEVEL_9_1,
			};

			
			ComPtr<IDXGIAdapter1> adapter;
			GetHardwareAdapter(adapter.GetAddressOf());

			// Create the Direct3D 11 API device object and a corresponding context.
			ComPtr<ID3D11Device> device;
			ComPtr<ID3D11DeviceContext> context;

			HRESULT hr = E_FAIL;
			if (adapter)
			{
				hr = D3D11CreateDevice(
					adapter.Get(),
					D3D_DRIVER_TYPE_UNKNOWN,
					0,
					create_flag,
					s_feature_levels,
					sizeof(s_feature_levels) / sizeof(D3D_FEATURE_LEVEL),
					D3D11_SDK_VERSION,
					device.GetAddressOf(),  // Returns the Direct3D device created.
					&d3d_feature_level_,     // Returns feature level of device created.
					context.GetAddressOf()  // Returns the device immediate context.
				);
			}
			if (FAILED(hr))
			{
				// If the initialization fails, fall back to the WARP device.
				// For more information on WARP, see: 
				// http://go.microsoft.com/fwlink/?LinkId=286690
				hr = D3D11CreateDevice(
					nullptr,
					D3D_DRIVER_TYPE_WARP, // Create a WARP device instead of a hardware device.
					0,
					create_flag,
					s_feature_levels,
					sizeof(s_feature_levels) / sizeof(D3D_FEATURE_LEVEL),
					D3D11_SDK_VERSION,
					device.GetAddressOf(),
					&d3d_feature_level_,
					context.GetAddressOf()
				);

				if (SUCCEEDED(hr))
				{
					LOG_INFO("Direct3D Adapter - WARP");
				}
			}
			ThrowIfFailed(hr);
#ifdef _DEBUG
			ComPtr<ID3D11Debug> d3dDebug;
			if (SUCCEEDED(device.As(&d3dDebug)))
			{
				ComPtr<ID3D11InfoQueue> d3dInfoQueue;
				if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue)))
				{

					d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
					d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
					D3D11_MESSAGE_ID hide[] =
					{
						D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
					};
					D3D11_INFO_QUEUE_FILTER filter = {};
					filter.DenyList.NumIDs = _countof(hide);
					filter.DenyList.pIDList = hide;
					d3dInfoQueue->AddStorageFilterEntries(&filter);
				}
			}
#endif
			ThrowIfFailed(device.As(&d3d_device_));
			ThrowIfFailed(context.As(&d3d_context_));

			// Create a descriptor for the swap chain.
			DXGI_SWAP_CHAIN_DESC1 swap_chain_desc = {};
			swap_chain_desc.Width = window_width_;
			swap_chain_desc.Height = window_height_;
			swap_chain_desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
			swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swap_chain_desc.BufferCount = 2;
			{
				UINT m4x_msaa_quality;
				d3d_device_->CheckMultisampleQualityLevels(DXGI_FORMAT_B8G8R8A8_UNORM, 4, &m4x_msaa_quality);
				if (m4x_msaa_quality <= 0)
				{
					LOG_WARNING("CheckMultisampleQualityLevels < 0");
				}
				if (d3d_options_ & C_ENABLE_4XMSAA)
				{
					swap_chain_desc.SampleDesc.Count = 4;
					swap_chain_desc.SampleDesc.Quality = m4x_msaa_quality - 1;
				}
				else
				{
					swap_chain_desc.SampleDesc.Count = 1;
					swap_chain_desc.SampleDesc.Quality = 0;
				}
			}
			swap_chain_desc.Scaling = DXGI_SCALING_STRETCH;
			swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			swap_chain_desc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
			swap_chain_desc.Flags = (d3d_options_ & C_ALLOW_TEARING) ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

			DXGI_SWAP_CHAIN_FULLSCREEN_DESC fs_swap_chain_desc = {};
			fs_swap_chain_desc.Windowed = TRUE;

			// Create a SwapChain from a Win32 window.
			HR(dxgi_factory_->CreateSwapChainForHwnd(
				d3d_device_.Get(),
				hwnd_,
				&swap_chain_desc,
				&fs_swap_chain_desc,
				nullptr, swap_chain_.ReleaseAndGetAddressOf()
			));
			// This class does not support exclusive full-screen mode and prevents DXGI from responding to the ALT+ENTER shortcut
			HR(dxgi_factory_->MakeWindowAssociation(hwnd_, DXGI_MWA_NO_ALT_ENTER));

			ret = InitWindowSizeDependentResources();
		}
		while (0);
		return ret;
	}

	bool ID3DApp::InitWindowSizeDependentResources()
	{
		bool ret = false;
		if (!hwnd_)
		{
			LOG_FATAL("window not init");
			return ret;
		}
		// Clear the previous window size specific context.
		ID3D11RenderTargetView* nullViews[] = { nullptr };
		d3d_context_->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
		d3d_render_target_view_.Reset();
		d3d_depth_stencil_view_.Reset();
		render_target_.Reset();
		depth_stencil_.Reset();
		d3d_context_->Flush();

		// Create a render target view of the swap chain back buffer.
		ThrowIfFailed(swap_chain_->GetBuffer(0, IID_PPV_ARGS(render_target_.ReleaseAndGetAddressOf())));

		CD3D11_RENDER_TARGET_VIEW_DESC render_target_view_desc(D3D11_RTV_DIMENSION_TEXTURE2D, DXGI_FORMAT_B8G8R8A8_UNORM);
		ThrowIfFailed(d3d_device_->CreateRenderTargetView(
			render_target_.Get(),
			&render_target_view_desc,
			d3d_render_target_view_.ReleaseAndGetAddressOf()
		));
		// Create a depth stencil view for use with 3D rendering if needed.
		CD3D11_TEXTURE2D_DESC depthStencilDesc(
			DXGI_FORMAT_D32_FLOAT,
			window_width_,
			window_height_,
			1, // This depth stencil view has only one texture.
			1, // Use a single mipmap level.
			D3D11_BIND_DEPTH_STENCIL
		);

		HR(d3d_device_->CreateTexture2D(
			&depthStencilDesc,
			nullptr,
			depth_stencil_.ReleaseAndGetAddressOf()
		));

		CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
		HR(d3d_device_->CreateDepthStencilView(
			depth_stencil_.Get(),
			&depthStencilViewDesc,
			d3d_depth_stencil_view_.ReleaseAndGetAddressOf()
		));
		
		// Set the 3D rendering viewport to target the entire window.
		screen_viewport_ = CD3D11_VIEWPORT(
			0.0f,
			0.0f,
			static_cast<float>(window_width_),
			static_cast<float>(window_height_)
		);

		return true;
	}
	// This method acquires the first available hardware adapter.
	// If no such adapter can be found, *ppAdapter will be set to nullptr.
	void ID3DApp::GetHardwareAdapter(IDXGIAdapter1** ppAdapter)
	{
		*ppAdapter = nullptr;

		ComPtr<IDXGIAdapter1> adapter;

#if defined(__dxgi1_6_h__) && defined(NTDDI_WIN10_RS4)
		ComPtr<IDXGIFactory6> factory6;
		HRESULT hr = dxgi_factory_.As(&factory6);
		if (SUCCEEDED(hr))
		{
			for (UINT adapterIndex = 0;
				DXGI_ERROR_NOT_FOUND != factory6->EnumAdapterByGpuPreference(
					adapterIndex,
					DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
					IID_PPV_ARGS(adapter.ReleaseAndGetAddressOf()));
				adapterIndex++)
			{
				DXGI_ADAPTER_DESC1 desc;
				adapter->GetDesc1(&desc);

				if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
				{
					// Don't select the Basic Render Driver adapter.
					continue;
				}
				wchar_t buff[256] = {};
				swprintf_s(buff, L"Direct3D Adapter (%u): VID:%04X, PID:%04X - %ls\n", adapterIndex, desc.VendorId, desc.DeviceId, desc.Description);
				LOG_INFO(StringUtil::WstrToCstr(buff).get());
				break;
			}
		}
		else
#endif
			for (UINT adapterIndex = 0;
				DXGI_ERROR_NOT_FOUND != dxgi_factory_->EnumAdapters1(
					adapterIndex,
					adapter.ReleaseAndGetAddressOf());
				adapterIndex++)
		{
			DXGI_ADAPTER_DESC1 desc;
			adapter->GetDesc1(&desc);

			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				// Don't select the Basic Render Driver adapter.
				continue;
			}
			wchar_t buff[256] = {};
			swprintf_s(buff, L"Direct3D Adapter (%u): VID:%04X, PID:%04X - %ls\n", adapterIndex, desc.VendorId, desc.DeviceId, desc.Description);
			LOG_INFO(StringUtil::WstrToCstr(buff).get());
			break;
		}
		*ppAdapter = adapter.Detach();
	}

	int	ID3DApp::Run()
	{
		MSG msg = { 0 };

		timer_.ResetElapsedTime();

		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				timer_.Tick([&]() {

					Update(timer_.GetElapsedSeconds());
				});
				UpdateFrameAndShow();
				Render();
			}
		}

		return (int)msg.wParam;
	}

	void ID3DApp::UpdateFrameAndShow()
	{
		static int frame_count = 0;
		static double time_elapsed = 0.0f;

		frame_count++;

		if ((timer_.GetTotalSeconds() - time_elapsed) >= 1.0f)
		{
			wchar_t out_buf[64];
			StringCbPrintfW(out_buf, 64, L"%ls  fps:%4d", window_name_.c_str(), frame_count);
			SetWindowText(hwnd_, out_buf);

			// Reset for next average.
			frame_count = 0;
			time_elapsed += 1.0f;
		}
	}
	LRESULT ID3DApp::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
			// WM_ACTIVATE is sent when the window is activated or deactivated.  
			// We pause the game when the window is deactivated and unpause it 
			// when it becomes active.  
		case WM_ACTIVATE:
			return 0;
		case WM_ACTIVATEAPP:
			return 0;

			// WM_SIZE is sent when the user resizes the window.  
		case WM_SIZE:
			LOG_INFO("resize window");
			return 0;

			// WM_DESTROY is sent when the window is being destroyed.
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
			// Catch this message so to prevent the window from becoming too small.
		case WM_GETMINMAXINFO:
			((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
			((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
			return 0;

			// 监测这些键盘/鼠标事件
		//case WM_INPUT:

		//case WM_LBUTTONDOWN:
		//case WM_MBUTTONDOWN:
		//case WM_RBUTTONDOWN:
		//case WM_XBUTTONDOWN:

		//case WM_LBUTTONUP:
		//case WM_MBUTTONUP:
		//case WM_RBUTTONUP:
		//case WM_XBUTTONUP:

		//case WM_MOUSEWHEEL:
		//case WM_MOUSEHOVER:
		//case WM_MOUSEMOVE:
		//	mMouse->ProcessMessage(msg, wParam, lParam);
		//	return 0;

		//case WM_KEYDOWN:
		//case WM_SYSKEYDOWN:
		//case WM_KEYUP:
		//case WM_SYSKEYUP:
		//	mKeyboard->ProcessMessage(msg, wParam, lParam);
		//	return 0;
		}
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	void ID3DApp::Render()
	{
		if (timer_.GetFrameCount() == 0)
		{
			return;
		}

		Clear();
		
		Draw();

		// Show the new frame.
		HRESULT hr;
		if (d3d_options_ & C_ALLOW_TEARING)
		{
			// Recommended to always use tearing if supported when using a sync interval of 0.
			hr = swap_chain_->Present(0, DXGI_PRESENT_ALLOW_TEARING);
		}
		else
		{
			// The first argument instructs DXGI to block until VSync, putting the application
			// to sleep until the next VSync. This ensures we don't waste any cycles rendering
			// frames that will never be displayed to the screen.
			hr = swap_chain_->Present(1, 0);
		}

		if (d3d_depth_stencil_view_)
		{
			// Discard the contents of the depth stencil.
			d3d_context_->DiscardView(d3d_depth_stencil_view_.Get());
		}
		ThrowIfFailed(hr);

		if (!dxgi_factory_->IsCurrent())
		{
			// Output information is cached on the DXGI Factory. If it is stale we need to create a new factory.
			HR(CreateDXGIFactory1(IID_PPV_ARGS(dxgi_factory_.ReleaseAndGetAddressOf())));
		}
	}

	void ID3DApp::Clear()
	{
		// Clear the views.
		d3d_context_->ClearRenderTargetView(d3d_render_target_view_.Get(), Colors::CornflowerBlue);
		d3d_context_->ClearDepthStencilView(d3d_depth_stencil_view_.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		d3d_context_->OMSetRenderTargets(1, d3d_render_target_view_.GetAddressOf(), d3d_depth_stencil_view_.Get());
		// Set the viewport.
		d3d_context_->RSSetViewports(1, &screen_viewport_);
	}
}