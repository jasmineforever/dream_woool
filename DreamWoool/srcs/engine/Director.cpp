#include "pch.h"
#include "Director.h"

namespace DW
{
	Director& Director::GetInstance()
	{
		static Director d;
		return d;
	}

	Director::Director()
	{

	}

	Director::~Director()
	{

	}

	void Director::InitRender(ID3D11Device1* d3d_device,
		ID3D11DeviceContext1* d3d_context,
		ID2D1RenderTarget* d2d_render_target,
		IDWriteFactory* dwrite_factory,
		ID2D1Factory* d2d_factory)
	{
		render_.Init(d3d_device, d3d_context, d2d_render_target, dwrite_factory, d2d_factory);
	}

	Scheduler& Director::GetScheduler()
	{
		return scheduler_;
	}

	Render& Director::GetRender()
	{
		return render_;
	}

	SimpleMath::Size Director::GetScreenSize()
	{
		return screen_size_;
	}

	void Director::SetScreenSize(const SimpleMath::Size& size)
	{
		screen_size_ = size;
	}

	void Director::RunScene(std::shared_ptr<Scene> scene)
	{
		if (now_scene_ != scene)
		{
			if (now_scene_)
			{
				now_scene_.reset();
			}
			now_scene_ = scene;
		}
	}

	std::shared_ptr<Scene> Director::GetRunningScene()
	{
		return now_scene_;
	}

	void Director::RenderScene()
	{
		if (now_scene_)
		{
			render_.RenderScene(now_scene_.get());
		}
	}

	void Director::Update(float dt)
	{
		scheduler_.Update(dt);

		if (now_scene_)
		{
			now_scene_->UpdateScene(dt);
		}
	}
}