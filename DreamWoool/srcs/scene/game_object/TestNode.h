#pragma once
#include "Node.h"
#include "Label.h"
#include "DrawNode.h"
namespace DW
{
	class TestNode : public Node
	{
	public:
		TestNode();
		~TestNode();
		static std::shared_ptr<TestNode> Create();

		void Update(float dt);
	private:
		bool Init();
	private:
		std::shared_ptr<Sprite> sprite1_;
		std::shared_ptr<Sprite> sprite2_;
		std::shared_ptr<Sprite> sprite3_;
		std::shared_ptr<Label>	label1_;
	};
}