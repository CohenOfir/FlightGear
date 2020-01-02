#include <iostream>
#include <bits/stdc++.h>
#include "fstream"
#include "ex3.h"
#include <unordered_map>
#include <thread>
#include <math.h>
#include <queue>
#include <stack>
#include <unistd.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;
//Global Variables
vector<string> arr;
//commans to bet sent to server
queue<string> commandsToServer;
unordered_map<string, Command *> commandsMap;
//maps of values recieved from server
unordered_map<string, Variable *> symbolsMap;
//maps between sims and values
unordered_map<string, double> clientValsMap;

//----Command Interface---
Command::~Command() {

}

//----------OpenServerCommand----------
OpenServerCommand::OpenServerCommand() {

}

OpenServerCommand::~OpenServerCommand() {

}
/**
 * the function goes to index in arr created by lexer and opens a server with "port" address.
 * @param index
 * @return - int - number of cells to skip in arr created by lexer.
 */
int OpenServerCommand::execute(int index) {
    int port = stoi(arr[index + 1]);
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd == -1) {
        cout << "could not open socket" << endl;
        exit(1);
    }
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(socketfd, (struct sockaddr *) &address, sizeof(address)) == -1) {
        cout << "could not bind the socket to an IP" << endl;
        exit(1);
    }
    if (listen(socketfd, SOMAXCONN) == -1) {
        cout << "Error during listening to the port " << endl;
    } else {
        std::cout << "Server is now listening ..." << std::endl;
    }


    socklen_t size = sizeof(address);
    int client_socket = accept(socketfd, (struct sockaddr *) &address, &size);
    if (client_socket == -1) {
        std::cerr << "Error accepting client" << std::endl;
        exit(1);
    }
    close(socketfd);

    thread thread1(readBufferValues, client_socket);
    thread1.detach();

    return 2;
}

/**
 * the function run an endless loop(till connection is over), which is reading raw by raw from the client
 * @param client_socket
 */
void readBufferValues(int client_socket) {
    while (1) {
        //reading from client
        char buffer[1024] = {0};
        int valread = read(client_socket, buffer, 1024);
        string stringBuffer = buffer;
        vector<double> valsArr;
        for (int i = 0; i < 36; i++) {
            if (stringBuffer.find(",") != string::npos) {
                string temp = stringBuffer;
                temp.erase(stringBuffer.find(",") + 1, stringBuffer.length());

                //taking 3 points after dot
                double pointLocation = temp.find(".");
                if (temp.length() - pointLocation > 3) {
                    temp.erase(pointLocation + 4, temp.length());
                }

                double value = stod(temp);
                valsArr.push_back(value);
                stringBuffer.erase(0, stringBuffer.find(",") + 1);

                //last value in string
            } else {
                double pointLocation = stringBuffer.find(".");
                if (stringBuffer.length() - pointLocation > 3) {
                    stringBuffer.erase(pointLocation + 4, stringBuffer.length());
                }

                double value = stod(stringBuffer);
                valsArr.push_back(value);
            }
        }
        setValsFromBuffer(valsArr);
        updateSymbolsMap();
    }
}
/**
 * the function is mapping sim to the updated value we read from client
 * @param valsArr
 */
