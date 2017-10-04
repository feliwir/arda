#include "constant.hpp"
#include "../../filesystem/memorystream.hpp"
#include "../../filesystem/util.hpp"
#include "../../core/debugger.hpp"
#include "../../core/exception.hpp"

arda::Constant::Constant(std::shared_ptr<IStream> stream)
{
	auto memStream = std::make_shared<MemoryStream>(stream);
	uint8_t* buffer = memStream->GetBuffer();

	//read magic
	std::string magic(17,'\0');
	memStream->Read(magic.data(), 17);

	if (magic != "Apt constant file")
		throw RuntimeException("Invalid constant file!");

	memStream->Seek(3, IStream::CURRENT);
	
	m_aptOffset = util::Read<uint32_t>(memStream);
	m_itemCount = util::Read<uint32_t>(memStream);
	memStream->Seek(4, IStream::CURRENT);

	for (uint32_t i = 0; i < m_itemCount; ++i)
	{
		Entry e;
		e.Type = static_cast<Type>(util::Read<uint32_t>(memStream));
		uint32_t value = util::Read<uint32_t>(memStream);

		switch (e.Type)
		{
		case STRING:
			e.StringValue = std::string(reinterpret_cast<const char*>(buffer + value));
			break;
		case NUMBER:
			e.NumberValue = value;
			break;
		case UNDEF:
			ARDA_LOG("Undefined constant entry at position " + std::to_string(i));
			break;
		default:
			break;
		}

		m_items.push_back(e);
	}
}

arda::Constant::Entry::Entry()
{
	Type = UNDEF;
	NumberValue = 0;
}
