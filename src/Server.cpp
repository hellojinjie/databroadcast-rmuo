/*
 * Server.cpp
 *
 *  Created on: 2011-1-10
 */

#include "Server.h"
#include <list>
#include <iostream>

using namespace std;

Server::Server()
{
    this->clock = 0;
}

Server::~Server()
{

}

void Server::startSimulation()
{

    while (true)
    {
        list<SimpleRequest> requests;
        this->client->generateRequests(requests);
        this->scheduler->addRequests(requests);
        if (!this->scheduler->doSchedule())
        {
            cout << "无法完成调度。" << endl;
            break;
        }

        cout << "server clock end: " << this->getClock() << endl;

        /* XXX 这里要注意 */
        if (this->getClock() >=
                Configure::getInstance("requestDeadlineMissRatio").front().totalSlot)
        {
            break;
        }
    }
}

Scheduler *Server::getScheduler() const
{
    return scheduler;
}

void Server::setScheduler(Scheduler *scheduler)
{
    this->scheduler = scheduler;
}

int Server::getClock()
{
    return this->clock;
}

int Server::incrementAndGetClock()
{
    this->clock++;
    return this->clock;
}

int Server::getAndIncrementClock()
{
    int tmp = this->clock;
    this->clock++;
    return tmp;
}

void Server::setClient(MobileClient *client)
{
    this->client = client;
}
