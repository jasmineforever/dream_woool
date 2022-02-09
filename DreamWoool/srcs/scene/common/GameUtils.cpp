#include "pch.h"
#include "GameUtils.h"
#include <cmath>
#define PI 3.14159265
double angle_to_radian(double degree, double min, double second)
{
	double flag = (degree < 0) ? -1.0 : 1.0;			//判断正负
	if (degree < 0)
	{
		degree = degree * (-1.0);
	}
	double angle = degree + min / 60 + second / 3600;
	double result = flag * (angle * PI) / 180;
	return result;
	//cout<<result<<endl;
}
//弧度转换为角度
void radian_to_angle(double rad, double ang[])
{
	double flag = (rad < 0) ? -1.0 : 1.0;
	if (rad < 0)
	{
		rad = rad * (-1.0);
	}
	double result = (rad * 180) / PI;
	double degree = int(result);
	double min = (result - degree) * 60;
	double second = (min - int(min)) * 60;
	ang[0] = flag * degree;
	ang[1] = int(min);
	ang[2] = second;
}

namespace DW
{
	Direction GameUtils::getDirectionInCocos(int src_x, int src_y, int dest_x, int dest_y)
	{
		Direction ret = Direction::right_down;
		auto radian = atan2(dest_y - src_y, dest_x - src_x);
		int angle = radian * (180.0 / PI);
		LOG_INFO("MOUSE DIR:%d", angle);
		if (angle > -22 && angle <= 22)
		{
			ret = Direction::right;
		}
		else if (angle > 22 && angle <= 65)
		{
			ret = Direction::up_right;
		}
		else if (angle > 65 && angle <= 113)
		{
			ret = Direction::up;
		}
		else if (angle > 113 && angle <= 158)
		{
			ret = Direction::left_up;
		}
		else if ((angle > 158 && angle <= 180) || (angle >= -180 && angle <= -158))
		{
			ret = Direction::left;
		}
		else if (angle > -158 && angle <= -113)
		{
			ret = Direction::left_down;
		}
		else if (angle > -113 && angle <= -65)
		{
			ret = Direction::down;
		}
		else
		{
			ret = Direction::right_down;
		}
		return ret;
	}
}