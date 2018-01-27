target("easycrossplatform_s")
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
	--Source Files
	add_includedirs("include")
	add_subdirs("deps/cryptopp","deps/libuv","deps/jsoncpp")
	add_files("src/*.cpp")
	add_deps("cryptopp_s")
	add_deps("libuv_s")
	add_deps("jsoncpp_s")
	--ExtLibs
	add_defines("CURL_STATICLIB")
	add_linkdirs("extlibs")
	if is_os("windows") then
		add_links("libcurl_s","mariadbclient")
	else
		add_links("curl")
		add_links("mariadbclient")
	end

target("easycrossplatform")
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
	--Source Files
	add_includedirs("include")
	add_subdirs("deps/cryptopp","deps/libuv","deps/jsoncpp")
	add_files("src/*.cpp")
	add_deps("cryptopp_s")
	add_deps("libuv_s")
	add_deps("jsoncpp_s")
	--ExtLibs
	add_linkdirs("extlibs")
	if is_os("windows") then
		add_links("libcurl")
		add_links("libmariadb")
	else
		add_links("curl")
		add_links("mariadb")
	end
	