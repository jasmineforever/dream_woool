#pragma once
#include "Node.h"
#include "SpriteFrame.h"
#include "VertexTypes.h"
namespace DW
{
	class Sprite : public Node
	{
	public:
		friend class Render;

		Sprite();

		~Sprite();

		void Draw();

		static std::shared_ptr<Sprite> Create();

		static std::shared_ptr<Sprite> Create(const std::string& png_file_path);

		static std::shared_ptr<Sprite> Create(std::shared_ptr<Texture> texture);

		void SetSpriteFrame(std::shared_ptr<SpriteFrame> sf);

	private:
		std::shared_ptr<SpriteFrame>			sprite_frame_;

		VertexPositionColorTexture				vertices_[4];

		static const uint16_t					indices_[6];

		Microsoft::WRL::ComPtr<ID3D11Buffer>	vertex_buffer_;

		Microsoft::WRL::ComPtr<ID3D11Buffer>	index_buffer_;

		Microsoft::WRL::ComPtr<ID3D11InputLayout> inputlayout_;
	private:
		void InitVertices();

		void InitGraphics();
	};
}

