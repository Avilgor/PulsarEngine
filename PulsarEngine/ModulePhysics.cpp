#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "PhysBody3D.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "Bullet/include/btBulletDynamicsCommon.h"
#include "Bullet/include/btBulletCollisionCommon.h"

#include <vector>

//#ifdef _DEBUG
#pragma comment (lib, "Bullet/libx86/BulletDynamics_debug.lib")
#pragma comment (lib, "Bullet/libx86/BulletCollision_debug.lib")
#pragma comment (lib, "Bullet/libx86/LinearMath_debug.lib")
/*#else
#pragma comment (lib, "Bullet/libx86/BulletDynamics.lib")
#pragma comment (lib, "Bullet/libx86/BulletCollision.lib")
#pragma comment (lib, "Bullet/libx86/LinearMath.lib")
#endif*/

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, "Physics", start_enabled)
{
	runningSimulation = false;
	simulationPause = false;
	steps = 15;
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
	gravity.setX(0.0f);
	gravity.setY(-9.81f);
	gravity.setZ(0.0f);
	world = new btDiscreteDynamicsWorld(dispatcher, broad_phase, solver, collision_conf);
	world->setDebugDrawer(debug_draw);
	world->setGravity(gravity);
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
	for (std::map<std::string, Component*>::iterator it = colliderComponents.begin(); it != colliderComponents.end(); ++it)
	{
		if ((*it).second->AsBoxCollider() != nullptr)
		{
			(*it).second->AsBoxCollider()->UpdateTransform();
		}
		else if ((*it).second->AsSphereCollider() != nullptr)
		{
			(*it).second->AsSphereCollider()->UpdateTransform();
		}
		else if ((*it).second->AsCapsuleCollider() != nullptr)
		{
			(*it).second->AsCapsuleCollider()->UpdateTransform();
		}
	}

	if (runningSimulation && !simulationPause && world != nullptr)
	{		
		world->stepSimulation(dt, steps);
		
		if (world->getDispatcher() != nullptr)
		{
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
						if (pbodyA->listener != nullptr) pbodyA->listener->OnCollision(pbodyA, pbodyB);
						if (pbodyB->listener != nullptr) pbodyB->listener->OnCollision(pbodyB, pbodyA);
					}
				}
			}
		}

		if(colliderComponents.size() > 0)
		{
			for (std::map<std::string, Component*>::iterator it = colliderComponents.begin(); it != colliderComponents.end(); ++it)
			{
				if ((*it).second->AsBoxCollider() != nullptr)
				{
					(*it).second->AsBoxCollider()->PhysicsUpdate();
				}
				else if ((*it).second->AsSphereCollider() != nullptr)
				{
					(*it).second->AsSphereCollider()->PhysicsUpdate();
				}
				else if ((*it).second->AsCapsuleCollider() != nullptr)
				{
					(*it).second->AsCapsuleCollider()->PhysicsUpdate();
				}
			}
		}
	}
	return UPDATE_CONTINUE;
}

update_status ModulePhysics::Update(float dt)
{
	return UPDATE_CONTINUE;
}

void ModulePhysics::ToggleSimulation(bool val)
{
	runningSimulation = val;
	if (runningSimulation)
	{
		/*delete world;
		delete vehicle_raycaster;
		world = new btDiscreteDynamicsWorld(dispatcher, broad_phase, solver, collision_conf);
		world->setDebugDrawer(debug_draw);
		world->setGravity(gravity);		
		vehicle_raycaster = new btDefaultVehicleRaycaster(world);
		*/
		for (std::map<std::string, PhysBody3D*>::iterator it = bodies.begin(); it != bodies.end(); ++it)
		{
			(*it).second->body->clearForces();
			btVector3 zeroVector(0, 0, 0);
			(*it).second->body->setLinearVelocity(zeroVector);
			(*it).second->body->setAngularVelocity(zeroVector);
			//world->addRigidBody((*it).second->body);
		}
		/*
		for (std::map<std::string, btTypedConstraint*>::iterator it = constraints.begin(); it != constraints.end(); ++it)
		{
			world->addConstraint((*it).second);
		}

		for (std::map<std::string, PhysVehicle3D*>::iterator it = vehicles.begin(); it != vehicles.end(); ++it)
		{
			world->addVehicle((*it).second->vehicle);
		}*/

		LOG("Physics simulation ON");
		LOG("Total bodies: %d", bodies.size()); 
		LOG("Total constraints: %d",constraints.size());
		LOG("Total vehicles: %d", vehicles.size());
	}
	else
	{
		simulationPause = false;
		for (std::map<std::string, PhysBody3D*>::iterator it = bodies.begin(); it != bodies.end(); ++it)
		{
			(*it).second->body->clearForces();
			btVector3 zeroVector(0, 0, 0);
			(*it).second->body->setLinearVelocity(zeroVector);
			(*it).second->body->setAngularVelocity(zeroVector);
		}
		LOG("Physics simulation OFF");
	}
}

