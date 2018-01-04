target("program")
    set_kind("binary")
    add_files("**.c","**.cpp","**.cxx","**.cc")
	add_linkdirs("lib")
	add_includedirs("include")
	add_links("mariadbclient")
	add_defines("CURL_STATICLIB","SFML_STATIC")
	if(is_plat("windows")) then
		--add_links("libcurl_s")
		add_links("winmm","ws2_32","wldap32","advapi32","Normaliz","Crypt32","libcurl_s")
	else
		add_ldflags("-lcurl")
		--add_links("curl_s")
	end
	add_links("easycrossplatform_s")