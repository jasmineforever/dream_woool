#pragma once
#include <functional>
#include "Render.h"
#include "SimpleMath.h"
#include "Scene.h"
#include "Render.h"
#include "Scheduler.h"
namespace DW
{
	class Director
	{
	public:
		~Director();

		static Director& GetInstance();

		void InitRender(ID3D11Device1* d3d_device,
			ID3D11DeviceContext1* d3d_context,
			ID2D1RenderTarget* d2d_render_target,
			IDWriteFactory* dwrite_factory,
			ID2D1Factory* d2d_factory);

		Render& GetRender();

		Scheduler& GetScheduler();

		void RunScene(std::shared_ptr<Scene> scene);

		std::shared_ptr<Scene> GetRunningScene();

		SimpleMath::Size GetScreenSize();

		void SetScreenSize(const SimpleMath::Size& size);

		void Update(float dt);

		void RenderScene();

	private:
		Director();
		Director(const Director&) = delete;
		Director& operator = (const Director&) = delete;
	private:
		Scheduler							scheduler_;
		Render								render_;
		std::mutex							func_run_mutex_;
		std::shared_ptr<Scene>				now_scene_;
		SimpleMath::Size					screen_size_;
	};
}