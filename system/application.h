// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

class Application
{
public:
	static Application *Instance();

	void Init(int argc, char* argv[]);
	void UpdateLoop();
	int Close();

private:
	Application();
	~Application();
};
