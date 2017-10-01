#pragma once
#include "../parser/block.hpp"

namespace arda::ini
{
	class DialogEvent final : public Block
	{
	public:
		DialogEvent();

		// Inherited via Block
		virtual void SetProperty(std::string_view name, const Property value) override;

		inline std::string& GetFilename() { return m_filename; }
		inline unsigned int GetVolume() { return m_volume; }
		inline std::string& GetSubmixSlider() { return m_submixSlider; }

		// Geerbt über Block
		virtual void Register(Ini & ini, std::string_view name) override;
	private:
		std::string m_filename;
		std::string m_comment;
		std::string m_submixSlider;
		unsigned int m_volume;

		static int m_defaultVolume;
	};
}