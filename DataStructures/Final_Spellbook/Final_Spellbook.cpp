/* Program name: Final_Spellbook
* Author: Spencer Smith
* Date last updated: 12/15/2018
* Purpose: Store and track spell
*/

#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;

struct node
{
	string data;
	struct node *left = NULL;
	struct node *right = NULL;
};

class linkedList
{
	node* origin;
public:
	linkedList(node* origin) {
		this->origin = origin;
	}

	node* getOrigin() {
		return origin;
	}

	linkedList() {
		this->origin = NULL;
	}

	void add(node* newNode) {
		//For our purposes, no duplicates should be allowed
		if (origin == NULL) {
			
			origin = newNode;
		}
		else {
			if (!find(newNode->data)) {
				node* nextNode = origin;
			//	cout << nextNode->data;
				while (nextNode->right != NULL) {
			//		cout << nextNode->right->data << endl;
			//		system("pause");
					nextNode = nextNode->right;
				}
				nextNode->right = newNode;
				
			}
		}
	}

	bool find(string str) {
		node* search = origin;
		while (search != NULL) {
		//	cout << search->data << " = " << str << "?" << endl;
		//	cout << (search->data == str);
			if (search->data == str) {
				return true;
			}
			else if (search->right == NULL) {
				return false;
			}
			else {
				search = search->right;
			}
		}
	}

	void print() {
		node* currentNode = origin;
		while (currentNode != NULL) {
			cout << currentNode->data << endl;
			currentNode = currentNode->right;
		}
	}
};


int printMenu(int spells) {
	cout << "You have " << spells << " spells left." << endl << endl;
	cout << "1) Set max spells" << endl;
	//cout << "2) Create new character" << endl;
	//cout << "3) Create new spell" << endl;
	cout << "2) Cast spell" << endl;
	cout << "3) Replenish spells" << endl;
	cout << "4) Find spell" << endl;
	//cout << "7) Save character data" << endl;
	cout << "-1) Exit" << endl;
	int x;
	cin >> x;

	if (x == 4) {
		cout << "1) By name" << endl;
		cout << "2) By school" << endl;
		cout << "3) By range" << endl;
		//cout << "4) Favorites" << endl;

		cin >> x;
		return (x + 4);
	}

	return x;
}

