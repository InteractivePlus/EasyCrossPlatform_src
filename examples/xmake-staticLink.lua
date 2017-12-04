target("program")
    set_kind("binary")
    add_files("**.c","**.cpp","**.cxx","**.cc","lib/easycrossplatform_s.*","**.lib","**.a")
	add_includedirs("include")