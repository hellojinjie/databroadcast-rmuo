/*
 * RMUOClient.h
 *
 *  Created on: 2011-1-10
 */

#ifndef RMUOCLIENT_H_
#define RMUOCLIENT_H_

#include "../MobileClient.h"

class RMUOClient: public MobileClient
{
public:
    RMUOClient(Server *server, int count);
    virtual ~RMUOClient();

private:

};

#endif /* RMUOCLIENT_H_ */
