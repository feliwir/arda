#pragma once
#include <string>
#include <map>
#include <boost/variant.hpp>

namespace arda
{
	class Template
	{
	public:
		enum Type
		{
			WEAPON	= 0,
			OBJECT	= 1
		};
	public:
		virtual void SetProperty(const std::string& name,const std::string& value);

		template<typename T>
		inline T GetProperty(const std::string& name)
		{
			return boost::get<T>(m_properties[name]);
		}

		static std::shared_ptr<Template> Create(const std::string& type);

		inline Type GetType()
		{
			return m_type;
		}

	protected:
		template<class T>
		inline void CreateProperty(const std::string& prop)
		{
			m_properties[prop] = T();
		}
	protected:
		std::map<std::string, boost::variant<int, float, std::string,bool>> m_properties;
		Type m_type;
	private:	
		static std::map<std::string, Type> m_types;
	};
}