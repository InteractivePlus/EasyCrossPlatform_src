# EasyCrossPlatform
Source Code for EasyCrossPlatform, only for development purpose.  
Language: [中文](README-ch.md)  
## Introduction
A easy-to-use cross-platform C++ library designed for PHP and Python Users.  
Supported System: UNIX / LINUX / WIN / MacOS  
Supported Architecture: x86(i386) / x64(x86_64) / ~~ARMv5~~ / ARMv6 / ~~ARMv7~~ / ~~ARMv8~~ /  
Powered by <a href="http://www.xsyds.cn/" target="_blank">BlueAirTechGroup</a>  
Contribution List: <a href="https://github.com/ToiletCommander">Windy</a>  
Libs: MultiThreadPool | MultiThread | FileIO | TCP & UDP Sockets | MySQL / MariaDB (Using MariaDB C Connector) | Encryption | HTTP(s)_Requests | JSON  
Test Env: Windows 10 + XMake + VS2017 | Ubuntu LTS16.04 + XMake + GCC  | RaspBian Stretch Nov 2017 + XMake + GCC | MacOS High Sierra + XMake + XCode + GCC  
100% Support List: Windows(x86, x64) | Ubuntu LTS 16.04(x64) | Raspbian Stretch Nov 2017(ARMv6)  
Require Support: Unix / Linux / Windows System + C++ 11 Support  
## Credit
The development of EasyCrossPlatform is given power by the following lib:  
[CppSocket](https://github.com/itomi/CppSocket) - CrossPlatform Socket Completely built based on it, but I fixed several bugs that affected the compile process, and also fixed several bugs on runtime.  
[Crypto++](https://github.com/weidai11/cryptopp) - Encryption Lib, EasyCrossPlatform Encryption built based on it.  
[Crypto++ Examples](https://github.com/sechaser/CryptoPP) - Crypto++ AES Example, EasyCrossPlatform AES Encryption built based on it.  
[MariaDB](https://mariadb.com/downloads/mariadb-tx/connector) - MySQL / MariaDB Connector Lib.  
[LibCurl](https://curl.haxx.se/) - EasyCrossPlatform Request Lib built based on it.  
## Why not GUI?
GUI requires too much dependencies, and we are considering using "mygui". However, the OpenGL environment in each machine can be different, and if you want to use a GUI Lib, we recommend MyGUI with OpenGL2.0 Implemention  
## Develop Theory
*"To build with less dependency, to code with less line"*  
Everybody Loves C++, but there is actually no user-friendly libs for beginners to start using C++ with full support of network and multithreading function, while covering the hardware and underlying technologies.   
Influenced by <a href="http://reasoning.biz" target="_blank">Reasoning</a> Library, however, that library is no longer maintained since 2008  
With less code, developers can develop more safe code preventing memory leaks and debug less. EasyCrossPlatform also brings noticeable performance while enabling developers to develop codes fast and secure.  
## Build
This project is not even released with beta and it is still in its early stage of development.  
Windows .LIB / .DLL Files will be released whenever a major version is comming out under [EasyCrossPlatform_release](https://github.com/EasyCrossPlatformLib/EasyCrossPlatform_release).  
If you cannot wait to see it, 
1) Enter the lib/mariadb directory and put the correct Files in the lib directory  
2) a) If you are building under Windows, Enter the lib/libcurl directory and put the correct Files in the lib directory  
   b) If you are building under Linux / Unix / MacOS, Simply Install LibCurl and you should be good to go.  
2) simply run xmake in the directory to generate a static/shared library that you can use.  
Remeber that the lib file generated is not cross-platform betweeen Win and Linux, and between different versions of Linux.  
You need to generate lib file each time when you are switching platforms.  
## Contribute
You can fork this project and request a Pull Request.   
You can also help improve this project by testing.   
Remember that you need to test your API first before you submit the request. Thank you!  