#pragma once
#include "Scene.h"
#include "Sprite.h"
#include "Actor.h"
namespace DW
{
	class GameScene : public Scene
	{
	public:
		static std::shared_ptr<GameScene> Create();
		GameScene();
		~GameScene();
	private:
		bool Init();
		GameScene(GameScene&) = delete;
		GameScene& operator=(const GameScene&) = delete;
	private:
		std::shared_ptr<Actor> hero_;
	};
}