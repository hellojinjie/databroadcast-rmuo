/*
 * DTIUScheduler.h
 *
 *  Created on: 2011-1-15
 */

#ifndef DTIUSCHEDULER_H_
#define DTIUSCHEDULER_H_

#include "../Scheduler.h"
#include <list>

using namespace std;

class DTIUScheduler: public Scheduler
{
public:
    DTIUScheduler(Server *server);
    virtual ~DTIUScheduler();

    void doSchedule();

private:

    /** SimpleRequest.readSet 为该 request的UnserveredSet */
    list<SimpleRequest> scheduleQueue;

    /** 对队列检查有没有请求错过截止期，返回错过截止期的请求的个数  */
    int checkDeadline();

    /** 对pendingQueue 进行截止期检查，返回错过截止期的请求的个数 */
    int checkDeadline(list<SimpleRequest> &pendingQueue);

    /** 对 pendingQueue 中的请求进行预处理，并将里面的请求放入scheduleQueue */
    void preprocess();

    /** 根据算法选取一个请求中的所有数据项发送 */
    SimpleRequest broadcast();

    /** 更新 UnserveredSet  */
    void postprocess(SimpleRequest request);

    /** 计算指定的请求的优先级 */
    double calculatePriority(SimpleRequest &request);
};

#endif /* DTIUSCHEDULER_H_ */
