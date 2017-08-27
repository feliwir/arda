#include <vector>
#include <string>
#include <memory>
#include <stdint.h>

struct AVIOContext;
struct AVFormatContext;

namespace arda
{
	class IStream;

    class AvStream
    {
	public:
		AvStream(std::shared_ptr<IStream> stream);
		~AvStream();

		inline AVIOContext* GetContext()
		{
			return m_ctx;
		}

		void Attach(AVFormatContext* ctx);

		static int64_t SeekFunc(void* ptr, int64_t pos, int whence);
		static int ReadFunc(void* ptr, uint8_t* buf, int buf_size);
	private:
		std::shared_ptr<IStream> m_stream;
		AVIOContext* m_ctx;
		uint8_t* m_buffer;
		const int m_bufSize = 32 * 1024;
    };
}