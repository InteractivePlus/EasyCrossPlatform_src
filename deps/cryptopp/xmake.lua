add_includedirs("include")
add_files("*.cpp")

if(is_plat("windows")) then --Windows, Only x86 and x64
	if is_arch("x86") then
		add_defines("_M_X86","EASYCROSSPLATFORM_ARCHITECTURE_X86")
	elseif is_arch("x64") then
		add_defines("_M_X64","EASYCROSSPLATFORM_ARCHITECTURE_X64")
		add_files("asm/x64/*.asm")
	end
	add_files("asm/shared/*.asm")
	add_links("ws2_32")
end
target("cryptopp_s")
	set_kind("static")
	
target("cryptopp")
	set_kind("shared")