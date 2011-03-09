/*
 * RequestDeadlineMissRatio.cpp
 * Author: hellojinjie
 * Email: hellojinjie@gmail.com
 * Created on: 2011-3-7
 */

#include "RequestDeadlineMissRatio.h"
#include "../Configure.h"
#include "../Statistics.h"
#include "../Server.h"
#include "../sin/SINScheduler.h"
#include "../dtiu/DTIUScheduler.h"
#include "../rmuo/RMUOScheduler.h"
#include "../rmuo/RMUOClient.h"
#include <map>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

RequestDeadlineMissRatio::RequestDeadlineMissRatio()
{

}

RequestDeadlineMissRatio::~RequestDeadlineMissRatio()
{

}

bool RequestDeadlineMissRatio::run()
{
    list<ConfigureItem> configureItems = Configure::getInstance("requestDeadlineMissRatio");
    list<ConfigureItem>::iterator iter;

    if (!configureItems.front().enable)
    {
        cout << "requestDeadlineMissRatio is disabled..." << endl << endl;
        return false;
    }

    /* 每个算法的随机数种子都要一样 */
    long seed = configureItems.front().seed;
    if (seed == 0)
    {
        seed = time(0);
    }

    /* 把种子打印出来，用来重现上次的计算过程 */
    cout << "requestDeadlineMissRatio start..." << endl;
    cout << "seed: " << seed << endl;

    int i; /* 这里的 i 就是 map 的第一个参数 */
    /* sin 算法 */
    for (iter = configureItems.begin(), i = 0; iter != configureItems.end(); iter++, i++)
    {
        srand(seed);
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
        srand(seed);
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
    for (iter = configureItems.begin(), i = 0; iter != configureItems.end(); iter++, i++)
    {
        srand(seed);
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

    return true;
}

void RequestDeadlineMissRatio::print()
{
    list<ConfigureItem> configureItems = Configure::getInstance("requestDeadlineMissRatio");
    /* 打印结果 */
    fstream result(configureItems.front().resultOutputFilename.c_str(), fstream::out);

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
