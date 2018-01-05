--64位Linux Make编译x32 make RELEASE=1 BITS=32
--64位Linux Xmake编译x32 xmake f -a i386
target("easycrossplatform_s")
	set_kind("static")
	add_files("src/**.cpp","src/**.cc","src/**.c")
	
	add_includedirs("src","include","include/extlibs")
	add_linkdirs("lib")
	add_defines("CURL_STATICLIB","SFML_STATIC")
	if(is_plat("windows")) then --Windows, Only x86 and x64
		if is_arch("x86") then
			add_defines("_M_X86","EASYCROSSPLATFORM_ARCHITECTURE_X86")
			add_files("src/asm/x86/**.asm","src/asm/shared/**.asm")
			add_linkdirs("lib/libcurl/Win_x86","lib/mariadb/Win_x86")
		elseif is_arch("x64") then
			add_defines("_M_X64","EASYCROSSPLATFORM_ARCHITECTURE_X64")
			add_files("src/asm/x64/**.asm","src/asm/shared/**.asm")
			add_linkdirs("lib/libcurl/Win_x64","lib/mariadb/Win_x64")
		end
	else --Linux Distribution, xmake only supports x86 and x64
		add_cxxflags("-std=c++11")
		if is_arch("i386") then
			add_defines("X86","EASYCROSSPLATFORM_ARCHITECTURE_X86")
			add_linkdirs("lib/mariadb/Linux_x86")
		elseif is_arch("x86_64") then
			add_defines("X64","EASYCROSSPLATFORM_ARCHITECTURE_X64")
			add_linkdirs("lib/mariadb/Linux_x64")
		--ARMS EASYCROSSPLATFORM_ARCHITECTURE_ARM64 / EASYCROSSPLATFORM_ARCHITECTURE_ARMv5 / EASYCROSSPLATFORM_ARCHITECTURE_ARMv6 / EASYCROSSPLATFORM_ARCHITECTURE_ARMv7 / EASYCROSSPLATFORM_ARCHITECTURE_ARMv8
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
	end
	
	add_links("mariadbclient")
	if(is_plat("windows")) then
		add_links("libcurl_s")
	else
		add_ldflags("-lcurl")
	end
	
target("easycrossplatform")
	set_kind("shared")
	add_files("src/**.cpp","src/**.cc","src/**.c")
	
	add_includedirs("src","include","include/extlibs")
	add_linkdirs("lib")
	
	if(is_plat("windows")) then --Windows, Only x86 and x64
		if is_arch("x86") then
			add_defines("_M_X86","EASYCROSSPLATFORM_ARCHITECTURE_X86")
			add_files("src/asm/x86/**.asm","src/asm/shared/**.asm")
			add_linkdirs("lib/libcurl/Win_x86","lib/mariadb/Win_x86")
		elseif is_arch("x64") then
			add_defines("_M_X64","EASYCROSSPLATFORM_ARCHITECTURE_X64")
			add_files("src/asm/x64/**.asm","src/asm/shared/**.asm")
			add_linkdirs("lib/libcurl/Win_x64","lib/mariadb/Win_x64")
		end
	else --Linux Distribution, xmake only supports x86 and x64
		add_cxxflags("-std=c++11")
		if is_arch("i386") then
			add_defines("X86","EASYCROSSPLATFORM_ARCHITECTURE_X86")
			add_linkdirs("lib/mariadb/Linux_x86")
		elseif is_arch("x86_64") then
			add_defines("X64","EASYCROSSPLATFORM_ARCHITECTURE_X64")
			add_linkdirs("lib/mariadb/Linux_x64")
		--ARMS EASYCROSSPLATFORM_ARCHITECTURE_ARM64 / EASYCROSSPLATFORM_ARCHITECTURE_ARMv5 / EASYCROSSPLATFORM_ARCHITECTURE_ARMv6 / EASYCROSSPLATFORM_ARCHITECTURE_ARMv7 / EASYCROSSPLATFORM_ARCHITECTURE_ARMv8
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
	end
	if(is_plat("windows")) then
		add_links("libmariadb")
		add_links("libcurl")
	else
		add_links("mariadb")
		add_ldflags("-lcurl")
	end
	add_rpathdirs("@loader_path/lib","@loader_path","@loader_path","@executable_path/lib","@executable_path")
