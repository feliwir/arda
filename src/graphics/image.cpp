#include "image.hpp"
#include "../filesystem/stream.hpp"
#include <jpeglib.h>

arda::Image::Image() :
m_width(0),
m_height(0),
m_format(RGBA),
m_buffer(nullptr)
{
}

arda::Image::Image(std::shared_ptr<IStream> stream) : Image()
{
	Load(stream);
}

arda::Image::~Image()
{
	m_width = m_height = 0;
	if (m_buffer)
		delete[] m_buffer;
}

inline bool arda::Image::Load(std::shared_ptr<IStream> stream)
{
	int size = stream->getSize();

	uint8_t* buffer = new uint8_t[size];

	stream->read(reinterpret_cast<char*>(buffer), size);
	
	Magic magic = GetMagic(buffer);

	switch (magic)
	{
	case JPEG:
		{
		JSAMPARRAY row_buffer;		/* Output row buffer */
		int row_stride;		/* physical row width in output buffer */
		int rc;
		jpeg_decompress_struct cinfo;
		jpeg_error_mgr jerr;

		//create decompression struct
		cinfo.err = jpeg_std_error(&jerr);
		jpeg_create_decompress(&cinfo);
		//set memory
		jpeg_mem_src(&cinfo, buffer, size);
		//read the header
		rc = jpeg_read_header(&cinfo, TRUE);
		//start decompression
		jpeg_start_decompress(&cinfo);

		m_width = cinfo.output_width;
		m_height = cinfo.output_height;
		m_bpp = cinfo.output_components*8;
		m_size = m_width*m_height*cinfo.output_components;
		m_buffer = new uint8_t[m_size];

		switch (cinfo.output_components)
		{
		case 3:
			m_format = RGB;
		case 4:
			m_format = RGBA;
		}

		// JSAMPLEs per row in output buffer 
		row_stride = cinfo.output_width * cinfo.output_components;

		
		while (cinfo.output_scanline < cinfo.output_height) {
			unsigned char *buffer_array[1];
			buffer_array[0] = m_buffer + \
				(cinfo.output_scanline) * row_stride;

			/* jpeg_read_scanlines expects an array of pointers to scanlines.
			* Here the array is only one element long, but you could ask for
			* more than one scanline at a time if that's more convenient.
			*/
			(void)jpeg_read_scanlines(&cinfo, buffer_array, 1);

		}

		/* Step 7: Finish decompression */

		(void)jpeg_finish_decompress(&cinfo);
		/* We can ignore the return value since suspension is not possible
		* with the stdio data source.
		*/

		/* Step 8: Release JPEG decompression object */

		/* This is an important step since it will release a good deal of memory. */
		jpeg_destroy_decompress(&cinfo);
		}
		break;
	}
	
	delete[] buffer;
	return false;
}

arda::Image::Magic arda::Image::GetMagic(uint8_t * m)
{
	//JPEG magic
	if ((m[0] == 0xFF) | (m[1] == 0xD8) | (m[2] == 0xFF))
		return JPEG;

	return Magic();
}
