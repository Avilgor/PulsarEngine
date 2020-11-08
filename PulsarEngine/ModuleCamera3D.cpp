#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "GameObject.h"
#include "Transform.h"


ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app,"Camera",start_enabled)
{

}

ModuleCamera3D::~ModuleCamera3D()
{
	
}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	camera = new Camera(nullptr);
	camera->frustum.SetViewPlaneDistances(0.1f, 2000.0f);
	camera->frustum.SetPerspective(1.0f, 1.0f);
	camera->frustum.SetPos(float3(0, 10, 5));
	camera->frustum.SetFront(float3::unitZ);
	camera->frustum.SetUp(float3::unitY);
	camera->LookAt(float3(0, 0, 0));
	reference = { 0.0f,0.0f,0.0f };
	lockRef = false;
	camera->UpdateCameraPlanes();
	mouseDrag = false;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");
	camera->DeleteComponent();
	delete camera;
	camera = nullptr;
	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	if (App->editor->mouse_in_scene || mouseDrag)
	{
		vec newPos(0.0f, 0.0f, 0.0f); 
		float speed = 1.0f;

		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_DOWN && App->editor->mouse_in_scene) mouseDrag = true;
		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_UP) mouseDrag = false;
		if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_DOWN && App->editor->mouse_in_scene) mouseDrag = true;
		if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_UP) mouseDrag = false;
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) speed *= 2;

		if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT && mouseDrag)
		{
			int motionX,motionY;
			motionX = App->input->GetMouseXMotion();
			motionY = App->input->GetMouseYMotion();

			if (motionX > 0) newPos.x -= speed * motionX;
			else if (motionX < 0)  newPos.x -= speed * motionX;

			if (motionY > 0) newPos.y += speed * 1.5f * motionY;
			else if (motionY < 0)  newPos.y += speed * 1.5f * motionY;
			
			Move(newPos);
		}

		if (App->input->GetMouseZ() < 0) Zoom(-10.0f);
		else if (App->input->GetMouseZ() > 0)  Zoom(10.0f);

		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT && mouseDrag)
		{
			float3 temp(0.0f,0.0f,0.0f);
			if (App->input->GetMouseXMotion() > 0) temp.x -= 10.0f;
			else if (App->input->GetMouseXMotion() < 0)  temp.x += 10.0f;

			if (App->input->GetMouseYMotion() > 0) temp.y -= 10.0f;
			else if (App->input->GetMouseYMotion() < 0)  temp.y += 10.0f;

			Pan(temp.x,temp.y);			
		}
	}
	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
float3 ModuleCamera3D::GetPos()
{
	return camera->frustum.Pos();
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(float3 pos)
{
	camera->LookAt(pos);
	reference = pos;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Zoom(float val)
{
	float distance = reference.Distance(camera->frustum.Pos());
	vec newPos = camera->frustum.Pos() + camera->frustum.Front() * val * distance * 0.05f;
	camera->frustum.SetPos(newPos);
	camera->UpdateCameraPlanes();
}

// -----------------------------------------------------------------
void ModuleCamera3D::Pan(float x, float y)
{	
	float3 vector = camera->frustum.Pos() - reference;
	Quat qY(camera->frustum.Up(), x * 0.003);
	Quat qX(camera->frustum.WorldRight(), y * 0.003);
	vector = qX.Transform(vector);
	vector = qY.Transform(vector);
	camera->frustum.SetPos(vector + reference);
	Look(reference);
}

// -----------------------------------------------------------------
void ModuleCamera3D::SetReference(float3 pos)
{
	reference = pos;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Move(vec val)
{	
	float d = reference.Distance(camera->frustum.Pos());
	float3 difX = camera->frustum.Up() * val.y * (d / 1000);
	float3 difY = camera->frustum.WorldRight() * val.x * (d / 1000);
	if (!lockRef)
	{
		reference += difX;
		reference += difY;
	}
	camera->frustum.SetPos(camera->frustum.Pos() + difX + difY);
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetGLViewMatrix()
{
	return camera->GetOpenGLViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetGLProjectionMatrix()
{
	return camera->GetOpenGLProjectionMatrix();
}