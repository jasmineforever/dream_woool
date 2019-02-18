#pragma once
#include <functional>
#include "Render.h"
#include "SimpleMath.h"
#include "Scene.h"
#include "Render.h"
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

		void RunFunctionInMainLoop(std::function<void ()> func);

		void RunScene(std::shared_ptr<Scene> scene);

		std::shared_ptr<Scene> GetRunningScene();

		SimpleMath::Size GetScreenSize();

		void SetScreenSize(const SimpleMath::Size& size);

		void Update(float dt);

		void RenderScene();

		int Schedule(std::function<void(float)> func,
			float interval_in_seconds,
			unsigned int repeat,
			float delay_in_seconds);

		void UnSchedule(int schedule_id);

	private:
		Director();
		Director(const Director&) = delete;
		Director& operator = (const Director&) = delete;
	private:
		Render								render_;
		std::mutex							func_run_mutex_;
		std::vector< std::function<void()>> functions_;
		std::shared_ptr<Scene>				now_scene_;
		SimpleMath::Size					screen_size_;
	};
}