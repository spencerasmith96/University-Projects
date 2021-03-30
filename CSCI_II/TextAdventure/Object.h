/* Program name: Object.h
* Author: Spencer Smith
* Date last updated: 5/12/2018
* Purpose: Header file for the Object class and subclasses Key and Tool
*/

#ifndef __OBJECT_H_INCLUDED__
#define __OBJECT_H_INCLUDED__
#pragma once

#include <string>
#include <vector>

using namespace std;

enum objectType{generic, key, tool, furniture};

//General purpose object
class Object {
	string name;
	objectType type;
	string description;
	
public:
	Object(objectType, string, string);

	string getName();

	string getDescription();

	objectType getType();

	virtual bool checkUsable(string);

	virtual void use();
};

//Single use key for specific furniture
class Key : public Object {
	string useCondition;

public:
	Key(string, string);
	bool checkUsable(string);
};

//Multi use tool
class Tool : public Object {
public:
	Tool(string, string);
};



#endif 

