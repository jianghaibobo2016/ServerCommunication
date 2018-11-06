/*
 * CommandParse.cpp
 *
 *  Created on: 2017年8月21日
 *      Author: root
 */

#include "CommandParse.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ThreadUtil.h>
#include <iostream>
using namespace std;
CommandParse::CommandParse() {
	// TODO Auto-generated constructor stub

}

CommandParse::~CommandParse() {
	// TODO Auto-generated destructor stub
}

void CommandParse::addCommand(const string& command,
		const int parametercount, const string& commandexplain) {
	for (size_t i = 0; i < _commands.size(); i++) {
		if (command == _commands[i].command) {
			cerr << "Error: exist command " << command << endl;
			return;
		}
	}
	CommandUnit commandunit;
	commandunit.command = command;
	commandunit.parametercount = parametercount;
	if (commandexplain.length() == 0) {
		char strtemp[1024];
		sprintf(strtemp, "%s has %d count parameter", command.c_str(),
				(int) parametercount);
		commandunit.explain = strtemp;
	} else
		commandunit.explain = commandexplain;
	_commands.push_back(commandunit);
}
void CommandParse::getLine(string& inputstring) {
	inputstring.clear();
	int inputchar;
	while (1) {
		inputchar = getchar();
		if (inputchar == EOF) //CTRL+D
		{
			ThreadUtil::Sleep(500);
			return;
		}
		else if (inputchar == 0x0a) //ENTER
			return;
		else {
			inputstring.push_back((char) inputchar);
		}
	}
}
void CommandParse::splitinputstring(const string& inputstring, string& command,
		vector<string>& parameters) {
	char* pSrcStr = (char*) inputstring.c_str();
	char* pFindPos;
	string splitstring;
	vector<string> splitstrings;
	command.clear();
	parameters.clear();
	while (1) {

		pFindPos = strchr(pSrcStr, 0x20); //find first space
		if (pFindPos == NULL) {
			if (strlen(pSrcStr) > 0)
				splitstrings.push_back(pSrcStr);
			else
				//empty string
				;
			break;
		} else {
			if (pSrcStr == pFindPos) //first is space
				pSrcStr += 1;
			else { //find space OK
				splitstring.clear();
				splitstring.insert(splitstring.end(), pSrcStr, pFindPos);
				//splitstring.push_back('\0');
				splitstrings.push_back(splitstring);
				pSrcStr = pFindPos;
			}
		}
	} //end while
	if (splitstrings.size() > 0) {
		command = splitstrings[0];
		splitstrings.erase(splitstrings.begin());
		parameters = splitstrings;
	} else {
		command = "";
		parameters.clear();
	}
}

void CommandParse::pollCommand() {
	vector<string> parameters;
	string command;
	string inputstring;
	while (1) {
		getLine(inputstring);
		splitinputstring(inputstring, command, parameters);
		if (command == "help") {
			helpCommand();
		} else if (command == "") {
			helpCommand();
		} else if (command == "man") {
			if (parameters.size() == 1) {
				manCommand(parameters[0]);
			} else { //man Command error
				cerr << "man Error , please use:" << endl << "    man Command"
						<< endl;
			}
		} else if (command == "clear") {
			system("clear");
		} else if (command == "exit") {
			return;
		} else {
			if (!checkParameter(command, parameters.size())) {
				continue;
			}
			eCommandResult ret = onCommand(command, parameters);
			if (ret == COMMAND_ERROR)
				cerr << "command parse error" << endl;
			else if (ret == COMMAND_NOT_SUPPORT)
				cerr << "not support command" << endl;
			else
				;
		}

	}
}
void CommandParse::manCommand(const string& command) {
	for (size_t i = 0; i < _commands.size(); i++) {
		if (command == _commands[i].command) {
			cout << _commands[i].command << ":" << _commands[i].explain << endl;
			return;
		}
	}
	cout << "command:" << command << " not find" << endl;
}
void CommandParse::helpCommand() {
	cout << "HELP: command list(use man show parameters)" << endl;
	for (size_t i = 0; i < _commands.size(); i++) {
		cout << "\t" << (i + 1) << "." << _commands[i].command << endl;
	}
}
bool CommandParse::checkParameter(const string& command, size_t intpucount) {
	for (size_t i = 0; i < _commands.size(); i++) {
		if (command == _commands[i].command) {
			if (_commands[i].parametercount == intpucount)
				return true;
			else {
				cout << "parameter input error" << endl;
				manCommand(command);
				return false;
			}
		}
	}
	cout << "Command not found" << endl;
	return false;
}
