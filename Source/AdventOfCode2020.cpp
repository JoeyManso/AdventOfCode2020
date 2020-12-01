// AdventOfCode2020.cpp
//

#include "AdventTimer.h"
#include "Days.h"

#include <sstream>
#include <vector>

double RunTimed(Day day, Part part, istream& is, ostream& os);

int main(int argc, const char* argv[])
{
    ostream os(cout.rdbuf());
    vector<string> days = vector<string>();
    if(argc > 1)
    {
        stringstream ss(argv[1]); // Turn the string into a stream.
        string arg;

        while(getline(ss, arg, '|')) 
        {
            days.push_back(arg);
        }
    }

    double totalTime(0.0);
    for(int d = Day::Day01; d < Day::TOTAL_DAYS; ++d)
    {
        Day day = Day(d);
        string dayStr = ToString(day);
        if(days.empty() || find(days.begin(), days.end(), dayStr) != days.end())
        {
            os << "*********************** " << dayStr << " ************************" << endl;
            ifstream is{ "Input/" + dayStr + ".txt" };
            for(int p = Part::Part01; p < Part::TOTAL_PARTS; ++p)
            {
                Part part = Part(p);
                os << ToString(part) << ":" << endl << endl;
                is.clear();
                is.seekg(0, ios::beg);
                totalTime += RunTimed(day, part, is, os);
                if(p != Part::TOTAL_PARTS-1)
                {
                    os << "------------------------------------------------------" << endl;
                }
            }
            is.close();
            os << "******************************************************" << endl << endl << endl;
        }
    }

    os.precision(5);
    os << "Total time: ";
    os.setf(ios::fixed, ios::floatfield);
    os << totalTime << "ms" << endl;

    system("pause");
    return EXIT_SUCCESS;
}

extern template void Run<Day01>(Part, istream&, ostream&);
extern template void Run<Day02>(Part, istream&, ostream&);
extern template void Run<Day03>(Part, istream&, ostream&);
extern template void Run<Day04>(Part, istream&, ostream&);
extern template void Run<Day05>(Part, istream&, ostream&);
extern template void Run<Day06>(Part, istream&, ostream&);
extern template void Run<Day07>(Part, istream&, ostream&);
extern template void Run<Day08>(Part, istream&, ostream&);
extern template void Run<Day09>(Part, istream&, ostream&);
extern template void Run<Day10>(Part, istream&, ostream&);
extern template void Run<Day11>(Part, istream&, ostream&);
extern template void Run<Day12>(Part, istream&, ostream&);
extern template void Run<Day13>(Part, istream&, ostream&);
extern template void Run<Day14>(Part, istream&, ostream&);
extern template void Run<Day15>(Part, istream&, ostream&);
extern template void Run<Day16>(Part, istream&, ostream&);
extern template void Run<Day17>(Part, istream&, ostream&);
extern template void Run<Day18>(Part, istream&, ostream&);
extern template void Run<Day19>(Part, istream&, ostream&);
extern template void Run<Day20>(Part, istream&, ostream&);
extern template void Run<Day21>(Part, istream&, ostream&);
extern template void Run<Day22>(Part, istream&, ostream&);
extern template void Run<Day23>(Part, istream&, ostream&);
extern template void Run<Day24>(Part, istream&, ostream&);
extern template void Run<Day25>(Part, istream&, ostream&);

double RunTimed(Day day, Part part, istream& is, ostream& os)
{
    double runTime(0.0);
    AdventTimer timer;

    switch(day)
    {
        case Day01:
            Run<Day01>(part, is, os);
            break;
        case Day02:
            Run<Day02>(part, is, os);
            break;
        case Day03:
            Run<Day03>(part, is, os);
            break;
        case Day04:
            Run<Day04>(part, is, os);
            break;
        case Day05:
            Run<Day05>(part, is, os);
            break;
        case Day06:
            Run<Day06>(part, is, os);
            break;
        case Day07:
            Run<Day07>(part, is, os);
            break;
        case Day08:
            Run<Day08>(part, is, os);
            break;
        case Day09:
            Run<Day09>(part, is, os);
            break;
        case Day10:
            Run<Day10>(part, is, os);
            break;
        case Day11:
            Run<Day11>(part, is, os);
            break;
        case Day12:
            Run<Day12>(part, is, os);
            break;
        case Day13:
            Run<Day13>(part, is, os);
            break;
        case Day14:
            Run<Day14>(part, is, os);
            break;
        case Day15:
            Run<Day15>(part, is, os);
            break;
        case Day16:
            Run<Day16>(part, is, os);
            break;
        case Day17:
            Run<Day17>(part, is, os);
            break;
        case Day18:
            Run<Day18>(part, is, os);
            break;
        case Day19:
            Run<Day19>(part, is, os);
            break;
        case Day20:
            Run<Day20>(part, is, os);
            break;
        case Day21:
            Run<Day21>(part, is, os);
            break;
        case Day22:
            Run<Day22>(part, is, os);
            break;
        case Day23:
            Run<Day23>(part, is, os);
            break;
        case Day24:
            Run<Day24>(part, is, os);
            break;
        case Day25:
            Run<Day25>(part, is, os);
            break;
        default:
            os << "Unable to parse day: " << ToString(day);
            break;
    }
    runTime = timer.current();
    os.precision(5);
    os << endl << "Time: ";
    os.setf(ios::fixed, ios::floatfield);
    os << runTime << "ms" << endl;
    return runTime;    
}