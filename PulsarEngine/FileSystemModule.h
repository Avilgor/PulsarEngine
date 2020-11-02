#ifndef __FileSystemModule_H__
#define __FileSystemModule_H__

#include "Module.h"
#include "Globals.h"

#include <vector>

struct SDL_RWops;
int close_sdl_rwops(SDL_RWops* rw);

struct aiFileIO;

class Config;
struct PathNode;
class RES_Material;
class Mesh;
class Material;
struct aiScene;
class RES_Mesh;

struct json_array_t;
typedef json_array_t JSON_Array;
struct json_value_t;
typedef json_value_t JSON_Value;
struct json_object_t;
typedef json_object_t JSON_Object;

class FileSystemModule : public Module
{
public:
	FileSystemModule(Application* app, bool start_enabled = true);
	~FileSystemModule();

	// Called before render is available
	bool Init(Config& config);

	// Called before quitting
	bool CleanUp();

	void CreateLibraryDirectories();

	// Utility functions
	bool AddPath(const char* path_or_zip);
	bool Exists(const char* file) const;
	bool CreateDir(const char* dir);
	bool IsDirectory(const char* file) const;
	const char* GetWriteDir() const;
	void DiscoverFiles(const char* directory, std::vector<std::string>& file_list, std::vector<std::string>& dir_list) const;
	void GetAllFilesWithExtension(const char* directory, const char* extension, std::vector<std::string>& file_list) const;
	PathNode GetAllFiles(const char* directory, std::vector<std::string>* filter_ext = nullptr, std::vector<std::string>* ignore_ext = nullptr) const;
	void GetRealDir(const char* path, std::string& output) const;
	std::string GetPathRelativeToAssets(const char* originalPath) const;

	bool HasExtension(const char* path) const;
	bool HasExtension(const char* path, std::string extension) const;
	bool HasExtension(const char* path, std::vector<std::string> extensions) const;

	std::string NormalizePath(const char* path) const;
	void SplitFilePath(const char* full_path, std::string* path, std::string* file = nullptr, std::string* extension = nullptr) const;

	// Open for Read/Write
	unsigned int Load(const char* path, const char* file, char** buffer) const;
	unsigned int Load(const char* file, char** buffer) const;

	bool DuplicateFile(const char* file, const char* dstFolder, std::string& relativePath);
	bool DuplicateFile(const char* srcFile, const char* dstFile);

	unsigned int Save(const char* file, const void* buffer, unsigned int size, bool append = false) const;
	bool Remove(const char* file);

	uint64 GetLastModTime(const char* filename);
	std::string GetUniqueName(const char* path, const char* name) const;
	void LoadTexture(const char* path, RES_Material* mat);
	void GetDroppedFile(const char* path);
	std::string GetFileExtension(const char* path);
	std::string GetFileName(const char* path);
	std::string GetFileAndExtension(const char* path);
	//JSON_Array* LoadJSon();
	//void SaveJSon();


	bool ImportMesh(Mesh* mesh, const char* path);
	bool ImportMaterialFBX(Material* mesh, const char* path);
	bool ImportMaterialFBX(Material* mesh, const char* pathfbx, const char* pathtext);
	bool ImportAll(Mesh* mesh, Material* mat, const char* path);
	void SaveMesh(RES_Mesh* mesh/*, char** fileBuffer*/);
	void LoadMesh(RES_Mesh* mesh, char* fileBuffer);
	void SaveMaterial(RES_Material* mat);
	void LoadMaterial(RES_Material* mat, char** buffer,uint size);
};

#endif //__FileSystemModule_H__