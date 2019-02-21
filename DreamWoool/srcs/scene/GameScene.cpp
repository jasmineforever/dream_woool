#include <pch.h>
#include "GameScene.h"
#include "Label.h"
#include "DrawNode.h"
#include "game_object/TestNode.h"
#include "game_object/TestSpriteNodeFrame.h"
#include "TTFInfo.h"
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
		auto bg = Sprite::Create("resources/test_map.bmp");
		AddChild(bg);
		auto node = TestNode::Create();
		node->SetPosition(1280 / 2, 800 / 2);
		AddChild(node);

		auto sp = TestSpriteNodeFrame::Create();
		sp->SetPosition(545, 800 - 545);
		AddChild(sp);
		return true;
	}
	GameScene::~GameScene()
	{

	}
}