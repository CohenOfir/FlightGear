#ifndef UNTITLED12_EX3_H
#define UNTITLED12_EX3_H

#include "ex1.h"

using namespace std;

class Command {
public:
    virtual int execute(int index) = 0;

    virtual ~Command();
};

class OpenServerCommand : public Command {
public:
    OpenServerCommand();

    ~OpenServerCommand();

    int execute(int index) override;
};

class ConnectCommand : public Command {
public:
    ConnectCommand();

    ~ConnectCommand();

    int execute(int index) override;
};

class DefineVarCommand : public Command {
public:
    DefineVarCommand();

    ~DefineVarCommand();

    int execute(int index) override;
};

class PrintCommand : public Command {
public:
    PrintCommand();

    ~PrintCommand();

    int execute(int index) override;
};

class SleepCommand : public Command {
public:
    SleepCommand();

    ~SleepCommand();

    int execute(int index) override;
};

class WhileCommand : public Command {
public:
    WhileCommand();

    ~WhileCommand();

    int execute(int index) override;
};

class SetValueCommand : public Command {
public:
    SetValueCommand();

    ~SetValueCommand();

    int execute(int index) override;
};

class IfCommand : public Command {
public:
    IfCommand();

    ~IfCommand();

    int execute(int index) override;
};

void parser();

void commandsMapInit();

bool expressionCheck(string leftSide, string rightSide, string oper);

vector<string> getExpression(string str);

void setVarsToInterpreter(Interpreter * inter);

void readBufferValues(int client_socket);

void setValsFromBuffer(vector<double> valsArr);

void updateSymbolsMap();

void setValuesToServer(int client_socket);

#endif //UNTITLED12_EX3_H