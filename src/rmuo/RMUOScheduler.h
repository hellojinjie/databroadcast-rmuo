/*
 * RMUOScheduler.h
 *
 *  Created on: 2011-1-9
 */

#ifndef RMUOSCHEDULER_H_
#define RMUOSCHEDULER_H_

#include "../Scheduler.h"

class RMUOScheduler: public Scheduler
{
public:
    RMUOScheduler();
    RMUOScheduler(Server *server);
    virtual ~RMUOScheduler();

    void doSchedule();
};

#endif /* RMUOSCHEDULER_H_ */
