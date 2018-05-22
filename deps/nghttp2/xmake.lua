
-- add checkings and generate results to config header
-- add_checking("arpa/inet.h", "limits.h", "fcntl.h", "xxxx.h")
--
function add_checking_to_config(...)
    local options = {}
    for _, header in ipairs({...}) do
        local define = header:upper():gsub("[%./]", "_")
        option(define)
            add_cincludes(header)
            add_defines_h("HAVE_" .. define)
        option_end()
        table.insert(options, define)
    end
    target("config")
        set_kind("phony")
        set_config_header("includes/automatedconfig.h")
        add_options(options)
	target_end()
end
add_checking_to_config("arpa/inet.h", "limits.h", "fcntl.h", "inttypes.h","netdb.h","netinet/in.h","pwd.h","sys/socket.h","sys/time.h","syslog.h","time.h","unistd.h")

add_includedirs("includes")
add_defines("BUILDING_NGHTTP2","HAVE_CONFIG_H")
add_files(
	"nghttp2_pq.c","nghttp2_map.c","nghttp2_queue.c",
	"nghttp2_frame.c",
	"nghttp2_buf.c",
	"nghttp2_stream.c", "nghttp2_outbound_item.c",
	"nghttp2_session.c", "nghttp2_submit.c",
	"nghttp2_helper.c",
	"nghttp2_npn.c",
	"nghttp2_hd.c", "nghttp2_hd_huffman.c", "nghttp2_hd_huffman_data.c",
	"nghttp2_version.c", 
	"nghttp2_priority_spec.c", 
	"nghttp2_option.c", 
	"nghttp2_callbacks.c", 
	"nghttp2_mem.c", 
	"nghttp2_http.c", 
	"nghttp2_rcbuf.c", 
	"nghttp2_debug.c"
)
if is_os("windows") then
	add_files("version.rc")
end

target("nghttp2_s")
	set_kind("static")
	add_defines("NGHTTP2_STATICLIB")

target("nghttp2")
	set_kind("shared")
	

