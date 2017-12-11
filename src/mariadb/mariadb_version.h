/* Copyright Abandoned 1996, 1999, 2001 MySQL AB
   This file is public domain and comes with NO WARRANTY of any kind */

/* Version numbers for protocol & mysqld */

#ifndef _mariadb_version_h_
#define _mariadb_version_h_

#ifdef _CUSTOMCONFIG_
#include <custom_conf.h>
#else
#define PROTOCOL_VERSION		10
#define MARIADB_CLIENT_VERSION_STR	"10.2.6"
#define MARIADB_VERSION_ID		100206
#define MYSQL_VERSION_ID		100206
#define MARIADB_PORT	        	3306
#define MARIADB_UNIX_ADDR               "/tmp/mysql.sock"
#define MYSQL_CONFIG_NAME		"my"

#define MARIADB_PACKAGE_VERSION "3.0.2"
#if defined(EASYCROSSPLATFORM_PLATFORM_IS_WINDOWS)
	#define MARIADB_SYSTEM_TYPE "Windows"
#elif defined(EASYCROSSPLATFORM_PLATFORM_IS_LINUX)
	#define MARIADB_SYSTEM_TYPE "Linux"
#elif defined(EASYCROSSPLATFORM_PLATFORM_IS_MAC)
	#define MARIADB_SYSTEM_TYPE "Darwin"
#endif
#if defined(EASYCROSSPLATFORM_PLATFORM_WINDOWS)
	#define MARIADB_MACHINE_TYPE "AMD64"
#else
	#if defined(EASYCROSSPLATFORM_ARCHITECTURE_X86)
		#define MARIADB_MACHINE_TYPE "i686"
	#elif defined(EASYCROSSPLATFORM_ARCHITECTURE_X64)
		#define MARIADB_MACHINE_TYPE "x86_64"
	#elif defined(EASYCROSSPLATFORM_ARCHITECTURE_ARM64)
		#define MARIADB_MACHINE_TYPE "aarch64"
	#endif
#endif

#define MARIADB_PLUGINDIR "lib/mariadb/plugin"

/* mysqld compile time options */
#ifndef MYSQL_CHARSET
#define MYSQL_CHARSET			""
#endif
#endif

#endif /* _mariadb_version_h_ */
