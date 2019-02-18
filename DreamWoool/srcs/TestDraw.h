#pragma once

namespace DW
{
	class TestDraw
	{
	public:
		void Draw();

		void CreateResources(Microsoft::WRL::ComPtr<ID3D11Device1> device,
							Microsoft::WRL::ComPtr<ID3D11DeviceContext1> context);


		Microsoft::WRL::ComPtr<ID3D11Device1>           m_d3dDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    m_d3dContext;
		struct VertexPositionColorTexture
		{
			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT4 color;
			DirectX::XMFLOAT2 textureCoordinate;
		};
		Microsoft::WRL::ComPtr<ID3D11Buffer>		vertexBuffer_;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		indexBuffer_;
		Microsoft::WRL::ComPtr <ID3D11Buffer>		cBuffer_;
		Microsoft::WRL::ComPtr<ID3D11VertexShader>	vertexShader_;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>	pixelShader_;
		Microsoft::WRL::ComPtr<ID3D11InputLayout>	inputlayout_;

		Microsoft::WRL::ComPtr<ID3D11BlendState>			blendState_;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	texture_;
		DirectX::XMMATRIX getViewportTransform();
		DirectX::XMMATRIX getScaleMatrix();
		DirectX::XMMATRIX getRotationMatrix();
	};
}