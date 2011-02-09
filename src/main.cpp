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

void requestDeadlineMissRatio()
{
    map<int, pair<ConfigureItem, StatisticsData> > sinCollected;
    map<int, pair<ConfigureItem, StatisticsData> > dtiuCollected;

    list<ConfigureItem> configureItems = Configure::getInstance("requestDeadlineMissRatio");
    list<ConfigureItem>::iterator iter;

    int i;
    /* sin 算法 */
    for (iter = configureItems.begin(), i = 0; iter != configureItems.end(); iter++, i++)
    {
        StatisticsData statistics;

        Server server;
        MobileClient client(&server, iter->clientNumber, *iter);
        SINScheduler scheduler(&server, &statistics);

        server.setClient(&client);
        server.setScheduler(&scheduler);

        server.startSimulation();
        sinCollected.insert(pair<int, pair<ConfigureItem, StatisticsData> >
                (i, pair<ConfigureItem, StatisticsData> (*iter, statistics)));
    }

    /* dtiu 算法 */
    for (iter = configureItems.begin(), i = 0; iter != configureItems.end(); iter++, i++)
    {
        StatisticsData statistics;

        Server server;
        MobileClient client(&server, iter->clientNumber, *iter);
        DTIUScheduler scheduler(&server, &statistics);

        server.setClient(&client);
        server.setScheduler(&scheduler);

        server.startSimulation();
        dtiuCollected.insert(pair<int, pair<ConfigureItem, StatisticsData> >
                (i, pair<ConfigureItem, StatisticsData> (*iter, statistics)));
    }

    for (map<int, pair<ConfigureItem, StatisticsData> >::iterator iter = sinCollected.begin();
            iter != sinCollected.end(); iter++)
    {
        cout << iter->second.first.queryItemNumberMin << " " << iter->second.first.queryItemNumberMax << " "
                << iter->second.second.getDeadlineMissRatio() << endl;
    }

    for (map<int, pair<ConfigureItem, StatisticsData> >::iterator iter = dtiuCollected.begin();
            iter != dtiuCollected.end(); iter++)
    {
        cout << iter->second.first.queryItemNumberMin << " " << iter->second.first.queryItemNumberMax << " "
                << iter->second.second.getDeadlineMissRatio() << endl;
    }
}

void run()
{
    requestDeadlineMissRatio();
}

void rand_init()
{
    long currentTime;
    time(&currentTime);
    srand(currentTime);
}