void setValsFromBuffer(vector<double> valsArr) {
    clientValsMap["/instrumentation/airspeed-indicator/indicated-speed-kt"] = valsArr[0];
    clientValsMap["/sim/time/warp"] = valsArr[1];
    clientValsMap["/controls/switches/magnetos"] = valsArr[2];
    clientValsMap["/instrumentation/heading-indicator/offset-deg"] = valsArr[3];
    clientValsMap["/instrumentation/altimeter/indicated-altitude-ft"] = valsArr[4];
    clientValsMap["/instrumentation/altimeter/pressure-alt-ft"] = valsArr[5];
    clientValsMap["/instrumentation/attitude-indicator/indicated-pitch-deg"] = valsArr[6];
    clientValsMap["/instrumentation/attitude-indicator/indicated-roll-deg"] = valsArr[7];
    clientValsMap["/instrumentation/attitude-indicator/internal-pitch-deg"] = valsArr[8];
    clientValsMap["/instrumentation/attitude-indicator/internal-roll-deg"] = valsArr[9];
    clientValsMap["/instrumentation/encoder/indicated-altitude-ft"] = valsArr[10];
    clientValsMap["/instrumentation/encoder/pressure-alt-ft"] = valsArr[11];
    clientValsMap["/instrumentation/gps/indicated-altitude-ft"] = valsArr[12];
    clientValsMap["/instrumentation/gps/indicated-ground-speed-kt"] = valsArr[13];
    clientValsMap["/instrumentation/gps/indicated-vertical-speed"] = valsArr[14];
    clientValsMap["/instrumentation/heading-indicator/indicated-heading-deg"] = valsArr[15];
    clientValsMap["/instrumentation/magnetic-compass/indicated-heading-deg"] = valsArr[16];
    clientValsMap["/instrumentation/slip-skid-ball/indicated-slip-skid"] = valsArr[17];
    clientValsMap["/instrumentation/turn-indicator/indicated-turn-rate"] = valsArr[18];
    clientValsMap["/instrumentation/vertical-speed-indicator/indicated-speed-fpm"] = valsArr[19];
    clientValsMap["/controls/flight/aileron"] = valsArr[20];
    clientValsMap["/controls/flight/elevator"] = valsArr[21];
    clientValsMap["/controls/flight/rudder"] = valsArr[22];
    clientValsMap["/controls/flight/flaps"] = valsArr[23];
    clientValsMap["/controls/engines/engine/throttle"] = valsArr[24];
    clientValsMap["/controls/engines/current-engine/throttle"] = valsArr[25];
    clientValsMap["/controls/switches/master-avionics"] = valsArr[26];
    clientValsMap["/controls/switches/starter"] = valsArr[27];
    clientValsMap["/engines/active-engine/auto-start"] = valsArr[28];
    clientValsMap["/controls/flight/speedbrake"] = valsArr[29];
    clientValsMap["/sim/model/c172p/brake-parking"] = valsArr[30];
    clientValsMap["/controls/engines/engine/primer"] = valsArr[31];
    clientValsMap["/controls/engines/current-engine/mixture"] = valsArr[32];
    clientValsMap["/controls/switches/master-bat"] = valsArr[33];
    clientValsMap["/controls/switches/master-alt"] = valsArr[34];
    clientValsMap["/engines/engine/rpm"] = valsArr[35];
}
/**
 * this function enter the new values by the variables requirement
 */
void updateSymbolsMap() {
    unordered_map<string, Variable *>::iterator it;
    for (it = symbolsMap.begin(); it != symbolsMap.end(); it++) {
        //direction of sim (<-)
        if (it->second->direction == 0) {
            it->second->value = clientValsMap[it->second->sim];
        }
    }
}


//----------ConnectCommand----------
ConnectCommand::ConnectCommand() {
}

ConnectCommand::~ConnectCommand() {
}
/**
 * the function goes to index in arr created by lexer and connects to server in "port" address.
 * @param index
 * @return - int - number of cells to skip in arr created by lexer.
 */
int ConnectCommand::execute(int index) {
    int port = stoi(arr[index + 2]);
    //create socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        //error
        cerr << "Could not create a socket" << endl;
        return -1;
    }

    //create a sockaddr obj to hold address of server
    sockaddr_in address; //in means IP4
    address.sin_family = AF_INET;//IP4
    address.sin_addr.s_addr = inet_addr("127.0.0.1");  //the localhost address
    address.sin_port = htons(port);
    // Requesting a connection with the server on local host
    int is_connect = connect(client_socket, (struct sockaddr *) &address, sizeof(address));
    if (is_connect == -1) {
        cerr << "Could not connect to host server" << endl;
        exit(1);
    } else {
        cout << "Client is now connected to server" << endl;
    }

    thread thread2(setValuesToServer, client_socket);
    thread2.detach();

    return 3;
}

/**
 * the function run an endless loop(till connection is over), witch writing commands to server.
 * @param client_socket
 */
void setValuesToServer(int client_socket) {
    while (1) {
        //if queue not empty - write to server the string in queue
        if (!commandsToServer.empty()) {
            string command = commandsToServer.front();
            commandsToServer.pop();
            char setCommand[command.length() + 1];
            strcpy(setCommand, command.c_str());
            int is_sent = send(client_socket, setCommand, strlen(setCommand), 0);
            if (is_sent == -1) {
                cout << "Error sending message" << endl;
            } else {
                cout << "message sent to server" << endl;
            }
        }
    }
}

//----------DefineVarCommand----------
DefineVarCommand::DefineVarCommand() {
}

DefineVarCommand::~DefineVarCommand() {

}

