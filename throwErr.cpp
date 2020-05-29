// RaddedMC's quick error thrower v1.0 -- throwErr.cpp

#include <string>
#include <iostream>
using namespace std;

#include "throwErr.h"

void throwErr(string reason, int code) {
	cerr << "Error: " << reason << endl;
	cerr << "Returning exit code " << code << "." << endl;
	exit(code);
}

void throwErr(string reason) {
	throwErr(reason, ITS_THE_USERS_FAULT);
}