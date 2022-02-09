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
	using oneDirectionSpriteFrames = std::vector <std::shared_ptr<SpriteFrame>>; //size = ÿ��������֡��
	using AllDirectionSpriteFrames = std::vector <oneDirectionSpriteFrames>;//size = Direction::size
	using MotionSpriteFrames = std::vector<AllDirectionSpriteFrames>;//size = MotionID::size
	struct HumanMotionInfo {
		int max_frame;
		float animate_time;
		float rest_time;
		float frame_time;
	};

	struct FlyToEffectInfo {
		int	direction_num;		//������� һ����16������
		int one_dir_frames;		//�����ж���֡��
		float animate_time;		//���һ�η��ж�����Ҫ��ʱ��
		float speed;			//�����ٶ�
	};

	struct EffectInfo {
		EffectType type;
		int		max_frame;
		float	animate_time;		//�����Ĳ���ʱ��
		FlyToEffectInfo* fly_to_info;
		float	loop_delay_time;	//������������ŵĻ� �м����ʱʱ��
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
