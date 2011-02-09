/*
 * Statistics.cpp
 *
 *  Created on: 2011-2-7
 */

#include "Statistics.h"
#include <string>
#include <map>

using namespace std;

Statistics& Statistics::getInstance(string algorithmName)
{
    static map<string, Statistics> instances;
    map<string, Statistics>::iterator finded = instances.find(algorithmName);
    if (finded == instances.end())
    {
        Statistics statistics(algorithmName);
        instances.insert(pair<string, Statistics>(algorithmName, statistics));
        /* 这里不能 return statistics */
        return instances.find(algorithmName)->second;
    }
    else
    {
        return finded->second;
    }
}

Statistics::Statistics(string algorithmName)
{
    this->algorithmName = algorithmName;
}

Statistics::~Statistics()
{

}

void Statistics::incrementDeadlineMissed()
{
    this->deadlineMissed++;
}

void Statistics::incrementTotalRequest()
{
    this->totalRequests++;
}

double Statistics::getDeadlineMissRatio()
{
    return (double) deadlineMissed / (double) totalRequests;
}
