#pragma once
#include "Node.h"
#include "VertexTypes.h"

namespace DW
{
	class DrawNode : public Node
	{
	public:
		friend class Render;

		DrawNode();

		~DrawNode();

		void DrawLine(Vector2 p_start, Vector2 p_end, XMVECTORF32 color = DirectX::Colors::White);

		void DrawQuad(Vector2 p, Size size, XMVECTORF32 color = DirectX::Colors::White);

		void DrawQuadSolid(Vector2 p, Size size, XMVECTORF32 color = DirectX::Colors::White);

		void DrawTriangle(Vector2 a, Vector2 b, Vector2 c, XMVECTORF32 color = DirectX::Colors::White);

		void Clear();

		void Draw();

		static std::shared_ptr<DrawNode> Create();
	private:
		struct PrimtiveData
		{
			VertexPositionColor*					vertices;
			int										vertex_size;
			Microsoft::WRL::ComPtr<ID3D11Buffer>	vertex_buffer;
			D3D11_PRIMITIVE_TOPOLOGY				topology;
		};
		void InitColor(PrimtiveData& primtive, XMVECTORF32 color);

		void CreateVertices(PrimtiveData& primtive);

		void InitGraphics();
	private:
		DrawNode(const DrawNode&) = delete;
		DrawNode& operator =(const DrawNode&) = delete;
	private:
		std::vector<PrimtiveData>					primtives_;
		Microsoft::WRL::ComPtr<ID3D11InputLayout>	inputlayout_;
	};
}

