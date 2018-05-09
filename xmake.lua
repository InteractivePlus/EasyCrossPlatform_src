option("build-on-solaris")
	--执行操作
	add_defines("CROSSPLATFORM_OS_STRING=\"solaris\"")
	add_defines("CROSSPLATFORM_OS_IS_SOLARIS")
	set_default(false)
	set_showmenu(true)
option_end()
option("build-on-zos")
	--执行操作
	add_defines("CROSSPLATFORM_OS_STRING=\"zos\"")
	add_defines("CROSSPLATFORM_OS_IS_ZOS")
	set_default(false)
	set_showmenu(true)
option_end()
option("build-on-freebsd")
	--执行操作
	add_defines("CROSSPLATFORM_OS_STRING=\"freebsd\"")
	add_defines("CROSSPLATFORM_OS_IS_FREEBSD")
	set_default(false)
	set_showmenu(true)
option_end()
option("build-on-dragonflybsd")
	--执行操作
	add_defines("CROSSPLATFORM_OS_STRING=\"dragonflybsd\"")
	add_defines("CROSSPLATFORM_OS_IS_DRAGONFLYBSD")
	set_default(false)
	set_showmenu(true)
option_end()
option("build-on-openbsd")
	--执行操作
	add_defines("CROSSPLATFORM_OS_STRING=\"openbsd\"")
	add_defines("CROSSPLATFORM_OS_IS_OPENBSD")
	set_default(false)
	set_showmenu(true)
option_end()
option("build-on-netbsd")
	--执行操作
	add_defines("CROSSPLATFORM_OS_STRING=\"netbsd\"")
	add_defines("CROSSPLATFORM_OS_IS_NETBSD")
	set_default(false)
	set_showmenu(true)
option_end()
option("build-on-linux")
	--执行操作
	--add_defines("CROSSPLATFORM_OS_STRING=\"linux\"")
	--add_defines("CROSSPLATFORM_OS_IS_UNIX")
	set_default(false)
	set_showmenu(true)
option_end()

target("easycrossplatform_s")
	add_options("build-on-linux",
				"build-on-solaris",
				"build-on-zos",
				"build-on-freebsd",
				"build-on-dragonflybsd",
				"build-on-openbsd",
				"build-on-netbsd"
	)
	set_kind("static")
	--OS Arch Declaration
	if is_arch("i386") or is_arch("x86") then
		add_defines("EASYCROSSPLATFORM_ARCHITECTURE_X86")
	elseif is_arch("x86_64") or is_arch("x64") then
		add_defines("EASYCROSSPLATFORM_ARCHITECTURE_X64")
	elseif is_arch("arm64*") then
		add_defines("EASYCROSSPLATFORM_ARCHITECTURE_ARM64")
	elseif is_arch("armv5*") then
		add_defines("EASYCROSSPLATFORM_ARCHITECTURE_ARMv5")
	elseif is_arch("armv6*") then
		add_defines("EASYCROSSPLATFORM_ARCHITECTURE_ARMv6")
	elseif is_arch("armv7*") then
		add_defines("EASYCROSSPLATFORM_ARCHITECTURE_ARMv7")
	elseif is_arch("armv8*") then
		add_defines("EASYCROSSPLATFORM_ARCHITECTURE_ARMv8")
	end
	--STD Declaration
	if (not is_os("windows")) then
		add_cxxflags("-std=c++11")
	end
	--Source Files
	add_includedirs("include")
	includes("deps/cryptopp","deps/libuv","deps/jsoncpp","deps/mbedtls","deps/wai","deps/brotli","deps/iconv")
	add_files("src/*.cpp")
	add_deps("cryptopp_s")
	add_deps("libuv_s")
	add_deps("jsoncpp_s")
	add_deps("mbedtls_s")
	add_deps("whereami_s")
	add_deps("brotli_s")
	add_defines("USING_STATIC_LIBICONV")
	add_deps("iconv_s")
	--ExtLibs
	add_linkdirs("extlibs")
	add_links("mariadbclient")

target("easycrossplatform")
	add_options("build-on-linux",
				"build-on-solaris",
				"build-on-zos",
				"build-on-freebsd",
				"build-on-dragonflybsd",
				"build-on-openbsd",
				"build-on-netbsd"
	)
	set_kind("shared")
	--OS Arch Declaration
	if is_arch("i386") or is_arch("x86") then
		add_defines("EASYCROSSPLATFORM_ARCHITECTURE_X86")
	elseif is_arch("x86_64") or is_arch("x64") then
		add_defines("EASYCROSSPLATFORM_ARCHITECTURE_X64")
	elseif is_arch("arm64*") then
		add_defines("EASYCROSSPLATFORM_ARCHITECTURE_ARM64")
	elseif is_arch("armv5*") then
		add_defines("EASYCROSSPLATFORM_ARCHITECTURE_ARMv5")
	elseif is_arch("armv6*") then
		add_defines("EASYCROSSPLATFORM_ARCHITECTURE_ARMv6")
	elseif is_arch("armv7*") then
		add_defines("EASYCROSSPLATFORM_ARCHITECTURE_ARMv7")
	elseif is_arch("armv8*") then
		add_defines("EASYCROSSPLATFORM_ARCHITECTURE_ARMv8")
	end
	--STD Declaration
	if (not is_os("windows")) then
		add_cxxflags("-std=c++11")
	end
	--Source Files
	add_includedirs("include")
	includes("deps/cryptopp","deps/libuv","deps/jsoncpp","deps/mbedtls","deps/wai","deps/brotli","deps/iconv")
	add_files("src/*.cpp")
	add_deps("cryptopp_s")
	add_deps("libuv_s")
	add_deps("jsoncpp_s")
	add_deps("mbedtls_s")
	add_deps("whereami_s")
	add_deps("brotli_s")
	add_defines("USING_STATIC_LIBICONV")
	add_deps("iconv_s")
	
	--ExtLibs
	add_linkdirs("extlibs")
	add_links("mariadb")
	