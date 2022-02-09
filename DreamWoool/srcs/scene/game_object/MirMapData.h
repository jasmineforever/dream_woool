#pragma once

namespace DW
{
	class MirMapData
	{
	public:
		friend class MirMapLoader;
#pragma pack(1) 
		struct ImageInfo
		{
			unsigned short object_image_width;
			unsigned short object_image_height;
			unsigned short object_image_offset_x;
			unsigned short object_image_offset_y;
			ImageInfo() :
				object_image_width(0),
				object_image_height(0),
				object_image_offset_x(0),
				object_image_offset_y(0)
			{}
		};
		struct TileInfo
		{
			bool	can_move;
			int		tiles_image_index;
			int		smtiles_image_index;
			int		object_image_index;
			int		object_file_index;
			ImageInfo image_info;
			TileInfo() :
				can_move(false),
				tiles_image_index(-1),
				smtiles_image_index(-1),
				object_image_index(-1),
				object_file_index(-1)
			{}
		};
#pragma pack() 
		MirMapData();

		~MirMapData();

		int GetMapWidth() const;

		int GetMapHeight() const;

		const std::vector<TileInfo>& GetTilesInfo() const;
	private:
		int	map_width_;
		int map_height_;
		std::vector<TileInfo> tiles_info_;
	};
}