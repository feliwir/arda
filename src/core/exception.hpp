#pragma once
#include <stdexcept>
#include "application.hpp"
#include "global.hpp"

namespace arda
{
	class RuntimeException : public std::runtime_error
	{
	public:
		RuntimeException(std::string error) : std::runtime_error(std::string(
			"["+
			std::to_string(GetGlobal().TimeFromStart<std::chrono::milliseconds>())+
			"]: "+error).c_str())
		{

		}
	};
}