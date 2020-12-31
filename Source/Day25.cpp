#include "Days.h"
#include <algorithm>

const int PUBLIC_KEY_CARD = 16915772;
const int PUBLIC_KEY_DOOR = 18447943;
const int PUBLIC_SUBJECT_NUMBER = 7;
const int MAGIC_DIVISOR = 20201227;

unsigned long long TransformValue(unsigned long long value, int subjectNumber)
{
	value *= subjectNumber;
	value %= MAGIC_DIVISOR;
	return value;
}

template <>
void Run<Day25>(Part part, istream& is, ostream& os)
{	
	if(part == Part01)
	{
		// Loop until values matching each public key are found
		size_t i = 1, loopSizeCard = 0, loopSizeDoor = 0;
		unsigned long long value = 1;
		while (loopSizeCard == 0 || loopSizeDoor == 0)
		{
			value = TransformValue(value, PUBLIC_SUBJECT_NUMBER);
			if (value == PUBLIC_KEY_CARD)
			{
				loopSizeCard = i;
				os << "Card Secret Loop Size: " << loopSizeCard << endl;
			}
			if (value == PUBLIC_KEY_DOOR)
			{
				loopSizeDoor = i;
				os << "Door Secret Loop Size: " << loopSizeDoor << endl;
			}
			++i;
		}	

		// Use the found secret loops to transform the public keys into encryption keys
		unsigned long long encryptionKeyCard = 1, encryptionKeyDoor = 1;
		for (i = 1; i <= max(loopSizeCard, loopSizeDoor); ++i)
		{
			if (i <= loopSizeDoor)
			{
				encryptionKeyCard = TransformValue(encryptionKeyCard, PUBLIC_KEY_CARD);
			}
			if (i <= loopSizeCard)
			{
				encryptionKeyDoor = TransformValue(encryptionKeyDoor, PUBLIC_KEY_DOOR);
			}
		}
		if (encryptionKeyCard == encryptionKeyDoor)
		{
			os << "Encryption Key: " << encryptionKeyCard << endl;
		}
		else
		{
			os << "Encryption Keys don't match!" << endl;
			os << "Card Encryption Key: " << encryptionKeyCard << endl;
			os << "Door Encryption Key: " << encryptionKeyDoor << endl;
		}
	}
}