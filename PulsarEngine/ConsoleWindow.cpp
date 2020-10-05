#include "ConsoleWindow.h"
#include "Application.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"

#include <string>

ConsoleWindow::ConsoleWindow(std::string name) : EditorWindow(name)
{}

ConsoleWindow::~ConsoleWindow()
{}


void ConsoleWindow::Draw()
{   
    ImGui::Begin(name.c_str(), &active);
    if (!App->ConsoleLogs.empty())
    {
        for (int i = 0; i < App->ConsoleLogs.size(); ++i)
        {
            ImGui::Text(App->ConsoleLogs[i].c_str());
        }
        //App->ConsoleLogs.clear();
    }
    ImGui::End();
}