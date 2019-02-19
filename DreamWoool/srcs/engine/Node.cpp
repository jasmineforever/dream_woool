#include <pch.h>
#include "Node.h"
#include "Director.h"
namespace DW
{
	struct Node::NodeSharedEnabler : public Node
	{
		template <typename... Args>
		NodeSharedEnabler(Args &&... args)
			:Node(std::forward<Args>(args)...)
		{
		}
	};
	std::shared_ptr<Node> Node::Create(const std::string& name)
	{
		//return std::make_shared<NodeSharedEnabler>(name);
		return std::make_shared<Node>(name);
	}
	Node::Node(const std::string& name):
		node_type_(NodeType::NODE),
		name_(name),
		local_z_order_(0),
		global_z_order_(0),
		visible_(true),
		transform_dirty_(true),
		transform_need_update_(true),
		transform_matrix_(XMMatrixIdentity()),
		transform_matrix_with_parent_(XMMatrixIdentity()),
		position_(0., 0.),
		position_with_parent_and_anchor_(0., 0.),
		scale_(1.0, 1.0),
		rotation_(0.),
		anchor_point_(0.,0.),
		alpha_(1.0),
		alpha_with_parent_(1.0),
		alpha_dirty_(true)
	{
	}
	void Node::SetLocalZOrder(int order)
	{
		local_z_order_ = order;
	}

	int Node::GetLocalZOrder()
	{
		return local_z_order_;
	}

	void Node::SetGlobalZOrder(int order)
	{
		global_z_order_ = order;
	}

	int Node::GetGlobalZOrder()
	{
		return global_z_order_;
	}

	void Node::SetPosition(const Vector2& position)
	{
		if (position_ != position)
		{
			position_ = position;
			transform_dirty_ = transform_need_update_ = true;
		}
	}

	void Node::SetPosition(float x, float y)
	{
		SetPosition(Vector2(x, y));
	}

	const Vector2& Node::GetPosition() const
	{
		return position_;
	}
	void Node::SetContentSize(const Size& size)
	{
		if (size_ != size)
		{
			size_ = size;
			transform_dirty_ = transform_need_update_ = true;
		}
	}

	void Node::SetContentSize(float width, float height)
	{
		SetContentSize(Size(width, height));
	}

	const Size& Node::GetContentSize() const
	{
		return size_;
	}

	void Node::SetAnchorPoint(const Vector2& point)
	{
		if (anchor_point_ != point)
		{
			anchor_point_ = point;
			transform_dirty_ = transform_need_update_ = true;
		}
	}

	void Node::SetAnchorPoint(float x, float y)
	{
		SetAnchorPoint(Vector2(x, y));
	}

	const Vector2& Node::GetAnchorPoint() const
	{
		return anchor_point_;
	}

	void Node::SetScale(float scale)
	{
		SetScale(Vector2(scale, scale));
	}

	void Node::SetScale(float scale_x, float scale_y)
	{
		SetScale(Vector2(scale_x, scale_y));
	}

	void Node::SetScale(const Vector2& scale)
	{
		if (scale_ != scale)
		{
			scale_ = scale;
			transform_dirty_ = transform_need_update_ = true;
		}
	}

	const Vector2& Node::GetScale() const
	{
		return scale_;
	}

	void Node::SetRotation(float rotaion)
	{
		if (abs(rotation_ - rotaion) >= FLT_TRUE_MIN)
		{
			rotation_ = rotaion;
			transform_dirty_ = transform_need_update_ = true;
		}
	}

	float Node::GetRotation()
	{
		return rotation_;
	}

	void Node::SetVisible(bool visible)
	{
		visible_ = visible;
	}

	bool Node::GetVisible()
	{
		return visible_;
	}

	void Node::SetName(const std::string& name)
	{
		name_ = name;
	}

	const std::string& Node::GetName() const
	{
		return name_;
	}

	NodeType Node::GetNodeType()
	{
		return node_type_;
	}

	void Node::SetNodeType(NodeType type)
	{
		node_type_ = type;
	}

	void Node::SetParent(std::shared_ptr<Node> parent)
	{
		parent_ = parent;
	}
	
	void Node::SetOpacity(float alpha)
	{
		alpha_ = alpha;
		alpha_dirty_ = true;
	}

	float Node::GetOpacity()
	{
		return alpha_;
	}

	void Node::AddChild(std::shared_ptr<Node> child, int local_z_order)
	{
		if (!child  || child.get() == this)
			return;
		auto parent = parent_.lock();
		if (parent == child)
			return;
		child->parent_ = shared_from_this();
		child->SetLocalZOrder(local_z_order);
		children_.push_back(child);
	}

	void Node::RemoveChild(std::shared_ptr<Node> child)
	{
		if (child)
		{
			for(auto cit = children_.cbegin();cit !=  children_.cend(); ++cit)
			{
				if ((*cit) == child)
				{
					children_.erase(cit);
					break;
				}
			}
		}
	}
	void Node::RemoveFromParent()
	{
		if (auto parent = parent_.lock())
		{
			parent->RemoveChild(shared_from_this());
		}
	}
	void Node::SortChildren()
	{
		std::sort(children_.begin(), children_.end(), [](std::shared_ptr<Node> a, std::shared_ptr<Node> b) -> bool
		{
			return a->local_z_order_ < b->local_z_order_;
		});
	}
	int Node::Schedule(std::function<void(float)> func,
		float interval_in_seconds,
		unsigned int repeat,
		float delay_in_seconds)
	{
		return Director::GetInstance().Schedule(func, interval_in_seconds, repeat, delay_in_seconds);
	}

