#include "Globals.h"
#include "JSonHandler.h"
#include "parson/parson.h"


JSonHandler::JSonHandler()
{
	root = json_value_init_object();
	node = json_value_get_object(root);
	arrayJSon = json_value_get_array(json_value_init_array());
}

//Constructor used for data read
JSonHandler::JSonHandler(const char* buffer)
{
	root = json_parse_string(buffer);
	if (root) node = json_value_get_object(root);	
}

JSonHandler::JSonHandler(JSON_Object* obj) 
{
	node = obj;
}

JSonHandler::~JSonHandler()
{
	if (root) json_value_free(root);	
}

void SetArray(JSON_Array* arrayJ)
{

}

//Getters
bool JSonHandler::GetBool(const char* name)
{
	if (json_object_has_value_of_type(node, name, JSONBoolean))
		return json_object_get_boolean(node, name);
	return false;
}


double JSonHandler::GetNum(const char* name)
{
	if (json_object_has_value_of_type(node, name, JSONNumber))
		return json_object_get_boolean(node, name);
	return 0;
}

std::string JSonHandler::GetString(const char* name)
{
	if (json_object_has_value_of_type(node, name, JSONString))
		return json_object_get_string(node, name);
	return "";
}

JSonHandler JSonHandler::GetNode(const char* name)
{
	return JSonHandler(json_object_get_object(node, name));
}

//Setters
void JSonHandler::SaveBool(const char* name, bool val)
{
	json_object_set_boolean(node, name, val);
}

void JSonHandler::SaveInt(const char* name, int val)
{
	json_object_set_number(node, name, val);
}

void JSonHandler::SaveFloat(const char* name, float val)
{
	json_object_set_number(node, name, val);
}

void JSonHandler::SaveString(const char* name, const char* val)
{
	json_object_set_string(node, name, val);
}