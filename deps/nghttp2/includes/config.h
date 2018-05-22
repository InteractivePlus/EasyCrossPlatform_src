#include <automatedconfig.h>
#ifdef HAVE_TIME_H
	#include <time.h>
#elif defined(HAVE_SYS_TIME_H)
	#inclue <sys/time.h>
#endif

/* Define to `int' if <sys/types.h> does not define. */
#ifndef ssize_t
	#define ssize_t int
#endif

/* Define to 1 if you have the `std::map::emplace`. */
#define HAVE_STD_MAP_EMPLACE 1

/* sizeof(int *) */
#ifdef EASYCROSSPLATFORM_ARCHITECTURE_X64
	#define SIZEOF_INT_P 8
#else
	#define SIZEOF_INT_P 4
#endif

/* sizeof(time_t) */
#ifdef EASYCROSSPLATFORM_ARCHITECTURE_X64
	#define SIZEOF_TIME_T 8
#else
	#define SIZEOF_TIME_T 4
#endif


/* Define to 1 if you have the `_Exit` function. */
#define HAVE__EXIT 0

/* Define to 1 if you have the `initgroups` function. */
#define HAVE_DECL_INITGROUPS 0

