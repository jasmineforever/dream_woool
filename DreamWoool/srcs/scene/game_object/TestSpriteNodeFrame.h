#pragma once
#include "Node.h"
#include "Sprite.h"
namespace DW
{
	class TestSpriteNodeFrame : public Node
	{
	public:
		TestSpriteNodeFrame();

		~TestSpriteNodeFrame();

		static std::shared_ptr<TestSpriteNodeFrame> Create();
	private:
		bool Init();
	private:
		std::shared_ptr<Sprite> sprite_;
		std::vector<std::shared_ptr<SpriteFrame>> sps_;
	};
}