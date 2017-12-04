target("program")
    set_kind("binary")
    add_files("**.c","**.cpp","**.cxx"）
	add_linkdirs("lib")
	add_links("easycrossplatform_s")
	add_includedirs("include")