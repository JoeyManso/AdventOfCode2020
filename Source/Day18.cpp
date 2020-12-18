#include "Days.h"
#include "AdventMath.h"
#include <stack>

struct ExpressionNode
{
	ExpressionNode(char inValue)
		: value(inValue), nodeLeft(nullptr), nodeRight(nullptr)
	{
	}

	~ExpressionNode()
	{
		if (nodeLeft)
		{
			delete nodeLeft;
		}
		if (nodeRight)
		{
			delete nodeRight;
		}
	}

	long long Evaluate()
	{
		long long evaluation = 0;
		
		// No children, should be a number
		if (!(nodeLeft || nodeRight))
		{
			evaluation = ToInt(value);
		}
		else
		{
			// Evaluate child values
			long long valueLeft = nodeLeft ? nodeLeft->Evaluate() : 0;
			long long valueRight = nodeRight ? nodeRight->Evaluate() : 0;

			// Return combination of values
			switch (value)
			{
				case '+':
					evaluation = valueLeft + valueRight;
					break;
				case '*':
					evaluation = valueLeft * valueRight;
					break;
			}
		}
		return evaluation;		
	}
	
	char value;
	ExpressionNode* nodeLeft;
	ExpressionNode* nodeRight;
};

ExpressionNode* ConstructExpressionTree(const string& infixExpression, Part part)
{
	static int operatorPriority[256];
	operatorPriority['*'] = 1;
	operatorPriority['+'] = 2;
	operatorPriority['('] = 3;
	stack<char> operatorStack;
	stack<ExpressionNode*> expressionStack;
	
	for (char c : infixExpression)
	{
		if (c != ' ')
		{
			if (isdigit(c))
			{
				// Add numbers as childless node leaves to the expression stack
				expressionStack.push(new ExpressionNode(c));
			}
			else
			{
				if (c == '(')
				{
					// Open parens should be added to the operator stack with no further processing
					operatorStack.push(c);
				}
				else
				{
					// Handle each operator (with priority) until all have been processed or a starting paren is found
					while (!operatorStack.empty() && operatorStack.top() != '(' && (part == Part01 || operatorPriority[operatorStack.top()] >= operatorPriority[c]))
					{
						// Create a node with the most recently added operator then pop it from the stack
						ExpressionNode* operatorNode = new ExpressionNode(operatorStack.top());
						operatorStack.pop();

						// Pop the top two current expressions, assigning them to our new node
						operatorNode->nodeRight = expressionStack.top();
						expressionStack.pop();
						operatorNode->nodeLeft = expressionStack.top();
						expressionStack.pop();

						// Add the completed operator node to our expression stack
						expressionStack.push(operatorNode);
					}

					if (c == ')')
					{
						// Hitting the end paren means we've processed the last known paren pair
						operatorStack.pop();
					}
					else
					{
						// Add all other operators to the operator stack
						operatorStack.push(c);
					}
				}
			}
		}
	}

	// Resolve lingering operations
	while (!operatorStack.empty())
	{
		// Create a node with the most recently added operator then pop it from the stack
		ExpressionNode* operatorNode = new ExpressionNode(operatorStack.top());
		operatorStack.pop();

		// Pop the top two current expressions, assigning them to our new node
		operatorNode->nodeRight = expressionStack.top();
		expressionStack.pop();
		operatorNode->nodeLeft = expressionStack.top();
		expressionStack.pop();

		// Add the completed operator node to our expression stack
		expressionStack.push(operatorNode);
	}
	ExpressionNode* expressionTree = expressionStack.top();
	return expressionTree;
}

template <>
void Run<Day18>(Part part, istream& is, ostream& os)
{	
	long long evaluationSum = 0;
	string arg;
	int numLines = 0;
	while (getline(is, arg))
	{
		ExpressionNode* expressionTree = ConstructExpressionTree(arg, part);
		long long expressionTreeEvaluation = expressionTree->Evaluate();
		evaluationSum += expressionTreeEvaluation;
		//os << arg << " == " << expressionTreeEvaluation << endl;
		delete expressionTree;
	}
	
	os << "Sum of all evaluations: " << evaluationSum << endl;
}