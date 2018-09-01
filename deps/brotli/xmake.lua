add_includedirs("include")
add_files("common/*.c","dec/*.c","enc/*.c")

target("brotli")
	set_kind("$(kind)")
	