#pragma once
#include <pch.h>
#include "SimpleMath.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;

namespace DW
{
	enum class NodeType
	{
		NODE,
		SPRITE,
		DRAW_NODE,
		LABEL
	};
	class Node: public std::enable_shared_from_this<Node>
	{
	public:
		//friend class Scene;
		//friend class Sprite;
		friend class Render;
		static std::shared_ptr<Node> Create(const std::string& name = "");

		Node(const std::string& name = "");

		virtual ~Node();

		void AddChild(std::shared_ptr<Node> child, int local_z_order = 0);

		void RemoveChild(std::shared_ptr<Node> child);

		void RemoveFromParent();

		void SetLocalZOrder(int order);

		int GetLocalZOrder();

		void SetGlobalZOrder(int order);

		int GetGlobalZOrder();

		void SetPosition(const Vector2& position);

		void SetPosition(float x, float y);

		const Vector2& GetPosition() const;

		void SetContentSize(const Size& size);

		void SetContentSize(float width, float height);

		const Size& GetContentSize() const;
		
		void SetAnchorPoint(const Vector2& point);

		void SetAnchorPoint(float x, float y);

		const Vector2& GetAnchorPoint() const;

		void SetScale(float scale);

		void SetScale(float scale_x, float scale_y);

		void SetScale(const Vector2& scale);

		const Vector2& GetScale() const;

		void SetRotation(float rotaion);

		float GetRotation();

		void SetVisible(bool visible);

		bool GetVisible();

		void SetName(const std::string& name);

		const std::string& GetName() const;

		NodeType GetNodeType();

		void SetNodeType(NodeType type);

		void SetOpacity(float alpha);

		float GetOpacity();

		virtual void Draw();

		virtual void Update(float t);

		int Schedule(std::function<void (float)> func, 
			float interval_in_seconds, 
			unsigned int repeat = 0, 
			float delay_in_seconds = 0.0);

		void UnSchedule(int schedule_id);

	protected:
		void VisitForDraw(bool is_parent_transform_dirty, bool is_parent_alpha_dirty);

		void VisitForUpdate(float t);
	private:
		Node(Node&) = delete;
		
		Node& operator=(const Node&) = delete;

		void SetParent(std::shared_ptr<Node> parent);

		void SortChildren();

		void UpdateTransformMatrix(bool is_dirty);

		void UpdateAlpha(bool is_dirty);

		const XMMATRIX GetTransformMatrixWithParent() const;

		XMMATRIX GetScaleMatrix();

		XMMATRIX GetRotationMatrix();

		XMMATRIX GetDisplacementMatrix();

		struct NodeSharedEnabler;
	protected:
		bool				transform_dirty_;
		bool				transform_need_update_;
		bool				alpha_dirty_;
		XMMATRIX			transform_matrix_with_parent_;
		float				alpha_with_parent_;
	private:
		NodeType			node_type_;
		int					local_z_order_;
		int					global_z_order_;
		bool				visible_;
		float				rotation_;
		Vector2				position_;
		Size				size_;
		Vector2				anchor_point_;
		Vector2				scale_;
		Vector2				position_with_parent_and_anchor_;

		std::vector<std::shared_ptr<Node>> children_;
		std::weak_ptr<Node>	parent_;
		std::string			name_;
		XMMATRIX			transform_matrix_;
		float				alpha_;
	};
}


