target("jsoncpp_s")
	set_kind("static")
	add_includedirs("include")
	add_files("*.cpp")

target("jsoncpp")
	set_kind("shared")
	add_includedirs("include")
	add_files("*.cpp")