/* Program name: FinalProject.cpp
* Author: Spencer Smith
* Date last updated: 5/12/2018
* Purpose: Create objects needed to run using given classes and reference appropriate external functions
*/

#include "AnalyzeString.h"
#include "Room.h"
#include "MapRooms.h"
#include "Object.h"
#include "Furniture.h"

int mainLoop(MapRooms& stationMap, vector<Object*>& inventory) {
	string fullInput;
	queue<string> stringCommands;
	cout << endl;
	getline(cin, fullInput);
	command genericCommand;

	try {
		//Breaks the user input into seperate strings, throws exception if empty
		stringCommands = analyzeString(fullInput);

		//Scans first few strings for a command, throws exception if none are found
		genericCommand = evalCommand(stringCommands);

		//If there is no target, but the command is correct passes string "nullerror"
		if (stringCommands.size() == 0)
			stringCommands.push("nullerror");

		//Executes the proper command
		switch (genericCommand) {
		case goTo:
			executeGoTo(stringCommands.front(), stationMap);
			break;
		case lookAt:
			executeLookAt(stringCommands.front(), stationMap, inventory);
			break;
		case pickUp:
			executePickUp(stringCommands.front(), stationMap, inventory);
			break;
		case use:
			executeUse(stringCommands, stationMap, inventory);
			break;
		case launch:
			if (stationMap.getCurrentRoom().getName() == "the escape pod") {
				return 1;
			}
			else {
				cout << "You're not in the escape pod." << endl;
			}
		case help:
			cout << "Say 'go [direction]' to change rooms" << endl;
			cout << "Say 'look at [object]' to get a description" << endl;
			cout << "or 'look around' to see what's around you" << endl;
			cout << "Say 'get [object]' to get an object from the room" << endl;
			cout << "Say 'use [object]' to use an object" << endl;
			cout << "or 'use [object] with [object]' to use 2 objects together." << endl;
			cout << "Say 'help' to print this list again" << endl;
		}
		
	}
	catch (const exception& e) {
		cout << e.what() << endl;
	}


	/*
	int commandNumber = stringCommands.size();
	for (int i = 0; i < commandNumber; i++) {
	cout << i;
	cout << stringCommands.front() << endl;
	stringCommands.pop();
	}
	*/
	return 0;
}

Room cell() {
	return Room("the cell", "It feels damp and rather dull.", "It's locked", false, false);
};
Room security() {
	return Room("the security room", "There are monitors galore", "It's locked", true, false);
};
Room captain() {
	return Room("the captain's quarters", "Just being in this room makes you feel empowered.", "It's locked", true, false);
};
Room mechanic() {
	return Room("the mechanical closet", "The room is litered with tools and tech. Most of it is broken.", "It's locked, the card reader looks to be running on battery.", true, false);
};
Room living() {
	return Room("the living quarters", "Empty beds and odd junk is scattered throughout.", "It's sealed shut.", true, false);
};
Room mess() {
	return Room("the mess hall", "Despite being called the mess hall, it's frowned upon to be messy in.", "It's locked.", false, false);
};
Room lobby() {
	return Room("the main lobby", "There's a large desk in the middle of the room.\nPapers are lying everywhere.", "It's locked.", false, false);
};
Room navigation() {
	return Room("the navigation room", "There's a massive disk in the center of the room with tons of buttons.\nIf memory serves, it displays a hologram for navigation.", "The turret turned on, it would be suicide to enter now.", false, true);
};
Room powerRoom() {
	return Room("the power room", "A large generator is along the wall, looks to be fully operational.", "It's sealed shut", true, false);
};
Room airLock() {
	return Room("the air lock", "A rather simple, but practical halway.", "You are not stronger than steel. Use the button?", true, false);
};
Room escapePod() {
	return Room("the escape pod", "There's a screen that says: Coordinates recieved. Say 'Launch' to launch.", "The new standard for escape pods is to have an updated star map to not get stranded.", true, false);
};

