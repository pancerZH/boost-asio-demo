# boost-asio-demo

    这个demo是为了局域网联机服务而编写的技术验证性程序

    创建了三个线程用于模拟服务端和客户端的连接

## 服务端

    initServer用于进行广播，模拟周期性发布房间信息
    initReceiver用于接收客户端回传的消息，并且回复一条描述成功建立连接的信息
    注意：udp协议中不存在连接，这里所谓的“成功建立连接”指的是成功进行了一次对话

## 客户端

    initClient负责接收服务端发出的消息，并且对信息进行初步分析：是广播内容还是描述连接成功的信息
    若是广播内容：打印消息并且回复，请求连接，并在接到第一条广播后记录服务端ip
    若是连接成功的消息：打印消息，打印“connected”，并将connected变量置为true

#### 此外

    main函数中的Sleep(5000)是为了模拟客户端在服务端广播开始后一段时间才开始监听

#### 运行截图
    
![image](https://github.com/zwz1551719/boost-asio-demo/raw/master/asio_running.PNG)
