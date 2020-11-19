#include "Globals.h"
#include "Application.h"
#include "FileSystemModule.h"
#include "PathNode.h"
#include "Scene.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Material.h"
#include "RES_Material.h"
#include "RES_Mesh.h"
#include "JSonHandler.h"


#include <fstream>
#include <filesystem>

#include "PhysFS/include/physfs.h"
#include "parson/parson.h"
#include "Assimp/include/cfileio.h"
#include "Assimp/include/types.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/mesh.h"
#include "Assimp/include/material.h"
#include "Assimp/include/texture.h"
#include "Devil/include/IL/ilu.h"
#include "Devil/include/IL/ilut.h"

#include <list>

#pragma comment (lib, "PhysFS/libx86/physfs.lib")
#pragma comment( lib, "Devil/lib/x86/DevIL.lib" )
#pragma comment( lib, "Devil/lib/x86/ILU.lib" )
#pragma comment( lib, "Devil/lib/x86/ILUT.lib" )
#pragma comment (lib, "Assimp/libx86/assimp.lib")

FileSystemModule::FileSystemModule(Application* app, bool start_enabled) : Module(app,"FileSystem" ,start_enabled)
{
	char* base_path = SDL_GetBasePath();
	PHYSFS_init(nullptr);
	SDL_free(base_path);

	//Setting the working directory as the writing directory
	if (PHYSFS_setWriteDir(".") == 0)
		LOG("File System error while creating write dir: %s\n", PHYSFS_getLastError());

	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	AddPath("."); //Adding ProjectFolder (working directory)
	AddPath("Assets");
	CreateLibraryDirectories();
}

FileSystemModule::~FileSystemModule()
{
	PHYSFS_deinit();
}


bool FileSystemModule::Init(Config& config)
{
	LOG("Loading File System...");
	bool ret = true;

	// Ask SDL for a write dir
	char* write_path = SDL_GetPrefPath(App->GetOrganizationName(), App->GetTitleName());
	
	SDL_free(write_path);
	App->LoadSettings();
	return ret;
}


bool FileSystemModule::CleanUp()
{
	return true;
}

void FileSystemModule::CreateLibraryDirectories()
{
	CreateDir(LIBRARY_PATH);
	//CreateDir(ASSETS_PATH);
	CreateDir(MESHES_PATH);
	CreateDir(TEXTURES_PATH);
	CreateDir(MATERIALS_PATH);
	//CreateDir(MODELS_PATH);
	//CreateDir(ANIMATIONS_PATH);
	//CreateDir(PARTICLES_PATH);
	//CreateDir(SHADERS_PATH);
	CreateDir(SCENES_PATH);
}

// Add a new zip file or folder
bool FileSystemModule::AddPath(const char* path_or_zip)
{
	bool ret = false;

	if (PHYSFS_mount(path_or_zip, nullptr, 1) == 0)
	{
		LOG("File System error while adding a path or zip: %s\n", PHYSFS_getLastError());
	}
	else
		ret = true;

	return ret;
}

// Check if a file exists
bool FileSystemModule::Exists(const char* file) const
{
	return PHYSFS_exists(file) != 0;
}

bool FileSystemModule::CreateDir(const char* dir)
{
	if (IsDirectory(dir) == false)
	{
		PHYSFS_mkdir(dir);
		return true;
	}
	return false;
}

// Check if a file is a directory
bool FileSystemModule::IsDirectory(const char* file) const
{
	return PHYSFS_isDirectory(file) != 0;
}

const char* FileSystemModule::GetWriteDir() const
{
	return PHYSFS_getWriteDir();
}

void FileSystemModule::DiscoverFiles(const char* directory, std::vector<std::string>& file_list, std::vector<std::string>& dir_list) const
{
	char** rc = PHYSFS_enumerateFiles(directory);
	char** i;

	for (i = rc; *i != nullptr; i++)
	{
		std::string str = std::string(directory) + std::string("/") + std::string(*i);

		if (IsDirectory(str.c_str()))
			dir_list.push_back(*i);
		else
			file_list.push_back(*i);
	}

	PHYSFS_freeList(rc);
}

