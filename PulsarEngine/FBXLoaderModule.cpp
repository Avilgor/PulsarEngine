#include "Globals.h"
#include "FBXLoaderModule.h"
#include "Mesh.h"
#include "Material.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/mesh.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

FBXLoaderModule::FBXLoaderModule()
{}

FBXLoaderModule::~FBXLoaderModule()
{}


bool FBXLoaderModule::ImportMesh(Mesh* mesh,const char* path)
{
	bool ret = true;
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		mesh->path = path;

		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			//mesh->name = scene->mMeshes[i]->mName.C_Str();
			MeshInfo newMesh;
			newMesh.name = scene->mMeshes[i]->mName.C_Str();
			newMesh.verticesSize = scene->mMeshes[i]->mNumVertices;
			newMesh.verticesArray = new float[newMesh.verticesSize * 3];
			memcpy(newMesh.verticesArray, scene->mMeshes[i]->mVertices, sizeof(float) * newMesh.verticesSize * 3);
			LOG("Vertices = %d", newMesh.verticesSize);
			/*mesh->verticesSize = scene->mMeshes[i]->mNumVertices;
			mesh->verticesArray = new float[mesh->verticesSize * 3];
			memcpy(mesh->verticesArray, scene->mMeshes[i]->mVertices, sizeof(float) * mesh->verticesSize * 3);
			LOG("Vertices = %d", mesh->verticesSize);*/

			//Indices
			if (scene->mMeshes[i]->HasFaces())
			{
				newMesh.indexSize = scene->mMeshes[i]->mNumFaces * 3;
				newMesh.indicesArray = new uint[newMesh.indexSize];
				//mesh->indexSize = scene->mMeshes[i]->mNumFaces * 3;
				//mesh->indicesArray = new uint[mesh->indexSize];

				for (uint a = 0; a < scene->mMeshes[i]->mNumFaces; a++)
				{
					if (scene->mMeshes[i]->mFaces[a].mNumIndices != 3)
					{
						LOG("Geometry face with != 3 indices!");
					}
					else
					{
						//memcpy(&mesh->indicesArray[a * 3], scene->mMeshes[i]->mFaces[a].mIndices, 3 * sizeof(uint));
						memcpy(&newMesh.indicesArray[a * 3], scene->mMeshes[i]->mFaces[a].mIndices, 3 * sizeof(uint));
					}
				}
			}

			//Normals
			if (scene->mMeshes[i]->HasNormals())
			{
				/*mesh->normalsSize = mesh->verticesSize;
				mesh->normalsArray = new float[mesh->normalsSize * 3];
				memcpy(mesh->normalsArray, scene->mMeshes[i]->mNormals, sizeof(float) * mesh->normalsSize * 3);*/
				newMesh.normalsSize = newMesh.verticesSize;
				newMesh.normalsArray = new float[newMesh.normalsSize * 3];
				memcpy(newMesh.normalsArray, scene->mMeshes[i]->mNormals, sizeof(float) * newMesh.normalsSize * 3);
			}

			//Texture coords
			if (scene->mMeshes[i]->HasTextureCoords(0))
			{
				//mesh->textSize = mesh->verticesSize;
				//mesh->texturesArray = new float[mesh->verticesSize * 2];
				newMesh.textSize = newMesh.verticesSize;
				newMesh.texturesArray = new float[newMesh.verticesSize * 2];

				//for (int a = 0; a < mesh->textSize; a++)
				for (int a = 0; a < newMesh.textSize; a++)
				{
					//mesh->texturesArray[a * 2] = scene->mMeshes[i]->mTextureCoords[0][a].x;
					//mesh->texturesArray[a * 2 + 1] = scene->mMeshes[i]->mTextureCoords[0][a].y;
					newMesh.texturesArray[a * 2] = scene->mMeshes[i]->mTextureCoords[0][a].x;
					newMesh.texturesArray[a * 2 + 1] = scene->mMeshes[i]->mTextureCoords[0][a].y;
				}
			}
			mesh->AddMesh(newMesh);
		}
		aiReleaseImport(scene);
		mesh->GenerateBuffers();
	}
	else
	{
		LOG("Error loading mesh %s", path);
	}
	
	return ret;
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
	
	/*
	if (scene->HasTextures())
	{
		if (scene->HasTextures())
		{

		}
		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading textures %s", path);
	}*/

	if(scene->HasMaterials())
	{
		if (scene->HasMaterials())
		{
			for (uint i = 0; i < scene->mNumMaterials; ++i)
			{
				MaterialInfo matInfo;
				aiMaterial* material = scene->mMaterials[i];
				matInfo.mat = material;			
				matInfo.texturesNum = material->GetTextureCount(aiTextureType_DIFFUSE);
				aiString path;
				material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
				matInfo.path = path.C_Str();				

				aiColor4D color;
				material->Get(AI_MATKEY_COLOR_DIFFUSE, color);				
				matInfo.color = Color(color.r, color.g, color.b, color.a);

				aiString matName;
				material->Get(AI_MATKEY_NAME, matName);
				matInfo.name = matName.C_Str();
				mat->SaveMaterial(matInfo);
			}
		}
		aiReleaseImport(scene);
	}
	else
	{
		LOG("Materials not found.");
	}

	return ret;
}