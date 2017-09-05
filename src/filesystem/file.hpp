#pragma once
#include <memory>
#include "entry.hpp"

namespace arda
{
	class IStream;

	class File final : public IEntry
	{
	public:
		File(std::shared_ptr<IStream> stream);

		inline std::shared_ptr<IStream> getStream() const
		{
			return m_stream;
		}

	protected:
		std::shared_ptr<IStream>  m_stream;
	};
}