#include "Days.h"
#include <regex>
#include <set>
#include <vector>

const regex RX_OP = regex(R"((acc|jmp|nop) ([+-]\d+))");

struct Operation
{
	Operation(string type, int amount)
		: opType(type)
		, opAmount(amount)
	{
	}

	void FlipOpType()
	{
		if (opType == "jmp")
		{
			opType = "nop";
		}
		else if (opType == "nop")
		{
			opType = "jmp";
		}
	}
	
	string opType;
	int opAmount;
};

int ExecuteOperations(const vector<Operation>& operations, bool& operationsLooped)
{
	set<const Operation*> executedOperations = {};
	operationsLooped = false;
	int accValue = 0;
	size_t i = 0;
	while (i < operations.size())
	{
		const Operation& operation = operations[i];
		if (executedOperations.find(&operation) != executedOperations.end())
		{
			operationsLooped = true;
			break;
		}
		else
		{
			// Increment accumulator
			if (operation.opType == "acc")
			{
				accValue += operation.opAmount;
				++i;
			}
			// Skip operations
			else if (operation.opType == "jmp")
			{
				i += operation.opAmount;
			}
			// Do nothing
			else if (operation.opType == "nop")
			{
				++i;
			}
			executedOperations.emplace(&operation);
		}
	}
	return accValue;
}

template <>
void Run<Day08>(Part part, istream& is, ostream& os)
{
	// Read all the operations to easily jump between them
	string arg;
	vector<Operation> operations = {};
	while (getline(is, arg))
	{
		smatch matchOp;
		regex_search(arg, matchOp, RX_OP);
		operations.push_back({matchOp.str(1), stoi(matchOp.str(2))});
	}
	
	int accValue = 0;
	bool operationsLooped = false;
	if (part == Part01)
	{
		// Grab accumulator result regardless of looping
		accValue = ExecuteOperations(operations, operationsLooped);
	}
	else if (part == Part02)
	{
		// Toggle each operation until a sequence completes without looping
		for (Operation& operation : operations)
		{
			operation.FlipOpType();
			accValue = ExecuteOperations(operations, operationsLooped);
			operation.FlipOpType();
			if(!operationsLooped)
				break;
		}
	}
	os << "Calculated Accumulator Value: " << accValue << endl;
}