int main() {
	cout << "Preloading spells. Please wait..." << endl;

	//initilization of all preloads
	node* newNode = new node;
	linkedList a2d;
	linkedList e2i;
	linkedList j2o;
	linkedList p2t;
	linkedList u2z;
	linkedList schools;
	linkedList range;

	map<int, string> nameID;
	map<string, string> nameSchool;
	map<string, string> nameDesc;
	map<string, string> nameComp;
	map<string, string> nameDur;
	map<string, string> nameRange;
	map<string, string> nameTime;
	
	
	//Preloads names
	string fname = "full_name.txt";
	fstream readFile;
	readFile.open(fname);
	//Checks to make sure file opened properly
	if (!readFile.is_open()) {
		cout << "There was an error opening the file: \"" << fname << "\"..." << endl;
		cout << "Make sure to include the filetype." << endl;
	}
	else {
		string name;
		int ID = 0;
		while (!readFile.eof()) {
			newNode = new node;
			getline(readFile, name);

			newNode->data = name;

			//Maps an ID to a specific name
			nameID[ID] = name;

			//Organizes names into 5 linked lists
			if ((name[0] >= 'a' && name[0] <= 'd') || name[0] >= 'A' && name[0] <= 'D') {
				a2d.add(newNode);
			}
			else if ((name[0] >= 'e' && name[0] <= 'i') || name[0] >= 'E' && name[0] <= 'I') {
				e2i.add(newNode);
			}
			else if ((name[0] >= 'j' && name[0] <= 'o') || name[0] >= 'J' && name[0] <= 'O') {
				j2o.add(newNode);
			}
			else if ((name[0] >= 'p' && name[0] <= 't') || name[0] >= 'P' && name[0] <= 'T') {
				p2t.add(newNode);
			}
			else if ((name[0] >= 'u' && name[0] <= 'z') || name[0] >= 'U' && name[0] <= 'Z') {
				u2z.add(newNode);
			}
			else {
				cout << "ERROR WHERE DO I PUT: " << name << endl;
			}
			ID++;
		}
	}
	readFile.close();


	//Preloads Schools
	fname = "full_school.txt";
	readFile.open(fname);
	//Checks to make sure file opened properly
	if (!readFile.is_open()) {
		cout << "There was an error opening the file: \"" << fname << "\"..." << endl;
		cout << "Make sure to include the filetype." << endl;
	}
	else {
		int ID = 0;
		string school;
		while (!readFile.eof()) {
			
			newNode = new node;
			getline(readFile, school);
			
			newNode->data = school;

			//Maps school to the name
			string name = nameID[ID];
			nameSchool[name] = school;
			
			//Adds to the link list of schools
			schools.add(newNode);
			ID++;
		}
	}
	readFile.close();

	//Preloads Descriptions
	fname = "full_description.txt";
	readFile.open(fname);
	//Checks to make sure file opened properly
	if (!readFile.is_open()) {
		cout << "There was an error opening the file: \"" << fname << "\"..." << endl;
		cout << "Make sure to include the filetype." << endl;
	}
	else {
		int ID = 0;
		string desc;
		while (!readFile.eof()) {

			newNode = new node;
			getline(readFile, desc);

			newNode->data = desc;

			//Maps desc to the name
			string name = nameID[ID];
			nameDesc[name] = desc;

			ID++;
		}
	}
	readFile.close();

	//Preloads Components
	fname = "full_components.txt";
	readFile.open(fname);
	//Checks to make sure file opened properly
	if (!readFile.is_open()) {
		cout << "There was an error opening the file: \"" << fname << "\"..." << endl;
		cout << "Make sure to include the filetype." << endl;
	}
	else {
		int ID = 0;
		string str;
		while (!readFile.eof()) {

			newNode = new node;
			getline(readFile, str);

			newNode->data = str;

			//Maps component to the name
			string name = nameID[ID];
			nameComp[name] = str;

			ID++;
		}
	}
	readFile.close();


	//Preloads duration
	fname = "full_duration.txt";
	readFile.open(fname);
	//Checks to make sure file opened properly
	if (!readFile.is_open()) {
		cout << "There was an error opening the file: \"" << fname << "\"..." << endl;
		cout << "Make sure to include the filetype." << endl;
	}
	else {
		int ID = 0;
		string str;
		while (!readFile.eof()) {

			newNode = new node;
			getline(readFile, str);

			newNode->data = str;

			//Maps duration to the name
			string name = nameID[ID];
			nameDur[name] = str;

			ID++;
		}
	}
	readFile.close();

	//Preloads range
	fname = "full_range.txt";
	readFile.open(fname);
	//Checks to make sure file opened properly
	if (!readFile.is_open()) {
		cout << "There was an error opening the file: \"" << fname << "\"..." << endl;
		cout << "Make sure to include the filetype." << endl;
	}
	else {
		int ID = 0;
		string str;
		while (!readFile.eof()) {

			newNode = new node;
			getline(readFile, str);

			newNode->data = str;

			//Maps range to the name
			string name = nameID[ID];
			nameRange[name] = str;

			//Add range to ranged linked list
			range.add(newNode);
			ID++;
		}
	}
	readFile.close();

	//Preloads casttime
	fname = "full_time.txt";
	readFile.open(fname);
	//Checks to make sure file opened properly
	if (!readFile.is_open()) {
		cout << "There was an error opening the file: \"" << fname << "\"..." << endl;
		cout << "Make sure to include the filetype." << endl;
	}
	else {
		int ID = 0;
		string str;
		while (!readFile.eof()) {

			newNode = new node;
			getline(readFile, str);

			newNode->data = str;

			//Maps casttime to the name
			string name = nameID[ID];
			nameTime[name] = str;

			ID++;
		}
	}
	readFile.close();

	cout << "Succesfully preloaded " << nameID.size() << " spells." << endl << endl;

	int spells = 10;
	int maxSpells = 10;

	//Enter main loop
	int selection = 0;
	while (selection != -1)
	{
		selection = printMenu(spells);
		
		//Set Max Spells
		if (selection == 1) {
			cout << "Current max is " << maxSpells << ". New max: ";
			cin >> maxSpells;

			if (maxSpells < spells)
				spells = maxSpells;
		}
		/*
		//New character
		else if (selection == 2) {

		}
		//Add new spell
		else if (selection == 3) {

		}*/
		//Cast spell
		 if (selection == 2) {
			if (spells != 0) {
				cout << "Spell casted." << endl;
				spells--;
			}
			else {
				cout << "No spells left to cast!" << endl;
			}
		}
		//Refresh spell
		else if (selection == 3) {
			spells = maxSpells;
		}
		//Save character data
		//else if (selection == 7) {
		//
		//}
		//Find spell
		//By name
		else if (selection == 5) {
			int x;
			cout << "1) A-D" << endl
				<< "2) E-I" << endl
				<< "3) J-O" << endl
				<< "4) P-T" << endl
				<< "5) U-Z" << endl;
			cin >> x;

			node* currentNode = NULL;

			if (x == 1)
				currentNode = a2d.getOrigin();
			else if (x == 2)
				currentNode = e2i.getOrigin();
			else if (x == 3)
				currentNode = j2o.getOrigin();
			else if (x == 4)
				currentNode = p2t.getOrigin();
			else if (x == 5)
				currentNode = u2z.getOrigin();

			
			while (currentNode != NULL) {
				int finder = 0;
				while (nameID[finder] != currentNode->data)
				{
					finder++;
				}
				cout << finder << ". " << currentNode->data << endl;
				currentNode = currentNode->right;
			}

			int ID;
			cout << "Spell ID to examine further: ";
			cin >> ID;

			cout << endl;
			cout << nameID[ID] << "\tSchool: " << nameSchool[nameID[ID]] << endl
				<< "Components: " << nameComp[nameID[ID]] << endl
				<< "Range: " << nameRange[nameID[ID]] << endl
				<< "Duration: " << nameDur[nameID[ID]] << endl
				<< "Cast time: " << nameTime[nameID[ID]] << endl
				<< nameDesc[nameID[ID]] << endl;
		}
		//By school
		else if (selection == 6) {
			node* currentNode = schools.getOrigin();
			int finder = 1;
			map<int, string> schoolSelect;

			//Displays all schools
			while (currentNode != NULL) {
				cout << finder << ". " << currentNode->data << endl;
				schoolSelect[finder] = currentNode->data;
				currentNode = currentNode->right;
				finder++;
			}

			int selection;
			cin >> selection;

			finder = 0;
			while (finder < nameID.size())
			{
				if (nameSchool[nameID[finder]] == schoolSelect[selection])
				{
					cout << finder << ". " << nameID[finder] << endl;
					
				}
				finder++;
			}
			
			int ID;
			cout << "Spell ID to examine further: ";
			cin >> ID;

			cout << endl;
			cout << nameID[ID] << "\tSchool: " << nameSchool[nameID[ID]] << endl
				<< "Components: " << nameComp[nameID[ID]] << endl
				<< "Range: " << nameRange[nameID[ID]] << endl
				<< "Duration: " << nameDur[nameID[ID]] << endl
				<< "Cast time: " << nameTime[nameID[ID]] << endl
				<< nameDesc[nameID[ID]] << endl;

		}
		//By range
		else if (selection == 7) {
			node* currentNode = range.getOrigin();
			int finder = 1;
			map<int, string> rangeSelect;

			//Displays all schools
			while (currentNode != NULL) {
				cout << finder << ". " << currentNode->data << endl;
				rangeSelect[finder] = currentNode->data;
				currentNode = currentNode->right;
				finder++;
			}

			int selection;
			cin >> selection;

			finder = 0;
			while (finder < nameID.size())
			{
				if (nameRange[nameID[finder]] == rangeSelect[selection])
				{
					cout << finder << ". " << nameID[finder] << endl;

				}
				finder++;
			}

			int ID;
			cout << "Spell ID to examine further: ";
			cin >> ID;

			cout << endl;
			cout << nameID[ID] << "\tSchool: " << nameSchool[nameID[ID]] << endl
				<< "Components: " << nameComp[nameID[ID]] << endl
				<< "Range: " << nameRange[nameID[ID]] << endl
				<< "Duration: " << nameDur[nameID[ID]] << endl
				<< "Cast time: " << nameTime[nameID[ID]] << endl
				<< nameDesc[nameID[ID]] << endl;
		}


	}


	system("pause");
	return 0;
}