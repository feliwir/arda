#pragma once
#include "../parser/block.hpp"

namespace arda::ini
{
	class Weapon final : public Block
	{
	public:
		// Inherited via Block
		virtual void SetProperty(std::string_view name,const Property value) override;

		// Geerbt über Block
		virtual void Register(Ini & ini, std::string_view name = "") override;
	};
}