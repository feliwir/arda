#include "application.hpp"
#include "exception.hpp"
#include <vector>
#include <iostream>

int main(int argc,char**argv)
{
	std::vector<std::string> args(argv , argv + argc);
	try
	{
		arda::Application app(args);
		app.Run();
	}
	catch (arda::RuntimeException& e)
	{
		std::cout << e.what() << std::endl;
		system("pause");
		return -1;
	}

	return 0;
}