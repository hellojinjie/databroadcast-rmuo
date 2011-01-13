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

    /* first, 处理 pendingQueue 里的请求 */
    for (list<SimpleRequest>::iterator requestIter = pendingQueue.begin(); requestIter
            != pendingQueue.end(); requestIter++)
    {
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
                    if (dataIter->deadline > requestIter->period + server->getClock())
                    {
                        dataIter->deadline = requestIter->period + server->getClock();
                    }
                    alreadyExist = true;
                    break;
                }
            }
            if (!alreadyExist)
            {
                SINDataItem item;
                item.deadline = requestIter->period + server->getClock();
                item.item = *readSetIter;
                item.requestCount = 1;
                requestItems.push_back(item);
            }
        }
        scheduleQueue.push_back(*requestIter);
    }
    pendingQueue.clear();

    /* second, 检查是否有request 已经 miss deadline */
    list<SINDataItem>::iterator dataIter;
    list<SINDataItem> tmp = requestItems;
    for (dataIter = tmp.begin(); dataIter != tmp.end(); dataIter++)
    {
        /* 加一是因为transmit要一个tick */
        if (dataIter->deadline > server->getClock() + 1)
        {
            /*
             * 如果这个 data item miss deadline 了， 就将这个data item 删了，
             * 请求了这个data item 的request都miss deadline 了，要处理他们，
             */
            requestItems.remove(*dataIter);
            /* 下面检查哪些request错过了截止期，并将他们删了，同时更新 requestItems 中的 requestCount */
            list<SimpleRequest>::iterator scheduleIter;
            list<SimpleRequest> scheduleQueueTmp = scheduleQueue;
            for (scheduleIter = scheduleQueueTmp.begin();
                    scheduleIter != scheduleQueueTmp.end(); scheduleIter++)
            {
                if (this->isInReadSet(*scheduleIter, dataIter->item))
                {
                    /* 这个数据项错过了截止期，该request 请求了这个数据项，就说明他也错过截止期了
                     * TODO 这个还应该更新 deadline miss ratio */
                    cout << "有一个请求错过了截止期：" << scheduleIter->id << endl;
                    scheduleQueue.remove(*scheduleIter);
                    /* 这个request 还请求了其他数据项，将相应的 requestCount 减一 */
                    list<int>::iterator otherDataIter;
                    scheduleIter->readSet.remove(dataIter->item);
                    for (otherDataIter = scheduleIter->readSet.begin();
                            otherDataIter != scheduleIter->readSet.end(); otherDataIter++)
                    {
                        list<SINDataItem>::iterator requestItemsIter;
                        list<SINDataItem> requestItemsTmp = requestItems;
                        for (requestItemsIter = requestItemsTmp.begin();
                                requestItemsIter != requestItemsTmp.end(); requestItemsIter++)
                        {
                            if (requestItemsIter->item == *otherDataIter)
                            {
                                requestItemsIter->requestCount--;
                                /* 如果 requestCount 减为 0 了，就要删除它 */
                                if (requestItemsIter->requestCount == 0)
                                {
                                    requestItems.remove(*requestItemsIter);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    /* third,  计算优先级，并选取 sin-1 值最小的那个data item广播*/
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
    server->incrementAndGetClock();

    /* fourth, 调整 scheduleQueue, 看看有没有 request 因为上面的data item 的广播而完成了 */
    list<SimpleRequest> scheduleQueueTmp = scheduleQueue;
    list<SimpleRequest>::iterator scheduleQueueTmpIter;
    for (scheduleQueueTmpIter = scheduleQueueTmp.begin();
            scheduleQueueTmpIter != scheduleQueueTmp.end(); scheduleQueueTmpIter++)
    {
        if (this->isInReadSet(*scheduleQueueTmpIter, minimumSINItem->item))
        {
            scheduleQueueTmpIter->receivedItem.push_back(minimumSINItem->item);
            scheduleQueueTmpIter->receivedItem.unique();
            if (scheduleQueueTmpIter->receivedItem.size() == scheduleQueueTmpIter->readSet.size())
            {
                /* 当前请求已经全部收到数据项, 删除该请求 */
                scheduleQueue.remove(*scheduleQueueTmpIter);
                cout << "该请求已经全部收到请求的数据：" << scheduleQueueTmpIter->id << endl;
            }
        }
    }
}
