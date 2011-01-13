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

using namespace std;

void run();
void rand_init();

int main()
{
    rand_init();
    run();
}

void run()
{
    /* sin 调度算法 */
    Server sinServer;
    SINScheduler *sinScheduler = new SINScheduler(&sinServer);
    MobileClient *sinClient = new MobileClient(&sinServer, 100);
    sinServer.setClient(sinClient);
    sinServer.setScheduler(sinScheduler);
    sinServer.startSimulation();

    /* dtiu 调度算法 */


    /* rmuo 调度算法 */

}

void rand_init()
{
    long currentTime;
    time(&currentTime);
    srand(currentTime);
}
