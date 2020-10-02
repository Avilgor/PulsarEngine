#include "Globals.h"
#include "Application.h"
#include "EditorMain.h"
#include "EditorWindow.h"
#include "ConfigWindow.h"
#include "SDL/include/SDL.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

#include <vector>

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
    //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    //io.ConfigWindowsMoveFromTitleBarOnly = true;

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
    WindowsList.push_back(new ConfigWindow("Configuration",mainWindow,App));
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
    /*
    // get the window size as a base for calculating widgets geometry
    int sdl_width = 0, sdl_height = 0, controls_width = 0;
    SDL_GetWindowSize(mainWindow, &sdl_width, &sdl_height);
    controls_width = sdl_width;
    // make controls widget width to be 1/3 of the main window width
    if ((controls_width /= 3) < 300) { controls_width = 300; }
    
    // position the controls widget in the top-right corner with some margin
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
    // here we set the calculated width and also make the height to be
    // be the height of the main window also with some margin
    ImGui::SetNextWindowSize(
        ImVec2(static_cast<float>(controls_width), static_cast<float>(sdl_height - 20)),
        ImGuiCond_Always
    );*/

    //Window   
}
