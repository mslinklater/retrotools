// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "t64window.h"
#include "resources/resource_t64.h"
#include "common.h"

T64Window::T64Window()
{

}

T64Window::~T64Window()
{

}

void T64Window::Draw()
{
	const ResourceT64* pRes = (const ResourceT64*)pResource.get();

	const ResourceT64::TapeRecord* pTape = pRes->GetTapeRecord();

	ImGui::Text("%s V0x%04x", pTape->GetDescriptor().c_str(), pTape->GetVersion());
	ImGui::Text("Entries:%d/%d", pTape->GetNumUsedEntries(), pTape->GetNumEntries());
	ImGui::Text("User:%s", pTape->GetUserDescription().c_str());

	for (int i = 0; i < pRes->GetNumFileRecords(); i++)
	{
		const ResourceT64::FileRecord *pFile = pRes->GetFileRecord(i);
		ImGui::Separator();
		ImGui::Text("Filename:%s", pFile->GetFilename().c_str());
		ImGui::Text("Addr:0x%04x-0x%04x", pFile->GetStartAddress(), pFile->GetEndAddress());
	}
}
