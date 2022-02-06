// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#if RUN_TESTS
#define CATCH_CONFIG_RUNNER
#include "shared_cpp/catch.hpp"
#endif

#include "system/application.h"

int main(int argc, char* argv[])
{
	Application::Instance()->Init(argc, argv);

#if RUN_TESTS
	int result = Catch::Session().run(argc, argv);
	if(result != 0)
	{
		LOGINFO("Unit tests failed...");
		return result;
	}
#endif

	Application::Instance()->UpdateLoop();

	return Application::Instance()->Close();
}
