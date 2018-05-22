add_defines("CROSSPLATFORM_OS_STRING=\"$(plat)\"")
add_defines("CROSSPLATFORM_OS_IS_$(plat:upper)")

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

--std declaration
if not is_os("windows") then
    set_languages("cxx11")
end
add_linkdirs("lib")
add_includedirs("include")
add_files("*.c","*.cpp","*.cxx","*.cc")
add_links("easycrossplatform")
add_defines("USING_STATIC_LIBICONV","NGHTTP2_STATICLIB")
add_links("cryptopp_s","libuv_s","jsoncpp_s","mbedtls_s","whereami_s","brotli_s","iconv_s","websocketpp_s","nghttp2_s")

if(is_plat("windows")) then
	add_links('iphlpapi','psapi','shell32','user32','userenv',"winmm","ws2_32","wldap32","advapi32","Normaliz","Crypt32")
else
	if is_plat("solaris") then
		add_cxflags("-pthreads")
		add_ldflags("-pthreads")
		add_links('kstat','nsl','sendfile','socket')
	else
		add_cxflags("-pthread")
		add_mxflags("-pthread")
		add_ldflags("-pthread")
	end
	if is_plat("linux") then
		add_links("dl","rt")
	end
end

target("program")
    set_kind("binary")
	add_defines("USING_UV_SHARED=1")
	add_links("mariadb")