void FileSystemModule::GetAllFilesWithExtension(const char* directory, const char* extension, std::vector<std::string>& file_list) const
{
	std::vector<std::string> files;
	std::vector<std::string> dirs;
	DiscoverFiles(directory, files, dirs);

	for (uint i = 0; i < files.size(); i++)
	{
		std::string ext;
		SplitFilePath(files[i].c_str(), nullptr, nullptr, &ext);

		if (ext == extension)
			file_list.push_back(files[i]);
	}
}

PathNode FileSystemModule::GetAllFiles(const char* directory, std::vector<std::string>* filter_ext, std::vector<std::string>* ignore_ext) 
{
	PathNode root;
	if (Exists(directory))
	{
		root.path = directory;
		SplitFilePath(directory, nullptr, &root.localPath);
		if (root.localPath == "") root.localPath = directory;

		std::vector<std::string> file_list, dir_list;
		DiscoverFiles(directory, file_list, dir_list);

		//Adding all child directories
		for (uint i = 0; i < dir_list.size(); i++)
		{
			std::string str = directory;
			str.append("/").append(dir_list[i]);
			root.children.push_back(GetAllFiles(str.c_str(), filter_ext, ignore_ext));
		}
		//Adding all child files
		for (uint i = 0; i < file_list.size(); i++)
		{
			//Filtering extensions
			bool filter = true, discard = false;
			if (filter_ext != nullptr)
			{
				filter = HasExtension(file_list[i].c_str(), *filter_ext);
			}
			if (ignore_ext != nullptr)
			{
				discard = HasExtension(file_list[i].c_str(), *ignore_ext);
			}
			if (filter == true && discard == false)
			{
				std::string str = directory;
				str.append("/").append(file_list[i]);
				root.children.push_back(GetAllFiles(str.c_str(), filter_ext, ignore_ext));
			}
		}
		root.isFile = HasExtension(root.path.c_str());
		if (root.isFile) root.localPath = GetFileAndExtension(directory);
		root.isLeaf = root.children.empty() == true;
	}
	return root;
}

void FileSystemModule::GetRealDir(const char* path, std::string& output) const
{
	output = PHYSFS_getBaseDir();

	std::string baseDir = PHYSFS_getBaseDir();
	std::string searchPath = *PHYSFS_getSearchPath();
	std::string realDir = PHYSFS_getRealDir(path);

	//output.append(*PHYSFS_getSearchPath()).append("/");
	//output.append(PHYSFS_getRealDir(path)).append("/").append(path);
	output.append(path);
}

std::string FileSystemModule::GetPathRelativeToAssets(const char* originalPath) const
{
	std::string ret;
	GetRealDir(originalPath, ret);

	return ret;
}

bool FileSystemModule::HasExtension(const char* path) const
{
	std::string ext = "";
	SplitFilePath(path, nullptr, nullptr, &ext);
	return ext != "";
}

bool FileSystemModule::HasExtension(const char* path, std::string extension) const
{
	std::string ext = "";
	SplitFilePath(path, nullptr, nullptr, &ext);
	return ext == extension;
}

bool FileSystemModule::HasExtension(const char* path, std::vector<std::string> extensions) const
{
	std::string ext = "";
	SplitFilePath(path, nullptr, nullptr, &ext);
	if (ext == "")
		return true;
	for (uint i = 0; i < extensions.size(); i++)
	{
		if (extensions[i] == ext)
			return true;
	}
	return false;
}

std::string FileSystemModule::NormalizePath(const char* full_path) const
{
	std::string newPath(full_path);
	for (int i = 0; i < newPath.size(); ++i)
	{
		if (newPath[i] == '\\')
			newPath[i] = '/';
	}
	return newPath;
}

