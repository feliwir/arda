#pragma once
#include <chrono>
#include <memory>
#include "../threading/pool.hpp"

namespace arda
{
	typedef std::chrono::high_resolution_clock clock;

	class Debugger;
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

		std::shared_ptr<Debugger> GetDebugger()
		{
			return m_debugger;
		}

	private:
		concurrent::ThreadPool<8> m_pool;
		clock::time_point m_start;
		std::shared_ptr<Debugger> m_debugger;
	};
}