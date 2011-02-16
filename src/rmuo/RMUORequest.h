/*
 * RMUORequest.h
 *
 *  Created on: 2011-1-20
 */

#ifndef RMUOREQUEST_H_
#define RMUOREQUEST_H_
#include <list>

using namespace std;

typedef struct TagRMUORequest
{
    int id;

    int period;

    double harmonicPeriod;

    int arrivalTime;

    list<int> readSet;

    list<int> receivedSet;

    list<int> uniqueSet;

    bool operator==(TagRMUORequest request)
    {
        return request.id == id;
    }

} RMUORequest;

#endif /* RMUOREQUEST_H_ */
