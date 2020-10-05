#include "Globals.h"
#include "Application.h"
#include "EditorMain.h"
#include "ConfigWindow.h"
#include "ConsoleWindow.h"
#include "MenuBar.h"
#include "SDL/include/SDL.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

#include <vector>
#include <string>

EditorMain::EditorMain(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

EditorMain::~EditorMain()
{}

// -----------------------------------------------------------------
bool EditorMain::Init()
{
	LOG("Initializing editor...");
	bool ret = true;
    mainWindow = App->window->window;

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	// Setup Dear ImGui style
	ImGui::StyleColorsLight();

	ImGui_ImplSDL2_InitForOpenGL(mainWindow, App->renderer3D->context);
    ImGui_ImplOpenGL3_Init();

	return ret;
}

// -----------------------------------------------------------------
bool EditorMain::Start()
{
	LOG("Starting editor...");
	bool ret = true;    
    WindowsList.push_back(new MenuBar(""));//0
    WindowsList.push_back(new ConfigWindow("Configuration"));//1
    WindowsList.push_back(new ConsoleWindow("Console"));//2
	return ret;
}

// -----------------------------------------------------------------
bool EditorMain::CleanUp()
{
	LOG("Cleaning editor...");
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	return true;
}

// -----------------------------------------------------------------
update_status EditorMain::PreUpdate(float dt)
{
    update_status status = UPDATE_CONTINUE;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(mainWindow);
    ImGui::NewFrame();
    ImGuiIO& io = ImGui::GetIO();

    return status;
}

// -----------------------------------------------------------------
update_status EditorMain::Update(float dt)
{
    update_status status = UPDATE_CONTINUE;

    if (!WindowsList.empty())
    {
        for (std::vector<EditorWindow*>::const_iterator it = WindowsList.begin(); it != WindowsList.end(); ++it)
        {
            if((*it)->IsActive()) (*it)->Draw();
        }

        for (std::vector<EditorWindow*>::const_iterator it = WindowsList.begin(); it != WindowsList.end(); ++it)
        {
            if ((*it)->IsActive()) (*it)->InfoProcessing();
        }

        // rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
   
	return status;
}

// -----------------------------------------------------------------
update_status EditorMain::PostUpdate(float dt)
{
    return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void EditorMain::ProccesInput(SDL_Event e)
{
    ImGui_ImplSDL2_ProcessEvent(&e);
}

void EditorMain::SetWindowsActive(int index, bool val)
{
    WindowsList[index]->SetActive(val);
}