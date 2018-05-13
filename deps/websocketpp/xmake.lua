target("websocketpp_s")
	set_kind("static")
	add_includedirs("include")
	add_files("src/*.c")

target("websocketpp")
	set_kind("shared")
	add_includedirs("include")
	add_files("src/*.c")