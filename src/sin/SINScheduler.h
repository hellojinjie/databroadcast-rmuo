/*
 * SINScheduler.h
 *
 *  Created on: 2011-1-11
 */

#ifndef SINSCHEDULER_H_
#define SINSCHEDULER_H_

#include "../Scheduler.h"
#include <list>

using namespace std;

typedef struct TagSINDataItem
{
    int item;
    /** 被请求的次数 */
    int requestCount;
    /** absolute deadline,所有请求该数据项的最小的那个 deadline */
    int deadline;

    /** 这个方法是必须的，不可以忘记了 */
    bool operator ==(TagSINDataItem a)
    {
        return item == a.item;
    }
} SINDataItem;


class SINScheduler: public Scheduler
{
public:
    SINScheduler();
    SINScheduler(Server *server, StatisticsData* statistics);
    virtual ~SINScheduler();

    void doSchedule();

private:
    list<SINDataItem> requestItems;
    list<SimpleRequest> scheduleQueue;
};

#endif /* SINSCHEDULER_H_ */
