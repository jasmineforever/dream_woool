#pragma once
#include "ID3DApp.h"
#include "graphics/TextRender.h"
#include "Scene.h"

namespace DW
{
	class GameApp : public ID3DApp
	{
	public:
		GameApp(HINSTANCE h_instance,
			size_t width = 1280,
			size_t height = 800,
			const std::string& window_name = "DreamWoool",
			unsigned int flags = C_FLIP_PRESENT);

		~GameApp();

		bool Init();

		void Update(double dt);

		void Draw();
	private:
		bool InitGameScene();

		bool InitEngine();
	private:
		//game Scene
		std::shared_ptr<Scene>		game_scene_;
		std::shared_ptr<Scene>		select_scene_;
		std::shared_ptr<Scene>		login_scene_;
		std::shared_ptr<Scene>		loading_scene_;
	};
}