#ifndef DAYS_H
#define DAYS_H

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

enum Day 
{
    Day01,
    Day02,
    Day03,
    Day04,
    Day05,
    Day06,
    Day07,
    Day08,
    Day09,
    Day10,
    Day11,
    Day12,
    Day13,
    Day14,
    Day15,
    Day16,
    Day17,
    Day18,
    Day19,
    Day20,
    Day21,
    Day22,
    Day23,
    Day24,
    Day25,
    TOTAL_DAYS
};

enum Part
{
    Part01,
    Part02,
    TOTAL_PARTS
};

string ToString(Day d);
string ToString(Part p);

template <Day day>
void Run(Part part, istream& is, ostream& os)
{
    os << ToString(day) << " " << ToString(part) << " has not been implemented" << endl;
    if(is.bad())
    {
        os << "Input file does not exist" << endl;
    }
};

#endif // DAYS_H