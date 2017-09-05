#pragma once
#include "../parser/block.hpp"

namespace arda
{
	class Weapon final : public Block
	{
		// Inherited via Block
		virtual void SetProperty(const std::string& name,const Property value) override;
	};
}