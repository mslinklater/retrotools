// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <GL/gl.h>

#include "../shared_cpp/windowbase.h"
#include "../shared_cpp/istateserialisation.h"
#include "../components/tia.h"

class Tia;

class TiaWindow : public WindowBase, public IStateSerialisation
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

private:
    Tia* pTia;

	bool bShowInfo;
	bool bShowRegisters;

	void DrawWriteRegister(uint32_t reg, const char* text, uint8_t val);
	void DrawReadRegister(uint32_t reg, const char* text, uint8_t val);

	// render stuff

	GLuint videoOutputTexture;
	static const int kOutputBufferSize = Tia::kOutputHorizontalResolution * Tia::kOutputVerticalResolution * 4;
	uint8_t outputBuffer[kOutputBufferSize];
};
