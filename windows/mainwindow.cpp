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
	if(ImGui::Button("Log Button"))
	{
		Command msg;
		msg.name = "Test";
		
		CommandCenter::Instance()->Broadcast(msg);
	}
	ImGui::End();
}
