#include "Globals.h"
#include "Application.h"
#include "EditorMain.h"
#include "SDL/include/SDL.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"


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
    io.ConfigWindowsMoveFromTitleBarOnly = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(mainWindow, App->renderer3D->context);
    ImGui_ImplOpenGL3_Init();

	return ret;
}

bool EditorMain::Start()
{
	LOG("Starting editor...");
	bool ret = true;
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
update_status EditorMain::Update(float dt)
{
    update_status status = UPDATE_CONTINUE;

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        // without it you won't have keyboard input and other things
        ImGui_ImplSDL2_ProcessEvent(&event);
        // you might also want to check io.WantCaptureMouse and io.WantCaptureKeyboard
        // before processing events

        switch (event.type)
        {
        case SDL_QUIT:
            status = UPDATE_STOP;
            break;

        case SDL_WINDOWEVENT:
            switch (event.window.event)
            {
            case SDL_WINDOWEVENT_RESIZED:
                App->window->width = event.window.data1;
                App->window->height = event.window.data2;
                //glViewport(0, 0, windowWidth, windowHeight);
                break;
            }
            break;

        case SDL_KEYDOWN:
            /*switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                 status = UPDATE_STOP;
                break;
            }*/
            break;
        }
    }

    // start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(mainWindow);
    ImGui::NewFrame();

    // a window is defined by Begin/End pair
    {
        static int counter = 0;
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
        );
        // create a window and append into it
        ImGui::Begin("Controls", NULL, ImGuiWindowFlags_NoResize);

        ImGui::Dummy(ImVec2(0.0f, 1.0f));
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Platform");
        ImGui::Text("%s", SDL_GetPlatform());
        ImGui::Text("CPU cores: %d", SDL_GetCPUCount());
        ImGui::Text("RAM: %.2f GB", SDL_GetSystemRAM() / 1024.0f);

        // buttons and most other widgets return true when clicked/edited/activated
        if (ImGui::Button("Counter button"))
        {
            LOG("counter button clicked");
            counter++;
        }
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::End();
    }

    // rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    SDL_GL_SwapWindow(mainWindow);
	return status;
}
