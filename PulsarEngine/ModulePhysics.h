#ifndef __ModulePhysic_H__
#define __ModulePhysic_H__

#include "Module.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
//#include "Bullet/src/btBulletDynamicsCommon.h"
#include "Bullet/include/btBulletDynamicsCommon.h"
#include "MathGeoLib/include/MathGeoLib.h"

#include <map>
#include <string>

#define GRAVITY btVector3(0.0f, -10.0f, 0.0f) 

class DebugDrawer;
class PhysBody3D;
//struct PhysVehicle3D;
//struct VehicleInfo;

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

	PhysBody3D* AddBody(BoxCollider* box, float mass = 1.0f);
	PhysBody3D* AddBody(SphereCollider* sphere, float mass = 1.0f);
	//PhysVehicle3D* AddVehicle(const VehicleInfo& info);

	void AddConstraintP2P(PhysBody3D& bodyA, PhysBody3D& bodyB, const vec3& anchorA, const vec3& anchorB, std::string id);
	void AddConstraintHinge(PhysBody3D& bodyA, PhysBody3D& bodyB, const vec3& anchorA, const vec3& anchorB, const vec3& axisS, const vec3& axisB, std::string id, bool disable_collision = false);

	void RemoveCollider(std::string uuid);
	void RemoveConstraint(btTypedConstraint* constraint);
	void RemoveBody(btRigidBody* body);

private:

	bool debug;

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
};

class DebugDrawer : public btIDebugDraw
{
public:
	DebugDrawer();

	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
	void reportErrorWarning(const char* warningString);
	void draw3dText(const btVector3& location, const char* textString);
	void setDebugMode(int debugMode);
	int	 getDebugMode() const;

	DebugDrawModes mode;
	Line line;
};

#endif //__ModulePhysic_H__