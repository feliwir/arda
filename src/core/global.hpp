#pragma once
#include <chrono>
#include "debugger.hpp"

namespace arda
{
	typedef std::chrono::high_resolution_clock clock;

	class Config;
	class Global
	{
	public:
		Global(Config& c);

		template<class T>
		long long TimeFromStart()
		{
			auto current_time = clock::now();
			return std::chrono::duration_cast<T>(current_time - m_start).count();
		}

	private:
		clock::time_point m_start;
		Debugger m_debugger;
	};
}