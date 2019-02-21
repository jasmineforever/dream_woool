#include "pch.h"
#include "Sprite.h"
#include "TestNode.h"
#include "SpriteFrameCache.h"
namespace DW
{
	TestNode::TestNode():
		Node("TestNode")
	{

	}
	TestNode::~TestNode()
	{

	}
	std::shared_ptr<TestNode> TestNode::Create()
	{
		auto ret = std::make_shared<TestNode>();
		if (ret->Init())
		{
			return ret;
		}
		return nullptr;
	}

	bool TestNode::Init()
	{
		//SetOpacity(0.5);
		sprite1_ = Sprite::Create("resources/texture.jpg");
		sprite1_->SetScale(0.5);
		AddChild(sprite1_);

		auto d = DrawNode::Create();
		d->DrawQuadSolid(Vector2(0, 0), Size(140, 60), DirectX::Colors::DarkOrange);
		d->SetContentSize(140, 60);
		d->SetAnchorPoint(0.5, 0.5);
		d->SetPosition(sprite1_->GetContentSize().width / 2, sprite1_->GetContentSize().height / 2);
		sprite1_->AddChild(d);

		label1_ = Label::Create("ÎÞÇéµÄÄã");
		sprite1_->AddChild(label1_);
		label1_->SetPosition(sprite1_->GetContentSize().width / 2, sprite1_->GetContentSize().height / 2);
		label1_->SetAnchorPoint(0.5, 0.5);
		label1_->SetRotation(XMConvertToRadians(90));

		sprite2_ = Sprite::Create("resources/texture1.jpg");
		sprite1_->AddChild(sprite2_);

		sprite3_ = Sprite::Create("resources/texture2.jpg");
		sprite2_->AddChild(sprite3_);

		Schedule(std::bind(&TestNode::UpdateTest, this, std::placeholders::_1), 0.1, INT_MAX, 10.0f);
		return true;
	}
	void TestNode::UpdateTest(float dt)
	{
		static bool left = false;
		if (GetPosition().x <= FLT_TRUE_MIN)
		{
			left = false;
		}
		else if (GetPosition().x > 1280.0)
		{
			left = true;
		}

		if (left)
		{
			SetPosition(GetPosition().x - 2, GetPosition().y);
			SetRotation(GetRotation() + XMConvertToRadians(-1));
			sprite2_->SetRotation(sprite2_->GetRotation() + XMConvertToRadians(0.5));
		}
		else
		{
			SetPosition(GetPosition().x + 2, GetPosition().y);
			SetRotation(GetRotation() + XMConvertToRadians(1));
			sprite2_->SetRotation(sprite2_->GetRotation() + XMConvertToRadians(-0.5));
		}
	}
}