/**
 * the function goes to index in arr created by lexer and creating variable with follows values in arr.
 * @param index
 * @return - int - number of cells to skip in arr created by lexer.
 */
int DefineVarCommand::execute(int index) {
    Variable *var = new Variable(arr[index + 1], 0);
    //var a = b
    if (arr[index + 2] == "=") {
        Interpreter *inter = new Interpreter();
        setVarsToInterpreter(inter);
        var->direction = -1;
        Expression *exp = inter->interpret(arr[index + 3]);
        var->value = exp->calculate();
        symbolsMap[arr[index + 1]] = var;
        return 4;
        //var a -> sim("add")
    } else {
        if (arr[index + 2] == "->") {
            var->direction = 1;
        } else {
            var->direction = 0;
        }
        var->sim = arr[index + 4];
        symbolsMap[arr[index + 1]] = var;
        return 5;
    }
}

//----------PrintCommand----------
PrintCommand::PrintCommand() {

}

PrintCommand::~PrintCommand() {

}
/**
 * the function goes to index in arr created by lexer and Prints the message/expression.
 * @param index
 * @return - int - number of cells to skip in arr created by lexer.
 */
int PrintCommand::execute(int index) {
    //print string
    if (arr[index + 1].find('"') != string::npos) {
        string temp = arr[index + 1];
        temp.erase(0, 1);
        temp.erase(temp.find('"'), 1);
        cout << temp << endl;
        //print Expression
    } else {
        string exp = arr[index+1];
        exp.erase(remove(exp.begin(), exp.end(), ' '), exp.end());
        Interpreter *inter = new Interpreter();
        setVarsToInterpreter(inter);
        Expression *e = inter->interpret(exp);
        cout << e->calculate() << endl;
    }
    return 2;
}

//----------SleepCommand----------
SleepCommand::SleepCommand() {

}

SleepCommand::~SleepCommand() {

}
/**
 * the function goes to index in arr created by lexer and sleep for follows value/expression in arr
 * @param index
 * @return - int - number of cells to skip in arr created by lexer.
 */
int SleepCommand::execute(int index) {
    Interpreter *inter = new Interpreter();
    setVarsToInterpreter(inter);
    Expression *e = inter->interpret(arr[index + 1]);
    int duration = (int)(e->calculate());
    this_thread::sleep_for(chrono::milliseconds(duration));
    return 2;
}

//----------WhileCommand----------
WhileCommand::WhileCommand() {

}

WhileCommand::~WhileCommand() {

}
/**
 * the function goes to index in arr created by lexer and create while loop with follows command in arr till '}'.
 * @param index
 * @return - int - number of cells to skip in arr created by lexer.
 */
int WhileCommand::execute(int index) {
    string str = arr[index + 1], rightSide, leftSide, oper;
    vector<string> expVector = getExpression(str);
    leftSide = expVector[0];
    oper = expVector[1];
    rightSide = expVector[2];
    expVector.clear();
    int countIndex = 0, k = index + 3, executeSkip;
    bool first = true;
    //Condition in while is false on first iteration - skip all block
    if (!expressionCheck(leftSide, rightSide, oper)) {
        while (arr[k] != "}") {
            k++;
            countIndex++;
        }
    } else {
        while (expressionCheck(leftSide, rightSide, oper)) {
            while (arr[k] != "}") {
                Command *c = commandsMap[arr[k]];
                executeSkip = c->execute(k);
                //executeSkip = commandsMap[arr[k]]->execute(k);
                k += executeSkip;
                if (first) {
                    countIndex += executeSkip;
                }
            }
            first = false;
            k -= countIndex;
        }
    }
    return (countIndex + 1);
}

//----------IfCommand----------
IfCommand::IfCommand() {

}

IfCommand::~IfCommand() {

}
/**
 * the function goes to index in arr created by lexer and creats if condition with follows value in arr till '}'
 * @param index
 * @return - int - number of cells to skip in arr created by lexer.
 */
int IfCommand::execute(int index) {
    string str = arr[index + 1], rightSide, leftSide, oper;
    vector<string> expVector = getExpression(str);
    leftSide = expVector[0];
    oper = expVector[1];
    rightSide = expVector[2];
    expVector.clear();
    int i = index + 3, executeSkip, k = 0;
    if (expressionCheck(leftSide, rightSide, oper)) {
        while (arr[i] != "}") {
            Command *c = commandsMap[arr[i]];
            executeSkip = c->execute(i);
            i += executeSkip;
            k += executeSkip;
        }
    } else {
        while (arr[i] != "}") {
            i++;
            k++;
        }
    }
    return (k + 1);
}

