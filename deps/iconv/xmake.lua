add_defines("BUILDING_LIBICONV","BUILDING_LIBCHARSET")
add_includedirs("include")
add_files("source/*.c")

target("iconv")
	set_kind("$(kind)")
	if is_kind('static') then
		add_defines("USING_STATIC_LIBICONV")
	end