void ModulePhysics::ToggleSimulationPause(bool val)
{
	if (runningSimulation)
	{
		simulationPause = val;
		if (simulationPause)
		{
			LOG("Physics simulation PAUSED");
		}
		else
		{
			LOG("Physics simulation RESUMED");
		}
	}
}

void ModulePhysics::SetGravity(float3 val)
{
	if (!runningSimulation)
	{
		gravity.setX(val.x);
		gravity.setY(val.y);
		gravity.setZ(val.z);
	}
}

float3 ModulePhysics::GetGravity()
{
	return float3(gravity.getX(),gravity.getY(),gravity.getZ());
}

void ModulePhysics::SetSimulationSteps(int val)
{
	if (!runningSimulation)
	{
		steps = val;
	}
}

void ModulePhysics::ResetModule()
{

}

void ModulePhysics::DebugDrawBody(btRigidBody* body)
{
	if (body != nullptr) world->debugDrawObject(body->getWorldTransform(), body->getCollisionShape(), btVector3(0.0f, 1.0f, 0.0f));
}

update_status ModulePhysics::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}


bool ModulePhysics::CleanUp()
{
	LOG("Unloading physics...");
	// Remove from the world all collision bodies
	for (std::map<std::string, PhysBody3D*>::iterator it = bodies.begin(); it != bodies.end(); ++it)
	{
		world->removeRigidBody((*it).second->body);
	}

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
	motions.clear();

	for (std::map<std::string, btCollisionShape*>::iterator it = shapes.begin(); it != shapes.end(); ++it)
	{
		delete (*it).second;
	}
	shapes.clear();

	for (std::map<std::string, PhysBody3D*>::iterator it = bodies.begin(); it != bodies.end(); ++it)
	{
		delete (*it).second;
	}
	bodies.clear();

	for (std::map<std::string, PhysVehicle3D*>::iterator it = vehicles.begin(); it != vehicles.end(); ++it)
	{
		delete (*it).second;
	}
	vehicles.clear();


	delete vehicle_raycaster;
	delete world;
	return true;
}

void ModulePhysics::RemoveConstraint(btTypedConstraint* constraint,std::string id)
{
	//LOG("Constraint id: %s",id.c_str());
	world->removeConstraint(constraint);
	if (!constraints.empty())
	{
		if (constraints.find(id) != constraints.end()) constraints.erase(id);
	}
}

void ModulePhysics::RemoveConstraint(std::string id)
{
	if (!constraints.empty())
	{
		if (constraints.find(id) != constraints.end())
		{
			world->removeConstraint(constraints[id]);
			constraints.erase(id);
		}
	}
}

void ModulePhysics::RemoveBody(btRigidBody* body, std::string id)
{
	world->removeRigidBody(body);
	if (bodies.find(id) != bodies.end()) bodies.erase(id);	
}

void ModulePhysics::AddBody(PhysBody3D* body,std::string id)
{
	world->addRigidBody(body->body);
	bodies.emplace(id,body);
}

void ModulePhysics::AddConstraint(btTypedConstraint* con, std::string id)
{
	world->addConstraint(con);
	constraints.emplace(id, con);
}

PhysBody3D* ModulePhysics::GetBodyByUUID(std::string id)
{
	if (bodies.find(id) != bodies.end()) return bodies[id];	
	else return nullptr;
}

Component* ModulePhysics::GetColliderByUUID(std::string id)
{
	if (colliderComponents.find(id) != colliderComponents.end()) return colliderComponents[id];
	else return nullptr;
}

void ModulePhysics::RemoveCollider(std::string uuid)
{
	if (bodies.find(uuid) != bodies.end())
	{
		world->removeRigidBody(bodies[uuid]->body);
	}

	if (motions.find(uuid) != motions.end())
	{
		delete motions[uuid];
		motions.erase(uuid);
	}

	if (shapes.find(uuid) != shapes.end())
	{
		delete shapes[uuid];
		shapes.erase(uuid);
	}

	if (bodies.find(uuid) != bodies.end())
	{
		delete bodies[uuid];
		bodies.erase(uuid);
	}

	if (colliderComponents.find(uuid) != colliderComponents.end())
	{
		colliderComponents.erase(uuid);
	}
}


PhysBody3D* ModulePhysics::AddBody(SphereCollider* sphere, float mass)
{
	btCollisionShape* shape = new btSphereShape(btScalar(0.5f));
	shapes.emplace(sphere->UUID, shape);

	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(sphere->GetTransformMat().Transposed().ptr());

	btVector3 localInertia(0, 0, 0);
	if (mass != 0.f) shape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	motions.emplace(sphere->UUID, myMotionState);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);
	PhysBody3D* pbody = new PhysBody3D(body);

	body->setUserPointer(pbody);
	world->addRigidBody(body);
	bodies.emplace(sphere->UUID, pbody);
	sphere->body = pbody;
	colliderComponents.emplace(sphere->UUID, sphere->component);

	return pbody;
}