//------SetValueCommand-------
SetValueCommand::SetValueCommand() {

}

SetValueCommand::~SetValueCommand() {

}
/**
 * the function goes to index in arr created by lexer and update variable value in map,
 * as well, update commandsToServer queue- strings in queue are later be wrirting to server.
 * @param index
 * @return - int - number of cells to skip in arr created by lexer.
 */
int SetValueCommand::execute(int index) {
    Interpreter *inter = new Interpreter();
    setVarsToInterpreter(inter);
    string a = arr[index + 3];
    Expression *exp = inter->interpret(a);
    symbolsMap[arr[index + 1]]->value = exp->calculate();
    string command =
            "set " + symbolsMap[arr[index + 1]]->sim + " " + to_string(symbolsMap[arr[index + 1]]->value) + "\r\n";
    commandsToServer.push(command);
    return 4;
}

//--------Functions------
/**
 * function reading file named "filename", and cerating matching  arr of strings.
 * @param filename
 */
vector<string> lexer(string filename) {
    ifstream file;
    file.open(filename);
    string line;
    vector<string> arr;

    if (file.is_open()) {
        //iterate all lines in file
        while (getline(file, line)) {
            //while
            if (line.find("while") != string::npos) {
                line.erase(remove(line.begin(), line.end(), ' '), line.end());
                line.erase(0, line.find("w"));
                arr.push_back("while");
                line.erase(0, 5);
                int start = line.find("{");
                line.erase(start, start);
                arr.push_back(line);
                arr.push_back("{");
                //if
            } else if (line.find("if") != string::npos) {
                line.erase(remove(line.begin(), line.end(), ' '), line.end());
                line.erase(0, line.find("i"));
                arr.push_back("if");
                line.erase(0, 2);
                int start = line.find("{");
                line.erase(start, start);
                arr.push_back(line);
                arr.push_back("{");
                //varibale = value/expresssion
            } else if ((line.find("=") != string::npos) && !(line.find("var") != string::npos) && !(line.find("Print") != string::npos)) {
                arr.push_back("setValue");
                line.erase(remove(line.begin(), line.end(), '\t'), line.end());
                line.erase(remove(line.begin(), line.end(), ' '), line.end());
                string temp = line;
                temp.erase(temp.find("="), temp.length());
                arr.push_back(temp);
                arr.push_back("=");
                line.erase(0, line.find("=") + 1);
                arr.push_back(line);
                // var a = value/expression
            } else if ((line.find("=") != string::npos) && (line.find("var") != string::npos)) {
                line.erase(remove(line.begin(), line.end(), '\t'), line.end());
                line.erase(remove(line.begin(), line.end(), ' '), line.end());
                arr.push_back("var");
                line.erase(0, 3);
                string temp = line;
                int start = temp.find("=");
                temp.erase(start, temp.length());
                arr.push_back(temp);
                arr.push_back("=");
                line.erase(0, start + 1);
                arr.push_back(line);
                //Print
            } else if (line.find("Print") != string::npos) {
                arr.push_back("Print");
                line.erase(0, line.find("(") + 1);
                line.erase(line.find(")"), 1);
                arr.push_back(line);
                //Sleep
            } else if (line.find("Sleep") != string::npos) {
                arr.push_back("Sleep");
                line.erase(0, line.find("(") + 1);
                line.erase(line.find(")"), line.find(")"));
                arr.push_back(line);
            } else {
                istringstream iss(line);
                //split into words;
                for (string word; iss >> word;) {
                    if (word.find("openDataServer") != string::npos) {
                        //command name
                        arr.push_back("openDataServer");
                        word.erase(0, 15);
                        int end = word.find(")");
                        if ( end!= -1) {
                            word.erase(end, end);
                        }
                        //port
                        arr.push_back(word);
                    } else if (word.find("connectControlClient") != string::npos) {
                        //command name
                        arr.push_back("connectControlClient");
                        word.erase(0, 22);
                        string temp = word;
                        int start = temp.find(",");
                        int end = temp.find(")") - 1;
                        temp.erase(start, end);
                        //IP address
                        arr.push_back(temp);
                        start = word.find(",") + 1;
                        word.erase(0, start);
                        end = word.find(")");
                        if (end!=-1) {
                            word.erase(end, end);
                        }

                        //port number
                        arr.push_back(word);

                    } else if (word.find("var") != string::npos) {
                        arr.push_back("var");

                    } else if (word.find("->") != string::npos) {
                        arr.push_back("->");

                    } else if (word.find("<-") != string::npos) {
                        arr.push_back("<-");

                    } else if (word.find("sim") != string::npos) {
                        arr.push_back("sim");
                        word.erase(0, 5);
                        int end = word.find(")");
                        word.erase(end - 1, end);
                        arr.push_back(word);

                    } else {
                        //skip on comments
                        if (word.find("//") != string::npos) {
                            break;
                        }
                        arr.push_back(word);
                    }
                }
            }
        }
        file.close();
    }
    return arr;

}
/**
 * the function goes to index in arr created by lexer and calling execute function for each command.
 */
