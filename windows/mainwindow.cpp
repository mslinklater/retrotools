#include "mainwindow.h"
#include "../imgui/imgui.h"
#include "../system/command.h"

MainWindow::MainWindow()
{
}

MainWindow::~MainWindow()
{
}

void MainWindow::Draw()
{
	ImGui::Begin("Vistella");
	ImGui::Text("Windows");
	ImGui::Separator();
	if(ImGui::Button("Log"))
	{
		Command msg;
		msg.name = "ToggleWindow";
		msg.payload = "Log";
		CommandCenter::Instance()->Broadcast(msg);
	}
	if(ImGui::Button("Memory"))
	{
		Command msg;
		msg.name = "ToggleWindow";
		msg.payload = "Memory";
		CommandCenter::Instance()->Broadcast(msg);
	}
	if(ImGui::Button("Disassembly"))
	{
		Command msg;
		msg.name = "ToggleWindow";
		msg.payload = "Disassembly";
		CommandCenter::Instance()->Broadcast(msg);
	}
	if(ImGui::Button("Symbols"))
	{
		Command msg;
		msg.name = "ToggleWindow";
		msg.payload = "Symbols";
		CommandCenter::Instance()->Broadcast(msg);
	}
	ImGui::End();
}
