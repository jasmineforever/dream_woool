#pragma once

namespace DW
{
	class MirMapData
	{
	public:
#pragma pack(1) 
		struct MirMapHeader
		{
			short	map_width;				//地图宽
			short	map_height;				//地图高
			char	title[16];				//标题
			char	last_update_time[8];	//最后更新时间
			char	map_type;				//新地图为2对应MirMapTile为14字节，其他对应的MirMapTile为12字节
			char reserved[23];				//保留字节
		};
		struct MirMapTile
		{
			short big_tile_image;				//地表索引, 最高位表示是否可以通过
			short small_title_image;			//表示地图可视物体图片(有时被称为可视数据/中间层/小地图块/地图补充背景等等)
			short object_tile_image;			//地表上层索引, 最高位表示是否可以通过
			unsigned char door_index;			//门索引，最高位为1表示有门，为0表示没有门。
			unsigned char door_offset;			//门偏移，最高位为1表示门打开了，为0表示门为关闭状态
			unsigned char animate_frame_num;	//地图块帧数
			unsigned char animate_tick;			// 跳帧数，指示当前地图块动态内容应该每隔多少帧变换当前显示的静态图片
			//index = (gAniCount % (btAniFrame * (1 + btAniTick))) / (1 + btAniTick)
			unsigned char object_file_index;		//资源文件索引
			unsigned char  light;					//一般为0/4/5
			unsigned char big_tile_file_index;		// tiles文件 索引
			unsigned char small_title__file_index;	//smtiles 索引
		};
#pragma pack()
		MirMapData();
		~MirMapData();
	};
}