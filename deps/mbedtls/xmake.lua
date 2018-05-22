add_files("*.c")
add_includedirs("mbedtls")

if(is_plat("windows")) then --Windows, Only x86 and x64
	add_links("ws2_32","Crypt32","advapi32")
end
target("mbedtls_s")
	set_kind("static")
	
target("mbedtls")
	set_kind("shared")