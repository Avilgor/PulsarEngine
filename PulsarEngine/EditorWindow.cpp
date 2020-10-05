#include "EditorWindow.h"
#include "ImGui/imgui_impl_sdl.h"
#include "SDL/include/SDL.h"

#include <string>

EditorWindow::EditorWindow(std::string n)
{
	name = n;
}

EditorWindow::~EditorWindow()
{}

void EditorWindow::SetActive(bool act)
{
	active = act;
}
