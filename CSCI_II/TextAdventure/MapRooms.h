#ifndef __MAPROOMS_H_INCLUDED__
#define __MAPROOMS_H_INCLUDED__

/* Program name: MapRooms.h
* Author: Spencer Smith
* Date last updated: 5/12/2018
* Purpose: HeaderFile file for the MapRooms class
*/

#pragma once

#include "Room.h"
#include <string>


class MapRooms {
	//Coordinates as if rooms were mapped on a 2d plane. 0,0 being the bottom left most room.
	int currentX;
	int currentY;
	const static int max_x = 10, max_y = 10;

	Room *AllRooms[max_x][max_y];
	Room *currentRoom;

public:

	MapRooms(int, int);

	void addRoom(Room&, int, int);

	void updateCurrentRoom();

	int getX();

	int getY();

	bool goNorth();

	bool goEast();

	bool goSouth();

	bool goWest();

	Room getCurrentRoom();

	string getAdjacentNames();

	void removeItem(int);
	
	Object& getItem(int);

	int itemInRoom(string);

	void togglePower();
};


#endif