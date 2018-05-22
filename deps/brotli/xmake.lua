add_includedirs("include")
add_files("common/*.c","dec/*.c","enc/*.c")
	
target("brotli_s")
	set_kind("static")

target("brotli")
	set_kind("shared")
	