#include "pch.h"
#include "Sprite.h"
#include "TestSpriteNodeFrame.h"
#include "SpriteFrameCache.h"
#include "SpriteFrameAnimation.h"
#include "Label.h"
namespace DW
{
	TestSpriteNodeFrame::TestSpriteNodeFrame():
		Node("TestSpriteNodeFrame")
	{

	}
	TestSpriteNodeFrame::~TestSpriteNodeFrame()
	{

	}
	std::shared_ptr<TestSpriteNodeFrame> TestSpriteNodeFrame::Create()
	{
		auto ret = std::make_shared<TestSpriteNodeFrame>();
		if (ret->Init())
		{
			return ret;
		}
		return nullptr;
	}

	bool TestSpriteNodeFrame::Init()
	{
		auto& cache = SpriteFrameCache::GetInstance();
		cache.LoadFromFile("resources/body_22.txt", "resources/body_22.png");

		char key_str[128];
		for (int i = 1; i <= 4; i++)
		{
			sprintf_s(key_str, "1_22_00_0_%02d", i);

			auto frame = cache.GetSpriteFrame(key_str);
			if (frame)
			{
				sps_.push_back(frame);
			}
		}
		if (sps_.size() == 4)
		{
			sps_.push_back(sps_[2]);
			sps_.push_back(sps_[1]);
		}
		sprite_ = Sprite::Create();
		sprite_->SetAnchorPoint(0, 0);
		AddChild(sprite_);
		
		auto label = Label::Create("ÏãÏãµÄÎÒ", 20, DWColor::Yellow, "resources/fonts/lantinghei.ttf");
		label->SetUnderline(true);
		label->SetBorder(true);
		AddChild(label);

		auto animation = SpriteFrameAnimation::Create(sps_, 1.5 / 6.0, true);
		sprite_->RunSpriteFrameAnimation(animation);
		return true;
	}
}