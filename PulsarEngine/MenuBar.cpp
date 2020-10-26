#include "Globals.h"
#include "Application.h"
#include "MenuBar.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"

#include <string>

MenuBar::MenuBar(std::string name) : EditorWindow(name)
{}

MenuBar::~MenuBar()
{}


update_status MenuBar::Draw()
{
	update_status ret = UPDATE_CONTINUE;

	ImGui::BeginMainMenuBar();
	
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("New")) {}
		if (ImGui::MenuItem("Open")) {}
		if (ImGui::MenuItem("Save")) {}
		if (ImGui::MenuItem("Exit")) { ret = UPDATE_STOP; }
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Create"))
	{
		if (ImGui::MenuItem("Gameobject")) App->scene->CreateEmptyGameobject();

		if (ImGui::MenuItem("Cube")) App->scene->CreateCube();

		if (ImGui::MenuItem("Pyramid")) App->scene->CreatePyramid();

		if (ImGui::MenuItem("Plane")) App->scene->CreatePlane();

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Edit"))
	{
		if (ImGui::MenuItem("Create child")) App->editor->CreateSelectionChild();

		if (ImGui::MenuItem("Delete all childs")) App->editor->DeleteSelectionChilds();

		if (ImGui::MenuItem("Delete selected")) App->editor->DeleteSelected();
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
		if (ImGui::MenuItem("Hierarchy"))App->editor->SetWindowsActive(4, true);

		if (ImGui::MenuItem("Inspector"))App->editor->SetWindowsActive(3, true);

		if (ImGui::MenuItem("Scene")) App->editor->SetWindowsActive(6, true);

		if (ImGui::MenuItem("Project")) App->editor->SetWindowsActive(5, true);
		
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

		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();
	
	if(renderOptions) DrawRenderOptions();

	return ret;
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
	if (ImGui::Checkbox("Blend", &blend)) App->renderer3D->ToggleBlend(blend);
	if (ImGui::Button("Reset To Default"))
	{
		depth = true;
		cullFace = false;
		light = true;
		colorMaterial = true;
		texture2D = true;
		App->renderer3D->ResetToDefault();
	}
	ImGui::End();
}