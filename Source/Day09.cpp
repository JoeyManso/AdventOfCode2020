#include "Days.h"
#include <vector>

size_t PREAMBLE_SIZE_MAX = 25;

/** Returns the sum of all numbers in the given vector */
long long CalculateSum(const vector<long long>& nums)
{
	long long sum = 0;
	for (auto num : nums)
	{
		sum += num;
	}
	return sum;
}

/** Returns true if the given number pool can combine to reach the target sum with 'n' values */
bool CanCalulateSum(const vector<long long>& numberPool, vector<long long>& canidateNums, long long targetSum, size_t idxStart, size_t n)
{
	if (canidateNums.size() == n)
	{
		// Reached candidates target, check if the sum matches the goal
		return CalculateSum(canidateNums) == targetSum;
	}
	else
	{
		// Else, recursively add candidates
		for (size_t i = idxStart; i < numberPool.size(); ++i)
		{
			canidateNums.push_back(numberPool[i]);
			if (CanCalulateSum(numberPool, canidateNums,targetSum, i + 1, n))
			{
				return true;
			}
			else
			{
				canidateNums.pop_back();
			}
		}
	}
	return false;
}

template <>
void Run<Day09>(Part part, istream& is, ostream& os)
{
	// Collect numbers for processing
	vector<long long> nums;
	string arg;
	while (getline(is, arg))
	{
		long long num = stoll(arg);
		nums.push_back(num);
	}
	
	// Determine target value, where no two numbers within the preamble range can match the sum
	size_t idxTarget = 0;
	vector<long long> preambleList = {};
	for (size_t i = 0; i < nums.size(); ++i)
	{
		if (preambleList.size() > PREAMBLE_SIZE_MAX)
		{
			preambleList.erase(preambleList.begin());
		}
		if (preambleList.size() == PREAMBLE_SIZE_MAX)
		{
			vector<long long> candidateNumbers = {};
			if (!CanCalulateSum(preambleList, candidateNumbers, nums[i], 0, 2))
			{
				idxTarget = i;
				break;
			}
		}
		preambleList.push_back(nums[i]);
	}

	// Search for continuous set of numbers that sum to the target
	vector<long long> candidateNums = {};
	for(size_t i = 0; i < idxTarget; ++i)
	{
		long long sum = CalculateSum(candidateNums);
		while (sum > nums[idxTarget])
		{
			candidateNums.erase(candidateNums.begin());
			sum = CalculateSum(candidateNums);
		}
		if (sum == nums[idxTarget])
		{
			break;
		}
		candidateNums.push_back(nums[i]);
	}

	// Grab min/max of the candidates
	long long candidateMin = LLONG_MAX;
	long long candidateMax = 0;
	for (auto candidateNum : candidateNums)
	{
		candidateMin = min(candidateNum, candidateMin);
		candidateMax = max(candidateNum, candidateMax);
	}

	if(part == Part01)
	{
		os << "First invalid number: " << nums[idxTarget] << endl;
	}
	else if(part == Part02)
	{
		os << "Encryption weakness: " << candidateMin + candidateMax << endl;
	}
}