	void Node::UnSchedule(int schedule_id)
	{
		return Director::GetInstance().UnSchedule(schedule_id);
	}
	void Node::Update(float t)
	{

	}
	void Node::Draw()
	{
		std::cout << name_ << ":"<<local_z_order_ <<" draw"<< std::endl;
	}
	const XMMATRIX Node::GetTransformMatrixWithParent() const
	{
		return transform_matrix_with_parent_;
	}
	
	void Node::UpdateAlpha(bool is_dirty)
	{
		if (is_dirty)
		{
			if (auto p = parent_.lock())
			{
				alpha_with_parent_ = alpha_ * p->alpha_with_parent_;
			}
			else
			{
				alpha_with_parent_ = alpha_;
			}
		}
	}

	void Node::UpdateTransformMatrix(bool is_dirty)
	{
		if (transform_need_update_)
		{	
			float x = anchor_point_.x * size_.width;
			float y = anchor_point_.y * size_.height;

			transform_matrix_ = XMMatrixTranslation(-x, -y, 0.) *
				XMMatrixScaling(scale_.x, scale_.y, 1.0) *
				XMMatrixRotationZ(rotation_) *
				XMMatrixTranslation(position_.x, position_.y, 0.);
			transform_need_update_ = false;
		}
		if (is_dirty)
		{
			if (auto p = parent_.lock())
			{
				transform_matrix_with_parent_ = transform_matrix_ * p->transform_matrix_with_parent_;
			}
			else
			{
				transform_matrix_with_parent_ = transform_matrix_;
			}
		}
	}
	//XMMATRIX Node::GetViewportTransform()
	//{
	//	// Compute the matrix.
	//	float xScale = 1280 / 800;
	//	float yScale = 800 / 600;

	//	return XMMATRIX
	//	(
	//		xScale, 0, 0, 0,
	//		0, -yScale, 0, 0,
	//		0, 0, 1, 0,
	//		-1, 1, 0, 1
	//	);
	//}
	XMMATRIX Node::GetDisplacementMatrix()
	{
		float x = position_.x - anchor_point_.x * size_.width;
		float y = position_.y - anchor_point_.y * size_.height;
		/*if (auto p = parent_.lock())
		{
			x += p->anchor_point_.x * p->size_.width;
			y += p->anchor_point_.y * p->size_.height;
		}*/
		return XMMatrixTranslation(position_with_parent_and_anchor_.x, position_with_parent_and_anchor_.y, 0.0f);
	}
	XMMATRIX Node::GetRotationMatrix()
	{
		float x = anchor_point_.x * size_.width;
		float y = anchor_point_.y * size_.height;
		return  XMMatrixTranslation(-x, -y, 0.) *
				XMMatrixRotationZ(rotation_) *
			XMMatrixTranslation(x, y, 0.);
		//return XMMatrixRotationZ(rotation_);
	}
	XMMATRIX Node::GetScaleMatrix()
	{
		float x = anchor_point_.x * size_.width;
		float y = anchor_point_.y * size_.height;
		return XMMatrixTranslation(-x, -y, 0.) *
			XMMatrixScaling(scale_.x, scale_.y, 1.0) * 
			XMMatrixTranslation(x, y, 0.);
		//float x = (anchor_point_.x * size_.width) * (1 - scale_.x);
		//float y = (anchor_point_.y * size_.height)* (1 - scale_.y);
		//return XMMATRIX
		//(
		//	scale_.x,	0,			0, 0,
		//	0,			scale_.y,	0, 0,
		//	0,			0,			1, 0,
		//	x,			y,			0, 1
		//);
	}
	void Node::VisitForUpdate(float t)
	{
		if (children_.size() > 0)
		{
			Update(t);

			for (auto& c : children_)
			{
				c->VisitForUpdate(t);
			}
		}
		else
		{
			Update(t);
		}
	}
	void Node::VisitForDraw(bool is_parent_transform_dirty, bool is_parent_alpha_dirty)
	{
		if (visible_)
		{
			bool is_transform_dirty = is_parent_transform_dirty | transform_dirty_;
			bool is_alpha_dirty = is_parent_alpha_dirty | alpha_dirty_;
			if (children_.size() > 0)
			{
				UpdateTransformMatrix(is_transform_dirty);
				UpdateAlpha(is_alpha_dirty);
		
				Director::GetInstance().GetRender().PushNodeToRenderQueue(this);
				SortChildren();

				for (auto& c : children_)
				{
					c->VisitForDraw(is_transform_dirty, is_alpha_dirty);
				}
			}
			else
			{
				UpdateTransformMatrix(is_transform_dirty);
				UpdateAlpha(is_alpha_dirty);
				Director::GetInstance().GetRender().PushNodeToRenderQueue(this);
			}
			transform_dirty_ = false;
			alpha_dirty_ = false;
		}
	}
	Node::~Node()
	{
		std::cout << name_ << " destroyed" << std::endl;
	}
}

