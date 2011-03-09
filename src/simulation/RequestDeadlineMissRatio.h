/*
 * RequestDeadlineMissRatio.h
 * Author: hellojinjie
 * Email: hellojinjie@gmail.com
 * Created on: 2011-3-7
 */

#ifndef REQUESTDEADLINEMISSRATIO_H_
#define REQUESTDEADLINEMISSRATIO_H_

#include "Simulation.h"

class RequestDeadlineMissRatio: public Simulation
{
public:
    RequestDeadlineMissRatio();
    virtual ~RequestDeadlineMissRatio();

    bool run();

    void print();

private:
    map<int, pair<ConfigureItem, StatisticsData> > sinCollected;
    map<int, pair<ConfigureItem, StatisticsData> > dtiuCollected;
    map<int, pair<ConfigureItem, StatisticsData> > rmuoCollected;
};

#endif /* REQUESTDEADLINEMISSRATIO_H_ */
