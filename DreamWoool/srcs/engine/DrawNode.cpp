#include <pch.h>
#include "DrawNode.h"
#include "Director.h"
namespace DW
{
	DrawNode::DrawNode():
		Node("")
	{
		SetNodeType(NodeType::DRAW_NODE);
	}
	DrawNode::~DrawNode()
	{
		Clear();
	}

	std::shared_ptr<DrawNode> DrawNode::Create()
	{
		auto p = std::make_shared<DrawNode>();
		p->InitGraphics();
		return p;
	}
	void DrawNode::InitGraphics()
	{
		auto& render = Director::GetInstance().GetRender();
		auto shader = render.GetDrawNodeDefaultVertexShaderCode();
		render.d3d_device->CreateInputLayout(VertexPositionColor::InputElements,
			VertexPositionColor::InputElementCount,
			shader.code,
			shader.size,
			inputlayout_.GetAddressOf());
	}
	void DrawNode::InitColor(PrimtiveData& primtive, XMVECTORF32 color)
	{
		if (primtive.vertices)
		{
			auto p = primtive.vertices;
			for (int i = 0; i < primtive.vertex_size; i++)
			{
				p[i].color = { color.f[0], color.f[1] , color.f[2], color.f[3] };
			}
		}
	}
	void DrawNode::CreateVertices(PrimtiveData& primtive)
	{
		D3D11_BUFFER_DESC bd = { 0 };
		bd.ByteWidth = sizeof(VertexPositionColor) * primtive.vertex_size;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		D3D11_SUBRESOURCE_DATA vv = { primtive.vertices, 0, 0 };
		Director::GetInstance().GetRender().d3d_device->CreateBuffer(&bd, &vv, primtive.vertex_buffer.GetAddressOf());
	}
	void DrawNode::DrawLine(Vector2 p_start, Vector2 p_end, XMVECTORF32 color)
	{
		PrimtiveData primtive;
		primtive.vertex_size = 2;
		primtive.vertices = new VertexPositionColor[primtive.vertex_size];
		auto p = primtive.vertices;
		p[0].position = { p_start.x, p_start.y, 0.0};
		p[1].position = { p_end.x, p_end.y, 0.0};
		InitColor(primtive, color);
		primtive.topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
		CreateVertices(primtive);
		primtives_.push_back(primtive);
	}
	void DrawNode::DrawQuad(Vector2 p_start, Size size, XMVECTORF32 color)
	{
		PrimtiveData primtive;
		primtive.vertex_size = 8;
		primtive.vertices = new VertexPositionColor[primtive.vertex_size];
		auto p = primtive.vertices;
		p[0].position = p[7].position = { p_start.x, p_start.y, 0.0 };
		p[1].position = p[2].position = { p_start.x + size.width, p_start.y, 0.0 };
		p[3].position = p[4].position = { p_start.x + size.width, p_start.y + size.height, 0.0 };
		p[5].position = p[6].position = { p_start.x, p_start.y + size.height, 0.0 };
		InitColor(primtive, color);
		primtive.topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
		CreateVertices(primtive);
		primtives_.push_back(primtive);
	}
	void DrawNode::DrawQuadSolid(Vector2 p_start, Size size, XMVECTORF32 color)
	{
		PrimtiveData primtive;
		primtive.vertex_size = 6;
		primtive.vertices = new VertexPositionColor[primtive.vertex_size];
		auto p = primtive.vertices;
		p[0].position = { p_start.x, p_start.y, 0.0 };
		p[1].position = p[4].position = { p_start.x + size.width, p_start.y, 0.0 };
		p[2].position = p[3].position = { p_start.x, p_start.y + size.height, 0.0 };
		p[5].position = { p_start.x + size.width, p_start.y + size.height, 0.0 };
		InitColor(primtive, color);
		primtive.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		CreateVertices(primtive);
		primtives_.push_back(primtive);
	}

	void DrawNode::DrawTriangle(Vector2 a, Vector2 b, Vector2 c, XMVECTORF32 color)
	{
		PrimtiveData primtive;
		primtive.vertex_size = 3;
		primtive.vertices = new VertexPositionColor[primtive.vertex_size];
		auto p = primtive.vertices;
		p[0].position = { a.x, a.y, 0.0 };
		p[1].position = { b.x, b.y, 0.0 };
		p[2].position = { c.x, c.y, 0.0 };
		InitColor(primtive, color);
		primtive.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		CreateVertices(primtive);
		primtives_.push_back(primtive);
	}

	void DrawNode::Clear()
	{
		for (auto& primtive : primtives_)
		{
			if (primtive.vertices)
			{
				delete[] primtive.vertices;
			}
		}
		primtives_.clear();
	}
	void DrawNode::Draw()
	{
		auto& render = Director::GetInstance().GetRender();
		for (int i = 0; i < primtives_.size(); i++)
		{
			UINT stride = sizeof(VertexPositionColor);
			UINT offset = 0;
			render.d3d_context->IASetVertexBuffers(0, 1, primtives_[i].vertex_buffer.GetAddressOf(), &stride, &offset);
			render.d3d_context->IASetInputLayout(inputlayout_.Get());
		
			D3D11_MAPPED_SUBRESOURCE ms;
			render.d3d_context->Map(primtives_[i].vertex_buffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
			memcpy(ms.pData, primtives_[i].vertices, sizeof(primtives_[i].vertices[0])* primtives_[i].vertex_size);
			render.d3d_context->Unmap(primtives_[i].vertex_buffer.Get(), NULL);

			render.d3d_context->IASetPrimitiveTopology(primtives_[i].topology);
			render.d3d_context->Draw(primtives_[i].vertex_size, 0);
		}
	}
}
