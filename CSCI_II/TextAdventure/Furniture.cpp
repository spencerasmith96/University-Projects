/* Program name: Furniture.cpp
* Author: Spencer Smith
* Date last updated: 5/12/2018
* Purpose: .cpp file for the Furniture class (sub class of Object) and sub classes Lock, PowerLever, Terminal, Turret, and Button
*/

#include "Furniture.h"


Furniture::Furniture(string name, string description) : Object(furniture, name, description) {
}

void Furniture::use() {
	cout << "You used the " << this->name;
}

Lock::Lock(Room& targetRoom, string useCondition, string name, string description) : Furniture(name, description) {
	this->connectedRoom = &targetRoom;
	this->useCondition = useCondition;
}

void Lock::use() {
	Room& helperRoom = *connectedRoom;
	helperRoom.unlock();
}

bool Lock::checkUsable(string checkString) {
	if (useCondition == checkString)
		return true;
	else
		return false;
}

PowerLever::PowerLever(MapRooms& targetMap, string name, string description) : Furniture(name, description){
	this->connectedMap = &targetMap;
	this->useCondition = "NULL";
}

bool PowerLever::checkUsable(string checkString) {
	if (useCondition == checkString)
		return true;
	else
		return false;
}
void PowerLever::use() {
	this->connectedMap->togglePower();
}

Terminal::Terminal(Room& targetRoom, string name, string description) : Lock(targetRoom, "NULL", name, description) {

}

void Terminal::use() {
	Room& helperRoom = *connectedRoom;
	if (helperRoom.getPower()) {
		string password;
		cout << "Enter password: ";
		cout << "Hint: First terrestrial to orbit Earth." << endl;
		getline(cin, password);
		if (password == "Laika") {
			helperRoom.unlock();
			cout << "Correct!" << endl;
		}
		else
			cout << "Incorrect." << endl;
	}
	else
		cout << "There's no power." << endl;
}

Turret::Turret(Room& targetRoom, string disableString, string name, string description) : Lock(targetRoom, disableString, name, description) {

}

void Turret::use() {
	Room& helperRoom = *connectedRoom;
	helperRoom.destroyTurret();
}

Button::Button(Room& targetRoom, string name, string description) : Lock(targetRoom, "NULL", name, description) {

}

void Button::use() {
	Room& helperRoom = *connectedRoom;
	if (helperRoom.getPower()) {
		cout << "The door slides open." << endl;
		helperRoom.unlock();
	}
	else
		cout << "Nothing happened. Maybe it needs power?" << endl;
}