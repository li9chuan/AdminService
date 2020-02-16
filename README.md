# C++服务管理工具&cmd

**管理C++服务，以及远程命令执行。包含AdminModule(lib,用于目标进程)，AdminService(web交互)，web(前端)三个工程。**
github： https://github.com/li9chuan/AdminService
DEMO： http://admin.mulanshanzhuang.com

在原有进程中包含AdminModule库，初始化后即可连接AdminService，windows可以启动以下批处理测试服务：
```cpp
AdminModule\code\test\app\start_connect_test_svr.bat
```
![img](http://imgs.mulanshanzhuang.com/admin/admin_web_exec.jpg)


## 一、定义命令&连接管理器

##### 1. 项目需要包含AdminModule生成的静态库(nelmisc nelnet)，增加.h包含目录 AdminModule\code\nel\include\\*
##### 2. #include "nel/net/admin_module.h"
##### 3. 初始化服务管理模块
```cpp
AdminModule.initAdmin( AdminService host:port, 片区, 进程类型, SvrID );
```
##### 4. 主循环中调用update
```cpp
AdminModule.updateAdmin();
```
##### 5. 结束前调用release
```cpp
AdminModule.releaseAdmin();
```
##### 6. 定义Command
```cpp
// I want to create a function that computes the square of the parameter and display the result
NLMISC_COMMAND(square,"display the square of the parameter","<value>")
{
	// check args, if there s not the right number of parameter, return bad
	if(args.size() != 1) return false;
	// get the value
	uint32 val;
	fromString(args[0], val);
	// display the result on the displayer
	log.displayNL("The square of %d is %d", val, val*val);
	return true;
}
```
##### 7. 进程连接AdminService后，通过web端调用Command
![img](http://imgs.mulanshanzhuang.com/admin/admin_web_square.jpg)

---
## 二、AdminService编译
### 1.AdminService linux编译,依赖protobuf3,libevent,openssl,mysql,zlib
```shell
pwd
/xxxx/xxxx/AdminService/code
cd ..
mkdir build
cd build
cmake ../code
make
```
![img](http://imgs.mulanshanzhuang.com/admin/admin_serv_make.jpg)
```shell
make install
```
![img](http://imgs.mulanshanzhuang.com/admin/admin_serv_makeinstall.jpg)

### 2.AdminService win编译
创建build目录，解压 external.7z，external中是vs2012依赖库，目录结构如下：
```
AdminService\build
AdminService\code
AdminService\external
```
使用CMake构建vs2012工程
![img](http://imgs.mulanshanzhuang.com/admin/admin_serv_cmake.jpg)

工程生成在build目录，Open Project:
![img](http://imgs.mulanshanzhuang.com/admin/admin_serv_vs.jpg)
## 三、web端

使用vue.js ，依赖工具
```
nodejs                  // https://nodejs.org/en/
yarn                    // npm install -g yarn
                        // yarn config set registry https://registry.npm.taobao.org
@vue-cli                // yarn global add @vue/cli
```
安装后，进入目录 ./AdminService/AdminWeb/ 输入yarn install 更新项目的依赖库。
开始运行中输入 vue ui 启动控制台，加载项目AdminWeb。

修改连接到的AdminService：
```
Vue.use(VueNativeSock, 'ws://admin.mulanshanzhuang.com:10390',
```

## 四、代码索引

AdminModuel
```
void sendMsg ( const CMessage& );   // 发消息到AdminService
void addCallbackArray (const TCallbackItem *callbackarray, sint arraysize); // 回调
```

AdminService
```
AdminService\tools\protobuf             // 与前端通信的proto目录
AdminService\code\EVA\server\msg.xml    // web消息过滤&proto反射
m_CallbackAdminModule.addCallbackArray  // AdminModule回调注册
m_CallbackWebSocket.addCallback         // web回调注册
```

web
```
switch(json['msgtype'])                                // 收消息处理
this.$socket.send( JSON.stringify(MsgExecCommand) );   // .vue 发送消息到AdminService
```




