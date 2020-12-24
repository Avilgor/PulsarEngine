#ifndef __ModulePhysic_H__
#define __ModulePhysic_H__

#include "Module.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"
#include "MathGeoLib/include/MathGeoLib.h"

#include <map>
#include <string>

class DebugDrawer;
class PhysBody3D;
struct PhysVehicle3D;
struct VehicleInfo;

class ModulePhysics: public Module
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
	void SetGravity(float3 val);
	void SetSimulationSteps(int val);

	PhysBody3D* AddBody(BoxCollider* box, float3 size,float mass = 10.0f);
	PhysBody3D* AddBody(SphereCollider* sphere, float mass = 10.0f);
	PhysBody3D* AddBody(CapsuleCollider* sphere, float mass = 10.0f);
	PhysVehicle3D* AddVehicle(const VehicleInfo& info);

	btTypedConstraint* AddConstraintPoint(PhysBody3D& bodyA, PhysBody3D& bodyB, const vec3& anchorA, const vec3& anchorB, std::string id);
	btTypedConstraint* AddConstraintHinge(PhysBody3D& bodyA, PhysBody3D& bodyB, const vec3& anchorA, const vec3& anchorB, const vec3& axisS, const vec3& axisB, std::string id, bool disable_collision = false);
	btTypedConstraint* AddConstraintSlider(PhysBody3D& bodyA, PhysBody3D& bodyB, const btTransform& anchorA, const btTransform& anchorB, std::string id);
	btTypedConstraint* AddConstraintCone(PhysBody3D& bodyA, PhysBody3D& bodyB, const btTransform& anchorA, const btTransform& anchorB, std::string id);

	void RemoveCollider(std::string uuid);
	void RemoveConstraint(btTypedConstraint* constraint, std::string id);
	void RemoveConstraint(std::string id);
	void RemoveBody(btRigidBody* body, std::string id);
	void AddBody(PhysBody3D* body, std::string id);
	void AddConstraint(btTypedConstraint* con, std::string id);
	PhysBody3D* GetBodyByUUID(std::string id);
	Component* GetColliderByUUID(std::string id);

	void DebugDrawBody(btRigidBody* body);
	void ToggleSimulation(bool val);
	void ToggleSimulationPause(bool val);
	int GetSteps() { return steps; }
	float3 GetGravity();

public:
	bool runningSimulation;
	bool simulationPause;

private:

	btDefaultCollisionConfiguration* collision_conf;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* broad_phase;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* world;
	btDefaultVehicleRaycaster* vehicle_raycaster;
	DebugDrawer* debug_draw;

	std::map<std::string,btCollisionShape*> shapes;
	std::map<std::string,PhysBody3D*> bodies;
	std::map<std::string,btDefaultMotionState*> motions;
	std::map<std::string,btTypedConstraint*> constraints;
	std::map<std::string, Component*> colliderComponents;
	std::map<std::string, PhysVehicle3D*> vehicles;

	btVector3 gravity;
	int steps;
};

class DebugDrawer : public btIDebugDraw
{
public:
	DebugDrawer() : line(0, 0, 0) 
	{}

	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
	void reportErrorWarning(const char* warningString);
	void draw3dText(const btVector3& location, const char* textString);
	void setDebugMode(int debugMode);
	int	 getDebugMode() const;

	DebugDrawModes mode;
	LinePrimitive line;
	Primitive point;
};

#endif //__ModulePhysic_H__