void FileSystemModule::SplitFilePath(const char* full_path, std::string* path, std::string* file, std::string* extension) const
{
	if (full_path != nullptr)
	{
		std::string full(full_path);
		size_t pos_separator = full.find_last_of("\\/");
		size_t pos_dot = full.find_last_of(".");

		if (path != nullptr)
		{
			if (pos_separator < full.length())
				*path = full.substr(0, pos_separator + 1);
			else
				path->clear();
		}

		if (file != nullptr)
		{
			if (pos_separator < full.length())
				*file = full.substr(pos_separator + 1, pos_dot - pos_separator - 1);
			else
				*file = full.substr(0, pos_dot);
		}

		if (extension != nullptr)
		{
			if (pos_dot < full.length())
				*extension = full.substr(pos_dot + 1);
			else
				extension->clear();
		}
	}
}

unsigned int FileSystemModule::Load(const char* path, const char* file, char** buffer) const
{
	std::string full_path(path);
	full_path += file;
	return Load(full_path.c_str(), buffer);
}

// Read a whole file and put it in a new buffer
uint FileSystemModule::Load(const char* file, char** buffer) const
{
	uint ret = 0;
	PHYSFS_file* fs_file = PHYSFS_openRead(file);

	if (fs_file != nullptr)
	{
		PHYSFS_sint32 size = (PHYSFS_sint32)PHYSFS_fileLength(fs_file);

		if (size > 0)
		{
			*buffer = new char[size + 1];
			uint readed = (uint)PHYSFS_read(fs_file, *buffer, 1, size);
			if (readed != size)
			{
				LOG("File System error while reading from file %s: %s\n", file, PHYSFS_getLastError());
				RELEASE_ARRAY(buffer);
			}
			else
			{
				ret = readed;
				//Adding end of file at the end of the buffer. Loading a shader file does not add this for some reason
				(*buffer)[size] = '\0';
			}
		}

		if (PHYSFS_close(fs_file) == 0)
			LOG("File System error while closing file %s: %s\n", file, PHYSFS_getLastError());
	}
	else
		LOG("File System error while opening file %s: %s\n", file, PHYSFS_getLastError());

	return ret;
}

bool FileSystemModule::DuplicateFile(const char* file, const char* dstFolder, std::string& relativePath)
{
	std::string fileStr, extensionStr;
	SplitFilePath(file, nullptr, &fileStr, &extensionStr);

	relativePath = relativePath.append(dstFolder).append("/") + fileStr.append(".") + extensionStr;
	std::string finalPath = std::string(*PHYSFS_getSearchPath()).append("/") + relativePath;

	return DuplicateFile(file, finalPath.c_str());

}

bool FileSystemModule::DuplicateFile(const char* srcFile, const char* dstFile)
{
	std::ifstream src;
	src.open(srcFile, std::ios::binary);
	bool srcOpen = src.is_open();
	std::ofstream  dst(dstFile, std::ios::binary);
	bool dstOpen = dst.is_open();

	dst << src.rdbuf();

	src.close();
	dst.close();

	if (srcOpen && dstOpen)
	{
		LOG("[success] File Duplicated Correctly");
		return true;
	}
	else
	{
		LOG("[error] File could not be duplicated");
		return false;
	}
}

bool FileSystemModule::MoveFileTo(const char* file, const char* dst)
{
	bool ret = false;
	if(DuplicateFile(file, dst)) ret = Remove(file);
	if (ret)
	{
		LOG("File moved to %s", dst);
	}
	else LOG("Failed to move file %s",file);
	return ret;
}

int close_sdl_rwops(SDL_RWops* rw)
{
	RELEASE_ARRAY(rw->hidden.mem.base);
	SDL_FreeRW(rw);
	return 0;
}

