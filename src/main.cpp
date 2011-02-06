//============================================================================
// Name        : databroadcast.cpp
// Author      : 
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <ctime>
#include <cstdlib>
#include "Server.h"
#include "Scheduler.h"
#include "MobileClient.h"
#include "sin/SINScheduler.h"
#include "dtiu/DTIUScheduler.h"
#include "rmuo/RMUOClient.h"
#include "rmuo/RMUOScheduler.h"
#include "Configure.h"

using namespace std;

void run();
void rand_init();

int main()
{
    rand_init();
    run();
}

void sin()
{
    Server sinServer;

    SINScheduler sinScheduler(&sinServer);
    MobileClient sinClient(&sinServer, 10);

    sinServer.setClient(&sinClient);
    sinServer.setScheduler(&sinScheduler);
    sinServer.startSimulation();
}

void dtiu()
{
    Server dtiuServer;

    DTIUScheduler dtiuScheduler(&dtiuServer);
    MobileClient dtiuClient(&dtiuServer, 10);

    dtiuServer.setClient(&dtiuClient);
    dtiuServer.setScheduler(&dtiuScheduler);
    dtiuServer.startSimulation();
}

void rmuo()
{
    Server rmuoServer;

    RMUOScheduler rmuoScheduler(&rmuoServer);
    RMUOClient rmuoClient(&rmuoServer, 10);

    rmuoServer.setClient(&rmuoClient);
    rmuoServer.setScheduler(&rmuoScheduler);
    rmuoServer.startSimulation();
}

void run()
{
    /* sin 调度算法 */
    //sin();

    /* dtiu 调度算法 */
    //dtiu();

    /* rmuo 调度算法 */
    //rmuo();
    Configure config("simulation.conf.js");
}

void rand_init()
{
    long currentTime;
    time(&currentTime);
    srand(currentTime);
}
