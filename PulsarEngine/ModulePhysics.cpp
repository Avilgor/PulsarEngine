#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "PhysBody3D.h"
#include "MathGeoLib/include/MathGeoLib.h"
//#include "Bullet/src/btBulletDynamicsCommon.h"
//#include "Bullet/src/btBulletCollisionCommon.h"
//#include "Bullet/src/btLinearMathAll.cpp"
#include "Bullet/include/btBulletDynamicsCommon.h"
#include "Bullet/include/btBulletCollisionCommon.h"

#include <vector>

#ifdef _DEBUG
#pragma comment (lib, "Bullet/libx86/BulletDynamics_debug.lib")
#pragma comment (lib, "Bullet/libx86/BulletCollision_debug.lib")
#pragma comment (lib, "Bullet/libx86/LinearMath_debug.lib")
#else
#pragma comment (lib, "Bullet/libx86/BulletDynamics.lib")
#pragma comment (lib, "Bullet/libx86/BulletCollision.lib")
#pragma comment (lib, "Bullet/libx86/LinearMath.lib")
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, "Physics", start_enabled)
{
	debug = true;

	collision_conf = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collision_conf);
	broad_phase = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	debug_draw = new DebugDrawer();
}

ModulePhysics::~ModulePhysics()
{
	delete debug_draw;
	delete solver;
	delete broad_phase;
	delete dispatcher;
	delete collision_conf;
}

bool ModulePhysics::Init()
{
	
	return true;
}

bool ModulePhysics::Start()
{
	LOG("Starting module physics...");
	bool ret = true;

	world = new btDiscreteDynamicsWorld(dispatcher, broad_phase, solver, collision_conf);
	world->setDebugDrawer(debug_draw);
	world->setGravity(GRAVITY);
	vehicle_raycaster = new btDefaultVehicleRaycaster(world);

	// Big plane as ground
	{
		btCollisionShape* colShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);

		btDefaultMotionState* myMotionState = new btDefaultMotionState();
		btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f, myMotionState, colShape);

		btRigidBody* body = new btRigidBody(rbInfo);
		world->addRigidBody(body);
	}

	return ret;
}

update_status ModulePhysics::PreUpdate(float dt)
{
	world->stepSimulation(dt, 15);

	int numManifolds = world->getDispatcher()->getNumManifolds();
	
	for (int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject* obA = (btCollisionObject*)(contactManifold->getBody0());
		btCollisionObject* obB = (btCollisionObject*)(contactManifold->getBody1());

		int numContacts = contactManifold->getNumContacts();
		if (numContacts > 0)
		{
			PhysBody3D* pbodyA = (PhysBody3D*)obA->getUserPointer();
			PhysBody3D* pbodyB = (PhysBody3D*)obB->getUserPointer();

			if (pbodyA && pbodyB)
			{
				if (pbodyA->listener != nullptr) pbodyA->listener->OnCollision(pbodyA,pbodyB);
				if (pbodyB->listener != nullptr) pbodyB->listener->OnCollision(pbodyB, pbodyA);
			}
		}
	}

	return UPDATE_CONTINUE;
}

update_status ModulePhysics::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if (debug == true)
	{
		world->debugDrawWorld();

		// Render vehicles
		/*for (int i = 0; i < vehicles.size(); i++)
		{ 
			item->data->Render();		
		}*/

		/*if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		{
			PhysSphere s(1);
			s.SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);
			float force = 30.0f;
			AddBody(s)->Push(-(App->camera->Z.x * force), -(App->camera->Z.y * force), -(App->camera->Z.z * force));
		}*/
	}

	return UPDATE_CONTINUE;
}

update_status ModulePhysics::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}


