#pragma once
#include <chrono>

namespace arda
{
	typedef std::chrono::high_resolution_clock clock;

	class Global
	{
	public:
		Global()
		{
			m_start = clock::now();
		}

		template<class T>
		long long TimeFromStart()
		{
			auto current_time = clock::now();
			return std::chrono::duration_cast<T>(current_time - m_start).count();
		}

	private:
		clock::time_point m_start;
	};
}