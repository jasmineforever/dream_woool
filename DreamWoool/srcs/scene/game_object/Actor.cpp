#include "pch.h"
#include "Actor.h"
#include <iostream>
namespace DW
{
	Actor::Actor():
		now_motion_(HumanMotionID::stand),
		now_direction_(Direction::up),
		motion_index_(0),
		motion_time_elapsed_(0.0),
		next_motion_frame_time_(0.0),
		is_ride_horse_(false)
	{
		human_sprites_.resize(static_cast<std::size_t>(HumanPart::size));
		//std::for_each(human_sprites_.begin(), human_sprites_.end(), [&](Sprite* &sp) { sp = nullptr;});
	}
	Actor::~Actor()
	{
	}
	std::shared_ptr<Actor>  Actor::Create(Gender gender, HairID hair, HairColor h_color, WeaponID weapon, ClothesID cloth, PatternColor p_color)
	{
		auto actor = std::make_shared<Actor>();

		if (actor && actor->Init(gender, hair, h_color, weapon, cloth, p_color))
		{
			return actor;
		}
		return nullptr;
	}

	bool Actor::Init(Gender gender, HairID hair, HairColor h_color, WeaponID weapon, ClothesID cloth, PatternColor p_color)
	{
		clothes_id_ = cloth;
		auto& res_pool = Singleton<ResourcesPool>::GetInstance();
		res_pool.GetHumanClothesSpriteFramesByID(cloth, human_frames_[static_cast<size_t>(HumanPart::clothes)]);
		res_pool.GetHumanPatternSpriteFramesByID(cloth, human_frames_[static_cast<size_t>(HumanPart::pattern)]);
		res_pool.GetHumanWeaponSpriteFramesByID(weapon, human_frames_[static_cast<size_t>(HumanPart::weapon)]);
		res_pool.GetHumanHairSpriteFramesByID(hair, human_frames_[static_cast<size_t>(HumanPart::hair)]);
		res_pool.GetHumanHorseSpriteFramesByID(HorseID::yingqin, human_frames_[static_cast<size_t>(HumanPart::horse)]);

		std::for_each(human_sprites_.begin(), human_sprites_.end(), [&](std::shared_ptr<Sprite> &sp) {
			sp = Sprite::Create();
			sp->SetAnchorPoint(0., 0.);
			AddChild(sp);
		});
		//auto pgl = new GLProgram();
		//pgl->initWithFilenames("colorbody.vsh", "colorbody.fsh");
		//pgl->retain();
		//pgl->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
		//pgl->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
		//pgl->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
		//pgl->link();
		//pgl->updateUniforms();
		//pgl->use();
		//human_sprites_[static_cast<size_t>(HumanPart::pattern)]->setGLProgram(pgl);
		//human_sprites_[static_cast<size_t>(HumanPart::pattern)]->getGLProgramState()->setUniformFloat("u_cH", 0.8);

		//auto pg2 = new GLProgram();
		//pg2->initWithFilenames("colorbody.vsh", "colorbody.fsh");
		//pg2->retain();
		//pg2->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
		//pg2->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
		//pg2->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
		//pg2->link();
		//pg2->updateUniforms();
		//pg2->use();
		//human_sprites_[static_cast<size_t>(HumanPart::hair)]->setLocalZOrder(3);
		//human_sprites_[static_cast<size_t>(HumanPart::hair)]->setGLProgram(pg2); //setShaderProgram
		//human_sprites_[static_cast<size_t>(HumanPart::hair)]->getGLProgramState()->setUniformFloat("u_cH", 0.49);

		human_sprites_[static_cast<size_t>(HumanPart::pattern)]->SetColor(DWColor::CHeDingHong);
		human_sprites_[static_cast<size_t>(HumanPart::hair)]->SetColor(DWColor::CCuiLv);
		human_sprites_[static_cast<size_t>(HumanPart::hair)]->SetLocalZOrder(2);
		human_sprites_[static_cast<size_t>(HumanPart::pattern)]->SetLocalZOrder(1);
		human_sprites_[static_cast<size_t>(HumanPart::clothes)]->SetLocalZOrder(1);
		blood_bar_ = BloodBar::Create(40, 4, 0, 325, 325, false);
		blood_bar_->SetPosition(108, 40);
		AddChild(blood_bar_);
		
		name_label_ = Label::Create("沧海一声笑", 12);
		name_label_->SetBorder(true);
		name_label_->SetPosition(100, 92);
		AddChild(name_label_);
		name_label_->SetLocalZOrder(30);
		return true;
	}
	void Actor::Update(float t)
	{
		int now_motion = static_cast<int>(now_motion_);
		float total_time = ResourcesPool::s_motion_info[now_motion].animate_time +
			ResourcesPool::s_motion_info[now_motion].rest_time;
		if (motion_time_elapsed_ > total_time)
		{
			if (now_motion_ == HumanMotionID::cast || now_motion_ == HumanMotionID::run || now_motion_ == HumanMotionID::walk)
			{
				motion_time_elapsed_ = 0;
			}
			motion_time_elapsed_ = 0;
			motion_index_ = 0;
			if (is_ride_horse_)
			{
				now_motion_ = HumanMotionID::ride_stand;
			}
			else
			{
				now_motion_ = HumanMotionID::stand;
			}
		}
		else if (motion_index_ != ResourcesPool::s_motion_info[now_motion].max_frame)
		{
			if (motion_index_ == 0)
			{
				ChangeFrame();
				next_motion_frame_time_ = ResourcesPool::s_motion_info[now_motion].frame_time;
				motion_index_ += 1;
				//updatePosition();
			}
			else if(motion_time_elapsed_ > next_motion_frame_time_)
			{
				ChangeFrame();
				next_motion_frame_time_ += ResourcesPool::s_motion_info[now_motion].frame_time;
				motion_index_ += 1;
				//updatePosition();
			}
			if (now_motion_ == HumanMotionID::walk || now_motion_ == HumanMotionID::run)
			{
				UpdatePosition();
			}
		}
		motion_time_elapsed_ += t;
	}
	void Actor::UpdatePosition()
	{
		//if (now_motion_ == HumanMotionID::walk || now_motion_ == HumanMotionID::run)
		//{
		//	if (motion_index_ + 1 == ResourcesPool::s_motion_info[static_cast<size_t>(now_motion_)].max_frame)
		//	{
		//		SetPosition(motion_end_postion_);
		//	}
		//	else
		//	{
		//		SetPosition(GetPosition() + motion_update_position_);
		//	}
		//}
	}
	void Actor::ChangeFrame()
	{
		//分为三大类 1.挖 2.骑马的动作 3.不骑马的动作
		if (now_motion_ == HumanMotionID::dig)
		{
			//只显示头发 衣服和 颜色匹配
		}
		else if (now_motion_ >= HumanMotionID::ride_stand && now_motion_ <= HumanMotionID::ride_death)
		{
			//只显示头发 衣服和 颜色匹配,和马
		}
		else
		{
			// 显示所有除了马
			human_sprites_[static_cast<size_t>(HumanPart::clothes)]->SetSpriteFrame(
				human_frames_[static_cast<size_t>(HumanPart::clothes)]
				[static_cast<size_t>(now_motion_)][static_cast<size_t>(now_direction_)][motion_index_]);

			if (clothes_id_ > ClothesID::naked_woman)
			{
				human_sprites_[static_cast<size_t>(HumanPart::pattern)]->SetSpriteFrame(
					human_frames_[static_cast<size_t>(HumanPart::pattern)]
					[static_cast<size_t>(now_motion_)][static_cast<size_t>(now_direction_)][motion_index_]);
			}
			
			if (hair_id_ != HairID::qingliang)
			{
				human_sprites_[static_cast<size_t>(HumanPart::hair)]->SetSpriteFrame(
					human_frames_[static_cast<size_t>(HumanPart::hair)]
					[static_cast<size_t>(now_motion_)][static_cast<size_t>(now_direction_)][motion_index_]);
			}
			if (weapon_id_ != WeaponID::no_weapon)
			{
				human_sprites_[static_cast<size_t>(HumanPart::weapon)]->SetSpriteFrame(
					human_frames_[static_cast<size_t>(HumanPart::weapon)]
					[static_cast<size_t>(now_motion_)][static_cast<size_t>(now_direction_)][motion_index_]);
			}
		}
	}
	void Actor::Run(Direction dir)
	{
		if (now_motion_ == HumanMotionID::stand)
		{
			now_motion_ = HumanMotionID::run;
			now_direction_ = dir;
			motion_time_elapsed_ = 0;
			motion_index_ = 0;
			next_motion_frame_time_ = ResourcesPool::s_motion_info[static_cast<size_t>(now_motion_)].frame_time;
			//float frames = ResourcesPool::s_motion_info[static_cast<size_t>(HumanMotionID::run)].max_frame;
			float frames = 60.0 * ResourcesPool::s_motion_info[static_cast<size_t>(HumanMotionID::run)].animate_time;
			switch (now_direction_)
			{
			case Direction::up:
				motion_end_postion_ = GetPosition() + Vector2(0, -64);
				motion_update_position_ = Vector2(0, -64) / frames;
				break;
			case Direction::up_right:
				motion_end_postion_ = GetPosition() + Vector2(128, -64);
				motion_update_position_ = Vector2(128, -64) / frames;
				break;
			case Direction::right:
				motion_end_postion_ = GetPosition() + Vector2(128, 0);
				motion_update_position_ = Vector2(128, 0) / frames;
				break;
			case Direction::right_down:
				motion_end_postion_ = GetPosition() + Vector2(128, 64);
				motion_update_position_ = Vector2(128, 64) / frames;
				break;
			case Direction::down:
				motion_end_postion_ = GetPosition() + Vector2(0, 64);
				motion_update_position_ = Vector2(0, 64) / frames;
				break;
			case Direction::left_down:
				motion_end_postion_ = GetPosition() + Vector2(-128, 64);
				motion_update_position_ = Vector2(-128, 64) / frames;
				break;
			case Direction::left:
				motion_end_postion_ = GetPosition() + Vector2(-128, 0);
				motion_update_position_ = Vector2(-128, 0) / frames;
				break;
			case Direction::left_up:
				motion_end_postion_ = GetPosition() + Vector2(-128, -64);
				motion_update_position_ = Vector2(-128, -64) / frames;
				break;
			}
		}
	}

