#pragma once

#include "PhysBody3D.h"
#include "Application.h"
#include "glmath.h"

#include <string>

class btRaycastVehicle;
struct PhysBody3D;

struct Wheel
{
	vec3 connection;
	vec3 direction; 
	vec3 axis;
	float suspensionRestLength; 
	float radius;
	float width;
	bool front; 
	bool drive; 
	bool brake;
	bool steering; 
};

struct VehicleInfo
{
	~VehicleInfo();
	std::string uuid = App->GenerateUUID_V4();
	vec3 chassis_size;
	vec3 chassis_offset;
	vec3 front_chassis_size;
	vec3 front_chassis_offset;
	vec3 rear_chassis_size;
	vec3 rear_chassis_offset;
	float cabine_radius;
	vec3 cabine_offset;
	float mass;
	float suspensionStiffness; 
	float suspensionCompression; 
	float suspensionDamping; 
	float maxSuspensionTravelCm; 
	float frictionSlip; 
	float maxSuspensionForce;

	Wheel* wheels;
	int num_wheels;
	vec3 antenaOffset;
};


struct PhysVehicle3D : public PhysBody3D
{
public:
	PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info);
	~PhysVehicle3D();

	void Render();
	void ApplyEngineForce(float force);
	void Brake(float force);
	void Turn(float degrees);
	float GetKmh() const;

public:

	VehicleInfo info;
	//btRigidBody* Body;
	btRaycastVehicle* vehicle = nullptr;
};