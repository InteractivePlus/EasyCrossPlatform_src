target("program")
    set_kind("binary")
    add_files("**.c","**.cpp","**.cxx"）
	add_linkdirs("lib")
	add_links("easycrossplatform")
	add_includedirs("include","include/extlibs")
	if(is_plat("windows")) then
		if is_arch("x86") then
			add_defines("EASYCROSSPLATFORM_ARCHITECTURE_X86")
		elseif is_arch("x64") then
			add_defines("EASYCROSSPLATFORM_ARCHITECTURE_X64")
		end
	else
		add_cxxflags("-std=c++11")
		if is_arch("i386") then
			add_defines("EASYCROSSPLATFORM_ARCHITECTURE_X86")
		elseif is_arch("x86_64") then
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
		if is_arch("arm*") then
			add_defines("EASYCROSSPLATFORM_ARCHITECTURE_ARM")
		end
	end