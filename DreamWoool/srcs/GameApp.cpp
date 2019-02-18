#include "pch.h"
#include "GameApp.h"
#include "GameScene.h"
#include "Render.h"
#include "TextureCache.h"
#include "Director.h"
using namespace DirectX;
namespace DW
{
	GameApp::GameApp(HINSTANCE h_instance, 
		size_t width,
		size_t height, 
		const std::string& window_name,
		unsigned int flags):
		ID3DApp(h_instance, width, height, window_name, flags)
	{

	}
	GameApp::~GameApp()
	{

	}
	bool GameApp::InitGameScene()
	{
		game_scene_ = GameScene::Create();
		if (game_scene_)
		{
			return true;
		}
		return false;
	}
	bool GameApp::InitEngine()
	{
		auto& director = Director::GetInstance();
		director.SetScreenSize(GetWindowSize());
		director.InitRender(d3d_device_.Get(),
			d3d_context_.Get(),
			d2d_render_target_.Get(),
			dwrite_factory_.Get(),
			d2d_factory_.Get());
		return true;
	}
	bool GameApp::Init()
	{
		if (!ID3DApp::Init())
		{
			return false;
		}
		if (!InitEngine())
		{
			return false;
		}
		if (!InitGameScene())
		{
			LOG_FATAL("init game scene failed!");
			return false;
		}
		Director::GetInstance().RunScene(game_scene_);
		return true;
	}
	void GameApp::Update(double dt)
	{
		Director::GetInstance().Update(dt);
	}
	void GameApp::Draw()
	{
		Director::GetInstance().RenderScene();
	}
	
}