// Save a whole buffer to disk
uint FileSystemModule::Save(const char* file, const void* buffer, unsigned int size, bool append) const
{
	unsigned int ret = 0;

	bool overwrite = PHYSFS_exists(file) != 0;
	PHYSFS_file* fs_file = (append) ? PHYSFS_openAppend(file) : PHYSFS_openWrite(file);

	if (fs_file != nullptr)
	{
		uint written = (uint)PHYSFS_write(fs_file, (const void*)buffer, 1, size);
		if (written != size)
		{
			LOG("[error] File System error while writing to file %s: %s", file, PHYSFS_getLastError());
		}
		else
		{
			if (append == true)
			{
				LOG("Added %u data to [%s%s]", size, GetWriteDir(), file);
			}
			else if (overwrite == true)
			{
				LOG("File [%s%s] overwritten with %u bytes", GetWriteDir(), file, size)
			}
			else
			{
				LOG("New file created [%s%s] of %u bytes", GetWriteDir(), file, size)
			}
			ret = written;
		}

		if (PHYSFS_close(fs_file) == 0)
			LOG("[error] File System error while closing file %s: %s", file, PHYSFS_getLastError());
	}
	else
		LOG("[error] File System error while opening file %s: %s", file, PHYSFS_getLastError());

	return ret;
}

bool FileSystemModule::Remove(const char* file)
{
	bool ret = false;

	if (file != nullptr)
	{
		//If it is a directory, we need to recursively remove all the files inside
		if (IsDirectory(file))
		{
			std::vector<std::string> containedFiles, containedDirs;
			PathNode rootDirectory = GetAllFiles(file);

			for (uint i = 0; i < rootDirectory.children.size(); ++i)
				Remove(rootDirectory.children[i].path.c_str());
		}

		if (PHYSFS_delete(file) != 0)
		{
			LOG("File deleted: [%s]", file);
			ret = true;
		}
		else
			LOG("File System error while trying to delete [%s]: %s", file, PHYSFS_getLastError());
	}

	return ret;
}

uint64 FileSystemModule::GetLastModTime(const char* filename)
{
	return PHYSFS_getLastModTime(filename);
}

std::string FileSystemModule::GetUniqueName(const char* path, const char* name) const
{
	std::vector<std::string> files, dirs;
	DiscoverFiles(path, files, dirs);

	std::string finalName(name);
	bool unique = false;

	for (uint i = 0; i < 50 && unique == false; ++i)
	{
		unique = true;

		//Build the compare name (name_i)
		if (i > 0)
		{
			finalName = std::string(name).append("_");
			if (i < 10)
				finalName.append("0");
			finalName.append(std::to_string(i));
		}

		//Iterate through all the files to find a matching name
		for (uint f = 0; f < files.size(); ++f)
		{
			if (finalName == files[f])
			{
				unique = false;
				break;
			}
		}
	}
	return finalName;
}

bool FileSystemModule::LoadTexture(const char* path, RES_Material* mat)
{
	/*
	* There is an error when a texture load failed,
	* making that any later load can't open texture file
	*/
	bool ret = true;
	uint imageID = 0;
	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

	if (imageID == 0)
	{
		LOG("Could not create a texture buffer to load: %s, %d", path, ilGetError());
		ret = false;
	}
	else if (ilLoadImage(path) == IL_FALSE)
	{
		LOG("Error trying to load the texture from %s", path);
		ret = false;
	}
	else
	{
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		mat->textureID = imageID;
		mat->textData = ilGetData();
		mat->textWidth = ilGetInteger(IL_IMAGE_WIDTH);
		mat->textHeight = ilGetInteger(IL_IMAGE_HEIGHT);

		ILenum error = ilGetError();

		if (error != IL_NO_ERROR)
		{
			LOG("%d: %s", error, iluErrorString(error));
			mat->textureID = -1;
			mat->textData = nullptr;
			mat->textWidth = mat->textHeight = 0;
			do error = ilGetError();
			while (error != IL_NO_ERROR);
			ret = false;
		}
		else
		{
			mat->name = GetFileName(path);
			SaveMaterial(mat);
			LOG("Texture: %s loaded successfully",path);
		}		
	}
	return ret;
}

