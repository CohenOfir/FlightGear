#ifndef ex1_H
#define ex1_H

#include <iostream>
#include <stack>
#include <queue>
#include <math.h>
#include <bits/stdc++.h>
#include "Expression.h"

using namespace std;

class BinaryOperator : public Expression {
protected:
    Expression *left;
    Expression *right;

public:
    BinaryOperator(Expression *left, Expression *right);

    double calculate();

    ~BinaryOperator();
};

class UnaryOperator : public Expression {
protected:
    Expression *expression;
public:
    UnaryOperator(Expression *expression);

    double calculate();

    ~UnaryOperator();
};

class Variable : public Expression {
public:

    string name;
    double value;
    string sim;
    int direction;

    Variable(string name, double value);

    Variable &operator++();

    Variable &operator--();

    Variable &operator+=(double num);

    Variable &operator-=(double num);

    Variable &operator++(int);

    Variable &operator--(int);

    double calculate();

    ~Variable();
};

class Plus : public BinaryOperator {
public:
    Plus(Expression *left, Expression *right);

    double calculate();

    ~Plus();
};

class Minus : public BinaryOperator {
public:
    Minus(Expression *left, Expression *right);

    double calculate();

    ~Minus();
};

class Mul : public BinaryOperator {
public:
    Mul(Expression *left, Expression *right);

    double calculate();

    ~Mul();
};

class Div : public BinaryOperator {
public:
    Div(Expression *left, Expression *right);

    double calculate();

    ~Div();
};

class Value : public Expression {
    const double value;

public:
    Value(double value);

    double calculate();

    ~Value();
};

class UPlus : public UnaryOperator {

public:
    UPlus(Expression *expression);

    double calculate();

    ~ UPlus();
};

class UMinus : public UnaryOperator {

public:
    UMinus(Expression *expression);

    double calculate();

    ~ UMinus();
};

class Interpreter {
    map<string, double> map1;
public:
    bool varNameValid(string name);

    bool valueValid(string value);

    bool assignValid(string input);

    bool inputValid(string input);

    double convertToDouble(char *string);

    string convertToString(char *arr);

    int priority(char operation);

    bool isNumber(char c);

    bool isLetter(char c);

    bool isOperation(char c);

    bool parenthesisCheck(string input);

    Expression *interpret(string string);

    void setVariables(string string);

    ~Interpreter();
};

#endif //ex1_H
