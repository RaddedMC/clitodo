// James N's command-line todo list app! -- main.cpp

// Stores list data in a text file that can be pulled up by calling the app's name
// Can edit order of items in list, due date for each item, or contents of each item
// Can 'finish' an item, deleting it

// clitodo   -- Reads and pulls up the list
// clitodo -a "CONTENTS" "DATE"   -- Adds a new item to the list, will prompt for contents and due date if not given
// clitodo -d #index   -- 'Finishes' an item, deleting it from the list
// clitodo -m #startdex #enddex   -- Moves an item
// clitodo -h   -- Pulls a help screen with version number
// clitodo -e #index "CONTENTS" "DATE" -- Changes the contents or due date of an item

// Learned about how to pull a specific environment variable ($HOME) for Linux here http://www.cplusplus.com/reference/cstdlib/getenv/

#define clitodo_ver 1.1
using namespace std;
#include "fileMgr.h"
#include "listMgr.h"
#include "throwErr.h"
#include "globals.h"

#include <iostream>
#include <string.h>
#include <string>
#include <stdio.h>

string ** list;
int num_pairs;

string toLower(string toBeLowered);

int main(int argc, char * argv[], char * env[]) {
	try {
		list = pullList();
	} catch (int e) {
		if (e == FILE_READ_ERROR) {
			throwErr("Unable to read listFile at " + getTDFileLocation() + ". Do you have permission?", e);
		} else if (e == FILE_WRITE_ERROR) {
			throwErr("Unable to write a listFile at " + getTDFileLocation() + ". Do you have permission?", e);
		}
	}
	switch (argc) {
		// One arg = print list + help
		case 1: {
			printList();
			cout << "-a to ADD, -d to DELETE, -m to MOVE, -e to EDIT, -h for HELP." << endl;
		break;}

		// Two args
		case 2: {
			printList();
			char * arg2 = argv[1];
			if (strncmp(arg2, "-a", 3) == 0) {
				// ADD
				while (true) {
					cout << "What would you like to add to the list? ";
					string newItem;
					getline(cin, newItem); //Found getline here: https://www.geeksforgeeks.org/getline-string-c/

					cout << "Does this item have a due date?\nEx. \"4/20/2020\", \"When that new Manjaro Update releases\"" << endl;
					cout << "If there is no due date, just type 'none'. ";
					fflush(stdout);
					string dd;
					getline(cin, dd);
					addItem(newItem, dd);
					cout << endl;
					printList();
					return 0;
				}
			}
			if (strncmp(arg2, "-h", 3) == 0) {
				cout << "RaddedMC's clitodo v" << clitodo_ver << endl;
				cout << "ListManager v" << listMgr_ver << endl;
				cout << "FileManager v" << fileMgr_ver << endl;
				cout << "clitodo   -- Reads and pulls up the list" << endl;
				cout << "clitodo -a \"CONTENTS\" \"DATE\"   -- Adds a new item to the list, will prompt for contents and due date if not given" << endl;
				cout << "clitodo -d #index   -- 'Finishes' an item, deleting it from the list" << endl;
				cout << "clitodo -m #startdex #enddex   -- Moves an item" << endl;
				cout << "clitodo -e #index \"CONTENTS\" \"DATE\" -- Changes the contents or due date of an item" << endl;
				cout << "clitodo -h   -- Pulls up this help screen" << endl;
				return 0;
			}
			if (strncmp(arg2, "-d", 3) == 0) {
				while (true) {
					if (num_pairs == 0) {
						throwErr("You have nothing in your list to delete! Add something and try again!");
					}
					cout << "What item # would you like to delete? ";

					string convertToI;
					getline(cin, convertToI);
					int index;
					try {
						index = stoi(convertToI.c_str());
					} catch (const std::invalid_argument&) { // Found out how to catch c++ exceptions (non-numeric) here: https://en.cppreference.com/w/cpp/language/try_catch
						cerr << "Invalid argument. Did you enter a number?" << endl;
						cerr << "Try again." << endl;
						continue;
					} catch (const std::out_of_range&) {
						cerr << "Invalid argument. It seems like your number is either really large or really small." << endl;
						cerr << "Try again." << endl;
						continue;
					}

					index--;
					if (index > (num_pairs-1) || index < 0) {
						cerr << "That number is out of bounds. Try again!" << endl;
						continue;
					}
					deleteItem(index);
					printList();
					return 0;
				}
			}
			if (strncmp(arg2, "-m", 3) == 0) {
				while (true) {
					if (num_pairs == 0) {
						throwErr("You have nothing in your list to move! Add something and try again!");
					} if (num_pairs == 1) {
						throwErr("You have only one item! Add something and try again!");
					}
					cout << "What item # would you like to move? ";

					string convertToI;
					getline(cin, convertToI);
					int indexSrc;
					try {
						indexSrc = stoi(convertToI.c_str());
					} catch (const std::invalid_argument&) { // Found out how to catch c++ exceptions (non-numeric) here: https://en.cppreference.com/w/cpp/language/try_catch
						cerr << "Invalid argument. Did you enter a number?" << endl;
						cerr << "Try again." << endl;
						continue;
					} catch (const std::out_of_range&) {
						cerr << "Invalid argument. It seems like your number is either really large or really small." << endl;
						cerr << "Try again." << endl;
						continue;
					}

					indexSrc--;
					cout << "What position would you like to move it to? (1-" << num_pairs << "): ";

					getline(cin, convertToI);
					int indexDes;
					try {
						indexDes = stoi(convertToI.c_str());
					} catch (const std::invalid_argument&) { // Found out how to catch c++ exceptions (non-numeric) here: https://en.cppreference.com/w/cpp/language/try_catch
						cerr << "Invalid argument. Did you enter a number?" << endl;
						cerr << "Try again." << endl;
						continue;
					} catch (const std::out_of_range&) {
						cerr << "Invalid argument. It seems like your number is either really large or really small." << endl;
						cerr << "Try again." << endl;
						continue;
					}

					indexDes--;
					if (indexSrc > (num_pairs-1) || indexSrc < 0 || indexDes > (num_pairs-1) || indexDes < 0 ) {
						cerr << "That number is out of bounds. Try again!" << endl;
						continue;
					}
					moveItem(indexSrc, indexDes);
					printList();
					return 0;
				}
			}
			if (strncmp(arg2, "-e", 3) == 0) {
				while (true) {
					if (num_pairs == 0) {
						throwErr("You have nothing in your list to edit! Add something and try again!");
					}
					cout << "What item # would you like to edit? ";
					string convertToI;
					getline(cin, convertToI);
					int index;
					try {
						index = stoi(convertToI.c_str());
					} catch (const std::invalid_argument&) { // Found out how to catch c++ exceptions (non-numeric) here: https://en.cppreference.com/w/cpp/language/try_catch
						cerr << "Invalid argument. Did you enter a number?" << endl;
						cerr << "Try again." << endl;
						continue;
					} catch (const std::out_of_range&) {
						cerr << "Invalid argument. It seems like your number is either really large or really small." << endl;
						cerr << "Try again." << endl;
						continue;
					}

					index--;
					if (index > (num_pairs-1) || index < 0) {
						cerr << "That number is out of bounds. Try again!" << endl;
						continue;
					}

					fflush(stdout);

					cout << "Would you like to change that item's name? [y/N]: ";
					string option;
					getline(cin, option);
					string newItem = list[index][0];
					string dd = list[index][1];
					fflush(stdout);
					if (strncmp(toLower(option).c_str(), "y", 2) == 0) {
						cout << "What would you like to change that item to? ";
						getline(cin, newItem); //Found getline here: https://www.geeksforgeeks.org/getline-string-c/
					}

					fflush(stdout);

					cout << "Would you like to change its due date? [y/N]: ";
					getline(cin, option);
					fflush(stdout);
					if (strncmp(toLower(option).c_str(), "y", 2) == 0) {
						cout << "What would you like to change that due date to? ";
						cout << "If you want to remove it, type 'none': ";
						getline(cin, dd); //Found getline here: https://www.geeksforgeeks.org/getline-string-c/
					}

					editItem(index, newItem, dd);

					cout << endl;
					printList();
					return 0;
				}
			}
			throwErr("Unrecognized argument.", 5);
		break;}

		// Three args
		case 3:{
			char * arg2 = argv[1];
			char * arg3 = argv[2];
			if (strncmp(arg2, "-a", 3) == 0) {
				// ADD
				string newItem = arg3;
				addItem(newItem, "none");
				cout << endl;
				printList();
				return 0;
			}
			if (strncmp(arg2, "-d", 3) == 0) {
				// DELETE
				if (num_pairs == 0) {
					throwErr("You have nothing in your list to delete! Add something and try again!");
				}
				int index;
				try {
					index = stoi(arg3);
				} catch (const std::invalid_argument&) { // Found out how to catch c++ exceptions (non-numeric) here: https://en.cppreference.com/w/cpp/language/try_catch
					throwErr("Invalid argument. Did you enter a number?");
				} catch (const std::out_of_range&) {
					throwErr("Invalid argument. It seems like your number is either really large or really small.");
				}
				index--;
				if (index > (num_pairs-1) || index < 0) {
					throwErr("That number is out of bounds. Try again!");
				}
				deleteItem(index);
				printList();
				return 0;
			}
			if (strncmp(arg2, "-e", 3) == 0) {
				while (true) {
					if (num_pairs == 0) {
						throwErr("You have nothing in your list to edit! Add something and try again!");
					}
					int index;
					try {
						index = stoi(arg3);
					} catch (const std::invalid_argument&) { // Found out how to catch c++ exceptions (non-numeric) here: https://en.cppreference.com/w/cpp/language/try_catch
						throwErr("Invalid argument. Did you enter a number?");
					} catch (const std::out_of_range&) {
						throwErr("Invalid argument. It seems like your number is either really large or really small.");
					}

					index--;
					if (index > (num_pairs-1) || index < 0) {
						throwErr("That number is out of bounds.");
					}

					fflush(stdout);

					cout << "Would you like to change that item's name? [y/N]: ";
					string option;
					getline(cin, option);
					string newItem = list[index][0];
					string dd = list[index][1];
					fflush(stdout);
					if (strncmp(toLower(option).c_str(), "y", 2) == 0) {
						cout << "What would you like to change that item to? ";
						getline(cin, newItem); //Found getline here: https://www.geeksforgeeks.org/getline-string-c/
					}

					fflush(stdout);

					cout << "Would you like to change its due date? [y/N]: ";
					getline(cin, option);
					fflush(stdout);
					if (strncmp(toLower(option).c_str(), "y", 2) == 0) {
						cout << "What would you like to change that due date to? ";
						cout << "If you want to remove it, type 'none': ";
						getline(cin, dd); //Found getline here: https://www.geeksforgeeks.org/getline-string-c/
					}

					editItem(index, newItem, dd);

					cout << endl;
					printList();
					return 0;
				}
			}
			throwErr("Unrecognized argument.");
		break;}

		// Four args!
		case 4:{
			char * arg2 = argv[1];
			char * arg3 = argv[2];
			char * arg4 = argv[3];
			if (strncmp(arg2, "-a", 3) == 0) {
				// ADD
				string newItem = arg3;
				string dd = arg4;
				addItem(newItem, arg4);
				cout << endl;
				printList();
				return 0;
			}
			if (strncmp(arg2, "-m", 3) == 0) {
				if (num_pairs == 0) {
					throwErr("You have nothing in your list to move! Add something and try again!");
				} if (num_pairs == 1) {
					throwErr("You have only one item! Add something and try again!");
				}
				int indexSrc; int indexDes;
				try {
					indexSrc = stoi(arg3);
					indexDes = stoi(arg4);
				} catch (const std::invalid_argument&) { // Found out how to catch c++ exceptions (non-numeric) here: https://en.cppreference.com/w/cpp/language/try_catch
					throwErr("Invalid argument. Did you enter a number?");
				} catch (const std::out_of_range&) {
					throwErr("Invalid argument. It seems like your number is either really large or really small.");
				}
				indexSrc--; indexDes--;
				if (indexSrc > (num_pairs-1) || indexSrc < 0 || indexDes > (num_pairs-1) || indexDes < 0 ) {
					throwErr("One of your numbers is out of bounds. Try again!");
					
				}
				moveItem(indexSrc, indexDes);
				printList();
				return 0;
			}
			if (strncmp(arg2, "-e", 3) == 0) {
				while (true) {
					if (num_pairs == 0) {
						throwErr("You have nothing in your list to edit! Add something and try again!");
					}
					int index;
					try {
						index = stoi(arg3);
					} catch (const std::invalid_argument&) { // Found out how to catch c++ exceptions (non-numeric) here: https://en.cppreference.com/w/cpp/language/try_catch
						throwErr("Invalid argument. Did you enter a number?");
					} catch (const std::out_of_range&) {
						throwErr("Invalid argument. It seems like your number is either really large or really small.");
					}

					index--;
					if (index > (num_pairs-1) || index < 0) {
						throwErr("That number is out of bounds.");
					}

					editItem(index, arg4, "none");

					cout << endl;
					printList();
					return 0;
				}
			}
			throwErr("Unrecognized argument.");
		break;}

		case 5:{
			char * arg2 = argv[1];
			char * arg3 = argv[2];
			char * arg4 = argv[3];
			char * arg5 = argv[4];
			if (strncmp(arg2, "-e", 3) == 0) {
				while (true) {
					if (num_pairs == 0) {
						throwErr("You have nothing in your list to edit! Add something and try again!");
					}
					int index;
					try {
						index = stoi(arg3);
					} catch (const std::invalid_argument&) { // Found out how to catch c++ exceptions (non-numeric) here: https://en.cppreference.com/w/cpp/language/try_catch
						throwErr("Invalid argument. Did you enter a number?");
					} catch (const std::out_of_range&) {
						throwErr("Invalid argument. It seems like your number is either really large or really small.");
					}

					index--;
					if (index > (num_pairs-1) || index < 0) {
						throwErr("That number is out of bounds.");
					}

					editItem(index, arg4, arg5);

					cout << endl;
					printList();
					return 0;
				}
			}
			throwErr("Unrecognized argument.");
		}

		default:{
			throwErr("Too many arguments! Did you forget to surround any text in \"quotes?\"");
		}
	}
}