void parser() {
    int i = 0;
    while (i < arr.size()) {
        //commandMap maps between string from arr into matching Command type.
        Command *c = commandsMap[arr[i]];
        if (c != nullptr) {
            i += c->execute(i);
        } else {
            i++;
        }
    }
}
/**
 * the function initialize commadMap, for all possible strings into matching Command type.
 */
void commandsMapInit() {
    commandsMap["openDataServer"] = new OpenServerCommand();
    commandsMap["connectControlClient"] = new ConnectCommand();
    commandsMap["var"] = new DefineVarCommand();
    commandsMap["while"] = new WhileCommand();
    commandsMap["Print"] = new PrintCommand();
    commandsMap["Sleep"] = new SleepCommand();
    commandsMap["setValue"] = new SetValueCommand();
    commandsMap["if"] = new IfCommand();
}
/**
 * the function evalute if/while conditions and returns true or false.
 * @param leftSide - left expression
 * @param rightSide - right condition
 * @param oper - operation
 */
bool expressionCheck(string leftSide, string rightSide, string oper) {
    Interpreter *inter = new Interpreter();
    setVarsToInterpreter(inter);
    Expression *left = inter->interpret(leftSide);
    Expression *right = inter->interpret(rightSide);


    if (oper == "==") {
        return (left->calculate() == right->calculate());

    } else if (oper == "<=") {
        return (left->calculate() <= right->calculate());

    } else if (oper == ">=") {
        return (left->calculate() >= right->calculate());

    } else if (oper == "!=") {
        return (left->calculate() != right->calculate());

    } else if (oper == "<") {
        return (left->calculate() < right->calculate());
        //(oper == ">")
    } else {
        return (left->calculate() > right->calculate());
    }
}
/**
 * the function is seting pre declared variable to interpereter.
 */
void setVarsToInterpreter(Interpreter *inter) {
    unordered_map<string, Variable *>::iterator iter = symbolsMap.begin();
    while (iter != symbolsMap.end()) {
        stringstream stream;
        stream << fixed << setprecision(3) << iter->second->value;
        string value = stream.str();
        inter->setVariables(iter->first + "=" + value);
        iter++;
    }
}
/**
 * the function splits string in array created by lexer into 3 string - left, opreatin, right.
 * @return vector of 3 strings.
 */
vector<string> getExpression(string str) {
    int i;
    vector<string> expVector;
    for (i = 0; i < str.length(); ++i) {
        if (str[i] == '!' || str[i] == '<' || str[i] == '>' || str[i] == '=') {
            break;
        }
    }
    string temp = str;
    string leftSide = temp.erase(i, temp.length());
    expVector.push_back(leftSide);
    string oper = {str[i]};
    if (str[i + 1] == '=') {
        oper += '=';
        i += 2;
    } else {
        i++;
    }
    expVector.push_back(oper);
    string rightSide = str.erase(0, i);
    expVector.push_back(rightSide);
    return expVector;
}

//**********BinaryOperator**********
BinaryOperator::BinaryOperator(Expression *left_, Expression *right_) {
    this->right = right_;
    this->left = left_;
}

double BinaryOperator::calculate() {
    return 0;
}

BinaryOperator::~BinaryOperator() {
    delete this->left;
    delete this->right;
}

//**********UnaryOperator**********
UnaryOperator::UnaryOperator(Expression *expression_) {
    this->expression = expression_;
}

double UnaryOperator::calculate() {
    return 0;
}

UnaryOperator::~UnaryOperator() {
    delete this->expression;
}


// ****************************Ex1******************************

//**********Variable**********
Variable::Variable(string name_, double value_) {
    this->name = name_;
    this->value = value_;
}

