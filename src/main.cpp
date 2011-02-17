//============================================================================
// Name        : databroadcast.cpp
// Author      : 
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
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
    run();
}

void requestDeadlineMissRatio()
{
    map<int, pair<ConfigureItem, StatisticsData> > sinCollected;
    map<int, pair<ConfigureItem, StatisticsData> > dtiuCollected;
    map<int, pair<ConfigureItem, StatisticsData> > rmuoCollected;

    list<ConfigureItem> configureItems = Configure::getInstance("requestDeadlineMissRatio");
    list<ConfigureItem>::iterator iter;

    /* 每个算法的随机数种子都要一样 */
    long seed = configureItems.front().seed;
    if (seed == 0)
    {
        seed = time(0);
    }

    /* 把种子打印出来，用来重现上次的计算过程 */
    cout << "seed: " << seed << endl;

    int i; /* 这里的 i 就是 map 的第一个参数 */
    /* sin 算法 */
    srand(seed);
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
    srand(seed);
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

    /* rmuo 算法 */
    srand(seed);
    for (iter = configureItems.begin(), i = 0; iter != configureItems.end(); iter++, i++)
    {
        StatisticsData statistics;

        Server server;
        RMUOClient client(&server, iter->clientNumber, *iter);
        RMUOScheduler scheduler(&server, &statistics);

        server.setClient(&client);
        server.setScheduler(&scheduler);

        server.startSimulation();
        rmuoCollected.insert(pair<int, pair<ConfigureItem, StatisticsData> >
               (i, pair<ConfigureItem, StatisticsData> (*iter, statistics)));
    }

    /* 打印结果 */
    fstream result("requestDeadlineMissRatio.result.txt", fstream::out);

    result << "sin statistics(QueryItem)" << endl;
    result << "min\tmax\tratio" << endl;
    for (map<int, pair<ConfigureItem, StatisticsData> >::iterator iter = sinCollected.begin();
            iter != sinCollected.end(); iter++)
    {
        result << iter->second.first.queryItemNumberMin << "\t" << iter->second.first.queryItemNumberMax << "\t"
                << iter->second.second.getDeadlineMissRatio() << endl;
    }

    result << "dtiu statistics(QueryItem)" << endl;
    result << "min\tmax\tratio" << endl;
    for (map<int, pair<ConfigureItem, StatisticsData> >::iterator iter = dtiuCollected.begin();
            iter != dtiuCollected.end(); iter++)
    {
        result << iter->second.first.queryItemNumberMin << "\t" << iter->second.first.queryItemNumberMax << "\t"
                << iter->second.second.getDeadlineMissRatio() << endl;
    }

    result << "rmuo statistics(QueryItem)" << endl;
    result << "min\tmax\tratio" << endl;
    for (map<int, pair<ConfigureItem, StatisticsData> >::iterator iter = rmuoCollected.begin();
            iter != rmuoCollected.end(); iter++)
    {
        result << iter->second.first.queryItemNumberMin << "\t" << iter->second.first.queryItemNumberMax << "\t"
                << iter->second.second.getDeadlineMissRatio() << endl;
    }
    result.close();
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
