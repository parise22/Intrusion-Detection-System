#include <iostream>
#include <string>
#include <fstream>
#include "function.h"
using namespace std;

int main(int argc, char *argv[])
{
	if(!argv[4])
	{
		//Event File
		readEvents(argv[1]);
		readBaseData(argv[2]);
		readTestEvents(argv[3]);

	}else{
		cerr << "Not correct amount of arguments added...\nPlease provide Events, Base-Data and Test-Events." << endl;
	}

	return 0;
} 