Variable &Variable::operator++() {
    ++this->value;
    return *this;
}

Variable &Variable::operator--() {
    --this->value;
    return *this;
}

Variable &Variable::operator+=(double num) {
    this->value += num;
    return *this;
}

Variable &Variable::operator-=(double num) {
    this->value -= num;
    return *this;
}

Variable &Variable::operator++(int) {
    this->value++;
    return *this;
}

Variable &Variable::operator--(int) {
    this->value--;
    return *this;
}

double Variable::calculate() {
    return this->value;
}

Variable::~Variable() {

}

//**********Plus**********
Plus::Plus(Expression *left_, Expression *right_) : BinaryOperator(left_, right_) {
}

double Plus::calculate() {
    return (this->left->calculate() + this->right->calculate());
}

Plus::~Plus() = default;

//**********Minus**********
Minus::Minus(Expression *left_, Expression *right_) : BinaryOperator(left_, right_) {
}

double Minus::calculate() {
    return (this->left->calculate() - this->right->calculate());
}

Minus::~Minus() =
default;

//**********Mul**********
Mul::Mul(Expression *left_, Expression *right_) : BinaryOperator(left_, right_) {
}

double Mul::calculate() {
    return (this->left->calculate() * this->right->calculate());
}

Mul::~Mul() = default;

//**********Div**********
Div::Div(Expression *left_, Expression *right_) : BinaryOperator(left_, right_) {
    if (right_->calculate() == 0) {
        throw ("division by zero");
    }
}

double Div::calculate() {
    return (this->left->calculate() / this->right->calculate());
}

Div::~Div() = default;

//**********UPlus**********
UPlus::UPlus(Expression *expression_) : UnaryOperator(expression_) {}

double UPlus::calculate() {
    return (this->expression->calculate());
}

UPlus::~UPlus() = default;

//**********UMinus**********
UMinus::UMinus(Expression *expression_) : UnaryOperator(expression_) {}

double UMinus::calculate() {
    return (-(this->expression->calculate()));
}

UMinus::~UMinus() = default;

//**********Value**********
Value::Value(double value_) : value(value_) {}

double Value::calculate() {
    return this->value;
}

Value::~Value() {
}

//**********Interpreter**********
Expression *Interpreter::interpret(string string) {
    stack<Expression *> expStack;
    queue<char *> queue;
    stack<char *> stack;
    if (!inputValid(string)) {
        throw ("illegal math expression");
    }
    for (int i = 0; string[i] != '\0'; i++) {
        int j = i + 1, k = 0;
        if (isNumber(string[i])) {
            char *assist = (char *) malloc(sizeof(char) * 4);
            assist[k] = string[i];
            while (isNumber(string[j])) {
                k++;
                assist[k] = string[j];
                i = j;
                j++;
            }
            assist[k + 1] = '\0';
            queue.push(assist);
        }

        if (isLetter(string[i])) {
            char *assist = (char *) malloc(sizeof(char) * 4);
            assist[k] = string[i];
            while (isLetter(string[j]) || isNumber(string[j]) || string[j] == '_') {
                k++;
                assist[k] = string[j];
                i = j;
                j++;
            }
            assist[k + 1] = '\0';
            queue.push(assist);
        }

        if (isOperation(string[i])) {
            char *assist = (char *) malloc(sizeof(char));
            *assist = string[i];
            if ((*assist == '+') && ((string[i - 1] == '(') || (i == 0))) {
                *assist = '$';
            }
            if ((*assist == '-') && ((string[i - 1] == '(') || (i == 0))) {
                *assist = '&';
            }

            if (stack.empty()) {
                stack.push(assist);
            } else if (priority(*stack.top()) < priority(*assist) && (*assist != ')')) {
                stack.push(assist);
            } else if (*assist == '(') {
                stack.push(assist);
            } else if (*assist == ')') {
                while (*stack.top() != '(') {
                    queue.push(stack.top());
                    stack.pop();
                }
                free(assist);
                free(stack.top());
                //remove ')' character.
                stack.pop();
            } else if (priority(*stack.top()) >= priority(*assist)) {
                while (!(stack.empty()) && (priority(*stack.top()) >= priority(*assist))) {
                    queue.push(stack.top());
                    stack.pop();
                }
                stack.push(assist);
            }
        }
    }

    //Move remaining objects in stack to queue.
    while (!stack.empty()) {
        queue.push(stack.top());
        stack.pop();
    }

    //Evaluate postfix form.
    while (!queue.empty()) {
        if (isNumber(*queue.front())) {
            Expression *e = new Value(convertToDouble(queue.front()));
            expStack.push(e);
        } else if (isLetter(*queue.front())) {
            Expression *e = new Variable(convertToString(queue.front()), map1[convertToString(queue.front())]);
            expStack.push(e);

        } else if (isOperation(*queue.front())) {
            Expression *left = nullptr, *right = nullptr, *single = nullptr;
            //Unary operations
            if (*queue.front() == '$') {
                single = new UPlus(expStack.top());
                expStack.pop();
                expStack.push(single);
            } else if (*queue.front() == '&') {
                single = new UMinus(expStack.top());
                expStack.pop();
                expStack.push(single);
                //binary operation
            } else {
                right = expStack.top();
                expStack.pop();
                left = expStack.top();
                expStack.pop();
                Expression *e;
                if (*queue.front() == '+') {
                    e = new Plus(left, right);
                }
                if (*queue.front() == '-') {
                    e = new Minus(left, right);
                }
                if (*queue.front() == '*') {
                    e = new Mul(left, right);
                }
                if (*queue.front() == '/') {
                    e = new Div(left, right);
                }
                expStack.push(e);
            }
        }
        //free(queue.front());
        queue.pop();


    }
    Expression *e = expStack.top();
    expStack.pop();
    return e;
}

