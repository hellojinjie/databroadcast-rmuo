/*
 * RMUOScheduler.cpp
 *
 *  Created on: 2011-1-9
 */

#include "RMUOScheduler.h"

RMUOScheduler::RMUOScheduler()
{

}

RMUOScheduler::RMUOScheduler(Server *server) : Scheduler(server)
{

}

RMUOScheduler::~RMUOScheduler()
{

}

void RMUOScheduler::doSchedule()
{

}

void RMUOScheduler::broadcast()
{
}



bool RMUOScheduler::verifySchedulability()
{

    return false;
}



void RMUOScheduler::preprocess()
{
    /* first, 将 pendingQueue 里的请求加入 scheduleQueue */
    list<SimpleRequest>::iterator simpleIter;
    for (simpleIter = pendingQueue.begin(); simpleIter != pendingQueue.end(); simpleIter++)
    {
        RMUORequest request;
        request.id = simpleIter->id;
        request.period = simpleIter->period;
        request.readSet = simpleIter->readSet;
        request.arrivalTime = simpleIter->arrivalTime;
        scheduleQueue.push_back(request);
    }
    pendingQueue.clear();

    /* second, 按 period 排序，再重新计算 scheduleQueue 里的 uniqueSet*/
    scheduleQueue.sort(requestComparison);
    list<int> sharedSet;
    list<RMUORequest>::iterator rmuoIter;
    for (rmuoIter = scheduleQueue.begin(); rmuoIter != scheduleQueue.end(); rmuoIter++)
    {
        list<int>::iterator iter;
        for (iter = rmuoIter->readSet.begin(); iter != rmuoIter->readSet.end(); iter++)
        {
            if (!isInList(sharedSet, *iter))
            {
                sharedSet.push_back(*iter);
                rmuoIter->uniqueSet.push_back(*iter);
            }
        }
    }
}

void RMUOScheduler::transformToHarmonic()
{
}

bool RMUOScheduler::requestComparison(RMUORequest r1, RMUORequest r2)
{
    if (r1.period < r2.period)
    {
        return true;
    }
    else if (r1.period == r2.period && r1.id < r2.id)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool RMUOScheduler::isInList(list<int> l, int i)
{
    for (list<int>::iterator iter = l.begin(); iter != l.end(); iter++)
    {
        if (*iter == i)
        {
            return true;
        }
    }
    return false;
}
