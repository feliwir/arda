#pragma once
#include "../parser/block.hpp"

namespace arda::ini
{
	class Music final : public Block
	{
	public:
		Music();

		// Inherited via Block
		virtual void SetProperty(std::string_view name, const Property value) override;

		inline std::string& GetFilename() { return m_filename; }
		inline unsigned int GetVolume() { return m_volume; }

		// Geerbt über Block
		virtual void Register(Ini & ini, std::string_view name) override;
	private:
		std::string m_filename;
		std::string m_control;
		std::string m_type;
		unsigned int m_volume;

		static int m_defaultVolume;
	};
}