std::string FileSystemModule::GetFilePath(const char* p)
{
	std::string path = "";
	std::string name = "";
	std::string extension = "";
	SplitFilePath(p, &path, &name, &extension);
	return path;
}

std::string FileSystemModule::GetFileExtension(const char* p)
{
	std::string path = "";
	std::string name = "";
	std::string extension = "";
	SplitFilePath(p,&path,&name,&extension);
	return extension;
}

std::string FileSystemModule::GetFileName(const char* p)
{
	std::string path = "";
	std::string name = "";
	std::string extension = "";
	SplitFilePath(p, &path, &name, &extension);
	return name;
}

std::string FileSystemModule::GetFileAndExtension(const char* p)
{
	std::string path = "";
	std::string name = "";
	std::string extension = "";
	SplitFilePath(p, &path, &name, &extension);
	name += ".";
	name = name.append(extension);
	return name;
}

std::string FileSystemModule::GetPathAndFile(const char* p)
{
	std::string path = "";
	std::string name = "";
	std::string extension = "";
	SplitFilePath(p, &path, &name, &extension);
	path = path.append(name);
	return path;
}

void FileSystemModule::GetDroppedFile(const char* path,GameObject* go,RES_Material* mat)
{
	LOG("Got dropped file");
	if (HasExtension(path, "fbx"))//Mesh
	{
		if (App->scene->state == SCENE_STOP)
		{
			if(go == nullptr) go = App->scene->GetActiveScene()->CreateEmptyGameobject();

			ImportFBX(path, go);	
		}
		else LOG("Cannot import files while scenne is running");
	}
	else if (HasExtension(path, "png") || HasExtension(path, "dds"))//Texture
	{
		if (!App->resourceManager->CheckMetaFile(GetFileAndExtension(path)))//Meta not found
		{
			if (App->scene->state == SCENE_STOP)
			{
				if (mat == nullptr) mat = new RES_Material();
				mat->name = "NewMaterial";
				mat->assetPath = path;
				mat->texturesNum = 1;
				//Load and save texture info
				if (LoadTexture(path, mat))
				{
					//Save material resource
					std::string resPath = App->resourceManager->Save_RES_Material(mat, GetFilePath(path).c_str());
					App->resourceManager->SaveResource(mat->resource);

					//Create asset file
					std::string tempId = App->GenerateUUID_V4();
					JSonHandler file;
					file.CreateArray("Objects");
					JSonHandler node = file.InsertNodeArray("Objects");
					node.CreateArray("Resources");
					node.InsertStringArray("Resources", mat->UUID.c_str());

					//Create meta file
					file.SaveString("Name", mat->name.c_str());
					Create_MetaFile(path, &file);
				}
			}
			else LOG("Cannot import files while scenne is running");
		}
		else
		{
			EngineResource* res = App->resourceManager->GetMetaResource(GetFileAndExtension(path));
			if (res != nullptr) mat = res->AsMaterial();
		}
		//Set texture to selected gameobjects
		if (App->editor->HasSelection())
		{
			std::vector<GameObject*> selected = App->editor->selectedGameObjects;
			for (std::vector<GameObject*>::iterator it = selected.begin(); it != selected.end(); ++it)
			{
				Component* comp = (*it)->GetFirstComponentType(MATERIAL_COMP);
				if (comp != nullptr)//Has material -> replace texture
				{ 
					if (comp->AsMaterial() != nullptr) comp->AsMaterial()->SetMaterial(mat);
				}
				else //Create material and add texture
				{
					comp = (*it)->AddComponent(MATERIAL_COMP);
					if (comp != nullptr)
					{
						if (comp->AsMaterial() != nullptr)
						{
							Component* comp2 = (*it)->GetFirstComponentType(MESH_COMP);
							if (comp2 != nullptr)//If has mesh, add material to mesh
							{
								comp2->AsMesh()->SetMaterial(comp->AsMaterial()->GetMaterial());
							}							
						}
					}
				}
			}
		}
	}
}

