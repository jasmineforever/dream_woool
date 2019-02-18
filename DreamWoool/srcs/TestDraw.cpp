#include "pch.h"
#include "TestDraw.h"
#include "WICTextureLoader.h"
namespace DW
{
	void TestDraw::CreateResources(Microsoft::WRL::ComPtr<ID3D11Device1> device,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext1> context)
	{
		m_d3dDevice = device;
		m_d3dContext = context;

		D3D11_SAMPLER_DESC samplerDesc;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samplerDesc.MinLOD = -FLT_MAX;
		samplerDesc.MaxLOD = FLT_MAX;
		ID3D11SamplerState* myLinearWrapSampler;
		HRESULT hr = m_d3dDevice->CreateSamplerState(&samplerDesc, &myLinearWrapSampler);
		m_d3dContext->PSSetSamplers(0, 1, &myLinearWrapSampler);
		// TODO: Initialize windows-size dependent objects here.
		VertexPositionColorTexture vertices[4] = {
			{{0, 0, 0.0},		{1.0, 1.0, 1.0, 1.0}, {0.0, 0.0}},
			{{200,0, 0.0},		{1.0, 1.0, 1.0, 1.0}, {1.0, 0.0}},
			{{0, 200, 0.0},	{1.0, 1.0, 1.0, 1.0}, {0.0, 1.0}},
			{{200, 200, 0.0},	{1.0, 1.0, 1.0, 1.0}, {1.0, 1.0}},
		};
		short indices[] = { 0,1,2,
							2,1,3 };
		D3D11_BUFFER_DESC bd = { 0 };
		bd.ByteWidth = sizeof(VertexPositionColorTexture) * ARRAYSIZE(vertices);
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		D3D11_SUBRESOURCE_DATA vv = { vertices, 0, 0 };

		m_d3dDevice->CreateBuffer(&bd, &vv, vertexBuffer_.GetAddressOf());
		D3D11_MAPPED_SUBRESOURCE ms;
		m_d3dContext->Map(vertexBuffer_.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
		memcpy(ms.pData, vertices, sizeof(vertices));                 // copy the data
		m_d3dContext->Unmap(vertexBuffer_.Get(), NULL);                                      // unmap the buffer


		bd.ByteWidth = sizeof(short) * ARRAYSIZE(indices);
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA srd = { indices, 0, 0 };

		m_d3dDevice->CreateBuffer(&bd, &srd, indexBuffer_.GetAddressOf());
		m_d3dContext->Map(indexBuffer_.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
		memcpy(ms.pData, indices, sizeof(indices));                   // copy the data
		m_d3dContext->Unmap(indexBuffer_.Get(), NULL);

		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = 4 * 4 * sizeof(float);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		m_d3dDevice->CreateBuffer(&bd, NULL, cBuffer_.GetAddressOf());
		m_d3dContext->VSSetConstantBuffers(0, 1, cBuffer_.GetAddressOf());

		//shader init
		ID3DBlob* vsBuffer = 0;
		bool compileResult = Utils::CompileD3DShader("resources/shaders/vs.hlsl",
			"VS_Main", "vs_4_0", &vsBuffer);
		if (compileResult == false)
		{
			MessageBox(0, L"ÔØÈë¶¥µã×ÅÉ«Æ÷´íÎó!", L"±àÒë´íÎó", MB_OK);
			return;
		}
		HRESULT d3dResult;
		d3dResult = m_d3dDevice->CreateVertexShader(vsBuffer->GetBufferPointer(
		), vsBuffer->GetBufferSize(), 0, vertexShader_.GetAddressOf());

		if (FAILED(d3dResult))
		{
			if (vsBuffer)
				vsBuffer->Release();
			return;
		}
		ID3DBlob* psBuffer = 0;
		compileResult = Utils::CompileD3DShader("resources/shaders/ps.hlsl",
			"PS_Main", "ps_4_0", &psBuffer);
		if (compileResult == false)
		{
			MessageBox(0, L"ÔØÈëÏñËØ×ÅÉ«Æ÷´íÎó!", L"±àÒë´íÎó", MB_OK);
			return;
		}
		d3dResult = m_d3dDevice->CreatePixelShader(psBuffer->GetBufferPointer(),
			psBuffer->GetBufferSize(), 0, pixelShader_.GetAddressOf());
		psBuffer->Release();
		if (FAILED(d3dResult))
		{
			return;
		}
		//
		m_d3dContext->VSSetShader(vertexShader_.Get(), nullptr, 0);
		m_d3dContext->PSSetShader(pixelShader_.Get(), nullptr, 0);
		//
		 // initialize input layout
		D3D11_INPUT_ELEMENT_DESC ied[] =
		{
			{"SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "COLOR",   0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		// create and set the input layout
		m_d3dDevice->CreateInputLayout(ied, ARRAYSIZE(ied), vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), inputlayout_.GetAddressOf());
		m_d3dContext->IASetInputLayout(inputlayout_.Get());
		//texture
		CreateWICTextureFromFile(m_d3dDevice.Get(), nullptr, L"resources/texture.png", nullptr, texture_.GetAddressOf(), 0);
		//
		D3D11_BLEND_DESC desc = {};

		desc.RenderTarget[0].BlendEnable = TRUE;

		desc.RenderTarget[0].SrcBlend = desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlend = desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		hr = m_d3dDevice->CreateBlendState(&desc, blendState_.GetAddressOf());

		m_d3dContext->OMSetBlendState(blendState_.Get(), nullptr, 0xFFFFFFFF);
		//transform
		//getScaleMatrix()
		XMMATRIX transformMatrix = XMMatrixIdentity();//* getScaleMatrix(); //*  XMMatrixTranslation(30.0f, 30.0f, 0.0f);
		transformMatrix *= getViewportTransform();
		m_d3dContext->UpdateSubresource(cBuffer_.Get(), 0, 0, &transformMatrix, 0, 0);
	}
	DirectX::XMMATRIX TestDraw::getRotationMatrix()
	{
		//XMMatrixTransformation2D();
		//return DirectX::XMMatrixRotationZ(XMConvertToRadians(10));
		//D3DXMatrixRotationX(&matRotateX, D3DXToRadian(50.0f));
		//D3DXMatrixRotationY(&matRotateY, D3DXToRadian(50.0f));


		return DirectX::XMMatrixTranslation(-100.0f, -100.0f, 0.0f) *
			DirectX::XMMatrixRotationZ(XMConvertToRadians(90)) *
			DirectX::XMMatrixTranslation(100.0f, 100.0f, 0.0f);
		float x = 100;
		float y = 100;
		float cv = cos(XMConvertToRadians(45));
		float sv = sin(XMConvertToRadians(45));
		return XMMATRIX
		(
			cv, sv, 0, 0,
			-sv, cv, 0, 0,
			(x*(1 - cv) + y * sv), (y*(1 - cv) - x * sv), 1, 0,
			0, 0, 0, 1
		);
	}
	DirectX::XMMATRIX TestDraw::getScaleMatrix()
	{
		return XMMATRIX
		(
			2.0, 0, 0, 0,
			0, 2.0, 0, 0,
			0, 0, 1, 0,
			100 * (1 - 2.0), 100 * (1 - 2.0), 0, 1
		);
	}
	DirectX::XMMATRIX TestDraw::getViewportTransform()
	{
		// Compute the matrix.
		float xScale = 2.0f / 1280;
		float yScale = 2.0f / 800;

		return XMMATRIX
		(
			xScale, 0, 0, 0,
			0, -yScale, 0, 0,
			0, 0, 1, 0,
			-1, 1, 0, 1
		);
	}

	void TestDraw::Draw()
	{
		UINT stride = sizeof(VertexPositionColorTexture);
		UINT offset = 0;
		m_d3dContext->IASetVertexBuffers(0, 1, vertexBuffer_.GetAddressOf(), &stride, &offset);
		m_d3dContext->IASetIndexBuffer(indexBuffer_.Get(), DXGI_FORMAT_R16_UINT, 0);
		m_d3dContext->PSSetShaderResources(0, 1, texture_.GetAddressOf());
		// set the primitive topology
		m_d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_d3dContext->DrawIndexed(6, 0, 0);
		/*d2d_render_target_->BeginDraw();
		text_layout_->Draw(nullptr, text_render_.get(), 0.0f, 0.0f);
		d2d_render_target_->EndDraw();*/
	}
}