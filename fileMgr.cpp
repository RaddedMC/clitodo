// James N's command-line todo list app! -- fileMgr.cpp

// readFile(string location) -- Returns a really long string that contains the data of a full text file.
// writeFile(string location, string text) -- Will write text to location.
// getTDFileLocation() -- The To-do list file is stored under the root of the user's home folder. This function returns that directory.
// saveList() -- Will read from the global list variable and save it to the TDFileLocation.
// parseTDFile(string TDFile) -- Takes a read in To-do list file, and will save its data to the global list variable.
// pullList() -- Performs the steps needed to read the list from the file, create the file if needed, and parse it to the global variable.


// Learned about file reads / stringstreams here https://www.tutorialspoint.com/Read-whole-ASCII-file-into-Cplusplus-std-string
// Learned about file output here https://stackoverflow.com/questions/15388041/how-to-write-stdstring-to-file
// Learned about exceptions / handling here http://www.cplusplus.com/doc/tutorial/exceptions/

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include "fileMgr.h"
#include "globals.h"
using namespace std;

string readFile(const string location) {
	ifstream inFile(location);
	string output;
	if (inFile) {
		stringstream outputS;
		outputS << inFile.rdbuf();
		output = outputS.str();
		inFile.close();
		return output;
	} else {
		throw FILE_READ_ERROR;
	}
}

void writeFile(const string location, const string text) {
	ofstream outFile(location);
	if (outFile) {
		outFile << text;
		outFile.close();
	} else {
		throw FILE_WRITE_ERROR;
	}
}

string getTDFileLocation() {
	string location = getenv("HOME");
	location.append("/.clitodo-listfile-DONOTEDIT");
	return location;
}

void saveList() {
	string outputFile;
	if (num_pairs == 0) {
		//An empty file should have two newlines
		outputFile += "\n";
	} else {
		for (int i = 0; i < num_pairs; i++) {
			outputFile += (list[i][0]);
			outputFile += "|";
			if (!list[i][1].empty()) {
				outputFile += (list[i][1]);
			}
			outputFile += "\n";
		}
	}
	outputFile += "\n";
	writeFile(getTDFileLocation(), outputFile);
	cout << "Changes saved!" << endl;
}

string ** parseTDFile(const string TDFile) {
	//cout << "DEBUG: Begin parsing..." << endl;
	int target_string = 0;
	num_pairs = 0;
	int current_pair = 0;


	//Look for \n's to determine number of pairs to allocate
	if (strncmp(TDFile.c_str(), "\n\n", 2) == 0) {
		num_pairs = 0;
		list = new string*[num_pairs];
		return list;
	}

	for (int i = 0; TDFile[i] != '\0'; i++) {
		if (TDFile[i] == '\n') {
			num_pairs++;
		}
	}
	num_pairs--; // To account for the \n at the end of the file
	//cout << "DEBUG: " << num_pairs << " pairs are needed... Begin allocation" << endl;

	//Allocate all pairs
	string ** parsedTDFile = new string*[num_pairs];
	//cout << "DEBUG: " << num_pairs << " pairs are allocated... Begin allocating each pair" << endl;
	for (int i = 0; i != num_pairs; i++) {
		//Allocate each pair
		parsedTDFile[i] = new string[2];
		//cout << "DEBUG: Pair " << i+1 << " Allocated!" << endl;
	}

	// Iterate through every character in the TDFile
	for (int i = 0; TDFile[i] != '\0'; i++) {
		// Read until you hit a pipe, null, or endl
		//cout << "DEBUG: Reading char #" << i << ", " << TDFile[i] << endl;
		if (TDFile[i] == '|') {
			// If that character was a pipe, we know to populate next string
			target_string = 1;
			//cout << "DEBUG: Read a pipe, setting target to dd string" << endl;
		} else if (TDFile[i] == '\n') {
			target_string = 0;
			current_pair++;
			//cout << "DEBUG: Read a \\n, setting target to title string" << endl;
		} else {
			parsedTDFile[current_pair][target_string] += TDFile[i];
			//cout << "DEBUG: char saved to parsedTDFile[" << num_pairs << "][" << target_string << "][" << i << "]" << endl;
		}
	}
	//cout << "DEBUG: Reading finished. Returning to calling function" << endl;
	// If the first character is null, don't do anything as the file is empty.
	list = parsedTDFile; // TODO: Change back to pass-by
	return parsedTDFile;
}

string ** pullList() {
	string TDFileLocation = getTDFileLocation();
	string unParsedTDFile;
	try {
		unParsedTDFile = readFile(TDFileLocation);
	} catch (int e) {
		// No file. Create it!
		cout << "No list file! Creating one at " << TDFileLocation << "..." << endl;
		try {
			writeFile(TDFileLocation, "\n\n");
		} catch (int e) {
			//Error creating file.
			throw FILE_WRITE_ERROR;
		}
		cout << "File successfully created." << endl;
		cout << "-a to ADD, -d to DELETE, -m to MOVE, -h for HELP." << endl;
		exit(0);
	}
	return parseTDFile(unParsedTDFile);
}