void FileSystemModule::SaveMeshBufferInfo(RES_Mesh* mesh)
{
	uint ranges[4] = { mesh->indexSize, mesh->verticesSize, mesh->normalsSize, mesh->textSize };

	uint size = sizeof(ranges) + (sizeof(uint) * mesh->indexSize) + (sizeof(float) * mesh->verticesSize * 3)
		+ (sizeof(float) * mesh->normalsSize * 3) + (sizeof(float) * mesh->textSize * 2);

	char* buffer = new char[size];
	char* cursor = buffer;
	
	//Store ranges
	uint bytes = sizeof(ranges);
	memcpy(cursor, ranges, bytes);
	cursor += bytes;

	//Indices
	bytes = sizeof(uint) * mesh->indexSize;
	memcpy(cursor, mesh->indicesArray, bytes);
	cursor += bytes;

	//Vertices
	bytes = sizeof(float) * mesh->verticesSize * 3;
	memcpy(cursor, mesh->verticesArray, bytes);
	cursor += bytes;

	//Normals
	if (mesh->normalsSize > 0)
	{
		bytes = sizeof(float) * mesh->normalsSize * 3;
		memcpy(cursor, mesh->normalsArray, bytes);
		cursor += bytes;
	}
	//Text
	if (mesh->textSize > 0)
	{
		bytes = sizeof(float) * mesh->textSize * 2;
		memcpy(cursor, mesh->texturesArray, bytes);
		cursor += bytes;
	}

	if (size > 0)
	{
		std::string pathfile = MESHES_PATH;
		pathfile.append(App->GenerateUUID_V4());
		pathfile.append(".meshbuff");
		Save(pathfile.c_str(), buffer, size);
		mesh->libPath = pathfile;
		RELEASE_ARRAY(buffer);
	}
}


void FileSystemModule::LoadMesh(RES_Mesh* mesh, char* buffer)
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
	mesh->verticesArray = new float[mesh->verticesSize*3];
	memcpy(mesh->verticesArray, cursor, bytes);
	cursor += bytes;

	//load normals
	bytes = sizeof(float) * mesh->normalsSize * 3;
	mesh->normalsArray = new float[mesh->normalsSize *3];
	memcpy(mesh->normalsArray, cursor, bytes);
	cursor += bytes;

	//load texcoords
	bytes = sizeof(float) * mesh->textSize * 2;
	mesh->texturesArray = new float[mesh->textSize * 2];
	memcpy(mesh->texturesArray, cursor, bytes);
	cursor += bytes;

	LOG("%s loaded in %.3f s", mesh->name.c_str(), timer.ReadSec());
}

void FileSystemModule::SaveMaterial(RES_Material* mat)
{
	char* buffer = nullptr;
	ILuint size;
	ILubyte* data;
	ilEnable(IL_FILE_OVERWRITE);
	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
	size = ilSaveL(IL_DDS, nullptr, 0);

	if (size > 0)
	{
		data = new ILubyte[size];
		if (ilSaveL(IL_DDS, data, size) > 0) buffer = (char*)data;		
	
		std::string pathfile = TEXTURES_PATH;
		pathfile.append(App->GenerateUUID_V4());
		pathfile += ".textbuff";
		Save(pathfile.c_str(), buffer, size);
		mat->libPath = pathfile;
		mat->bufferSize = int(size);
		RELEASE_ARRAY(data);
	}
}