void Interpreter::setVariables(string string) {
    if (!assignValid(string)) {
        throw ("illegal variable assignment!");
    }

    unsigned int index, i = 0;
    char *value = nullptr, *name = nullptr;
    bool flag = true;
    queue<char *> queue;
    while (flag) {
        if (isLetter(string[i])) {
            index = string.find("=");
            name = (char *) malloc(sizeof(char) * (index + 1));
            int k = 0;
            for (unsigned int j = i; j < index; j++) {
                name[k] = string[j];
                k++;
            }
            name[k] = '\0';
            queue.push(name);
        } else if (isNumber(string[i]) || string[i] == '-') {
            index = string.find(";");
            if (index > string.length()) {
                index = string.length();
                flag = false;
            }
            value = (char *) malloc(sizeof(char) * (index + 1));
            int k = 0;
            for (unsigned int j = i; j < index; j++) {
                value[k] = string[j];
                k++;
            }
            value[k] = '\0';
            queue.push(value);

        }
        string.erase(0, index + 1);
    }

    while (!queue.empty()) {
        char *name_ = queue.front();
        queue.pop();
        char *value_ = queue.front();
        queue.pop();
        map1[convertToString(name_)] = convertToDouble(value_);
    }
}

bool Interpreter::inputValid(string input) {
    //first char check
    if ((!isNumber(input[0]) && !isLetter(input[0]) && (input[0] != '(') && input[0] != '-') || input[0] == '.') {
        return false;
    }
    //last char check
    char last = input[input.length() - 1];
    if ((isOperation(last) || last == '(' || last == '.') && last != ')') {
        return false;
    }

    if (!parenthesisCheck(input)) {
        return false;
    }
    for (unsigned int i = 0; i < input.length(); ++i) {
        if (!isOperation(input[i]) && !isLetter(input[i]) && !isNumber(input[i]) && input[i] != ')' &&
            input[i] != '(' && input[i] != '&' && input[i] != '&') {
            return false;
        }

        // two operators in a row '5++2'
        if ((input[i] == '+' && input[i + 1] == '+') || (input[i] == '-' && input[i + 1] == '-')
            || (input[i] == '*' && input[i + 1] == '*') || (input[i] == '*' && input[i + 1] == '*')) {
            return false;
        }
        //'(' after '-' and before variable "-(x)",.
        /**
        if (input[i] == '-' && i + 1 <= input.length() - 1) {
            if (isLetter(input[i + 1])) {
                return false;
            }
        }
         **/
        // (.5+3) , (5..3+1), (5+3.)
        if (input[i] == '.' && (i + 1 <= input.length() - 1)) {
            if ((!isNumber(input[i + 1])) || (input[i + 1] == '.') || (!isNumber(input[i - 1]))) {
                return false;
            }

        }

        if (input[i] == '(') {
            int j = i - 1;
            if ((j >= 0) && (!isOperation(input[j]))) {
                return false;
            }
        }

        if (isLetter(input[i])) {
            int j = i + 1;
            while ((isLetter(input[j]) || input[j] == '_' || isNumber(input[j])) && input[j] != '.') {
                j++;
            }
            string assist;
            for (int m = i; m < j; m++) {
                assist += input[m];
            }
            if (map1.find(assist) == map1.end()) {
                return false;
            } else {
                //skip entire variable name
                i = j - 1;
            }
        }
    }
    return true;
}