PhysBody3D* ModulePhysics::AddBody(BoxCollider* cube, float3 size,float mass)
{
	btCollisionShape* shape = new btBoxShape(btVector3(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f));
	shapes.emplace(cube->UUID, shape);

	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(cube->GetTransformMat().Transposed().ptr());

	btVector3 localInertia(0, 0, 0);
	if (mass != 0.f) shape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	motions.emplace(cube->UUID,myMotionState);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);
	PhysBody3D* pbody = new PhysBody3D(body);

	body->setUserPointer(pbody);
	world->addRigidBody(body);
	bodies.emplace(cube->UUID,pbody);

	cube->body = pbody;
	colliderComponents.emplace(cube->UUID,cube->component);
	

	return pbody;
}

PhysBody3D* ModulePhysics::AddBody(CapsuleCollider* capsule, float mass)
{
	btCollisionShape* shape = new btCapsuleShape(btScalar(0.5f),btScalar(2.0f));
	shapes.emplace(capsule->UUID, shape);

	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(capsule->GetTransformMat().Transposed().ptr());

	btVector3 localInertia(0, 0, 0);
	if (mass != 0.f) shape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	motions.emplace(capsule->UUID, myMotionState);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);
	PhysBody3D* pbody = new PhysBody3D(body);

	body->setUserPointer(pbody);
	world->addRigidBody(body);
	bodies.emplace(capsule->UUID, pbody);
	capsule->body = pbody;
	colliderComponents.emplace(capsule->UUID, capsule->component);

	return pbody;
}

PhysVehicle3D* ModulePhysics::AddVehicle(const VehicleInfo& info)
{
	btCompoundShape* comShape = new btCompoundShape();
	shapes.emplace(info.uuid,comShape);

	btCollisionShape* colShape = new btBoxShape(btVector3(info.chassis_size.x * 0.5f, info.chassis_size.y * 0.5f, info.chassis_size.z * 0.5f));
	//shapes.emplace(info.uuid,colShape);

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
	
	PhysVehicle3D* pvehicle = new PhysVehicle3D(_body, vehicle, info);
	_body->setUserPointer(pvehicle);
	world->addVehicle(vehicle);
	vehicles.emplace(info.uuid,pvehicle);
	
	return pvehicle;
}

// ---------------------------------------------------------
btTypedConstraint* ModulePhysics::AddConstraintPoint(PhysBody3D& bodyA, PhysBody3D& bodyB, const vec3& anchorA, const vec3& anchorB, std::string id)
{
	btTypedConstraint* p2p = new btPoint2PointConstraint(
		*(bodyA.body),
		*(bodyB.body),
		btVector3(anchorA.x, anchorA.y, anchorA.z),
		btVector3(anchorB.x, anchorB.y, anchorB.z));
	world->addConstraint(p2p);
	constraints.emplace(id,p2p);
	p2p->setDbgDrawSize(2.0f);
	return p2p;
}


btTypedConstraint* ModulePhysics::AddConstraintHinge(PhysBody3D& bodyA, PhysBody3D& bodyB, const vec3& anchorA, const vec3& anchorB, const vec3& axisA, const vec3& axisB, std::string id,bool disable_collision)
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
	return hinge;
}

btTypedConstraint* ModulePhysics::AddConstraintSlider(PhysBody3D& bodyA, PhysBody3D& bodyB, const btTransform& anchorA, const btTransform& anchorB, std::string id)
{
	btTypedConstraint* slider = new btSliderConstraint(
		*(bodyA.body),
		*(bodyB.body),
		anchorA,
		anchorB,
		true);
	world->addConstraint(slider);
	constraints.emplace(id, slider);
	slider->setDbgDrawSize(2.0f);
	return slider;
}

btTypedConstraint* ModulePhysics::AddConstraintCone(PhysBody3D& bodyA, PhysBody3D& bodyB, const btTransform& anchorA, const btTransform& anchorB, std::string id)
{
	btTypedConstraint* cone = new btConeTwistConstraint(
		*(bodyA.body),
		*(bodyB.body),
		anchorA,
		anchorB);
	world->addConstraint(cone);
	constraints.emplace(id, cone);
	cone->setDbgDrawSize(2.0f);
	return cone;
}

// =============================================
void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	line.origin.Set(from.getX(), from.getY(), from.getZ());
	line.destination.Set(to.getX(), to.getY(), to.getZ());
	line.color.Set(color.getX(), color.getY(), color.getZ());
	line.Render();
}

void DebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	point.transform.translate(PointOnB.getX(), PointOnB.getY(), PointOnB.getZ());
	point.color.Set(color.getX(), color.getY(), color.getZ());
	point.Render();
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
