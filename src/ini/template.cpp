#include "template.hpp"
#include "weapon.hpp"
#include <iostream>

std::map<std::string,arda::Template::Type> arda::Template::m_types =
{
	{"Weapon", Template::WEAPON},
	{"Object", Template::OBJECT}
};

void arda::Template::SetProperty(const std::string & name, const std::string & value)
{
	auto it = m_properties.find(name);
	
	try
	{
		if (it != m_properties.end())
		{
			switch (it->second.which())
			{
				//INT 
			case 0:
				m_properties[name] = std::stoi(value);
				break;
				//FLOAT
			case 1:
				m_properties[name] = std::stof(value);
				break;
				//STRING
			case 2:
				m_properties[name] = value;
				break;
				//BOOLEAN
			case 3:
				m_properties[name] = (value == "Yes") ? true : false;
				break;
			}
		}
		else
		{
			std::cout << "Unknown property: " << name << std::endl;
		}
	}
	catch (...)
	{
		std::cout << "Invalid argument!" << std::endl;
	}
}

std::shared_ptr<arda::Template> arda::Template::Create(const std::string & type)
{
	std::shared_ptr<Template> ptr = nullptr;
	auto it = m_types.find(type);
	
	if (it != m_types.end())
	{
		switch (it->second)
		{
		case WEAPON:
			ptr = std::make_shared<Weapon>();
			break;
		default:
			ptr = nullptr;
		}

		if(ptr)
			ptr->m_type = it->second;
	}

	return ptr;
}
