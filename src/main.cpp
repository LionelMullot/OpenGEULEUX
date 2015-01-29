
#include <iostream>

#include "Core/App.h"

int main( int argc, char** argv ) 
{
	core::App::init_app(argc, argv);
	if (core::App::run_app() == 0)
	{
		core::App::exit_app();
	}
}
