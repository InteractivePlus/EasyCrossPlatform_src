add_includedirs("include")
add_files("src/*.cpp")
target("websocketpp_s")
	set_kind("static")

target("websocketpp")
	set_kind("shared")