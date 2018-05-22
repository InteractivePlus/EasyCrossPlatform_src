add_includedirs("include")
if is_plat("windows") then

		add_files(
				'win/async.c',
				'win/core.c',
				'win/detect-wakeup.c',
				'win/dl.c',
				'win/error.c',
				'win/fs.c',
				'win/fs-event.c',
				'win/getaddrinfo.c',
				'win/getnameinfo.c',
				'win/handle.c',
				'win/loop-watcher.c',
				'win/pipe.c',
				'win/thread.c',
				'win/poll.c',
				'win/process.c',
				'win/process-stdio.c',
				'win/req.c',
				'win/signal.c',
				'win/snprintf.c',
				'win/stream.c',
				'win/tcp.c',
				'win/tty.c',
				'win/timer.c',
				'win/udp.c',
				'win/util.c',
				'win/winapi.c',
				'win/winsock.c'
		)
		add_links('advapi32','iphlpapi','psapi','shell32','user32','userenv','ws2_32')
	else
		add_files(
				'unix/async.c',
				'unix/atomic-ops.h',
				'unix/core.c',
				'unix/dl.c',
				'unix/fs.c',
				'unix/getaddrinfo.c',
				'unix/getnameinfo.c',
				'unix/loop.c',
				'unix/loop-watcher.c',
				'unix/pipe.c',
				'unix/poll.c',
				'unix/process.c',
				'unix/signal.c',
				'unix/spinlock.h',
				'unix/stream.c',
				'unix/tcp.c',
				'unix/thread.c',
				'unix/timer.c',
				'unix/tty.c',
				'unix/udp.c'
		)
		if is_plat("solaris") then
			add_cxflags("-pthreads")
			add_ldflags("-pthreads")
		else
			add_cxflags("-pthread")
			add_mxflags("-pthread")
			add_ldflags("-pthread")
		end
		
		if (is_plat("macosx") or is_plat("ios") or is_plat("linux") or is_plat("android") or is_plat("zos")) then
			add_files('unix/proctitle.c') --Only MacOS, iOS, Linux, Android, ZOS
		end
		
		if (is_plat("macosx") or is_plat("ios")) then
			add_files('unix/darwin.c',
					'unix/fsevents.c',
					'unix/darwin-proctitle.c'
			)
			add_defines('_DARWIN_USE_64_BIT_INODE=1','_DARWIN_UNLIMITED_SELECT=1')
		end
		if is_plat("linux") then
			add_defines("_GNU_SOURCE","_POSIX_C_SOURCE=200112")
			add_files('unix/linux-core.c',
					'unix/linux-inotify.c',
					'unix/linux-syscalls.c',
					'unix/linux-syscalls.h',
					'unix/procfs-exepath.c',
					'unix/sysinfo-loadavg.c',
					'unix/sysinfo-memory.c'
			)
			add_links("dl","rt")
		end
		if is_plat("android") then
			add_files('unix/linux-core.c',
					'unix/linux-inotify.c',
					'unix/linux-syscalls.c',
					'unix/pthread-fixes.c',
					'unix/android-ifaddrs.c',
					'unix/procfs-exepath.c',
					'unix/sysinfo-loadavg.c',
					'unix/sysinfo-memory.c'
			)
			add_links("dl")
		end
		if is_plat("solaris") then
			add_files("unix/no-proctitle.c",
					"unix/sunos.c"
			)
			add_defines('__EXTENSIONS__','_XOPEN_SOURCE=500')
			add_links('kstat','nsl','sendfile','socket')
			
		end
		if (is_plat("freebsd") or is_plat("dragonflybsd")) then
			add_files("unix/freebsd.c")
		end
		if is_plat("openbsd") then
			add_files("unix/openbsd.c")
		end
		if is_plat("netbsd") then
			add_files("unix/netbsd.c")
		end
		if (is_plat("freebsd") or is_plat("dragonflybsd") or is_plat("openbsd") or is_plat("netbsd")) then
			add_files("unix/posix-hrtime.c")
		end
		if (is_plat("macosx") or is_plat("ios") or is_plat("freebsd") or is_plat("dragonflybsd") or is_plat("openbsd") or is_plat("netbsd")) then
			add_files('unix/bsd-ifaddrs.c','unix/kqueue.c')
		end
		if is_plat("zos") then
			add_files('unix/pthread-fixes.c',
					'unix/os390.c',
					'unix/os390-syscalls.c'
			)
			
		end
	end
--编译LibUV进行时
add_files('fs-poll.c',
			'inet.c',
			'threadpool.c',
			'uv-data-getter-setters.c',
			'uv-common.c',
			'version.c')
if (not(is_plat("windows"))) then
		add_defines('_LARGEFILE_SOURCE', '_FILE_OFFSET_BITS=64')
end

target("libuv_s")
	set_kind("static")
	
target("libuv")
	set_kind("shared")
	add_defines("USING_UV_SHARED=1")