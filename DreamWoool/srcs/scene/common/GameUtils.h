#pragma once
#include "dwTypedef.h"
namespace DW
{
	class GameUtils
	{
	public:
		//����ĵ���cocos ����ϵ
		static Direction getDirectionInCocos(int src_x, int src_y, int dest_x, int dest_y);
	};
}