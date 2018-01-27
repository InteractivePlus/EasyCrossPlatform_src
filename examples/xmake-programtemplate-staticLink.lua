target("program")
    set_kind("binary")
	add_defines("CURL_STATICLIB")
	--add_defines("USING_UV_SHARED=1")	
    add_files("**.c","**.cpp","**.cxx","**.cc")
	add_linkdirs("lib")
	add_includedirs("include")
	
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
	
	
	add_links("cryptopp_s","libuv_s","jsoncpp_s")
	add_links("mariadbclient")
	if(is_plat("windows")) then
		add_links("libcurl_s")
		add_links('iphlpapi','psapi','shell32','user32','userenv',"winmm","ws2_32","wldap32","advapi32","Normaliz","Crypt32")
	else
		if is_option("build-on-solaris") then
			add_ldflags("-pthreads")
			add_links('kstat','nsl','sendfile','socket')
		else
			add_ldflags("-pthread")
		end
		if is_option("build-on-linux") then
			add_links("dl","rt")
		end
		add_links("curl")
	end
	add_links("easycrossplatform_s")