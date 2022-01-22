#include "resourceswindow.h"
#include "common.h"

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
			if (ImGui::SmallButton("Info")) 
			{ 
				// open up details window for resource
			}

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%s", "Id");
//			ImGui::Text("%s", resource->GetId().c_str());

			ImGui::TableSetColumnIndex(2);
			ImGui::Text("%s", "TYPE");
//			ImGui::Text("%s", ResourceManager::Instance()->ResourceTypeToString(resource->GetResourceType()).c_str());

			ImGui::TableSetColumnIndex(3);
//			ImGui::Text("%s", resource->GetFilename().c_str());
			ImGui::Text("%s", "FILENAME");
		}
		ImGui::EndTable();
	}
}
