#include "pch.h"
#include "MirMap.h"
#include "TextureCache.h"
namespace DW
{
	MirMap::MirMap():
		Node("MirMap")
	{

	}
	MirMap::~MirMap()
	{

	}
	std::shared_ptr<MirMap> MirMap::Create()
	{
		auto ret = std::make_shared<MirMap>();
		if (ret->Init())
		{
			return ret;
		}
		return nullptr;
	}

	bool MirMap::Init()
	{
		return true;
	}
}