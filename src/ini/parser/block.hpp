#pragma once
#include <variant>

namespace arda
{
    class Block
    {
	public:
		virtual void SetProperty(std::variant<int,float> prop) = 0;
    };
}