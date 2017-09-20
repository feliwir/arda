#include "image.hpp"
#include "../filesystem/stream.hpp"
#include "../filesystem/util.hpp"
#include "../core/debugger.hpp"
#include <jpeglib.h>
#include <gli/target.hpp>
#include <gli/format.hpp>
#include <gli/core/storage_linear.hpp>
#include <glm/glm.hpp>

arda::Image::Image()
{
}

arda::Image::Image(std::shared_ptr<IStream> stream) : Image()
{
	Load(stream);
}

arda::Image::~Image()
{
}

inline bool arda::Image::Load(std::shared_ptr<IStream> stream)
{
	gli::target target;
	gli::format format;

	int width, height, bpp;

	int size = stream->GetSize();

	uint8_t* buffer = new uint8_t[size];

	stream->Read(reinterpret_cast<char*>(buffer), size);
	
	Magic magic = GetMagic(buffer);

	switch (magic)
	{
	case JPEG:
		{
		//2D texture for sure
		target = gli::TARGET_2D;

		int row_stride;
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

		width = cinfo.output_width;
		height = cinfo.output_height;
		bpp = cinfo.output_components*8;
		size = width*height*cinfo.output_components;
		
		switch (cinfo.output_components)
		{
		case 3:
			format = gli::FORMAT_RGB8_UNORM_PACK8;
			break;
		case 4:
			format = gli::FORMAT_RGBA8_UNORM_PACK8;
			break;
		}

		m_img = gli::texture(target, format ,gli::texture::extent_type(width, height, 1), 1, 1, 1);

		uint8_t* buffer = reinterpret_cast<uint8_t*>(m_img.data(0, 0, 0));

		// JSAMPLEs per row in output buffer 
		row_stride = cinfo.output_width * cinfo.output_components;

		
		while (cinfo.output_scanline < cinfo.output_height) {
			unsigned char *buffer_array[1];
			buffer_array[0] = buffer + \
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
	case DDS:
		m_img = gli::load(reinterpret_cast<const char*>(buffer), size);
		break;
	default:
		ARDA_LOG("Unknown texture format!");
		break;
	}
	
	delete[] buffer;
	return false;
}

arda::Image::Magic arda::Image::GetMagic(uint8_t * m)
{
	//JPEG magic
	if (util::CheckFourCC<uint8_t, 3>(m, { { 0xFF, 0xD8,0xFF} }))
		return JPEG;

	//DDS magic
	if (util::CheckFourCC<char>(m, { { 'D','D','S',' '} }))
		return DDS;

	return Magic();
}
