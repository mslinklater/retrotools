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

	if(ImGui::BeginTable("ResourcesTable", 1, 0, ImVec2(0, 100), 0.0))
	{
		// column headings
		ImGui::TableSetupColumn("ID", 0, 0.0f, 0);
		ImGui::TableHeadersRow();

		for (auto resource : resources)
		{
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("ThisID");
		}
		ImGui::EndTable();
	}
}