int main() {
	//Creates station map, specifies coordinates for starting room.
	MapRooms stationMap = MapRooms(3,0);
	
	//Creates the rooms
	Room initRoom00 = cell();
	Room initRoom01 = security();
	Room initRoom02 = captain();
	Room initRoom03 = mechanic();
	Room initRoom04 = living();
	Room initRoom05 = mess();
	Room initRoom06 = lobby();
	Room initRoom07 = navigation();
	Room initRoom08 = powerRoom();
	Room initRoom09 = airLock();
	Room initRoom10 = escapePod();

	//Creates the items
	Key rustyKey("RustyKey", "The weather sure has done a number to this key... Wait you're indoors. How did it rust?");
	Lock oldLock(initRoom01, "RustyKey", "OldLock", "An archaic, but effective method to secure something.");
	Key starChart("StarMap", "A map to the stars. You hope this is up to date...");
	Key keyCard("KeyCard", "A card to be used with a card reader.");
	Lock cardReader(initRoom03, "KeyCard", "CardReader", "The light is blinking, so it must have power.");
	Lock sealedDoor1(initRoom04, "Plasmatorch", "SealedDoor", "The door has been sealed shut, some poor welding job.");
	Lock sealedDoor2(initRoom08, "Plasmatorch", "SealedDoor", "The door has been sealed shut, some poor welding job.");
	Tool plasmatorch("Plasmatorch", "It get's very hot very fast.\nDon't look directly at it when using.");
	Turret navTurret(initRoom07, "Plasmatorch", "Turret", "Basically a glorified booby trap.");
	Terminal captainTerminal(initRoom02, "Terminal", "A computer terminal");
	PowerLever mainLever(stationMap, "Lever", "The word 'POWER' is marker at the top.\nWhat could it mean?");
	Button powerDoor(initRoom09, "Button", "It's a button, presumably, for the door.");
	Lock starInput(initRoom10, "StarMap", "MapSlot", "A recepticale for a star map");
	Tool historyBook("Book", "A History of Space Travel\n1948-Albert 1 became the first animal to enter space\n1957-Laika became the first animal to orbit earth\n1962- John Glenn first American to orbit Earth\n1969- First moon landing\n2020- Second moon landing\n2050- First extraterrestr-\nThe rest is unreadable.");

	//Adds the items to the rooms
	initRoom00.addObject(&rustyKey, "on the floor");
	initRoom00.addObject(&oldLock, "on the other side of the bars");
	initRoom01.addObject(&cardReader, "next to the east door.");
	initRoom01.addObject(&captainTerminal, "next to the west door");
	initRoom02.addObject(&starChart, "on the captain's desk.");
	initRoom03.addObject(&plasmatorch, "hanging on the wall.");
	initRoom04.addObject(&historyBook, "Laying on a nightstand");
	initRoom05.addObject(&sealedDoor1, "along the west wall");
	initRoom06.addObject(&keyCard, "with the spare keys.");
	initRoom07.addObject(&navTurret, "in the north east corner.");
	initRoom07.addObject(&cardReader, "next to the souther door.");
	initRoom07.addObject(&sealedDoor2, "along the east wall");
	initRoom08.addObject(&mainLever, "in the middle of the generator");
	initRoom06.addObject(&powerDoor, "next to the northern door");
	initRoom09.addObject(&starInput, "next to the northern door");
	

	//Opens initial room
	initRoom00.open();

	//Adds the rooms to the station map
	stationMap.addRoom(initRoom00, 3, 0);
	stationMap.addRoom(initRoom01, 3, 1);
	stationMap.addRoom(initRoom02, 2, 1);
	stationMap.addRoom(initRoom03, 4, 1);
	stationMap.addRoom(initRoom04, 1, 2);
	stationMap.addRoom(initRoom05, 2, 2);
	stationMap.addRoom(initRoom06, 3, 2);
	stationMap.addRoom(initRoom07, 4, 2);
	stationMap.addRoom(initRoom08, 5, 2);
	stationMap.addRoom(initRoom09, 3, 3);
	stationMap.addRoom(initRoom10, 3, 4);

	//Initialized the inventory
	vector<Object*> inventory;

	//Starting text
	cout << "The year is 2304 and you're a space mercinary." << endl;
	cout << "Your latest assignment was to infiltrate a pirate station and take care of the problem." << endl;
	cout << "Unfortunately you were captured and put in a cell." << endl;
	cout << "Shortly after there were some explosion and exchanging of fire." << endl;
	cout << "Luckily the commotion knocked the key into your cell." << endl;
	cout << "The light flickerd as the station switched to emergency power." << endl;
	cout << "Some time after you heard several escape pods taking off." << endl;
	cout << "\nNow is the time to leave." << endl;

	int running = 0;
	while (running == 0){
		Room currentRoom = stationMap.getCurrentRoom();
		currentRoom.open();
		running = mainLoop(stationMap, inventory);
	}
	cout << "The escape pod blasts off and you set out upon your journey." << endl;
	cout << "\nCongratulations, you won! Thank you for playing." << endl;
	cout << "\nPress enter to exit..." << endl;
	cin.get();
	return 0;
}