	void Actor::Walk(Direction dir)
	{
		if (now_motion_ == HumanMotionID::stand)
		{
			now_motion_ = HumanMotionID::walk;
			now_direction_ = dir;
			motion_time_elapsed_ = 0;
			motion_index_ = 0;
			next_motion_frame_time_ = ResourcesPool::s_motion_info[static_cast<size_t>(now_motion_)].frame_time;
			//float frames = ResourcesPool::s_motion_info[static_cast<size_t>(HumanMotionID::walk)].max_frame;
			float frames = 60.0 * ResourcesPool::s_motion_info[static_cast<size_t>(HumanMotionID::walk)].animate_time;
			switch (now_direction_)
			{
			case Direction::up:
				motion_end_postion_ = GetPosition() + Vector2(0, -32);
				motion_update_position_ = Vector2(0, -32) / frames;
				break;
			case Direction::up_right:
				motion_end_postion_ = GetPosition() + Vector2(64, -32);
				motion_update_position_ = Vector2(64, -32) / frames;
				break;
			case Direction::right:
				motion_end_postion_ = GetPosition() + Vector2(64, 0);
				motion_update_position_ = Vector2(64, 0) / frames;
				break;
			case Direction::right_down:
				motion_end_postion_ = GetPosition() + Vector2(64, -32);
				motion_update_position_ = Vector2(64, -32) / frames;
				break;
			case Direction::down:
				motion_end_postion_ = GetPosition() + Vector2(0, 32);
				motion_update_position_ = Vector2(0, 32) / frames;
				break;
			case Direction::left_down:
				motion_end_postion_ = GetPosition() + Vector2(-64, 32);
				motion_update_position_ = Vector2(-64, 32) / frames;
				break;
			case Direction::left:
				motion_end_postion_ = GetPosition() + Vector2(-64, 0);
				motion_update_position_ = Vector2(-64, 0) / frames;
				break;
			case Direction::left_up:
				motion_end_postion_ = GetPosition() + Vector2(-64, -32);
				motion_update_position_ = Vector2(-64, -32) / frames;
				break;
			}
		}
	}
	void Actor::NowDeath()
	{
		now_motion_ = HumanMotionID::death;
		motion_time_elapsed_ = 0;
		motion_index_ = 0;
		next_motion_frame_time_ = ResourcesPool::s_motion_info[static_cast<size_t>(now_motion_)].frame_time;
	}
	void Actor::Cast(EffectID id, Direction dir)
	{
		//if (now_motion_ == HumanMotionID::stand)
		//{
		//	EffectData data;
		//	if (ResourcesPool::getInstance().GetEffectByID(EffectID::bingjianshu2, data))
		//	{
		//		cocos2d::Vector<SpriteFrame*> cc_vec;
		//		for (int i = 0; i < data.frames.size(); i++)
		//		{
		//			//381, 381
		//			data.frames[i]->setOffset(data.frames[i]->getOffset() + Vec2(-381, 381));
		//			cc_vec.pushBack(data.frames[i]);
		//		}
		//		auto* s = Sprite::create();
		//		addChild(s);
		//		auto* animation = Animation::createWithSpriteFrames(cc_vec, ResourcesPool::s_effect_info[static_cast<size_t>(EffectID::bingjianshu2)].animate_time / ResourcesPool::s_effect_info[static_cast<size_t>(EffectID::bingjianshu2)].max_frame);
		//		auto* animate = CCAnimate::create(animation);
		//		s->runAction(animate);
		//	}
		//	now_motion_ = HumanMotionID::cast;
		//	motion_time_elapsed_ = 0;
		//	motion_index_ = 0;
		//	next_motion_frame_time_ = ResourcesPool::s_motion_info[static_cast<size_t>(now_motion_)].frame_time;
		//}
	}
}