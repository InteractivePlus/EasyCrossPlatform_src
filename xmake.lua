target("easycrossplatform_s")
	set_kind("static")
	add_files("src/**.cpp","src/**.cc","src/**.c")
	
	add_includedirs("src","include")
	add_linkdirs("lib")
	
	if(is_plat("windows")) then --Windows, Only x86 and x64
		if is_arch("x86") then
			add_defines("_M_X86","EASYCROSSPLATFORM_ARCHITECTURE_X86")
			add_files("src/asm/x86/**.asm","src/asm/shared/**.asm")
		elseif is_arch("x64") then
			add_defines("_M_X64","EASYCROSSPLATFORM_ARCHITECTURE_X64")
			add_files("src/asm/x64/**.asm","src/asm/shared/**.asm")
		end
	else --Linux Distribution, xmake only supports x86 and x64
		add_cxxflags("-std=c++11")
		if is_arch("i386") then
			add_defines("X86","EASYCROSSPLATFORM_ARCHITECTURE_X86")
		elseif is_arch("x86_64") then
			add_defines("X64","EASYCROSSPLATFORM_ARCHITECTURE_X64")
		end
	end
	
	add_links("mariadbclient")
	--on_install(function (target)
		
	--end)
target("easycrossplatform")
	set_kind("shared")
	add_files("src/**.cpp","src/**.cc","src/**.c")
	
	add_includedirs("src","include")
	add_linkdirs("lib")
	
	if(is_plat("windows")) then --Windows, Only x86 and x64
		if is_arch("x86") then
			add_defines("_M_X86","EASYCROSSPLATFORM_ARCHITECTURE_X86")
			add_files("src/asm/x86/**.asm","src/asm/shared/**.asm")
		elseif is_arch("x64") then
			add_defines("_M_X64","EASYCROSSPLATFORM_ARCHITECTURE_X64")
			add_files("src/asm/x64/**.asm","src/asm/shared/**.asm")
		end
	else --Linux Distribution, xmake only supports x86 and x64
		add_cxxflags("-std=c++11")
		if is_arch("i386") then
			add_defines("X86","EASYCROSSPLATFORM_ARCHITECTURE_X86")
		elseif is_arch("x86_64") then
			add_defines("X64","EASYCROSSPLATFORM_ARCHITECTURE_X64")
		end
	end
	if(is_plat("windows")) then
		add_links("libmariadb")
	end
	add_rpathdirs("@loader_path/lib","@loader_path","@loader_path","@executable_path/lib","@executable_path")
	
