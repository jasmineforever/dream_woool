#pragma once
#include "Node.h"
#include "Sprite.h"
#include "MirMapData.h"
namespace DW
{
	class MirMap : public Node
	{
	public:
		MirMap();

		~MirMap();

		static std::shared_ptr<MirMap> Create(const MirMapData& mapData, const::std::string& resource_path);
		
	private:
		bool Init(const MirMapData& mapData, const::std::string& resource_path);
	private:
		std::vector<std::shared_ptr<Sprite>> nodes_;
	};
}