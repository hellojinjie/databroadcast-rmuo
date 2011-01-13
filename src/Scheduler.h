/*
 * Scheduler.h
 *
 *  Created on: 2011-1-9
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <list>
#include "MobileClient.h"
#include "SimpleRequest.h"

using namespace std;

/* forward declare */
class Server;

class Scheduler
{
public:
    Scheduler();
    Scheduler(Server *server);
    virtual ~Scheduler();

    /**
     * 在一个调度周期内，调度数据项的发送
     * 对于RM-UO来说，一个调度周期就是一个 hyperperiod
     * 对于 SIN 来说，一个调度周期就是一个数据项
     * 对于DTIU来说，一个调度周期就是调度一个请求中的所有数据项
     */
    virtual void doSchedule() = 0;

    /** 将请求加入待处理队列 */
    virtual void addRequests(list<SimpleRequest> requests);

    /** 设置对 Server 的引用  */
    virtual void setServer(Server *server);

protected:
    /* they should be protected, not private */
    Server *server;
    list<SimpleRequest> pendingQueue;
};

#endif /* SCHEDULER_H_ */
