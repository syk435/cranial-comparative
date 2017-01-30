// PreProcessingBaseConnectiveFaces.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
	ifstream input;
	input.open(argv[1]);

	ofstream output;
	output.open(argv[2]);

	if (!output.is_open()) {
		cout << "Cannot open file!" << argv[2] << endl;
	}

	if (!input.is_open()) {
		cout << "Cannot open file!" << argv[1] << endl;
	}

	if (std::string(argv[1]).find(".obj") != string::npos) 
	{
		std::string s;
		while (std::getline(input, s))
		{
			if (s.size()>0 && s.at(0) == 'f')
			{
				output << s << "\n";
			}
		}
	}

	input.close();
	output.close();
}

