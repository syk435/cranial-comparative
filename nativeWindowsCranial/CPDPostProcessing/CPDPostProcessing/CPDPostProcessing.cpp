// CPDPostProcessing.cpp : Defines the entry point for the console application.
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

	ifstream input1;
	input1.open(argv[2]);

	ifstream input2;
	input2.open(argv[3]);

	ifstream input3;
	input3.open(argv[4]);

	ofstream output;
	output.open(argv[5]);

	if (!output.is_open()) {
		cout << "Cannot open file!" << argv[2] << endl;
	}

	if (!input.is_open()) {
		cout << argv[1] << endl;
	}

	std::string s;
	while (std::getline(input, s))
	{
		if (s.size()>0)
		{
			output << s << "\n";
		}
	}

	while (std::getline(input1, s))
	{
		if (s.size()>0)
		{
			output << s << "\n";
		}
	}

	while (std::getline(input2, s))
	{
		if (s.size()>0)
		{
			output << s << "\n";
		}
	}

	output << endl;
	while (std::getline(input3, s))
	{
		if (s.size()>0)
		{
			output << s << "\n";
		}
	}

	input.close();
	input1.close();
	input2.close();
	input3.close();
	output.close();
}

