#pragma once
#include "../parser/block.hpp"

namespace arda::ini
{
	class Weapon final : public Block
	{
		// Inherited via Block
		virtual void SetProperty(std::string_view name,const Property value) override;
	};
}