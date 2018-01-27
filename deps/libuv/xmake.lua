target("libuv_s")
	set_kind("static")
	add_includedirs("include")
	--编译LibUV进行时
	add_files('fs-poll.c',
			'inet.c',
			'threadpool.c',
			'uv-data-getter-setters.c',
			'uv-common.c',
			'version.c')
	if is_os("windows") then
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
		if is_option("build-on-solaris") then
			add_ldflags("-pthreads")
		else
			add_ldflags("-pthread")
		end
		
		if (is_os("macosx") or is_os("ios") or is_option("build-on-linux") or is_os("android") or is_option("build-on-zos")) then
			add_files('unix/proctitle.c') --Only MacOS, iOS, Linux, Android, ZOS
		end
		
		if (is_os("macosx") or is_os("ios")) then
			add_files('unix/darwin.c',
					'unix/fsevents.c',
					'unix/darwin-proctitle.c'
			)
			add_defines('_DARWIN_USE_64_BIT_INODE=1','_DARWIN_UNLIMITED_SELECT=1')
		end
		if is_option("build-on-linux") then
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
		if is_os("android") then
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
		if is_option("build-on-solaris") then
			add_files("unix/no-proctitle.c",
					"unix/sunos.c"
			)
			add_defines('__EXTENSIONS__','_XOPEN_SOURCE=500')
			add_links('kstat','nsl','sendfile','socket')
			
		end
		if (is_option("build-on-freebsd") or is_os("build-on-dragonflybsd")) then
			add_files("unix/freebsd.c")
		end
		if is_option("build-on-openbsd") then
			add_files("unix/openbsd.c")
		end
		if is_option("build-on-netbsd") then
			add_files("unix/netbsd.c")
		end
		if (is_option("build-on-freebsd") or is_option("build-on-dragonflybsd") or is_option("build-on-openbsd") or is_option("build-on-netbsd")) then
			add_files("unix/posix-hrtime.c")
		end
		if (is_os("macosx") or is_os("ios") or is_option("build-on-freebsd") or is_option("build-on-dragonflybsd") or is_option("build-on-openbsd") or is_option("build-on-netbsd")) then
			add_files('unix/bsd-ifaddrs.c','unix/kqueue.c')
		end
		if is_option("build-on-zos") then
			add_files('unix/pthread-fixes.c',
					'unix/os390.c',
					'unix/os390-syscalls.c'
			)
			
		end
	end
	if ( not(is_os("windows")) ) then
		add_defines('_LARGEFILE_SOURCE', '_FILE_OFFSET_BITS=64')
	end
target("libuv")
	set_kind("shared")
	add_includedirs("include")
	add_defines("USING_UV_SHARED=1")
	--编译LibUV进行时
	add_files('fs-poll.c',
			'inet.c',
			'threadpool.c',
			'uv-data-getter-setters.c',
			'uv-common.c',
			'version.c')
	if is_os("windows") then
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
		if is_option("build-on-solaris") then
			add_ldflags("-pthreads")
		else
			add_ldflags("-pthread")
		end
		
		if (is_os("macosx") or is_os("ios") or is_option("build-on-linux") or is_os("android") or is_option("build-on-zos")) then
			add_files('unix/proctitle.c') --Only MacOS, iOS, Linux, Android, ZOS
		end
		
		if (is_os("macosx") or is_os("ios")) then
			add_files('unix/darwin.c',
					'unix/fsevents.c',
					'unix/darwin-proctitle.c'
			)
			add_defines('_DARWIN_USE_64_BIT_INODE=1','_DARWIN_UNLIMITED_SELECT=1')
		end
		if is_option("build-on-linux") then
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
		if is_os("android") then
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
		if is_option("build-on-solaris") then
			add_files("unix/no-proctitle.c",
					"unix/sunos.c"
			)
			add_defines('__EXTENSIONS__','_XOPEN_SOURCE=500')
			add_links('kstat','nsl','sendfile','socket')
			
		end
		if (is_option("build-on-freebsd") or is_os("build-on-dragonflybsd")) then
			add_files("unix/freebsd.c")
		end
		if is_option("build-on-openbsd") then
			add_files("unix/openbsd.c")
		end
		if is_option("build-on-netbsd") then
			add_files("unix/netbsd.c")
		end
		if (is_option("build-on-freebsd") or is_option("build-on-dragonflybsd") or is_option("build-on-openbsd") or is_option("build-on-netbsd")) then
			add_files("unix/posix-hrtime.c")
		end
		if (is_os("macosx") or is_os("ios") or is_option("build-on-freebsd") or is_option("build-on-dragonflybsd") or is_option("build-on-openbsd") or is_option("build-on-netbsd")) then
			add_files('unix/bsd-ifaddrs.c','unix/kqueue.c')
		end
		if is_option("build-on-zos") then
			add_files('unix/pthread-fixes.c',
					'unix/os390.c',
					'unix/os390-syscalls.c'
			)
			
		end
	end
	if ( not(is_os("windows")) ) then
		add_defines('_LARGEFILE_SOURCE', '_FILE_OFFSET_BITS=64')
	end