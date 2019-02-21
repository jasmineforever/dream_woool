#pragma once
#include "Node.h"
namespace DW
{
	class MirMap : public Node
	{
	public:
		MirMap();

		~MirMap();

		static std::shared_ptr<MirMap> Create();
		
	private:
		bool Init();
	private:
	
	};
}