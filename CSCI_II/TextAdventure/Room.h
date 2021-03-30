/* Program name: Room.h
* Author: Spencer Smith
* Date last updated: 5/12/2018
* Purpose: Header file for the Room class
*/

#ifndef __ROOM_H_INCLUDED__
#define __ROOM_H_INCLUDED__

#pragma once

#include <string>
#include <iostream>
#include <vector>
#include "Object.h"


using namespace std;

class Room {
	string name;
	string description;
	string lockedDescription;
	bool discovered;
	bool locked;
	bool power;
	bool turret;
	vector<Object*> roomObjects;
	vector<string> objectPlacements;

public:
	friend ostream& operator<<(ostream& os, Room& room) {
		os << room.getDescription() << endl;
		return os;
	};

	Room(string, string, string, bool, bool);

	string getName();

	string getLockDescription();

	bool isDiscovered();

	bool open();

	void addObject(Object*, string);

	string getDescription();

	const vector<Object*>& getObjects();

	void removeObject(int);

	Object& getObject(int);

	int itemInRoom(string);

	void unlock();

	void lock();

	void togglePower();

	bool getPower();

	void destroyTurret();
};

#endif