#include <pch.h>
#include "Texture.h"
namespace DW
{
	Texture::Texture()
	{
	}
	Texture::~Texture()
	{
	}
	int Texture::Width()
	{
		return width_;
	}
	int Texture::Height()
	{
		return height_;
	}
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Texture::View()
	{
		return view_;
	}
}
