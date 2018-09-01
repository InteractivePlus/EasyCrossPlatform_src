add_includedirs("include")
add_files("src/*.cpp")

target("websocketpp")
	set_kind("$(kind)")