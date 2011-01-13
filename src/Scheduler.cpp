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

bool Scheduler::isInReadSet(SimpleRequest request, int data)
{
    list<int>::iterator iter;
    for (iter = request.readSet.begin(); iter != request.readSet.end(); iter++)
    {
        if (data == *iter)
        {
            return true;
        }
    }
    return false;
}

void Scheduler::setServer(Server *server)
{
    this->server = server;
}

