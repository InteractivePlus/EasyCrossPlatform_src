target("program")
    set_kind("binary")
    add_files("**.c","**.cpp","**.cxx"）
	add_linkdirs("lib")
	add_links("easycrossplatform")
	add_includedirs("include","include/extlibs")