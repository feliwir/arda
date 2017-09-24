#pragma once
#include <variant>
#include <memory>
#include <string_view>
#include "property.hpp"

namespace arda
{
	class Ini;

    class Block : public std::enable_shared_from_this<Block>
    {
	public:
		virtual void SetProperty(std::string_view name,const Property value) = 0;

		virtual void Register(Ini& ini,std::string_view name="") = 0;

		template<class T>
		static inline std::shared_ptr<T> Create()
		{
			return std::make_shared<T>();
		}
    };
}