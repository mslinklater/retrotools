// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#if 0
#pragma once

#include <GL/gl.h>

#include "system/window/windowbase.h"
#include "system/istateserialisation.h"
#include "system/icommandhandler.h"
#include "components//stella/tia.h"

class Tia;

class TiaWindow : public WindowBase
				, public IStateSerialisation
				, public ICommandHandler
{
public:
	TiaWindow();
	virtual ~TiaWindow();
	
	virtual void Draw(void);

    void SetTia(Tia* tia);

	ISERIALISATION_API

	// ICommandProcessor
	virtual bool HandleCommand(const std::shared_ptr<CommandBase> command);
	// ~ICommandProcessor

private:
    Tia* pTia;

	bool bShowInfo;
	bool bShowRegisters;
	bool bShowVBlank;
	bool bShowHBlank;
	bool bShowTV;
	bool bShowLocation;
	bool bLockPixels;
	bool bShowPF;
	bool bShowP0;
	bool bShowP1;
	bool bShowCPU;

	static const int kRasterCountHistorySize = 100;
	float rasterCountHistory[kRasterCountHistorySize];
	uint32_t	rasterCountOffset;

	void DrawWriteRegister(uint32_t reg, const char* text, uint8_t val);
	void DrawReadRegister(uint32_t reg, const char* text, uint8_t val);

	void DrawInfo();
	void DrawRegisters();

	// render stuff

	GLuint videoOutputTexture;
	static const int kOutputBufferSize = Tia::kOutputHorizontalResolution * Tia::kOutputVerticalResolution * 4;
	uint8_t outputBuffer[kOutputBufferSize];
};
#endif
