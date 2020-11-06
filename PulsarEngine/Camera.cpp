#include "Globals.h"
#include "Camera.h"

Camera::Camera(GameObject* parent) : Component(parent, CAMERA_COMP)
{
	component->camera = this;
}

Camera::~Camera()
{}

void Camera::UpdateComponent()
{

}

void Camera::DeleteComponent()
{

}

void Camera::SaveComponent(JSonHandler* file)
{

}

void Camera::LoadComponent(JSonHandler* file)
{

}