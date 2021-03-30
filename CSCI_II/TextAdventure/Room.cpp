/* Program name: Room.cpp
* Author: Spencer Smith
* Date last updated: 5/12/2018
* Purpose: .cpp file for the Room class
*/

#ifndef __ROOM_CPP_INCLUDED__
#define __ROOM_CPP_INCLUDED__

#pragma once

#include "Room.h"
#include "Object.h"
#include <vector>
#include <string>


Room::Room(string name, string description, string lockDescription, bool locked, bool turret) {
	this->locked = locked;
	this->description = description;
	this->lockedDescription = lockDescription;
	this->name = name;
	this->discovered = false;
	this->power = false;
	this->turret = turret;
}

string Room::getName() {
	if (discovered == true)
		return this->name;
	else
		return "a door";
}

string Room::getLockDescription() {
	return this->lockedDescription;
}

bool Room::open() {
	if (locked == false) {
		this->discovered = true;
		return true;
	}
	else
		return false;
}

bool Room::isDiscovered() {
	return discovered;
}

void Room::addObject(Object *newObject, string objectPlacement) {
	this->roomObjects.push_back(newObject);
	this->objectPlacements.push_back(objectPlacement);
}
/*
void Room::addFurniture(Furniture& newFurniture) {
	this->roomObjects.push_back(newFurniture);
}
*/
string Room::getDescription() {
	string returnString;

	returnString = this->description;

	int objectLoop = this->roomObjects.size();

	for (int i = 0; i < objectLoop; i++) {
		returnString = returnString + "\nThere is a " + roomObjects[i]->getName() + ' ' + objectPlacements[i];
	}

	return returnString;
}

const vector<Object*>& Room::getObjects()
{
	return this->roomObjects;
}


void Room::removeObject(int index) {
	this->roomObjects.erase(roomObjects.begin() + index);
	this->objectPlacements.erase(objectPlacements.begin() + index);
}

Object& Room::getObject(int index) {
	Object& returnItem = *roomObjects[index];
	return returnItem;
}

int Room::itemInRoom(string objectName)
{
	int objectLoop = this->roomObjects.size();

	for (int i = 0; i < objectLoop; i++) {
		if (objectName == roomObjects[i]->getName()) {
			return i;
		}
	}
	return -1;
}

void Room::unlock() {
	this->locked = false;
}

void::Room::lock() {
	this->locked = true;
}

void Room::togglePower() {
	if (this->getPower()){
		power = false;
		if (this->turret) {
			this->locked = false;
		}
	}
	else {
		power = true;
		if (this->turret) {
			this->locked = true;
		}
	}
	
}

bool Room::getPower() {
	return this->power;
}

void Room::destroyTurret() {
	this->turret = false;
}

#endif