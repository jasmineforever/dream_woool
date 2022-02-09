#pragma once
#include "Sprite.h"
#include "DrawNode.h"
namespace DW
{
	class TestMap : public Node
	{
	public:
		TestMap();

		~TestMap();

		static std::shared_ptr<TestMap> Create(const std::string& map_image_path);

		void Update(float dt);
	private:
		bool Init(const std::string& map_image_path);
	private:
		std::shared_ptr<Sprite> sp_;
	};
}