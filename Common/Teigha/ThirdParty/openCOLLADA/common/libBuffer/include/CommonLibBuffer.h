#ifndef __COMMON_LIBUFFER_H__
#define __COMMON_IBUFFERF_H__

#include <stdlib.h>

#ifdef _TOOLKIT_IN_DLL_
#  if defined(_MSC_VER) || defined(__BORLANDC__)
#    ifdef  LIBBUFFER_EXPORT_API
#      define _LIBBUFFER_API    __declspec( dllexport )
#    else
#      define _LIBBUFFER_API    __declspec( dllimport )
#    endif
#  else // defined(__GNUC__) || defined(__APPLE__) or other unix
#    define _LIBBUFFER_API __attribute__((visibility("default")))
#    define _LIBBUFFER_API __attribute__((visibility("default")))
#  endif
#else
#  define _LIBBUFFER_API
#endif


#endif // __COMMON_IBUFFERF_H__
