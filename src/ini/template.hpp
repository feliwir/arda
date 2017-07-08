#pragma once
#include <string>
#include <map>
#include <boost/variant.hpp>

namespace arda
{
	class Template
	{
	private:
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

		inline void IntProperty(const std::string& prop)	{ m_properties[prop] = int(); }
		inline void FloatProperty(const std::string& prop)	{ m_properties[prop] = float(); }
		inline void StringProperty(const std::string& prop) { m_properties[prop] = std::string(); }
		inline void BoolProperty(const std::string& prop)	{ m_properties[prop] = bool(); }
	protected:
		std::map<std::string, boost::variant<int, float, std::string,bool>> m_properties;
	private:
		static std::map<std::string, Type> m_types;
	};
}