#include "Globals.h"
#include "Camera.h"
#include "JSonHandler.h"
#include "MathGeoLib/include/MathGeoLib.h"

Camera::Camera(GameObject* parent) : Component(parent, CAMERA_COMP)
{
	component->camera = this;
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetPos(float3(0, 0, 0));
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);
	frustum.SetViewPlaneDistances(0.1f, 1000.0f);
	frustum.SetPerspective(1.0f, 1.0f);

	UpdateCameraPlanes();
}

Camera::~Camera()
{
}

void Camera::UpdateComponent()
{

}

void Camera::UpdateCamera(const float4x4& global)
{
	frustum.SetFront(global.WorldZ());
	frustum.SetUp(global.WorldY());
	float3 position;
	float3 scale;
	Quat quat;
	global.Decompose(position, quat, scale);
	frustum.SetPos(position);
	UpdateCameraPlanes();
}

void Camera::DeleteComponent()
{

}

void Camera::SaveComponent(JSonHandler* file)
{
	JSonHandler node = file->InsertNodeArray("Components");
	node.SaveNum("CompType", (double)compType);
	node.SaveString("UUID", UUID.c_str());
	node.SaveNum("FOV", GetFOV());
	node.SaveNum("AspectRatio",GetAspectRatio());
	node.SaveNum("NearPlane",GetNearPlane());
	node.SaveNum("FarPlane",GetFarPlane());

	//Position
	node.CreateArray("Position");
	node.InsertNumArray("Position", frustum.Pos().x);
	node.InsertNumArray("Position", frustum.Pos().y);
	node.InsertNumArray("Position", frustum.Pos().z);
	
}

void Camera::LoadComponent(JSonHandler* file)
{
	UUID = file->GetString("UUID");
	SetFOV((float)file->GetNum("FOV"));
	SetAspectRatio((float)file->GetNum("AspectRatio"));
	SetNearPlane((float)file->GetNum("NearPlane"));
	SetFarPlane((float)file->GetNum("FarPlane"));

	file->LoadArray("Position");
	vec pos((float)file->GetNumArray("Position", 0), (float)file->GetNumArray("Position", 1), (float)file->GetNumArray("Position", 2));
	frustum.SetPos(pos);
	UpdateCameraPlanes();
}

void Camera::LookAt(float3 pos)
{
	float3 v = pos - frustum.Pos();
	float3x3 matrix = float3x3::LookAt(frustum.Front(), v.Normalized(), frustum.Up(), float3::unitY);
	frustum.SetFront(matrix.MulDir(frustum.Front()).Normalized());
	frustum.SetUp(matrix.MulDir(frustum.Up()).Normalized());
	UpdateCameraPlanes();
}

void Camera::UpdateCameraPlanes()
{
	frustum.GetPlanes(planes);
}

void Camera::SetNearPlane(float val)
{
	if (val > 0 && val < frustum.FarPlaneDistance()) frustum.SetViewPlaneDistances(val, frustum.FarPlaneDistance());
	UpdateCameraPlanes();
}

void Camera::SetFarPlane(float val)
{
	if (val > 0 && val > frustum.NearPlaneDistance()) frustum.SetViewPlaneDistances(frustum.NearPlaneDistance(), val);
	UpdateCameraPlanes();
}

void Camera::SetFOV(float val)
{
	val *= DEGTORAD;
	frustum.SetVerticalFovAndAspectRatio(val, frustum.AspectRatio());
	UpdateCameraPlanes();
}

void Camera::SetAspectRatio(float val)
{
	frustum.SetHorizontalFovAndAspectRatio(frustum.HorizontalFov(), val);
	UpdateCameraPlanes();
}

float* Camera::GetOpenGLViewMatrix()
{
	float4x4 m = frustum.ViewMatrix();
	m.Transpose();
	return (float*)m.v;
}

float* Camera::GetOpenGLProjectionMatrix()
{
	float4x4 m = frustum.ProjectionMatrix();
	m.Transpose();
	return (float*)m.v;
}
