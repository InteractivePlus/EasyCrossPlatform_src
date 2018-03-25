# EasyCrossPlatform
用简单的代码, 写出毫无妥协性能的程序  
[形随意动](https://github.com/BlueAirTechGroup/)旗下项目, 2017年10月15日开始动工  
版本: 1.1Alpha-Dev  
Language: [English](README.md)  
**稳定版本可在Release页面中找到**  
## 介绍
为了PHP与Python用户创建的快速开发且跨平台的C++框架  
支持系统: LINUX / WIN / MacOS  
支持CPU架构: x86(i386) / x64(x86_64) / ARMv5 / ARMv6 / ARMv7 / ARMv8  
[形随意动](https://www.xsyds.cn/)作品  
贡献列表: [年迈的老秋风](https://github.com/ToiletCommander)  
包含库: 多线程池 | 多线程 | 文件读写 | TLS & ~~DTLS~~ & TCP & UDP Sockets 套接字 | MySQL / MariaDB(使用MariaDB C Connector) | 加密库 | 压缩库(Deflate-Gzip, Deflate Zlib, Brotli) | HTTP(s)请求 | JSON | ~~HTTP1.X解析器~~  
需要支持: Unix / Linux / Windows系统 + C++11  
## 毫不妥协的性能
Socket库以Libuv为底层, 开发了跨平台的Socket库.  
根据测试, Release版本在1万并发的情况下延时仅240ms.  
## 依赖
EasyCrossPlatform的发展是由下面几个库驱动的:  
[LibUV](https://github.com/libuv/libuv) - CrossPlatform套接字完全基于此库编写, 我给他写了一层包装库.  
[Crypto++](https://github.com/weidai11/cryptopp) - 加密库, 未来的EasyCrossPlatform加密库会基于此加密库.  
[Crypto++ Examples](https://github.com/sechaser/CryptoPP) - Crypto++ AES例子, EasyCrossPlatform AES 加密库基于此实例搭建  
[MariaDB](https://mariadb.com/downloads/mariadb-tx/connector) - MySQL / MariaDB 连接器.  
[LibCurl](https://curl.haxx.se/) - EasyCrossPlatform的HTTP(s)请求库完全基于此库编写.  
[JSONCPP](https://github.com/henshao/jsoncpp) - 一个魔改版的JSONCPP, 比原版的快速很多, EasyCrossPlatform的JSON解析库完全基于此库编写  
[mbedtls](https://github.com/ARMmbed/mbedtls) - 一个SSL/TLS和其他加密方法的库, 用于编写TLS加密的套接字  
[whereami](https://github.com/gpakosz/whereami) - 两个文件的小型库, 告诉程序它自己在哪  
[Brotli](https://github.com/google/brotli) - 谷歌Brotli压缩算法库  
*以上项目的协议完全保持原样, 请务必参照他们的项目工程页面*  
## 为什么没有用户界面库?
我们曾考虑过使用"MyGUI"库, 但是它包含了太多的依赖且在每个桌面环境中, OPENGL的配置都可以是不一样的. 如果您需要用户界面, 我们推荐使用MyGUI和它的OPENGL2.0实现.  
## 开发理念
*"使用更少的依赖, 使用更少的代码"*  
大家都爱C++, 但是并没有任何好用的库, 来帮助初学者快速上手C++的多线程, 网络, 用户交互开发, 同时享受C++带来的无与伦比的速度与效率优势  
受到 [Reasoning](http://reasoning.biz/) 库的影响, 但是这个库自2008年便不再开发  
用EasyCrossPlatform开发, 可以有效的减少代码行数, 意味着更少Bug和溢出漏洞的可能性. EasyCrossPlatform在实现层趋向于将底层代码封装的更简易, 但是仍旧保留不妥协的性能  
## 编译
此项目仍在早期开发阶段, 且并没有考虑开始发行, Beta版都不! 不过如果您等不及了, 可以编译此库以使用它.  
注意, 目前我们使用的编译工具*xmake不支持在ARM64上进行运行/编译*, 除非您在X86/X64机器上进行交叉编译, 否则您将无法使用ARM64版本的EASYCROSSPLATFORM.  
1) 在/extlibs/MariaDB文件夹中选择您系统对应的正确版本, 并将系统版本文件夹内的文件拷贝到/extlibs目录下
2) a) 如果您在Windows机器上编译, 在extlibs/LibCurl文件夹中选择您系统对应的正确版本, 并将系统版本文件夹的文件拷贝到/extlibs目录下  
   b) 如果您在Linux / Unix / MacOS 上编译, 您需要手动安装LibCurl, 然后您就可以编译EasyCrossPlatform了.  
3) 目录下运行xmake即可生成静态/动态链接库.  
(注意Windows和Linux的库互不通用, 不同版本的Linux静态库也不通用).  
*如果您运行的是Unix / Linux系统, 您需要指定您的Linux系统类型, 请查看Linux_BuildOptions.txt  
## 贡献
欢迎提交Pull Request.   
您也可以通过测试来提交   
注意, 在您提交Pull Request请求前, 请您对您的API进行测试. 谢谢!  