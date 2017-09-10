#pragma once
#include <chrono>
#include "debugger.hpp"
#include "../threading/pool.hpp"

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


		concurrent::ThreadPool<8>& GetThreadPool()
		{
			return m_pool;
		}

	private:
		concurrent::ThreadPool<8> m_pool;
		clock::time_point m_start;
		Debugger m_debugger;
	};
}