/* 第一行的文字在程序里不被读取 */
/* 模拟实验配置信息, json 数据格式 */
{
    /* 通用的默认配置信息 */
    "common" : {
        /* 数据库大小 */
        "dbsize" : 6000,
        /* 总的时槽数 */
        "totalSlot" : 10000,
        /* 客户端数量 */
        "clientNumber" : 10,
        /* 每次请求访问的数据项的个数的最小值 */
        "queryItemNumberMin" : 2,
        /* 每次请求访问的数据项的个数的最大值 */
        "queryItemNumberMax" : 20,
        /* 每次请求的周期的最小值 */
        "queryPeriodMin" : 30,
        /* 每次请求的周期的最大值 */
        "queryPeriodMax" : 300,
        /* zipf 分布函数的参数 */
        "theta" : 1,
        /* 随机数种子 , 0 表示用当前时间   //1299315439 1299315465*/
        "seed" : 1299315439
    },
    
    "requestDeadlineMissRatio" : {
        "resultOutputFilename" : "requestDeadlineMissRatio.result.txt",
        "enable" : false,
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
    },
    
    "utilizationAndDeadlineMissRatio" : {
        "resultOutputFilename" : "utilizationAndDeadlineMissRatio.result.txt",
        "enable" : true,
        "config" : {
            /* 客户端的数量的最小值 */
            "clientNumberMin" : 10,
            /* 客户端数量的最大值 */
            "clientNumberMax" : 200,
            /* 客户端数量增加步长 */
            "clientNumberIncreaseStep" : 5
        }
    }
}
