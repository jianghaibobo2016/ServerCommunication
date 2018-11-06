/*
 * CommandParse.h
 *
 *  Created on: 2017年8月21日
 *      Author: root
 */

#ifndef COMMANDPARSE_H_
#define COMMANDPARSE_H_
#include <string>
#include <vector>
using namespace std;

class CommandParse {
public:
	CommandParse();
	virtual ~CommandParse();
	void addCommand(const string& commandname, const int parametercount, const string& commandexplain=string(""));
	void pollCommand();
	typedef enum {
		COMMAND_FINISH = 1,
		COMMAND_NOT_SUPPORT = 0,
		COMMAND_ERROR = 2
	}eCommandResult;
	virtual eCommandResult onCommand(const string& command, const vector<string> parameters) = 0;

private:
	void getLine(string& inputstring);
	void manCommand(const string& command);
	void helpCommand();
	bool checkParameter(const string& command, size_t intpucount);
	void splitinputstring(const string& inputstring, string& command, vector<string>& parameters);
	struct CommandUnit{
		string command;
		string explain;
		size_t parametercount;
	};
	vector<CommandUnit> _commands;
};

#endif /* COMMANDPARSE_H_ */
