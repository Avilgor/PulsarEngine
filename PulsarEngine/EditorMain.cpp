#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "EditorMain.h"
#include "ConfigWindow.h"
#include "ConsoleWindow.h"
#include "MenuBar.h"
#include "ProjectWindow.h"
#include "SceneWindow.h"
#include "InspectorWindow.h"
#include "HierarchyWindow.h"
#include "Transform.h"
#include "SDL/include/SDL.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"


#include <vector>
#include <string>

EditorMain::EditorMain(Application* app, bool start_enabled) : Module(app,"Editor",start_enabled)
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
	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(mainWindow, App->renderer3D->context);
    ImGui_ImplOpenGL3_Init();

    dockFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking;
    dockFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoNavFocus /*| ImGuiWindowFlags_NoBackground*/;

    dockspace_flags = ImGuiDockNodeFlags_None;

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
    WindowsList.push_back(new InspectorWindow("Inspector"));//3
    WindowsList.push_back(new HierarchyWindow("Hierarchy"));//4
    WindowsList.push_back(new ProjectWindow("Project name"));//5
    WindowsList.push_back(new SceneWindow("Scene"));//6

	return ret;
}

void EditorMain::SelectOne(GameObject* go)
{
    if (!selectedGameObjects.empty())
    {
        for (std::vector<GameObject*>::iterator it = selectedGameObjects.begin(); it != selectedGameObjects.end(); ++it)
        {
            (*it)->selected = false;
        }
        selectedGameObjects.clear();
    }
    selectedGameObjects.push_back(go);
    go->selected = true;
}

void EditorMain::AddSelection(GameObject* go)
{
    go->selected = true;
    selectedGameObjects.push_back(go);
}

// -----------------------------------------------------------------
void EditorMain::RemoveSelection(GameObject* go)
{
    if (!selectedGameObjects.empty())
    {
        std::vector<GameObject*> temp = selectedGameObjects;
        for (std::vector<GameObject*>::iterator it = selectedGameObjects.begin(); it != selectedGameObjects.end(); ++it)
        {
            if (go->UUID.compare((*it)->UUID) != 0) temp.push_back((*it));
            else go->selected = false;
        }
        selectedGameObjects.clear();
        selectedGameObjects = temp;
        temp.clear();
    }
}

// -----------------------------------------------------------------
void EditorMain::DeleteSelected()
{
    if (!selectedGameObjects.empty())
    {
        for (std::vector<GameObject*>::iterator it = selectedGameObjects.begin(); it != selectedGameObjects.end(); ++it)
        {
            (*it)->selected = false;
            (*it)->DeleteGameobject();
        }
        selectedGameObjects.clear();
    }
}


// -----------------------------------------------------------------
void EditorMain::EmptySelected()
{
    if (!selectedGameObjects.empty())
    {
        for (std::vector<GameObject*>::iterator it = selectedGameObjects.begin(); it != selectedGameObjects.end(); ++it)
        {
            (*it)->selected = false;
        }
        selectedGameObjects.clear();
    }
}

// -----------------------------------------------------------------
void EditorMain::CreateSelectionChild()
{
    for (std::vector<GameObject*>::iterator it = selectedGameObjects.begin(); it != selectedGameObjects.end(); ++it)
    {
        (*it)->CreateChild();
    }
}

// -----------------------------------------------------------------
void EditorMain::DeleteSelectionChilds()
{
    for (std::vector<GameObject*>::iterator it = selectedGameObjects.begin(); it != selectedGameObjects.end(); ++it)
    {
        (*it)->DeleteAllChilds();
    }
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

    //Save input states for all editor modules
    ctrl = App->input->GetKey(SDL_SCANCODE_LCTRL);
    deleteKey = App->input->GetKey(SDL_SCANCODE_DELETE);
    leftMouse = App->input->GetMouseButton(SDL_BUTTON_LEFT);
    rightMouse = App->input->GetMouseButton(SDL_BUTTON_RIGHT);

    //if (mouse_in_scene && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN) EmptySelected();//Temp
    if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT)
    {
        if (!selectedGameObjects.empty())
        {
            float3 pos = selectedGameObjects[0]->transform->GetLocalPosition();
            App->camera->Look(float3(pos.x, pos.y, pos.z));
        }
    }

    if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
    {
        if (!selectedGameObjects.empty())
        {
            float3 pos = selectedGameObjects[0]->transform->GetLocalPosition();
            App->camera->Look(float3(pos.x, pos.y, pos.z));
        }
    }

    //Process inputs
    if (deleteKey == KEY_DOWN) DeleteSelected();

    //Update editor windows
    if (!WindowsList.empty())
    {
        status = RenderDock();
        if (status == UPDATE_CONTINUE)
        {           
            for (std::vector<EditorWindow*>::const_iterator it = WindowsList.begin(); it != WindowsList.end(); ++it)
            {
                if (status == UPDATE_CONTINUE)
                {
                    if ((*it)->IsActive()) (*it)->InfoProcessing();
                }
                else break;
            }           
        }
    }  
   
	return status;
}

// -----------------------------------------------------------------
update_status EditorMain::PostUpdate(float dt)
{
    update_status status = UPDATE_CONTINUE;
    if (!WindowsList.empty())
    {
        for (std::vector<EditorWindow*>::const_iterator it = WindowsList.begin(); it != WindowsList.end(); ++it)
        {
            if (status == UPDATE_CONTINUE)
            {
                if ((*it)->IsActive()) status = (*it)->Draw();
            }
            else break;
        }
        // rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());        
    }
    return status;
}

// -----------------------------------------------------------------
void EditorMain::ProccesInput(SDL_Event e)
{
    ImGui_ImplSDL2_ProcessEvent(&e);
}

// -----------------------------------------------------------------
void EditorMain::SetWindowsActive(int index, bool val)
{
    WindowsList[index]->SetActive(val);
}

// -----------------------------------------------------------------
update_status EditorMain::RenderDock()
{
    update_status status = UPDATE_CONTINUE;

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->GetWorkPos());
    ImGui::SetNextWindowSize(viewport->GetWorkSize());
    ImGui::SetNextWindowBgAlpha(0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("Project",&dock, dockFlags);

    ImGui::PopStyleVar();
    ImGui::PopStyleVar(2);

    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

    ImGui::End();
 

    return status;
}

bool EditorMain::HasSelection()
{
    if (selectedGameObjects.empty()) return false;
    else return true;
}