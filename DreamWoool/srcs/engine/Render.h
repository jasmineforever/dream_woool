#pragma once
#include "Scene.h"
#include "CommonStates.h"

namespace DW
{
	struct ShaderCode
	{
		const BYTE* code;
		UINT size;
	};
	class Render
	{
	public:
		Render();

		~Render();

		void RenderScene(Scene* scene);

		void Init(ID3D11Device1* d3d_device,
			ID3D11DeviceContext1* d3d_context,
			ID2D1RenderTarget* d2d_render_target,
			IDWriteFactory* dwrite_factory,
			ID2D1Factory* d2d_factory);

		
		ShaderCode GetSpriteDefaultVertexShaderCode();

		ShaderCode GetDrawNodeDefaultVertexShaderCode();
		
		void PushNodeToRenderQueue(Node* node);

	private:
		Render(const Render&) = delete;

		Render& operator=(const Render&) = delete;

		void InitShaders();

		DirectX::XMMATRIX GetViewportTransform();
	public:
		ID3D11Device1*			d3d_device;
		ID3D11DeviceContext1*	d3d_context;
		ID2D1RenderTarget*		d2d_render_target;
		IDWriteFactory*			dwrite_factory;
		ID2D1Factory*			d2d_factory;
	private:
		struct TransformFinal
		{
			DirectX::XMMATRIX	transform_matrix;
			float				alpha;
		} transform_;
		std::vector<Node*>				nodes_;
		std::unique_ptr<CommonStates>	common_states_;
		Microsoft::WRL::ComPtr<ID3D11VertexShader>	sprite_vertex_shader_;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>	sprite_pixel_shader_;
		Microsoft::WRL::ComPtr<ID3D11VertexShader>	draw_node_vertex_shader_;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>	draw_node_pixel_shader_;
		Microsoft::WRL::ComPtr <ID3D11Buffer>		c_buffer_;
	};
}