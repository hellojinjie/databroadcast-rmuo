/*
 * RMUOScheduler.cpp
 *
 *  Created on: 2011-1-9
 */

#include "RMUOScheduler.h"
#include <cmath>
#include <iostream>
#include <cassert>

using namespace std;

RMUOScheduler::RMUOScheduler(Server *server, StatisticsData* statistics) :
     Scheduler(server, statistics), needVerifySchedulability(true)
{
}

RMUOScheduler::~RMUOScheduler()
{

}

bool RMUOScheduler::doSchedule()
{
    preprocess();

    if (scheduleQueue.size() != 0)
    {
        if (!verifySchedulability())
        {
            cout << "该请求队列不可调度，结束该算法" << endl;
            return false;
        }

        transformToHarmonic();

        broadcast();
    }
    else
    {
        this->server->incrementAndGetClock();
        cout << "请求队列里没有数据，一个空的时槽" << endl;
    }

    return true;
}

void RMUOScheduler::preprocess()
{
    if (pendingQueue.size() == 0)
    {
        needVerifySchedulability = false;
        return;
    }

    /* 有新的请求加入就要重新验证是否可调度 */
    needVerifySchedulability = true;

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
        rmuoIter->uniqueSet.clear();
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
    /* for debug */
    for (rmuoIter = scheduleQueue.begin(); rmuoIter != scheduleQueue.end(); rmuoIter++)
    {
        cout << "in request:" << rmuoIter->id << " uniqueSet: ";
        list<int>::iterator iter;
        for (iter = rmuoIter->uniqueSet.begin(); iter != rmuoIter->uniqueSet.end(); iter++)
        {
            cout << *iter << " ";
        }
        cout << endl;
    }
}

bool RMUOScheduler::verifySchedulability()
{
    if (needVerifySchedulability == false)
    {
        return true;
    }

    double utilization = 0.0;
    list<RMUORequest>::iterator scheduleQueueIter;
    for (scheduleQueueIter = scheduleQueue.begin(); scheduleQueueIter != scheduleQueue.end(); scheduleQueueIter++)
    {
        utilization = utilization + (double) scheduleQueueIter->uniqueSet.size()
                / (double) scheduleQueueIter->period;
    }

    int m = scheduleQueue.size();
    double maxBound = (double) ((double) m * (pow(2, 1.0 / m) - 1.0));

    this->statistics->bandwidthUtilization = utilization;
    this->statistics->maxBound = maxBound;

    /* for debug */
    cout << "utilization: " << utilization << ", maxBound: " << maxBound << endl;

    if (utilization <= maxBound)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void RMUOScheduler::transformToHarmonic()
{

    if (needVerifySchedulability == false)
    {
        return;
    }

    /* zero, 先清空 harmonicQueue，这个不能忘了，不然这个队列的数据就越来越多了 */
    harmonicQueue.clear();

    /* first, 找个那个 l_j */
    RMUORequest shortestPeriodRequest = scheduleQueue.front();
    double l_star = 0;
    double A_j_min = 0;
    list<RMUORequest>::iterator iter;
    for (iter = scheduleQueue.begin(); iter != scheduleQueue.end(); iter++)
    {
        double l_j = iter->period / pow(2, ceil(
                log(iter->period / shortestPeriodRequest.period) / log(2)));
        if (l_j <= shortestPeriodRequest.period / 2.0 || l_j > shortestPeriodRequest.period)
        {
            continue;
        }

        double A_j = 0;
        list<RMUORequest>::iterator innerIter;
        for (innerIter = scheduleQueue.begin(); innerIter != scheduleQueue.end(); innerIter++)
        {
            A_j = A_j + innerIter->period / (l_j * pow(2, floor(log(innerIter->period
                    / shortestPeriodRequest.period) / log(2))));
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
        iter1->harmonicPeriod = l_star * pow(2, floor(log(iter1->period
                / shortestPeriodRequest.period) / log(2)));
    }

    /* third, 对上面计算的周期进行向上取整, 这个 for 循环本应该和上面的那个写在一起 */
    list<RMUORequest>::iterator iter2;
    int priority = 0;
    double priorHarmonicPeriod = 0;
    bool needCeil = false;
    double basePeriod = scheduleQueue.front().harmonicPeriod;
    if (basePeriod != (int) basePeriod)
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
            cout << endl << "priority: " << priority << " period: " << iter2->harmonicPeriod
                    << " has requests: ";
        }
        if (needCeil)
        {
            iter2->harmonicPeriod = basePeriod * priority;
        }
        harmonicQueue[priority].push_back(*iter2);
        cout << iter2->id << " ";
    }
    cout << endl;
}

