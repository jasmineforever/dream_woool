#pragma once
namespace DW
{
	class Texture
	{
		friend class TextureCache;
	public:
		Texture();
		~Texture();
		int Width();
		int Height();
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> View();
		static std::shared_ptr<Texture> CreateWithShaderResourceView();
	private:
		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;
	private:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> view_;
		int	width_;
		int height_;
	};
}


