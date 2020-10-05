#include "ConfigWindow.h"
#include "Application.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"

#include <string>

ConfigWindow::ConfigWindow(std::string name) : EditorWindow(name)
{
    width = App->window->width;
    height = App->window->height;
    for (int i = 0; i < 100; i++)
    {
        fps[i] = 0;
        ms[i] = 0;
    }
    plotTimer.Start();
}

ConfigWindow::~ConfigWindow()
{}

void ConfigWindow::Draw()
{
    width = App->window->width;
    height = App->window->height;
    if (plotTimer.ReadSec() >= 1.0f)
    {
        for (int i = 0; i < 99; i++)
        {
            fps[i] = fps[i+1];
            ms[i] = ms[i+1];
        }
        fps[99] = App->fps;
        ms[99] = App->ms;
        plotTimer.Start();
    }

    ImGui::Begin(name.c_str(), &active);
    if (ImGui::CollapsingHeader("System info"))
    {        
        //ImGui::InputText("Project Name", appName, 100, ImGuiInputTextFlags_EnterReturnsTrue);
        ImGui::Text("Platform:");
        ImGui::SameLine();
        ImGui::Text("%s", SDL_GetPlatform());
        ImGui::Text("CPU cores: %d", SDL_GetCPUCount());
        ImGui::Text("RAM: %.2f GB", SDL_GetSystemRAM() / 1024.0f);
        ImGui::Text("Current fps: %.2f",App->fps);
        ImGui::SameLine();
        ImGui::Text("Current ms: %.2f", App->ms);
        
        ImGui::PlotLines("FPS", fps, IM_ARRAYSIZE(fps), 0, NULL, 0.0f, 120.0f, ImVec2(0, 80));
        ImGui::PlotHistogram("MS", ms, IM_ARRAYSIZE(ms), 0, NULL, 0.0f, 40.0f, ImVec2(0, 80));
    }
    if (ImGui::CollapsingHeader("Window options"))
    {
        if(ImGui::Checkbox("Fullscreen", &fullscreen)) App->window->SetFullscreen(fullscreen);
        if (ImGui::Checkbox("Borderless", &borderless)) App->window->SetBorderless(borderless);
        if(ImGui::Checkbox("Resizable", &resizable)) App->window->SetResizable(resizable);
        if(ImGui::Checkbox("Full desktop", &maximize)) App->window->SetMaximize(maximize);
        if (ImGui::SliderInt("Width", &width, 800, 1920)) App->window->SetWindowsSize(width, height);
        if(ImGui::SliderInt("Height", &height, 600, 1080)) App->window->SetWindowsSize(width, height);       
        if (ImGui::Button("Reset size")) App->window->RestoreDefaultSize();
        ImGui::Separator();
        if(ImGui::SliderFloat("Brightness", &brightness, 0.0f, 1.0f)) App->window->SetBrightness(brightness);
        ImGui::Separator();
        ImGui::Text("Windows theme:");
        if (ImGui::Button("Classic")) ImGui::StyleColorsClassic();
        ImGui::SameLine();
        if (ImGui::Button("Dark")) ImGui::StyleColorsDark();
        ImGui::SameLine();
        if (ImGui::Button("Light")) ImGui::StyleColorsLight();
    }

    ImGui::End();
}