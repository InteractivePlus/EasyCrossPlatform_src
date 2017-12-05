# EasyCrossPlatform
EasyCrossPlatform源代码, 仅开发用<br />
Language: <a href="README.md">English</a>
## 介绍
为了PHP与Python用户创建的快速开发且跨平台的C++框架<br />
<a href="http://www.xsyds.cn/" target="_blank">形随意动</a> 作品<br />
贡献列表: <a href="https://github.com/ToiletCommander">年迈的老秋风</a><br />
包含库: 多线程池 | 多线程 | 文件读写 | TCP & UDP Sockets 套接字 | MySQL / MariaDB(使用MariaDB C Connector) | <strike>HTTP(s)请求</strike><br />
测试环境: Windows 10 + Visual Studio 2017 | Ubuntu LTS16.04LTS + g++<br />
需要支持: Unix / Linux / Windows系统 + C++11
## 为什么没有用户界面库?
我们曾考虑过使用"MyGUI"库, 但是它包含了太多的依赖且在每个桌面环境中, OPENGL的配置都可以是不一样的. 如果您需要用户界面, 我们推荐使用MyGUI和它的OPENGL2.0实现.
## 开发理念
大家都爱C++, 但是并没有任何好用的库, 来帮助初学者快速上手C++的多线程, 网络, 用户交互开发, 同时享受C++带来的无与伦比的速度与效率优势<br />
受到 <a href="http://reasoning.biz" target="_blank">Reasoning</a> 库的影响, 但是这个库自2008年便不再开发<br />
用EasyCrossPlatform开发, 可以有效的减少代码行数, 意味着更少Bug和溢出漏洞的可能性. EasyCrossPlatform在实现层趋向于将底层代码封装的更简易, 但是仍旧保留不妥协的性能
## 编译
此项目仍在早期开发阶段, 且并没有考虑开始发行, Beta版都不! 不过如果您等不及了, 可以编译此库以使用它.<br />
1) 在Lib/MariaDB文件夹中选择您系统对应的正确版本, 并拷贝到Lib目录下
2) 目录下运行xmake即可生成静态/动态链接库. (注意Windows和Linux的库互不通用, 不同版本的Linux静态库也不通用).
## 贡献
欢迎提交Pull Request. <br />
您也可以通过测试来提交 <br />
注意, 在您提交Pull Request请求前, 请您对您的API进行测试. 谢谢!