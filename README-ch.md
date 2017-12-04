# EasyCrossPlatform
EasyCrossPlatform源代码, 仅开发用
Language: <a href="README.md">English</a>
## 介绍
为了PHP与Python用户创建的快速开发且跨平台的C++框架<br />
<a href="http://www.xsyds.cn/" target="_blank">形随意动</a> 作品<br />
贡献列表: <a href="https://github.com/ToiletCommander">年迈的老秋风</a><br />
包含库: 多线程池 | 多线程 | 文件读写 | TCP & UDP Sockets 套接字 | <strike>HTTP(s)请求</strike> | <strike>用户界面</strike><br />
测试环境: Windows 10 + Visual Studio 2017 | Ubuntu LTS16.04LTS + g++<br />
需要支持: Unix / Windows System | C++ 11
## 开发理念
大家都爱C++, 但是并没有任何好用的库, 来帮助初学者快速上手C++的多线程, 网络, 用户交互开发, 同时享受C++带来的无与伦比的速度与效率优势<br />
受到 <a href="http://reasoning.biz" target="_blank">Reasoning</a> 库的影响, 但是这个库自2008年便不再开发<br />
用EasyCrossPlatform开发, 可以有效的减少代码行数, 意味着更少Bug和溢出漏洞的可能性. EasyCrossPlatform在实现层趋向于将底层代码封装的更简易, 但是仍旧保留不妥协的性能<br />
## 编译
此项目仍在早期开发阶段, 且并没有考虑开始发行, Beta版都不! 不过如果您等不及了, 可以编译此库以使用它.
直接在文件夹内运行xmake即可生成静态链接库. (注意Windows和Linux的库互不通用, 不同版本的Linux静态库也不通用).