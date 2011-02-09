/*
 * Statistics.h
 *
 *  Created on: 2011-2-7
 */

#ifndef STATISTICS_H_
#define STATISTICS_H_

#include <string>

using namespace std;

typedef struct TagStatisticsData
{
    int totalRequest;
    int missDeadlineRequest;

    TagStatisticsData()
    {
        totalRequest = 0;
        missDeadlineRequest = 0;
    }

    double getDeadlineMissRatio()
    {
        return (double) missDeadlineRequest / (double) totalRequest;
    }

    bool operator< (const TagStatisticsData& d) const
    {
        return (double) missDeadlineRequest / (double) totalRequest < (double) d.missDeadlineRequest / (double) d.totalRequest;
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
