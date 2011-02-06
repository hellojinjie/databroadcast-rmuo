/* 模拟实验配置信息, json 数据格式 */
{
    /* 三个算法都用到的配置信息 */
    "common" : {
        /* 数据库大小 */
        "dbsize" : 2000,
        /* 客户端数量 */
        "clientNumber" : 100,
        /* 每次请求访问的数据项的个数的最小值 */
        "queryItemNumberMin" : 2,
        /* 每次请求访问的数据项的个数的最大值 */
        "queryItemNumberMax" : 6,
        /* 每次请求的周期的最小值 */
        "queryPeriodMin" : 10,
        /* 每次请求的周期的最大值 */
        "queryPeriodMax" : 16,
        /* zipf 分布函数的参数 */
        "theta" : 1
    },
    
    "rmuo" : {
    },
    
    "dtiu" : {
    },
    
    "sin" : {
    }
}
