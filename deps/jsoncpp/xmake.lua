add_includedirs("include")
add_files("*.cpp")
target("jsoncpp_s")
	set_kind("static")

target("jsoncpp")
	set_kind("shared")