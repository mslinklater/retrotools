// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "resourceswindow.h"
#include "common.h"
#include "system/commandhelpers.h"

ResourcesWindow::ResourcesWindow()
{

}

ResourcesWindow::~ResourcesWindow()
{

}
	
void ResourcesWindow::Draw(void)
{
	auto resources = ResourceManager::Instance()->GetResources();

	if(ImGui::BeginTable("ResourcesTable", 4, 0, ImVec2(0, 100), 0.0))
	{
		// column headings
		ImGui::TableSetupColumn("Info", 0, 0.0f, 0);
		ImGui::TableSetupColumn("ID", 0, 0.0f, 0);
		ImGui::TableSetupColumn("Type", 0, 0.0f, 0);
		ImGui::TableSetupColumn("Filename", 0, 0.0f, 0);
		ImGui::TableHeadersRow();

		for (auto resource : resources)
		{
			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			if (ImGui::SmallButton("Close")) 
			{ 
				// close window
				CommandHelpers::CloseResourceWindow(resource.second.Id);
				// close resource
				ResourceManager::Instance()->CloseResource(resource.second.Id);
			}
			ImGui::SameLine();
			if (ImGui::SmallButton("Info")) 
			{ 
				CommandHelpers::OpenResourceWindow(resource.second.Id);
			}

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%s", resource.second.Id.c_str());

			ImGui::TableSetColumnIndex(2);
			ImGui::Text("%s", ResourceManager::Instance()->ResourceTypeToString(resource.second.type).c_str());

			ImGui::TableSetColumnIndex(3);
			ImGui::Text("%s", resource.second.filename.c_str());
		}
		ImGui::EndTable();
	}
}
