/*
 * Scheduler.cpp
 *
 *  Created on: 2011-1-9
 */

#include "Scheduler.h"

Scheduler::Scheduler()
{

}

Scheduler::Scheduler(Server *server)
{
    this->server = server;
}

Scheduler::~Scheduler()
{
}

void Scheduler::addRequests(list<SimpleRequest> requests)
{
    for (list<SimpleRequest>::iterator iter = requests.begin(); iter != requests.end(); iter++)
    {
        pendingQueue.push_back(*iter);
    }
}

void Scheduler::setServer(Server *server)
{
    this->server = server;
}

