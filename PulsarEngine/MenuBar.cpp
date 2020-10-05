#include "MenuBar.h"
#include "Globals.h"
#include "Application.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"

#include <string>

MenuBar::MenuBar(std::string name) : EditorWindow(name)
{}

MenuBar::~MenuBar()
{}


void MenuBar::Draw()
{
	ImGui::BeginMainMenuBar();
	
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("New")) {}
		if (ImGui::MenuItem("Open")) {}
		if (ImGui::MenuItem("Save")) {}
		ImGui::EndMenu();
	}


	if (ImGui::BeginMenu("Edit"))
	{
		if (ImGui::MenuItem("Create")) {}

		if (ImGui::MenuItem("Delete selection")) {}
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("View"))
	{
		if (ImGui::MenuItem("Shaded")) App->renderer3D->ToggleShaded();

		if (ImGui::MenuItem("Wireframe")) App->renderer3D->ToggleWireframe();

		if (ImGui::MenuItem("Render options")) ToggleRenderOptions();
		
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Windows"))
	{
		if (ImGui::MenuItem("Hierachy"))
		{
		}
		if (ImGui::MenuItem("Inspector"))
		{
		}
		if (ImGui::MenuItem("Scene"))
		{

		}
		if (ImGui::MenuItem("Console")) App->editor->SetWindowsActive(2, true);
		
		if (ImGui::MenuItem("Configuration")) App->editor->SetWindowsActive(1,true);
		
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Help"))
	{
		if (ImGui::MenuItem("Latest version"))
			ShellExecuteA(NULL, "open", "https://github.com/Avilgor/PulsarEngine/releases", NULL, NULL, SW_SHOWNORMAL);

		if (ImGui::MenuItem("Report a bug"))
			ShellExecuteA(NULL, "open", "https://github.com/Avilgor/PulsarEngine/issues", NULL, NULL, SW_SHOWNORMAL);

		if (ImGui::MenuItem("View on GitHub"))
			ShellExecuteA(NULL, "open", "https://github.com/Avilgor/PulsarEngine", NULL, NULL, SW_SHOWNORMAL);

		//if (ImGui::MenuItem("About"))

		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();
	
	if(renderOptions) DrawRenderOptions();
} 

void MenuBar::ToggleRenderOptions()
{
	renderOptions = !renderOptions;
}

void MenuBar::DrawRenderOptions()
{
	ImGui::Begin("Renderer", &renderOptions);

	if (ImGui::Checkbox("Depth test", &depth)) App->renderer3D->ToggleDepthTest(depth);
	if (ImGui::Checkbox("Cull face", &cullFace)) App->renderer3D->ToggleCullFace(cullFace);
	if (ImGui::Checkbox("Lightning", &light)) App->renderer3D->ToggleLights(light);
	if (ImGui::Checkbox("Color materials", &colorMaterial)) App->renderer3D->ToggleColorMaterial(colorMaterial);
	if (ImGui::Checkbox("2D textures", &texture2D)) App->renderer3D->Toggle2DTextures(texture2D);
	if (ImGui::Button("ResetToDefault"))
	{
		depth = false;
		cullFace = true;
		light = true;
		colorMaterial = true;
		texture2D = true;
		App->renderer3D->ResetToDefault();
	}
	ImGui::End();
}