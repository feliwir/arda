#pragma once
#include <memory>

namespace arda
{
	class IEntry : public std::enable_shared_from_this<IEntry>
	{
	public:
		enum Type
		{
			DIRECTORY	= 0,
			REGULAR		= 1
		};

		inline IEntry(Type t) : m_type(t)
		{}

		inline Type getType() const { return m_type; }

		inline static bool isRegular(const IEntry& e)
		{
			return e.getType() == REGULAR;
		}

		inline static bool isDirectory(const IEntry& e)
		{
			return e.getType() == DIRECTORY;
		}
	protected:
		Type m_type;
	};
}