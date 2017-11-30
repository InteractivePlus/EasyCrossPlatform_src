--Not Available Right Now
target("program")
	set_kind("binary")
	add_files("**.c","**.cpp","**.cxx","**.cc","**.lib","**.a")
    add_deps("easycrossplatform")
	add_includedirs("include","src/include")