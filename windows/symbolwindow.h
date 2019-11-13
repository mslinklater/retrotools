// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

class SymbolStore;

class SymbolWindow
{
public:
	SymbolWindow();
	virtual ~SymbolWindow();
	
	void Draw(bool* pShow);
	void SetSymbolStore(SymbolStore* p);
private:
	SymbolStore* pStore;
};
