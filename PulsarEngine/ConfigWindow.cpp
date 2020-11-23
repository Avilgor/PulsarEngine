#include "Globals.h"
#include "Application.h"
#include "ConfigWindow.h"
#include "EngineResource.h"
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

update_status ConfigWindow::Draw()
{
    update_status ret = UPDATE_CONTINUE;
   
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

    ImGui::SetNextWindowBgAlpha(1.0f);
    ImGui::Begin(name.c_str(), &active);
    if (ImGui::CollapsingHeader("System info"))
    {        
        ImGui::Text("Platform:");
        ImGui::SameLine();
        ImGui::Text("%s", SDL_GetPlatform());
        ImGui::Text("CPU cores: %d", SDL_GetCPUCount());
        ImGui::Text("RAM: %.2f GB", SDL_GetSystemRAM() / 1024.0f);
        ImGui::Text("Current fps: %.2f",App->fps);
        ImGui::SameLine();
        ImGui::Text("Current ms: %.2f", App->ms);
        
        ImGui::PlotLines("FPS", fps, IM_ARRAYSIZE(fps), 0, NULL, 0.0f, 120.0f, ImVec2(0, 80));
        ImGui::PlotLines("MS", ms, IM_ARRAYSIZE(ms), 0, NULL, 0.0f, 40.0f, ImVec2(0, 80));
    }

    if (ImGui::CollapsingHeader("Window options"))
    {
        if(ImGui::Checkbox("Fullscreen", &fullscreen)) App->window->SetFullscreen(fullscreen);
        if (ImGui::Checkbox("Borderless", &borderless)) App->window->SetBorderless(borderless);
        if(ImGui::Checkbox("Resizable", &resizable)) App->window->SetResizable(resizable);
        if(ImGui::Checkbox("Full desktop", &maximize)) App->window->SetMaximize(maximize);
        if (ImGui::InputInt("Width", &width,1,100,ImGuiInputTextFlags_EnterReturnsTrue) || ImGui::InputInt("Height", &height, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            if(width >= 400 && height >= 400) App->window->SetWindowsSize(width, height);
        }
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

    if (ImGui::CollapsingHeader("Loaded resources"))
    {
        std::map<std::string, EngineResource*> res = App->resourceManager->GetLoadedResources();
        if (!res.empty())
        {
            ImGui::Text("Name/");
            ImGui::SameLine();
            ImGui::Text("References");
            for (std::map<std::string, EngineResource*>::iterator it = res.begin(); it != res.end(); it++)
            {
                ImGui::Text((*it).second->name.c_str());
                ImGui::SameLine();
                ImGui::Text("%d", (*it).second->references);
            }
        }
        else
        {
            ImGui::Text("No resources loaded.");
        }      
    }

    ImGui::End();

    width = App->window->width;
    height = App->window->height;

    return ret;
}