#pragma once

namespace arda
{
	class Image;

	class ITexture
	{
	public:
		ITexture(int width,int height);
		ITexture(Image& img);

		virtual void Update(Image& img) = 0;
		virtual void Bind(int i) = 0;
	protected:
		virtual void Generate() = 0;
	protected:
		int m_width;
		int m_height;
	};
}