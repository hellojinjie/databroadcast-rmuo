/*
 * DTIUScheduler.cpp
 *
 *  Created on: 2011-1-15
 */

#include "DTIUScheduler.h"
#include <iostream>

using namespace std;

DTIUScheduler::DTIUScheduler(Server *server, StatisticsData* statistics) : Scheduler(server, statistics)
{

}

DTIUScheduler::~DTIUScheduler()
{

}

bool DTIUScheduler::doSchedule()
{
    for (list<SimpleRequest>::const_iterator iter = pendingQueue.begin();
            iter != pendingQueue.end(); iter++)
    {
        statistics->totalRequest[*iter]++;
    }

    /* first, 检查有没有错过截止期的 */
    int count = checkDeadline();

    /* second, 对 pendingQueue 进行预处理 */
    preprocess();

    if (scheduleQueue.size() > 0)
    {
        /* third, 根据算法选择一个 request进行广播 */
        SimpleRequest request = broadcast();

        /* fourth, 调整 UnserveredSet */
        postprocess(request);
    }
    else
    {
        cout << "调度队列里没有数据，一个空的时槽" << endl;
        server->incrementAndGetClock();
    }
    return true;
}

int DTIUScheduler::checkDeadline()
{
    int deadlineMissCount = 0;

    list<SimpleRequest>::iterator iter;
    for (iter = scheduleQueue.begin(); iter != scheduleQueue.end(); )
    {
        /* 三个算法检查的方法都是不一样的，要仔细想 */
        if (iter->arrivalTime + iter->period < server->getClock() + (int)(iter->readSet.size()))
        {
            cout << "该请求错过截止期：" << iter->id << endl;
            iter = scheduleQueue.erase(iter);
            statistics->missDeadlineRequest[*iter]++;
            deadlineMissCount++;
        }
        else
        {
            iter++;
        }
    }

    return deadlineMissCount + checkDeadline(pendingQueue);
}

/** 为啥这个要单独一个函数啊？ */
int DTIUScheduler::checkDeadline(list<SimpleRequest> &pendingQueue)
{
    /* 变量定义了，在用之前一定要记得初始化 */
    int deadlineMissCount = 0;

    list<SimpleRequest>::iterator iter;
    for (iter = pendingQueue.begin(); iter != pendingQueue.end(); )
    {
        if (iter->arrivalTime + iter->period < server->getClock() + (int)(iter->readSet.size()))
        {
            cout << "该请求错过截止期(pendingQueue)：" << iter->id << endl;
            iter = pendingQueue.erase(iter);
            statistics->missDeadlineRequest[*iter]++;
            deadlineMissCount++;
        }
        else
        {
            iter++;
        }
    }

    return deadlineMissCount;
}

void DTIUScheduler::preprocess()
{
    list<SimpleRequest>::iterator iter;
    for (iter = pendingQueue.begin(); iter != pendingQueue.end(); iter++)
    {
        scheduleQueue.push_back(*iter);
    }
    cout << "加入 scheduleQueue 请求个数：" << pendingQueue.size() << endl;

    /* XXX 这里我想要更新的是 BandwidthUtilization
     * 但是这个方法很笨，一定要注意其他地方的修改会不会对这个造成影响
     * 在整个算法中整个过程应该只算一遍
     */
    if (this->statistics->bandwidthUtilization == 0)
    {
        double utilization = 0.0;
        list<SimpleRequest>::const_iterator iter;
        for (iter = scheduleQueue.begin(); iter != scheduleQueue.end(); iter++)
        {
            utilization += (double) iter->readSet.size() / (double) iter->period;
        }
        this->statistics->bandwidthUtilization = utilization;
    }

    pendingQueue.clear();
}

/* 这个实现的效率太低了, 而且是实在太低了  */
SimpleRequest DTIUScheduler::broadcast()
{
    list<SimpleRequest>::iterator selectedIter = scheduleQueue.begin();
    double maxPriority = calculatePriority(*selectedIter);

    list<SimpleRequest>::iterator iter;
    for (iter = scheduleQueue.begin(), iter++; iter != scheduleQueue.end(); iter++)
    {
        double priority = calculatePriority(*iter);
        if (maxPriority < priority)
        {
            maxPriority = priority;
            selectedIter = iter;
        }
        else
        {
            if (maxPriority == priority)
            {
                if (iter->id < selectedIter->id)
                {
                    selectedIter = iter;
                }
            }
        }
    }

    list<int>::iterator readSetIter;
    for (readSetIter = selectedIter->readSet.begin();
            readSetIter != selectedIter->readSet.end(); readSetIter++)
    {
        int currentTime = server->incrementAndGetClock();
        cout << "广播数据项:" << *readSetIter << " at clock:" << currentTime << endl;
    }

    SimpleRequest request = *selectedIter;
    cout << "该请求已经完成调度:" << selectedIter->id << endl;
    scheduleQueue.erase(selectedIter);

    return request;
}

void DTIUScheduler::postprocess(SimpleRequest request)
{
    list<int>::iterator itemIter;
    for (itemIter = request.readSet.begin(); itemIter != request.readSet.end(); itemIter++)
    {
        list<SimpleRequest>::iterator requestIter;
        for (requestIter = scheduleQueue.begin(); requestIter != scheduleQueue.end(); )
        {
            bool deleteFlag = false;
            if (this->isInReadSet(*requestIter, *itemIter))
            {
                requestIter->readSet.remove(*itemIter);
                if (requestIter->readSet.size() == 0)
                {
                    /* 该请求仅通过共享其他请求的数据项就完成了调度 */
                    cout << "该请求已经完成调度：" << requestIter->id << endl;
                    deleteFlag = true;
                }
            }
            if (deleteFlag)
            {
                requestIter = scheduleQueue.erase(requestIter);
            }
            else
            {
                requestIter++;
            }
        }
    }
}

double DTIUScheduler::calculatePriority(SimpleRequest &request)
{
    int slackTime = request.arrivalTime + request.period - server->getClock();
    double urg = 1.0 / ((double) slackTime + (double) (request.readSet.size()));

    int totalRequestCount = 0;
    list<int>::iterator itemIter;
    for (itemIter = request.readSet.begin(); itemIter != request.readSet.end(); itemIter++)
    {
        list<SimpleRequest>::iterator requestIter;
        for (requestIter = scheduleQueue.begin(); requestIter != scheduleQueue.end(); requestIter++)
        {
            if (this->isInReadSet(*requestIter, *itemIter))
            {
                totalRequestCount++;
            }
        }
    }
    double temp = (double) totalRequestCount / (double) (request.readSet.size());

    double priority = temp * urg;

    return priority;
}
