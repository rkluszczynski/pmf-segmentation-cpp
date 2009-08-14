#include "../include/statistics.h"

namespace pmf
{
    Statistics::Statistics() : numberOfBirths(0), numberOfUpdates(0), numberOfDeaths(0) { }
    /*
    Statistics::Statistics(const Statistics & stats)
    {
        numberOfBirths = stats.GetNumberOfBirths();
        numberOfUpdates = stats.GetNumberOfUpdates();
        numberOfDeaths = stats.GetNumberOfDeaths();
    }
    //*/
    Statistics::~Statistics() { }

    unsigned int Statistics::GetNumberOfBirths()  { return numberOfBirths; }
    unsigned int Statistics::GetNumberOfUpdates()  { return numberOfUpdates; }
    unsigned int Statistics::GetNumberOfDeaths()  { return numberOfDeaths; }


    void Statistics::IncrementBirth() { ++numberOfBirths; }
    void Statistics::IncrementUpdate() { ++numberOfUpdates; }
    void Statistics::IncrementDeath() { ++numberOfDeaths; }

}
