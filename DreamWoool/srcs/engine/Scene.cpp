#include <pch.h>
#include "Scene.h"

namespace DW
{
	Scene::Scene(const std::string& name):
		Node(name)
	{
	}
	Scene::~Scene()
	{
	}
	std::shared_ptr<Scene> Scene::Create(const std::string& name)
	{
		return std::shared_ptr<Scene>(new Scene(name));
	}
	void Scene::Render()
	{
		VisitForDraw(transform_dirty_);
	}
	void Scene::UpdateScene(float t)
	{
		VisitForUpdate(t);
	}
}

