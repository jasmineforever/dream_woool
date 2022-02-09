#pragma once
#include <vector>
#include <SpriteFrame.h>
namespace DW
{
	struct MapPosition
	{
		int x;
		int y;
		MapPosition(int _x = 0, int _y = 0):
			x(_x),
			y(_y) 
		{}
	};
	enum class Direction
	{
		up = 0,
		up_right,
		right,
		right_down,
		down,
		left_down,
		left,
		left_up,
		size
	};

	enum  class Job
	{
		warrior = 0,
		wizard,
		taos
	};
	
	enum class Gender
	{
		male = 0,
		female
	};

	enum class HumanMotionID
	{
		stand = 0,
		walk,
		run,
		hit,
		be_hit,
		cast,
		death,
		dig,
		ride_stand,
		ride_walk,
		ride_run,
		ride_death,
		ride_be_hit,
		size
	};
	enum class HumanPart
	{
		hair = 0,
		clothes,
		weapon,
		pattern,
		horse,
		shield,
		wing,
		size
	};

	//item
	enum class WeaponID
	{
		no_weapon = 0,
		wumujian,
		xiaoyaowujishan = 42,
		size
	};

	enum class HorseID
	{
		zaohong = 0,
		wuzhui,
		xuelong,
		yingqin,
		huangjin,
		size
	};
	enum class ClothesID
	{
		naked_man = 0,
		naked_woman,
		buyi_man = 2,
		buyi_woman,
		qingkui_man,
		qingkui_woman,
		zhongkui_man,
		zhongkui_woman,
		mopao_man,
		mopao_woman,
		linghun_man,
		linghun_woman,
		zhanshen_man,
		zhanshen_woman,
		emo_man,
		emo_woman,
		youling_man,
		youling_woman,
		tianmo_man,
		shengzhan_woman,
		fashen_man,
		nishang_woman,
		tianzun_man,
		tianshi_woman,
		jinpeng_man,
		jinpeng_woman,
		size
	};

	enum class HairID
	{
		qingliang = 0,
		haofang,
		size
	};

	enum class HairColor
	{
		red = 0xff,

	};

	enum class PatternColor
	{
		red = 0xff,

	};
	enum class MagicID
	{
		normal_hit = 0,
		xiaohuoqiu,
	};
	enum class MouseState
	{
		NORMAL = 0,
		L_DOWN,
		R_DOWN
	};
	enum class EffectID
	{
		xiaohuoqiu1,
		xiaohuoqiu2,
		zhiliaoshu1,
		zhiliaoshu2,
		bingjianshu1,
		bingjianshu2,
		size
	};

	//需要实现的效果类型(需要解决 读取对应效果类型的数据 以及展示)
	//1.人物技能 2.人物箭头 3.套装展示(如幻彩套装， 升级效果等) 4.怪物技能 5.怪物攻击效果 投掷怪物 黄金魔斗士 地火兽弓箭手 投矛手 七情天仙等
	//1.技能
		//第一阶段需要实现的技能如下
		//战士:初级剑法、血影刀法、攻杀剑法、护身真气、刺杀剑法、半月弯刀、野蛮冲撞、雷霆剑、烈火剑法、破击剑法、突斩、破盾斩、金刚护体
		//法师:小火球、抗拒火环、诱惑之光、雷电术、冰箭术、火墙、疾光电影、冰龙破、地狱雷光、魔法盾、圣言术、五雷轰、冰咆哮、冰旋风、兽灵术、风影盾
		//狂龙紫电、化身蝙蝠
		//道士:治疗术、精神战法、施毒术、灵魂道符、骷髅召唤、隐身术、替身法符、集体隐身术、幽灵盾、神圣战甲术、困魔咒、群体治愈术、召唤神兽、神光术
		//解毒术、幽冥火咒、强化骷髅术
		//技能效果分为以下几种：
		//1.无效果(初级剑法、双倍技能、精神战法)
		//2.普攻动作附带的效果(血影刀法、攻杀剑法、刺杀剑法、半月弯刀、雷霆剑、烈火剑法、破击剑法、破盾斩)
		//3.人物自身效果(护身真气、金刚护体、抗拒火环、风影盾、魔法盾、隐身术、地狱雷光)
		//4.释放者可能带有前摇动作(动作效果绑定施法动作),然后对某个方向播放动画(疾光电影、冰龙破)
		//5.释放者可能带有前摇动作(动作效果绑定施法动作)对指定目标或者坐标播放动画，这类动作很多
		//细分的话 可以根据技能是否在空中飞行分为以下两类
		//5.1小火球、灵魂道符、幽冥火咒
		//5.2雷电术、冰箭术、狂龙紫电====
		//6.地面效果(火墙、困魔咒、)
	//2，3.箭头以及套装展示或者升级动画这种序列帧动画 只需控制帧率 延时 是否持续 坐标即可
	//4，5.投掷类 相当于飞行，有的也可能有击中目标的效果 BOSS效果暂不考虑 到时候单独做

	enum class EffectType
	{
		target,//效果绑定目标或者如果没有坐标就绑定xy
		hit, //效果绑定普攻
		cast,//效果绑定前摇
		cast_dir,//从目标出发指定方向
		fly_to_target,//效果飞行后绑定目标(16方向)
		ground//地面效果

	};
	using FlyToEffectData = std::vector<std::vector<std::shared_ptr<SpriteFrame>>>;
	struct EffectData
	{
		FlyToEffectData				fly_effect;//type == fly_to_target 时 用来表示飞行中的动画
		std::vector<std::shared_ptr<SpriteFrame>>	frames;
	};
}