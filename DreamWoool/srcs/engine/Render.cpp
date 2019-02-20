#include "pch.h"
#include "Render.h"
#include "Sprite.h"
#include "DrawNode.h"
#include "Director.h"
#include "shaders/compiled/basic_sprite_vs.inc"
#include "shaders/compiled/basic_sprite_ps.inc"
#include "shaders/compiled/basic_draw_ps.inc"
#include "shaders/compiled/basic_draw_vs.inc"
namespace DW
{
	Render::Render():
		d3d_device(nullptr),
		d3d_context(nullptr),
		d2d_render_target(nullptr),
		dwrite_factory(nullptr),
		d2d_factory(nullptr)
	{

	}
	Render::~Render()
	{

	}
	void Render::Init(ID3D11Device1* _d3d_device,
		ID3D11DeviceContext1* _d3d_context,
		ID2D1RenderTarget* _d2d_render_target,
		IDWriteFactory* _dwrite_factory,
		ID2D1Factory* _d2d_factory)
	{
		d3d_device = _d3d_device;
		d3d_context = _d3d_context;
		d2d_render_target = _d2d_render_target;
		dwrite_factory = _dwrite_factory;
		d2d_factory = _d2d_factory;
		common_states_.reset();
		common_states_ = std::make_unique<CommonStates>(_d3d_device);

		//make the font clear
		d2d_render_target->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_ALIASED);
		d2d_render_target->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
		InitShaders();
	}
	void Render::InitShaders()
	{
		HR(d3d_device->CreateVertexShader(g_basic_sprite_vs_main,
			sizeof(g_basic_sprite_vs_main), 0, sprite_vertex_shader_.GetAddressOf()));

		HR(d3d_device->CreatePixelShader(g_basic_sprite_ps_main,
			sizeof(g_basic_sprite_ps_main), 0, sprite_pixel_shader_.GetAddressOf()));

		HR(d3d_device->CreateVertexShader(g_basic_draw_vs_main,
			sizeof(g_basic_draw_vs_main), 0, draw_node_vertex_shader_.GetAddressOf()));

		HR(d3d_device->CreatePixelShader(g_basic_draw_ps_main,
			sizeof(g_basic_draw_ps_main), 0, draw_node_pixel_shader_.GetAddressOf()));

		D3D11_BUFFER_DESC bd = { 0 };
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(TransformFinal);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		d3d_device->CreateBuffer(&bd, NULL, c_buffer_.GetAddressOf());
	}
	ShaderCode Render::GetSpriteDefaultVertexShaderCode()
	{
		ShaderCode code;
		code.code = &g_basic_sprite_vs_main[0];
		code.size = sizeof(g_basic_sprite_vs_main);
		return code;
	}

	DirectX::XMMATRIX Render::GetViewportTransform()
	{
		// Compute the matrix.
		auto size = Director::GetInstance().GetScreenSize();
		float xScale = 2.0f / size.width;
		float yScale = 2.0f / size.height;

		return DirectX::XMMATRIX
		(
			xScale, 0, 0, 0,
			0, -yScale, 0, 0,
			0, 0, 1, 0,
			-1, 1, 0, 1
		);
	}
	ShaderCode Render::GetDrawNodeDefaultVertexShaderCode()
	{
		ShaderCode code;
		code.code = &g_basic_draw_vs_main[0];
		code.size = sizeof(g_basic_draw_vs_main);
		return code;
	}
	void Render::PushNodeToRenderQueue(Node* node)
	{
		nodes_.push_back(node);
	}
	void Render::RenderScene(Scene* scene)
	{
		nodes_.clear();
		if (scene)
		{
			scene->Render();
		}
		std::sort(nodes_.begin(), nodes_.end(), [](Node* a, Node* b) {

			return a->GetGlobalZOrder() > b->GetGlobalZOrder();
		});
		for (auto node : nodes_)
		{
			auto type = node->GetNodeType();
			d3d_context->OMSetBlendState(common_states_->NonPremultiplied(), nullptr, 0xFFFFFFFF);
			d3d_context->OMSetDepthStencilState(common_states_->DepthNone(), 0);

			if (type == NodeType::SPRITE)
			{
				auto sample_state = common_states_->LinearClamp();
				d3d_context->PSSetSamplers(0, 1, &sample_state);
				d3d_context->VSSetShader(sprite_vertex_shader_.Get(), nullptr, 0);
				d3d_context->PSSetShader(sprite_pixel_shader_.Get(), nullptr, 0);
				d3d_context->VSSetConstantBuffers(0, 1, c_buffer_.GetAddressOf());
				transform_.transform_matrix = node->transform_matrix_with_parent_ * GetViewportTransform();
				transform_.alpha = node->alpha_with_parent_;
				d3d_context->UpdateSubresource(c_buffer_.Get(), 0, 0, &transform_, 0, 0);
			}
			else if (type == NodeType::DRAW_NODE)
			{
				d3d_context->VSSetShader(draw_node_vertex_shader_.Get(), nullptr, 0);
				d3d_context->PSSetShader(draw_node_pixel_shader_.Get(), nullptr, 0);
				d3d_context->VSSetConstantBuffers(0, 1, c_buffer_.GetAddressOf());
				transform_.transform_matrix = node->transform_matrix_with_parent_ * GetViewportTransform();
				transform_.alpha = node->alpha_with_parent_;
				d3d_context->UpdateSubresource(c_buffer_.Get(), 0, 0, &transform_, 0, 0);
			}
			else if (type == NodeType::LABEL)
			{

			}
			node->Draw();
		}
	}
}