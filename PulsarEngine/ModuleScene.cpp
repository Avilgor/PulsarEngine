#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "Primitive.h"
#include "Glew\include\GL\glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/lib/Win32/glew32.lib") /* link Glew lib   */

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleScene::~ModuleScene()
{}

bool ModuleScene::Init()
{
	return true;
}

// Load assets
bool ModuleScene::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;	

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	p = new PlanePrimitive(0, 1, 0, 0);
	p->axis = true;

	//cube = new Cube();
	//cube->SetPos(0,1,0);

	return ret;
}

// Load assets
bool ModuleScene::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// PreUpdate
update_status ModuleScene::PreUpdate(float dt)
{
	
	return UPDATE_CONTINUE;
}

// Update
update_status ModuleScene::Update(float dt)
{
	p->Render();
	//cube->Render();

	//glLineWidth(2.0f);
    //GLuint vIndex[] = { 0,1,2, 0,2,3, 1,4,5, 1,5,2, 6,0,3, 6,3,7, 3,2,5, 3,5,7, 6,4,1, 6,1,0, 4,6,7, 4,7,5};
    //GLfloat vArray[] = { 0.0f,0.0f,0.0f, 1.0f,0.0f,0.0f, 1.0f,1.0f,0.0f, 0.0f,1.0f,0.0f, 1.0f,0.0f,-1.0f, 1.0f,1.0f,-1.0f, 0.0f,0.0f,-1.0f, 0.0f,1.0f,-1.0f };
    /*GLfloat vArray[] = { 0.0f,0.0f,0.0f, 1.0f,0.0f,0.0f, 1.0f,1.0f,0.0f,    1.0f,1.0f,0.0f, 0.0f,1.0f,0.0f, 0.0f,0.0f,0.0f,    1.0f,0.0f,0.0f,  1.0f,0.0f,-1.0f, 1.0f,1.0f,-1.0f,   
    1.0f,0.0f,0.0f, 1.0f,1.0f,-1.0f, 1.0f,1.0f,0.0f,    0.0f,0.0f,-1.0f, 0.0f,0.0f,0.0f, 0.0f,1.0f,0.0f,    0.0f,0.0f,-1.0f, 0.0f,1.0f,0.0f, 0.0f,1.0f,-1.0f, 
    0.0f,1.0f,0.0f, 1.0f,1.0f,0.0f, 1.0f,1.0f,-1.0f,    0.0f,1.0f,0.0f, 1.0f,1.0f,-1.0f, 0.0f,1.0f,-1.0f,   1.0f,0.0f,-1.0f, 0.0f,0.0f,-1.0f, 0.0f,1.0f,-1.0f,  
    1.0f,0.0f,-1.0f, 0.0f,1.0f,-1.0f, 1.0f,1.0f,-1.0f,   0.0f,0.0f,-1.0f, 1.0f,0.0f,-1.0f, 1.0f,0.0f,0.0f,     0.0f,0.0f,-1.0f, 1.0f,0.0f,0.0f, 0.0f,0.0f,0.0f };*/
    /*uint id = 0;
    uint indexID = 1;
    uint indexNum = sizeof(vIndex) / sizeof(*vIndex);
    int vertices = (sizeof(vArray) / sizeof(*vArray)) / 3;
    glGenBuffers(1, (GLuint*) &(id));
    glBindBuffer(GL_ARRAY_BUFFER,id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices*3, vArray,GL_STATIC_DRAW);
    glGenBuffers(1, (GLuint*)&(indexID));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indexNum, vIndex, GL_STATIC_DRAW);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER,id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexID);
    glVertexPointer(3, GL_FLOAT, 0, NULL);
    glDrawElements(GL_TRIANGLES, indexNum, GL_UNSIGNED_INT, NULL);
    //glDrawArrays(GL_TRIANGLES,0, vertices);
    glDisableClientState(GL_VERTEX_ARRAY);*/

    App->renderer3D->RenderPiramyd(0.0f, 0.0f, 0.0f);
    App->renderer3D->RenderCube(2.0f,0.0f,0.0f);
    //App->renderer3D->RenderSphere(0.5f,4,8,-3.0f,1.0f,0.0f);

    /*glBegin(GL_TRIANGLES);  
    
    GLfloat v0[] = { 0.0f,0.0f,0.0f };//face left bot
    GLfloat v1[] = { 1.0f,0.0f,0.0f };//face right bot
    GLfloat v2[] = { 1.0f,1.0f,0.0f };//face right top
    GLfloat v3[] = { 0.0f,1.0f,0.0f };//face left top
    GLfloat v4[] = { 1.0f,0.0f,-1.0f };//right bot
    GLfloat v5[] = { 1.0f,1.0f,-1.0f };//right top
    GLfloat v6[] = { 0.0f,0.0f,-1.0f };//left bot
    GLfloat v7[] = { 0.0f,1.0f,-1.0f };//left top
    

    
    //Front
    glVertex3fv(v0);
    glVertex3fv(v1);
    glVertex3fv(v2);

    glVertex3fv(v2);    
    glVertex3fv(v3);
    glVertex3fv(v0);

    //Right
    glVertex3fv(v1);    
    glVertex3fv(v4);
    glVertex3fv(v5);

    glVertex3fv(v1);   
    glVertex3fv(v5);
    glVertex3fv(v2);

    //Left
    glVertex3fv(v6);    
    glVertex3fv(v0);
    glVertex3fv(v3);

    glVertex3fv(v6); 
    glVertex3fv(v3);
    glVertex3fv(v7);

    //Top
    glVertex3fv(v6);
    glVertex3fv(v0);
    glVertex3fv(v3);

    glVertex3fv(v6);
    glVertex3fv(v3);
    glVertex3fv(v7);

    //Back
    glVertex3fv(v6);
    glVertex3fv(v0);
    glVertex3fv(v3);

    glVertex3fv(v6);
    glVertex3fv(v3);
    glVertex3fv(v7);

    //Bottom
    glVertex3fv(v6);
    glVertex3fv(v0);
    glVertex3fv(v3);

    glVertex3fv(v6);
    glVertex3fv(v3);
    glVertex3fv(v7);
    

    glEnd();

	glLineWidth(1.0f);*/

	return UPDATE_CONTINUE;
}

// PostUpdate
update_status ModuleScene::PostUpdate(float dt)
{	

	return UPDATE_CONTINUE;
}
