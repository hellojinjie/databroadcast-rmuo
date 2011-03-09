//============================================================================
// Name        : databroadcast.cpp
// Author      : hellojinjie
// Version     :
// Copyright   : copyleft
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "simulation/BandwidthUtilizationAndDeadlineMissRatio.h"
#include "simulation/RequestDeadlineMissRatio.h"

using namespace std;

void run();

int main()
{
    run();
}

void run()
{
    RequestDeadlineMissRatio simulationOne;
    if (simulationOne.run())
    {
        simulationOne.print();
    }

    BandwidthUtilizationAndDeadlineMissRatio simulationTwo;
    if (simulationTwo.run())
    {
        simulationTwo.print();
    }
}
