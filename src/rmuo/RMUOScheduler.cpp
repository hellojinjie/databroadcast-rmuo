/*
 * RMUOScheduler.cpp
 *
 *  Created on: 2011-1-9
 */

#include "RMUOScheduler.h"
#include <cmath>


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
    preprocess();

    if (scheduleQueue.size() != 0)
    {
        verifySchedulability();

        transformToHarmonic();

        broadcast();
    }
    else
    {
        this->server->incrementAndGetClock();
        cout << "请求队列里没有数据，一个空的时槽" << endl;
    }
}

void RMUOScheduler::broadcast()
{

}

void RMUOScheduler::checkDeadline()
{

}

bool RMUOScheduler::verifySchedulability()
{
    double utilization = 0.0;
    list<RMUORequest>::iterator scheduleQueueIter;
    for (scheduleQueueIter = scheduleQueue.begin();
            scheduleQueueIter != scheduleQueue.end(); scheduleQueueIter++)
    {
        utilization = utilization +
                scheduleQueueIter->uniqueSet.size() / scheduleQueueIter->period;
    }

    int m = scheduleQueue.size();
    double maxBound = (double) m * (pow(2, 1 / m) - 1.0);

    if (utilization <= maxBound)
    {
        return true;
    }
    else
    {
        return false;
    }
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
    /* first, 找个那个 l_j */
    RMUORequest shortestPeriodRequest = *scheduleQueue.begin();
    double l_star = 0;
    double A_j_min = 0;
    list<RMUORequest>::iterator iter;
    for (iter = scheduleQueue.begin(); iter != scheduleQueue.end(); iter++)
    {
        double l_j = iter->period /
                pow(2, ceil(log(iter->period / shortestPeriodRequest.period) / log(2)));
        if (l_j <= shortestPeriodRequest.period / 2 || l_j > shortestPeriodRequest.period)
        {
            continue;
        }

        double A_j = 0;
        list<RMUORequest>::iterator innerIter;
        for (innerIter = scheduleQueue.begin(); innerIter != scheduleQueue.end(); innerIter++)
        {
            A_j = A_j + innerIter->period / l_j *
                    pow(2, floor(log(innerIter->period / shortestPeriodRequest.period) / log(2)));
        }

        if (A_j_min == 0)
        {
            A_j_min = A_j;
            l_star = l_j;
        }
        else if (A_j < A_j_min)
        {
            A_j_min = A_j;
            l_star = l_j;
        }
    }

    /* second, 计算新的广播周期 */
    list<RMUORequest>::iterator iter1;
    for (iter1 = scheduleQueue.begin(); iter1 != scheduleQueue.end(); iter1++)
    {
        iter1->harmonicPeriod = l_star *
                pow(2, floor(log(iter1->period / shortestPeriodRequest.period) / log(2)));
    }

    /* third, 对上面计算的周期进行向上取整, 这个 for 循环本应该和上面的那个写在一起 */
    list<RMUORequest>::iterator iter2;
    int priority = 0;
    double priorHarmonicPeriod = 0;
    bool needCeil = false;
    double basePeriod = scheduleQueue.front().harmonicPeriod;
    if (basePeriod != (int)basePeriod)
    {
        basePeriod = ceil(basePeriod);
        needCeil = true;
    }
    for (iter2 = scheduleQueue.begin(); iter2 != scheduleQueue.end(); iter2++)
    {
        if (iter2->harmonicPeriod != priorHarmonicPeriod)
        {
            priority++;
            priorHarmonicPeriod = iter2->harmonicPeriod;
            list<RMUORequest> l;
            pair<int, list<RMUORequest> > p(priority, l);
            harmonicQueue.insert(p);
        }
        if (needCeil)
        {
            iter2->harmonicPeriod = basePeriod * priority;
        }
        harmonicQueue[priority].push_back(*iter2);
    }
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
