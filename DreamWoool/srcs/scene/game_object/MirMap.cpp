#include "pch.h"
#include "MirMap.h"
#include "TextureCache.h"
namespace DW
{
	MirMap::MirMap():
		Node("MirMap")
	{

	}
	MirMap::~MirMap()
	{

	}

	std::shared_ptr<MirMap> MirMap::Create(const MirMapData& mapData, const::std::string& resource_path)
	{
		auto p = std::make_shared<MirMap>();
		if (p)
		{
			if(!p->Init(mapData, resource_path))
			{
				return nullptr;
			}
		}
		return p;
	}

	bool MirMap::Init(const MirMapData& mapData, const::std::string& resource_path)
	{
		bool ret = false;

		char file_path[256];

		auto& tiles_info = mapData.GetTilesInfo();
		for (int i = 0; i < tiles_info.size(); i++)
		{
			if (tiles_info[i].object_file_index >= 0 && tiles_info[i].object_file_index >= 0)
			{
				sprintf_s(file_path, "%s/Objects%d/%05d.BMP", resource_path.c_str(), tiles_info[i].object_file_index + 1, tiles_info[i].object_image_index);
				if (!Utils::IsFileExist(file_path))
				{
					LOG_WARNING("in map create, %s not found.", file_path);
					auto sp = Sprite::Create();
					sp->SetAnchorPoint(0., 0.);
					nodes_.push_back(sp);
					continue;
				}
				auto texture = TextureCache::GetInstance().LoadFromFile(file_path, false);

				if (!texture)
				{
					LOG_WARNING("in map create, can't create texture:%s", file_path);
					auto sp = Sprite::Create();
					sp->SetAnchorPoint(0., 0.);
					nodes_.push_back(sp);
					continue;
				}
				auto sp = Sprite::Create(texture);
				sp->SetAnchorPoint(0., 0.);
				if (!sp)
				{
					LOG_FATAL("in map create, can't create sprite:%s", file_path);
					break;
				}
				nodes_.push_back(sp);
			}
		}
		ret = true;
		//test
		//for (int i = 0; i < mapData.GetMapWidth(); i++)
		//{
		//	for (int j = 0; j < mapData.GetMapHeight(); j++)
		//	{
		//		int index = i * mapData.GetMapHeight() + j;
		//		nodes_[index]->SetPosition(48 * i + tiles_info[index].image_info.object_image_offset_x,
		//								32 * j + tiles_info[index].image_info.object_image_offset_y);
		//		AddChild(nodes_[index]);
		//	}
		//}
		//30*27 ∏ˆœ‘ æ
		for (int i = 0; i < 30; i++)
		{
			for (int j = 0; j < 27; j++)
			{
				auto index = i * mapData.GetMapHeight() + j;
				nodes_[index]->SetPosition(48 * i + tiles_info[index].image_info.object_image_offset_x,
					32 * j + tiles_info[index].image_info.object_image_offset_y);
				AddChild(nodes_[index]);
			}
		}
		return ret;
	}
}