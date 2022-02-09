//
// Created by xiaofeng on 2018/6/25.
//
#include "pch.h"
#include "ResourcesPool.h"
#include "SpriteFrameCache.h"
#include <algorithm>
namespace DW
{
	#define CHECK_SP_IF_NULLPTR_RETURN_FALSE(sp) if(!sp) return false
	//TODO 动作index 改了 记得改这里
	const HumanMotionInfo ResourcesPool::s_motion_info[static_cast<size_t>(HumanMotionID::size)] = {
		{7, 2.0, 0.0, 2.0 / 7.0},	//stand
		{18, 0.8, 0.0, 0.8 / 18.0},		//walk
		{16, 0.72, 0.0, 0.8 / 16.0},			//run
		{6, 1.0, 0.0, 1.0 / 5.0},//hit
		{3, 1.0, 0.0, 1.0 / 2.0},//be hit
		{8, 1.0, 0.0, 0.7 / 8.0},//cast
		{4, 1.0, 999.0, 0.2 / 3.0},//death
		{3, 1.0, 0.0, 1.0 / 2.0},//dig
		{4, 1.0, 0.0, 1.0 / 6.0},//ride stand
		{10, 1.0, 0.0, 1.0 / 9.0},//ride_walk
		{10, 1.0, 0.0, 1.0 / 9.0},//ride_run
		{8, 1.0, 999.0, 1.0 / 7.0},//ride_death
		{3, 1.0, 0.0, 1.0 / 2.0}//ride_be_hit
	};

	const EffectInfo ResourcesPool::s_effect_info[static_cast<size_t>(EffectID::size)] = {
		{EffectType::cast, 10, 0.5, nullptr, 0.0},//xiaohuoqiu1
		{EffectType::fly_to_target, 20, 0.5, new FlyToEffectInfo{16, 6, 0.2, 100.0}, 0.0},//xiaohuoqiu2,
		{EffectType::cast, 10, 1, nullptr, 0.0},//zhiliaoshu1
		{EffectType::target, 20, 1.0, nullptr, 0.0},//zhiliaoshu2
		{EffectType::cast, 10, 0.5, nullptr, 0.0},//bingjianshu1
		{EffectType::target, 11, 1.0, nullptr, 0.0}//bingjianshu2
	};
    ResourcesPool::ResourcesPool()
    {
       
    }
    ResourcesPool::~ResourcesPool()
    {

    }

