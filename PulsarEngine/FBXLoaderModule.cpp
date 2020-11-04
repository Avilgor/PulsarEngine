#include "Globals.h"
#include "Application.h"
#include "FBXLoaderModule.h"
#include "FileSystemModule.h"
#include "Mesh.h"
#include "Material.h"
#include "RES_Mesh.h"
#include "Timer.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/mesh.h"
#include "Assimp/include/material.h"
#include "Assimp/include/texture.h"
#include "Devil/include/IL/ilu.h"
#include "Devil/include/IL/ilut.h"

#include <string>

#pragma comment( lib, "Devil/lib/x86/DevIL.lib" )
#pragma comment( lib, "Devil/lib/x86/ILU.lib" )
#pragma comment( lib, "Devil/lib/x86/ILUT.lib" )
#pragma comment (lib, "Assimp/libx86/assimp.lib")

FBXLoaderModule::FBXLoaderModule()
{
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
}

FBXLoaderModule::~FBXLoaderModule()
{}

uint FBXLoaderModule::SaveMesh(RES_Mesh* mesh, char** fileBuffer)
{
	uint ranges[4] = { mesh->indexSize, mesh->verticesSize, mesh->normalsSize, mesh->textSize };

	uint size = sizeof(4) + sizeof(uint) + sizeof(uint) * mesh->indexSize + (sizeof(float) * mesh->verticesSize * 3)
		 + sizeof(float) * mesh->normalsSize * 3+ (sizeof(float) * mesh->textSize * 2);

	char* buffer = new char[size];
	char* cursor = buffer;
	LOG("Start Save");
	//Store ranges
	uint bytes = sizeof(ranges);
	memcpy(cursor, ranges, bytes);
	cursor += bytes;
	//LOG("Ranges");
	//Indices
	bytes = sizeof(uint) * mesh->indexSize;
	memcpy(cursor, mesh->indicesArray, bytes);
	cursor += bytes;
	//LOG("Indices");
	//Vertices
	bytes = sizeof(float) * mesh->verticesSize * 3;
	memcpy(cursor, mesh->verticesArray, bytes);
	cursor += bytes;
	//LOG("Vertices");
	//Normals
	if (mesh->normalsSize > 0)
	{
		bytes = sizeof(float) * mesh->normalsSize * 3;
		memcpy(cursor, mesh->normalsArray, bytes);
		cursor += bytes;
		//LOG("Normals");
	}
	//Text
	if (mesh->textSize > 0)
	{
		bytes = sizeof(float) * mesh->textSize * 2;
		memcpy(cursor, mesh->texturesArray, bytes);
		cursor += bytes;
		//LOG("Textures");
	}

	return size;
}

void FBXLoaderModule::SaveTexture(char* fileBuffer)
{
	/*char* buffer = nullptr;
	uint size = 0;


	if (size > 0)
	{
		App->fileSystem->Save("", buffer, size);

		RELEASE_ARRAY(buffer);
	}*/
}

void FBXLoaderModule::LoadMesh(RES_Mesh* mesh,char* buffer)
{
	Timer timer;
	timer.Start();

	char* cursor = buffer;

	uint ranges[4];
	uint bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);
	cursor += bytes;

	mesh->indexSize = ranges[0];
	mesh->verticesSize = ranges[1];
	mesh->normalsSize = ranges[2];
	mesh->textSize = ranges[3];

	// Load indices
	bytes = sizeof(uint) * mesh->indexSize;
	mesh->indicesArray = new uint[mesh->indexSize];
	memcpy(mesh->indicesArray, cursor, bytes);
	cursor += bytes;

	//load vertices
	bytes = sizeof(float) * mesh->verticesSize * 3;
	memcpy(mesh->verticesArray, cursor, bytes);
	cursor += bytes;

	//load normals
	bytes = sizeof(float) * mesh->normalsSize * 3;
	memcpy(mesh->normalsArray, cursor, bytes);
	cursor += bytes;

	//load texcoords
	bytes = sizeof(float) * mesh->textSize * 3;
	memcpy(mesh->texturesArray, cursor, bytes);
	cursor += bytes;

	LOG("%s loaded in %.3f s", mesh->name, timer.ReadSec());
}

