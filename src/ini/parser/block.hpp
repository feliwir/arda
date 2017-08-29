#pragma once
#include <variant>
#include <memory>
#include "property.hpp"

namespace arda
{
    class Block
    {
	public:
		virtual void SetProperty(const std::string& name,const Property value) = 0;

		template<class T>
		static inline std::shared_ptr<T> Create()
		{
			return std::make_shared<T>();
		}
    };
}