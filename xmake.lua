-- xmake f -p solaris
-- xmake f -p zos
-- xmake f -p freebsd
-- xmake f -p dragonflybsd
-- xmake f -p openbsd
-- xmake f -p netbsd
-- xmake f -p unix
--
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
	
-- add link and include directories
add_includedirs("include")
add_linkdirs("extlibs")
	
-- includes
add_defines("USING_STATIC_LIBICONV","NGHTTP2_STATICLIB")
includes("deps/cryptopp","deps/libuv","deps/jsoncpp","deps/mbedtls","deps/wai","deps/brotli","deps/iconv","deps/websocketpp","deps/nghttp2")

-- define target
target("easycrossplatform_s")

    -- set kind
	set_kind("static")

	-- add files
	add_files("src/*.cpp")

	-- add links
	add_links("mariadbclient")
	
	-- add deps
	add_deps("cryptopp_s", "libuv_s", "jsoncpp_s", "mbedtls_s", "whereami_s", "brotli_s", "iconv_s", "websocketpp_s","nghttp2_s")

-- define target
target("easycrossplatform")

    -- set kind
	set_kind("shared")

	-- add files
	add_files("src/*.cpp")

	-- add links
	add_links("mariadb")
	
	-- add deps
	add_deps("cryptopp_s", "libuv_s", "jsoncpp_s", "mbedtls_s", "whereami_s", "brotli_s", "iconv_s", "websocketpp_s","nghttp2_s")
	
