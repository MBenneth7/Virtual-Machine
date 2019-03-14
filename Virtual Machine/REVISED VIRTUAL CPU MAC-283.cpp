// REVISED VIRTUAL CPU MAC-283.cpp : Defines the entry point for the console application.
//

/*
MAC-283
VIRTUAL CPU LAB
Mark Sayong
E-mail: Mbenneth7@yahoo.com
*/

//#include "stdafx.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <iomanip>
#include <stdio.h>
#include <cmath>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

//GLOBAL VARIABLES

int opcode; // represents the commands' const int value
double operand; // keeps track of the numbers stored in the ram
int step = 0; // keeps track of ram
double acc = 0; //holds answer of computation
string filename;
ifstream file;
ofstream userFile;

//GLOBAL ARRAYS

vector <double> ram;
map <string, int> textCmds;
map <string, int> ::iterator iTxt;

//INSTRUCTION SET

const int CLR = 0;
const int ADD = 1;
const int SUB = 2;
const int MUL = 3;
const int DIV = 4;
const int POW = 5;
const int BNZ = 6;
const int BNZR = 7;
const int OUT = 8;
const int HALT = 9;
const int LOAD = 10;
const int STORE = 11;
const int JMP = 12;

//PROTOTYPE FUNCTIONS

void virtualMenu();
void getFile(ifstream &);
void createFile();
void textSetup();
void loadText();
void execute(int);
void executionProcess();
void resetProgram();


int main()
{
	virtualMenu();
}

//FUNCTION SPECIFICATIONS

void virtualMenu()
{

	int select;
	bool progRun = true;
	bool menuSelect = false;

	while (progRun != false)
	{
		cout << "\tVIRTUAL CPU MENU\n\n";

		cout << "COMMANDS\n\n";
		cout << "1 - Open file \n";
		cout << "2 - Execute File\n";
		cout << "3 - Create File\n";
		cout << "4 - Close current file\n";
		cout << "5 - Shut Down\n";

		do
		{
			cout << "\nSelect Command(#1-5): ";
			cin >> select;

			if (cin.fail()) // prevents unnecessary inputs
			{
				cin.clear();
				cin.ignore();
				cout << "Invalid input. \n";
			}
			else
				menuSelect = true;

		} while (menuSelect == false);

		cout << endl;

		switch (select) // cases for selecting options
		{
            case 1:
                cout << "Opening File...\n";

                if (!file.is_open())
                    getFile(file);
                else
                    cout << "\nThere is a file currently in use.\n\n";

                break;

            case 2:

                cout << "Executing File...\n\n";

                executionProcess();

                cout << endl << endl;
                break;

            case 3:

                cout << "Creating File...\n\n";

                if (file.is_open())
                    cout << "There is a file currently in use.\n\n";
                else
                    createFile();
                break;

            case 4:

                cout << "Closing current file...\n\n";

                if (!file.is_open())
                    cout << "There is no file in use. \n\n";
                else
                {
                    file.close();
                    cout << "File is closed.\n\n";
                }
                break;

            case 5:
                cout << "Shutting Down...\n\n";
                progRun = false;
                break;

            default:
                cout << "Invalid input.\n\n";
                break;
		}

	}
}


void getFile(ifstream &file)
{
	cout << "\nEnter filename: ";
	cin >> filename;

	file.open(filename.c_str());

	if (file.is_open()) // checks if file is open
		cout << "\nFile successfully open\n\n";
	else
		cout << "\nError opening file.\n\n";
}

void textSetup() // setup map language
{
	textCmds["CLR"] = CLR;
	textCmds["ADD"] = ADD;
	textCmds["SUB"] = SUB;
	textCmds["MUL"] = MUL;
	textCmds["DIV"] = DIV;
	textCmds["POW"] = POW;
	textCmds["BNZ"] = BNZ;
	textCmds["BNZR"] = BNZR;
	textCmds["OUT"] = OUT;
	textCmds["HALT"] = HALT;
	textCmds["LOAD"] = LOAD;
	textCmds["STORE"] = STORE;
	textCmds["JMP"] = JMP;
}

void loadText() // analyze text in file and assigns map value
{
	string text;
	int pos = 1;

	cout << "PROGRAM INSTRUCTIONS\n";
	while (!file.eof() && file >> text)
	{
		cout << pos++ << ": " << text << endl;
		iTxt = textCmds.find(text);

		if (iTxt != textCmds.end())
			ram.push_back(iTxt->second);
		else
			ram.push_back(atoi(text.c_str())); //"atoi" turns string into int for C++ 99 instead of "stoi"
	}
}

void execute(int opcode) // Command Specifications
{
	switch (opcode)
	{
	case CLR:
		acc = 0;
		step++;
		break;

	case ADD:
		operand = ram[++step];
		acc = acc + operand;
		step++;
		break;

	case SUB:
		operand = ram[++step];
		acc = acc - operand;
		step++;
		break;

	case MUL:
		operand = ram[++step];
		acc = acc * operand;
		step++;
		break;

	case DIV:
		operand = ram[++step];
		acc = acc / operand;
		step++;
		break;

	case POW:
		operand = ram[++step];
		acc = pow(acc, operand);
		step++;
		break;

	case BNZ:
		operand = ram[++step];
		if (acc != 0)
			step = operand;
		else
			step++;
		break;

	case BNZR:
		operand = ram[++step];
		if (acc != 0)
			step = step + operand;
		else
			step++;
		break;

	case LOAD:
		operand = ram[++step];
		acc = ram[operand];
		step++;
		break;

	case STORE:
		operand = ram[++step];
		ram[operand] = acc;
		step++;
		break;

	case JMP:
		operand = ram[++step];
		step = operand;
		break;

	case OUT:
		cout << "\nANSWER: " << acc;
		step++;
		break;

	case HALT:
		break;
	}
}

void executionProcess()
{
	if (file.is_open())
	{
		textSetup();
		loadText();

		do
		{
			opcode = ram[step];
			execute(opcode);
		} while (opcode != HALT);
	}
	else
		cout << "Please open/create a file.\n";

	if (file.is_open())
		resetProgram(); //resets everything after execution

}

void createFile()
{

	string inputName, userInput;
	char ans;

	cout << "Filename: ";
	cin >> inputName;
	userFile.open(inputName.c_str());

	if (!userFile)
		cout << "\nFile could not be opened.\n";

	cout << "\nEnter Commands and Variables\n";
	cout << "Enter 'end' to finish\n\n";
	cout << "COMMAND LIST\n";

	while (getline(cin, userInput))
	{
		transform(userInput.begin(), userInput.end(), userInput.begin(), ::toupper); //Takes string and converts it to upper case
		userFile << userInput << endl;

		if (userInput == ("END"))
			break;
	}

	userFile.close();
	cout << "\nFile has been created.\n\n";
}

void resetProgram() // Resets and clears existing data in program
{
	file.close();
	cout << "\n\nExecution terminated.\nFile closed.\n";
	acc = 0;
	step = 0;
	textCmds.clear();
	ram.clear();
}

