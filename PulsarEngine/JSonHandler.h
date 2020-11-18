#ifndef __JSonHandler_H__
#define __JSonHandler_H__

#include "parson\parson.h"
#include "Globals.h"
#include "MathGeoLib\include\MathGeoLib.h"

#include <string>
#include <map>

struct json_object_t;
typedef struct json_object_t JSON_Object;

struct json_value_t;
typedef struct json_value_t  JSON_Value;

struct json_array_t;
typedef struct json_array_t  JSON_Array;


struct JSonArrayData
{
public:
	void Clean()
	{
		delete dataArray;
	}
	JSON_Array* dataArray = nullptr;
	int size = 0;
};

class JSonHandler
{
public:
	JSonHandler();
	JSonHandler(const char* buffer);
	JSonHandler(JSON_Object* obj);
	~JSonHandler();

	uint Serialize(char** buffer);
	bool NodeExist();
	void CreateArray(const char* name);
	JSonHandler CreateNode(const char* name);
	bool LoadArray(const char* name);

	//Getters Node
	int GetNodeItemsCount();
	bool GetBool(const char* name);
	double GetNum(const char* name);
	std::string GetString(const char* name);
	JSonHandler GetNode(const char* name);
	double GetNumArray(const char* arName, uint index);
	std::string GetStringArray(const char* arName, uint index);
	bool GetBoolArray(const char* arName,uint index);	
	JSonHandler GetNodeArray(const char* arName,uint index);
	int GetArrayCount(const char* arName);

	//Setters Node	
	void SaveBool(const char* name, bool val);
	void SaveNum(const char* name,double val);
	void SaveString(const char* name, const char* val);
	void InsertBoolArray(const char* arName, bool val);
	void InsertNumArray(const char* arName, double val);
	void InsertStringArray(const char* arName, const char* val);
	JSonHandler InsertNodeArray(const char* arName);
	

private:

	JSON_Value* root = nullptr; 
	JSON_Object* node = nullptr;
	std::map<std::string, JSonArrayData> nodeArrays;
};

#endif //__JSonHandler_H__