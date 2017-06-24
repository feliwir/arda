#pragma once
#include <string>
#include <vector>
#include <stdint.h>

namespace arda
{
    class Config
    {
	public:
		Config(const std::vector<std::string>& args);
		~Config();

		void Update();

		inline void setTitle(const std::string& title) { m_title = title; }
		inline const std::string& getTitle() const { return m_title; }

		inline void setWidth(const uint32_t width) { m_width = width; }
		inline const uint32_t getWidth() const { return m_width; }

		inline void setHeight(const uint32_t height) { m_height = height; }
		inline const uint32_t getHeight() const	{ return m_height; }

		inline void setRoot(const std::string& root) { m_root = root; }
		inline const std::string& getRootDir() const { return m_root; }

		inline void setIniDir(const std::string& dir) { m_iniDir = dir; }
		inline const std::string& getIniDir() const { return m_iniDir; }


	private:
		bool m_debug;
		std::string m_title;
		std::string m_root;
		std::string m_iniDir;
		uint32_t m_width;
		uint32_t m_height;

    };
}