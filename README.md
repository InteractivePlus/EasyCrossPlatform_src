# EasyCrossPlatform
Write Easy Code with uncompromising performances.  
Version: 1.0Alpha  
Language: [中文](README-ch.md)  
## Introduction
A easy-to-use cross-platform C++ library designed for PHP and Python Users.  
Supported System: LINUX / WIN / MacOS  
Supported Architecture: x86(i386) / x64(x86_64) / ARMv5 / ARMv6 / ARMv7 / ARMv8  
Powered by [BlueAirTechGroup](https://www.xsyds.cn/)  
Contribution List: [Windy](https://github.com/ToiletCommander)  
Libs: MultiThreadPool | MultiThread | FileIO | TCP & UDP Sockets | MySQL / MariaDB (Using MariaDB C Connector) | Encryption | HTTP(s)_Requests | JSON  
Require Support: MacOSX / Linux / Windows System and C++ 11 Support  
## Uncompromising Performances?
Right, we use LibUV as our basis for our socket class.  
As benchmark showed, on release version, the socket Delay is 240ms with 1W connections.  
## Credit
The development of EasyCrossPlatform is given power by the following lib:  
[LibUV](https://github.com/libuv/libuv) - CrossPlatform Socket Completely built based on it, and I made an wrapper to make programming with LibUV easier.  
[Crypto++](https://github.com/weidai11/cryptopp) - Encryption Lib, EasyCrossPlatform Encryption built based on it.  
[Crypto++ Examples](https://github.com/sechaser/CryptoPP) - Crypto++ AES Example, EasyCrossPlatform AES Encryption built based on it.  
[MariaDB](https://mariadb.com/downloads/mariadb-tx/connector) - MySQL / MariaDB Connector Lib.  
[LibCurl](https://curl.haxx.se/) - EasyCrossPlatform Request Lib built based on it.  
[JSONCPP](https://github.com/henshao/jsoncpp) - A quicker version of JSONCPP, a JSON parser, EasyCrossPlatform JSON class completely built based on it.  
## Why not GUI?
GUI requires too much dependencies, and we are considering using "mygui". However, the OpenGL environment in each machine can be different, and if you want to use a GUI Lib, we recommend MyGUI with OpenGL2.0 Implemention  
## Develop Theory
*"To build with less dependency, to code with less line"*  
Everybody Loves C++, but there is actually no user-friendly libs for beginners to start using C++ with full support of network and multithreading function, while covering the hardware and underlying technologies.   
Influenced by [Reasoning](http://reasoning.biz) Library, however, that library is no longer maintained since 2008  
With less code, developers can develop more safe code preventing memory leaks and debug less. EasyCrossPlatform also brings noticeable performance while enabling developers to develop codes fast and secure.  
## Build
This project is not even released with beta and it is still in its early stage of development.  
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