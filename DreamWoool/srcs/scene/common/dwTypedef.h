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

	//��Ҫʵ�ֵ�Ч������(��Ҫ��� ��ȡ��ӦЧ�����͵����� �Լ�չʾ)
	//1.���＼�� 2.�����ͷ 3.��װչʾ(��ò���װ�� ����Ч����) 4.���＼�� 5.���﹥��Ч�� Ͷ������ �ƽ�ħ��ʿ �ػ��޹����� Ͷì�� �������ɵ�
	//1.����
		//��һ�׶���Ҫʵ�ֵļ�������
		//սʿ:����������ѪӰ��������ɱ������������������ɱ�����������䵶��Ұ����ײ�����������һ𽣷����ƻ�������ͻն���ƶ�ն����ջ���
		//��ʦ:С���򡢿��ܻ𻷡��ջ�֮�⡢�׵���������������ǽ�������Ӱ�������ơ������׹⡢ħ���ܡ�ʥ���������׺䡢�������������硢����������Ӱ��
		//�����ϵ硢��������
		//��ʿ:������������ս����ʩ�������������������ٻ�����������������������������������ܡ���ʥս��������ħ�䡢Ⱥ�����������ٻ����ޡ������
		//�ⶾ������ڤ���䡢ǿ��������
		//����Ч����Ϊ���¼��֣�
		//1.��Ч��(����������˫�����ܡ�����ս��)
		//2.�չ�����������Ч��(ѪӰ��������ɱ��������ɱ�����������䵶�����������һ𽣷����ƻ��������ƶ�ն)
		//3.��������Ч��(������������ջ��塢���ܻ𻷡���Ӱ�ܡ�ħ���ܡ��������������׹�)
		//4.�ͷ��߿��ܴ���ǰҡ����(����Ч����ʩ������),Ȼ���ĳ�����򲥷Ŷ���(�����Ӱ��������)
		//5.�ͷ��߿��ܴ���ǰҡ����(����Ч����ʩ������)��ָ��Ŀ��������겥�Ŷ��������ද���ܶ�
		//ϸ�ֵĻ� ���Ը��ݼ����Ƿ��ڿ��з��з�Ϊ��������
		//5.1С��������������ڤ����
		//5.2�׵������������������ϵ�====
		//6.����Ч��(��ǽ����ħ�䡢)
	//2��3.��ͷ�Լ���װչʾ��������������������֡���� ֻ�����֡�� ��ʱ �Ƿ���� ���꼴��
	//4��5.Ͷ���� �൱�ڷ��У��е�Ҳ�����л���Ŀ���Ч�� BOSSЧ���ݲ����� ��ʱ�򵥶���

	enum class EffectType
	{
		target,//Ч����Ŀ��������û������Ͱ�xy
		hit, //Ч�����չ�
		cast,//Ч����ǰҡ
		cast_dir,//��Ŀ�����ָ������
		fly_to_target,//Ч�����к��Ŀ��(16����)
		ground//����Ч��

	};
	using FlyToEffectData = std::vector<std::vector<std::shared_ptr<SpriteFrame>>>;
	struct EffectData
	{
		FlyToEffectData				fly_effect;//type == fly_to_target ʱ ������ʾ�����еĶ���
		std::vector<std::shared_ptr<SpriteFrame>>	frames;
	};
}