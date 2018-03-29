option("build-on-solaris")
	--执行操作
	set_default(false)
	set_showmenu(true)
option_end()
option("build-on-linux")
	--执行操作
	set_default(false)
	set_showmenu(true)
option_end()
option("build-on-zos")
	--执行操作
	set_default(false)
	set_showmenu(true)
option_end()
option("build-on-freebsd")
	--执行操作
	set_default(false)
	set_showmenu(true)
option_end()
option("build-on-dragonflybsd")
	--执行操作
	set_default(false)
	set_showmenu(true)
option_end()
option("build-on-openbsd")
	--执行操作
	set_default(false)
	set_showmenu(true)
option_end()
option("build-on-netbsd")
	--执行操作
	set_default(false)
	set_showmenu(true)
option_end()

target("program")
	add_options("build-on-linux",
				"build-on-solaris",
				"build-on-zos",
				"build-on-freebsd",
				"build-on-dragonflybsd",
				"build-on-openbsd",
				"build-on-netbsd"
	)
    set_kind("binary")
	add_defines("USING_UV_SHARED=1")	
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
	--STD Declaration
	if (not is_os("windows")) then
		add_cxflags("-std=c++11")
	end
	--Always put the libs that need dependencies in front of their dependencies.
	add_links("easycrossplatform")
	add_links("cryptopp_s","libuv_s","jsoncpp_s","mbedtls_s","whereami_s","brotli_s")
	add_links("mariadb")
	if(is_plat("windows")) then
		add_links('iphlpapi','psapi','shell32','user32','userenv',"winmm","ws2_32","wldap32","advapi32","Normaliz","Crypt32")
	else
		if is_option("build-on-solaris") then
			add_cxflags("-pthreads")
			add_ldflags("-pthreads")
			add_links('kstat','nsl','sendfile','socket')
		else
			add_cxflags("-pthread")
			add_mxflags("-pthread")
			add_ldflags("-pthread")
		end
		if is_option("build-on-linux") then
			add_links("dl","rt")
		end
	end