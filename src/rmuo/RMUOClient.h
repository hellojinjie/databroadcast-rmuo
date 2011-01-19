/*
 * RMUOClient.h
 *
 *  Created on: 2011-1-10
 */

#ifndef RMUOCLIENT_H_
#define RMUOCLIENT_H_

#include "../MobileClient.h"
#include "../SimpleRequest.h"
#include <list>
#include <iostream>

using namespace std;

class RMUOClient: public MobileClient
{
public:
    RMUOClient(Server *server, int count);
    virtual ~RMUOClient();

    /** 所有的请求都只发一遍就可以了  */
    virtual int generateRequests(list<SimpleRequest> &requests);
private:

};

#endif /* RMUOCLIENT_H_ */
