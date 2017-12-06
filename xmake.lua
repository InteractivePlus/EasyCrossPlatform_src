target("easycrossplatform_s")
	set_kind("static")
	add_files("src/**.cpp","src/**.c")
	if (is_plat("windows")) then
		add_files("src/**.asm")
	else
		--add_files("src/**.S")
	end
	add_includedirs("src")
	add_linkdirs("lib")
	add_links("mariadbclient")
	
	if(is_plat("windows")) then --Windows, Only x86 and x64
		if is_arch("x86") then
			add_defines("_M_X86","EASYCROSSPLATFORM_ARCHITECTURE_X86")
		elseif is_arch("x64") then
			add_defines("_M_X64","EASYCROSSPLATFORM_ARCHITECTURE_X64")
		end
	else --Linux Distribution, xmake only supports x86 and x64
		if is_arch("i386") then
			add_defines("X86","EASYCROSSPLATFORM_ARCHITECTURE_X86")
		elseif is_arch("x86_64") then
			add_defines("X64","EASYCROSSPLATFORM_ARCHITECTURE_X64")
		end
	end
	--nasm -f elf32 rdrand.S -DX86 -g -o rdrand-x86.o
	--nasm -f elfx32 rdrand.S -DX32 -g -o rdrand-x32.o 这个是Linux X64用的X32文件
	--nasm -f elf64 rdrand.S -DX64 -g -o rdrand-x64.o
	
	--on_install(function (target)
		
	--end)
target("easycrossplatform")
	set_kind("shared")
	add_files("src/**.cpp","src/**.c")
	if (is_plat("windows")) then
		add_files("src/**.asm")
	else
		--add_files("src/**.S")
	end
	add_includedirs("src")
	add_linkdirs("lib")
	add_links("libmariadb")
	if(is_plat("windows")) then --Windows, Only x86 and x64
		if is_arch("x86") then
			add_defines("_M_X86","EASYCROSSPLATFORM_ARCHITECTURE_X86")
		elseif is_arch("x64") then
			add_defines("_M_X64","EASYCROSSPLATFORM_ARCHITECTURE_X64")
		end
	else --Linux Distribution, xmake only supports x86 and x64
		if is_arch("i386") then
			add_defines("X86","EASYCROSSPLATFORM_ARCHITECTURE_X86")
		elseif is_arch("x86_64") then
			add_defines("X64","EASYCROSSPLATFORM_ARCHITECTURE_X64")
		end
	end
	