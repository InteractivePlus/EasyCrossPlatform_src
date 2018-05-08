# EasyCrossPlatform
Write Easy Code with uncompromising performances.  
A project under [BlueAirTechGroup](https://github.com/BlueAirTechGroup), started from 15 Oct 2017  
Version: 1.1Gama-DevF(F-Nearly Final Version)  
Language: [中文](README-ch.md)  
**You can find stable release under release page**  
## Introduction
A easy-to-use cross-platform C++ library designed for PHP and Python Users.  
Supported System: LINUX / WIN / MacOS  
Supported Architecture: x86(i386) / x64(x86_64) / ARMv5 / ARMv6 / ARMv7 / ARMv8  
**We did not test on Big Endian Machines, please be noticed that Socket might not work according to expectation when communicating between an Big-Endian and Small-Endian Machines**  
*Luckily, our modern CPUs are almost ALL Small-Endian*  
Powered by [BlueAirTechGroup](https://www.xsyds.cn/)  
Contribution List: [Windy](https://github.com/ToiletCommander)  
Libs: MultiThreadPool | MultiThread | FileIO | TLS & ~~DTLS~~ & TCP & UDP Sockets | MySQL / MariaDB (Using MariaDB C Connector) | Encryption | Compression(Deflate-Gzip, Deflate-Zlib, Brotli) | HTTP(s)_Requests | JSON | HTTP1.X Parser | International Charsets | Regex Expression Match/Search/Replace  
Require Support: MacOSX / Linux / Windows System and C++ 11 Support  
## Uncompromising Performances?
Right, we use LibUV as our basis for our socket class.  
As benchmark showed, on release version, the socket Delay is 240ms with 10k connections.  
## Credit
The development of EasyCrossPlatform is given power by the following lib:  
[LibUV](https://github.com/libuv/libuv) - CrossPlatform Socket Completely built based on it, and I made an wrapper to make programming with LibUV easier.  
[Crypto++](https://github.com/weidai11/cryptopp) - Encryption Lib, EasyCrossPlatform Encryption built based on it.  
[Crypto++ Examples](https://github.com/sechaser/CryptoPP) - Crypto++ AES Example, EasyCrossPlatform AES Encryption built based on it.  
[MariaDB](https://mariadb.com/downloads/mariadb-tx/connector) - MySQL / MariaDB Connector Lib.  
[JSONCPP](https://github.com/henshao/jsoncpp) - A quicker version of JSONCPP, a JSON parser, EasyCrossPlatform JSON class completely built based on it.  
[mbedtls](https://github.com/ARMmbed/mbedtls) - A encryption and TLS/SSL lib for building TLS-connected sockets.  
[whereami](https://github.com/gpakosz/whereami) - A two-files lib which tells the program where it is.  
[brotli](https://github.com/google/brotli) - Google Brotli Compression Lib  
*Those projects remain in their own licenses, please refer to their project site.*  
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
1. You need to install [XMake](http://xmake.io/), the build tool that we use to simplify our building process.
2. Enter the /extlibs/mariadb directory and put the correct Files in the /lib directory  
	- e.g. 
	- Copy extlibs/mariadb/Win_x86/mariadbclient.lib to extlibs/mariadbclient.lib
	- Copy extlibs/mariadb/Win_x86/mariadb.lib to extlibs/mariadb.lib
	- Copy extlibs/mariadb/Win_x86/mariadb.dll to extlibs/mariadb.dll
3. If you are 
	1) Either of the following Linux system's user, you need to run either of following command(depends on your system)  

```
xmake f --build-on-solaris=y
xmake f --build-on-zos=y
xmake f --build-on-freebsd=y
xmake f --build-on-dragonflybsd=y
xmake f --build-on-openbsd=y
xmake f --build-on-netbsd=y
```

	2) Any Other Linux User(not including Mac)

```
xmake f --build-on-linux=y
```

4. simply run xmake in the directory to generate a static/shared library that you can use.  
Remeber that the lib file generated is not cross-platform betweeen Win and Linux, and between different versions of Linux.  
You need to generate lib file each time when you are switching platforms.  
## Contribute
You can fork this project and request a Pull Request.   
You can also help improve this project by testing.   
Remember that you need to test your API first before you submit the request. Thank you!  