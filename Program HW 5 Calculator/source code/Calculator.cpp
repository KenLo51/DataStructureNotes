#include "Calculator.h"


void Calculator::printTokens(Queue<Token>& q) {
	QueueNode<Token>* p = q.Data();
	while (p != NULL) {
		if (p->data.type == TOKEN_OPERATOR || p->data.type == TOKEN_BRACKET) {
			printf(" %c ", p->data.data.op.op);
		}
		if (p->data.type == TOKEN_VALUE) {
			printf(" %7.2f ", p->data.data.val);
		}
		p = p->next;
	}
	printf("\n");
}

unsigned int Calculator::OperatorPrecedence(const char op) {
	switch (op)	{
	case OPERATOR_NEGATIVE:
	case OPERATOR_POSITIVE :
		return 1;
	case OPERATOR_POWER:
		return 2;
	case OPERATOR_MUL:
	case OPERATOR_DIV:
	case OPERATOR_MOD:
		return 3;
	case OPERATOR_ADD:
	case OPERATOR_SUB:
		return 4;
	default:
		return 0;
	}
	return 0;
}

void Calculator::Tokenize(const char* formulaStr, Queue<Token>& infixQ) {
	std::string formula;

	// read valid charactors
	for (unsigned int i = 0; formulaStr[i] != '\0'; i++) {
		// skip unused charactors
		if (std::string(" \t=#").find(formulaStr[i]) != std::string::npos) continue;
		formula.append(1, formulaStr[i]);
	}

	// Tokenize
	while (formula.size() > 0) {
		// value
		if (formula[0] >= '0' && formula[0] <= '9') {
			size_t idx = 0;
			// convert datatype from string to double and save operand token
			Token newToken(std::stof(formula, &idx));
			infixQ.Push(newToken, -1);
			// update unprocessed string
			formula = formula.substr(idx);
			continue;
		}
		// operator
		else {
			char op = OPERATOR_UNKNOW;
			char opChar = formula[0];
			if (opChar == '(') op = OPERATOR_BRACKETBEG;
			if (opChar == ')') op = OPERATOR_BRACKETEND;
			if (infixQ.isEmpty()) {
				// is unary operation 
				if (opChar == '-') op = OPERATOR_NEGATIVE;
				if (opChar == '+') op = OPERATOR_POSITIVE;
			}
			else if (infixQ[-1].type == TOKEN_OPERATOR) {
				// is unary operation 
				if (opChar == '-') op = OPERATOR_NEGATIVE;
				if (opChar == '+') op = OPERATOR_POSITIVE;
			}
			else {
				// is binary operation 
				if (opChar == '+') op = OPERATOR_ADD;
				if (opChar == '-') op = OPERATOR_SUB;
				if (opChar == '*') op = OPERATOR_MUL;
				if (opChar == '/') op = OPERATOR_DIV;
				if (opChar == '%') op = OPERATOR_MOD;
				if (opChar == '^') op = OPERATOR_POWER;
			}
			// add to infix
			//printf("op %c => %c\n", opChar, op);
			infixQ.Push(Token(op), -1);
			formula = formula.substr(1);
			continue;
		}
	}

	// add the ending token
	infixQ.Push(Token(OPERATOR_END), -1);
}
void Calculator::Postfix(Queue<Token>& infixQ, Queue<Token>& postfixQ) {
	Queue<Token> operatorStack;
	QueueNode<Token>* infixNodePtr = infixQ.Data();

	while (infixNodePtr != NULL) {
		//printTokens(postfixQ);
		//printTokens(operatorStack);
		//printf("\n");

		Token& token = infixNodePtr->data;
		//
		if (token.type == TOKEN_VALUE) {
			postfixQ.Push(token, -1);
			infixNodePtr = infixNodePtr->next;
			continue;
		}
		//
		if (token.type == TOKEN_OPERATOR || token.type == TOKEN_BRACKET) {
			// brackets
			char op = token.data.op.op;
			if (op == OPERATOR_BRACKETBEG) {
				operatorStack.Push(token, -1);
				infixNodePtr = infixNodePtr->next;
				continue;
			}
			if (op == OPERATOR_BRACKETEND || op == OPERATOR_END) {
				Token popitem;
				do {
					if (operatorStack.isEmpty()) break;
					popitem = operatorStack.Pop(-1);
					//printf("pop item %c stzckSize = %u\n", popitem.data.op.op, operatorStack.Size());
					if (popitem.data.op.op != OPERATOR_BRACKETBEG) postfixQ.Push(popitem, -1);
				} while (!(popitem.data.op.op == OPERATOR_BRACKETBEG || operatorStack.isEmpty()));
				infixNodePtr = infixNodePtr->next;
				continue;
			}


			// operator
			unsigned int newPrecedence = OperatorPrecedence(token.data.op.op);
			// pop item from stack if needed
			while (!operatorStack.isEmpty()) {
				// 
				if (operatorStack[-1].data.op.op == OPERATOR_BRACKETBEG) {
					break;
				}
				unsigned int stackPrecedence = 0;
				stackPrecedence = OperatorPrecedence(operatorStack[-1].data.op.op);
				// check the priority between two operator
				if (newPrecedence >= stackPrecedence) postfixQ.Push(operatorStack.Pop(-1), -1);
				else break;
			}
			// push new operator
			operatorStack.Push(token, -1);
			infixNodePtr = infixNodePtr->next;
			continue;
		}
	}
}
double Calculator::Evaluate(Queue<Token>& postfixQ) {
	Queue<Token> evaluateStack;

	QueueNode<Token>* postfixNodePtr = postfixQ.Data();

	while (postfixNodePtr != NULL) {
		if (postfixNodePtr->data.type == TOKEN_VALUE) {
			evaluateStack.Push(postfixNodePtr->data, -1);
		}
		if (postfixNodePtr->data.type == TOKEN_OPERATOR) {
			double operand2 = 0.0f, operand1 = 0.0f;
			double result = 0.0f;
			if (postfixNodePtr->data.data.op.nOperand > evaluateStack.Size()) {
				std::cerr << "Invalid Expression" << std::endl;
				throw;
			}
			if (postfixNodePtr->data.data.op.nOperand == 0) {
				std::cerr << "Evaluate error : " << postfixNodePtr->data.data.op.op << std::endl;
				throw;
			}
			else if (postfixNodePtr->data.data.op.nOperand == 1) {
				operand2 = evaluateStack.Pop().data.val;
				result = postfixNodePtr->data.data.op.Calculate(operand2, 0.0f);
			}
			else{
				operand2 = evaluateStack.Pop().data.val;
				operand1 = evaluateStack.Pop().data.val;
				result = postfixNodePtr->data.data.op.Calculate(operand1, operand2);
			}
			evaluateStack.Push(Token(result), -1);
			// printf("op %c : %lf %lf %lf\n", postfixNodePtr->data.data.op.op, operand1, operand2, result);
		}

		postfixNodePtr = postfixNodePtr->next;
	}

	return evaluateStack[0].data.val;
}
double Calculator::Calculate(const char* formula) {
	Queue<Token> infixQ, postfixQ;
	double result;

	Tokenize(formula, infixQ);
	std::cout << "TOKENIZE: ";
	printTokens(infixQ);
	Postfix(infixQ, postfixQ);
	std::cout << "POSTFIX : ";
	printTokens(postfixQ);
	result = Evaluate(postfixQ);

	return result;
}