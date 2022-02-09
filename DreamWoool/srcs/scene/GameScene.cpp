#include <pch.h>
#include "GameScene.h"
#include "Label.h"
#include "DrawNode.h"
#include "game_object/TestNode.h"
#include "game_object/TestSpriteNodeFrame.h"
#include "MirMapLoader.h"
#include "MirMap.h"
#include "BloodBar.h"
#include "ResourcesPool.h"
#include "Director.h"
#include "TestMap.h"

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
		/*auto& map_loader = MirMapLoader::GetInstance();
		map_loader.LoadImageInfo(
			"G:/动态中地图+真彩地图+碧水山庄地图+锁妖塔地图素材/碧水山庄/碧水山庄/Objects15.wil",
			"G:/动态中地图+真彩地图+碧水山庄地图+锁妖塔地图素材/碧水山庄/碧水山庄/Objects15.wix");

		map_loader.LoadImageInfo(
			"G:/动态中地图+真彩地图+碧水山庄地图+锁妖塔地图素材/碧水山庄/碧水山庄/Objects42.wil",
			"G:/动态中地图+真彩地图+碧水山庄地图+锁妖塔地图素材/碧水山庄/碧水山庄/Objects42.wix");

		MirMapData map_data;
		map_loader.LoadMirMapFromFile("G:/动态中地图+真彩地图+碧水山庄地图+锁妖塔地图素材/碧水山庄/碧水山庄/BHSZ.map", map_data);
		
		auto map = MirMap::Create(map_data, "G:/动态中地图+真彩地图+碧水山庄地图+锁妖塔地图素材/碧水山庄/碧水山庄/1");

		AddChild(map);*/

		auto bg = TestMap::Create("resources/test_map.jpg");
		AddChild(bg);
		std::vector<std::string> human_res = { "resources/human/body_22.txt", 
			"resources/human/pattern_22.txt", 
			"resources/human/weapon_42.txt",
			"resources/human/hair_1.txt", 
			"resources/human/horse_03.txt",
			"resources/human/effect.txt" };
		ResourcesPool::GetInstance().AsyncLoadSpriteSheet(human_res, [&](bool result) {
			
			Director::GetInstance().GetScheduler().RunFunctionInMainLoop([&]() {
				
				hero_ = Actor::Create(Gender::female, HairID::haofang, HairColor::red, WeaponID::xiaoyaowujishan, ClothesID::tianzun_man, PatternColor::red);
				hero_->SetPosition(545, 800 - 545);
				AddChild(hero_);
			});
		});
		Schedule([this](float dt) 
		{
			hero_->Run(Direction::left_up);
		}, 0.736, 1000, 1.);
		return true;
	}
	GameScene::~GameScene()
	{

	}
}