bool ModulePhysics::CleanUp()
{
	LOG("Unloading physics...");
	// Remove from the world all collision bodies
	for (int i = world->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = world->getCollisionObjectArray()[i];
		world->removeCollisionObject(obj);
	}

	for (std::map<std::string, btTypedConstraint*>::iterator it = constraints.begin(); it != constraints.end(); ++it)
	{
		world->removeConstraint((*it).second);
		delete (*it).second;
	}
	constraints.clear();

	for (std::map<std::string, btDefaultMotionState*>::iterator it = motions.begin(); it != motions.end(); ++it)
	{
		delete (*it).second;
	}

	for (std::map<std::string, btCollisionShape*>::iterator it = shapes.begin(); it != shapes.end(); ++it)
	{
		delete (*it).second;
	}

	for (std::map<std::string, PhysBody3D*>::iterator it = bodies.begin(); it != bodies.end(); ++it)
	{
		delete (*it).second;
	}

	//for (int i = 0; i < vehicles.size(); i++) delete vehicles[i];
	//vehicles.clear();

	delete vehicle_raycaster;
	delete world;
	return true;
}

/*void ModulePhysics::RemoveConstraint(btTypedConstraint* constraint)
{
	world->removeConstraint(constraint);
}

void ModulePhysics::RemoveBody(btRigidBody* body)
{
	world->removeRigidBody(body);
}*/

void ModulePhysics::RemoveCollider(std::string uuid)
{

}


PhysBody3D* ModulePhysics::AddBody(SphereCollider* sphere, float mass)
{
	btSphereShape* shape = new btSphereShape(btScalar(sphere->rad));
	btCollisionShape* colShape = shape;
	//sphere->shape = shape;
	shapes.emplace(sphere->UUID, colShape);

	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(sphere->GetTransform());

	btVector3 localInertia(0, 0, 0);
	if (mass != 0.f)
		colShape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	motions.emplace(sphere->UUID, myMotionState);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);
	PhysBody3D* pbody = new PhysBody3D(body);


	//pbody->collType = coll;
	body->setUserPointer(pbody);
	world->addRigidBody(body);
	bodies.emplace(sphere->UUID, pbody);
	//sphere.body = pbody;

	return pbody;
}


PhysBody3D* ModulePhysics::AddBody(BoxCollider* cube, float mass)
{
	btBoxShape* shape = new btBoxShape(btVector3(cube->scale.x * 0.5f, cube->scale.y * 0.5f, cube->scale.z * 0.5f));
	btCollisionShape* colShape = shape;
	//cube->shape = shape;
	shapes.emplace(cube->UUID,colShape);
	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(cube->GetTransform());
	btVector3 localInertia(0, 0, 0);
	if (mass != 0.f)
		colShape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	motions.emplace(cube->UUID,myMotionState);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);
	PhysBody3D* pbody = new PhysBody3D(body);

	body->setUserPointer(pbody);
	world->addRigidBody(body);
	bodies.emplace(cube->UUID,pbody);
	cube->body = pbody;

	return pbody;
}

/*
PhysBody3D* ModulePhysics::AddBody(Cylinder& cylinder, float mass)
{
	btCollisionShape* colShape = new btCylinderShapeX(btVector3(cylinder.height * 0.5f, cylinder.radius, 0.0f));
	shapes.push_back(colShape);

	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(&cylinder.transform);

	btVector3 localInertia(0, 0, 0);
	if (mass != 0.f)
		colShape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	motions.push_back(myMotionState);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);
	PhysBody3D* pbody = new PhysBody3D(body);

	body->setUserPointer(pbody);
	world->addRigidBody(body);
	bodies.push_back(pbody);
	//cylinder.body = pbody;

	return pbody;
}*/

