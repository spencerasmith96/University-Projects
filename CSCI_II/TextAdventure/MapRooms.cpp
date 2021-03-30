/* Program name: MapRooms.cpp
* Author: Spencer Smith
* Date last updated: 5/12/2018
* Purpose: .cpp file for the MapRooms class
*/

#ifndef __MAPROOMS_CPP_INCLUDED__
#define __MAPROOMS_CPP_INCLUDED__

#include "MapRooms.h"
#include "Room.h"
#include <iostream>

MapRooms::MapRooms(int startingX, int startingY) {
	this->currentX = startingX;
	this->currentY = startingY;
	for (int i = 0; i < max_x; i++) {
		for (int j = 0; j < max_x; j++) {
			AllRooms[i][j] = 0;
		}
	}
}

void MapRooms::addRoom(Room& newRoom, int x, int y) {
	this->AllRooms[x][y] = &newRoom;
	updateCurrentRoom();
}

void MapRooms::updateCurrentRoom() {
	this->currentRoom = AllRooms[currentX][currentY];
}

int MapRooms::getX() {
	return this->currentX;
}

int MapRooms::getY() {
	return this->currentY;
}

bool MapRooms::goNorth()
{
	if (this->currentX == max_y) {
		throw out_of_range("There's no room there");
		return false;
	}
	else {
		Room* nextPointer;
		nextPointer = AllRooms[currentX][currentY + 1];
		if (nextPointer != nullptr) {
			Room &nextRoom = *nextPointer;
			bool unlocked = nextRoom.open();
			if (unlocked) {
				this->currentY++;
				cout << "Moving to " << nextRoom.getName() << endl;
				updateCurrentRoom();
				//this->currentRoom = &nextRoom;
				return true;
			}
			else
				throw out_of_range(nextRoom.getLockDescription());
			return false;
		}
		else {
			throw out_of_range("There's no room there");
			return false;
		}
	}
}

bool MapRooms::goWest()
{
	if (this->currentX == 0) {
		throw out_of_range("There's no room there");
		return false;
	}
	else {
		Room* nextPointer;
		nextPointer = AllRooms[currentX - 1][currentY];
		if (nextPointer != nullptr) {
			Room &nextRoom = *nextPointer;
			bool unlocked = nextRoom.open();
			if (unlocked) {
				this->currentX--;
				cout << "Moving to " << nextRoom.getName() << endl;
				updateCurrentRoom();
				//this->currentRoom = &nextRoom;
				return true;
			}
			else
				throw out_of_range(nextRoom.getLockDescription());
			return false;
		}
		else {
			throw out_of_range("There's no room there");
			return false;
		}
	}
}

bool MapRooms::goSouth()
{
	if (this->currentY == 0) {
		throw out_of_range("There's no room there");
		return false;
	}
	else {
		Room* nextPointer;
		nextPointer = AllRooms[currentX][currentY - 1];
		if (nextPointer != nullptr) {
			Room nextRoom = *nextPointer;
			bool unlocked = nextRoom.open();
			if (unlocked) {
				this->currentY--;
				cout << "Moving to " << nextRoom.getName() << endl;
				updateCurrentRoom();
				//this->currentRoom = &nextRoom;
				return true;
			}
			else
				throw out_of_range(nextRoom.getLockDescription());
			return false;
		}
		else {
			throw out_of_range("There's no room there");
			return false;
		}
	}
}

bool MapRooms::goEast()
{
	if (this->currentX == max_x) {
		throw out_of_range("There's no room there");
		return false;
	}
	else {
		Room* nextPointer;
		nextPointer = AllRooms[currentX + 1][currentY];
		if (nextPointer != nullptr) {
			Room &nextRoom = *nextPointer;
			bool unlocked = nextRoom.open();
			if (unlocked) {
				this->currentX++;
				cout << "Moving to " << nextRoom.getName() << endl;
				updateCurrentRoom();
				//this->currentRoom = &nextRoom;
				return true;
			}
			else
				throw out_of_range(nextRoom.getLockDescription());
			return false;
		}
		else {
			throw out_of_range("There's no room there");
			return false;
		}
	}
}

Room MapRooms::getCurrentRoom()
{
	if (currentRoom == nullptr) {
		throw("AHHHHHHHH, YOU'RE IN SPACE");
	}
	Room& helperRoom = *currentRoom;
	//string returnRoom = helperRoom.getName();
	//string returnRoom = "placeholder";
	return helperRoom;
	
}

string MapRooms::getAdjacentNames() {
	string returnString;

	if (this->currentX != 0) {
		Room* nextPointer;
		nextPointer = AllRooms[currentX - 1][currentY];
		if (nextPointer != nullptr) {
			Room &nextRoom = *nextPointer;
			returnString = returnString + "There is " + nextRoom.getName() + " to the west\n";
		}
	}
	if (this->currentX != this->max_x) {
		Room* nextPointer;
		nextPointer = AllRooms[currentX + 1][currentY];
		if (nextPointer != nullptr) {
			Room &nextRoom = *nextPointer;
			returnString = returnString + "There is " + nextRoom.getName() + " to the east\n";
		}
	}
	if (this->currentY != 0) {
		Room* nextPointer;
		nextPointer = AllRooms[currentX][currentY - 1];
		if (nextPointer != nullptr) {
			Room &nextRoom = *nextPointer;
			returnString = returnString + "There is " + nextRoom.getName() + " to the south\n";
		}
	}
	if (this->currentY != this->max_y) {
		Room* nextPointer;
		nextPointer = AllRooms[currentX][currentY + 1];
		if (nextPointer != nullptr) {
			Room &nextRoom = *nextPointer;
			returnString = returnString + "There is " + nextRoom.getName() + " to the  north\n";
		}
	}

	return returnString;
}

void MapRooms::removeItem(int index) {
	Room* helpPointer = AllRooms[currentX][currentY];
	Room &helpRoom = *helpPointer;
	helpRoom.removeObject(index);
	updateCurrentRoom();
}

Object& MapRooms::getItem(int index) {
	Room* helpPointer = AllRooms[currentX][currentY];
	Room &helpRoom = *helpPointer;
	Object& returnObject = helpRoom.getObject(index);
	return returnObject;
}

int MapRooms::itemInRoom(string itemName) {
	Room* helpPointer = AllRooms[currentX][currentY];
	Room &helpRoom = *helpPointer;
	int returnIndex = helpRoom.itemInRoom(itemName);
	return returnIndex;
}

void MapRooms::togglePower() {
	Room* helpPointer;
	if (this->getCurrentRoom().getPower())
		cout << "Everything falls back to an eerie silence." << endl;
	else
		cout << "The station hums to life as the main lights come on." << endl;
	for (int i = 0; i < max_x; i++) {
		for (int j = 0; j < max_x; j++) {
			helpPointer = AllRooms[i][j];
			if (helpPointer != nullptr) {
				Room &helpRoom = *helpPointer;
				helpRoom.togglePower();
			}
		}
	}
}

#endif