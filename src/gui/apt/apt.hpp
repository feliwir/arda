#pragma once
#include <memory>

namespace arda
{
	class IStream;
	class Constant;

    class Apt 
    {
	public:
		Apt(std::shared_ptr<IStream> stream, std::shared_ptr<Constant> data);
		~Apt();
	private:
		std::shared_ptr<IStream> m_stream;
    };
}