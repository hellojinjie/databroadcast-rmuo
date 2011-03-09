/*
 * BandwidthUtilizationAndDeadlineMissRatio.h
 * Author: hellojinjie
 * Email: hellojinjie@gmail.com
 * Created on: 2011-3-7
 */

#ifndef BANDWIDTHUTILIZATIONANDDEADLINEMISSRATIO_H_
#define BANDWIDTHUTILIZATIONANDDEADLINEMISSRATIO_H_

#include "Simulation.h"

class BandwidthUtilizationAndDeadlineMissRatio: public Simulation
{
public:
    BandwidthUtilizationAndDeadlineMissRatio();
    virtual ~BandwidthUtilizationAndDeadlineMissRatio();

    bool run();
    void print();

private:
    list<pair<ConfigureItem, StatisticsData> > sinCollected;
    list<pair<ConfigureItem, StatisticsData> > dtiuCollected;
    list<pair<ConfigureItem, StatisticsData> > rmuoCollected;
};

#endif /* BANDWIDTHUTILIZATIONANDDEADLINEMISSRATIO_H_ */
