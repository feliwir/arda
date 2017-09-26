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

		inline void RegisterSpeech(std::shared_ptr<Block> speech, std::string_view name)
		{
			m_speeches.emplace(std::string(name), speech);
		}

		inline void RegisterMusic(std::shared_ptr<Block> music, std::string_view name)
		{
			m_music.emplace(std::string(name), music);
		}

		inline void RegisterVideo(std::shared_ptr<Block> vid, std::string_view name)
		{
			m_videos.emplace(std::string(name), vid);
		}

		template<class T>
		inline std::shared_ptr<T> GetBlock(std::string_view name = "");

		inline std::vector<std::string>& GetGlobalIncludes() { return m_globalIncludes; }
		std::shared_ptr<ParsingContext> GetContext(const std::string& path,bool load=true);
	private:
		FileSystem& m_fs;
		std::mutex m_access;
		std::vector<std::string> m_globalIncludes;
		std::map<const std::string, std::shared_ptr<ParsingContext>> m_files;
		std::map<const std::string, std::shared_ptr<Block>> m_videos;
		std::map<const std::string, std::shared_ptr<Block>> m_speeches;
		std::map<const std::string, std::shared_ptr<Block>> m_music;
		std::map<const std::string, std::shared_ptr<Block>> m_weapons;
	};

	template<>
	inline std::shared_ptr<ini::Speech> Ini::GetBlock(std::string_view name)
	{
		return std::dynamic_pointer_cast<ini::Speech>(m_speeches[std::string(name)]);
	}

	template<>
	inline std::shared_ptr<ini::Video> Ini::GetBlock(std::string_view name)
	{
		return std::dynamic_pointer_cast<ini::Video>(m_videos[std::string(name)]);
	}

	template<>
	inline std::shared_ptr<ini::Weapon> Ini::GetBlock(std::string_view name)
	{
		return std::dynamic_pointer_cast<ini::Weapon>(m_weapons[std::string(name)]);
	}

}