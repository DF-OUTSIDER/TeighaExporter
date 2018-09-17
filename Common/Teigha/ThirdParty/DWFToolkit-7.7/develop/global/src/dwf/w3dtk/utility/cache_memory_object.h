/*
* Copyright (c) 1998 by Tech Soft 3D, LLC.
* The information contained herein is confidential and proprietary to
* Tech Soft 3D, LLC., and considered a trade secret as defined under
* civil and criminal statutes.	 Tech Soft 3D shall pursue its civil
* and criminal remedies in the event of unauthorized use or misappropriation
* of its trade secrets.  Use of this information by anyone other than
* authorized employees of Tech Soft 3D, LLC. is granted only under a
* written non-disclosure agreement, expressly prescribing the scope and
* manner of such use.
*
* $Id: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/w3dtk/utility/cache_memory_object.h#1 $
*/

#ifndef CACHE_MEMORY_OBJECT_H
#define CACHE_MEMORY_OBJECT_H

#include <stdlib.h>

#if defined(_MSC_VER) && !defined(DWFTK_STATIC)
#	ifndef DWFTK_EXPORT_API
#		define _W3DTK_API __declspec(dllimport)
#	else
#			define _W3DTK_API __declspec(dllexport)
#	endif
#else
#	define _W3DTK_API
#endif


#define FREE_LIST_SIZE 32

class _W3DTK_API CMObject {
public:
	CMObject ( void * p_in = 0, size_t size_in = 0 ) : p(p_in), size(size_in) {};

	~CMObject () {};

	void *p;
	size_t size;
};

class _W3DTK_API CacheMemoryObject {
public:
	CacheMemoryObject ();
	virtual ~CacheMemoryObject ();

	static void * operator new ( size_t size );

 	static void operator delete ( void * p, size_t size ) throw ();

private:
	static void * free_list_small[8][FREE_LIST_SIZE];
	static int cursor_small[8];
	static CMObject free_list_large[FREE_LIST_SIZE];
	static int cursor_large;
	static int ref_count;
};


#endif
