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
	~JSonHandler();

	//Getters
	bool GetBool();
	int GetInt();
	float GetFloat();
	std::string GetString();
 
	//Setters
	void SaveBool(bool val);
	void SaveInt(int val);
	void SaveFloat(float val);
	void SaveString(std::string val);

private:

	JSON_Value* root = nullptr; 
	JSON_Object* node = nullptr;
	JSON_Array* arrayJSon = nullptr;
	int size = 0;
};

#endif //__JSonHandler_H__