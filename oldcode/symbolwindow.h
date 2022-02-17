// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#if 0

#pragma once
#include <string>
#include "../symbolstore.h"
#include "system/window/windowbase.h"

class SymbolWindow : public WindowBase
{
public:
	SymbolWindow();
	virtual ~SymbolWindow();
	
	virtual void Draw(void);
	
	void SetSymbolStore(SymbolStore* p);
private:
	SymbolStore* pStore;
	bool editingSymbol;
	Symbol symbolBeingEdited;
	
	uint32_t	filterFlags;
	
	bool		filterMemoryRead;
	bool		filterMemoryWrite;
	bool		filterAddressLabel;
	bool		filterHardware;
	bool		filterUser;
	bool		filterAuto;
	bool		filterJumpLabel;
	bool		filterBranchLabel;	
};

#endif