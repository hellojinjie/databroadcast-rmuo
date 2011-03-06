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
#include <assert.h>
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

    if (!configureItems.front().enable)
    {
        cout << "requestDeadlineMissRatio is disabled..." << endl << endl;
        return;
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

bool readSetSizeComparsion(SimpleRequest r1, SimpleRequest r2)
{
    if (r1.readSet.size() < r2.readSet.size())
    {
        return true;
    }
    else
    {
        if (r1.readSet.size() == r2.readSet.size() && r1.period < r2.period)
        {
            return true;
        }
    }
    return false;
}

template<class Server, class Scheduler, class Client>
void runBandwidthUtilizationAndDeadlineMissRatio(long seed, list<pair<ConfigureItem, StatisticsData> > &collected)
{
    list<ConfigureItem> configureItems = Configure::getInstance("utilizationAndDeadlineMissRatio");

    int clientNumberMin = configureItems.front().clientNumberMin;
    int clientNumberMax = configureItems.front().clientNumberMax;
    int clientNumberIncreaseStep = configureItems.front().clientNumberIncreaseStep;

    for (int clientNumber = clientNumberMin; clientNumber <= clientNumberMax; clientNumber += clientNumberIncreaseStep)
    {
        srand(seed);
        configureItems.front().seed = seed;
        StatisticsData statistics;

        Server server;
        Scheduler scheduler(&server, &statistics);
        Client client(&server, clientNumberMin, configureItems.front());

        list<SimpleRequest> clients = client.getClients();
        assert((int)clients.size() > clientNumberIncreaseStep);
        clients.sort(readSetSizeComparsion);
        list<SimpleRequest> clientsToAdd;
        int i = 0;
        list<SimpleRequest>::const_reverse_iterator iter = clients.rbegin();
        for ( ; i < clientNumberIncreaseStep; i++, iter++)
        {
            clientsToAdd.push_back(*iter);
        }
        for (int i = 0; i < (clientNumber - clientNumberMin) / clientNumberIncreaseStep; i++)
        {
            client.addClients(clientsToAdd);
        }

        server.setClient(&client);
        server.setScheduler(&scheduler);

        server.startSimulation();
        collected.push_back(pair<ConfigureItem, StatisticsData> (configureItems.front(), statistics));
    }
}

void printUtilizationAndDeadlineMissRatio(fstream &resultStream, string title, list<pair<ConfigureItem, StatisticsData> > &collected)
{
    list<ConfigureItem> configureItems = Configure::getInstance("utilizationAndDeadlineMissRatio");

    int clientNumberMin = configureItems.front().clientNumberMin;
    int clientNumberIncreaseStep = configureItems.front().clientNumberIncreaseStep;
    int i = 0;
    resultStream << title << endl;
    resultStream << "seed\t\tclient\t\tratio\t\tutilaztion\t\tmaxBound" << endl;
    for (list<pair<ConfigureItem, StatisticsData> >::iterator iter = collected.begin();
           iter != collected.end(); iter++)
    {
        resultStream << iter->first.seed << "\t\t"
               << (clientNumberMin + clientNumberIncreaseStep * i++) << "\t\t"
               << iter->second.getDeadlineMissRatio() << "\t\t"
               << iter->second.bandwidthUtilization << "\t\t"
               << iter->second.maxBound << endl;
    }
}

void utilizationAndDeadlineMissRatio()
{
    list<pair<ConfigureItem, StatisticsData> > sinCollected;
    list<pair<ConfigureItem, StatisticsData> > dtiuCollected;
    list<pair<ConfigureItem, StatisticsData> > rmuoCollected;

    list<ConfigureItem> configureItems = Configure::getInstance("utilizationAndDeadlineMissRatio");

    if (!configureItems.front().enable)
    {
        cout << "utilizationAndDeadlineMissRatio is disabled..." << endl << endl;
        return;
    }

    /* 每个算法的随机数种子都要一样 */
    long seed = configureItems.front().seed;
    if (seed == 0)
    {
        seed = time(0);
    }

    /* 把种子打印出来，用来重现上次的计算过程 */
    cout << "utilizationAndDeadlineMissRatio start..." << endl;
    cout << "seed: " << seed << endl;

    /* rmuo 算法 */
    runBandwidthUtilizationAndDeadlineMissRatio<Server, RMUOScheduler, RMUOClient>(seed, rmuoCollected);

    /* sin 算法 */
    runBandwidthUtilizationAndDeadlineMissRatio<Server, SINScheduler, MobileClient>(seed, sinCollected);

    /* dtiu 算法 */
    runBandwidthUtilizationAndDeadlineMissRatio<Server, DTIUScheduler, MobileClient>(seed, dtiuCollected);

    /* 打印结果 */
    fstream result(configureItems.front().resultOutputFilename.c_str(), fstream::out);

    printUtilizationAndDeadlineMissRatio(result, "sin client number range", sinCollected);
    printUtilizationAndDeadlineMissRatio(result, "dtiu client number range", dtiuCollected);
    printUtilizationAndDeadlineMissRatio(result, "rmuo client number range", rmuoCollected);

    result.close();
}

void run()
{
    requestDeadlineMissRatio();
    utilizationAndDeadlineMissRatio();
}

void rand_init()
{
    long currentTime;
    time(&currentTime);
    srand(currentTime);
}
