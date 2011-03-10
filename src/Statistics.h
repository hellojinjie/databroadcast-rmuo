/*
 * Statistics.h
 *
 *  Created on: 2011-2-7
 */

#ifndef STATISTICS_H_
#define STATISTICS_H_

#include <string>
#include <map>
#include "SimpleRequest.h"

using namespace std;

typedef struct TagStatisticsData
{
    map<SimpleRequest, int> totalRequest;
    map<SimpleRequest, int> missDeadlineRequest;

    double bandwidthUtilization;
    double maxBound; /* rmuo 中的利用率的边界 */

    TagStatisticsData()
    {
        bandwidthUtilization = 0;
        maxBound = 0;
    }

    double getDeadlineMissRatio()
    {
        if (totalRequest.size() == 0)
        {
            return 1;
        }

        int total = 0;
        int miss = 0;
        for (map<SimpleRequest, int>::const_iterator iter = totalRequest.begin();
                iter != totalRequest.end(); iter++)
        {
            total += iter->second;
        }
        for (map<SimpleRequest, int>::const_iterator iter = missDeadlineRequest.begin();
                iter != missDeadlineRequest.end(); iter++)
        {
            miss += iter->second;
        }

        return (double) miss / (double) total;
    }

    bool operator< (const TagStatisticsData& d) const
    {
        return (double) this->getDeadlineMissRatio() < d.getDeadlineMissRatio();
    }
} StatisticsData;

/** 这个类留着，不用, 这个文件只用到上面的那个结构体。 */
class Statistics
{
public:
    static Statistics& getInstance(string algorithmName);

    void incrementTotalRequest();
    void incrementDeadlineMissed();

    double getDeadlineMissRatio();

    virtual ~Statistics();

private:
    Statistics(string algorithmName);

public:
    string algorithmName;

private:
    int deadlineMissed;
    int totalRequests;
};

#endif /* STATISTICS_H_ */
