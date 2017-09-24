#pragma once
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <mutex>
#include "blocks/all.hpp"

namespace arda
{
	class Block;
	class Config;
	class FileSystem;
	class IEntry;
	class ParsingContext;

	class Ini
	{
	public:
		Ini(Config& config,FileSystem& fs);
		~Ini();

		inline void RegisterVideo(std::shared_ptr<Block> vid, std::string_view name)
		{
			m_videos.emplace(std::string(name), vid);
		}

		template<class T>
		inline std::shared_ptr<T> GetBlock(std::string_view name = "");

		template<>
		inline std::shared_ptr<ini::Video> GetBlock(std::string_view name)
		{
			return std::dynamic_pointer_cast<ini::Video>(m_videos[std::string(name)]);
		}

		template<>
		inline std::shared_ptr<ini::Weapon> GetBlock(std::string_view name)
		{
			return std::dynamic_pointer_cast<ini::Weapon>(m_videos[std::string(name)]);
		}

		inline std::vector<std::string>& GetGlobalIncludes() { return m_globalIncludes; }
		std::shared_ptr<ParsingContext> GetContext(const std::string& path,bool load=true);
	private:
		FileSystem& m_fs;
		std::mutex m_access;
		std::vector<std::string> m_globalIncludes;
		std::map<const std::string, std::shared_ptr<ParsingContext>> m_files;
		std::map<const std::string, std::shared_ptr<Block>> m_videos;
		std::map<const std::string, std::shared_ptr<Block>> m_weapons;
	};
}