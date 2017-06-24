#pragma once

namespace arda
{
	class Config;
	class FileSystem;
	class IEntry;

	class Ini
	{
	public:
		Ini(Config& config,FileSystem& fs);
		~Ini();
	};
}