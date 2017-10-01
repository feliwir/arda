#include "global.hpp"
#include "debugger.hpp"
extern "C"
{
#include <libavformat/avformat.h>
}

arda::Global::Global(Config & c) : m_debugger(std::make_shared<Debugger>(c))
{
	m_start = clock::now();
	//register all LIBAV codecs & formats here
	av_register_all();
}

arda::Global::~Global()
{
	m_pool.JoinAll();
}
