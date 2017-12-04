target("easycrossplatform_s")
	set_kind("static")
	add_files("src/*.cpp","src/*.c","lib/mysqlcppconn-static.*")
	add_includedirs("src")
	--on_install(function (target)
		
	--end)
--target("easycrossplatform")
	--set_kind("shared")
	--add_files("src/*.cpp","src/*.c")
	--add_includedirs("src")
	