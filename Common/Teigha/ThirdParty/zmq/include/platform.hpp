#ifndef __ZMQ_PLATFORM_HPP_INCLUDED__
#define __ZMQ_PLATFORM_HPP_INCLUDED__

#if __APPLE__
#include "TargetConditionals.h"
#endif


/* #undef ZMQ_USE_KQUEUE */
#ifndef _WIN32
#if !(defined(TARGET_IPHONE_SIMULATOR) || defined(TARGET_OS_IPHONE) || defined(__sun))
#define ZMQ_USE_EPOLL
#endif
#else
/* #undef ZMQ_USE_EPOLL */
#endif
/* #undef ZMQ_USE_DEVPOLL */
/* #undef ZMQ_USE_POLL */
#if defined(__sun)
#define ZMQ_USE_DEVPOLL 1
#endif

#ifndef _WIN32
/* #undef ZMQ_USE_SELECT */
#if defined(TARGET_IPHONE_SIMULATOR) || defined(TARGET_OS_IPHONE)
#define ZMQ_USE_SELECT
#endif
#else
#define ZMQ_USE_SELECT
#endif

/* #undef ZMQ_FORCE_MUTEXES */

#ifndef _WIN32
#define HAVE_FORK
#define HAVE_CLOCK_GETTIME
/* #undef HAVE_GETHRTIME */
#define ZMQ_HAVE_UIO

/* #define ZMQ_HAVE_EVENTFD */
#define ZMQ_HAVE_IFADDRS

#if !(defined(TARGET_IPHONE_SIMULATOR) || defined(TARGET_OS_IPHONE) || defined(__sun))
#define ZMQ_HAVE_SO_PEERCRED
#endif
/* #undef ZMQ_HAVE_LOCAL_PEERCRED */

/* #define ZMQ_HAVE_SOCK_CLOEXEC */
#define ZMQ_HAVE_SO_KEEPALIVE

#ifdef __sun
#define KEEPCNT_DISABLED 1
#endif

#ifndef KEEPCNT_DISABLED
#define ZMQ_HAVE_TCP_KEEPCNT
#if !(defined(TARGET_IPHONE_SIMULATOR) || defined(TARGET_OS_IPHONE))
#define ZMQ_HAVE_TCP_KEEPIDLE
#endif
#define ZMQ_HAVE_TCP_KEEPINTVL
#endif //KEEPCNT_DISABLED
#else
/* #undef HAVE_FORK */
/* #undef HAVE_CLOCK_GETTIME */
/* #undef HAVE_GETHRTIME */
/* #undef ZMQ_HAVE_UIO */

/* #undef ZMQ_HAVE_EVENTFD */
/* #undef ZMQ_HAVE_IFADDRS */

/* #undef ZMQ_HAVE_SO_PEERCRED */
/* #undef ZMQ_HAVE_LOCAL_PEERCRED */

/* #undef ZMQ_HAVE_SOCK_CLOEXEC */
/* #undef ZMQ_HAVE_SO_KEEPALIVE */
/* #undef ZMQ_HAVE_TCP_KEEPCNT */
/* #undef ZMQ_HAVE_TCP_KEEPIDLE */
/* #undef ZMQ_HAVE_TCP_KEEPINTVL */
/* #undef ZMQ_HAVE_TCP_KEEPALIVE */
#endif
/* #undef ZMQ_HAVE_OPENPGM */
/* #undef ZMQ_MAKE_VALGRIND_HAPPY */


#ifdef _AIX
  #define ZMQ_HAVE_AIX
#endif

#if defined ANDROID
  #define ZMQ_HAVE_ANDROID
#endif

#if defined __CYGWIN__
  #define ZMQ_HAVE_CYGWIN
#endif

#if defined __MINGW32__
  #define ZMQ_HAVE_MINGW32
#endif

#if defined(__FreeBSD__) || defined(__DragonFly__) || defined(__FreeBSD_kernel__)
  #define ZMQ_HAVE_FREEBSD
#endif

#if defined __hpux
  #define ZMQ_HAVE_HPUX
#endif

#if defined __linux__
  #define ZMQ_HAVE_LINUX
#endif

#if defined __NetBSD__
  #define ZMQ_HAVE_NETBSD
#endif

#if defined __OpenBSD__
  #define ZMQ_HAVE_OPENBSD
#endif

#if defined __VMS
  #define ZMQ_HAVE_OPENVMS
#endif

#if defined __APPLE__
  #define ZMQ_HAVE_OSX
#endif

#if defined __QNXNTO__
  #define ZMQ_HAVE_QNXNTO
#endif

#if defined(sun) || defined(__sun)
  #define ZMQ_HAVE_SOLARIS
#endif

#ifdef _WIN32
#define ZMQ_HAVE_WINDOWS
#endif

#if !defined(_WIN32_WCE) 
#define ZMQ_HAVE_SYS_TYPES
#endif

#if (defined(_MSC_VER) && _MSC_VER < 1500)
//  Require Windows XP or higher with MinGW for getaddrinfo().
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif

#endif
