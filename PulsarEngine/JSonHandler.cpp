#include "Globals.h"
#include "JSonHandler.h"
#include "parson/parson.h"


JSonHandler::JSonHandler()
{
	root = json_value_init_object();
	node = json_value_get_object(root);
}

JSonHandler::~JSonHandler()
{
	if (root) json_value_free(root);	
}

//Getters
bool JSonHandler::GetBool()
{

}

int JSonHandler::GetInt()
{

}

float JSonHandler::GetFloat()
{

}

std::string JSonHandler::GetString()
{

}

//Setters
void JSonHandler::SaveBool(bool val)
{

}

void JSonHandler::SaveInt(int val)
{

}

void JSonHandler::SaveFloat(float val)
{

}

void JSonHandler::SaveString(std::string val)
{

}