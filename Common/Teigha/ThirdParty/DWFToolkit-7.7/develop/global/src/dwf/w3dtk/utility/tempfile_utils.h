
/*
* $Id: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/w3dtk/utility/tempfile_utils.h#1 $
*/

#ifndef _TEMPFILE_UTILS
#define _TEMPFILE_UTILS

#include <wchar.h>

#if defined(_MSC_VER) && !defined(DWFTK_STATIC)
#	ifndef DWFTK_EXPORT_API
#		define _W3DTK_API __declspec(dllimport)
#	else
#			define _W3DTK_API __declspec(dllexport)
#	endif
#else
#	define _W3DTK_API
#endif

#define alter
#define TEMPFILE_UTILS_BUFFER_SIZE 4096

// Populates the passed in buffer which will be populated with a path to a temporary file (with optional extension)
extern _W3DTK_API void GenerateTempFileName ( char alter * tempFileNameOut, char const * extension = 0 );

// Populates the passed in buffer which will be populated with a path to a temporary file (with optional extension)
extern _W3DTK_API void GenerateTempFileName ( wchar_t alter * tempFileNameOut, wchar_t const * extension = 0 );

// Populates the passed in buffer with a path to a temporary directory
extern _W3DTK_API void GenerateTempDirectoryName ( wchar_t alter * tempDirectoryNameOut );

#endif // _TEMPFILE_UTILS
