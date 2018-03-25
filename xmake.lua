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
	--OSType Declaration
	if is_os("windows") then
		add_defines("CROSSPLATFORM_OS_STRING=\"windows\"");
	elseif is_os("linux") then
		add_defines("CROSSPLATFORM_OS_STRING=\"linux\"");
	elseif is_os("macosx") then
		add_defines("CROSSPLATFORM_OS_STRING=\"macosx\"");
	end
	--STD Declaration
	if (not is_os("windows")) then
		add_cxflags("-std=c++11")
	end
	--Source Files
	add_includedirs("include")
	includes("deps/cryptopp","deps/libuv","deps/jsoncpp","deps/mbedtls","deps/wai","deps/brotli")
	add_files("src/*.cpp")
	add_deps("cryptopp_s")
	add_deps("libuv_s")
	add_deps("jsoncpp_s")
	add_deps("mbedtls_s")
	add_deps("whereami_s")
	add_deps("brotli_s")
	--ExtLibs
	add_defines("CURL_STATICLIB")
	add_linkdirs("extlibs")
	add_links("mariadbclient")
	if is_os("windows") then
		add_links("curl_s")
	else
		add_links("curl")
	end

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
	--OSType Declaration
	if is_os("windows") then
		add_defines("CROSSPLATFORM_OS_STRING=\"windows\"");
	elseif is_os("linux") then
		add_defines("CROSSPLATFORM_OS_STRING=\"linux\"");
	elseif is_os("macosx") then
		add_defines("CROSSPLATFORM_OS_STRING=\"macosx\"");
	end
	--STD Declaration
	if (not is_os("windows")) then
		add_cxflags("-std=c++11")
	end
	--Source Files
	add_includedirs("include")
	includes("deps/cryptopp","deps/libuv","deps/jsoncpp","deps/mbedtls","deps/wai","deps/brotli")
	add_files("src/*.cpp")
	add_deps("cryptopp_s")
	add_deps("libuv_s")
	add_deps("jsoncpp_s")
	add_deps("mbedtls_s")
	add_deps("whereami_s")
	add_deps("brotli_s")
	--ExtLibs
	add_linkdirs("extlibs")
	add_links("curl")
	add_links("mariadb")
	