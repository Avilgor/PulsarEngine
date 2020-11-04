#include "Globals.h"
#include "JSonHandler.h"
#include "parson/parson.h"

#include <map>

JSonHandler::JSonHandler()
{
	root = json_value_init_object();
	node = json_value_get_object(root);
}

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
	nodeArrays.clear();	
	if (root) json_value_free(root);	
}

bool JSonHandler::NodeExist()
{
	if (root != nullptr) return true;
	else return false;
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
		return json_object_get_number(node, name);
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

int JSonHandler::GetNodeItemsCount()
{
	if (node != nullptr) json_object_get_count(node);
	else return 0;
}

double JSonHandler::GetNumArray(const char* arName, uint index)
{
	if (!nodeArrays.empty())
	{
		if (nodeArrays.find(arName) != nodeArrays.end()) //Array exists
		{
			return json_array_get_number(nodeArrays[arName].dataArray, index);
		}
		else return 0;
	} 
	else return 0;
}

std::string JSonHandler::GetStringArray(const char* arName, uint index)
{
	if (!nodeArrays.empty())
	{
		if (nodeArrays.find(arName) != nodeArrays.end()) //Array exists
		{
			return json_array_get_string(nodeArrays[arName].dataArray, index);
		}
		else return "";
	} 
	else return "";
}

bool JSonHandler::GetBoolArray(const char* arName, uint index)
{
	if (!nodeArrays.empty())
	{
		if (nodeArrays.find(arName) != nodeArrays.end()) //Array exists
		{
			return json_array_get_boolean(nodeArrays[arName].dataArray, index);
		}
		else return false;
	}
	else return false;
}

int JSonHandler::GetArrayCount(const char* arName)
{
	if (!nodeArrays.empty())
	{
		if (nodeArrays.find(arName) != nodeArrays.end()) //Array exists
		{
			return nodeArrays[arName].size;
		}
		else return false;
	}
	else return false;
}

JSonHandler JSonHandler::GetNodeArray(const char* arName, uint index)
{
	if (!nodeArrays.empty())
	{
		for (std::map<std::string, JSonArrayData>::iterator it = nodeArrays.begin(); it != nodeArrays.end(); ++it)
		{			
			//LOG("Map name: %s",(*it).first.c_str());
			//LOG("Array name: %s",arName);
			if ((*it).first.compare(arName) == 0)
			{
				JSonHandler temp(json_array_get_object((*it).second.dataArray, index));
				return temp;
			}
		}
	}	
	return JSonHandler();
}

//Setters
void JSonHandler::SaveBool(const char* name, bool val)
{
	json_object_set_boolean(node, name, val);
}

void JSonHandler::SaveNum(const char* name, double val)
{
	json_object_set_number(node, name, val);
}


void JSonHandler::SaveString(const char* name, const char* val)
{
	json_object_set_string(node, name, val);
}

JSonHandler JSonHandler::CreateNode(const char* name)
{
	json_object_set_value(node, name, json_value_init_object());
	return JSonHandler(json_object_get_object(node, name));
}

void JSonHandler::InsertBoolArray(const char* arName, bool val)
{
	if (!nodeArrays.empty())
	{
		if (nodeArrays.find(arName) != nodeArrays.end()) //Array exists
		{
			json_array_append_boolean(nodeArrays[arName].dataArray, val);
			nodeArrays[arName].size++;
		}
	}
}

void JSonHandler::InsertNumArray(const char* arName, double val)
{
	if (!nodeArrays.empty())
	{
		if (nodeArrays.find(arName) != nodeArrays.end()) //Array exists
		{
			json_array_append_number(nodeArrays[arName].dataArray, val);
			nodeArrays[arName].size++;
		}
	}
}

void JSonHandler::InsertStringArray(const char* arName, const char* val)
{
	if (!nodeArrays.empty())
	{
		if (nodeArrays.find(arName) != nodeArrays.end()) //Array exists
		{
			json_array_append_string(nodeArrays[arName].dataArray, val);
			nodeArrays[arName].size++;
		}
	}
}

JSonHandler JSonHandler::InsertNodeArray(const char* arName)
{
	if (!nodeArrays.empty())
	{
		if (nodeArrays.find(arName) != nodeArrays.end()) //Array exists
		{
			json_array_append_value(nodeArrays[arName].dataArray, json_value_init_object());
			nodeArrays[arName].size++;
			return JSonHandler(json_array_get_object(nodeArrays[arName].dataArray, nodeArrays[arName].size - 1));
		}
	}
}

///////

void JSonHandler::LoadArray(const char* name)
{
	if (json_object_has_value_of_type(node, name, JSONArray))
	{
		JSonArrayData data;
		data.dataArray = json_object_get_array(node, name);
		data.size = json_array_get_count(data.dataArray);
		nodeArrays.emplace(name, data);
		//LOG("Array %s loaded",name);
	}
	else LOG("Array not found");
}

uint JSonHandler::Serialize(char** buffer)
{
	size_t size = json_serialization_size_pretty(root);
	*buffer = new char[size];
	json_serialize_to_buffer_pretty(root, *buffer, size);
	return size;
}


void JSonHandler::CreateArray(const char* name)
{
	json_object_set_value(node, name, json_value_init_array());
	JSonArrayData newArray;
	newArray.dataArray = json_object_get_array(node, name);
	newArray.size = json_array_get_count(newArray.dataArray);
	nodeArrays.emplace(name, newArray);	
}