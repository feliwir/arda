#include "debugger.hpp"
#include "config.hpp"
#include "application.hpp"
#include "global.hpp"
#include <boost/bind.hpp>
#include <iostream>

arda::Debugger::Debugger(Config& c) :
	m_socket(m_service),
	m_acceptor(m_service),
	m_connected(false)
{ 
	uint16_t port = c.GetDebugPort();
	std::string host = "127.0.0.1";//localhost

	m_endpoint = boost::asio::ip::tcp::endpoint(ip::address::from_string(host), port);
	
	//first see if we can find anything on port
	m_socket.async_connect(m_endpoint, boost::bind(&Debugger::HandleConnect,this,placeholders::error));
	
	//have a seperate thread running the events:
	m_thread = std::thread(boost::bind(&io_service::run, &m_service));
}

arda::Debugger::~Debugger()
{
	m_service.stop();
	m_thread.join();
}

void arda::Debugger::Log(std::string_view msg) const
{
	auto ms = GetGlobal().TimeFromStart<std::chrono::milliseconds>();
	std::cout<<"["<< ms/1000.0<<"s] " << msg << std::endl;
}

void arda::Debugger::HandleConnect(const boost::system::error_code & error)
{
	if (error.value() == boost::system::errc::success)
	{
		Log("Connected to remote debugger");
		m_connected = true;
	}		
	else
		Log("Failed to connect to remote debugger: " + error.message());	

}

void arda::Debugger::HandleWrite(const boost::system::error_code & error)
{
}

void arda::Debugger::HandleRead(const boost::system::error_code & error)
{
}
