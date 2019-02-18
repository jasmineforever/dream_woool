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
	int Texture::width()
	{
		return width_;
	}
	int Texture::height()
	{
		return height_;
	}
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Texture::view()
	{
		return view_;
	}
}
