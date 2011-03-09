/*
 * BandwidthUtilizationAndDeadlineMissRatio.cpp
 * Author: hellojinjie
 * Email: hellojinjie@gmail.com
 * Created on: 2011-3-7
 */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <map>
#include <list>
#include <string>
#include "BandwidthUtilizationAndDeadlineMissRatio.h"
#include "../Configure.h"
#include "../Statistics.h"
#include "../Server.h"
#include "../sin/SINScheduler.h"
#include "../dtiu/DTIUScheduler.h"
#include "../rmuo/RMUOScheduler.h"
#include "../rmuo/RMUOClient.h"

using namespace std;

template<class Server, class Scheduler, class Client>
void runBandwidthUtilizationAndDeadlineMissRatio(
        long seed, list<pair<ConfigureItem, StatisticsData> > &collected)
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

void printUtilizationAndDeadlineMissRatio(
        fstream &resultStream, string title,
        list<pair<ConfigureItem, StatisticsData> > &collected)
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

BandwidthUtilizationAndDeadlineMissRatio::BandwidthUtilizationAndDeadlineMissRatio()
{

}

BandwidthUtilizationAndDeadlineMissRatio::~BandwidthUtilizationAndDeadlineMissRatio()
{

}

bool BandwidthUtilizationAndDeadlineMissRatio::run()
{
    list<ConfigureItem> configureItems = Configure::getInstance("utilizationAndDeadlineMissRatio");

    if (!configureItems.front().enable)
    {
       cout << "utilizationAndDeadlineMissRatio is disabled..." << endl << endl;
       return false;
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
    runBandwidthUtilizationAndDeadlineMissRatio<Server, RMUOScheduler, RMUOClient>(
            seed, rmuoCollected);

    /* sin 算法 */
    runBandwidthUtilizationAndDeadlineMissRatio<Server, SINScheduler, MobileClient>(
            seed, sinCollected);

    /* dtiu 算法 */
    runBandwidthUtilizationAndDeadlineMissRatio<Server, DTIUScheduler, MobileClient>(
            seed, dtiuCollected);

    return true;
}

void BandwidthUtilizationAndDeadlineMissRatio::print()
{
    list<ConfigureItem> configureItems = Configure::getInstance("utilizationAndDeadlineMissRatio");

    /* 打印结果 */
    fstream result(configureItems.front().resultOutputFilename.c_str(), fstream::out);

    printUtilizationAndDeadlineMissRatio(result, "sin client number range", sinCollected);
    printUtilizationAndDeadlineMissRatio(result, "dtiu client number range", dtiuCollected);
    printUtilizationAndDeadlineMissRatio(result, "rmuo client number range", rmuoCollected);

    result.close();
}


