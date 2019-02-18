#pragma once
//lib dependents
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "winmm.lib")
#include "StepTimer.h"
#include "SimpleMath.h"
namespace DW
{
	static const unsigned int C_FLIP_PRESENT = 0x1;
	static const unsigned int C_ALLOW_TEARING= 0x2;
	static const unsigned int C_ENABLE_4XMSAA = 0x4;
	class ID3DApp
	{
	public:
		ID3DApp(HINSTANCE h_instance, 
			size_t width,
			size_t height, 
			const std::string& window_name,
			unsigned int flags = C_FLIP_PRESENT);

		virtual ~ID3DApp();

		HINSTANCE GetAppInstance() const;

		HWND GetWindowHandle() const;

		DirectX::SimpleMath::Size GetWindowSize();

		float GetAspectRatio() const;

		inline static ID3DApp* GetInstance() { return instance_; }

		int	Run();

		virtual bool Init();

		virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		//virtual void OnResize() = 0;

		virtual void Update(double dt) = 0;

		virtual void Draw() = 0;

	private:
		bool InitEnv();

		bool InitWindow(); 

		bool InitD2D();

		bool InitD3D();

		bool InitWindowSizeDependentResources();

		void GetHardwareAdapter(IDXGIAdapter1** ppAdapter); //

		void UpdateFrameAndShow(); // ����ÿ��֡�����ڴ�����ʾ

		void Render();

		void Clear();
	protected:
		// ʹ��ģ�����(C++11)��������
		template <class T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;
		// Direct2D objects
		ComPtr<ID2D1Factory>				d2d_factory_;							// D2D����
		ComPtr<ID2D1RenderTarget>			d2d_render_target_;						// D2D��ȾĿ��
		ComPtr<IDWriteFactory>				dwrite_factory_;						// DWrite����
		// Direct3D objects.
		ComPtr<IDXGIFactory2>               dxgi_factory_;
		ComPtr<ID3D11Device1>               d3d_device_;
		ComPtr<ID3D11DeviceContext1>        d3d_context_;
		ComPtr<IDXGISwapChain1>             swap_chain_;
		// Direct3D rendering objects. Required for 3D.
		ComPtr<ID3D11Texture2D>				render_target_;
		ComPtr<ID3D11Texture2D>				depth_stencil_;
		ComPtr<ID3D11RenderTargetView>		d3d_render_target_view_;
		ComPtr<ID3D11DepthStencilView>		d3d_depth_stencil_view_;
		D3D11_VIEWPORT                      screen_viewport_;

	private:
		static				ID3DApp* instance_;
		StepTimer			timer_;
		HINSTANCE			h_instance_;
		size_t				window_width_;
		size_t				window_height_;
		std::wstring		window_name_;
		HWND				hwnd_;
		bool				is_paused_;
		unsigned int		d3d_options_;
		D3D_FEATURE_LEVEL	d3d_feature_level_;
	};
}