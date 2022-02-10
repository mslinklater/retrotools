// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "t64window.h"
#include "resources/resource_t64.h"
#include "system/common.h"

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

    if (ImGui::CollapsingHeader("Payloads"))
	{
		for(auto payload : pRes->GetPayloads())
		{
		    if (ImGui::CollapsingHeader(payload.name.c_str()))
			{
				ImGui::Text("Offset:%d", payload.offset);
				ImGui::Text("Size:%d", payload.size);
			}			
		}
	}


	for (int i = 0; i < pRes->GetNumFileRecords(); i++)
	{
		const ResourceT64::FileRecord *pFile = pRes->GetFileRecord(i);
		ImGui::Separator();
		ImGui::Text("Filename:%s", pFile->GetFilename().c_str());
		ImGui::Text("Addr:0x%04x-0x%04x", pFile->GetStartAddress(), pFile->GetEndAddress());

		// window of binary contents
	    if (ImGui::CollapsingHeader("Contents"))
		{
			ImGui::BeginChild("Contents", ImVec2(400, 200), true, 0);

			// display the contents
			int startAddr = pFile->GetStartAddress();
			int endAddr = pFile->GetEndAddress();
			for (int addr = startAddr; addr < endAddr; addr += 16)
			{
//				char byte = pFile->GetByte(addr);
			}

			ImGui::EndChild();
		}
	}
}
