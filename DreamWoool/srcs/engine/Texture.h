#pragma once
namespace DW
{
	class Texture
	{
		friend class TextureCache;
	public:
		Texture();
		~Texture();
		int width();
		int height();
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> view();
		static std::shared_ptr<Texture> createWithShaderResourceView();
	private:
		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;
	private:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> view_;
		int	width_;
		int height_;
	};
}