bool FBXLoaderModule::ImportMesh(Mesh* mesh,const char* path)
{
	bool ret = true;
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{		
		mesh->path = path;
		mesh->name = App->fileSystem->GetFileName(path);

		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			RES_Mesh* newMesh = new RES_Mesh();
			newMesh->name = mesh->name;
			newMesh->name.append(std::to_string(i));
			//LOG("Name: %s",newMesh->name.c_str());
			newMesh->name = scene->mMeshes[i]->mName.C_Str();
			newMesh->verticesSize = scene->mMeshes[i]->mNumVertices;
			newMesh->verticesArray = new float[newMesh->verticesSize * 3];
			memcpy(newMesh->verticesArray, scene->mMeshes[i]->mVertices, sizeof(float) * newMesh->verticesSize * 3);
			//Indices
			if (scene->mMeshes[i]->HasFaces())
			{
				newMesh->indexSize = scene->mMeshes[i]->mNumFaces * 3;
				newMesh->indicesArray = new uint[newMesh->indexSize];

				for (uint a = 0; a < scene->mMeshes[i]->mNumFaces; a++)
				{
					if (scene->mMeshes[i]->mFaces[a].mNumIndices != 3)
					{
						LOG("Geometry face with != 3 indices!");
					}
					else memcpy(&newMesh->indicesArray[a * 3], scene->mMeshes[i]->mFaces[a].mIndices, 3 * sizeof(uint));
				}
			}

			//Normals
			if (scene->mMeshes[i]->HasNormals())
			{
				newMesh->normalsSize = newMesh->verticesSize;
				newMesh->normalsArray = new float[newMesh->normalsSize * 3];
				memcpy(newMesh->normalsArray, scene->mMeshes[i]->mNormals, sizeof(float) * newMesh->normalsSize * 3);
			}

			//Texture coords
			if (scene->mMeshes[i]->HasTextureCoords(0))
			{
				newMesh->textSize = newMesh->verticesSize;
				newMesh->texturesArray = new float[newMesh->verticesSize * 2];

				for (int a = 0; a < newMesh->textSize; a++)
				{
					newMesh->texturesArray[a * 2] = scene->mMeshes[i]->mTextureCoords[0][a].x;
					newMesh->texturesArray[a * 2 + 1] = scene->mMeshes[i]->mTextureCoords[0][a].y;
				}
			}

			char* buffer = nullptr;
			uint size = 0;	
			size = SaveMesh(newMesh, &buffer);
			//LOG("Size: %d", size);
			/*if (size > 0)
			{
				std::string pathfile = "Library/Meshes/";
				pathfile.append(newMesh->name.c_str());
				LOG("Path: %s", pathfile.c_str());
				App->fileSystem->Save(pathfile.c_str(), buffer, size);
			}*/
			RELEASE_ARRAY(buffer);
			LOG("End save");
			mesh->AddMesh(/*LoadMesh(i, scene,mesh->name.c_str())*/newMesh);
		}
		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading mesh %s", path);
	}
	
	return ret;
}
/*
RES_Mesh* FBXLoaderModule::LoadMesh(int num, const aiScene* scene,const char* name)
{

	
	return newMesh;
}
*/
/*
bool FBXLoaderModule::ImportAll(Mesh* mesh, Material* mat, const char* path)
{
	bool ret = true;
	ret = ImportMesh( mesh, path);
	if(ret) ImportMaterialFBX(mat, path);

	return ret;
}


bool FBXLoaderModule::ImportMaterialFBX(Material* mat, const char* path)
{
	bool ret = true;
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	
	if(scene != nullptr && scene->HasMaterials())
	{		
		for (uint i = 0; i < scene->mNumMaterials; ++i)
		{
			RES_Material* matInfo = new RES_Material();
			aiMaterial* material = scene->mMaterials[i];		
			matInfo->texturesNum = material->GetTextureCount(aiTextureType_DIFFUSE);
			std::string extension;
			std::string filename;
			std::string dir;
			App->fileSystem->SplitFilePath(path,&dir,&filename,&extension);
			aiString tempPath;

			material->GetTexture(aiTextureType_DIFFUSE, 0, &tempPath);
			//LOG("Texture base path: %s", tempPath.C_Str());
			matInfo->path = dir.append(tempPath.C_Str());
			aiColor4D color;
			material->Get(AI_MATKEY_COLOR_DIFFUSE, color);				
			matInfo->color = Color(color.r, color.g, color.b, color.a);

			aiString matName;
			material->Get(AI_MATKEY_NAME, matName);
			matInfo->name = matName.C_Str();
			mat->SaveMaterial(matInfo);
		}
		
		aiReleaseImport(scene);
		mat->GenerateBuffers();
	}
	else
	{
		LOG("Materials not found.");
	}

	return ret;
}

bool FBXLoaderModule::ImportMaterialFBX(Material* mat, const char* pathfbx, const char* pathtext)
{
	bool ret = true;
	const aiScene* scene = aiImportFile(pathfbx, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMaterials())
	{
		for (uint i = 0; i < scene->mNumMaterials; ++i)
		{
			RES_Material* matInfo = new RES_Material();
			aiMaterial* material = scene->mMaterials[i];
			matInfo->texturesNum = material->GetTextureCount(aiTextureType_DIFFUSE);
			matInfo->path = pathtext;
			aiColor4D color;
			material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
			matInfo->color = Color(color.r, color.g, color.b, color.a);

			aiString matName;
			material->Get(AI_MATKEY_NAME, matName);
			matInfo->name = matName.C_Str();
			mat->SaveMaterial(matInfo);
		}

		aiReleaseImport(scene);
		mat->GenerateBuffers();
	}
	else
	{
		LOG("Materials not found.");
	}

	return ret;
}*/