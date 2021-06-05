// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <GL/gl.h>

#include "windowbase.h"
#include "istateserialisation.h"
#include "icommandprocessor.h"
#include "components//stella/tia.h"

class Tia;

class TiaWindow : public WindowBase
				, public IStateSerialisation
				, public ICommandProcessor
{
public:
	TiaWindow();
	virtual ~TiaWindow();
	
	virtual void Draw(void);

    void SetTia(Tia* tia);

	// IStateSerialisation
	virtual void SerialiseState(json& object);
	virtual void DeserialiseState(json& object);
	// ~IStateSerialisation

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
