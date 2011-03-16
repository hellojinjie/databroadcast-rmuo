程序目录结构：
│  README.txt       # 本说明文档
│  run.bat          # 程序的运行脚本
│  simulation.conf.js   # 程序的配置文件,json 数据格式
│  
├─Debug            # 程序的二进制文件
│              
├─doc              # 存放文档的目录
│  │  half_done.xls               # 由实验数据画的图表
│  │  image001.gif
│  │  image002.gif
│  │  数据广播模拟实验设计.doc    # 本实验的设计文档
│  │  
│  └─half_done.files             # execl 另存为网页，用于取出 execl 里的图片
│          
├─lib              # 程序用到的外部类库
│  │  libgcc_s_dw2-1.dll
│  │  libstdc++-6.dll
│  │  
│  └─jsoncpp
│              
├─log       # 调试用的日志信息
│      
└─src       # 源代码目录
    │  Configure.cpp
    │  Configure.h
    │  main.cpp
    │  MobileClient.cpp
    │  MobileClient.h
    │  Scheduler.cpp
    │  Scheduler.h
    │  Server.cpp
    │  Server.h
    │  SimpleRequest.h
    │  Statistics.cpp
    │  Statistics.h
    │  util.cpp
    │  util.h
    │  
    ├─dtiu
    │      DTIUScheduler.cpp
    │      DTIUScheduler.h
    │      
    ├─rmuo
    │      RMUOClient.cpp
    │      RMUOClient.h
    │      RMUORequest.h
    │      RMUOScheduler.cpp
    │      RMUOScheduler.h
    │      
    └─sin
            SINScheduler.cpp
            SINScheduler.h
            
