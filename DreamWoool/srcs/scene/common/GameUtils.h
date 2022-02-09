#pragma once
#include "dwTypedef.h"
namespace DW
{
	class GameUtils
	{
	public:
		//传入的点是cocos 坐标系
		static Direction getDirectionInCocos(int src_x, int src_y, int dest_x, int dest_y);
	};
}