/*
 * SINScheduler.cpp
 *
 *  Created on: 2011-1-11
 */

#include "SINScheduler.h"

SINScheduler::SINScheduler()
{

}

SINScheduler::SINScheduler(Server *server) : Scheduler(server)
{

}

SINScheduler::~SINScheduler()
{

}

void SINScheduler::doSchedule()
{
    /* first, 处理 pendingQueue 里的请求 */
    for (list<SimpleRequest>::iterator iter = pendingQueue.begin(); iter != pendingQueue.end(); iter++)
    {

    }
}
