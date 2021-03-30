/* Program name: Object.cpp
* Author: Spencer Smith
* Date last updated: 5/12/2018
* Purpose: .cpp file for the Object class and subclasses Key and Tool
*/

#ifndef __OBJECT_CPP_INCLUDED__
#define __OBJECT_CPP_INCLUDED__

#include "Object.h"
#include <iostream>

Object::Object(objectType type, string name, string description) {
	this->type = type;
	this->name = name;
	this->description = description;
}

string Object::getDescription() {
	return this->description;
}

objectType Object::getType()
{
	return this->type;
}

string Object::getName() {
	return this->name;
}

void Object::use() {
	cout << "You used the " << this->name;
}

bool Object::checkUsable(string checkString) {
	return false;
}

Key::Key(string name, string description) : Object(key, name, description) {
}

bool Key::checkUsable(string checkString) {
	if (useCondition == checkString)
		return true;
	else
		return false;
}

Tool::Tool(string name, string description) : Object(tool, name, description) {
}

#endif