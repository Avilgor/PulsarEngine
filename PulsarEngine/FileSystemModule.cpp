#include "Globals.h"
#include "Application.h"
#include "FileSystemModule.h"
#include "PathNode.h"
#include "Material.h"
#include "PhysFS/include/physfs.h"
#include <fstream>
#include <filesystem>

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

FileSystemModule::FileSystemModule(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	char* base_path = SDL_GetBasePath();
	PHYSFS_init(nullptr);
	SDL_free(base_path);

	//Setting the working directory as the writing directory
	if (PHYSFS_setWriteDir(".") == 0)
		LOG("File System error while creating write dir: %s\n", PHYSFS_getLastError());

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

	return ret;
}


bool FileSystemModule::CleanUp()
{
	return true;
}

void FileSystemModule::CreateLibraryDirectories()
{
	CreateDir(LIBRARY_PATH);
	CreateDir(FOLDERS_PATH);
	CreateDir(MESHES_PATH);
	CreateDir(TEXTURES_PATH);
	CreateDir(MATERIALS_PATH);
	CreateDir(MODELS_PATH);
	CreateDir(ANIMATIONS_PATH);
	CreateDir(PARTICLES_PATH);
	CreateDir(SHADERS_PATH);
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
	//TODO: erase first annoying dot (".")
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

PathNode FileSystemModule::GetAllFiles(const char* directory, std::vector<std::string>* filter_ext, std::vector<std::string>* ignore_ext) const
{
	PathNode root;
	if (Exists(directory))
	{
		root.path = directory;
		App->fileSystem->SplitFilePath(directory, nullptr, &root.localPath);
		if (root.localPath == "")
			root.localPath = directory;

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

	output.append(*PHYSFS_getSearchPath()).append("/");
	output.append(PHYSFS_getRealDir(path)).append("/").append(path);
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

void FileSystemModule::LoadTexture(const char* path, MaterialInfo* mat)
{
	//std::string tempPath = "Assets/";
	//GetRealDir(path,tempPath);
	uint imageID = 0;

	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

	if (imageID == 0)
	{
		LOG("Could not create a texture buffer to load: %s, %d", path, ilGetError());
	}
	else if (ilLoadImage(path) == IL_FALSE)
	{
		LOG("Error trying to load the texture from %s", path);
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
		}
		else
		{
			LOG("Texture: %s loaded successfully");
		}
	}
}