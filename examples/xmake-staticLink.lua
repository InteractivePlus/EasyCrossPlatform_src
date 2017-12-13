target("program")
    set_kind("binary")
    add_files("**.c","**.cpp","**.cxx","**.cc")
	add_linkdirs("lib")
	add_includedirs("include")
	add_links("mariadbclient")
	if(is_plat("windows")) then
		--add_links("libcurl_s")
	else
		--add_links("curl_s")
	end
	add_links("easycrossplatform_s")