// ---------------------------------------------------------
/*PhysVehicle3D* ModulePhysics::AddVehicle(const VehicleInfo& info)
{
	btCompoundShape* comShape = new btCompoundShape();
	shapes.push_back(comShape);

	btCollisionShape* colShape = new btBoxShape(btVector3(info.chassis_size.x * 0.5f, info.chassis_size.y * 0.5f, info.chassis_size.z * 0.5f));
	shapes.push_back(colShape);

	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(btVector3(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z));

	comShape->addChildShape(trans, colShape);

	btTransform startTransform;
	startTransform.setIdentity();

	btVector3 localInertia(0, 0, 0);
	comShape->calculateLocalInertia(info.mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(info.mass, myMotionState, comShape, localInertia);

	btRigidBody* _body = new btRigidBody(rbInfo);
	_body->setContactProcessingThreshold(BT_LARGE_FLOAT);
	_body->setActivationState(DISABLE_DEACTIVATION);

	world->addRigidBody(_body);
	_body->collType = CAR;

	btRaycastVehicle::btVehicleTuning tuning;
	tuning.m_frictionSlip = info.frictionSlip;
	tuning.m_maxSuspensionForce = info.maxSuspensionForce;
	tuning.m_maxSuspensionTravelCm = info.maxSuspensionTravelCm;
	tuning.m_suspensionCompression = info.suspensionCompression;
	tuning.m_suspensionDamping = info.suspensionDamping;
	tuning.m_suspensionStiffness = info.suspensionStiffness;

	btRaycastVehicle* vehicle = new btRaycastVehicle(tuning, _body, vehicle_raycaster);

	vehicle->setCoordinateSystem(0, 1, 2);

	for (int i = 0; i < info.num_wheels; ++i)
	{
		btVector3 conn(info.wheels[i].connection.x, info.wheels[i].connection.y, info.wheels[i].connection.z);
		btVector3 dir(info.wheels[i].direction.x, info.wheels[i].direction.y, info.wheels[i].direction.z);
		btVector3 axis(info.wheels[i].axis.x, info.wheels[i].axis.y, info.wheels[i].axis.z);

		vehicle->addWheel(conn, dir, axis, info.wheels[i].suspensionRestLength, info.wheels[i].radius, tuning, info.wheels[i].front);
	}
	// ---------------------
	
	PhysVehicle3D* pvehicle = new PhysVehicle3D(_body, vehicle, info);
	_body->setUserPointer(pvehicle);
	//pvehicle->body = _body;
	world->addVehicle(vehicle);
	vehicles.push_back(pvehicle);
	
	return pvehicle;
}*/

// ---------------------------------------------------------
void ModulePhysics::AddConstraintP2P(PhysBody3D& bodyA, PhysBody3D& bodyB, const vec3& anchorA, const vec3& anchorB, std::string id)
{
	btTypedConstraint* p2p = new btPoint2PointConstraint(
		*(bodyA.body),
		*(bodyB.body),
		btVector3(anchorA.x, anchorA.y, anchorA.z),
		btVector3(anchorB.x, anchorB.y, anchorB.z));
	world->addConstraint(p2p);
	constraints.emplace(id,p2p);
	p2p->setDbgDrawSize(2.0f);
}


void ModulePhysics::AddConstraintHinge(PhysBody3D& bodyA, PhysBody3D& bodyB, const vec3& anchorA, const vec3& anchorB, const vec3& axisA, const vec3& axisB, std::string id,bool disable_collision)
{
	btHingeConstraint* hinge = new btHingeConstraint(
		*(bodyA.body),
		*(bodyB.body),
		btVector3(anchorA.x, anchorA.y, anchorA.z),
		btVector3(anchorB.x, anchorB.y, anchorB.z),
		btVector3(axisA.x, axisA.y, axisA.z),
		btVector3(axisB.x, axisB.y, axisB.z));

	world->addConstraint(hinge, disable_collision);
	constraints.emplace(id,hinge);
	hinge->setDbgDrawSize(2.0f);
}

// =============================================
DebugDrawer::DebugDrawer()
{
	line.dir = { 0, 0, 0 };
}

void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	line.pos = { from.getX(), from.getY(), from.getZ() };
	line.dir = {to.getX(), to.getY(), to.getZ()};
	App->renderer3D->RenderLine(line.pos,line.dir);
}

void DebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	//point.transform.translate(PointOnB.getX(), PointOnB.getY(), PointOnB.getZ());
	//point.Render();
}

void DebugDrawer::reportErrorWarning(const char* warningString)
{
	LOG("Bullet warning: %s", warningString);
}

void DebugDrawer::draw3dText(const btVector3& location, const char* textString)
{
	LOG("Bullet draw text: %s", textString);
}

void DebugDrawer::setDebugMode(int debugMode)
{
	mode = (DebugDrawModes)debugMode;
}

int	 DebugDrawer::getDebugMode() const
{
	return mode;
}
