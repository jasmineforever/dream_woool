#pragma once
#include "Node.h"
namespace DW
{
	class Scene : public Node
	{
	public:
		Scene(const std::string& name);

		virtual ~Scene();

		static std::shared_ptr<Scene> Create(const std::string& name = "");

		void Render();

		void UpdateScene(float t);

	private:
		Scene(Scene&) = delete;

		Scene& operator=(const Scene&) = delete;

	};
}


