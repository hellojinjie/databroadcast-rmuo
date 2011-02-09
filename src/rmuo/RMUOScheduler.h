/*
 * RMUOScheduler.h
 *
 *  Created on: 2011-1-9
 */

#ifndef RMUOSCHEDULER_H_
#define RMUOSCHEDULER_H_

#include "../Scheduler.h"
#include "RMUORequest.h"
#include <list>
#include <map>

using namespace std;

class RMUOScheduler: public Scheduler
{
public:
    RMUOScheduler(Server *server, StatisticsData* statistics);
    virtual ~RMUOScheduler();

    void doSchedule();

private:
    /** 已经按 period 从小到大排序的队列，如果 period 相等，按 request id 排序 */
    list<RMUORequest> scheduleQueue;

    /** 已排序的，并且已经将 period 转化成 harmonicPeriod,
     * 优先级（也就是周期）相同的放在一个list里， key 就是对应的优先级
     * key 是从 1 开始的 */
    map<int, list<RMUORequest> > harmonicQueue;

    /** 将 pendingQueue 中的 SimpleRequest 转化成 RMUORequest 加入 scheduleQueue */
    void preprocess();

    /** 在一个 hyperPeriod 内广播数据项 */
    void broadcast();

    /* 每发送一个数据项就对每个请求检查是否已经错过了截止期了，如果有错过的，就说明程序错了，或算法错了。 */
    void checkDeadline();

    /** 检查 scheduleQueue，验证其是否可调度 */
    bool verifySchedulability();

    /** 将 scheduleQueue 中请求的period 转化成 harmonicPeriod，并加入 harmonicQueue */
    void transformToHarmonic();

    /** 这个函数是在给request 排序的时候用的  */
    bool static requestComparison(RMUORequest r1, RMUORequest r2);

    bool isInList(list<int> l, int i);

};

#endif /* RMUOSCHEDULER_H_ */
