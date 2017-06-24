#include "file.hpp"

arda::File::File(std::shared_ptr<IStream> stream) : IEntry(REGULAR), m_stream(stream)
{
}
