# EasyCrossPlatform
EasyCrossPlatform源代码, 仅开发用  
Language: [English](README.md)
## 介绍
为了PHP与Python用户创建的快速开发且跨平台的C++框架  
支持系统: UNIX / LINUX / WIN / MacOS(理论支持) / iOS(可能支持)  
支持CPU架构: x86(i386) / x64(x86_64) / ~~ARM64~~  
<a href="http://www.xsyds.cn/" target="_blank">形随意动</a> 作品  
贡献列表: <a href="https://github.com/ToiletCommander">年迈的老秋风</a>  
包含库: 多线程池 | 多线程 | 文件读写 | TCP & UDP Sockets 套接字 | MySQL / MariaDB(使用MariaDB C Connector) | 加密库 | HTTP(s)请求  
测试环境: Windows 10 + XMake + VS2017 | Ubuntu LTS16.04LTS + XMake + GCC  | RaspBian Stretch Nov 2017 + XMake + GCC | MacOS High Sierra + XMake + XCode + GCC  
需要支持: Unix / Linux / Windows系统 + C++11  
## 依赖
EasyCrossPlatform的发展是由下面几个库驱动的:  
[CppSocket](https://github.com/itomi/CppSocket) - CrossPlatform套接字完全基于此库编写, 但是我对它部分强烈影响编译/运行结果的Bug予以了修复.  
[Crypto++](https://github.com/weidai11/cryptopp) - 加密库, 未来的EasyCrossPlatform加密库会基于此加密库.  
[Crypto++ Examples](https://github.com/sechaser/CryptoPP) - Crypto++ AES例子, EasyCrossPlatform AES 加密库基于此实例搭建  
[MariaDB](https://mariadb.com/downloads/mariadb-tx/connector) - MySQL / MariaDB 连接器.  
[LibCurl](https://curl.haxx.se/) - EasyCrossPlatform的HTTP(s)请求库完全基于此库编写.  
## 为什么没有用户界面库?
我们曾考虑过使用"MyGUI"库, 但是它包含了太多的依赖且在每个桌面环境中, OPENGL的配置都可以是不一样的. 如果您需要用户界面, 我们推荐使用MyGUI和它的OPENGL2.0实现.  
## 开发理念
*"使用更少的依赖, 使用更少的代码"*  
大家都爱C++, 但是并没有任何好用的库, 来帮助初学者快速上手C++的多线程, 网络, 用户交互开发, 同时享受C++带来的无与伦比的速度与效率优势  
受到 <a href="http://reasoning.biz" target="_blank">Reasoning</a> 库的影响, 但是这个库自2008年便不再开发  
用EasyCrossPlatform开发, 可以有效的减少代码行数, 意味着更少Bug和溢出漏洞的可能性. EasyCrossPlatform在实现层趋向于将底层代码封装的更简易, 但是仍旧保留不妥协的性能  
## 编译
此项目仍在早期开发阶段, 且并没有考虑开始发行, Beta版都不! 不过如果您等不及了, 可以编译此库以使用它.  
注意, 目前我们使用的编译工具*xmake不支持在ARM64上进行运行/编译*, 除非您在X86/X64机器上进行交叉编译, 否则您将无法使用ARM64版本的EASYCROSSPLATFORM.  
1) 在Lib/MariaDB文件夹中选择您系统对应的正确版本, 并拷贝到Lib目录下
2) 目录下运行xmake即可生成静态/动态链接库. (注意Windows和Linux的库互不通用, 不同版本的Linux静态库也不通用).  
## 贡献
欢迎提交Pull Request.   
您也可以通过测试来提交   
注意, 在您提交Pull Request请求前, 请您对您的API进行测试. 谢谢!  