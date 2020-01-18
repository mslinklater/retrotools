// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "tiawindow.h"
#include "../imgui/imgui.h"
#include "../shared_cpp/log.h"

TiaWindow::TiaWindow()
: bShowInfo(false)
, bShowRegisters(false)
, bShowVBlank(false)
, bShowHBlank(false)
, bShowLocation(false)
{
	// create output buffer
	glGenTextures(1, &videoOutputTexture);
	glBindTexture(GL_TEXTURE_2D, videoOutputTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

//	for(int i=0 ; i<kOutputBufferSize ; i++)
//	{
//		outputBuffer[i] = i & 255;
//	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Tia::kOutputHorizontalResolution, Tia::kOutputVerticalResolution, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)&outputBuffer);
}

TiaWindow::~TiaWindow()
{
	// free output buffer
	glDeleteTextures(1, &videoOutputTexture);
}

void TiaWindow::DrawWriteRegister(uint32_t reg, const char* text, uint8_t val)
{
	bool temp = pTia->GetWriteBreakpoint(reg);
	bool backup = temp;
	ImGui::Checkbox(text, &temp);
	if(temp != backup)
	{
		pTia->SetWriteBreakpoint(reg, temp);
	}
	ImGui::SameLine();
    ImGui::Text(":0x%02x", val);
}

void TiaWindow::DrawReadRegister(uint32_t reg, const char* text, uint8_t val)
{
	bool temp = pTia->GetReadBreakpoint(reg);
	bool backup = temp;
	ImGui::Checkbox(text, &temp);
	if(temp != backup)
	{
		pTia->SetReadBreakpoint(reg, temp);
	}
	ImGui::SameLine();
    ImGui::Text(":0x%02x", val);
}

void TiaWindow::Draw(void)
{
	ImGui::Checkbox("Info", &bShowInfo);
	ImGui::SameLine();
	ImGui::Checkbox("Registers", &bShowRegisters);

	if(bShowInfo)
	{
		ImGui::Separator();
		ImGui::Text("Ticks: %" PRId64, pTia->GetTicksSinceBoot());
		ImGui::Text("RasterX: %d", pTia->GetRasterX());
		ImGui::Text("RasterY: %d", pTia->GetRasterY());
		ImGui::Text("Frame: %d", pTia->GetFrameNum());
	}

	if(bShowRegisters)
	{
		ImGui::Separator();
		ImGui::Text("Write registers");
		ImGui::Separator();
		ImGui::Columns(4, NULL, true);

		DrawWriteRegister(Tia::kVSYNC, " VSYNC", pTia->GetVSYNC());
		ImGui::NextColumn();
		DrawWriteRegister(Tia::kVBLANK, "VBLANK", pTia->GetVBLANK());
		ImGui::NextColumn();
		DrawWriteRegister(Tia::kWSYNC, " WSYNC", pTia->GetWSYNC());
		ImGui::NextColumn();
		DrawWriteRegister(Tia::kRSYNC, " RSYNC", pTia->GetRSYNC());

		ImGui::Columns(1);
		ImGui::Separator();

		ImGui::Columns(4, NULL, true);
		DrawWriteRegister(Tia::kNUSIZ0, "NUSIZ0", pTia->GetNUSIZ0());
		ImGui::NextColumn();
		DrawWriteRegister(Tia::kNUSIZ1, "NUSIZ1", pTia->GetNUSIZ1());
		ImGui::NextColumn();
		DrawWriteRegister(Tia::kCOLUP0, "COLUP0", pTia->GetCOLUP0());
		ImGui::NextColumn();
		DrawWriteRegister(Tia::kCOLUP1, "COLUP1", pTia->GetCOLUP1());

		ImGui::Columns(1);
		ImGui::Separator();

		ImGui::Columns(4, NULL, true);
		DrawWriteRegister(Tia::kCOLUPF, "COLUPF", pTia->GetCOLUPF());
		ImGui::NextColumn();
		DrawWriteRegister(Tia::kCOLUBK, "COLUBK", pTia->GetCOLUBK());
		ImGui::NextColumn();
		DrawWriteRegister(Tia::kREFP0, " REFP0", pTia->GetREFP0());
		ImGui::NextColumn();
		DrawWriteRegister(Tia::kREFP1, " REFP1", pTia->GetREFP1());
		ImGui::Columns(1);

		ImGui::Separator();
		ImGui::Columns(4, NULL, true);
		DrawWriteRegister(Tia::kPF0, "   PF0", pTia->GetPF0());
		ImGui::NextColumn();
		DrawWriteRegister(Tia::kPF1, "   PF1", pTia->GetPF1());
		ImGui::NextColumn();
		DrawWriteRegister(Tia::kPF2, "   PF2", pTia->GetPF2());
		ImGui::NextColumn();
		DrawWriteRegister(Tia::kRESP0, " RESP0", pTia->GetRESP0());
		ImGui::Columns(1);

		ImGui::Separator();
		ImGui::Columns(4, NULL, true);
		DrawWriteRegister(Tia::kRESP1, " RESP1", pTia->GetRESP1());
		ImGui::NextColumn();
		DrawWriteRegister(Tia::kRESM0, " RESM0", pTia->GetRESM0());
		ImGui::NextColumn();
		DrawWriteRegister(Tia::kRESM1, " RESM1", pTia->GetRESM1());
		ImGui::NextColumn();
		DrawWriteRegister(Tia::kRESBL, " RESBL", pTia->GetRESBL());
		ImGui::Columns(1);

		ImGui::Separator();
		ImGui::Columns(4, NULL, true);
		DrawWriteRegister(Tia::kAUDC0, " AUDC0", pTia->GetAUDC0());
		ImGui::NextColumn();
		DrawWriteRegister(Tia::kAUDC1, " AUDC1", pTia->GetAUDC1());
		ImGui::NextColumn();
		DrawWriteRegister(Tia::kAUDF0, " AUDF0", pTia->GetAUDF0());
		ImGui::NextColumn();
		DrawWriteRegister(Tia::kAUDF1, " AUDF1", pTia->GetAUDF1());
		ImGui::Columns(1);

		ImGui::Separator();
		ImGui::Columns(4, NULL, true);
		DrawWriteRegister(Tia::kAUDV0, " AUDV0", pTia->GetAUDV0());
		ImGui::NextColumn();
		DrawWriteRegister(Tia::kAUDV1, " AUDV1", pTia->GetAUDV1());
		ImGui::NextColumn();
		DrawWriteRegister(Tia::kGRP0, "  GRP0", pTia->GetGRP0());
		ImGui::NextColumn();
		DrawWriteRegister(Tia::kGRP1, "  GRP1", pTia->GetGRP1());
		ImGui::Columns(1);

		ImGui::Separator();
		ImGui::Columns(4, NULL, true);
		DrawWriteRegister(Tia::kENAM0, " ENAM0", pTia->GetENAM0());
		ImGui::NextColumn();
		DrawWriteRegister(Tia::kENAM1, " ENAM1", pTia->GetENAM1());
		ImGui::NextColumn();
		DrawWriteRegister(Tia::kENABL, " ENABL", pTia->GetENABL());
		ImGui::NextColumn();
		DrawWriteRegister(Tia::kHMP0, "  HMP1", pTia->GetHMP0());
		ImGui::Columns(1);

		ImGui::Separator();
		ImGui::Columns(4, NULL, true);
		DrawWriteRegister(Tia::kHMP1, "  HMP1", pTia->GetHMP1());
		ImGui::NextColumn();
		DrawWriteRegister(Tia::kHMM0, "  HMM0", pTia->GetHMM0());
		ImGui::NextColumn();
		DrawWriteRegister(Tia::kHMM1, "  HMM1", pTia->GetHMM1());
		ImGui::NextColumn();
		DrawWriteRegister(Tia::kHMBL, "  HMBL", pTia->GetHMBL());
		ImGui::Columns(1);

		ImGui::Separator();
		ImGui::Columns(4, NULL, true);
		DrawWriteRegister(Tia::kVDELP0, "VDELP0", pTia->GetVDELP0());
		ImGui::NextColumn();
		DrawWriteRegister(Tia::kVDELP1, "VDELP1", pTia->GetVDELP1());
		ImGui::NextColumn();
		DrawWriteRegister(Tia::kVDELBL, "VDELBL", pTia->GetVDELBL());
		ImGui::NextColumn();
		DrawWriteRegister(Tia::kRESMP0, "RESMP0", pTia->GetRESMP0());
		ImGui::Columns(1);

		ImGui::Separator();
		ImGui::Columns(4, NULL, true);
		DrawWriteRegister(Tia::kRESMP1, "RESMP1", pTia->GetRESMP1());
		ImGui::NextColumn();
		DrawWriteRegister(Tia::kHMOVE, " HMOVE", pTia->GetHMOVE());
		ImGui::NextColumn();
		DrawWriteRegister(Tia::kHMCLR, " HMCLR", pTia->GetHMCLR());
		ImGui::NextColumn();
		DrawWriteRegister(Tia::kCXCLR, " CXCLR", pTia->GetCXCLR());
		ImGui::Columns(1);
		ImGui::Separator();

		ImGui::Text("Read registers");
		ImGui::Separator();
		ImGui::Columns(4, NULL, true);
		DrawReadRegister(Tia::kCXM0P, " CXM0P", pTia->GetCXM0P());
		ImGui::NextColumn();
		DrawReadRegister(Tia::kCXM1P, " CXM1P", pTia->GetCXM1P());
		ImGui::NextColumn();
		DrawReadRegister(Tia::kCXP0FB, "CXP0FB", pTia->GetCXP0FB());
		ImGui::NextColumn();
		DrawReadRegister(Tia::kCXP1FB, "CXP1FB", pTia->GetCXP1FB());
		ImGui::Columns(1);

		ImGui::Separator();
		ImGui::Columns(4, NULL, true);
		DrawReadRegister(Tia::kCXM0FB, "CXM0FB", pTia->GetCXM0FB());
		ImGui::NextColumn();
		DrawReadRegister(Tia::kCXM1FB, "CXM1FB", pTia->GetCXM1FB());
		ImGui::NextColumn();
		DrawReadRegister(Tia::kCXBLPF, "CXBLPF", pTia->GetCXBLPF());
		ImGui::NextColumn();
		DrawReadRegister(Tia::kCXPPMM, "CXPPMM", pTia->GetCXPPMM());
		ImGui::Columns(1);

		ImGui::Separator();
		ImGui::Columns(4, NULL, true);
		DrawReadRegister(Tia::kINPT0, " INPT0", pTia->GetINPT0());
		ImGui::NextColumn();
		DrawReadRegister(Tia::kINPT1, " INPT1", pTia->GetINPT1());
		ImGui::NextColumn();
		DrawReadRegister(Tia::kINPT2, " INPT2", pTia->GetINPT2());
		ImGui::NextColumn();
		DrawReadRegister(Tia::kINPT3, " INPT3", pTia->GetINPT3());
		ImGui::Columns(1);

		ImGui::Separator();
		ImGui::Columns(4, NULL, true);
		DrawReadRegister(Tia::kINPT4, " INPT4", pTia->GetINPT4());
		ImGui::NextColumn();
		DrawReadRegister(Tia::kINPT5, " INPT5", pTia->GetINPT5());
		ImGui::NextColumn();
		ImGui::Text(" ");
		ImGui::NextColumn();
		ImGui::Text(" ");
		ImGui::Columns(1);
		ImGui::Separator();
	}

    // Draw screen image
	const uint8_t* pPalette = pTia->GetPalette();
	const uint8_t* pPixels = pTia->GetPixels();
	uint32_t numPixels = Tia::kOutputVerticalResolution * Tia::kOutputHorizontalResolution;
	for(uint32_t i=0 ; i<numPixels ; i++)
	{
		// read pixel
		uint8_t pixel = pPixels[i];

		// get palette colour for that pixel
		uint8_t r = pPalette[pixel*3];
		uint8_t g = pPalette[(pixel*3)+1];
		uint8_t b = pPalette[(pixel*3)+2];
		uint8_t a = 255;

		// write RGBA in to texture
		outputBuffer[i*4] = r;
		outputBuffer[(i*4)+1] = g;
		outputBuffer[(i*4)+2] = b;
		outputBuffer[(i*4)+3] = a;
	}

	// build image from tia pixels and palette

	glBindTexture(GL_TEXTURE_2D, videoOutputTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Tia::kOutputHorizontalResolution, Tia::kOutputVerticalResolution, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)&outputBuffer);

	float imageWidth = ImGui::GetWindowWidth() - 20.0f;
	ImGui::Image((void*)(intptr_t)videoOutputTexture, ImVec2(imageWidth, imageWidth * 0.75f));
}

