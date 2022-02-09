#pragma once
#include <vector>
#include "dwTypedef.h"
#include "ResourcesPool.h"
#include "BloodBar.h"
#include "Sprite.h"
using namespace DirectX;
using namespace SimpleMath;
namespace DW
{
	class Actor : public Node
	{
	public:
		Actor();

		~Actor();

		void Run(Direction dir);

		void Walk(Direction dir);

		void Turn(Direction dir);

		void Hit(Direction dir, MagicID m_id = MagicID::normal_hit);

		void Cast(EffectID m_id, Direction dir);

		void ShowEffect(EffectID m_id);

		void ShowFlyToEffect(EffectID m_id);

		void HitByMagic(MagicID m_id);

		void BeHit(Direction dir);

		void NowDeath();

		void Death(Direction dir);

		void Dig(Direction dir);

		void SetClothes(ClothesID cloth, PatternColor p_color);

		void SetWeapon(WeaponID weapon);

		void SetShield(WeaponID weapon);

		void SetWing(WeaponID weapon);

		void SetHair(HairID hair, HairColor color);

		static std::shared_ptr<Actor> Create(Gender gender, HairID hair, HairColor h_color, WeaponID weapon, ClothesID cloth, PatternColor p_color);

		void Update(float t) override;
	private:

		bool Init(Gender gender, HairID hair, HairColor h_color, WeaponID weapon, ClothesID cloth, PatternColor p_color);
		
		void ChangeFrame();

		void UpdatePosition();
	private: 
		MotionSpriteFrames human_frames_[static_cast<size_t>(HumanPart::size)];//7个部分,12个动作, 8个方向
		std::shared_ptr<BloodBar>	blood_bar_;
		WeaponID					weapon_id_;
		HairID						hair_id_;
		ClothesID					clothes_id_;
		Gender						gender_;
		std::vector<std::shared_ptr<Sprite>>		human_sprites_;
		HumanMotionID				now_motion_;
		Direction					now_direction_;
		int							motion_index_;
		float						motion_time_elapsed_;
		float						next_motion_frame_time_;
		bool						is_ride_horse_;
		Vector2						motion_update_position_;
		Vector2						motion_end_postion_;
		Vector2						old_postion_;
		std::shared_ptr <Label>		name_label_;
	};
}