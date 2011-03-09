/*
 * Simulation.h
 * Author: hellojinjie
 * Email: hellojinjie@gmail.com
 * Created on: 2011-3-7
 */

#ifndef SIMULATION_H_
#define SIMULATION_H_

#include <map>
#include "../SimpleRequest.h"
#include "../Configure.h"
#include "../Statistics.h"

using namespace std;

class Simulation
{
public:
    Simulation();
    virtual ~Simulation();

    virtual bool run()=0;

    virtual void print()=0;

};

#endif /* SIMULATION_H_ */
