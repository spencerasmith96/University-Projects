/* Program name: Execution.h
* Author: Spencer Smith
* Date last updated: 5/12/2018
* Purpose: Stores external commands to be referenced in FinalProject.cpp
*/

#ifndef __EXECUTION_INCLUDED__
#define __EXECUTION_INCLUDED__
#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include "Room.h"
#include "MapRooms.h"
#include "Object.h"



void executeGoTo(string target, MapRooms& stationMap) {
	

	if(target == "nullerror")
		throw out_of_range("Go where?");

	if (target == "left" || target == "west")
		stationMap.goWest();

	else if (target == "right" || target == "east")
		stationMap.goEast();

	else if (target == "up" || target == "north")
		stationMap.goNorth();

	else if (target == "down" || target == "south")
		stationMap.goSouth();
}

//Returns the description of the desired target
void executeLookAt(string target, MapRooms& stationMap, vector<Object*> inventory) {
	Room currentRoom = stationMap.getCurrentRoom();
	if (target == "nullerror")
		throw out_of_range("Examine what?");

	//Looks to see if target was the inventory
	else if (target == "inventory") {
		const int numberOfLoop = inventory.size() - 1;
		for (int i = 0; i < numberOfLoop; i++) {
			cout << inventory[i]->getName() << endl;
		}
		if (numberOfLoop >= 0) {
			cout << inventory[numberOfLoop]->getName() << endl;
		}
		return;
	}

	//Looks to see if target was the current room
	else if (target == "room" || target == "around") {
		cout << currentRoom;
		cout << stationMap.getAdjacentNames();
		return;
	}

	else {
		//Looks to see if target was in the inventory
		int numberOfLoop = inventory.size();
		for (int i = 0; i < numberOfLoop; i++) {
			string examinationString = inventory[i]->getName();
			if (target == examinationString) {
				cout << inventory[i]->getDescription() << endl;
				return;
			}
			examinationString[0] = tolower(examinationString[0]);
			if (target == examinationString) {
				cout << inventory[i]->getDescription() << endl;
				return;
			}
		}

		//Looks to see  if target was in the current room
		vector<Object*> roomObjects = currentRoom.getObjects();		
		numberOfLoop = roomObjects.size();
		for (int i = 0; i < numberOfLoop; i++) {
			string examinationString = roomObjects[i]->getName();
			if (target == examinationString) {
				cout << roomObjects[i]->getDescription() << endl;
				return;
			}
			examinationString[0] = tolower(examinationString[0]);
			if (target == examinationString) {
				cout << roomObjects[i]->getDescription() << endl;
				return;
			}
		}
	}
}

void executePickUp(string target, MapRooms& stationMap, vector<Object*>& inventory) {
	if (target == "nullerror")
		throw out_of_range("Pick up what?");
	
	else {
		int index = stationMap.itemInRoom(target);
		if (index != -1) {
			Object& newItem = stationMap.getItem(index);
			if (newItem.getType() != furniture) {
				inventory.push_back(&newItem);
				stationMap.removeItem(index);
				cout << "You added " << newItem.getName() << " to your inventory." << endl;
			}
			else {
				throw out_of_range("You can't pick that up!");
			}
		}
		else {
			cout << "You couldn't find " << target << " in this room." << endl;
		}
	}
}

void executeUse(queue<string> stringCommands, MapRooms& stationMap, vector<Object*>& inventory) {
	string item1, item2;

	//Records first item
	item1 = stringCommands.front();
	if (item1 == "nullerror")
		throw out_of_range("Use what?");

	//If no other item is provided, tries to find item1 and see if item1 can be used with "NULL"
	stringCommands.pop();
	if (stringCommands.size() == 0) {
		string target = item1;

		//Looks to see if target was in the inventory
		int numberOfLoop = inventory.size();
		for (int i = 0; i < numberOfLoop; i++) {
			string examinationString = inventory[i]->getName();
			if (target == examinationString) {
				inventory[i]->use();
				return;
			}
		}
		//Looks to see  if target was in the current room
		Room currentRoom = stationMap.getCurrentRoom();
		vector<Object*> roomObjects = currentRoom.getObjects();
		numberOfLoop = roomObjects.size();
		for (int i = 0; i < numberOfLoop; i++) {
			string examinationString = roomObjects[i]->getName();
			if (target == examinationString) {
				if (roomObjects[i]->checkUsable("NULL")) {
					cout << "You used the " << roomObjects[i]->getName() << endl;
					roomObjects[i]->use();
					return;
				}
			}
		}
		cout << "Nothing happened..." << endl;
		return;
	}
	stringCommands.pop();
	if (stringCommands.size() == 0) {
		string errorMessage = "Use " + item1 + " on what?";
		throw out_of_range(errorMessage);
	}

	//If there was a second item with a fill word like 'on' tries to use the two (first item must always be in inventory)
	item2 = stringCommands.front();
	//Looks for item1 in inventory, breaks if not found
	string target = item1;
	int numberOfLoop = inventory.size();
	bool found = false;
	int item1Index;
	for (int i = 0; i < numberOfLoop; i++) {
		string examinationString = inventory[i]->getName();
		if (target == examinationString) {
			found = true;
			item1Index = i;
		}
	}
	if (found == false) {
		cout << "You don't have " << item1 << " in your inventory!" << endl;
		return;
	}

	//Looks to see if item2 was in the inventory
	target = item2;
	numberOfLoop = inventory.size();
	for (int i = 0; i < numberOfLoop; i++) {
		string examinationString = inventory[i]->getName();
		if (target == examinationString) {
			if (inventory[i]->checkUsable(item1)) {
				inventory[i]->use();
				if (inventory[item1Index]->getType() == key) {
					inventory.erase(inventory.begin() + item1Index);
				}
				return;
			}
			else {
				cout << "Check spelling, objects are case sensitive." << endl;
			}
		}
	}
	//Looks to see  if target was in the current room
	Room currentRoom = stationMap.getCurrentRoom();
	vector<Object*> roomObjects = currentRoom.getObjects();
	numberOfLoop = roomObjects.size();
	for (int i = 0; i < numberOfLoop; i++) {
		string examinationString = roomObjects[i]->getName();
		if (target == examinationString) {
			//Item1 is a valid object to use on item2
			if (roomObjects[i]->checkUsable(item1)) {
				cout << "You used the " << item1 << " on " << item2 << endl;
				roomObjects[i]->use();
				//Removes item1 from inventory if it's a key type
				if (inventory[item1Index]->getType() == key) {
					inventory.erase(inventory.begin() + item1Index);
				}
				return;
			}
			else {
				cout << "Nothing happened..." << endl;
			}
		}
	}
	return;
}

#endif