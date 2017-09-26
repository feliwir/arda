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

		inline void SetTitle(const std::string& title) { m_title = title; }
		inline const std::string& GetTitle() const { return m_title; }

		inline void SetWidth(const uint32_t width) { m_width = width; }
		inline const uint32_t GetWidth() const { return m_width; }

		inline void SetHeight(const uint32_t height) { m_height = height; }
		inline const uint32_t GetHeight() const	{ return m_height; }

		inline void SetRoot(const std::string& root) { m_root = root; }
		inline const std::string& GetRootDir() const { return m_root; }

		inline void SetIniDir(const std::string& dir) { m_iniDir = dir; }
		inline const std::string& GetIniDir() const { return m_iniDir; }

		inline void SetIniPreprocessor(const bool pp) { m_iniPp = pp; }
		inline const bool GetIniPreprocessor() const { return m_iniPp; }

		inline void SetDebugPort(const uint16_t port) { m_debugPort = port; }
		inline uint16_t GetDebugPort() const { return m_debugPort; }

		inline bool IsDebug() const { return m_debug; }

		inline int SetGraphicsAPI(const int api) { m_gfxApi = api; }
		inline int GetGraphicsAPI() const { return m_gfxApi; }
	private:
		bool m_debug;
		int m_gfxApi;
		uint16_t m_debugPort;
		std::string m_title;
		std::string m_root;
		std::string m_iniDir;
		uint32_t m_width;
		uint32_t m_height;
		bool m_iniPp;


    };
}