/* Program name: AnalyzeString.h
* Author: Spencer Smith
* Date last updated: 5/12/2018
* Purpose: Stores external commands to be referenced in FinalProject.cpp
*/

#ifndef __ANALYZESTRING_INCLUDED__
#define __ANALYZESTRING_INCLUDED__

#pragma once
#include "Execution.h"
#include <iostream>
#include <string>
#include <queue>

using namespace std;
enum command{goTo, lookAt, pickUp, use, launch, help};
static const char space = ' ';
static const string emptyInput = "Any idea is better than no idea. Concentrate, there must be a way out.";

//Removes excess spaces from front and back of a string
void shrinkString(string& givenString) {
	
	char lastChar = givenString[givenString.size() - 1];

	//Loops until no spaces are found at the front or end
	while (givenString[0] == space || lastChar == space) {
		//Removes front space if it's there
		if (givenString[0] == space) {
			givenString = givenString.substr(1, givenString.size());
		}

		//Guards string error
		if (givenString.size() == 0)
			throw out_of_range(emptyInput);
		else if(givenString.size() == 1 && lastChar == space)
			throw out_of_range(emptyInput);

		//Removes back space if it's there
		if (lastChar == space) {
			givenString = givenString.substr(0, givenString.size() - 1);
			lastChar = givenString[givenString.size() - 1];
		}
	}
}

//Returns a queue of substrings to be used as commands
queue<string> analyzeString(string currInput) {
	queue<string> outCommands;

	//Guards against an empty string
	if (currInput.size() == 0)
		throw out_of_range(emptyInput);

	shrinkString(currInput);

	//Adds all substrings of currInput to the queue, using spaces as seperation points
	int currIndex = 0;
	while (currIndex != currInput.size() - 1){

		if (currInput[currIndex] == space) {
			outCommands.push(currInput.substr(0, currIndex));
			currInput = currInput.substr(currIndex, currInput.size());
			shrinkString(currInput);
			currIndex = 0;
		}
		else
			currIndex++;
	}
	outCommands.push(currInput);

	//Returns queu of all substrings
	return outCommands;
}

//Returns a command from the queue of strings to find which function to use
command evalCommand(queue<string>& fullCommands) {

	string evalString = fullCommands.front();

	//Find strings that could match the goTo command
	if (evalString == "go" || evalString == "Go" || evalString == "move" || evalString == "Move") {
		fullCommands.pop();

		//Throws out "to" string if it exists, not needed.
		if (fullCommands.size() >= 1) {
			evalString = fullCommands.front();

			if (evalString == "to") {
				fullCommands.pop();

				if (fullCommands.size() >= 1) {
					evalString = fullCommands.front();

					if (evalString == "the")
						fullCommands.pop();
				}
			}
		}

		return goTo;
	}
	//Find strings that could match the lookAt command
	else if (evalString == "look" || evalString == "Look" || evalString == "examine" || evalString == "Examine") {
		fullCommands.pop();

		//Throws out "at" string if it exists, not needed.
		if (fullCommands.size() >= 1) {
			evalString = fullCommands.front();

			if (evalString == "at")
				fullCommands.pop();
		}
		return lookAt;
	}
	//Find strings that could match the pickUp command
	else if (evalString == "Pick" || evalString == "pick" || evalString == "Get" || evalString == "get" || evalString == "Grab" || evalString == "grab") {
		fullCommands.pop();

		//Throws out "up" and/or "the" string if it exists, not needed.
		if (fullCommands.size() >= 1) {
			evalString = fullCommands.front();

			if (evalString == "up" || evalString == "the") {
				fullCommands.pop();

				if (fullCommands.size() >= 1) {
					evalString = fullCommands.front();

					if (evalString == "the")
						fullCommands.pop();
				}
			}
		}
		return pickUp;
	}
	//Find strings that could match the use command
	else if (evalString == "Use" || evalString == "use") {
		fullCommands.pop();

		if (evalString == "the")
			fullCommands.pop();

		return use;
	}
	//Returns the launch command
	else if (evalString == "Launch" || evalString == "launch") {
		return launch;
	}
	else if (evalString == "Help" || evalString == "help"){
		return help;
	}
	else
		throw out_of_range("Command not understood");

}




#endif