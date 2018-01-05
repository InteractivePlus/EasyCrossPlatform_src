target("program")
    set_kind("binary")
    add_files("**.c","**.cpp","**.cxx","**.cc")
	add_linkdirs("lib")
	add_includedirs("include")
	add_links("mariadbclient")
	add_defines("CURL_STATICLIB","SFML_STATIC")
	if(is_plat("windows")) then
		if is_arch("x86") then
			add_defines("EASYCROSSPLATFORM_ARCHITECTURE_X86")
			add_linkdirs("lib/libcurl/Win_x86","lib/mariadb/Win_x86")
		elseif is_arch("x64") then
			add_defines("EASYCROSSPLATFORM_ARCHITECTURE_X64")
			add_linkdirs("lib/libcurl/Win_x64","lib/mariadb/Win_x64")
		end
		add_links("winmm","ws2_32","wldap32","advapi32","Normaliz","Crypt32","libcurl_s")
	else
		add_cxxflags("-std=c++11")
		if is_arch("i386") then
			add_defines("EASYCROSSPLATFORM_ARCHITECTURE_X86")
			add_linkdirs("lib/mariadb/Linux_x86")
		elseif is_arch("x86_64") then
			add_defines("EASYCROSSPLATFORM_ARCHITECTURE_X64")
			add_linkdirs("lib/mariadb/Linux_x64")
		elseif is_arch("arm64*") then
			add_defines("EASYCROSSPLATFORM_ARCHITECTURE_ARM64")
			add_linkdirs("lib/mariadb/Linux_ARM64")
		elseif is_arch("armv5*") then
			add_defines("EASYCROSSPLATFORM_ARCHITECTURE_ARMv5")
		elseif is_arch("armv6*") then
			add_defines("EASYCROSSPLATFORM_ARCHITECTURE_ARMv6")
			add_linkdirs("lib/mariadb/Linux_ARMv6")
		elseif is_arch("armv7*") then
			add_defines("EASYCROSSPLATFORM_ARCHITECTURE_ARMv7")
		elseif is_arch("armv8*") then
			add_defines("EASYCROSSPLATFORM_ARCHITECTURE_ARMv8")
		end
		if is_arch("arm*") then
			add_defines("EASYCROSSPLATFORM_ARCHITECTURE_ARM")
		end
		add_ldflags("-lcurl")
	end
	add_links("easycrossplatform_s")