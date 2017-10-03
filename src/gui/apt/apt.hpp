#pragma once
#include <memory>

namespace arda
{
	class IStream;

    class Apt 
    {
	public:
		Apt(std::shared_ptr<IStream> stream);
		~Apt();
    };
}