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

class SINScheduler: public Scheduler
{
public:
    SINScheduler();
    SINScheduler(Server *server);
    virtual ~SINScheduler();

    void doSchedule();

private:
    typedef struct {
        int item;
        /** 被请求的次数 */
        int requestCount;
        /** absolute deadline,所有请求该数据项的最小的那个 deadline */
        unsigned int deadline;
    } SINDataItem;
    list<SINDataItem> requestItems;
    list<SimpleRequest> scheduleQueue;
};

#endif /* SINSCHEDULER_H_ */