void RMUOScheduler::broadcast()
{
    int priorityLevels = harmonicQueue.size(); /* 共有多少个优先级 */
    int hyperperiod = harmonicQueue[priorityLevels].front().harmonicPeriod;
    int minPeriod = harmonicQueue[1].front().harmonicPeriod;

    /* first, 为每一个优先级生成UnserveredSet */
    map<int, list<int> > unserveredSet;
    map<int, list<RMUORequest> >::iterator iter;
    for (iter = harmonicQueue.begin(); iter != harmonicQueue.end(); iter++)
    {
        list<int> unservered;
        for (list<RMUORequest>::iterator innerIter = iter->second.begin(); innerIter
                != iter->second.end(); innerIter++)
        {
            unservered.insert(unservered.end(), innerIter->uniqueSet.begin(),
                    innerIter->uniqueSet.end());
        }
        pair<int, list<int> > data(iter->first, unservered);
        unserveredSet.insert(data);
    }

    /* for debug */
    cout << "priorityLevels:" << priorityLevels << ", hyperperiod:" << hyperperiod
            << ", minPeriod:" << minPeriod << endl;
    for (map<int, list<int> >::const_iterator iter = unserveredSet.begin(); iter
            != unserveredSet.end(); iter++)
    {
        cout << "unservered set, priority: " << iter->first << ", data: ";
        for (list<int>::const_iterator innerIter = iter->second.begin(); innerIter
                != iter->second.end(); innerIter++)
        {
            cout << *innerIter << ",";
        }
        cout << " total size:" << iter->second.size();
        cout << endl;
    }

    /* second, 按优先级广播数据 */
    /* 先定义一个临时用的 map */
    map<int, list<int> > workingSet;
    for (int clock = 0; clock < hyperperiod; clock++)
    {
        /* 开始之前先收集统计数据:总的请求个数
         * 并且做必要的初始化 */
        list<RMUORequest>::iterator iter;
        for (iter = scheduleQueue.begin(); iter != scheduleQueue.end(); iter++)
        {
            int rightTime = this->server->getClock() % iter->period;
            if (rightTime == 0)
            {
                iter->arrivalTime = this->server->getClock();
                iter->receivedSet.clear();
                statistics->totalRequest++;
            }
        }

        /* 先装填数据 */
        for (int i = 1; i <= priorityLevels; i++)
        {
            /* 在该优先级的新一个广播周期到来的时候，初始化该优先级的未广播数据，
             * 该算法已经确保在上个周期中，数据都已经广播出去，没有错过截止期的 */
            if (clock % (i * minPeriod) == 0)
            {
                workingSet[i] = unserveredSet[i];
            }
        }

        /* 然后挑优先级高的一个数据广播 */
        int i = 1;
        for (i = 1; i <= priorityLevels; i++)
        {
            if (workingSet[i].size() == 0)
            {
                continue;
            }
            else
            {
                int item = workingSet[i].front();
                workingSet[i].pop_front();
                int serverClock = this->server->incrementAndGetClock();
                cout << "broadcast item:" << item << " at clock:" << serverClock << endl;
                /* 检查其余的请求有没有错过截止期,这个应该针对每个数据项检查，还是每个请求检查
                 * 这里正对什么检查是没关系的，因为这个算法只要有错过截止期了，就说明算法错了*/
                checkDeadline(item);
                break;
            }
        }

        /* 判断上一步有没有数据广播出去 */
        if (i > priorityLevels)
        {
            int serverClock = this->server->incrementAndGetClock();
            cout << "no item broadcast at clock: " << serverClock << endl;
        }
    }
}

void RMUOScheduler::checkDeadline(int dataItem)
{
    list<RMUORequest>::iterator iter;
    for (iter = scheduleQueue.begin(); iter != scheduleQueue.end(); iter++)
    {
        /* 先调整 receivedSet */
        if (isInList(iter->readSet, dataItem))
        {
            /* 下面这行的前提是什么？
             * 每个周期结束后，receivedSet 都要正确的清空 */
            iter->receivedSet.push_back(dataItem);
            iter->receivedSet.unique();
            if (iter->receivedSet.size() == iter->readSet.size())
            {
                cout << "request: " << iter->id << " received all data." << endl;
            }
        }

        /* 检查是否有错过截止期的
         * 每个算法的检查方法都是不一样的，不可以照搬 */
        if (server->getClock() + (int) iter->readSet.size() - (int) iter->receivedSet.size()
                > iter->arrivalTime + iter->period)
        {
            /* 不应该会运行到这里的  */
            cout << "在 RMUO 算法中有请求错过截止期了" << endl;
            assert(false);
        }
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
