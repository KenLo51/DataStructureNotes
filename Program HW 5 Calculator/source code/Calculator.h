#ifndef CALCULATOR_H_
#define CALCULATOR_H_

#include <stdio.h>
#include <math.h>

#include "queue.h"
#include "string"

#define TOKEN_UNKNOW 1
#define TOKEN_VALUE 2
#define TOKEN_OPERATOR 3
#define TOKEN_BRACKET 4

#define OPERATOR_UNKNOW 'u'
#define OPERATOR_ADD '+'
#define OPERATOR_SUB '-'
#define OPERATOR_MUL '*'
#define OPERATOR_DIV '/'
#define OPERATOR_MOD '%'
#define OPERATOR_POSITIVE 'p'
#define OPERATOR_NEGATIVE 'n'
#define OPERATOR_END 'e'
#define OPERATOR_BRACKETBEG '('
#define OPERATOR_BRACKETEND ')'
#define OPERATOR_POWER '^'

class Calculator {
	// define datatype
public:
	// data of operator token
	struct TokenOperator {
		// operator
		char op;
		// num of nOperand
		unsigned char nOperand;
		// operator function
		double (*Calculate)(double, double);

		// constructor
		struct TokenOperator() : op(OPERATOR_UNKNOW), nOperand(0), Calculate(NULL) {};
		struct TokenOperator(char op){
			TokenOperator::op = op;
			switch (op)	{
			case OPERATOR_ADD:
				Calculate = Calculator::OperatorFnAdd;
				nOperand = 2;
				break;
			case OPERATOR_SUB:
				Calculate = Calculator::OperatorFnSub;
				nOperand = 2;
				break;
			case OPERATOR_MUL:
				Calculate = Calculator::OperatorFnMul;
				nOperand = 2;
				break;
			case OPERATOR_DIV:
				Calculate = Calculator::OperatorFnDiv;
				nOperand = 2;
				break;
			case OPERATOR_MOD:
				Calculate = Calculator::OperatorFnMod;
				nOperand = 2;
				break;
			case OPERATOR_NEGATIVE:
				Calculate = Calculator::OperatorFnNegative;
				nOperand = 1;
				break;
			case OPERATOR_POSITIVE:
				Calculate = Calculator::OperatorFnPositive;
				nOperand = 1;
				break;
			case OPERATOR_POWER:
				Calculate = Calculator::OperatorFnPower;
				nOperand = 2;
				break;
			case OPERATOR_UNKNOW:
			case OPERATOR_BRACKETBEG:
			case OPERATOR_BRACKETEND:
			default:
				Calculate = Calculator::OperatorFnUnknow;
				nOperand = 0;
			}
		}
	};
	union TokenData{
		double val;
		TokenOperator op;

		union TokenData() : val(0.0f) {};
		union TokenData(double val) : val(val) {};
		union TokenData(char op) : op(op) {};
	}data;
	struct Token {
		unsigned char type;
		TokenData data;

		struct Token() : type(TOKEN_UNKNOW) {};
		struct Token(double val) : type(TOKEN_VALUE), data(val) {};
		struct Token(char op) : data(op) {
			if (op == OPERATOR_BRACKETBEG || op == OPERATOR_BRACKETEND) type = TOKEN_BRACKET;
			else type = TOKEN_OPERATOR;
		};
	};

private:
	// define the process of each Operator
	static double OperatorFnUnknow(double a, double b) {
		return 0.0f;
	}
	static double OperatorFnAdd(double a, double b) {
		return a + b;
	}
	static double OperatorFnSub(double a, double b) {
		return a - b;
	}
	static double OperatorFnMul(double a, double b) {
		return a * b;
	}
	static double OperatorFnDiv(double a, double b) {
		return a / b;
	}
	static double OperatorFnMod(double a, double b) {
		return a - ((int)(a / b) * b);
	}
	static double OperatorFnPositive(double a, double b) {
		return a;
	}
	static double OperatorFnNegative(double a, double b) {
		return -a;
	}
	static double OperatorFnPower(double a, double b) {
		return pow(a, b);
	}

private:
	static void printTokens(Queue<Token>&);

	// get the priority of operator
	static unsigned int OperatorPrecedence(const char);

	static void Tokenize(const char*, Queue<Token>&);
	static void Postfix(Queue<Token>&, Queue<Token>&);
	static double Evaluate(Queue<Token>&);
public:
	static double Calculate(const char*);
};

#endif