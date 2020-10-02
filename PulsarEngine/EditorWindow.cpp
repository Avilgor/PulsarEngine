#include "EditorWindow.h"
#include "Application.h"
#include "ImGui/imgui_impl_sdl.h"
#include "SDL/include/SDL.h"

#include <string>

EditorWindow::EditorWindow(std::string n, SDL_Window* win,Application* ap)
{
	name = n;
	window = win;
	App = ap;
}

EditorWindow::~EditorWindow()
{}

void EditorWindow::SetActive(bool act)
{
	active = act;
}
