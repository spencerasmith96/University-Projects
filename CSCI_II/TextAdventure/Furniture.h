/* Program name: Furniture.h
* Author: Spencer Smith
* Date last updated: 5/12/2018
* Purpose: Header file for the Furniture class (sub class of Object) and sub classes Lock, PowerLever, Terminal, Turret, and Button
*/

#ifndef __FURNITURE_H_INCLUDED__
#define __FURNITURE_H_INCLUDED__

#pragma once

#include "Room.h"
#include "MapRooms.h"
#include <string>

//Static object in room, can't be picked up
class Furniture : public Object{
	string name;
	string description;
public:
	Furniture(string, string);
	void use();
};

class Lock : public Furniture {
	string usedDescription;
	string useCondition;
protected:
	Room* connectedRoom;
public:
	Lock(Room&, string, string, string);
	bool checkUsable(string);
	void use();
};

class PowerLever : public Furniture {
	string usedDescription;
	string useCondition;
	MapRooms* connectedMap;
public:
	PowerLever(MapRooms&, string, string);
	bool checkUsable(string);
	void use();
};

class Terminal : public Lock {
public:
	Terminal(Room&, string, string);
	void use();
};

class Turret : public Lock {
public:
	Turret(Room&, string, string, string);
	void use();
};

class Button : public Lock {
public:
	Button(Room&, string, string);
	void use();
};
#endif