void FileSystemModule::LoadMaterial(RES_Material* mat/*, char** buffer, uint size*/)
{
	if (mat != nullptr)
	{
		Timer timer;
		timer.Start();

		ILuint ImageName;
		ilGenImages(1, &ImageName);
		ilBindImage(ImageName);

		ilEnable(IL_ORIGIN_SET);
		ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

		if (ilLoadImage(mat->libPath.c_str()) == IL_FALSE)
		{
			LOG("Error trying to load the texture from %s", mat->libPath.c_str());
		}
		else
		{
			ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
			mat->textureID = ImageName;
			mat->textData = ilGetData();
			mat->textWidth = ilGetInteger(IL_IMAGE_WIDTH);
			mat->textHeight = ilGetInteger(IL_IMAGE_HEIGHT);
			//LOG("Text size H:%d/W:%d",mat->textWidth,mat->textHeight);
		}		
	}
	else LOG("Error loading texture to material.");
}

bool FileSystemModule::ImportFBX(const char* path, GameObject* parent)
{
	bool ret = true;
	std::string metaPath = GetFileAndExtension(path);

	if (App->resourceManager->CheckMetaFile(metaPath.c_str()) == false) //Meta file not found
	{
		if (App->scene->state == SCENE_STOP)
		{
			parent->name = GetFileName(path);
			const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

			if (scene != nullptr && scene->HasMeshes())
			{
				JSonHandler file;
				file.CreateArray("Objects");
				for (int i = 0; i < scene->mNumMeshes; i++)
				{
					GameObject* gameobject = parent->CreateChild();
					Component* temp = gameobject->AddComponent(MESH_COMP);
					if (temp != nullptr)
					{
						Mesh* meshcomp = temp->AsMesh();
						if (meshcomp != nullptr)
						{
							JSonHandler node = file.InsertNodeArray("Objects");
							node.CreateArray("Resources");
							RES_Mesh* newMesh = new RES_Mesh();
							newMesh->name = scene->mMeshes[i]->mName.C_Str();
							newMesh->name.append(std::to_string(i));
							newMesh->assetPath = path;

							//Vertex
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

							//Save mesh info
							SaveMeshBufferInfo(newMesh);

							//Set pulsar asset info
							node.InsertStringArray("Resources", newMesh->UUID.c_str());

							//Save mesh resource file
							std::string resPath = App->resourceManager->Save_RES_Mesh(newMesh, GetFilePath(path).c_str());

							//Save into resource manager map
							App->resourceManager->SaveResource(newMesh);

							//Load resource
							App->resourceManager->LoadResource(newMesh->UUID);

							if (scene->HasMaterials())
							{
								RES_Material* tempMat = ImportMaterialFBX(scene->mMaterials[scene->mMeshes[i]->mMaterialIndex], gameobject, path);
								meshcomp->SetMaterial(tempMat);
								node.InsertStringArray("Resources", tempMat->UUID.c_str());
							}
							meshcomp->SetMesh(newMesh);
						}
						else
						{
							LOG("Mesh component not found in go %s", gameobject->name.c_str());
							ret = false;
						}
					}
					else
					{
						LOG("Error creating mesh component");
						ret = false;
					}

				}
				//Save asset
				//std::string assetID = App->GenerateUUID_V4();
				//CreatePulsarAsset(&file, assetID, parent->name);

				//Create meta file
				file.SaveString("Name", parent->name.c_str());
				Create_MetaFile(path, &file);

				aiReleaseImport(scene);
			}
			else
			{
				LOG("Error loading mesh %s", path);
				ret = false;
			}
		}
		else LOG("Cannot import file while scene is running");
	}
	else //Meta file found
	{
		//LOG("Meta file exist");
		App->resourceManager->ImportFBXFromMeta(GetFileAndExtension(path),parent);
	}

	return ret;
}


void FileSystemModule::UnloadTexure(uint id)
{
	ilDeleteImages(1, &id);
}

