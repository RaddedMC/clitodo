// James N's command-line todo list app! -- listMgr.cpp

// toLower(string toBeLowered) -- Converts the given string to lowercase. Will modify the given variable!!
// moveItem(int srcDex, int desDex) -- Moves the list item at srcDex to the destination at desDex. Works from the global list variable.
// addItem(string item, string dd) -- Adds an item with name item and due date dd (or no due date if 'none') to the global list variable.
// deleteItem(int index) -- Deletes the item at the given index from the global list variable.
// printList() -- Prints a nicely formatted version of the global list variable to the user.
// editItem(int index, string item, string dd) -- Replaces the item from the global list variable at index with a new item with name item and due date dd (or no due date if 'none').


#include <iostream>
#include <string.h>
#include <string>
#include <stdio.h>
#include "throwErr.h"
#include "globals.h"
#include "listMgr.h"
#include "fileMgr.h"
using namespace std;

string toLower(string toBeLowered) {
	for (int i = 0; i < toBeLowered.length(); i++) {
		toBeLowered[i] = tolower(toBeLowered[i]);
	}
	return toBeLowered;
}

void moveItem(const int srcDex, const int desDex) {
	//Create a new list with more rows
	string ** tempList = new string*[num_pairs+1];

	//Copy everything into the list
	for (int i = 0; i < num_pairs; i++) {
		tempList[i] = list[i];
	}

	// If des < src (moving up)
	if (desDex < srcDex) {
		// Copy until you reach desDex, then copy value from indexSrc, then copy everything else after, skipping indexSrc
		for (int i = 0, offset = 0; i < num_pairs; i++) {
			if (i == desDex) {
				tempList[i] = list[srcDex];
				offset--;
			} else if (i == srcDex) {
				tempList[i] = list[i+offset];
				offset++;
			} else {
				tempList[i] = list[i+offset];
			}
		}
		delete[] list;
		list = tempList;
	}	// If src < des (moving down)
	else if (srcDex < desDex) {
		// Copy until you reach srcDex, skip it, and copy value from srcDex to desDex.
		for (int i = num_pairs-1, offset = 0; i >= 0; i--) {
			if (i == desDex) {
				tempList[i] = list[srcDex];
				offset++;
			} else if (i == srcDex) {
				tempList[i] = list[i+offset];
				offset--;
			} else {
				tempList[i] = list[i+offset];
			}
		}
		delete[] list;
		list = tempList;
	} else {
		// Do nothing, You're not moving anything anywhere :D
	}
	cout << endl;
	saveList();
}

void addItem(const string item, const string dd) {
	// Eliminate pipes and newlines
	string checkedItem;
	for (int i = 0; i < item.length(); i++) {
		if (!(item[i] == '|' || item[i] == '\n')) {
			checkedItem += item[i];
		}
	}

	string checkedDD;
	for (int i = 0; i < dd.length(); i++) {
		if (!(dd[i] == '|' || dd[i] == '\n')) {
			checkedDD += dd[i];
		}
	}

	//Create a new list with more rows
	string ** newList = new string*[num_pairs+1];

	//Copy everything into the list
	for (int i = 0; i < num_pairs; i++) {
		newList[i] = list[i];
	}

	//Put the new item at the bottom
	newList[num_pairs] = new string[2];
	newList[num_pairs][0] = checkedItem;
	if (strncmp(toLower(checkedDD).c_str(), "none", 4) != 0) {
		newList[num_pairs][1] = checkedDD;
	}
	delete[] list;
	list = newList;
	num_pairs++;
	saveList();
}

void deleteItem(const int index) {
	if (index >= 0 && index < num_pairs) {
		// Okay to delete
		cout << "Deleting " << list[index][0] << "..." << endl;
		// Create new list with one less item
		string ** newList = new string*[num_pairs-1];
		// Copy items until you reach certain index
		for (int i = 0; i < num_pairs-1; i++) {
			if (i < index) {
				newList[i] = list[i];
			} else {
				// One you reach that index, skip it and keep copying
				newList[i] = list[i+1];
			}
		}
		// Overwrite old list as new list
		delete[] list;
		list = newList;
		num_pairs--;
		saveList();
	} else {
		throwErr("Invalid index. Please pick a number that is in range.", 14);
	}
}

void printList() {
	if (num_pairs == 0) {
		cout << "---------------YOUR---LIST---------------" << endl;
		cout << "It looks like your todo list is empty!" << endl;
	}
	else {
		cout << "---------------YOUR---LIST---------------" << endl;
		for (int i = 0; i < num_pairs; i++) {
			cout << "#" << i+1 << ": " << list[i][0] << endl;
			if (!list[i][1].empty()) {
				cout << "DUE: " << list[i][1] << endl;
			}
			if (i != num_pairs-1) {
				cout << endl;
			}
		}
	}
	cout << "-----------------------------------------" << endl;
}

void editItem(const int index, const string item, const string dd) {
	//Check for pipe chars and newlines, like in addItem
	string checkedItem;
	for (int i = 0; i < item.length(); i++) {
		if (!(item[i] == '|' || item[i] == '\n')) {
			checkedItem += item[i];
		}
	}

	string checkedDD;
	for (int i = 0; i < dd.length(); i++) {
		if (!(dd[i] == '|' || dd[i] == '\n')) {
			checkedDD += dd[i];
		}
	}

	//Replace index on list with items
	list[index][0] = checkedItem;
	if (strncmp(toLower(checkedDD).c_str(), "none", 4) != 0) {
		list[index][1] = checkedDD;
	}

	saveList();
}