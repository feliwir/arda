#pragma once
#include <thread>
#include <string_view>
#include <boost/asio.hpp>
#include "application.hpp"
#include "global.hpp"
using namespace boost::asio;

namespace arda
{
	class Config;

	class Debugger
	{
	public:
		Debugger(Config& c);
		~Debugger();

		void Log(std::string_view msg) const;
	private:
		void HandleConnect(const boost::system::error_code& error);
		void HandleWrite(const boost::system::error_code& error);
		void HandleRead(const boost::system::error_code& error);

	private:
		bool m_connected;
		io_service m_service;
		std::thread m_thread;
		ip::tcp::endpoint m_endpoint;
		ip::tcp::socket m_socket;
		ip::tcp::acceptor m_acceptor;
	};

	inline void ARDA_LOG(std::string_view msg)
	{
		GetGlobal().GetDebugger()->Log(msg);
	}
}