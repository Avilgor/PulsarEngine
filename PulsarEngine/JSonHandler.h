#ifndef __JSonHandler_H__
#define __JSonHandler_H__

#include "parson\parson.h"
#include "Globals.h"
#include "MathGeoLib\include\MathGeoLib.h"

#include <string>
#include <vector>

struct json_object_t;
typedef struct json_object_t JSON_Object;

struct json_value_t;
typedef struct json_value_t  JSON_Value;

struct json_array_t;
typedef struct json_array_t  JSON_Array;

class JSonHandler
{
public:
	JSonHandler();
	JSonHandler(const char* buffer);
	JSonHandler(JSON_Object* obj);
	~JSonHandler();

	//void SetArray(JSON_Array* arrayJ);
	uint Serialize(char** buffer);
	bool NodeExist();

	//Getters Node
	bool GetBool(const char* name);
	double GetNum(const char* name);
	std::string GetString(const char* name);
	JSonHandler GetNode(const char* name);
	//JSON_Array GetArray(const char* name);

	//Setters Node
	void SaveBool(const char* name, bool val);
	void SaveInt(const char* name,int val);
	void SaveFloat(const char* name,float val);
	void SaveString(const char* name, const char* val);
	JSonHandler SetNode(const char* name);

private:

	JSON_Value* root = nullptr; 
	JSON_Object* node = nullptr;
	JSON_Array* arrayJSon = nullptr;
	int size = 0;
};

#endif //__JSonHandler_H__