void FileSystemModule::Create_MetaFile(std::string file, JSonHandler* node)
{
	//Save meta file
	node->SaveNum("LastModificationTime", GetLastModTime(file.c_str()));
	node->SaveString("UUID", App->GenerateUUID_V4().c_str());
	//node->SaveString("FilePath", GetFilePath(file.c_str()).c_str());

	//Write to file
	char* buffer = nullptr;
	uint size = node->Serialize(&buffer);
	std::string fileName = file;
	fileName.append(".meta");
	App->fileSystem->Save(fileName.c_str(), buffer, size);
	RELEASE_ARRAY(buffer);
	App->resourceManager->LoadMetaFiles();
}


/*void FileSystemModule::CreatePulsarAsset(JSonHandler* node, std::string uuid,std::string name)
{
	node->SaveString("UUID",uuid.c_str());
	node->SaveString("FileName",name.c_str());

	//Write to file
	char* buffer = nullptr;
	uint size = node->Serialize(&buffer);
	std::string fileName = ASSETS_PATH;
	fileName.append(uuid.c_str());
	fileName.append(".psasset");
	App->fileSystem->Save(fileName.c_str(), buffer, size);
	RELEASE_ARRAY(buffer);
}*/


RES_Material* FileSystemModule::ImportMaterialFBX(aiMaterial* material, GameObject* go,const char* fbxPath)
{
	RES_Material* matInfo = nullptr;

	aiString textPath;
	material->GetTexture(aiTextureType_DIFFUSE, 0, &textPath);
	
	//LOG("Meta path %s", textPath.C_Str());

	if (App->resourceManager->CheckMetaFile(textPath.C_Str()) == false ) //Meta file not found
	{
		if (App->scene->state == SCENE_STOP)
		{
			Component* tempComp = go->AddComponent(MATERIAL_COMP);
			if (tempComp != nullptr)
			{
				Material* mat = tempComp->AsMaterial();
				if (mat != nullptr)
				{
					JSonHandler file;
					//file.CreateArray("Objects");
					//JSonHandler node = file.InsertNodeArray("Objects");
					//node.CreateArray("Resources");
					matInfo = new RES_Material();
					matInfo->texturesNum = material->GetTextureCount(aiTextureType_DIFFUSE);
					std::string dir = GetFilePath(fbxPath);
					dir.append(textPath.C_Str());
					dir = NormalizePath(dir.c_str());
					matInfo->assetPath = dir;

					aiColor4D color;
					material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
					matInfo->color = Color(color.r, color.g, color.b, color.a);

					matInfo->name = GetFileName(dir.c_str());

					//Load and save texture info
					if (LoadTexture(dir.c_str(), matInfo))
					{
						//Save material resource
						std::string resPath = App->resourceManager->Save_RES_Material(matInfo, GetFilePath(fbxPath).c_str());
						App->resourceManager->SaveResource(matInfo);

						//Create asset file
						//std::string tempId = App->GenerateUUID_V4();
						//node.InsertStringArray("Resources", mat->UUID.c_str());
						//CreatePulsarAsset(&file, tempId, GetFileName(dir.c_str()));

						//Create meta file
						file.SaveString("Name", matInfo->name.c_str());
						file.SaveString("ResourceID", matInfo->UUID.c_str());
						Create_MetaFile(dir.c_str(), &file);

						//Load resource 
						App->resourceManager->LoadResource(matInfo->UUID);

						mat->SetMaterial(matInfo);
					}
				}
				else LOG("Material component not found in %s gameobject.", go->name.c_str());
			}
			else LOG("Failed to create material component.");
		}
		else LOG("Cannot import files while scene running");
	}
	else
	{
		//LOG("Material already have meta file");
		Component* tempComp = go->AddComponent(MATERIAL_COMP);
		if (tempComp != nullptr)
		{
			Material* mat = tempComp->AsMaterial();
			if (mat != nullptr)
			{
				matInfo = App->resourceManager->GetMetaResource(textPath.C_Str())->AsMaterial();
				mat->SetMaterial(matInfo);
			}			
		}
	}
	
	return matInfo;
}