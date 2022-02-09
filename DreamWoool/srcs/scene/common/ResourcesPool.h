#pragma once
#include <functional>
#include <vector>
#include <string>
#include "dwTypedef.h"
#include "Singleton.h"
#include "Texture.h"
#include "TextureCache.h"
namespace DW
{
	using oneDirectionSpriteFrames = std::vector <std::shared_ptr<SpriteFrame>>; //size = 每个动作的帧数
	using AllDirectionSpriteFrames = std::vector <oneDirectionSpriteFrames>;//size = Direction::size
	using MotionSpriteFrames = std::vector<AllDirectionSpriteFrames>;//size = MotionID::size
	struct HumanMotionInfo {
		int max_frame;
		float animate_time;
		float rest_time;
		float frame_time;
	};

	struct FlyToEffectInfo {
		int	direction_num;		//方向个数 一般是16个方向
		int one_dir_frames;		//飞行中动画帧数
		float animate_time;		//完成一次飞行动画需要的时间
		float speed;			//飞行速度
	};

	struct EffectInfo {
		EffectType type;
		int		max_frame;
		float	animate_time;		//动画的播放时间
		FlyToEffectInfo* fly_to_info;
		float	loop_delay_time;	//如果是连续播放的话 中间的延时时间
	};
    class ResourcesPool : public Singleton<ResourcesPool>
    {
    public:
		friend Singleton<ResourcesPool>;

        using ResourceLoadedCallback = std::function<void(bool result)>;

        ~ResourcesPool();

        void AsyncLoadSpriteSheet(const std::vector<std::string>& sprite_sheet_files, ResourceLoadedCallback callback);

		bool GetHumanClothesSpriteFramesByID(ClothesID id, MotionSpriteFrames& out);

		bool GetHumanPatternSpriteFramesByID(ClothesID id, MotionSpriteFrames& out);

		bool GetHumanWeaponSpriteFramesByID(WeaponID id, MotionSpriteFrames& out);

		bool GetHumanHairSpriteFramesByID(HairID id, MotionSpriteFrames& out);

		bool GetHumanHorseSpriteFramesByID(HorseID id, MotionSpriteFrames& out);
		//
		bool GetEffectByID(EffectID id, EffectData& out);

	public:
		static const HumanMotionInfo s_motion_info[static_cast<size_t>(HumanMotionID::size)];

		static const EffectInfo		s_effect_info[static_cast<size_t>(EffectID::size)];
    private:
		ResourcesPool();

		ResourcesPool(const ResourcesPool&) = delete;

		ResourcesPool& operator=(const ResourcesPool&) = delete;

		/*void OnTextureFileLoaded(std::shared_ptr<Texture> texture);*/

		bool GetHumanSpriteFramesByID(HumanPart part, int main_id, MotionSpriteFrames& out);

		std::shared_ptr<SpriteFrame> LoadAndInitSpriteFrame(const std::string& key);
    private:
		ResourceLoadedCallback		load_callback_;
		int							now_load_index_;
		std::vector<std::string>	p_lists_;
		std::string					now_load_plist_;
    };
}
