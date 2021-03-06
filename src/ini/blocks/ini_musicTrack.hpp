#pragma once
#include "../parser/block.hpp"

namespace arda::ini
{
	class MusicTrack final : public Block
	{
	public:
		MusicTrack();

		// Inherited via Block
		virtual void SetProperty(std::string_view name, const Property value) override;

		inline std::string& GetFilename() { return m_filename; }
		inline unsigned int GetVolume() { return m_volume; }

		// Geerbt �ber Block
		virtual void Register(Ini & ini, std::string_view name) override;
	private:
		std::string m_filename;
		std::string m_control;
		std::string m_type;
		std::string m_submixSlider;
		unsigned int m_reverbEffektLevel;
		unsigned int m_dryLevel;
		unsigned int m_volume;

		static int m_defaultVolume;
	};
}