bool Interpreter::assignValid(string input) {
    if (!isLetter(input[0])) {
        return false;
    }

    bool stopFlag = false;
    while (!stopFlag) {
        unsigned int equalsIndex = input.find('='), delimiterIndex = input.find(";");
        if (delimiterIndex >= input.length() - 1) {
            if (equalsIndex >= input.length() - 1) {
                return false;
            }
            stopFlag = true;
        }

        string varName;
        for (unsigned int i = 0; i < equalsIndex; i++) {
            varName += input[i];
        }
        if (!varNameValid(varName)) {
            return false;
        }
        string value;
        int stop = delimiterIndex;
        if (delimiterIndex >= input.length()) {
            stop = input.length();
        }
        for (int i = equalsIndex + 1; i < stop; i++) {
            value += input[i];
        }
        if (!valueValid((value))) {
            return false;
        }
        input.erase(0, (delimiterIndex + 1));
    }
    return true;
}

bool Interpreter::valueValid(string value) {
    //cant be '.' sign in first and last index in number (.5, 22.)
    if (value[0] == '.' || value[value.length() - 1] == '.') {
        return false;
    }

    for (unsigned int i = 0; i < value.length(); ++i) {
        if ((!isNumber(value[i])) && (value[i] != '.')) {
            if (i == 0 && value[i] == '-') {
                int a = 1;
            } else {
                return false;

            }


        }
    }
    return true;
}

bool Interpreter::varNameValid(string name) {
    if (!isLetter(name[0])) {
        return false;
    }

    for (unsigned int i = 0; i < name.length(); ++i) {
        if ((!isLetter(name[i]) && (name[i] != '_') && (!isNumber(name[i]))) || (name[i]) == '.') {
            return false;
        }
    }
    return true;
}

bool Interpreter::parenthesisCheck(string input) {
    bool flag = true;
    while (flag) {
        unsigned int open = input.find('(');
        unsigned int close = input.find(')');
        int open1 = input.find('(');
        int close1 = close = input.find(')');
        if ((close1 < open1) || (close1 > 0 && open1 < 0)) {
            return false;
        }

        if (((close <= input.length() - 1) && (close1 >= 0)) && ((open <= input.length() - 1) && (open1 >= 0))) {
            input[open] = '@';
            input[close] = '@';
        } else {
            flag = false;
        }
    }
    return true;
}

double Interpreter::convertToDouble(char *string) {
    int i = 0, pointLocation = 0, length = 0, pow1;
    while (string[i] != '\0') {
        if (string[i] == '.') {
            pointLocation = i;
        }
        length++;
        i++;
    }

    if (pointLocation != 0) {
        pow1 = pointLocation - 1;
    } else {
        pow1 = length - 1;
    }

    double sum = 0;
    for (int j = 0; j < length; j++) {
        if (string[j] != '.') {
            sum += (((int) string[j] - 48) * (pow(10, pow1)));
            pow1--;
        }
    }


    return sum;
}

string Interpreter::convertToString(char *arr) {
    string s = "";
    int i = 0;
    while (arr[i] != '\0') {
        s = s + arr[i];
        i++;
    }
    return s;
}

int Interpreter::priority(char operation) {
    switch (operation) {
        case '+':
            return 1;
        case '-':
            return 1;
        case '&' :
            return 1;
        case '$' :
            return 1;
        case '*':
            return 2;
        case '/':
            return 2;
        case '(':
            return 0;
        case ')':
            return 100;
        default:
            return -1;
    }
}

bool Interpreter::isNumber(char c) {
    if ((c >= '0' && c <= '9') || c == '.') {
        return true;
    }
    return false;
}

bool Interpreter::isLetter(char c) {
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
        return true;
    }
    return false;
}

bool Interpreter::isOperation(char c) {
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == '&' || c == '$') {
        return true;
    }
    return false;
}

Interpreter::~Interpreter() {

}

//Main
int main() {
    string filename = "fly.txt";
    arr = lexer(filename);
    commandsMapInit();
    parser();
    return 0;
}