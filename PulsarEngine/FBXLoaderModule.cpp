#include "Globals.h"
#include "Application.h"
#include "FBXLoaderModule.h"
#include "FileSystemModule.h"
#include "Mesh.h"
#include "Material.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/mesh.h"
#include "Assimp/include/material.h"
#include "Assimp/include/texture.h"
#include "Devil/include/IL/ilu.h"
#include "Devil/include/IL/ilut.h"

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

void FBXLoaderModule::Save()
{

}

void FBXLoaderModule::Load()
{

}

bool FBXLoaderModule::ImportMesh(Mesh* mesh,const char* path)
{
	bool ret = true;
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		mesh->path = path;

		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			mesh->AddMesh(LoadMesh(i, scene));
		}
		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading mesh %s", path);
	}
	
	return ret;
}

MeshInfo FBXLoaderModule::LoadMesh(int num, const aiScene* scene)
{
	MeshInfo newMesh;
	newMesh.name = scene->mMeshes[num]->mName.C_Str();
	newMesh.verticesSize = scene->mMeshes[num]->mNumVertices;
	newMesh.verticesArray = new float[newMesh.verticesSize * 3];
	memcpy(newMesh.verticesArray, scene->mMeshes[num]->mVertices, sizeof(float) * newMesh.verticesSize * 3);

	//Indices
	if (scene->mMeshes[num]->HasFaces())
	{
		newMesh.indexSize = scene->mMeshes[num]->mNumFaces * 3;
		newMesh.indicesArray = new uint[newMesh.indexSize];

		for (uint a = 0; a < scene->mMeshes[num]->mNumFaces; a++)
		{
			if (scene->mMeshes[num]->mFaces[a].mNumIndices != 3)
			{
				LOG("Geometry face with != 3 indices!");
			}
			else memcpy(&newMesh.indicesArray[a * 3], scene->mMeshes[num]->mFaces[a].mIndices, 3 * sizeof(uint));
			
		}
	}

	//Normals
	if (scene->mMeshes[num]->HasNormals())
	{
		newMesh.normalsSize = newMesh.verticesSize;
		newMesh.normalsArray = new float[newMesh.normalsSize * 3];
		memcpy(newMesh.normalsArray, scene->mMeshes[num]->mNormals, sizeof(float) * newMesh.normalsSize * 3);
	}

	//Texture coords
	if (scene->mMeshes[num]->HasTextureCoords(0))
	{
		newMesh.textSize = newMesh.verticesSize;
		newMesh.texturesArray = new float[newMesh.verticesSize * 2];

		for (int a = 0; a < newMesh.textSize; a++)
		{
			newMesh.texturesArray[a * 2] = scene->mMeshes[num]->mTextureCoords[0][a].x;
			newMesh.texturesArray[a * 2 + 1] = scene->mMeshes[num]->mTextureCoords[0][a].y;
		}
	}
	return newMesh;
}


bool FBXLoaderModule::ImportAll(Mesh* mesh, Material* mat, const char* path)
{
	bool ret = true;
	ret = ImportMesh( mesh, path);
	if(ret) ImportMaterial(mat, path);

	return ret;
}

bool FBXLoaderModule::ImportMaterial(Material* mat, const char* path)
{
	bool ret = true;
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	
	if(scene->HasMaterials())
	{		
		for (uint i = 0; i < scene->mNumMaterials; ++i)
		{
			MaterialInfo matInfo;
			aiMaterial* material = scene->mMaterials[i];		
			matInfo.texturesNum = material->GetTextureCount(aiTextureType_DIFFUSE);
			std::string extension;
			std::string filename;
			std::string dir;
			App->fileSystem->SplitFilePath(path,&dir,&filename,&extension);
			aiString tempPath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &tempPath);
			matInfo.path = dir.append(tempPath.C_Str());
			aiColor4D color;
			material->Get(AI_MATKEY_COLOR_DIFFUSE, color);				
			matInfo.color = Color(color.r, color.g, color.b, color.a);

			aiString matName;
			material->Get(AI_MATKEY_NAME, matName);
			matInfo.name = matName.C_Str();
			mat->SaveMaterial(matInfo);
		}
		
		aiReleaseImport(scene);
		mat->GenerateBuffer();
	}
	else
	{
		LOG("Materials not found.");
	}

	return ret;
}