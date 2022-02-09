#include "pch.h"
#include "TestMap.h"
#include "TextureCache.h"
namespace DW
{
	TestMap::TestMap()
	{

	}
	TestMap::~TestMap()
	{

	}
	std::shared_ptr<TestMap> TestMap::Create(const std::string& map_image_path)
	{
		auto ret = std::make_shared<TestMap>();
		if (ret->Init(map_image_path))
		{
			return ret;
		}
		return nullptr;
	}

	bool TestMap::Init(const std::string& map_image_path)
	{
		sp_ = Sprite::Create(map_image_path);
		sp_->SetAnchorPoint(0, 0);
		auto x = -sp_->GetContentSize().width / 2;
		auto y = -sp_->GetContentSize().height / 2;
		sp_->SetPosition(x, y);
		AddChild(sp_);
		return true;
	}
	void TestMap::Update(float dt)
	{
		//Sprite::Update(dt);
		auto pos = sp_->GetPosition();
		sp_->SetPosition(pos.x + 2, pos.y + 1);
	}
}