	/*void ResourcesPool::onTextureFileLoaded(Texture2D* texture)
	{
		if (texture)
		{
			if (now_load_index_ < p_lists_.size())
			{
				SpriteFrameCache::getInstance()->addSpriteFramesWithFile(p_lists_[now_load_index_].append(".plist"), texture);
			}
			if (now_load_index_ == p_lists_.size() - 1)
			{
				if(load_callback_)
					load_callback_(true);
			}
			else
			{
				now_load_index_ += 1;
				Director::getInstance()->getTextureCache()->addImageAsync(p_lists_[now_load_index_] + ".png",
					CC_CALLBACK_1(ResourcesPool::onTextureFileLoaded, this));
			}
		}
		else
		{
			if (load_callback_)
			{
				load_callback_(false);
			}
		}
	}*/
	bool ResourcesPool::GetHumanClothesSpriteFramesByID(ClothesID id, MotionSpriteFrames& out)
	{
		return GetHumanSpriteFramesByID(HumanPart::clothes, static_cast<int>(id), out);
	}
	bool ResourcesPool::GetHumanPatternSpriteFramesByID(ClothesID id, MotionSpriteFrames& out)
	{
		return GetHumanSpriteFramesByID(HumanPart::pattern, static_cast<int>(id), out);
	}
	bool ResourcesPool::GetHumanHorseSpriteFramesByID(HorseID id, MotionSpriteFrames& out)
	{
		return GetHumanSpriteFramesByID(HumanPart::horse, static_cast<int>(id), out);
	}
	bool ResourcesPool::GetHumanHairSpriteFramesByID(HairID id, MotionSpriteFrames& out)
	{
		return GetHumanSpriteFramesByID(HumanPart::hair, static_cast<int>(id), out);
	}
	bool ResourcesPool::GetHumanWeaponSpriteFramesByID(WeaponID id, MotionSpriteFrames& out)
	{
		return GetHumanSpriteFramesByID(HumanPart::weapon, static_cast<int>(id), out);
	}
	bool ResourcesPool::GetEffectByID(EffectID id, EffectData& out)
	{
		char file_name[256];
		out.frames.clear();
		size_t iid = static_cast<size_t>(id);
		if (EffectType::cast == s_effect_info[iid].type)
		{
			for (int i = 1; i <= s_effect_info[iid].max_frame; i++)
			{
				sprintf_s(file_name, "effect_%03d_%03d", id, i);
				auto sp = LoadAndInitSpriteFrame(file_name);
				CHECK_SP_IF_NULLPTR_RETURN_FALSE(sp);
				out.frames.push_back(sp);
			}
		}
		else if (EffectType::fly_to_target == s_effect_info[iid].type)
		{
			s_effect_info[iid].fly_to_info->direction_num;
			size_t r_index = 1;
			out.fly_effect.resize(s_effect_info[iid].fly_to_info->direction_num);
			for (int i = 0; i < s_effect_info[iid].fly_to_info->direction_num; i++)
			{
				for (int j = 0; j < s_effect_info[iid].fly_to_info->one_dir_frames; j++)
				{
					sprintf_s(file_name, "effect_%03d_%03d", id, r_index);
					auto sp = LoadAndInitSpriteFrame(file_name);
					CHECK_SP_IF_NULLPTR_RETURN_FALSE(sp);
					out.fly_effect[i].push_back(sp);
					r_index += 1;
				}
			}
			r_index -= 1;
			for (int i = 1; i <= s_effect_info[iid].max_frame; i++)
			{
				sprintf_s(file_name, "effect_%03d_%03d", id, r_index + i);
				auto sp = LoadAndInitSpriteFrame(file_name);
				CHECK_SP_IF_NULLPTR_RETURN_FALSE(sp);
				out.frames.push_back(sp);
			}
		}
		else if (EffectType::target == s_effect_info[iid].type)
		{
			for (int i = 1; i <= s_effect_info[iid].max_frame; i++)
			{
				sprintf_s(file_name, "effect_%03d_%03d", id, i);
				auto sp = LoadAndInitSpriteFrame(file_name);
				CHECK_SP_IF_NULLPTR_RETURN_FALSE(sp);
				out.frames.push_back(sp);
			}
		}
		return true;
	}
	bool ResourcesPool::GetHumanSpriteFramesByID(HumanPart part, int main_id, MotionSpriteFrames& out)
	{
		out.clear();
		char file_name[256];
		out.resize(static_cast<size_t>(HumanMotionID::size));
		for (int i = 0; i < static_cast<size_t>(HumanMotionID::size); i++)
		{
			//衣服，头发，颜色匹配，翅膀，马， 武器，盾牌
			int max_frames = s_motion_info[i].max_frame;
			auto now_motion = static_cast<HumanMotionID>(i);
			//武器和盾牌在骑马的动作里，不需要显示
			if(((now_motion >= HumanMotionID::dig) || (now_motion >= HumanMotionID::ride_stand &&
				now_motion <= HumanMotionID::ride_be_hit)) &&
				(part == HumanPart::weapon || part == HumanPart::shield))
			{
				continue;
			}
			//没马的动作不要load
			else if ((now_motion < HumanMotionID::ride_stand || now_motion > HumanMotionID::ride_be_hit) && part == HumanPart::horse)
			{
				continue;
			}
			out[i].resize(static_cast<size_t>(Direction::size));

			for (int j = 0; j < static_cast<size_t>(Direction::size); j++)
			{
				if (HumanMotionID::stand == now_motion)
				{
					for (int k = 1; k <= 4; k++)
					{
						sprintf_s(file_name, "%d_%02d_%02d_%d_%02d", part, main_id, i, j, k);
						auto sp = LoadAndInitSpriteFrame(file_name);
						CHECK_SP_IF_NULLPTR_RETURN_FALSE(sp);
						out[i][j].push_back(sp);
					}
					for (int k = 3; k >= 1; k--)
					{
						sprintf_s(file_name, "%d_%02d_%02d_%d_%02d", part, main_id, i, j, k);
						auto sp = LoadAndInitSpriteFrame(file_name);
						CHECK_SP_IF_NULLPTR_RETURN_FALSE(sp);
						out[i][j].push_back(sp);
					}
				}
				else if (HumanMotionID::walk == now_motion )//|| HumanMotionID::run == now_motion)
				{
					for (int k = 1; k <= 16; k++)
					{
						sprintf_s(file_name, "%d_%02d_%02d_%d_%02d", part, main_id, i, j, k);
						auto sp = LoadAndInitSpriteFrame(file_name);
						CHECK_SP_IF_NULLPTR_RETURN_FALSE(sp);
						out[i][j].push_back(sp);
					}
					for (int k = 15; k >= 14; k--)
					{
						sprintf_s(file_name, "%d_%02d_%02d_%d_%02d", part, main_id, i, j, k);
						auto sp = LoadAndInitSpriteFrame(file_name);
						CHECK_SP_IF_NULLPTR_RETURN_FALSE(sp);
						out[i][j].push_back(sp);
					}
				}
				else if (HumanMotionID::ride_walk == now_motion || HumanMotionID::ride_run == now_motion)
				{
					for (int k = 1; k <= 8; k++)
					{
						sprintf_s(file_name, "%d_%02d_%02d_%d_%02d", part, main_id, i, j, k);
						auto sp = LoadAndInitSpriteFrame(file_name);
						CHECK_SP_IF_NULLPTR_RETURN_FALSE(sp);
						out[i][j].push_back(sp);
					}
					for (int k = 7; k >= 6; k--)
					{
						sprintf_s(file_name, "%d_%02d_%02d_%d_%02d", part, main_id, i, j, k);
						auto sp = LoadAndInitSpriteFrame(file_name);
						CHECK_SP_IF_NULLPTR_RETURN_FALSE(sp);
						out[i][j].push_back(sp);
					}
				}
				else
				{
					for (int k = 1; k <= max_frames; k++)
					{
						sprintf_s(file_name, "%d_%02d_%02d_%d_%02d", part, main_id, i, j, k);
						auto sp = LoadAndInitSpriteFrame(file_name);
						CHECK_SP_IF_NULLPTR_RETURN_FALSE(sp);
						out[i][j].push_back(sp);
					}
				}
			}
		}
		return true;
	}
	std::shared_ptr<SpriteFrame> ResourcesPool::LoadAndInitSpriteFrame(const std::string& key)
	{
		auto sp = SpriteFrameCache::GetInstance().GetSpriteFrame(key);
		return sp;
	}
	void ResourcesPool::AsyncLoadSpriteSheet(const std::vector<std::string>& sprite_sheet_files, ResourceLoadedCallback callback)
	{
		for (int i = 0; i < sprite_sheet_files.size(); ++i)
		{
			auto image_name = Utils::GetParentPath(sprite_sheet_files[i]) + "/" +
				Utils::GetFileNameInPath(sprite_sheet_files[i]) + ".png";
			SpriteFrameCache::GetInstance().LoadFromFile(sprite_sheet_files[i], image_name);
		}
		if (callback)
		{
			callback(true);
		}
	}
}