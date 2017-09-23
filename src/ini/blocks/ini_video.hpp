#pragma once
#include "../parser/block.hpp"

namespace arda::ini
{
	class Video final : public Block
	{
	public:
		Video();

		// Inherited via Block
		virtual void SetProperty(std::string_view name, const Property value) override;

		inline std::string& GetFilename() { return m_filename; }
		inline unsigned int GetVolume() { return m_volume; }
	private:
		std::string m_filename;
		std::string m_comment;
		unsigned int m_volume;
		bool m_isDefault;

		static int m_defaultVolume;
	};
}