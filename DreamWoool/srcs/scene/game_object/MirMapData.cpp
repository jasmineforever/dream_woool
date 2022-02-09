#include "pch.h"
#include "MirMapData.h"
namespace DW
{
	MirMapData::MirMapData()
	{

	}
	MirMapData::~MirMapData()
	{

	}

	int MirMapData::GetMapWidth() const
	{
		return map_width_;
	}

	int MirMapData::GetMapHeight() const
	{
		return map_height_;
	}

	const std::vector<MirMapData::TileInfo>& MirMapData::GetTilesInfo() const
	{
		return tiles_info_;
	}
}