void TiaWindow::SetTia(Tia* tia)
{
    pTia = tia;
}

void TiaWindow::SerialiseState(json& object)
{
	LOGINFO("TiaWindow::SerialiseState");

	json windowJson = json::object();

	windowJson["showInfo"] = bShowInfo;
	windowJson["showRegisters"] = bShowRegisters;

	// serialise breakpoint states
	for(int i=0 ; i<Tia::kNumReadRegisters ; i++)
	{
		char buffer[8];
		sprintf(&buffer[0], "rbp%d", i);
		windowJson[buffer] = pTia->GetReadBreakpoint(i);
	}
	for(int i=0 ; i<Tia::kNumWriteRegisters ; i++)
	{
		char buffer[8];
		sprintf(&buffer[0], "wbp%d", i);
		windowJson[buffer] = pTia->GetWriteBreakpoint(i);
	}

	object["tiawindow"] = windowJson;
}

void TiaWindow::DeserialiseState(json& object)
{
	LOGINFO("TiaWindow::DeserialiseState");

	json windowJson = object["tiawindow"];
	if(windowJson.is_object())
	{
		if(windowJson.contains("showInfo"))
		{
			bShowInfo = windowJson["showInfo"].get<bool>();
		}
		if(windowJson.contains("showRegisters"))
		{
			bShowRegisters = windowJson["showRegisters"].get<bool>();
		}
		for(int i=0 ; i<Tia::kNumReadRegisters ; i++)
		{
			char buffer[8];
			sprintf(&buffer[0], "rbp%d", i);
			if(windowJson.contains(buffer))
			{
				pTia->SetReadBreakpoint(i, windowJson[buffer].get<bool>());
			}
		}
		for(int i=0 ; i<Tia::kNumWriteRegisters ; i++)
		{
			char buffer[8];
			sprintf(&buffer[0], "wbp%d", i);
			if(windowJson.contains(buffer))
			{
				pTia->SetWriteBreakpoint(i, windowJson[buffer].get<bool>());
			}
		}
	}
}
