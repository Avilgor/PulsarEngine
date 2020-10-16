#include "Globals.h"
#include "FBXLoaderModule.h"
#include "Mesh.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/mesh.h"

#include <list>

#pragma comment (lib, "Assimp/libx86/assimp.lib")

FBXLoaderModule::FBXLoaderModule(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

FBXLoaderModule::~FBXLoaderModule()
{}



bool FBXLoaderModule::Start()
{
	bool ret = true;
	return ret;
}

update_status FBXLoaderModule::Update(float dt)
{
	if (!Meshes.empty())
	{
		for (std::list<Mesh*>::iterator it = Meshes.begin(); it != Meshes.end(); ++it)
		{
			(*it)->Render();
		}
	}
	return UPDATE_CONTINUE;
}

bool FBXLoaderModule::CleanUp()
{
	bool ret = true;
	for (std::list<Mesh*>::iterator it = Meshes.begin(); it != Meshes.end(); ++it)
	{
		delete (*it);
	}
	Meshes.clear();

	return ret;
}

bool FBXLoaderModule::ImportMesh(const char* path)
{
	bool ret = true;
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			Mesh* mesh = new Mesh();
			mesh->verticesSize = scene->mMeshes[i]->mNumVertices;
			mesh->verticesArray = new float[mesh->verticesSize * 3];
			memcpy(mesh->verticesArray, scene->mMeshes[i]->mVertices, sizeof(float) * mesh->verticesSize * 3);
			LOG("Vertices = %d",mesh->verticesSize);

			//Indices
			if (scene->mMeshes[i]->HasFaces())
			{
				mesh->indexSize = scene->mMeshes[i]->mNumFaces * 3;
				mesh->indicesArray = new uint[mesh->indexSize];

				for (uint a = 0; a < scene->mMeshes[i]->mNumFaces; a++)
				{
					if (scene->mMeshes[i]->mFaces[a].mNumIndices != 3)
					{
						LOG("Geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&mesh->indicesArray[a * 3], scene->mMeshes[i]->mFaces[a].mIndices, 3 * sizeof(uint));
					}
				}
			}

			//Normals
			if (scene->mMeshes[i]->HasNormals())
			{
				mesh->normalsSize = mesh->verticesSize;
				mesh->normalsArray = new float[mesh->normalsSize * 3];
				memcpy(mesh->normalsArray, scene->mMeshes[i]->mNormals, sizeof(float) * mesh->normalsSize * 3);
			}

			//Textures
			if (scene->mMeshes[i]->HasTextureCoords(0))
			{
				mesh->textSize = mesh->verticesSize;
				mesh->texturesArray = new float[mesh->verticesSize * 2];

				for (int a = 0; a < mesh->textSize; a++)
				{
					mesh->texturesArray[a * 2] = scene->mMeshes[i]->mTextureCoords[0][a].x;
					mesh->texturesArray[a * 2 + 1] = scene->mMeshes[i]->mTextureCoords[0][a].y;
				}
			}
			mesh->LoadImportedMesh();
			Meshes.push_back(mesh);
		}
		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading scene % s", path);
	}
	
	return ret;
}