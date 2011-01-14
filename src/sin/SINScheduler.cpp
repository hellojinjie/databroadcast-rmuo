/*
 * SINScheduler.cpp
 *
 *  Created on: 2011-1-11
 */

#include "SINScheduler.h"
#include <iostream>

using namespace std;

SINScheduler::SINScheduler()
{

}

SINScheduler::SINScheduler(Server *server) :
    Scheduler(server)
{

}

SINScheduler::~SINScheduler()
{

}

/**
 * 这个算法的数据结构设计的不好，导致实现的时候很烦
 * STL list 里的元素允许重复，但 remove 时会将所有重复的都删除了
 */
void SINScheduler::doSchedule()
{
    cout << "开始一次调度" << endl;

    /* first, 检查是否有request 已经 miss deadline */
    list<SINDataItem>::iterator dataIter;
    for (dataIter = requestItems.begin(); dataIter != requestItems.end(); )
    {
        /* 加一是因为transmit要一个tick */
        if (dataIter->requestCount != 0 && dataIter->deadline < server->getClock() + 1)
        {
            /* 如果这个 data item miss deadline 了， 就将这个data item 删了，
             * 请求了这个data item 的request都miss deadline 了，要处理他们，
             */
            SINDataItem item = *dataIter;
            cout << "错过截止期的 item：" << item.item << endl;
            dataIter = requestItems.erase(dataIter);
            /* 下面检查哪些request错过了截止期，并将他们删了，同时更新 requestItems 中的 requestCount */
            list<SimpleRequest>::iterator scheduleIter = scheduleQueue.begin();
            while (scheduleIter != scheduleQueue.end())
            {
                // cout << "item:" << item.item << " request: " << scheduleIter->id << " scheduleQueue size: " << scheduleQueue.size() << endl;
                if (this->isInReadSet(*scheduleIter, item.item))
                {
                    /* 这个数据项错过了截止期，该request 请求了这个数据项，就说明他也错过截止期了
                     * TODO 这个还应该更新 deadline miss ratio
                     * 这个request 还请求了其他数据项，将相应的 requestCount 减一 */
                    scheduleIter->readSet.remove(item.item);
                    list<int>::iterator otherDataIter;
                    for (otherDataIter = scheduleIter->readSet.begin();
                            otherDataIter != scheduleIter->readSet.end(); otherDataIter++)
                    {
                        list<SINDataItem>::iterator requestItemsIter;
                        for (requestItemsIter = requestItems.begin();
                                requestItemsIter != requestItems.end(); requestItemsIter++)
                        {
                            if (requestItemsIter->item == *otherDataIter)
                            {
                                requestItemsIter->requestCount--;
                            }
                        }
                    }
                    cout << "该请求错过截止期, id:" << scheduleIter->id << endl;
                    scheduleIter = scheduleQueue.erase(scheduleIter);
                    cout << "after erase scheduleQueue size:" << scheduleQueue.size() << endl;
                }
                else
                {
                    scheduleIter++;
                }
            }
        }
        else
        {
            dataIter++;
        }
    }
    /* 将requestCount 是0 的清除掉 */
    for (list<SINDataItem>::iterator iter = requestItems.begin(); iter != requestItems.end(); )
    {
        if (iter->requestCount == 0)
        {
            iter = requestItems.erase(iter);
        }
        else
        {
            iter++;
        }
    }

    /* second, 处理 pendingQueue 里的请求 */
    cout << "pendingQueue has requests: " << pendingQueue.size() << endl;
    for (list<SimpleRequest>::iterator requestIter = pendingQueue.begin(); requestIter
            != pendingQueue.end(); requestIter++)
    {
        /* 先判断要加入的这个请求是不是已经错过截止期了 */
        if (requestIter->arrivalTime + requestIter->period < (int)this->server->getClock())
        {
            /* TODO 这里要记录错过截止期的请求 */
            continue;
        }
        /* 将新加入的请求中的ReadSet 中的data item 加入到 requestItems */
        list<int>::iterator readSetIter;
        for (readSetIter = requestIter->readSet.begin(); readSetIter != requestIter->readSet.end(); readSetIter++)
        {
            list<SINDataItem>::iterator dataIter;
            bool alreadyExist = false;
            for (dataIter = requestItems.begin(); dataIter != requestItems.end(); dataIter++)
            {
                if (dataIter->item == *readSetIter)
                {
                    dataIter->requestCount++;
                    if (dataIter->deadline > requestIter->period + requestIter->arrivalTime)
                    {
                        dataIter->deadline = requestIter->period + requestIter->arrivalTime;
                    }
                    alreadyExist = true;
                    break;
                }
            }
            if (!alreadyExist)
            {
                SINDataItem item;
                item.deadline = requestIter->period + requestIter->arrivalTime;
                item.item = *readSetIter;
                item.requestCount = 1;
                requestItems.push_back(item);
            }
        }
        /* 同时将该请求加入调度队列 */
        scheduleQueue.push_back(*requestIter);
    }
    pendingQueue.clear();
    cout << "scheduleQueue 共有请求个数: " << scheduleQueue.size() << endl;

    /* third,  计算优先级，并选取 sin-1 值最小的那个data item广播*/
    if (requestItems.size() == 0)
    {
        /* 没有数据项要广播, 一个空的时槽 */
        server->incrementAndGetClock();
        return;
    }
    list<SINDataItem>::iterator minimumSINItem = requestItems.begin();
    double minimumSINValue = (double) (minimumSINItem->deadline - server->getClock()) /
            (double) minimumSINItem->requestCount;
    list<SINDataItem>::iterator iter;
    for (iter = minimumSINItem++; iter != requestItems.end(); iter++)
    {
        double sin = (double) (iter->deadline - server->getClock()) /
                (double) iter->requestCount;
        if (sin < minimumSINValue)
        {
            minimumSINValue = sin;
            minimumSINItem = iter;
        }
        else if (sin == minimumSINValue)
        {
            if (iter->deadline < minimumSINItem->deadline)
            {
                minimumSINItem = iter;
            }
        }
    }
    requestItems.remove(*minimumSINItem);
    /* 当前时间增一，表示这个 sin 值最小的数据项已经广播完成 */
    unsigned int clock = server->incrementAndGetClock();
    cout << "broadcast item:" << minimumSINItem->item << " at clock:" << clock << endl;

    /* fourth, 调整 scheduleQueue, 看看有没有 request 因为上面的data item 的广播而完成了 */
    list<SimpleRequest> needToRemove;
    list<SimpleRequest>::iterator scheduleQueueIter;
    for (scheduleQueueIter = scheduleQueue.begin();
            scheduleQueueIter != scheduleQueue.end(); scheduleQueueIter++)
    {
        if (this->isInReadSet(*scheduleQueueIter, minimumSINItem->item))
        {
            scheduleQueueIter->receivedItem.push_back(minimumSINItem->item);
            scheduleQueueIter->receivedItem.unique();
            if (scheduleQueueIter->receivedItem.size() == scheduleQueueIter->readSet.size())
            {
                /* 当前请求已经全部收到数据项, 删除该请求 */
                needToRemove.push_back(*scheduleQueueIter);
                cout << "该请求已经全部收到请求的数据,id：" << scheduleQueueIter->id << endl;
            }
        }
    }
    for (list<SimpleRequest>::iterator i = needToRemove.begin(); i != needToRemove.end(); i++)
    {
        scheduleQueue.remove(*i);
    }
}
