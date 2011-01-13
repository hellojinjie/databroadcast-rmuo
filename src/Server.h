/*
 * Server.h
 *
 *  Created on: 2011-1-10
 */

#ifndef SERVER_H_
#define SERVER_H_

#include "MobileClient.h"
#include "Scheduler.h"
/* forward declare */
class Scheduler;
class MobileClient;

class Server
{
public:
    Server();
    virtual ~Server();

    Scheduler *getScheduler() const;
    void setScheduler(Scheduler *scheduler);

    void startSimulation();

    unsigned int getClock();
    unsigned int getAndIncrementClock();
    unsigned int incrementAndGetClock();
    void setClient(MobileClient *client);

private:
    Scheduler *scheduler;

    MobileClient *client;

    /**
     * 模拟程序的时间，每调度一个数据项就增一
     * 假设模拟程序的运行时间不会超过 2^32 tick
     */
    unsigned int clock;
};

#endif /* SERVER_H_ */
