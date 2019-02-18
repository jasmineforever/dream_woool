#include <pch.h>
#include "GameScene.h"
#include "Label.h"
#include "DrawNode.h"
#include "game_object/TestNode.h"

namespace DW
{
	std::shared_ptr<GameScene> GameScene::Create()
	{
		auto ptr = std::shared_ptr<GameScene>(new GameScene());
		if (ptr)
		{
			if (ptr->Init())
			{
				return ptr;
			}
		}
		return nullptr;
	}
	GameScene::GameScene():
		Scene("game scene")
	{

	}
	bool GameScene::Init()
	{
		auto bg = Sprite::Create("resources/xinshou.jpg");
		AddChild(bg);
		auto node = TestNode::Create();
		node->SetPosition(1280 / 2, 800 / 2);
		AddChild(node);
		return true;
	}
	GameScene::~GameScene()
	{

	}
}