/* 第一行的文字在程序里不被读取 */
/* 模拟实验配置信息, json 数据格式 */
{
    /* 通用的默认配置信息 */
    "common" : {
        /* 数据库大小 */
        "dbsize" : 200,
        /* 总的时槽数 */
        "totalSlot" : 3000,
        /* 客户端数量 */
        "clientNumber" : 10,
        /* 每次请求访问的数据项的个数的最小值 */
        "queryItemNumberMin" : 2,
        /* 每次请求访问的数据项的个数的最大值 */
        "queryItemNumberMax" : 6,
        /* 每次请求的周期的最小值 */
        "queryPeriodMin" : 10,
        /* 每次请求的周期的最大值 */
        "queryPeriodMax" : 40,
        /* zipf 分布函数的参数 */
        "theta" : 0.8,
        /* 随机数种子 , 0 表示用当前时间 */
        "seed" : 1297909173
    },
    
    "requestDeadlineMissRatio" : {
        "resultOutputFilename" : "result/requestDeadlineMissRatio.txt",
        "config" : [ {
            "queryItemNumberMin" : 1,
            "queryItemNumberMax" : 1
        }, {
            "queryItemNumberMin" : 1,
            "queryItemNumberMax" : 3
        }, {
            "queryItemNumberMin" : 3,
            "queryItemNumberMax" : 5
        }, {
            "queryItemNumberMin" : 5,
            "queryItemNumberMax" : 7
        }, {
            "queryItemNumberMin" : 7,
            "queryItemNumberMax" : 9
        } ]
    }
}
