/*
 * Copyright (c) 2008, Autodesk, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Autodesk, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY Autodesk, Inc. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Autodesk, Inc. OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


/*	Have we been here before? */
#ifndef __CS_MAPH__
#define __CS_MAPH__

/* We do this before anything else. */
#include "cs_clientBeg.h"

/*
	We use six preprocessor constants which indicate the environment
	in which the code is being compiled and in which it is to operate.
	We try to do this automatically if possible.  If you need to add
	some for your environment, please let us know.

	The six preprocessor constants are:

		_MEM_MODEL		memory model, possible values are:
						_mm_SMALL
						_mm_COMPACT
						_mm_MEDIUM
						_mm_LARGE
						_mm_HUGE
						_mm_VIRTUAL
						_mm_FLAT	(not virtual, not segmented)
						_mm_WIN16	(weird)

		_FILE_SYSTEM	file system, possible values are:
						_fs_MS_DOS	(includes FAT 32 for now)
						_fs_NT
						_fs_UNIX

		_OPR_SYSTEM		operating system, possible values are:
						_os_MS_DOS		3.01 and above
						_os_WIN16		3.00 and above
						_os_WIN32		only one version
						_os_WINNT		3.50 and above
						_os_WIN64		3.50 and above
						_os_UNIX		System V and above
						_os_PHARLAP		Pharlap DOS extender
						_os_RATIONAL	Tenberry DOS extender
						_os_UNIX		Generic UNIX (is there such a thing?)

		_RUN_TIME		run time library in use, possible values are:
						_rt_WC386		WATCOM 386 (32 bit)
						_rt_WC286		WATCOM 286 (16 bit)
						_rt_MSC16		Microsoft, C (prior to VC++)
						_rt_MSVC16		Microsoft, VC++, 1.?
						_rt_MSVC32		Microsoft, VC++ >= 2.??
						_rt_MSDOTNET	Microsoft, VC++ >= 7.0
						_rt_MSWIN64		Microsoft, VC++ x64
						_rt_METAWARE	Metaware 3.11
						_rt_BORLAND16	Borland C++ 16 bit
						_rt_BORLAND32	Borland C++ 16 bit
						_rt_UNIXPCC		UNIX portable C compiler (Linux)
						_rt_SUN32		Sun Solaris 32 bit compiler
						_rt_SUN64		Sun Solaris 64 bit compiler
						_rt_MOSXUNIX
						_rt_HPUX		Hewlett Packard UNIX
						_rt_AIX			IBM AIX versionof UNIX

		_PROCESSOR		the processor/architecture for which the compiled code is to run on:
						_pc_GENERIC		Not known, generate generic code
						_pc_IX86		Intel 86 processor
						_pc_IA64		Itanium 64 bit processor
						_pc_MOT32		Motorola 32 bit processor
						_pc_GENPCKT		Generic pocket PC.

*/

#define _mm_NONE      0
#define _mm_SMALL     1
#define _mm_COMPACT   2
#define _mm_MEDIUM    3
#define _mm_LARGE     4
#define _mm_HUGE      5
#define _mm_VIRTUAL   6
#define _mm_FLAT      7
#define _mm_WIN16     8

#define _fs_NONE      0
#define _fs_MS_DOS    1
#define _fs_NT        2
#define _fs_UNIX      3

#define _os_NONE      0
#define _os_MS_DOS    1
#define _os_WIN16     2
#define _os_WIN32     4
#define _os_WINNT     5
#define _os_UNIX      6
#define _os_PHARLAP   7
#define _os_RATIONAL  8
#define _os_SOLARIS  10

#define _mfc_VER4     1
#define _mfc_VER7     2
#define _mfc_VER8     3

/*
	With regard to the following list, we do at times
	refer to _RUN_TIME < _rt_UNIXPCC to refer to all
	WinTel environments.  You must consider this before
	changing any of these values, or adding new ones.
*/

#define _rt_WC386       1
#define _rt_WC286       2
#define _rt_MSC16       3
#define _rt_MSVC16      4
#define _rt_MSVC32      5
#define _rt_METAWARE    6
#define _rt_BORLAND16   7
#define _rt_BORLAND32   8
#define _rt_MSDOTNET   10
#define _rt_MSWIN64    11
/* Anything less than the numeric value assigned to _rt_UNIXPCC is considered
   to be a Microsoft/Personal Computer type of thing.  I.e., supports stuff
   like findfirst rather than ftw.  */
#define _rt_UNIXPCC    101
#define _rt_SUN32      102
#define _rt_SUN64      103
#define _rt_MOSXUNIX   104
#define _rt_HPUX       105
#define _rt_AIX        106

/*
	With regard to the following list, we will refer to ranges of the value
	at times.  Values less than 400 are reserved for 32 bit processors including
	WinTel and Motorola type processors.  Values between, and including, 401 thru
	599 are reserved for 64 bit processors.  Values between, and including, 601
	thru 799 are reserved for hand held, pocket PC type processor architectures.
	
	Currently, the processor is a concern only for structure alignment issues.
*/
#define _pc_BASE_16       0x100
#define _pc_GENERIC_16    0x101
#define _pc_BASE_32       0x200
#define _pc_GENERIC_32    0x201
#define _pc_IX86          0x202
#define _pc_MOT32         0x203
#define _pc_SPARC32       0x204        
#define _pc_BASE_64       0x400
#define _pc_GENERIC_64    0x401
#define _pc_IA64          0x402
#define _pc_SPARC64       0x403        
#define _pc_BASE_CE       0x800
#define _pc_GENERIC_CE    0x801

#undef _MEM_MODEL
#undef _FILE_SYSTEM
#undef _OPR_SYSTEM
#undef _RUN_TIME
#undef _MFC_VERSION
#undef _PROCESSOR

/* I found the following information on the Web; thought capturing it and
   permanently recording it here may be helpful.

	MSVC++ 12.0 _MSC_VER == 1800 (Visual Studio 2013)
	MSVC++ 11.0 _MSC_VER == 1700 (Visual Studio 2012)
	MSVC++ 10.0 _MSC_VER == 1600 (Visual Studio 2010)
	MSVC++ 9.0  _MSC_VER == 1500 (Visual Studio 2008)
	MSVC++ 8.0  _MSC_VER == 1400 (Visual Studio 2005)
	MSVC++ 7.1  _MSC_VER == 1310 (Visual Studio 2003)
	MSVC++ 7.0  _MSC_VER == 1300
	MSVC++ 6.0  _MSC_VER == 1200
	MSVC++ 5.0  _MSC_VER == 1100
*/

/*
	OK, configure for this compiler, and run time environment.
*/

#if defined (__ZTC__)			/* Zortech, then Symantec, now D-Mars???. */
#	define mktemp tmpnam
#	define _MEM_MODEL	_mm_VIRTUAL
#	define _FILE_SYSTEM	_fs_MS_DOS
#	define _OPR_SYSTEM	_os_MS_DOS
#	define _RUN_TIME	_rt_MSC16
#	define _PROCESSOR	_pc_IX86
#elif defined (__TURBOC__)		/* Borland C++ */
#	if defined (__WIN32__)
#		define _MEM_MODEL	_mm_VIRTUAL
#		define _FILE_SYSTEM	_fs_MS_DOS
#		define _OPR_SYSTEM	_os_WIN32
#		define _RUN_TIME	_rt_BORLAND32
#		define _PROCESSOR	_pc_IX86
#	elif defined (_Windows)
#		define _MEM_MODEL	_mm_WIN16
#		define _FILE_SYSTEM	_fs_MS_DOS
#		define _OPR_SYSTEM	_os_WIN16
#		define _RUN_TIME	_rt_BORLAND16
#		define _PROCESSOR	_pc_IX86
#	elif defined (__SMALL__)
#		define _MEM_MODEL	_mm_SMALL
#		define _FILE_SYSTEM	_fs_MS_DOS 
#		define _OPR_SYSTEM	_os_MS_DOS
#		define _RUN_TIME	_rt_BORLAND16
#		define _PROCESSOR	_pc_IX86
#	elif defined (__COMPACT__)
#		define _MEM_MODEL	_mm_COMPACT
#		define _FILE_SYSTEM	_fs_MS_DOS
#		define _OPR_SYSTEM	_os_MS_DOS
#		define _RUN_TIME	_rt_BORLAND16
#		define _PROCESSOR	_pc_IX86
#	elif defined (__MEDIUM__)
#		define _MEM_MODEL	_mm_MEDIUM
#		define _FILE_SYSTEM	_fs_MS_DOS
#		define _OPR_SYSTEM	_os_MS_DOS
#		define _RUN_TIME	_rt_BORLAND16
#		define _PROCESSOR	_pc_IX86
#	elif defined (__LARGE__)
#		define _MEM_MODEL	_mm_LARGE
#		define _FILE_SYSTEM	_fs_MS_DOS
#		define _OPR_SYSTEM	_os_MS_DOS
#		define _RUN_TIME	_rt_BORLAND16
#		define _PROCESSOR	_pc_IX86
#	elif defined (__HUGE__)
#		define _MEM_MODEL	_mm_HUGE
#		define _FILE_SYSTEM	_fs_MS_DOS
#		define _OPR_SYSTEM	_os_MS_DOS
#		define _RUN_TIME	_rt_BORLAND16
#		define _PROCESSOR	_pc_IX86
#	elif defined (_WIN32)
#		define _MEM_MODEL	_mm_VIRTUAL
#		define _FILE_SYSTEM	_fs_MS_DOS
#		define _OPR_SYSTEM	_os_WIN32
#		define _RUN_TIME	_rt_BORLAND32
#		define _PROCESSOR	_pc_IX86
#	else
#		define _MEM_MODEL	_mm_WIN16
#		define _FILE_SYSTEM	_fs_MS_DOS
#		define _OPR_SYSTEM	_os_WIN16
#		define _RUN_TIME	_rt_BORLAND16
#		define _PROCESSOR	_pc_IX86
#	endif
#elif defined (_META_)			/* Meta Ware High C */
#	define _MEM_MODEL	_mm_VIRTUAL
#	define _FILE_SYSTEM	_fs_MS_DOS
#	define _OPR_SYSTEM	_os_PHARLAP
#	define _RUN_TIME	_rt_METAWARE
#	define _PROCESSOR	_pc_IX86
#elif defined (__WATCOMC__)		/* Watcom */
#	if defined (__NT__)
#		define _MEM_MODEL	_mm_VIRTUAL
#		define _FILE_SYSTEM	_fs_MS_DOS
#		define _OPR_SYSTEM	_os_WINNT
#		define _RUN_TIME	_rt_WC386
#		define _PROCESSOR	_pc_IX86
#	elif defined (__WINDOW386__)
#		define _MEM_MODEL	_mm_VIRTUAL
#		define _FILE_SYSTEM	_fs_MS_DOS
#		define _OPR_SYSTEM	_os_WIN32
#		define _RUN_TIME	_rt_WC386
#		define _PROCESSOR	_pc_IX86
#	else
#		define _MEM_MODEL	_mm_VIRTUAL
#		define _FILE_SYSTEM	_fs_MS_DOS
#		define _OPR_SYSTEM	_os_RATIONAL
#		define _RUN_TIME	_rt_WC386
#		define _PROCESSOR	_pc_IX86
#	endif
#elif defined (_MSC_VER) && _MSC_VER >= 1400	/* MS Visual C++ V8, aka .NET 2005 */
#	define _MEM_MODEL	_mm_VIRTUAL
#	define _MFC_VERSION _mfc_VER8
#	if defined (_WIN64)
#		define _FILE_SYSTEM		_fs_NT
#		define _OPR_SYSTEM		_os_WIN64
#		define _RUN_TIME		_rt_MSWIN64
#		if defined (_M_IA64)
#			define _PROCESSOR	_pc_IA64
#		else
#			define _PROCESSOR	_pcIX86
#		endif
#	else
#		define _FILE_SYSTEM		_fs_MS_DOS
#		define _OPR_SYSTEM		_os_WIN32
#		define _RUN_TIME		_rt_MSDOTNET
#		if defined (_M_IA64)
#			define _PROCESSOR	_pc_IA64
#		else
#			define _PROCESSOR	_pcIX86
#		endif
#	endif
#elif defined (_MSC_VER) && _MSC_VER >= 1300	/* MS Visual C++ V7, aka .NET */
#	define _MEM_MODEL	_mm_VIRTUAL
#	define _FILE_SYSTEM	_fs_MS_DOS
#	define _OPR_SYSTEM	_os_WINNT
#	define _RUN_TIME	_rt_MSDOTNET
#	define _MFC_VERSION	_mfc_VER7
#	define _PROCESSOR	_pc_IX86
#elif defined (_MSC_VER) && _MSC_VER >= 900		/* MS Visual C++ 2.x or later */
#	define _MEM_MODEL	_mm_VIRTUAL
#	define _FILE_SYSTEM	_fs_MS_DOS
#	define _OPR_SYSTEM	_os_WINNT
#	define _RUN_TIME	_rt_MSVC32
#	define _MFC_VERSION _mfc_VER4
#	define _PROCESSOR	_pc_IX86
#elif defined (_MSC_VER) && _MSC_VER >= 800 && _MSC_VER < 900 /* MS Visual C++ 1.0 */
#	if defined (M_I86HM)
#		define _MEM_MODEL  	_mm_HUGE
#		define _FILE_SYSTEM	_fs_MS_DOS
#		define _OPR_SYSTEM	_os_MS_DOS
#		define _RUN_TIME	_rt_MSVC16
#		define _PROCESSOR	_pc_IX86
#	elif defined (M_I86LM)
#		define _MEM_MODEL	_mm_LARGE
#		define _FILE_SYSTEM	_fs_MS_DOS
#		define _OPR_SYSTEM	_os_MS_DOS
#		define _RUN_TIME	_rt_MSVC16
#		define _PROCESSOR	_pc_IX86
#	elif defined (M_I86CM)
#		define _MEM_MODEL	_mm_COMPACT
#		define _FILE_SYSTEM	_fs_MS_DOS
#		define _OPR_SYSTEM	_os_MS_DOS
#		define _RUN_TIME	_rt_MSVC16
#		define _PROCESSOR	_pc_IX86
#	elif defined (M_I86MM)
#		define _MEM_MODEL	_mm_MEDIUM
#		define _FILE_SYSTEM	_fs_MS_DOS
#		define _OPR_SYSTEM	_os_MS_DOS
#		define _RUN_TIME	_rt_MSVC16
#		define _PROCESSOR	_pc_IX86
#	elif defined (M_I86SM)
#		define _MEM_MODEL	_mm_SMALL
#		define _FILE_SYSTEM	_fs_MS_DOS
#		define _OPR_SYSTEM	_os_MS_DOS
#		define _RUN_TIME	_rt_MSVC16
#		define _PROCESSOR	_pc_IX86
#	else
#		define _MEM_MODEL	_mm_WIN16
#		define _FILE_SYSTEM	_fs_MS_DOS
#		define _OPR_SYSTEM	_os_WIN16
#		define _RUN_TIME	_rt_MSVC16
#		define _PROCESSOR	_pc_IX86
#	endif
#elif defined (_WIN32)				/* Any other with WIN32 defined */
#	define _MEM_MODEL	_mm_VIRTUAL
#	define _FILE_SYSTEM	_fs_MS_DOS
#	define _OPR_SYSTEM	_os_WINNT
#ifdef __GNUC__ 
#	define _RUN_TIME	_rt_UNIXPCC
#else
#	define _RUN_TIME	_rt_MSVC32
#endif
#	define _PROCESSOR	_pc_IX86
#elif defined (sun) || defined (__SUN__)			/* Sun Microsystems. */
#	define _MEM_MODEL	_mm_VIRTUAL
#	define _FILE_SYSTEM	_fs_UNIX
#	define _OPR_SYSTEM	_os_SOLARIS
#	if defined (_LP64)
#		define _RUN_TIME	_rt_SUN64
#		define _PROCESSOR	_pc_SPARC64
#	else
#		define _RUN_TIME	_rt_SUN32
#		define _PROCESSOR	_pc_SPARC32
#	endif
#else
#	if defined (M_I86SM)		/* Microsoft, prior to visual C++ */
#		define _MEM_MODEL	_mm_SMALL
#		define _FILE_SYSTEM	_fs_MS_DOS		
#		define _OPR_SYSTEM	_os_MS_DOS
#		define _RUN_TIME	_rt_MSC16
#		define _PROCESSOR	_pc_IX86
#	elif defined (M_I86CM)		/* Microsoft, prior to visual C++ */
#		define _MEM_MODEL	_mm_COMPACT
#		define _FILE_SYSTEM	_fs_MS_DOS			
#		define _OPR_SYSTEM	_os_MS_DOS
#		define _RUN_TIME	_rt_MSC16
#		define _PROCESSOR	_pc_IX86
#	elif defined (M_I86MM)		/* Microsoft, prior to visual C++ */
#		define _MEM_MODEL	_mm_MEDIUM
#		define _FILE_SYSTEM	_fs_MS_DOS
#		define _OPR_SYSTEM	_os_MS_DOS
#		define _RUN_TIME	_rt_MSC16
#		define _PROCESSOR	_pc_IX86
#	elif defined (M_I86LM)		/* Microsoft, prior to visual C++ */
#		define _MEM_MODEL	_mm_LARGE
#		define _FILE_SYSTEM	_fs_MS_DOS
#		define _OPR_SYSTEM	_os_MS_DOS
#		define _RUN_TIME	_rt_MSC16
#		define _PROCESSOR	_pc_IX86
#	elif defined (M_I86HM)		/* Microsoft, prior to visual C++ */
#		define _MEM_MODEL  	_mm_HUGE
#		define _FILE_SYSTEM	_fs_MS_DOS
#		define _OPR_SYSTEM	_os_MS_DOS
#		define _RUN_TIME	_rt_MSC16
#		define _PROCESSOR	_pc_IX86
#	elif defined (__HP_aCC) || defined (hpux) || defined (__hpux__) || defined (__hpux)
#		define _MEM_MODEL  	_mm_VIRTUAL
#		define _FILE_SYSTEM	_fs_UNIX
#		define _OPR_SYSTEM	_os_UNIX
#		define _RUN_TIME	_rt_HPUX
#		define _PROCESSOR	_pc_GENERIC_32
#	elif defined (_AIX)
#		define _MEM_MODEL  	_mm_VIRTUAL
#		define _FILE_SYSTEM	_fs_UNIX
#		define _OPR_SYSTEM	_os_UNIX
#		define _RUN_TIME	_rt_AIX
#		define _PROCESSOR	_pc_GENERIC_32
#	elif defined (__APPLE__)
#		define _MEM_MODEL  	_mm_VIRTUAL
#		define _FILE_SYSTEM	_fs_UNIX
#		define _OPR_SYSTEM	_os_UNIX
#		define _RUN_TIME	_rt_MOSXUNIX
#		define _PROCESSOR	_pc_GENERIC_32
#	else						/* The default, if all else fails. */
#		define _MEM_MODEL  	_mm_FLAT
#		define _FILE_SYSTEM	_fs_UNIX
#		define _OPR_SYSTEM	_os_UNIX
#		define _RUN_TIME	_rt_UNIXPCC
#		define _PROCESSOR	_pc_GENERIC_32
#	endif
#endif

#ifndef _MEM_MODEL
#	error Memory model not defined!!!
#endif
#ifndef _FILE_SYSTEM
#	error File system not defined!!!
#endif
#ifndef _OPR_SYSTEM
#	error Operating system not defined!!!
#endif
#ifndef _RUN_TIME
#	error Run time library not defined!!!
#endif
#ifndef _PROCESSOR
#	error Run time library not defined!!!
#endif

#if _RUN_TIME == _rt_MSWIN64
#	if defined (__MFC__)
#		define _WIN32_WINDOWS 0x0410
#		define WINVER 0x0410
#	endif
#endif

/* If this is a Microsoft compiler, use pragma's to turn off several warnings
   which are more of a nusiance then a help. */
#if defined (_MSC_VER) && _MSC_VER >= 800	/* MS Visual C++ 1.0 or later */
#	pragma warning( disable : 4090 )		// bypassing compiler 'const' protection
#	pragma warning( disable : 4100 )		// unreferenced formal parameter
#	pragma warning( disable : 4251 )		// XXX needs to have dll-interface to be used by clients of class YYY (whatever that means)
#	pragma warning( disable : 4273 )		// inconsistent DLL linkage (whatever that means)
#	pragma warning( disable : 4275 )		// non DLL-interface classkey 'identifier' used as base for DLL-interface classkey 'identifier' (whatever that means)
#	pragma warning( disable : 4290 )		// C++ exception specification ignored except to indicate a function is not __declspec(nothrow)
#	pragma warning( disable : 4702 )		// unreachable code, produced by several STL templates
#	pragma warning( disable : 4786 )		// symbol name truncated to 256 chars in debug database
#	pragma warning( disable : 4800 )		// forcing value to bool true/false (performance warning)
#	pragma warning( disable : 4996 )		// POSIX name for this item is deprecated.

#	pragma warning( disable : 6330 )		// Analyze: isspace et. al. require an unsigned argument, we usually provide a char
#	pragma warning( disable : 6303 )		// Analyze: doesn't line use of the %S format specifier in swprintf
#	pragma warning( disable : 6054 )		// Source argument (arg 2) may not be null terminiated (security issue)
#endif

#if _MEM_MODEL == _mm_LARGE	|| _MEM_MODEL == _mm_COMPACT || _MEM_MODEL == _mm_HUGE
#	define Huge huge		/* Used only where pointer comparisons are made. */
#else
#	define Huge				/* Essentially disabled for most current systems. */
#endif

/* Constants can be a pain in C, but not having them in C++ is a bigger pain.
   If we want to be ANSI, we need use const.  We use a special Const define
   here so you can switch if you have to. */

#define Const const

/*
	The following are used in an attempt to solve alignment
	problems experienced on certain Sun compilers. Essentially,
	they are used to force a double alignment before the
	definition of a structure or union, either static or
	automatic. We need lots of them in order to keep the
	variable names from duplicating themselves.
*/
#if   _RUN_TIME == _rt_SUN32 || _RUN_TIME == _rt_SUN64
#	define __ALIGNMENT__1 double __dummy__1;
#	define __ALIGNMENT__2 double __dummy__2;
#	define __ALIGNMENT__3 double __dummy__3;
#	define __ALIGNMENT__4 double __dummy__4;
#	define __ALIGNMENT__5 double __dummy__5;
#	define __ALIGNMENT__6 double __dummy__6;
#	define __ALIGNMENT__10 static double __dummy__10;
#	define __ALIGNMENT__11 static double __dummy__11;
#	define __ALIGNMENT__12 static double __dummy__12;
#	define __ALIGNMENT__13 static double __dummy__13;
#	define __ALIGNMENT__14 static double __dummy__14;
#	define __ALIGNMENT__15 static double __dummy__15;
#	define __ALIGNMENT__16 static double __dummy__16;
#else
#	define __ALIGNMENT__1
#	define __ALIGNMENT__2
#	define __ALIGNMENT__3
#	define __ALIGNMENT__4
#	define __ALIGNMENT__5
#	define __ALIGNMENT__6
#	define __ALIGNMENT__10
#	define __ALIGNMENT__11
#	define __ALIGNMENT__12
#	define __ALIGNMENT__13
#	define __ALIGNMENT__14
#	define __ALIGNMENT__15
#	define __ALIGNMENT__16
#endif

/*
	Where it used to be appropriate, CS_MAP functions declare
	certain variables, usually pointers of some sort, as
	cs_Register type variables.  The following define is used
	to control whether you want the compiler to see these
	specifications or not.  The default is no.
*/

#define cs_Register

/*
	The following defines are used control the function
	calling convention of modules in different environments.

	Every function is declared with one of these, depending
	upon its level.  The levels currently used are listed
	below.  The holes in the scheme are resevred for future
	use.  The ifndef's enable the specification of these
	defines on the command line.

	EXP_DATA	Used to indicate possible references to
				data in a DLL.

	EXP_LVL1	High Level Interface, what used to be
				called the Single Function Interface.  This
				is typically the interface provided for
				Visual Basic et. al.

	EXP_LVL3	Highest Performance Interface, what used to
				be called the Three/Six Function interface.
				Usually used inside of heavy duty applications.

	EXP_LVL5	Support functions accessible to, but not
				normally used by users.

	EXP_LVL7	Internal functions which may be of some value
				independent of the other lowest level functions;
				not normally expected to be accessed by users.

	EXP_LVL9	Lowest level functions which cannot be successfully
				used independent of other functions.
*/

#ifndef EXP_LVL1
#	if defined (DLL_16)
#		define EXP_LVL1 _pascal
#		define EXP_DATA
#	elif defined (DLL_32)
#		define EXP_LVL1 __stdcall
#		define EXP_DATA __declspec(dllimport)
#	elif defined (DLL_64)
#		define EXP_LVL1 __stdcall
#		define EXP_DATA __declspec(dllimport)
#	else
#		define EXP_LVL1
#		define EXP_DATA
#	endif
#endif
#ifndef EXP_LVL2
#	define EXP_LVL2
#endif
#ifndef EXP_LVL3
#	if defined (DLL_16)
#		define EXP_LVL3 _pascal
#	else
#		define EXP_LVL3
#	endif
#endif
#ifndef EXP_LVL4
#	define EXP_LVL4
#endif
#ifndef EXP_LVL5
#	define EXP_LVL5
#endif
#ifndef EXP_LVL6
#	define EXP_LVL6
#endif
#ifndef EXP_LVL7
#	define EXP_LVL7
#endif
#ifndef EXP_LVL8
#	define EXP_LVL8
#endif
#ifndef EXP_LVL9
#	define EXP_LVL9
#endif

/*
	We now include the necessary include files.  We do this
	once, here, to insulate individual code modules from the
	various different locations/names which various
	compilers/platforms may use.  If there is a standard for
	these things, it does seem to be followed very well.

	The following enables CS_MAP to be compiled with all
	five C compilers we have.  Changes may be necessary
	for your compiler.

	Note, also, we now include all files for all modules.
	We have chosen to do this as precompiled headers
	is a common feature and thus including all of them
	actually speeds things up.  Why is it that the
	slowest compilers, i.e. the ones that need pre-compiled
	headers the most, don't support it???

	There is one exception to this.  <ctype.h> is included
	directly in the modules that require it.
*/

/* The following is a list of includes which appears to work on
   all systems. */
#	include <stdlib.h>
#	include <limits.h>
#	include <stdio.h>
#if (_RUN_TIME == _rt_BORLAND32)
#	include <wctype.h>
#ifndef __cplusplus
#	include <math.h>
#endif
#else
#	include <math.h>
#endif
#	include <string.h>

/* Deal with the Unix, Dos, Windows, and Windows CE variations here.
   Note, the Microsoft Embedded Visual C++ compiler issues bogus
   messages concerning include files which it can't find when
   processsing the following.  I spent an hour trying to figure
   out what it doesn't like with no luck.  The modules seem to
   compile OK. */
#if _RUN_TIME == _rt_WC386
#	include <time.h>
#elif _RUN_TIME == _rt_WC286
#	include <time.h>
#elif _RUN_TIME == _rt_MSC16
#	include <time.h>
#elif _RUN_TIME == _rt_MSVC16
#	include <time.h>
#elif _RUN_TIME == _rt_MSDOTNET
#	include <time.h>
#elif _RUN_TIME == _rt_MSWIN64
#	include <time.h>
#elif _RUN_TIME == _rt_MSVC32
#	include <time.h>
#elif _RUN_TIME == _rt_METAWARE
#	include <time.h>
#elif _RUN_TIME == _rt_BORLAND16
#	include <time.h>
#elif _RUN_TIME == _rt_BORLAND32
#	include <time.h>
#elif _RUN_TIME == _rt_UNIXPCC
#	include <time.h>
#	include <malloc.h>
#	include <sys/types.h>
#	include <sys/stat.h>
#	include <unistd.h>
#elif _RUN_TIME == _rt_SUN32
#	include <time.h>
#	include <malloc.h>
#	include <sys/types.h>
#	include <sys/stat.h>
#	include <unistd.h>
#       include <wctype.h>
#elif _RUN_TIME == _rt_SUN64
#	include <time.h>
#	include <malloc.h>
#	include <sys/types.h>
#	include <sys/stat.h>
#	include <unistd.h>
#       include <wctype.h>
#elif _RUN_TIME == _rt_MOSXUNIX
#	include <time.h>
#	include <sys/types.h>
#	include <sys/stat.h>
#elif _RUN_TIME == _rt_HPUX
#	include <time.h>
#	include <malloc.h>
#	include <sys/types.h>
#	include <sys/stat.h>
#	include <unistd.h>
#elif _RUN_TIME == _rt_AIX
#	include <time.h>
#	include <malloc.h>
#	include <sys/types.h>
#	include <sys/stat.h>
#	include <unistd.h>
#else							/* default to a generic UNIX configuation. */
#	include <time.h>
#	include <malloc.h>
#	include <sys/types.h>
#	include <sys/stat.h>
#	include <unistd.h>
#endif

#define csFILE     FILE
#if _FILE_SYSTEM != _fs_UNIX
#	define CS_fopen   fopen
#endif
#define CS_fread   fread
#define CS_fwrite  fwrite
#define CS_fclose  fclose
#define CS_fseek   fseek
#define CS_ftell   ftell
#define CS_fflush  fflush
#define CS_feof    feof
#define CS_ferror  ferror
#define CS_fgetc   fgetc
#define CS_fputc   fputc
#define CS_fgets   fgets
#define CS_fputs   fputs
#define CS_fprintf fprintf
#define CS_sprintf sprintf

/*
	In order to be compatible with various C compilers,
	modify these defines as required.  They are used to
	force binary/text mode of file open as required.
	
	The following are suitable for most compilers.

	The O_BINARY and O_TEXT defines are no longer used
	by CS-MAP.  Maintained for legacy purposes only.
*/
#if _RUN_TIME == _rt_UNIXPCC || _RUN_TIME == _rt_SUN32 || _RUN_TIME == _rt_SUN64 || _RUN_TIME == _rt_MOSXUNIX || _RUN_TIME == _rt_HPUX
#	ifndef O_BINARY
#		define O_BINARY 0x8000	/* used for mode in open (); */
#	endif
#	ifndef O_TEXT
#		define O_TEXT 0x4000	/* used for mode in open (); */
#	endif
	/* Text Mode */
#	define _STRM_TXTRD "r"		/* open an existing file for read only */
#	define _STRM_TXTWR "w"		/* create a new file (or truncate old) for writing only */
#	define _STRM_TXTRW "w+"		/* open new file for read and write */
#	define _STRM_TXTUP "r+"		/* open existing file for read and write */
	/* Binary Mode */
#	define _STRM_BINRD "r"		/* open an existing file for read only */
#	define _STRM_BINWR "w"		/* create a new file (or truncate old) for writing only */
#	define _STRM_BINRW "w+"		/* open existing file for read and write */
#	define _STRM_BINUP "r+"		/* open existing file for read and write */
#else
	/* Text Mode */
#	define _STRM_TXTRD "rt"		/* open an existing file for read only */
#	define _STRM_TXTWR "wt"		/* create a new file (or truncate old) for writing only */
#	define _STRM_TXTRW "w+t"	/* open existing file for read and write */
#	define _STRM_TXTUP "r+t"	/* open existing file for read and write */
	/* Binary Mode */
#	define _STRM_BINRD "rb"		/* open an existing file for read only */
#	define _STRM_BINWR "wb"		/* create a new file (or truncate old) for writing only */
#	define _STRM_BINRW "w+b"	/* open existing file for read and write */
#	define _STRM_BINUP "r+b"	/* open existing file for read and write */
#endif

/* These defines are necessary to compile under Linux, Unix, etc.
   Microsoft uses the underbar character to indicate functions which are
   not part of the ANSI standard. */
#if _RUN_TIME >= _rt_UNIXPCC
#	define _stat stat
#	define _fileno fileno
#endif

#if _RUN_TIME == _rt_MSWIN64
	typedef __time32_t cs_Time_;
#else
	typedef time_t cs_Time_;
#endif

/* 
   The following typedefs are used to provide some portability in the
   64 bit processor area.  Specifically (at this time), these typedef's
   are used wherever a long or unsigned long has been traditionally used.
   We use them where a 4 byte (32 bit) quantity is required for proper
   operation of the system.
   
   Adjust these type defs for you environment as necessary to produce a
   32 bit integral type.
 */

#if ((LONG_MAX - 4) > 2147483647L)		/* How big is a long??? */
	/* It appears that a long is an 8 byte object (longer than 4 anyway). */
	typedef int long32_t;				/* This assumes an int is 32 bits. */
	typedef unsigned int ulong32_t;		/* This assumes an unsigned int is 32 bits. */
#else
	/* It appears that a long is 32 bits. */
	typedef long long32_t;
	typedef unsigned long ulong32_t;
#endif

/******************************************************************************
*******************************************************************************
*******************************************************************************
***                                                                         ***
***   It is the intent of the designer of this mess that all system         ***
***  dependencies should be dealt with above this point in this header.     ***
***  What follows should work on just about any system.  Of course, I       ***
***  think Custer said something similar to that also.                      ***
***                                                                         ***
*******************************************************************************
*******************************************************************************
******************************************************************************/

/*
	There used to be a constant, defined in CSdata, named cs_MoloK,
	which was the sine of one second of arc.  Since this constant
	is now used in many places other than the Molodensky
	Transformation, we have changed its name to cs_Sin1Sec.

	The following define is to keep things working until all
	references to cs_MoloK are worked out of the code.
*/

#define cs_MoloK cs_Sin1Sec

/*
	The following defines the type of a magic number which
	identifies Coordinate System, Datum, and Ellipsoid
	dictionary files.
*/

typedef long32_t cs_magic_t;

/*
	If not yet defined elsewhere:
*/

#ifndef NULL
#	define NULL 0
#endif

#ifndef NULLFP
#	define NULLFP (int (*)())0
#endif

#ifndef TRUE
#	define TRUE 1
#endif

#ifndef FALSE
#	define FALSE 0
#endif

#ifndef PI
#	define PI 3.1415926535897932384626434
#endif

#ifndef PI_OVR_2
#	define PI_OVR_2 1.5707963267948966192
#endif

#ifndef PI_OVR_4
#	define PI_OVR_4 0.785398163397448309615
#endif

#ifndef ONE_DEGREE
#	define ONE_DEGREE 0.01745329251994329577
#endif

#ifndef RADIAN
#	define RADIAN 57.2957795130823208772
#endif

/*
	The following is used as an approximation of infinity.  We
	specifically use a value which is small enough so that the
	value can be manipulated in the normal fashion without causing
	floating point exceptions.  Keeping the value significantly
	less than 1.0E+38 means it will work, to a certain extent, for
	floats as well as doubles.
	
	Note, at version 11.0, Microsoft's Visual Studio started
	defining a value for INFINITY which is quite contrary to the
	above described characteristics.  Therefore, we simply
	avoid all use of this macro as of 12 Aug 2014.
*/

#ifndef XX
#	define XX 0
#endif

#ifndef YY
#	define YY 1
#endif

#ifndef ZZ
#	define ZZ 2
#endif

#ifndef LNG
#	define LNG 0
#endif

#ifndef LAT
#	define LAT 1
#endif

#ifndef HGT
#	define HGT 2
#endif

#ifndef MAXINT
#	define MAXINT 32767	/* Minimum value which should be safe
						   in any environment. */
#endif

/*
	The following define valid values for the type argument
	if the CSdfltpro function.  This is used to specify which
	default is being processed, and thereby eliminates the need
	to duplicate a lot of code.
*/

#define cs_DFLTSW_DT 1
#define cs_DFLTSW_EL 2
#define cs_DFLTSW_LU 3
#define cs_DFLTSW_AU 4

/*
	The following define the characters used in the default
	processing scheme.  The IDNT characters are used to
	surround a value which is defaultable.  The CS_MAP
	library uses the REPL characters to surround a replaced
	value, indicating that the value came from a default
	substitutition.

	Since the IDNT characters actually end up in the dictionaries,
	it is important that these characters don't change.
	Therefore, we restict them to compile time options only
	by defining them in this header file.
*/

#define cs_DFLT_IDNTBEG '['
#define cs_DFLT_IDNTEND ']'
#define cs_DFLT_REPLBEG '('
#define cs_DFLT_REPLEND ')'

/*
	The following defines define possible values of the bit mapped
	argument to the CS_cschk function.  These options make it
	possible to use this function in a variety of different
	ways.  The Coordinate System compiler environment, where the
	datums and ellipsoids need to be checked against the dictionaries
	being generated (not necessarily the currently active one) is the
	primary reason for all of this.
*/

#define cs_CSCHK_DATUM	1	/* Turns on checking of the datum key
							   name against the currently active
							   datum dictionary. */
#define cs_CSCHK_ELLIPS	2	/* Turns on checking of the ellipsoid
							   key name against the currently
							   active ellipsoid dictionary. */
#define cs_CSCHK_REPORT 4	/* Instructs CS_cschk to report all
							   errors to CS_erpt. */

/*
	The following define the return value possibilities from
	the coordinate conversion functions.

	The first three defines are bit map values.  These are
	returned by the CS_xychk, CS_llchk, CS?????X, and CS?????L
	functions.

	The second three are simple status values returned by the
	forward and inverse conversion functions.
*/

#define cs_CNVRT_OK    0
#define cs_CNVRT_USFL  1
#define cs_CNVRT_DOMN  2
#define cs_CNVRT_ERR   4096

#define cs_CNVRT_NRML  cs_CNVRT_OK
#define cs_CNVRT_INDF  cs_CNVRT_USFL
#define cs_CNVRT_RNG   cs_CNVRT_DOMN
#define cs_CNVRT_DEMO   4095

/*
	The following define the bits allocated for the return
	value from the CS_basic functions.
*/

#define cs_BASIC_SRCRNG 1	/* Source coordinate is outside the
							   domain of the source coordinate
							   system. */
#define cs_BASIC_DSTRNG 2	/* The source geographic coordinate was
							   outside the range of the datum
							   conversion algorithm/database. */
#define cs_BASIC_DTCWRN 4	/* The possibly shifted geographic
							   coordinates were outside the domain
							   of the destination (target) coordinate
							   system. */
#define cs_BASIC_DEMO   128	/* The system is operating in demonstration
							   mode. */

/*
	The following manifest constants are used to control the
	processing of non-standard cartesian systems, e.g. a
	left handed cartesian system.

	These constants are used to form bit maps, so all values
	must be a power of two.
*/

#define cs_QUAD_INVX   1
#define cs_QUAD_INVY   2
#define cs_QUAD_SWAP   4
#define cs_QUAD_MASK   0x7
#define cs_QUAD_RSRVD  8		/* Reserved for use by MSI. */

/*
	The declared size of all character arrays used to
	carry path names.  Largest name supported is one
	less than this value.
*/

#ifndef MAXPATH
#	if _RUN_TIME >= _rt_UNIXPCC
#		define MAXPATH 2048
#	else
#		define MAXPATH 260
#	endif
#endif

#define cs_ERRMSG_SIZE 256
#define csMAXPATH 260

/*
	We define another copy of this value as the inclusion
	of certain system files has an equivalent define.  In
	these routines, we use the system value if it gets
	defined, otherwise we use CSMAXPATH.
*/

#if _RUN_TIME >= _rt_UNIXPCC
#	define CSMAXPATH 2048
#else
#	define CSMAXPATH 260
#endif

/*
	The following are used to create option bit maps for
	the CS??comp functions.  These are functions called
	by the Coordinate System Compiler main line.
*/

#define cs_CMPLR_DEMO     1
#define cs_CMPLR_CRYPT    2
#define cs_CMPLR_TEST     4
#define cs_CMPLR_WARN     8
#define cs_CMPLR_EXTENTS 16

/*
	The following define specifies the size of the arrays
	used to carry the dictionary file names and the datum
	conversion catalog file names.  Thus, this limits only
	the size of the names of the dictionaries.  This value
	includes the file name, extension, separator(i.e. the dot),
	and the terminating null character.  While 64 is perhaps
	small by today's standards,	it should be plenty for
	dictionary files, etc.
*/

#define cs_FNM_MAXLEN 64

/*
	The following defines are used in the Double/ASCII conversion
	functions provided with CS-MAP (CS_atof and CS_ftoa).

	CS_atof returns a long which contains the following information.
	CS_ftoa accepts a long with this information and uses it to
	format the ASCII result.
*/

#define cs_ATOF_PRCMSK    ( 0x1F)	/* # of digits (plus one) after the
									   decimal point (zero value reserved
									   as a special flag */
#define cs_ATOF_VALLNG  (1L <<  5)	/* Ignored by ftoa. Set by atof if
									   value is valid for longitude */
#define cs_ATOF_VALLAT  (1L <<  6)	/* Ignored by ftoa. Set by atof if
									   value is valid for latitude. */
#define cs_ATOF_MINSEC  (1L <<  7)	/* minutes and seconds were used */
#define cs_ATOF_MINUTE  (1L <<  8)	/* minutes only were used */
#define cs_ATOF_FRACTN  (1L <<  9)	/* a fractional portion was present */
#define cs_ATOF_EXPNT   (1L << 10)	/* value was in 1.0E-01 form */
#define cs_ATOF_OVRFLW  (1L << 11)	/* value is in *.* (overflow) form */
#define cs_ATOF_COMMA   (1L << 12)	/* Commas were present */
#define cs_ATOF_DIRCHR  (1L << 13)	/* A valid directional characters was
									   present. */
#define cs_ATOF_XEAST   (1L << 14)	/* True indicates that the value
									   should be considered a longitude,
									   easting, or X value for the
									   purposes of choosing a directional
									   character. */
#define cs_ATOF_MINSEC0 (1L << 15)	/* Include leading zeros on minutes and
									   seconds. */
#define cs_ATOF_DEG0    (1L << 16)	/* Include leading zeros on minutes and
									   seconds. */
#define cs_ATOF_0BLNK   (1L << 17)	/* Output a zero value as a null
									   string. On input, set for a
									   null string input. */
#define cs_ATOF_FORCE3	(1L << 18)	/* Forces 3 characters for degrees
									   when deg/min/sec output is used
									   for output. Useful in outputing
									   longitudes. */
#define cs_ATOF_RATIO   (1L << 19)	/* Forces output as a ratio, i.e.
									   1:17,000. Other flags, such as
									   COMA and precision apply to the
									   numeric value. */
#define cs_ATOF_SECS60  (1L << 20)	/* What was expected to be seconds
									   evaluated to a value >= 60 */
#define cs_ATOF_MINS60  (1L << 21)	/* What was expected to be minutes
									   evaluated to a value >= 60 */
#define cs_ATOF_MLTPNT  (1L << 22)	/* Multiple decimal point indications
									   were present */
#define cs_ATOF_MLTSGN  (1L << 23)	/* Multiple sign indications were
									   present. */
#define cs_ATOF_ERRCMA  (1L << 24)	/* Commas were encountered where
									   unexpected. */
#define cs_ATOF_FMTERR  (1L << 25)	/* General format error. */
#define cs_ATOF_RATERR  (1L << 26)	/* Ratio format error. */

#define cs_ATOF_RNG360  (1L << 29)	/* 360 Range flag. */

#define cs_ATOF_ERRFLG  (0x80000000L)	/* General error flag. */

/*
	Some commonly used default format specifications for
	formatting coordinates, scale factors, and convergence
	angles.
*/

#define cs_ATOF_XXXDFLT (cs_ATOF_COMMA  | 3L)
#define cs_ATOF_YYYDFLT (cs_ATOF_COMMA  | 3L)
#define cs_ATOF_LNGDFLT (cs_ATOF_MINSEC | cs_ATOF_MINSEC0 | cs_ATOF_DIRCHR | cs_ATOF_XEAST | cs_ATOF_FORCE3 | 3L)
#define cs_ATOF_LATDFLT (cs_ATOF_MINSEC | cs_ATOF_MINSEC0 | cs_ATOF_DIRCHR | 3L)
#define cs_ATOF_SCLDFLT (7L)
#define cs_ATOF_CNVDFLT (cs_ATOF_MINSEC | cs_ATOF_MINSEC0 | cs_ATOF_DIRCHR | cs_ATOF_XEAST | 1L)
#define cs_ATOF_REDDFLT (cs_ATOF_RATIO  | cs_ATOF_COMA    | 3L;

/*
	The following define the use of the bits in the
	projection table flags word.  Also defined is a
	unique code value for each projection.  Thus,
	code can be written completely independent of
	the name provided.
*/

#define cs_PRJCOD_END       0
#define cs_PRJCOD_UNITY     1
/*#define cs_PRJCOD_LMBRT     2       Obsolete, not to be reused. */
#define cs_PRJCOD_TRMER     3
#define cs_PRJCOD_ALBER     4
#define cs_PRJCOD_OBLQM     5
#define cs_PRJCOD_MRCAT     6
#define cs_PRJCOD_AZMED     7
#define cs_PRJCOD_LMTAN     8
#define cs_PRJCOD_PLYCN     9
#define cs_PRJCOD_MODPC    10
#define cs_PRJCOD_AZMEA    11
#define cs_PRJCOD_EDCNC    12
#define cs_PRJCOD_MILLR    13
/*#define cs_PRJCOD_STERO    14		Obsolete, not to be reused. */
#define cs_PRJCOD_MSTRO    15
#define cs_PRJCOD_NZLND    16
#define cs_PRJCOD_SINUS    17
#define cs_PRJCOD_ORTHO    18
#define cs_PRJCOD_GNOMC    19
#define cs_PRJCOD_EDCYL    20       /* Deprecated, preserved for LEGACY use only.
									   Use cs_PRJCOD_EDCYLE which supports both the
									   spherical form and the ellipsoidal form of
									   this projection.  The original form is retained
									   only to maintain consistency of definitions
									   which referenced this system to a datum and
									   thus to an ellipsoid. */
#define cs_PRJCOD_VDGRN    21
#define cs_PRJCOD_CSINI    22
#define cs_PRJCOD_ROBIN    23
#define cs_PRJCOD_BONNE    24
#define cs_PRJCOD_EKRT4    25
#define cs_PRJCOD_EKRT6    26
#define cs_PRJCOD_MOLWD    27
#define cs_PRJCOD_HMLSN    28
#define cs_PRJCOD_NACYL    29
#define cs_PRJCOD_TACYL    30
#define cs_PRJCOD_BPCNC    31
#define cs_PRJCOD_SWISS    32
#define cs_PRJCOD_PSTRO    33		/* Polar sterographic */
#define cs_PRJCOD_OSTRO    34		/* Oblique Sterographic */
#define cs_PRJCOD_SSTRO    35		/* Synder's Oblique Sterographic */
#define cs_PRJCOD_LM1SP    36		/* Single standard parallel variation
									   of the Lambert Conformal Conic. */
#define cs_PRJCOD_LM2SP    37		/* Double standard parallel variation
									   of the Lambert Conformal Conic. */
#define cs_PRJCOD_LMBLG    38		/* Belgian variation of the Lambert
									   Conformal Conic Projection. */
#define cs_PRJCOD_WCCSL    39		/* Wisconsin County Coordinate System
									   variation of the Lambert Conformal
									   Conic. */
#define cs_PRJCOD_WCCST    40		/* Wisconsin County Coordinate System
									   variation of the Transverse Mercator
									   projection. */
#define cs_PRJCOD_MNDOTL   41		/* Minnesota Department of Transportation
									   variation of the Lambert Conformal
									   Conic. */
#define cs_PRJCOD_MNDOTT   42		/* Minnesota Department of Transportation
									   variation of the Transverse Mercator
									   projection. */
#define cs_PRJCOD_SOTRM    43		/* South Oriented variation of the
									   Transverse Mercator Projection. */
#define cs_PRJCOD_UTM      44		/* The UTM direct variation of the
									   Transverse Mercator projection. */
#define cs_PRJCOD_TRMRS    45		/* Transverse Mercator per
									   J. P. Snyder. */
#define cs_PRJCOD_GAUSSK   46		/* Gauss-Kruger: Transverse Mercator without
									   scale reduction parameter. */
#define cs_PRJCOD_KROVAK   47		/* Czech Krovak, with precise origin. */
#define cs_PRJCOD_KROVK1   48		/* Obsolete, do not reuse.  Retained for
									   compatibility with previous releases
									   only. Czech Krovak, with rounded origin. */
#define cs_PRJCOD_MRCATK   49		/* Standard Mercator with a scale reduction
									   factor instead of a standard parallel. */
#define cs_PRJCOD_OCCNC    50		/* Oblique conformal conic. */
#define cs_PRJCOD_KRVK95   51		/* Czech Krovak, with precise origin, includes
									   S-JTSK/95 adjustment. */
#define cs_PRJCOD_KRVK951  52		/* Obsolete, do not reuse.  Retained for
									   compatibility with previous releases
									   only. Czech Krovak, with rounded origin,
									   includes S-JTSK/95 adjustment. */
#define cs_PRJCOD_PSTROSL  53		/* Polar sterographic with standard latitude */
#define cs_PRJCOD_TRMERAF  54		/* Transverse Mercator with affiine post-processor. */
#define cs_PRJCOD_NERTH    55		/* Non-georeferenced coordinate system.  Named
									   Non-Earth by Map Info. */
#define cs_PRJCOD_OBQCYL   56		/* Oblique Cylindrical, a generalized version of the
									   Swiss projection, specifically for Hungary. */
#define cs_PRJCOD_SYS34    57		/* Combination of Transverse Mercator and a polynomial
									   expansion used in Denmark. */
#define cs_PRJCOD_OSTN97   58		/* The Transverse Mercator with specific parameters,
									   with the OSTN97 grid shift tacked on.  This is
									   a combination of a projection and a datum shift. */
#define cs_PRJCOD_AZEDE    59		/* Azimuthal Equi-Distant, Elevated ellipsoid. */
#define cs_PRJCOD_OSTN02   60		/* The Transverse Mercator with specific parameters,
									   with the OSTN02 grid shift tacked on.  This is
									   a combination of a projection and a datum shift. */
#define cs_PRJCOD_SYS34_99 61		/* Combination of Transverse Mercator and polynomial
									   expansion used in Denmark.  Polynominals are of the
									   1999 vintage. */
#define cs_PRJCOD_TRMRKRG  62		/* Variation of the Transverse Mercator which uses the
									   Kruger Formulation. */
#define cs_PRJCOD_WINKL    63		/* Winkel-Tripel projection. */
#define cs_PRJCOD_NRTHSRT  64		/* Nerth with scale and rotation.  */
#define cs_PRJCOD_LMBRTAF  65		/* Lambert Conformal Conic with affiine post-processor. */
#define cs_PRJCOD_SYS34_01 66		/* Combination of Transverse Mercator and polynomial
									   expansion used in Denmark.  Polynominals are of the
									   1999 vintage, except for Bornholm, which are post
									   1999. */
#define cs_PRJCOD_EDCYLE   67		/* Equidistant Cylindrical, ellipsoid form supported.
									   This variation replaces the original variation which
									   only supported the psherical form of this projection. */
#define cs_PRJCOD_PCARREE  68		/* Plate Carree, standard form.  This is _NOT_ the same
									   as EPSG 9825 - Pseudo Plate Carree. */
#define cs_PRJCOD_MRCATPV  69		/* Psuedo Mercator, Popular Visualization. */
#define cs_PRJCOD_LMMICH   70		/* Lambert Conformal Conic,.Michigan Variation */

#define cs_PRJCOD_HOM1UV   ((cs_PRJCOD_OBLQM << 8) + 1)
#define cs_PRJCOD_HOM1XY   ((cs_PRJCOD_OBLQM << 8) + 2)
#define cs_PRJCOD_HOM2UV   ((cs_PRJCOD_OBLQM << 8) + 3)
#define cs_PRJCOD_HOM2XY   ((cs_PRJCOD_OBLQM << 8) + 4)
#define cs_PRJCOD_RSKEW    ((cs_PRJCOD_OBLQM << 8) + 5)
#define cs_PRJCOD_RSKEWC   ((cs_PRJCOD_OBLQM << 8) + 6)
#define cs_PRJCOD_RSKEWO   ((cs_PRJCOD_OBLQM << 8) + 7)

#define cs_PRJCOD_NOTYET   500
#define cs_PRJCOD_LABORD   (cs_PRJCOD_NOTYET + 1)
#define cs_PRJCOD_TUNGRD   (cs_PRJCOD_NOTYET + 2)
#define cs_PRJCOD_LMNC     (cs_PRJCOD_NOTYET + 3)
#define cs_PRJCOD_AZMEAS   (cs_PRJCOD_NOTYET + 4)
#define cs_PRJCOD_PSEUDO   (cs_PRJCOD_NOTYET + 5)
#define cs_PRJCOD_LMWO     (cs_PRJCOD_NOTYET + 6)

#define cs_PRJFLG_SPHERE  (1L <<  0)	/* Sphere supported. */
#define cs_PRJFLG_ELLIPS  (1L <<  1)	/* Ellipsoid supported. */

#define cs_PRJFLG_SCALK   (1L <<  2)	/* Analytical K available */
#define cs_PRJFLG_SCALH   (1L <<  3)	/* Analytical H available */
#define cs_PRJFLG_CNVRG   (1L <<  4)	/* Analytical C available */

#define cs_PRJFLG_CNFRM   (1L <<  5)	/* Conformal */
#define cs_PRJFLG_EAREA   (1L <<  6)	/* Equal area */
#define cs_PRJFLG_EDIST   (1L <<  7)	/* Equal distant, either h or
										   k is always 1. */
#define cs_PRJFLG_AZMTH   (1L <<  8)	/* Azimuthal */
#define cs_PRJFLG_GEOGR   (1L <<  9)	/* Geographic coordinates */

										/* Modifiers to surface type: */
#define cs_PRJFLG_OBLQ    (1L << 10)	/* Oblique */
#define cs_PRJFLG_TRNSV   (1L << 11)	/* Transverse */
#define cs_PRJFLG_PSEUDO  (1L << 12)	/* Psuedo */
#define cs_PRJFLG_INTR    (1L << 13)	/* Interuptible */

						/* Surface Type */
						/* Generally considered to be: */
#define cs_PRJFLG_CYLND   (1L << 14)	/* Cylindrical */
#define cs_PRJFLG_CONIC   (1L << 15)	/* Conic */
#define cs_PRJFLG_FLAT    (1L << 16)	/* Azimuthal */
#define cs_PRJFLG_OTHER   (1L << 17)	/* Other */

						/* Additional processes */
#define cs_PRJFLG_AFFINE  (1L << 20)	/* Affine transformation post-processer */
#define cs_PRJFLG_RNTRNT  (1L << 21)	/* Projection code is reentrant */

						/* Parameter Control */
#define cs_PRJFLG_ORGFLS  (1L << 24)	/* The projection does NOT support
										   a false origin. */
#define	cs_PRJFLG_SCLRED  (1L << 25)	/* Scale Reduction is supported. */
#define cs_PRJFLG_ORGLAT  (1L << 26)	/* Projection does not use an
										   origin latitude paremeter. */
#define cs_PRJFLG_ORGLNG  (1L << 27)	/* Projection does not use an
										   origin longitude parameter.
										   In many cases, prj_prm1 is the
										   origin latitude and is labeled
										   central meridian. */

/*
	Except for the three bits described below, the remaining
	bits are reserved for future development by CS-MAP.
*/

#define cs_PRJFLG_USER1   (1L << 28)	/* Reserverd for application programmer */
#define cs_PRJFLG_USER2   (1L << 29)	/* Reserverd for application programmer */
#define cs_PRJFLG_USER3   (1L << 30)	/* Reserverd for application programmer */

/*
	The following codes are used to associate programming in the
	CS_prjprm module with the table defined in the CSdataPJ
	module. This somewhat indesirable situation arises from
	our desire to keep all English language strings in as few
	modules as possible.  A specific code value is, essentially,
	an index into the table defined in CSdataPJ.  A table
	within CS_prjprm defines the specific entries for a specific
	projection.

	Note that a specific parmeter, e.g. central meridian, is used
	for several projections.  We keep a single copy of this
	definition and reuse it for several projections.
*/

#define cs_PRMCOD_NOTUSED      0		/* Not used. */
#define cs_PRMCOD_CNTMER       1		/* Central Meridian */
#define cs_PRMCOD_NSTDPLL      2		/* Northern Standard Parallel */
#define cs_PRMCOD_SSTDPLL      3		/* Southern Standard Parallel */
#define cs_PRMCOD_STDPLL       4		/* Standard Parallel */
#define cs_PRMCOD_GCP1LNG      5		/* First Great Circle Point Longitude */
#define cs_PRMCOD_GCP1LAT      6		/* First Great Circle Point Latitude */
#define cs_PRMCOD_GCP2LNG      7		/* Second Great Circle Point Longitude */
#define cs_PRMCOD_GCP2LAT      8		/* Second Great Circle Point Latitude */
#define cs_PRMCOD_GCPLNG       9		/* Great Circle Point Longitude */
#define cs_PRMCOD_GCPLAT      10		/* Great Circle Point Latitude */
#define cs_PRMCOD_GCAZM       11		/* Great Circle Azimuth */
#define cs_PRMCOD_YAXISAZ     12		/* Y axis azimuth */
#define cs_PRMCOD_ESTDMER     13		/* Eastern Standard Meridian */
#define cs_PRMCOD_NPARALL     14		/* Northern Parallel */
#define cs_PRMCOD_SPARALL     15		/* Southern Parallel */
#define cs_PRMCOD_P1LNG       16		/* First pole longitude */
#define cs_PRMCOD_P1LAT       17		/* First pole latitude */
#define cs_PRMCOD_P2LNG       18		/* Second pole longitude */
#define cs_PRMCOD_P2LAT       19		/* Second pole latitude */
#define cs_PRMCOD_ADP1P2      20		/* Distance between the poles. */
#define cs_PRMCOD_ADSP1       21		/* Angular distance to the first
										   standard parallel */
#define cs_PRMCOD_ADSP2       22		/* Angular distance to the second
										   standard parallel */
#define cs_PRMCOD_CMPLXAN     23		/* Complex Parameter A (n).  Label
										   string includes a %s for
										   generation by sprintf. */
#define cs_PRMCOD_CMPLXBN     24		/* Complex Parameter B (n).  Label
										   string includes a %s for
										   generation by sprintf. */
#define cs_PRMCOD_WESTLL      25		/* Western end of Longitude range. */
#define cs_PRMCOD_EASTLL      26		/* Eastern end of Longitude range. */
#define cs_PRMCOD_UTMZN       27		/* UTM Zone Number */
#define cs_PRMCOD_HSNS        28		/* North/South Hemisphere */
#define cs_PRMCOD_GHGT        29		/* Average Geoid Height */
#define cs_PRMCOD_AELEV       30		/* Average Elevation */
#define cs_PRMCOD_POLELNG     31		/* Oblique Pole Longitude */
#define cs_PRMCOD_POLELAT     32		/* Oblique Pole Latitude */
#define cs_PRMCOD_OSTDPLL     33		/* Oblique Cone Standard Parallel */
#define cs_PRMCOD_STDCIR      34		/* Standard Circle Latitude */
#define cs_PRMCOD_AFA0        35		/* Affine Transformation A0 parameter */
#define cs_PRMCOD_AFB0        36		/* Affine Transformation B0 parameter */
#define cs_PRMCOD_AFA1        37		/* Affine Transformation A1 parameter */
#define cs_PRMCOD_AFA2        38		/* Affine Transformation A2 parameter */
#define cs_PRMCOD_AFB1        39		/* Affine Transformation B1 parameter */
#define cs_PRMCOD_AFB2        40		/* Affine Transformation B2 parameter */
#define cs_PRMCOD_NRMLPLL     41		/* Normal Parallel */
#define cs_PRMCOD_DENRGN      42		/* Danish System 24 Region */
#define cs_PRMCOD_SKWAZM      43		/* Rectified Skew Orthomorphic, Skew
										   Azimuth at projection origin. */
#define cs_PRMCOD_SCLROTORGX  44		/* X Coordinate of Scale/Roatet Origin */
#define cs_PRMCOD_SCLROTORGY  45		/* Y Coordinate of Scale/Roatet Origin */
#define cs_PRMCOD_NRTHSCL     46		/* Cartesian Scale Factor */
#define cs_PRMCOD_NRTHROT     47		/* Cartesian Rotation Angle */
#define cs_PRMCOD_ELPSCL      48		/* Ellipsoid Scale */
#define cs_PRMCOD_MAXIDX      48		/* Used to prevent erroneous memory access */

		/* These are not parameters as far as CS-MAP is concerned,
		   but they are in the WKT way of things.  So we invent some
		   bogus code values for them. */

#define cs_WKTCOD_FEAST    101
#define cs_WKTCOD_FNORTH   102
#define cs_WKTCOD_ORGLNG   103
#define cs_WKTCOD_ORGLAT   104
#define cs_WKTCOD_SCLRED   105

#define cs_WKTCOD_DELTAX   151
#define cs_WKTCOD_DELTAY   152
#define cs_WKTCOD_DELTAZ   153
#define cs_WKTCOD_ROTATX   154
#define cs_WKTCOD_ROTATY   155
#define cs_WKTCOD_ROTATZ   156
#define cs_WKTCOD_BWSCAL   157

#define cs_PRMCOD_INVLD    255

#define cs_PRMCOD_NOTYET   499

/******************************************************************************
	WKT Flavor -- The following enumerates the various flavors of WKT
	supported.  Generally this value is used to select the group of names which
	are searched to identify objects which are named in WKT; specifically
	projections, methods, parameters, etc.  Those flavors appearing after the
	wktFlvrUnknown tag are not yet supported. */
enum ErcWktFlavor {	wktFlvrNone = 0,
					wktFlvrOgc,
					wktFlvrGeoTiff,
					wktFlvrEsri,
					wktFlvrOracle,
					wktFlvrGeoTools,
					wktFlvrEpsg,
					wktFlvrOracle9,
					wktFlvrAutodesk,
					wktFlvrUnknown,
					wktFlvrAppAlt,
					wktFlvrLclAlt,
					wktFlvrLegacy
				  };

/*
	The following enumerates the various parameters supported by WKT, and the
	various names used to identify these.  Nothing like a "standard" :>)

	All ESPG parameters are listed even though many are not really used by
	anything in the real world.  In these cases, the parameter "types" are
	mapped, via the cs_PRMCOD_?????? values to the usual use in reality.

	Since the original form of WKT used the PARAMETER element to convey the
	three/seven datum shift parameters, we include them in the parameter table.

	We considered making the enumeration tag values map to EPSG codes, but
	we decided to remain as independent of EPSG as possible.  They are not
	software developers and often make changes that us developers would frown
	upon.
*/
enum EcsWktParameter {	csWktPrmNone = 0,
						/* Some datum transformation parameters */
						csWktPrmXTranslation = 101,
						csWktPrmYTranslation,
						csWktPrmZTranslation,
						csWktPrmXRotation,
						csWktPrmYRotation,
						csWktPrmZRotation,
						csWktPrmDatumScale,
						/* Projection parameters */
						csWktPrmLatitudeOfOrg = 201,
						csWktPrmLongitudeOfOrg,
						csWktPrmScaleAtOrigin,
						csWktPrmFalseEasting,
						csWktPrmFalseNorthing,
						csWktPrmLatitudeOfCtr,
						csWktPrmLongitudeOfCtr,
						csWktPrmAzimuth,
						csWktPrmRectifiedGrdAng,
						csWktPrmScaleAtCenter,
						csWktPrmEastingOfCtr,
						csWktPrmNorthingOfCtr,
						csWktPrmPsdoParallelLat,
						csWktPrmPsdoParallelScl,
						csWktPrmLatOfFalseOrg,
						csWktPrmLngOfFalseOrg,
						csWktPrmStdParallel1,
						csWktPrmStdParallel2,
						csWktPrmEastingFalseOrg,
						csWktPrmNorthingFalseOrg,
						csWktPrmSphericalOrgLat,
						csWktPrmSphericalOrgLng,
						csWktPrmSystemWestLimit,
						csWktPrmSystemZoneWidth,
						csWktPrmLatOfStdParall,
						csWktPrmZone,
						csWktPrmRegion,
						/* The following are not used by anyone, except EPSG,
						   and they are by-in-large superfluous. */
						csWktPrmSemiMajor,
						csWktPrmSemiMinor,
						csWktPrmXScale,
						csWktPrmYScale,
						csWktPrmXYRotation,
						csWktPrmPseudoStdParall1,
						csWktPrmPseudoStdParall2,
						csWktPrmOption,
						csWktPrmHeight,
						csWktPrmCentralParallel,
						/* The following WKT,  but not EPSG parameters */
						csWktPrmLatFirstPoint,
						csWktPrmLngFirstPoint,
						csWktPrmLatSecondPoint,
						csWktPrmLngSecondPoint,
						/* The following neither WKT or EPSG, unique to CS-MAP */
						csWktPrmYaxisAzimuth,
						/* Cartesian transformation parameters */
						csWktPrmAffineA0 = 501,
						csWktPrmAffineB0,
						csWktPrmAffineA1,
						csWktPrmAffineB1,
						csWktPrmAffineA2,
						csWktPrmAffineB2,
						/* The following is used to terminate tables, etc. */
						csWktPrmUnknown
					 };

/*
	The following define the currently supported physical types
	for projection parameters.
*/

#define cs_PRMPTYP_NONE      0		/* No secification. */
#define cs_PRMPTYP_DBL       1		/* Double */

/*
	The following define the currently supported logical types
	for projection parameters.
*/

#define cs_PRMLTYP_NONE      0		/* No specification */
#define cs_PRMLTYP_LNG       1		/* Longitude */
#define cs_PRMLTYP_LAT       2		/* Latitude */
#define cs_PRMLTYP_AZM       3		/* Azimuth */
#define cs_PRMLTYP_ANGD      4		/* Angular Distance */
#define cs_PRMLTYP_CMPLXC    5		/* Complex Coefficient */
#define cs_PRMLTYP_ZNBR      6		/* UTM Zone Number */
#define cs_PRMLTYP_HSNS      7		/* Hemisphere selection;
									   North >= 0.0
									   South <= 0.0 */
#define cs_PRMLTYP_GHGT      8		/* Geoid Height in meters */
#define cs_PRMLTYP_ELEV	     9		/* Elevation, system units. */
#define cs_PRMLTYP_AFCOEF   10		/* Affine Transformation Coefficient. */
#define cs_PRMLTYP_XYCRD    11		/* X/Y Coordinate */
#define cs_PRMLTYP_SCALE    12		/* Scale value */

/* The following defines are used as a bit map to in a 32 bit unsigned long
   to keep track of and check the existence of all required parameters for
   a specific projection.  This is how the compiler now locates a
   missing parameter.  (Previously, if zero was a valid value for a missing
   parameter, the system would not be able to report the fact that the
   parameter was missing from the definition.

   Below the bit definitions is a constant definition for each projection
   variation.  This value indicates the required parameters for each
   projection and/or variation of same.  This value should go into the
   projection table.  Since CS-MAP is on life support, we may find a
   less disruptive way of tdoing this.

   This issue really only applies to doubles where a zero is also a
   valid value.  We include Units in this just for consistency.
*/

#define cs_PRMBMP_UNIT     (1L <<   0)	/* System Units */
#define cs_PRMBMP_PRJPRM1  (1L <<   1)	/* Projection Parameter  1 */
#define cs_PRMBMP_PRJPRM2  (1L <<   2)	/* Projection Parameter  2 */
#define cs_PRMBMP_PRJPRM3  (1L <<   3)	/* Projection Parameter  3 */
#define cs_PRMBMP_PRJPRM4  (1L <<   4)	/* Projection Parameter  4 */
#define cs_PRMBMP_PRJPRM5  (1L <<   5)	/* Projection Parameter  5 */
#define cs_PRMBMP_PRJPRM6  (1L <<   6)	/* Projection Parameter  6 */
#define cs_PRMBMP_PRJPRM7  (1L <<   7)	/* Projection Parameter  7*/
#define cs_PRMBMP_PRJPRM8  (1L <<   8)	/* Projection Parameter  8 */
#define cs_PRMBMP_PRJPRM9  (1L <<   9)	/* Projection Parameter  9 */
#define cs_PRMBMP_PRJPRM10 (1L <<  10)	/* Projection Parameter 10 */
#define cs_PRMBMP_PRJPRM11 (1L <<  11)	/* Projection Parameter 11 */
#define cs_PRMBMP_PRJPRM12 (1L <<  12)	/* Projection Parameter 12 */
#define cs_PRMBMP_PRJPRM13 (1L <<  13)	/* Projection Parameter 13 */
#define cs_PRMBMP_PRJPRM14 (1L <<  14)	/* Projection Parameter 14 */
#define cs_PRMBMP_PRJPRM15 (1L <<  15)	/* Projection Parameter 15 */
#define cs_PRMBMP_PRJPRM16 (1L <<  16)	/* Projection Parameter 16 */
#define cs_PRMBMP_PRJPRM17 (1L <<  17)	/* Projection Parameter 17 */
#define cs_PRMBMP_PRJPRM18 (1L <<  18)	/* Projection Parameter 18 */
#define cs_PRMBMP_PRJPRM19 (1L <<  19)	/* Projection Parameter 19 */
#define cs_PRMBMP_PRJPRM20 (1L <<  20)	/* Projection Parameter 20 */
#define cs_PRMBMP_PRJPRM21 (1L <<  21)	/* Projection Parameter 21 */
#define cs_PRMBMP_PRJPRM22 (1L <<  22)	/* Projection Parameter 22 */
#define cs_PRMBMP_PRJPRM23 (1L <<  23)	/* Projection Parameter 23 */
#define cs_PRMBMP_PRJPRM24 (1L <<  24)	/* Projection Parameter 24 */
#define cs_PRMBMP_ORGLNG   (1L <<  25)	/* Origin Longitude */
#define cs_PRMBMP_ORGLAT   (1L <<  26)	/* Origin Latitude */
#define cs_PRMBMP_SCLRED   (1L <<  27)	/* Origin Scale Reduction */
#define cs_PRMBMP_XXXOFF   (1L <<  28)	/* Origin False Easting */
#define cs_PRMBMP_YYYOFF   (1L <<  29)	/* Origin False Northing */
#define cs_PRMBMP_ELPSCL   (1L <<  30)	/* Ellipsoid Scale */

/* Each of the following is a bit map of the REQUIRED parameters for each
   projection or variation thereof. */

#define cs_PJPRMBMP_UNITY		(cs_PRMBMP_UNIT)UL
#define cs_PJPRMBMP_TRMER		(cs_PRMBMP_UNIT   | cs_PRMBMP_PRJPRM1  | cs_PRMBMP_ORGLAT   | cs_PRMBMP_SCLRED   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_ALBER		(cs_PRMBMP_UNIT   | cs_PRMBMP_PRJPRM1  | cs_PRMBMP_PRJPRM2  | cs_PRMBMP_ORGLNG   | cs_PRMBMP_ORGLAT   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_MRCAT		(cs_PRMBMP_UNIT   | cs_PRMBMP_PRJPRM1  | cs_PRMBMP_ORGLNG   | cs_PRMBMP_ORGLAT   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_AZMED		(cs_PRMBMP_UNIT   | cs_PRMBMP_PRJPRM1  | cs_PRMBMP_ORGLNG   | cs_PRMBMP_ORGLAT   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_LMTAN		(cs_PRMBMP_UNIT   | cs_PRMBMP_ORGLNG   | cs_PRMBMP_ORGLAT   | cs_PRMBMP_SCLRED   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_PLYCN		(cs_PRMBMP_UNIT   | cs_PRMBMP_ORGLNG   | cs_PRMBMP_ORGLAT   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_MODPC		(cs_PRMBMP_UNIT   | cs_PRMBMP_PRJPRM1  | cs_PRMBMP_PRJPRM2  | cs_PRMBMP_ORGLNG   | cs_PRMBMP_ORGLAT   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_AZMEA		(cs_PRMBMP_UNIT   | cs_PRMBMP_PRJPRM1  | cs_PRMBMP_ORGLNG   | cs_PRMBMP_ORGLAT   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_EDCNC		(cs_PRMBMP_UNIT   | cs_PRMBMP_PRJPRM1  | cs_PRMBMP_PRJPRM2  | cs_PRMBMP_ORGLNG   | cs_PRMBMP_ORGLAT   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_MILLR		(cs_PRMBMP_UNIT   | cs_PRMBMP_PRJPRM1  | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_MSTRO		(cs_PRMBMP_UNIT   | cs_PRMBMP_ORGLNG   | cs_PRMBMP_ORGLAT   | cs_PRMBMP_SCLRED   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_NZLND		(cs_PRMBMP_UNIT   | cs_PRMBMP_ORGLNG   | cs_PRMBMP_ORGLAT   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_SINUS		(cs_PRMBMP_UNIT   | cs_PRMBMP_ORGLNG   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_ORTHO		(cs_PRMBMP_UNIT   | cs_PRMBMP_ORGLNG   | cs_PRMBMP_ORGLAT   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_GNOMC		(cs_PRMBMP_UNIT   | cs_PRMBMP_ORGLNG   | cs_PRMBMP_ORGLAT   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_EDCYL		(cs_PRMBMP_UNIT   | cs_PRMBMP_PRJPRM1  | cs_PRMBMP_ORGLNG   | cs_PRMBMP_ORGLAT   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_VDGRN		(cs_PRMBMP_UNIT   | cs_PRMBMP_ORGLNG   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_CSINI		(cs_PRMBMP_UNIT   | cs_PRMBMP_PRJPRM1  | cs_PRMBMP_ORGLAT   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_ROBIN		(cs_PRMBMP_UNIT   | cs_PRMBMP_ORGLNG   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_BONNE		(cs_PRMBMP_UNIT   | cs_PRMBMP_ORGLNG   | cs_PRMBMP_ORGLAT   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_EKRT4		(cs_PRMBMP_UNIT   | cs_PRMBMP_ORGLNG   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_EKRT6		(cs_PRMBMP_UNIT   | cs_PRMBMP_ORGLNG   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_MOLWD		(cs_PRMBMP_UNIT   | cs_PRMBMP_ORGLNG   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_HMLSN		(cs_PRMBMP_UNIT   | cs_PRMBMP_ORGLNG   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_NACYL		(cs_PRMBMP_UNIT   | cs_PRMBMP_PRJPRM1  | cs_PRMBMP_ORGLNG   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_TACYL		(cs_PRMBMP_UNIT   | cs_PRMBMP_ORGLNG   | cs_PRMBMP_ORGLAT   | cs_PRMBMP_SCLRED   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_BPCNC		(cs_PRMBMP_UNIT   | cs_PRMBMP_PRJPRM1  | cs_PRMBMP_PRJPRM2  | cs_PRMBMP_PRJPRM3  | cs_PRMBMP_PRJPRM4  | cs_PRMBMP_PRJPRM5  | cs_PRMBMP_PRJPRM6  | cs_PRMBMP_PRJPRM7  | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_SWISS		(cs_PRMBMP_UNIT   | cs_PRMBMP_ORGLNG   | cs_PRMBMP_ORGLAT   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_PSTRO		(cs_PRMBMP_UNIT   | cs_PRMBMP_ORGLNG   | cs_PRMBMP_ORGLAT   | cs_PRMBMP_SCLRED   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_OSTRO		(cs_PRMBMP_UNIT   | cs_PRMBMP_ORGLNG   | cs_PRMBMP_ORGLAT   | cs_PRMBMP_SCLRED   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_SSTRO		(cs_PRMBMP_UNIT   | cs_PRMBMP_ORGLNG   | cs_PRMBMP_ORGLAT   | cs_PRMBMP_SCLRED   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_LM1SP		(cs_PRMBMP_UNIT   | cs_PRMBMP_ORGLNG   | cs_PRMBMP_ORGLAT   | cs_PRMBMP_SCLRED   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_LM2SP		(cs_PRMBMP_UNIT   | cs_PRMBMP_PRJPRM1  | cs_PRMBMP_PRJPRM2  | cs_PRMBMP_ORGLNG   | cs_PRMBMP_ORGLAT   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_LMBLG		(cs_PRMBMP_UNIT   | cs_PRMBMP_PRJPRM1  | cs_PRMBMP_PRJPRM2  | cs_PRMBMP_ORGLNG   | cs_PRMBMP_ORGLAT   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_WCCSL		(cs_PRMBMP_UNIT   | cs_PRMBMP_PRJPRM1  | cs_PRMBMP_PRJPRM2  | cs_PRMBMP_PRJPRM3  | cs_PRMBMP_PRJPRM4  | cs_PRMBMP_ORGLNG   | cs_PRMBMP_ORGLAT   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_WCCST		(cs_PRMBMP_UNIT   | cs_PRMBMP_PRJPRM1  | cs_PRMBMP_PRJPRM2  | cs_PRMBMP_PRJPRM3  | cs_PRMBMP_ORGLAT   | cs_PRMBMP_SCLRED   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_MNDOTL		(cs_PRMBMP_UNIT   | cs_PRMBMP_PRJPRM1  | cs_PRMBMP_PRJPRM2  | cs_PRMBMP_PRJPRM3  | cs_PRMBMP_ORGLNG   | cs_PRMBMP_ORGLAT   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_MNDOTT		(cs_PRMBMP_UNIT   | cs_PRMBMP_PRJPRM1  | cs_PRMBMP_PRJPRM2  | cs_PRMBMP_ORGLAT   | cs_PRMBMP_SCLRED   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_SOTRM		(cs_PRMBMP_UNIT   | cs_PRMBMP_PRJPRM1  | cs_PRMBMP_ORGLAT   | cs_PRMBMP_SCLRED   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_UTM			(cs_PRMBMP_UNIT   | cs_PRMBMP_PRJPRM1  | cs_PRMBMP_PRJPRM2  )UL
#define cs_PJPRMBMP_TRMRS		(cs_PRMBMP_UNIT   | cs_PRMBMP_PRJPRM1  | cs_PRMBMP_ORGLAT   | cs_PRMBMP_SCLRED   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_GAUSSK		(cs_PRMBMP_UNIT   | cs_PRMBMP_PRJPRM1  | cs_PRMBMP_ORGLAT   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_KROVAK		(cs_PRMBMP_UNIT   | cs_PRMBMP_PRJPRM1  | cs_PRMBMP_PRJPRM2  | cs_PRMBMP_PRJPRM3  | cs_PRMBMP_ORGLNG   | cs_PRMBMP_ORGLAT   | cs_PRMBMP_SCLRED   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_KROVK1		(cs_PRMBMP_UNIT   | cs_PRMBMP_PRJPRM1  | cs_PRMBMP_PRJPRM2  | cs_PRMBMP_PRJPRM3  | cs_PRMBMP_ORGLNG   | cs_PRMBMP_ORGLAT   | cs_PRMBMP_SCLRED   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_MRCATK		(cs_PRMBMP_UNIT   | cs_PRMBMP_PRJPRM1  | cs_PRMBMP_SCLRED   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_KRVK95		(cs_PRMBMP_UNIT   | cs_PRMBMP_PRJPRM1  | cs_PRMBMP_PRJPRM2  | cs_PRMBMP_PRJPRM3  | cs_PRMBMP_ORGLNG   | cs_PRMBMP_ORGLAT   | cs_PRMBMP_SCLRED   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_KRVK951		(cs_PRMBMP_UNIT   | cs_PRMBMP_PRJPRM1  | cs_PRMBMP_PRJPRM2  | cs_PRMBMP_PRJPRM3  | cs_PRMBMP_ORGLNG   | cs_PRMBMP_ORGLAT   | cs_PRMBMP_SCLRED   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_PSTROSL		(cs_PRMBMP_UNIT   | cs_PRMBMP_PRJPRM1  | cs_PRMBMP_ORGLNG   | cs_PRMBMP_ORGLAT   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_TRMERAF		(cs_PRMBMP_UNIT   | cs_PRMBMP_PRJPRM1  | cs_PRMBMP_PRJPRM2  | cs_PRMBMP_PRJPRM3  | cs_PRMBMP_PRJPRM4  | cs_PRMBMP_PRJPRM5  | cs_PRMBMP_PRJPRM6  | cs_PRMBMP_PRJPRM7  | cs_PRMBMP_ORGLAT   | cs_PRMBMP_SCLRED   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_NERTH		(cs_PRMBMP_UNIT   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_OBQCYL		(cs_PRMBMP_UNIT   | cs_PRMBMP_PRJPRM1  | cs_PRMBMP_ORGLNG   | cs_PRMBMP_ORGLAT   | cs_PRMBMP_SCLRED   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_SYS34		(cs_PRMBMP_UNIT   | cs_PRMBMP_PRJPRM1  )UL
#define cs_PJPRMBMP_SYS34_99	(cs_PRMBMP_UNIT   | cs_PRMBMP_PRJPRM1  )UL
#define cs_PJPRMBMP_OSTN97		(cs_PRMBMP_UNIT   )UL
#define cs_PJPRMBMP_AZEDE		(cs_PRMBMP_UNIT   | cs_PRMBMP_PRJPRM1  | cs_PRMBMP_PRJPRM2  | cs_PRMBMP_ORGLNG   | cs_PRMBMP_ORGLAT   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_OSTN02		(cs_PRMBMP_UNIT   )UL
#define cs_PJPRMBMP_HOM1UV		(cs_PRMBMP_UNIT   | cs_PRMBMP_PRJPRM1  | cs_PRMBMP_PRJPRM2  | cs_PRMBMP_PRJPRM3  | cs_PRMBMP_SCLRED   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_HOM1XY		(cs_PRMBMP_UNIT   | cs_PRMBMP_PRJPRM1  | cs_PRMBMP_PRJPRM2  | cs_PRMBMP_PRJPRM3  | cs_PRMBMP_SCLRED   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_HOM2UV		(cs_PRMBMP_UNIT   | cs_PRMBMP_PRJPRM1  | cs_PRMBMP_PRJPRM2  | cs_PRMBMP_PRJPRM3  | cs_PRMBMP_PRJPRM4  | cs_PRMBMP_SCLRED   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_HOM2XY		(cs_PRMBMP_UNIT   | cs_PRMBMP_PRJPRM1  | cs_PRMBMP_PRJPRM2  | cs_PRMBMP_PRJPRM3  | cs_PRMBMP_PRJPRM4  | cs_PRMBMP_SCLRED   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_RSKEW		(cs_PRMBMP_UNIT   | cs_PRMBMP_PRJPRM1  | cs_PRMBMP_PRJPRM2  | cs_PRMBMP_PRJPRM3  | cs_PRMBMP_SCLRED   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_RSKEWC		(cs_PRMBMP_UNIT   | cs_PRMBMP_PRJPRM1  | cs_PRMBMP_PRJPRM2  | cs_PRMBMP_PRJPRM3  | cs_PRMBMP_SCLRED   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_RSKEWO		(cs_PRMBMP_UNIT   | cs_PRMBMP_PRJPRM1  | cs_PRMBMP_PRJPRM2  | cs_PRMBMP_PRJPRM3  | cs_PRMBMP_SCLRED   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_WINKL		(cs_PRMBMP_UNIT   | cs_PRMBMP_ORGLNG   | cs_PRMBMP_PRJPRM1  | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_NRTHSRT		(cs_PRMBMP_UNIT   | cs_PRMBMP_PRJPRM1  | cs_PRMBMP_PRJPRM2  | cs_PRMBMP_PRJPRM3  | cs_PRMBMP_PRJPRM4  | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF )UL
#define cs_PJPRMBMP_LMMICH		(cs_PRMBMP_UNIT   | cs_PRMBMP_PRJPRM1  | cs_PRMBMP_PRJPRM2  | cs_PRMBMP_ORGLNG   | cs_PRMBMP_ORGLAT   | cs_PRMBMP_XXXOFF   | cs_PRMBMP_YYYOFF | cs_PRMBMP_ELPSCL)UL

/*
	Maximum length of a Coordinate System, Datum, and or
	Ellipsoid name.  The structures are defined such that
	a maximum of 23 can be supported.  Usually a number
	less than this is used to conserve dialog box real estate.

	Note: Multiple Regression data file names are the same
	as datum names.  Thus, datum names longer than 8
	characters may be a problem in some environments.
*/

#define cs_KEYNM_DEF 24
#define cs_KEYNM_MAX (cs_KEYNM_DEF - 1)
#define cs_KEYNM_DEF1 64
#define cs_KEYNM_MAX1 (cs_KEYNM_DEF1 - 1)

/*
	The following define the default file names for the coordinate
	system, datum, and ellipsoid dictionary files; and also for the
	several datum conversion catalog files.  These are referenced in
	the CSdata module.  File names can now be changed at run time using,
	for example, CS_csfnm.  The directory portion is defined in CSdata.c
	and can be modified at run time by CS_altdr.

	Note, beginning with release 6.04, all file name comparisons
	performed by CS-MAP are case insensitive.  This refers to
	comparisons only. The names given below are the names which are
	used to initialize the cs_Csname (et. al.) arrays and what will
	actually be used to open the file.  Thus, depending upon the
	environment, the case of the name may matter, it may not. What
	operating systems do with case in file names gets more interesting
	every day. The names as given are the recommended names. Note,
	the misspelling of Elipsoid is intentional, a hold over from MS-DOS
	days.
*/

#define cs_CS_NAME           "Coordsys.CSD"
#define cs_DAT_NAME          "Datums.CSD"
#define cs_ELL_NAME          "Elipsoid.CSD"
#define cs_CT_NAME           "Category.CSD"
#define cs_GX_NAME           "GeodeticTransform.CSD"
#define cs_GP_NAME           "GeodeticPath.CSD"
#define cs_NMP_NAME          "NameMapper.csv"
#define cs_GEOID_NAME        "GeoidHeight.gdc"
#define cs_VERTCON_NAME      "Vertcon.gdc"

#define cs_OSTN97_NAME       "OSTN97.TXT"
#define cs_OSTN97_NAME_BIN   "OSTN97._nt"
#define cs_OSTN02_NAME       "OSTN02.txt"
#define cs_OSTN02_NAME_BIN   "OSTN02._02"

/*
	The following defines the environmental variable which CS_altdr
	now uses to locate the mapping data directory when instructed
	to obtain such from the environment.
*/

#define cs_ENV_VAR "CS_MAP_DIR"

/*
	The following defines the key code name of the Unity
	projection.  This named is now referenced in the code
	and this projection name has special meaning.

	In release 7.01, specific code numbers where assigned to
	each projection, and CS_MAP modified to use the code
	numbers instead of the name.  This define is, therefore,
	obsolete, is retained for compatibility with old code,
	and should not be used in new code.  It will eventually
	disappear.
*/

#define cs_PRJNM_UNITY "LL"

/*
	The following three structures are that which were used
	for release 5.0 of CS_MAP. They are retained for used by
	the CS_??rup functions to upgrade data from previous
	releases to the current release. NOTE: that we establish
	the compiler packing used on these structures to what we
	believe will produce what was used on the previous releases
	for the various products. The documentation on this issue
	is rather sketchy in all cases. This may need to be modified
	for your application.
*/

#if   _RUN_TIME == _rt_BORLAND16 || _RUN_TIME == _rt_BORLAND32
#	pragma option -a1
#elif _RUN_TIME == _rt_METAWARE
#	pragma Align_members (1)
#elif _RUN_TIME == _rt_MSC16 || _RUN_TIME == _rt_MSVC16 || _RUN_TIME == _rt_MSDOTNET || \
														   _RUN_TIME == _rt_MSWIN64 ||  \
														   _RUN_TIME == _rt_MSVC32
#	pragma pack (1)
#elif _RUN_TIME == _rt_HPUX
#	pragma pack 1
#elif _RUN_TIME == _rt_AIX
#    pragma options align=packed
#elif _RUN_TIME >= _rt_UNIXPCC
#    pragma pack (1)
#else
	/* I don't know what would work for WATCOM. */
#endif

struct cs_Eldef05_
{
	char key_nm [12];       /* Key name, used to identify a specific
							   entry in the table. */
	char fill [4];          /* Fill out the structure to a memory
							   boundary which is a multiple of
							   eight.  This is done to ensure
							   the portability of Datum dictionaries. */
	double e_rad;           /* Equatorial radius IN METERS. */
	double p_rad;           /* Polar radius IN METERS. */
	double flat;            /* Flattening ratio, e.g. 1.0/297.0 */
	double ecent;           /* Ecentricity. */
	short protect;          /* TRUE indicates that this is a
							   distribution definition.  Typically
							   used to require a confirmation before
							   modification. */
	char name [64];         /* Full descriptive name of the
							   datum. */
};
#define cs_BSWP_ELDEF05 "12c4cdddds64c"


struct cs_Dtdef05_
{
	char key_nm [10];
	char ell_knm [10];
	char fill [4];
	double delta_X;
	double delta_Y;
	double delta_Z;
	short protect;
	char name [64];
};
#define cs_BSWP_DTDEF05 "10c10c4cddds64c"

struct cs_Csdef05_
{
	char key_nm [10];
	char dat_knm [10];
	char prj_knm [10];
	char unit [16];
	char fill [2];
	double prj_prm1;
	double prj_prm2;
	double prj_prm3;
	double prj_prm4;
	double prj_prm5;
	double prj_prm6;
	double org_lng;
	double org_lat;
	double x_off;
	double y_off;
	double scl_red;
	double unit_scl;
	double map_scl;
	double scale;
	double zero [2];
	short protect;
	char desc_nm [64];
};
#define cs_BSWP_CSDEF05 "10c10c10c16c2cdddddddddddddd2ds64c"

/*
	Turn off whatever packing was used for release 5 stuff, then
	turn it back on at whatever is appropriate for release 6.
*/

#if   _RUN_TIME == _rt_BORLAND16 || _RUN_TIME == _rt_BORLAND32
#	pragma option -a.
#elif _RUN_TIME == _rt_METAWARE
#	pragma Align_members ()
#elif _RUN_TIME == _rt_MSC16 || _RUN_TIME == _rt_MSVC16 || _RUN_TIME == _rt_MSDOTNET || \
														   _RUN_TIME == _rt_MSWIN64 ||  \
														   _RUN_TIME == _rt_MSVC32
#	pragma pack ()
#elif _RUN_TIME == _rt_HPUX
#	pragma pack
#elif _RUN_TIME == _rt_AIX
#   pragma options align=reset
#elif _RUN_TIME >= _rt_UNIXPCC
#   pragma pack ()
#else
		/* I don't know what would work for WATCOM. */
#endif


#if   _RUN_TIME == _rt_BORLAND16 || _RUN_TIME == _rt_BORLAND32
#	pragma option -a1
#elif _RUN_TIME == _rt_METAWARE
#	pragma Align_members (1)
#elif _RUN_TIME == _rt_MSC16 || _RUN_TIME == _rt_MSVC16 || _RUN_TIME == _rt_MSDOTNET || \
														   _RUN_TIME == _rt_MSWIN64 ||  \
														   _RUN_TIME == _rt_MSVC32
#	pragma pack (1)
#elif _RUN_TIME == _rt_HPUX
#	pragma pack 1
#elif _RUN_TIME == _rt_AIX
#    pragma options align=packed
#elif _RUN_TIME >= _rt_UNIXPCC
#    pragma pack (1)
#else
		/* I don't know what would work for WATCOM. */
#endif

/*
	The following two structures are that which were used
	for release 6.0 of CS_MAP (the cs_Eldef_ structure did not
	change). They are retained for used by the CS_??rup functions
	to upgrade data from previous releases to the current release.
	NOTE: that we establish the compiler packing used on these
	structures to 1, which is what was used by CS-MAP in those
	releases. This may need to be modified for your application.
*/

struct cs_Dtdef06_
{
	char key_nm [24];
	char ell_knm [24];
	char locatn [24];
	char cntry_st [48];
	char fill [8];	
	double delta_X;
	double delta_Y;
	double delta_Z;
	double rot_X;
	double rot_Y;
	double rot_Z;
	double bwscale;
	char name [64];
	char source [64];
	short protect;
	short fill01;	
	short fill02;
	short fill03;
	short fill04;
	short fill05;
	short fill06;
	short fill07;
};
#define cs_BSWP_DTDEF06 "24c24c24c48c8cddddddd64c64cs7s"

struct cs_Csdef06_
{
	char key_nm [24];
	char dat_knm [24];
	char elp_knm [24];
	char prj_knm [24];
	char  group [24];
	char locatn [24];
	char cntry_st [48];
	char unit [16];
	char fill [8];
	double prj_prm1;
	double prj_prm2;
	double prj_prm3;
	double prj_prm4;
	double prj_prm5;
	double prj_prm6;
	double prj_prm7;
	double prj_prm8;
	double prj_prm9;
	double prj_prm10;
	double prj_prm11;
	double prj_prm12;
	double prj_prm13;
	double prj_prm14;
	double prj_prm15;
	double prj_prm16;
	double prj_prm17;
	double prj_prm18;
	double prj_prm19;
	double prj_prm20;
	double prj_prm21;
	double prj_prm22;
	double prj_prm23;
	double prj_prm24;
	double org_lng;
	double org_lat;
	double x_off; 
	double y_off;
	double scl_red;
	double unit_scl;
	double map_scl;
	double scale;
	double zero [2];
	double hgt_xx;
	double hgt_yy;
	double hgt_zz;
	double ll_min [2];
	double ll_max [2];
	double xy_min [2];
	double xy_max [2];
	char desc_nm [64];
	char source [64];
	short quad;
	short order;
	short zones;
	short protect;
	short fill01;
	short fill02;
	short fill03;
	short fill04;
};
#define cs_BSWP_CSDEF06 "24c24c24c24c24c24c48c16c8c24ddddddddd2dddd2d2d2d2d64c64cssss4s"

struct cs_GrfMreg06_
{
	cs_magic_t magic;
	long32_t mr_size;
	unsigned char max_uu;
	unsigned char max_vv;
	unsigned char lat_idx;
	unsigned char hgt_idx;
	ulong32_t lng_map [4];
	ulong32_t lat_map [4];
	ulong32_t hgt_map [4];
	double kk;
	double uu_off;
	double vv_off;
	double coefs [150];
};
#define cs_BSWP_GRFMREG06 "ll4c4l4l4lddd150d"

/* The following structure definitions defin the form of the Coordinate System
   Category feature.  The Category Feature is another means by which coordinate
   systems can be organized; primarily for the purposes of coordinate system
   browers.  Categories differ from groups in that a coordinate system can
   belong to more than one category; while a coordinate system can only belong
   to a single group. */

#define cs_CATDEF_UNUSED "<unused>"
#define cs_CATDEF_CATNMSZ 128
#define cs_CATDEF_ALLOC 12

/* The following structure represents a coordinate system name as it appears
   within a category entry. */
struct cs_CtItmName_
{
	char csName [cs_KEYNM_DEF];
};


/* The following is the form of a category.  It has a name, and a list of
   coordinate system names. */
struct cs_Ctdef_
{
	struct cs_Ctdef_ *next;
	struct cs_Ctdef_ *previous;
	int protect;
	char ctName [cs_CATDEF_CATNMSZ];
	ulong32_t nameCnt;
	ulong32_t allocCnt;
	struct cs_CtItmName_ *csNames;
	ulong32_t userNameIndex; //at which index in [ctName] start the user added CS names; 
};


/*
	Turn off whatever packing was used for release 6 stuff, then
	turn it back on at whatever is appropriate for release 7.
*/

#if   _RUN_TIME == _rt_BORLAND16 || _RUN_TIME == _rt_BORLAND32
#	pragma option -a.
#elif _RUN_TIME == _rt_METAWARE
#	pragma Align_members ()
#elif _RUN_TIME == _rt_MSC16 || _RUN_TIME == _rt_MSVC16 || _RUN_TIME == _rt_MSDOTNET || \
														   _RUN_TIME == _rt_MSWIN64 ||  \
														   _RUN_TIME == _rt_MSVC32
#	pragma pack ()
#elif _RUN_TIME == _rt_HPUX
#	pragma pack
#elif _RUN_TIME == _rt_AIX
#	pragma options align=reset
#elif _RUN_TIME >= _rt_UNIXPCC
#	pragma pack ()
#else
	/* I don't know what would work for WATCOM. */
#endif


#if   _RUN_TIME == _rt_BORLAND16 || _RUN_TIME == _rt_BORLAND32
#	pragma option -a1
#elif _RUN_TIME == _rt_METAWARE
#	pragma Align_members (1)
#elif _RUN_TIME == _rt_MSC16 || _RUN_TIME == _rt_MSVC16 || _RUN_TIME == _rt_MSDOTNET || \
														   _RUN_TIME == _rt_MSWIN64 ||  \
														   _RUN_TIME == _rt_MSVC32
#	pragma pack (1)
#elif _RUN_TIME == _rt_HPUX
#	pragma pack 1
#elif _RUN_TIME == _rt_AIX
#	pragma options align=packed
#elif _RUN_TIME >= _rt_UNIXPCC
#	pragma pack (1)
#else
	/* I don't know what would work for WATCOM. */
#endif

/*
	The following structure defines the form of the Ellipsoid
	Dictionary.  This file has a 4 byte magic number on the front
	which is followed by any number of the following structures.
	The list is maintained in order by key name and is accessed via
	binary search techniques.  CS_dtloc puts together a datum
	definition (cs_Dtdef_) and an ellipsoid definition (cs_Eldef_)
	and returns a complete datum definition structure (cs_Datum_).

	Therefore, take care not to get these three different
	structures confused.  In general, programs which manipulate
	the contents of the dicionaries involved will used the
	cs_Eldef_ and cs_Dtdef_ structures while programs which
	actually perform calculations will use CS_dtloc to obtain
	a pointer to a malloc'ed cs_Datum_ structure and use its
	contents to perform all calculations.

	In the following structure, the units of the radii are (now)
	required to be in meters.
*/

struct cs_Eldef_
{
	char key_nm [24];       /* Key name, used to identify a specific
							   entry in the table. */
	char group [6];         /* Used to group ellipsoids for selection
							   purposes. */
	char fill [2];			/* Filler, also used for encryption. */
	double e_rad;           /* Equatorial radius IN METERS. */
	double p_rad;           /* Polar radius IN METERS. */
	double flat;            /* Flattening ratio, e.g. 1.0/297.0 */
	double ecent;           /* Ecentricity. */
	char name [64];         /* Full descriptive name of the
							   datum. */
	char source [64];		/* Description of where the data for this
							   coordinate system came from. */
	short protect;          /* TRUE indicates that this is a
							   distribution definition.  Typically
							   used to require a confirmation before
							   modification. */
	short epsgNbr;			/* EPSG Code number for this definition,
							   if known.*/
	short wktFlvr;			/* WKT flavor, if dervied from WKT, else zero */
	short fill01;			/* Fill out to a multple of 16 to avoid
							   affects of byte aligment options in
							   many compilers. */
	short fill02;			/* Ditto. */
	short fill03;			/* Ditto. */
	short fill04;			/* Ditto. */
	short fill05;			/* Ditto. */
};
#define cs_BSWP_ELDEF "24c8cdddd64c64cs7s"

/*
	The following structure defines the form of records in the
	Datum Dictionary.  The datum dictionary consists of a 4 byte
	magic number followed by any number of these structures.
	Records are maintained in sorted order by key name and are
	accessed by binary search technique.  Data in this file may
	be encrypted.

	DO NOT CONFUSE this structure with the cs_Datum_ structure
	defined below.  This structure is used to carry raw data
	concerning the datum.  The cs_Datum_ structure is that returned
	by CS_dtloc which combines the information in this structure with
	that from the referenced ellipsoid to return a complete datum
	definition.
*/


/*
	This is the new structure of the Datum Dictionary.
	Four new elements, which support the Bursa Wolfe/
	Seven Parameter datum transformations, have been added.
*/

struct cs_Dtdef_
{
	char key_nm [24];	/* Key name, used to locate a specific
						   entry in the dictionary. */
	char ell_knm [24];	/* Ellipsoid key name. Used to access the
						   Ellipsoid Dictionary to obtain the
						   ellipsoid upon which the datum is
						   based. */
	char  group [24];	/* The classification into which this
						   datum falls. */
	char locatn [24];	/* Field by which coordinate systems can
						   be classified by location, for example,
						   world, North America, Central America,
						   Europe, etc.  To be used by selector
						   programs. */
	char cntry_st [48];	/* Up to 24 two character codes which define
						   the countries (or US states) in which
						   this coordinate system is designed to be
						   used.  We use the US postal code
						   abbreviations for states in lower case,
						   and POSC country abbreviations in upper
						   case. This also, is intended for use by
						   a coordinate system selector program. */
	char fill [8];		/* Fill the sturcture out to a boundary
						   of 8 bytes, for portability
						   considerations.  Note, we need some
						   fill, even if 8 bytes, to provide a
						   place for encryption to work. */

						/* The following values are usually
						   extracted from DMA Technical Report
						   8350.2-B */

	double delta_X;		/* X component of the vector from the
						   WGS-84 geocenter to the geocenter of
						   this datum. */
	double delta_Y;		/* Y component of the vector from the
						   WGS-84 geocenter to the geocenter of
						   this datum. */
	double delta_Z;		/* Z component of the vector from this
						   WGS-84 geocenter to the geocenter of
						   this datum. */

						/* If a Bursa/Wolfe definition has been
						   made, one of the following values will
						   be non-zero.  If a pure Molodensky
						   definition is provided, the values
						   of the rot_X, rot_Y, rot_Z, and
						   bwscale are all set to hard zero.
						   This is the switch which is used to
						   determine is a Bursa/Wolf conversion
						   has been defined as opposed to a
						   standard Molodensky. */

	double rot_X;		/* Angle from WGS-84 X axis to local
						   geodetic system X axis in arc
						   seconds, east is positive. */
	double rot_Y;		/* Angle from WGS-84 Y axis to local
						   geodetic system Y axis in arc
						   seconds, north is positive. */
	double rot_Z;		/* Angle from WGS-84 Z axis to local
						   geodetic system Z axis in arc
						   seconds, use right hand rule. */
	double bwscale;		/* Scale factor in parts per million.
						   Don't include the base 1.0; we
						   add that at setup time. */
	char name [64];		/* Full descriptive name of the
						   datum. */
	char source [64];	/* Description of where the data for this
						   coordinate system came from. */
	short protect;		/* TRUE indicates that this is a 
						   distribution definition.  Typically
						   requires confirmation for change. */
	short to84_via;		/* Conversion technique, one of:
						   cs_DTCTYP_MOLO
						   cs_DTCTYP_BURS
						   cs_DTCTYP_MREG
						   cs_DTCTYP_NAD27
						   cs_DTCTYP_NAD83
						   cs_DTCTYP_WGS84
						   cs_DTCTYP_WGS72
						   cs_DTCTYP_HPGN
						   cs_DTCTYP_AGD66
						   cs_DTCTYP_GEOCTR
						   cs_DTCTYP_6PARM
						   cs_DTCTYP_4PARM
						   cs_DTCTYP_AGD84
						   cs_DTCTYP_NZGD49
						   cs_DTCTYP_ATS77
						   .
						   .
						   .
						   */
	short epsgNbr;		/* EPSG number, if known */
	short wktFlvr;		/* WKT flavor, if dervied from WKT, else zero */
	short fill01;		/* Fill out to a multple of 16 to avoid
						   affects of byte aligment options in
						   many compilers. */
	short fill02;		/* Ditto. */
	short fill03;		/* Ditto. */
	short fill04;		/* Ditto. */
};
#define cs_BSWP_DTDEF "24c24c24c24c48c8cddddddd64c64css6s"

/*
	cs_Datum_ is the structure returned by CS_dtloc (et al).  It
	includes all information pertaining to the datum and the
	ellipsoid.  This is NOT the structure of the Datum dictionary.
	The information obtained from the Datum Dictionary plus
	information obtained from the Ellipsoid Dictionary, are
	combined to produce this structure.

	Since this structure is never written to disk, changes in
	this structure should not affect anyone.
*/

struct cs_Datum_
{
	char key_nm [24];	/* Key name, used to locate the definition
						   in the Datum Dictionary. */
	char ell_knm [24];	/* Ellipsoid key name. */
	double e_rad;		/* Equatorial radius in meters. */
	double p_rad;		/* Polar radius in meters. */
	double flat;		/* Flattening ratio, e.g. 1.0/297.0 */
	double ecent;		/* Ecentricity (note, not squared). */
	double delta_X;		/* Position of the geocenter relative to
						   WGS-84 in meters. */
	double delta_Y;
	double delta_Z;

						/* If a Bursa/Wolfe definition has been
						   made, one of the following values will
						   be non-zero.  If a pure Molodensky
						   definition is provided, all four
						   values will be exactly zero. */

	double rot_X;		/* Angle from WGS-84 X axis to local
						   geodetic system X axis in arc
						   seconds, east is positive. */
	double rot_Y;		/* Angle from WGS-84 Y axis to local
						   geodetic system Y axis in arc
						   seconds, north is positive. */
	double rot_Z;		/* Angle from WGS-84 Z axis to local
						   geodetic system Z axis in arc
						   seconds, use right hand rule. */
	double bwscale;		/* Scale factor in parts per million.
						   Don't include the base 1.0; we
						   add that at setup time. */
	short to84_via;		/* Code which indicates how to convert
						   from this datum to WGS84. */
	char dt_name [64];	/* Full descriptive name of the
						   datum. */
	char el_name [64];	/* Full descriptive name of the
						   ellipsoid in use. */
};

/*
	cs_Csdef_ is the record structure of the coordinate system
	dictionary.  The dictionary file consists of these records
	sorted in order by the key name of the coordinate system.
	A magic number appears on the front of the file.  This
	structure is normally obtained from CS_csdef.
*/

struct cs_Csdef_
{
	char key_nm [24];		/* The name used to identify the
							   coordinate system. */
	char dat_knm [24];		/* The key name of the datum upon which
							   the coordinate system is based. */
	char elp_knm [24];		/* The key name of the ellipsoid upon
							   which the coordinate system is based. */
	char prj_knm [24];		/* The key name of the projection upon
							   which the coordinate system is based,
							   eight characters max. */
	char  group [24];		/* The classification into which this
							   coordinate system falls.  I.e.
							   State Plane 27, State Plane 83,
							   UTM 27, etc. */
	char locatn [24];		/* Field by which coordinate systems can
							   be classified by location, for example,
							   world, North America, Central America,
							   Europe, etc.  To be used by selector
							   programs. */
	char cntry_st [48];		/* Up to 24 two character codes which define
							   the countries (or US states) in which
							   this coordinate system is designed to be
							   used.  We use the US postal code
							   abbreviations for states in lower case,
							   and POSC country abbreviations in upper
							   case. This also, is intended for use by
							   a coordinate system selector program. */
	char unit [16];			/* The name of the units of the coordinate
							   system, i.e. the units of the resulting
							   coordinate system. */
	char fill [8];			/* Fill out to a boundary of 8. */
	double prj_prm1;
	double prj_prm2;
	double prj_prm3;
	double prj_prm4;
	double prj_prm5;
	double prj_prm6;
	double prj_prm7;
	double prj_prm8;
	double prj_prm9;
	double prj_prm10;
	double prj_prm11;
	double prj_prm12;
	double prj_prm13;
	double prj_prm14;
	double prj_prm15;
	double prj_prm16;
	double prj_prm17;
	double prj_prm18;
	double prj_prm19;
	double prj_prm20;
	double prj_prm21;
	double prj_prm22;
	double prj_prm23;
	double prj_prm24;
							/* Twenty four projection parameters.
							   The actual contents depend upon the
							   projection.  For example, for
							   Transverse Mercator only prj_prm1
							   is used and it contains the Central
							   Meridian.  Values in degrees as
							   opposed to radians. */
	double org_lng;
	double org_lat;			/* The origin of the projection.  Values
							   are in degrees.  For several
							   projections, parm1 carries the origin
							   longitude (i.e. central meridian). */
	double x_off;			/* The false easting to be applied to keep X
							   coordinates positive.  Values are in the
							   units of the resulting coordinates. */
	double y_off;			/* The false northing to be applied to keep
							   the Y coordinates positive.  Values are in
							   the units of the resulting coordinates. */
	double scl_red;			/* The scale reduction which is used
							   on some projections to distribute
							   the distortion uniformily across
							   the map, else 1.0. */
	double unit_scl;		/* The scale factor required to get
							   from coordinate system units to meters
							   by multiplication.  This factor is used
							   to convert scalars (i.e. text height,
							   elevations, etc.) in the system unit to
							   meters by multiplication.  It is also used
							   to convert scalars from meters to the
							   system unit by division. */
	double map_scl;			/* The scale factor to get to the desired
							   map scale by division (e.g. 24000 for a
							   USGS topo quad map).  This feature of
							   CS_MAP is only used when one is trying
							   to produce inches, millimeters, etc. on
							   an existing map.  In this case, one sets
							   the unit to inches, millimeters, whatever,
							   and sets this value appropriately.  Usually,
							   this value is set to 1.0. */
	double scale;			/* A single scale factor which includes
							   all the unit scale and the map scale
							   factors defined above.  This factor
							   must convert meters to coordinate system
							   units by multiplication.  Therefore, it
							   is essentially:
							   
								1.0 / (unit_scl * map_scl).

							   This value is used to convert the ellipsoid
							   equatorial radius to system units before
							   all other calculations are made.  This
							   variable exists primarily for historical
							   reasons. */
	double zero [2];		/* Absolute values of X & Y which are
							   smaller than this are to be converted
							   to a hard zero.  Set by the compiler
							   to the system unit equivalent of .01
							   millimeters by the setup function.
							   This feature is included to prevent
							   output such as 2.345E-05 which is
							   usually undesirable. */

			/* The following values are set to zero
			   by the compiler and are an attempt to
			   prepare for future changes. */

	double hgt_lng;		/* Longitude of the elevation point. */
	double hgt_lat;		/* Latitude of the elevation point. */
	double hgt_zz;		/* Elevation of the coordinate system;
						   typically the actual elevation at
						   elevation average point.  This is
						   an orthometric height, i.e. height
						   above the geoid. */
	double geoid_sep;	/* If defined by the user, the height of
						   the geoid above the ellipsoid, also
						   known as the geoid separation, at the
						   elevation point. */

				/* Lat/Longs outside the rectangle
				   established by the following
				   cause a warning message when
				   converted???  If the min is
				   greater than the max, the
				   setup function generates these
				   automatcially. */
	double ll_min [2];
	double ll_max [2];
				/* XY's outside the rectangle
				   established by the following
				   cause a warning message when
				   converted???  If the min is
				   greater than the max, the
				   setup function generates these
				   automatcially. */
	double xy_min [2];
	double xy_max [2];


	char desc_nm [64];      /* The complete name of the coordinate
							   system. */
	char source [64];		/* Description of where the data for this
							   coordinate system came from. */
	short quad;				/* Quadrant of the cartesian coordinates.
							   Used to handle coordinate systems in
							   which X increases to the left, etc. */
	short order;			/* Order of the complex series, if any
							   used for this coordinate system.
							   Order is currently computed automatically,
							   so this field is currently ignored. */
	short zones;			/* Number of zones in an interrupted
							   coordinate system definition, such
							   as sinusoidal and Goode homolosine.
							   Currently, the number of zones is
							   automatically calculated and this
							   field is ignored. This, of course,
							   could change. */
	short protect;			/* Set to TRUE is this definition is to
							   be protected from being changed by
							   users. */
	short epsg_qd;			/* In the same form as the quad member of this
							   structure, this elelment carries quad as
							   specified by the EPSG database, originally
							   populated with values from EPSG 7.05 */
	short srid;				/* The Oracle SRID number if known, else 0. */
	short epsgNbr;			/* EPSG number, if known */
	short wktFlvr;			/* WKT flavor, if dervied from WKT, else zero */
};
#define cs_BSWP_CSDEF "24c24c24c24c24c24c48c16c8c24ddddddddd2ddddd2d2d2d2d64c64cssss4s"

/*
	We've finished defining all structures which get written to
	disk. We return packing to whatever the user needs.  Thus,
	hopefully, you can use whatever packing you need for your
	application, while I've specified what I need for mine.
*/

#if   _RUN_TIME == _rt_BORLAND16 || _RUN_TIME == _rt_BORLAND32
#	pragma option -a.
#elif _RUN_TIME == _rt_METAWARE
#	pragma Align_members ()
#elif _RUN_TIME == _rt_MSC16 || _RUN_TIME == _rt_MSVC16 || _RUN_TIME == _rt_MSDOTNET || \
														   _RUN_TIME == _rt_MSWIN64 ||  \
														   _RUN_TIME == _rt_MSVC32
#	pragma pack ()
#elif _RUN_TIME == _rt_HPUX
#	pragma pack
#elif _RUN_TIME == _rt_AIX
#	pragma options align=reset
#elif _RUN_TIME >= _rt_UNIXPCC
#	pragma pack ()
#else
	/* I don't know what would work for WATCOM. */
#endif

/*
	The following structure defines the form of a complex
	number.
*/

struct cs_Cmplx_
{
	double real;
	double img;
};

/*
	The following structures are used to compute the meridian arc
	length from the equator to a specific latitude on the ellipsoid.
	Significant computational savings are derived from setting these
	structures up once in the setup function and then using the
	results for the individual calculations. Note, these structures
	are designed for use with the Williams algorithm that we
	now use.

	The nomenclature is Forward: latitude --> distance.
				Inverse: distance --> latitude.

	Distances are always in the same unit as the equatorial
	radius supplied to the setup functions.
*/

struct cs_MmcofF_
{
	double rect_rad;		/* Radius of the rectifying sphere, includes
							   scale reduction where approriate. */
	double a_prime;			/* Horner coefficients. */
	double b_prime;
	double c_prime;
	double d_prime;			/* This term is, perhaps, overkill. Results
							   of +- 3.5E-05 meters are obtained without
							   this term; but we get +-3.75E-06 with it.
							   It costs one add and one multiply as we
							   have implemented it. */
};
struct cs_MmcofI_
{
	double rect_rad;		/* Radius of the rectifying sphere, includes
							   scale reduction where approriate. */
	double coef_0;
	double coef_2;
	double coef_4;
	double coef_6;
};

/*
	The following structure is used to carry the coefficients
	of the series required to compute a modified M used only for the
	American polyconic projection. Quite frankly, I don't know exactly
	what the difference between Mp and the normal M is.
*/
struct cs_MpcofF_
{
	double a_prime;
	double b_prime;
	double c_prime;
	double d_prime;
};

/*
	The following structures are used to carry the coefficients
	required for a computation of geographic latitude from the
	conformal latitude, Chi, and vice versa. The element
	names are taken from Synder, Map Projections - A Working
	Manual, U.S. Geological Survey Professional Paper 1395.
*/

struct cs_ChicofI_
{
	double a_prime;
	double b_prime;
	double c_prime;
	double d_prime;
};

struct cs_ChicofF_
{
	double a_prime;
	double b_prime;
	double c_prime;
	double d_prime;
};


/*
	Same as above, but this time the Authalic (equal area)
	latitude.
*/

struct cs_BtcofF_
{
	double ecent;
	double e_sq;
	double qPole;
	double a_prime;
	double b_prime;
	double c_prime;
	double d_prime;         /* Currently, always zero. */
};

struct cs_BtcofI_
{
	double ecent;
	double e_sq;
	double qPole;
	double a_prime;
	double b_prime;
	double c_prime;
	double d_prime;         /* Currently, always zero. */
};

/*
	The following structure is used for pseudocylindrical
	projections, such as the Sinusoidal, Homolosine, and
	Mollweide, which are commonly used in interrupted form.
	An array of 8 of these structures is used to support
	up to eight different zones.   Each zone has it's own
	central meridian and can be activated for both hemispheres,
	or for each hemisphere individually.

	When we chose 24 as the number of parameters, we thought
	six zones would be sufficient.  We now support eight
	zones.  This leaves use with three doubles to define
	each zone.  The three doubles are used for the west,
	central, and east meridians of each zone.  The following
	convention is used to encode the hemisphere to which
	a zone applies into these three numbers.

	If a zone applies only to the northern hemisphere, the
	magnitude of the westernmost longitude (i.e. prj_prm1)
	is increased by 1000.0.  If a zone applies only to the
	southern hemisphere, the magnitude is increased by 2000.0.
	Note these are magnitudes; thus:

		if (northern)
		{
			if (west_lng < 0.0) west_lng -= 1000.0;
			else		    west_lng += 1000.0;
		}

	In this structure, the longitudes are in radians, greater
	than -PI and less than or equal to PI.  The width is used
	to actually test a lat/long for being in the zone; thus
	the problems of a zone which straddles -180 are avoided.

	Normally, a zone includes the western edge, but not the
	eastern edge.  However, in the eastern most zone, the
	zone also includes the easternmost edge.
*/

struct cs_Zone_
{
	double west_lng;	/* Longitude of the western
						   extent of the zone in
						   radians:
							  -PI < west_lng <= PI */
	double cent_lng;	/* Central meridian of the zone
						   in radians. */
	double east_lng;	/* Longitude of the eastern
						   extent of the zone in
						   radians. */
	double width;		/* Width of the zone in
						   radians.
							 0.0 < width < 2*PI  */
	double west_xx;		/* Western extent of the zone
						   in cartesian units. */
	double east_xx;		/* Eastern extent of the zone
						   in cartesian units. */
	double x_off;		/* The X coordinate of the
						   central meridian, or
						   False Easting of the
						   zone. */
	short  ns_flag;		/* Zero says zone applies to both
						   hemispheres.  -1 say southern only,
						   +1 says northern only. */
	short  east_flg;	/* TRUE says that this is the
						   easternmost zone and the
						   eastern extent is included
						   in the zone. */
};

/*
	The following structure is designed to contain
	all the variables required to convert between
	a Lambert projection and latitudes and longitudes
	for a specific instance, i.e. an established set of
	reference parallels.  Once the reference parallels
	are established, all of these values need be
	computed only once.  The element names have been
	chosen to jive with the USGS publication.
*/
#define cs_LMBRT_TWOSP 1
#define cs_LMBRT_ONESP 2
#define cs_LMBRT_BELGN 3
#define cs_LMBRT_MICHIGAN 4
#define csLMBRT_AFFINE (1 << 9)	/* This bit is set in the quad variable
								   when the affine post processor is active. */

struct cs_Lmbrt_
{
	double org_lng;			/* The longitude of the origin
							   of the coordinate system.  Value is
							   in radians. */
	double org_lat;			/* The latitude of the origin.  Value is
							   in radians. */
	double k;				/* The scale factor in use.  This includes
							   the map scale and the units conversion
							   from the meters in which ellipsoids
							   are defined to whatever the
							   coordinate system units are. */
	double x_off;			/* The false easting in coordinate
							   system units. */
	double y_off;			/* The false northing in coordinate
							   system units. */
	double belgium;			/* This implements the Belgian variation.
							   Zero if not the Belgian variation.
							   29.2985 seconds (in radians) if Belgian. */
	double e_rad;			/* Equatorial radius of the ellipsoid in
							   use.a */
	double ecent;			/* The ecentricity of the datum in
							   use. */
	double e_sq;			/* The square of the ecentricity of the
							   datum in use. */
	double e_ovr_2;			/* One half of the ecentricity. */
	double pole_test;		/* The length, in coordinate system units,
							   of the equialent of .001 seconds of
							   arc, used to test a cartesian coordinate
							   for proximity with a pole. */
	double theta_max;		/* Maximum magnitude of theta which is
							   within the domain of the coordinate
							   system. */
	double infinity;		/* The value used for rho when the
							   coordinate being converted is outside
							   the mathematical domain of the
							   coordinate system. */

				/* The following are parameters
				   which can be computed once
				   given the above information.
				   Nomenclature follows that
				   used in the USGS publication.
				   A digit following a name should (usually)
				   be read as a subscript. */

	double t0;
	double n;				/* The cone constant, negative for the
							   Southern aspect. */
	double one_ovr_n;		/* reciprocal of n */
	double pi_o2n;			/* Latitude of the focus pole, pi over 2
							   with the sign of n. */
	double F;				/* Akin to the scale reduction factor of
							   other projections. */
	double rho0;			/* Radius of the arc representing origin
							   latitude. */
	double aF;				/* The actual radius used in the cartesian
							   to polar (and vice versa) conversions. */
	double k_tlat;			/* Tan phi1 term for spherical scale
							   calculations. */
	double k0;				/* scale reduction factor, used only in the
							   grid scale functions. */
	double ellipsoidK;		/* Ellipsoid scale factor, used only in the
							   MICHIGAN variation as of this writing. */

	/* Affine post processor stuff */
	double affineA0;		/* Coefficients for Affine transformation. */
	double affineA1;
	double affineA2;
	double affineB0;
	double affineB1;
	double affineB2;
	double affineK;			/* Affect on the grid scale factor.  Note, the
							   affine process makes this thing non-conformal! */
	double affineGamma;		/* Affect on the convergence angle.  This should
							   be consistent. */
	double affineDenom;		/* Constant term used in the affine inverse. */

	struct cs_ChicofI_ chicofI;
							/* The coefficients of the series expansion
							   for converting the conformal latitude,
							   Chi, to the ellipsoidal geodetic
							   latitude.  See CSchiIsu and CSchiIcal. */
	short quad;				/* Non-zero specifies a cartesian system
							   other than the standard right handed
							   system. */
};

/*
	The following structure is designed to contain
	all of the variables required to convert to the
	Transverse Mercator projection from latitude and
	longitudes and vice versa.  The UTM coordinate
	system is a special case of this projection, as is
	the State Plane Coordinate System for certain states.
*/

#define csTRMER_MNDOT (1 << 8)	/* This bit is set in the quad variable
								   when the Minnesota DOT variation is
								   being used. */
#define csTRMER_AFFINE (1 << 9)	/* This bit is set in the quad variable
								   when the affine post processor is active. */
#define csTRMER_OSTN97 (1 << 10)/* This bit is set in the quad variable
								   when the OSTN97 post processor is active. */
#define csTRMER_OSTN02 (1 << 11)/* This bit is set in the quad variable
								   when the OSTN02 post processor is active. */
struct cs_Trmer_
{
	double cent_lng;		/* Longitude of the central meridian.
							   There are 60 of these associated
							   with the UTM system.  In the
							   State Plane Coordinate System,
							   each zone has its own specific
							   central meridian.  In either
							   case, it represents the longitude
							   origin.  The value is in radians. */
	double org_lat;			/* Latiude of the origin of the
							   map.  This is usually zero for
							   standard UTM maps.  However,
							   it is set to specific values
							   for each State Plane Zone.
							   The value is in radians. */
	double k;				/* Scale factor on the central
							   meridian.  This includes the
							   map scale factor, the units
							   conversion, and the scale
							   reduction factor. */
	double k0;				/* This is the actual scale reduction
							   factor, the scale function needs
							   this unencumbered by other
							   scale factors. */
	double x_off;			/* The false easting, in coordinate
							   system units. */
	double y_off;			/* The false northing, in coordinate
							   system units. */
				/* Datum values captured from the
				   active datum, repeated here
				   to prevent recalculation. */
	double ecent;
	double e_sq;
	double e_rad;

	double Rk;				/* Scaled radius of the earth. */
	double Rk_ovr_2;		/* Scaled radius of the earth divided
							   by two. */
	double var_K;			/* Scale factor used by certain
							   variations of the basic projection. */
	double eprim_sq;		/* The ecenticity prime squared
							   value required by the transformation
							   which can be computed once, once
							   the datum has been specified. */
	double xx_max;			/* The X coordinate value, in user units,
							   but not includeing false origin, which
							   we use to approximate the eastern and
							   western extents of the projectionable
							   region. */
	double M0;				/* Meridonal distance from the equator
							   to the origin latitude. */

	double affineA0;		/* Coefficients for Affine transformation. */
	double affineA1;
	double affineA2;
	double affineB0;
	double affineB1;
	double affineB2;
	double affineK;			/* Affect on the grid scale factor.  Note, the
							   affine process makes this thing non-conformal! */
	double affineGamma;		/* Affect on the convergence angle.  This should
							   be consistent. */

	double nTerm;			/* Term used in the Kruger formulation */
	double aHat;			/* Term used in the Kruger formulation */
	double k0aHat;			/* Term used in the Kruger formulation */
	double beta1;			/* Term used in the Kruger formulation */
	double beta2;			/* Term used in the Kruger formulation */
	double beta3;			/* Term used in the Kruger formulation */
	double beta4;			/* Term used in the Kruger formulation */
	double delta1;			/* Term used in the Kruger formulation */
	double delta2;			/* Term used in the Kruger formulation */
	double delta3;			/* Term used in the Kruger formulation */
	double delta4;			/* Term used in the Kruger formulation */

	double affineDenom;		/* Constant term used in the affine inverse. */

	struct cs_MmcofF_ mmcofF;/* Coefficients necessary to compute
							   meridonal distance from the equator. */
	struct cs_MmcofI_ mmcofI;/* Coeffieients necessary to compute the
							   foot print lataitude. */
	struct cs_ChicofF_ chicofF;/* Coefficients for calculating conformal
								  latitude by series */
	struct cs_ChicofI_ chicofI;/* Coefficients for converting conformal
								  latitude back to geographic latitude by
								  series. */

	short quad;				/* Non-zero specifies a cartesian system
							   other than the standard right handed
							   system.  Can also carry bits concerning
							   specific variations. This bundling is
							   done for performance reasons. */
	short kruger;			/* non-zero says use the Kruger Formulation */
};

/*
	The following is the structure definition for the
	Transverse Mercator ala Snyder. While this is not
	as precise, or as fast, as the above implementation,
	it produces the exact numbers some folks like to see.
	So, since the customer is always right. . . .
*/

struct cs_Trmrs_
{
	double cent_lng;		/* Longitude of the central meridian.
							   There are 60 of these associated
							   with the UTM system.  In the
							   State Plane Coordinate System,
							   each zone has its own specific
							   central meridian.  In either
							   case, it represents the longitude
							   origin.  The value is in radians. */
	double org_lat;			/* Latiude of the origin of the
							   map.  This is usually zero for
							   standard UTM maps.  However,
							   it is set to specific values
							   for each State Plane Zone.
							   The value is in radians. */
	double k;				/* Scale factor on the central
							   meridian.  This includes the
							   map scale factor, the units
							   conversion, and the scale
							   reduction factor. */
	double k0;				/* This is the actual scale reduction
							   factor, the scale function needs
							   this unencumbered by other
							   scale factors. */
	double x_off;			/* The false easting, in coordinate
							   system units. */
	double y_off;			/* The false northing, in coordinate
							   system units. */
				/* Datum values captured from the
				   active datum, repeated here
				   to prevent recalculation. */
	double ecent;
	double e_sq;
	double e_rad;

	double Rk;				/* Scaled radius of the earth. */
	double Rk_ovr_2;		/* Scaled radius of the earth divided
							   by two. */
	double eprim_sq;		/* The ecenticity prime squared
							   value required by the transformation
							   which can be computed once, once
							   the datum has been specified. */

				/* The following four values are the
				   coefficients used to compute
				   M for a given latitude.  They
				   need only be computed once,
				   once the datum is known. */
	double Mcoef0;
	double Mcoef2;
	double Mcoef4;
	double Mcoef6;
	double M0;              /* The value of M0, the distance
							   along the surface of the
							   ellipsoid from the equator to the
							   latitude origin of the map.  This
							   is normally zero for standard
							   UTM maps, but could be something
							   other than zero for a Transverse
							   Mercator other than a UTM. */

				/* The following are coefficients
				   and values used to compute the
				   inverse which need only be
				   computed once. */
	double e1;
	double mucoef2;
	double mucoef4;
	double mucoef6;
	double mucoef8;
	double xx_max;			/* The X coordinate value, in user units,
							   but not including false origin, which
							   we use to approximate the eastern and
							   western extents of the projectionable
							   region. */
	short quad;				/* Non-zero specifies a cartesian system
							   other than the standard right handed
							   system.  Can also carry bits concerning
							   specific variations. This bundling is
							   done for performance reasons. */
};

/*
	The following structure is designed to contain
	all of the variables required to convert to the
	Albers Equal Area projection from latitude and
	longitudes and vice versa.

	This structure includes all of the projection
	parameters as well as mapping values which need
	only be computed once per map.
*/

struct cs_Alber_
{
	double ref_lat1;		/* Latitude of the first of two
							   standard parallels, the essence
							   of any conic projection.  Value
							   is in radians. */ 
	double ref_lat2;		/* Latitude of the second standard
							   parallel, usually the most
							   northerly of the two.  Value is
							   in radians. */
	double org_lng;			/* The longitude of the origin
							   of the coordinate system, i.e. the
							   location where x == 0.  Value is
							   in radians. */
	double org_lat;			/* The latitude of the origin.  Value is
							   in radians. */
	double k;				/* The scale factor in use.  This includes
							   map scale factor and units conversion. */
	double x_off;			/* The false easting, in coordinate
							   system units. */
	double y_off;			/* The false northing, in coordinate
							   system units. */
	double ka;
	double ecent;			/* The ecentricity of the datum in
							   use. */
	double e_sq;			/* The square of the ecentricity of the
							   datum in use. */

				/* The following are parameters
				   which can be computed once
				   given the above information.
				   Nomenclature follows that
				   used in the USGS publication. */
	double n;				/* The cone constant.  Always greater than
							   zero, and always less than one.
							   Otherwise we wouldn't have a cone. */
	double C;				/* Constant related to the radius of the arc
							   for the focus pole. */
	double rho0;			/* Radius of the arc for the origin
							   latitude. */
	double max_rho;			/* Maximum value of rho for the coordinate
							   system. */
	double min_rho;			/* minimum value of rho for the coordinate
							   system, used to test a cartesian
							   coordinate for being at the focus pole. */
	double theta_max;		/* Maximum theta for this coordinate system */
	double one_mm;			/* Equivalent of one millimeter in the
							   system unit. */
	double R_ovr_n;			/* e_rad / n */
	double one__esq;		/* 1.0 - e_sq */
	double one_o_2e;		/* 1.0 / (2.0 * ecent) */
	double beta_term;		/* This term includes all factors
							   necessary to compute the sine of
							   Beta, given q, by multiplication. */
	struct cs_BtcofI_ btcoefI;
							/* Coefficients for the inverse
							   calculation (i.e. power series) of
							   geographic latitude from the authalic
							   latitude Beta. */
	short quad;				/* Non-zero specifies a cartesian system
							   other than the standard right handed
							   system. */
};

/*
	The following structure is used for all variations of the Oblique
	Mercator projection.  This includes	the HOM1UV, HOM1XY, HOM2UV,
	and HOM2XY projections. 	They all use the same math with slight
	variations. Therefore, much of all of this is shared.
	
	An inclusive OR of defines next given are used to select
	variations upon calling the general setup function. Several
	of these defines are assigned a zero value. These can be
	used to add documentation to your code.
*/

#define cs_OBLQM_1POINT   0				/* one point mode */
#define cs_OBLQM_2POINT  (1 <<  0)		/* two point mode */
#define cs_OBLQM_UNRECT   0				/* unrectified cartesians */
#define cs_OBLQM_RECT    (1 <<  3)		/* normal rectification */
#define cs_OBLQM_ALASKA  (1 <<  4)		/* alaska special rectification */
#define cs_OBLQM_UVORG    0				/* u & v == 0 at proj origin */
#define cs_OBLQM_UVCTR   (1 <<  6)		/* u & v == 0 at proj center */

struct cs_Oblqm_
{
	double lng_1;			/* Longitude of the first point, in
							   radians, in the two point case. */
	double lat_1;			/* Latitude of the first point, in
							   radians, in the two point case. */
	double lng_2;			/* Longitude of the second point, in
							   radians, in the two point case. */
	double lat_2;			/* Latitude of the second point, in
							   radians, in the two point case. */
	double lng_c;			/* Longitude of the center of the map,
							   in radians. */
	double lat_c;			/* Longitude of the center of the map,
							   in radians. */
	double lng_0;			/* Longitude of the projection origin,
							   as computed by CSoblqmS, in radians. */
	double lat_0;			/* Latitude of the projection origin,
							   as computed by CSoblqmS, in radians. */
	double az;				/* The azimuth of the central line, in
							   radians, at the projection center.
							   Computed in the two point case. */
	double lng_p;			/* The longitude of the primary pole,
							   in radians.  Computed only for the
							   spherical case.*/
	double lat_p;			/* The latitude of the primary pole,
							   in radians.  Computed only for the
							   spherical case.*/
	double k;				/* Scale factor on the central
							   line.  This includes map scale,
							   units conversion, and scale
							   reduction. */
	double k0;				/* The actual scale reduction.  The
							   scale function needs this
							   unencumbered by the other factors. */
	double x_off;			/* The false easting, in coordinate
							   system units. */
	double y_off;			/* The false northing, in coordinate
							   system units. */
	double uc;				/* u coordinate at the center of
							   the projection. Vc is always zero, */

				/* Datum values captured from the
				   active datum, repeated here
				   to prevent recalculation. */
	double ecent;
	double e_sq;
	double e_ovr_2;
	double e_rad;
				/* Other constants which are computed
				   once per map. */

	double ak;				/* Scaled radius of the earth. */
	double sin_lat_p;		/* Sine and cosine of the location of
							   the pole.  Computed for the sphere
							   only. */
	double cos_lat_p;
	double sin_az;			/* Sine and cosine of the provided,
							   sometimes computed, azimuth of the
							   central line. */
	double cos_az;
	double vv_max;			/* Absolute value of the maximum value
							   of vv which we consider in range;
							   about .001 seconds of arc short of
							   90 degrees from the central line. */
	double uu_max;			/* Similarly, maximum uu value. */
	double sin_gam_0;		/* Sine of gamma zero. */
	double cos_gam_0;		/* Cosine of gamma zero. */
	double A;
	double B;
	double A_ovr_B;			/* A = divided by B. */
	double one_ovr_B;		/* one divided by B. */
	double E;
	double gamma_0;			/* The azimuth of the central line at the
								projection origin. */
	struct cs_ChicofI_ chicofI;
				/* The coefficients of the series expansion
				   for converting the conformal latitude,
				   Chi, to the ellipsoidal geodetic
				   latitude.  See CSchiIsu and CSchiIcal. */
	short one_pnt;			/* TRUE (i.e. non-zero) indicates that
							   the one point form was used to establish
							   the central geodesic for this system. */
	short uv_ctr;			/* TRUE (i.e. non-zero) indicates that
							   the unrectified coordinates are to be related
							   to the projection center, as opposed to
							   the projection origin. */			
	short rect_flg;			/* Sepcifies if, and how, the cartesian
							   coordinates are to be rectified. */
	short azmIsSkew;		/* Flag which indicates at the point on the
							   central geodesic the azimuth parameter
							   applies.  Zero says the projection center
							   (the traditional value for CS-MAP) and
							   non-zero says the projection origin.  In
							   the latter case, the azimuth is often called
							   the skew azimuth. */
	short quad;				/* Non-zero specifies a cartesian system
							   other than the standard right handed
							   system. */
};
/*
	The following defines are used internal to the Oblique
	Mercator code, They should not be used by any code not
	a part of the Oblique Mercator set of functions.
*/
#define csOBLQM_UNRECT 0
#define csOBLQM_RECT   1
#define csOBLQM_ALASKA 2

/*
	The following structure is designed to contain
	all of the variables required to convert to the
	Mercator projection from latitude and longitudes
	and vice versa.
*/

struct cs_Mrcat_
{
	double cent_lng;		/* Longitude of the central meridian.
							   It represents the longitude origin.
							   The value is in radians. */
	double std_lat;			/* Latiude of the standard parallel
							   of the system.  This is usually zero
							   for most uses of the Mercator.
							   Non-zero values have the same affect
							   as scale reduction in the Transverse
							   Mercator and other Cylindricals.
							   The value is in radians. */
	double k;				/* Scale factor.  Includes map scale
							   and units conversion.  Does NOT
							   include reduction due to standard
							   parallel. */
	double x_off;			/* The offset to be added to the
							   calculated x values.  This can
							   be used to cause points to the
							   west of the central meridian to
							   have positive values.  Note, the
							   offset is applied after scaling. */
	double y_off;			/* The y offset, applied after
							   scaling. */

				/* Datum values captured from the
				   active dataum, repeated here
				   to prevent recalculation. */
	double ecent;
	double e_sq;

	double cos_sp;			/* The cosine of the latitude of the
							   standard parallel; properly adjusted
							   for the ellipsoid in the ellipsoidal
							   case.  For the common Mercator, this
							   is 1.0 in both cases; i.e. the standard
							   parallel is the equator. */
	double Rk;				/* Scaled radius of the earth.  In the
							   ellipsoidal case, this is the scaled
							   equatorial radius. */
	double Rk_ovr_2;		/* Scaled radius of the earth, divided by
							   2.0.  In the ellipsoidal case, this is
							   the equatorial radius. */
	double Rfact;			/* Scaled radius factor which includes the
							   possible affect of a standard parallel
							   other than the equator. (Rk * cos_sp) */
	double Rfact_2;			/* Scaled radius factor divided by 2.0.
							   Includes the possible affect of a
							   standard parallel other than the
							   equator. (Rfact / 2.0) */
	double yy_max;			/* The Y value which arbitraly use to
							   represent the north pole which should,
							   theoretically, be infinitiy.  Does
							   not include the false origin, if
							   any. */
	double eastLimit;		/* The eastern limit of the range of the
							   CRS definition, relative to the central
							   meridian, in radians.  That is, a
							   copy of the cs_Csdef_ ll_max [0] value made
							   relative to the central meridian and converted
							   to radians. The value is typically a positive
							   value indicating that it refers to a meridian
							   which is east of the central meridian.
							   Necessary here as the CSmrcatF & CSmrcatI
							   functions do not have access to the cs_Csdef_
							   structure for the active CRS definition. */
	double westLimit;		/* The western limit of the range of the
							   CRS definition, relative to the central
							   meridian, in radians.  That is, a
							   copy of the cs_Csdef_ ll_max [0] value made
							   relative to the central meridian and converted
							   to radians.  The value is typically a negative
							   value indicating that it refers to a meridian
							   which is west of the central meridian.
							   Necessary here as the CSmrcatF & CSmrcatI
							   functions do not have access to the cs_Csdef_
							   structure for the active CRS definition. */

	struct cs_ChicofI_ chicofI;
							/* The coefficients of the series expansion
							   for converting the conformal latitude,
							   Chi, to the ellipsoidal geodetic
							   latitude.  See CSchiIsu and CSchiIcal. */
	short quad;				/* Non-zero specifies a cartesian system
							   other than the standard right handed
							   system. */
	unsigned short prj_code;
							/* Implementation of the Popular
							   Visualisation Pseudo Mercator
							   requires something to tell the
							   calculation functions which
							   variation is being used (specifically
							   the scale functions) */
};

/*
	The following structure is designed to contain
	all of the variables required to convert to the
	Azimuthal Equidistant projection from latitude and
	longitudes and vice versa.
*/

#define cs_AZMED_POINT 1
#define cs_AZMED_ANGLE 2

#define cs_AZMED_NORTH   1
#define cs_AZMED_SOUTH   2
#define cs_AZMED_EQUATOR 3
#define cs_AZMED_OBLIQUE 4
#define cs_AZMED_GUAM 5

struct cs_Azmed_
{
	double org_lng;			/* Origin longitude, in radians. */
	double org_lat;			/* Origin latitude, in radians. */
	double nrth_lng;		/* Longitude of the northing reference
							   point in radians. */
	double nrth_lat;		/* Latitude of the northing reference
							   point in radians. */ 
	double k;				/* The mapping scale factor, includes
							   map scale and units of measure. */
	double x_off;			/* The coordinate system X value for the
							   origin point, also known as the
							   False Easting. */
	double y_off;			/* The coordinate system Y value for the
							   origin point, also known as the
							   False Northing. */

	double Az;				/* The azimuth, east of north, of the
							   rotation implied by the northing
							   point, in radians. */
	double quad_d;			/* The quad parameter from the
							   coordinate system definition.
							   This parameters is used to handle
							   left handed coordinate systems.
							   The value is always taken as an
							   integer.  An integer value outside
							   of the range of 1 thru 4 is
							   considered to be a 1. */
	double ka;				/* The scaled equitorial radius. */
	double two_ka;			/* 2 * ka */
	double x_scale;			/* A scale factor to be applied
							   independently to the X values of the
							   coordinate system.  Specifically
							   intended for use to handle left
							   handed coordinate systems. */
	double y_scale;			/* A scale factor to be applied
							   independently to the Y values of the
							   coordinate system.  Specifically
							   intended for use to handle left
							   handed coordinate systems. */
	double ecent;			/* Eccentricity of the ellipsoid being
							   used. */
	double e_sq;			/* Sqaure of the eccentricity. */
	double one_esq;			/* One minus eccentricity squared. */
	double rt_one_esq;		/* sqrt (One minus eccentricity squared). */

	double cos_org_lat;		/* Cosine of the origin latitude. */
	double sin_org_lat;		/* Sine of the origin latitude. */
	double sin_cos;			/* Product of sin_org_lat and sin_org_lat. */
	double e_sq_cos_sq;		/* e_sq times cos_org_lat squared. */
	double e_sq_sin_sq;		/* e_sq times sin_org_lat squared. */
	double one_mm;			/* Equivalent of one millimeter in the
							   system unit. */
	double max_rho;			/* Radius, in system units, of the circle
							   which defines the caartesian extent
							   of the system. */
	double cos_Az;			/* Cosine of the northing azimuth. */
	double sin_Az;			/* Sine of the northing azimuth. */

	struct cs_MmcofF_ mmcofF;
							/* The coefficients necessary to compute
							   meridonal distance from the equator. */
	struct cs_MmcofI_ mmcofI;
							/* The coefficients necessary to compute
							   latitude from the meridonal distance. */

	double Mp;				/* Distance of the pole from the equator
							   for this ellipsoid. */
	double M1;				/* Distance of the origin latitude from
							   the equator for this ellipsoid. */
	double e_sin_p1;		/* e * sine (org_lat) */
	double e_cos_p1;		/* e * cos (org_lat) */
	double N1;				/* N1 per Synder, page 199. */
	double psi_t1;			/* e * e * N1 * sin (org_lat) */
	double G;				/* e * sin_org_lat / rt_one_esq */
	double G_sq_3;			/* 3.0 times G squared */

	double e_one;			/* [1-sqrt(1-e_eq)]/[1+sqrt(1-e_sq] */
	short aspect;			/* Set to the appropriate define
							   for the aspect in use. */
	short quad;				/* Non-zero specifies a cartesian system
							   other than the standard right handed
							   system. */
};

/*
	The following structure is designed to contain all of the
	variables required to convert to the Lambert Tangential
	projection from latitude and longitudes and vice versa.
*/

struct cs_Lmtan_
{
	double org_lng;			/* Origin longitude, in radians. */
	double org_lat;			/* Origin latitude, in radians. */
	double k;				/* The mapping scale factor, includes
							   map scale, scale reduction factor,
							   and units conversion. */
	double x_off;			/* The coordinate system X value for the
							   origin point, also known as the
							   False Easting. */
	double y_off;			/* The coordinate system Y value for the
							   origin point, also known as the
							   False Northing. */

	double ecent;			/* Eccentricity of the ellipsoid being
							   used. */
	double e_sq;			/* Square of the eccentricity. */
	double kerad;			/* Equatorial radius of the earth in
							   system units, no scale reduction. */
	double e_ovr_2;			/* Eccentricity / 2 */

	double sin_org_lat;		/* Sine of the origin latitude. */
	double ka;				/* Scaled equatorial radius of the
							   earth. */
	double one_mm;			/* equivalent of one millimeter in
							   system units. */
	double max_Gamma;		/* Maximum polar angle in radians. */
	double max_R;			/* Maximum polar radius. */

	double N0;				/* Radius of curvature of the prime
							   vertical section. */
	double R0;				/* Radius of the arc of origin latitude. */
	double c;
	double abs_c;			/* absolute value of c. */
	double L0;
	short quad;				/* Non-zero specifies a cartesian system
							   other than the standard right handed
							   system. */
};

/*
	The following structure contains all of the information
	necessary to perform conversions based on the American
	Polyconic Projection.
*/

struct cs_Plycn_
{
	double cent_lng;		/* Longitude of the central meridian,
							   in radians. */
	double org_lat;			/* Latiude of the origin of the
							   map.  That is, the latitude
							   which is orthoginal to the
							   central meridian.  The value
							   is in radians. */
	double x_off;			/* The offset to be added to the
							   calculated x values.  This can
							   be used to cause points to the
							   west of the central meridian to
							   have positive values.  Note, the
							   offset is applied after scaling. */
	double y_off;			/* The y offset, applied after
							   scaling. */
				/* Datum values captured from the
				   active dataum, repeated here
				   to prevent recalculation. */
	double ecent;			/* Ellipsoid eccentricity. */
	double e_sq;			/* Ellipsoid eccentricity squared. */
	double e_rad;           /* Usually referred to as "a", the
							   equatorial radius of the ellipsoid. */
	double e_rad_sq;		/* The equatorial radius of the ellipsoid
							   squared. */
	double one_e_sq;		/* One minus eccentricity squared, for the
							   scaling calculation. */

	double small_xy;		/* Set to the value of X and Y coordinates
							   which corresponds to 0.001 seconds of
							   arc, the angle we use to determine
							   proximity to the equator and/or the
							   poles. */
	double k;				/* The scale of the coordinate system,
							   includes unit scaling (from meters
							   to the coordinate system unit) and
							   the map scale factor. */
	double Rk;				/* Scaled radius of the earth, in the
							   units of the coordinate system. */
	double Rk_ovr_2;		/* Scaled radius of the earth divided
							   by two. */
	double Rk_sq;			/* Scaled radius of the earth squared. */
	double Rlat0;			/* The Y cartesian value, sans offsets,
							   of all geodetic locations on the
							   equator.  (Normal calculation are
							   indeterminate on the equator.) */
	double M0;				/* The meridonal distance from the equator
							   to the origin latitude. */
	double max_rho;			/* Cartesian distance from the intersection
							   of the equator and the origin longitude
							   which represents the domain of the
							   coordinate system.  We can't inverse
							   X's and Y's outside this range. */
	double max_rho_sq;		/* Max_rho, squared. For performance. */
	struct cs_MmcofF_ mmcofF;
				/* The coefficients necessary to compute
				   meridonal distance from the equator. */
	struct cs_MpcofF_ mpcofF;
							/* Coefficients necessary to compute Mp. */
	short quad;				/* Non-zero specifies a cartesian system
							   other than the standard right handed
							   system. */
};

/*
	The following structure carries all parameters necessary
	for the calculations necessary for the Modified Polyconic
	as used in the International Map of the World.  That is,
	the polyconic as modified by Lallemand.  This projection
	is usually used to map a region of 6 degrees of longitude
	by 4 degrees of latitude.
*/

struct cs_Modpc_
{
	double lng0;			/* Longitude of the central meridian,
							   in radians. */
	double lng1;			/* Longitude of the eastern standard
							   meridian in radians. */
	double lat1;			/* Latitude of the southern standard
							   parallel in radians. */
	double lat2;			/* Latitude of the northern standard
							   parallel in radians. */
	double x_off;			/* The offset to be added to the
							   calculated x values.  This can
							   be used to cause points to the
							   west of the central meridian to
							   have positive values.  Note, the
							   offset is applied after scaling. */
	double y_off;			/* The y offset, applied after
							   scaling. */
				/* Datum values captured from the
				   active dataum, repeated here
				   to prevent recalculation. */
	double ecent;			/* Ellipsoid eccentricity. */
	double e_sq;			/* Ellipsoid eccentricity squared. */
	double e_rad;			/* Usually referred to as "a", the
							   equatorial radius of the ellipsoid. */
	double n_limit;			/* Northern latitude limit. */
	double s_limit;			/* Northern latitude limit. */
	double small_xy;		/* Value used to terminate the iterative
							   inverse solution. */
	double max_xx;			/* The maximum magnitude of X (sans false
							   origin) that is within the domain of
							   the coordinate system. */
	double max_yy;			/* The maximum value of Y (sans false
							   origin) that is within the domain of
							   the coordinate system. */
	double min_yy;			/* The minimum value of X (sans false
							   origin) that is within the domain of
							   the coordinate system. */
	double k;				/* The scale of the coordinate system,
							   includes unit scaling (from meters
							   to the coordinate system unit) and
							   the map scale factor. */
	double ak;				/* Scaled radius of the earth, in the
							   units of the coordinate system. */

	double R1;				/* Radius of the arc which defines the
							   the southern standard parallel. */
	double R2;				/* Radius of the arc which defines the
							   the northern standard parallel. */
	double C2;				/* Distance on the map (i.e. distance
							   in coordinate system units) from
							   lat1 to lat2 along the central
							   meridian. */
	double sin_lat2;		/* The sine of the northern standard
							   parallel. */
	double sin_lat1;		/* The sine of the southern standard
							   parallel. */
	double cos_lat2;		/* The cosine of the northern standard
							   parallel. */
	double P_x;
	double Q_x;
	double P_y;
	double Q_y;
	struct cs_MmcofF_ mmcofF;
							/* Coefficients necessary to compute
							   meridonal distance from the equator. */
	short quad;				/* Non-zero specifies a cartesian system
							   other than the standard right handed
							   system. */
};

/*
	The following supports the Lambert Equal Area Azmuthal
	projection.
*/

#define cs_AZMEA_POINT   1
#define cs_AZMEA_ANGLE   2

#define cs_AZMEA_NORTH   1
#define cs_AZMEA_SOUTH   2
#define cs_AZMEA_EQUATOR 3
#define cs_AZMEA_OBLIQUE 4

struct cs_Azmea_
{
	double org_lng;			/* Origin longitude, in radians. */
	double org_lat;			/* Origin latitude, in radians. */
	double k;				/* The mapping scale factor, includes
							   map scale and units of measure. */
	double x_off;			/* The coordinate system X value for the
							   origin point, also known as the
							   False Easting. */
	double y_off;			/* The coordinate system Y value for the
							   origin point, also known as the
							   False Northing. */
	double Az;				/* The azimuth, east of north, of the
							   rotation implied by the northing
							   point, or specified by the user,
							   in radians. */
	double quad_d;			/* The quad parameter from the
							   coordinate system definition.
							   This parameter is used to handle
							   left handed coordinate systems.
							   The value is always taken as an
							   integer.  A integer value outside
							   of the range of 1 thru 4 is
							   considered to be a 1. */

	double ka;				/* The scaled equitorial radius.  Used
							   as the radius of the sphere, i.e.
							   R in the reference. */
	double x_scale;			/* A scale factor to be applied
							   independently to the X values of the
							   coordinate system.  Specifically
							   intended for use to handle left
							   handed coordinate systems. */
	double y_scale;			/* A scale factor to be applied
							   independently to the Y values of the
							   coordinate system.  Specically
							   intended for use to handle left
							   handed coordinate systems. */
	double one_mm;			/* Actually, one tenth of a millimeter
							   in terms of the cartesian system
							   units.  Used to trap zero coordinate
							   values. */
	double ecent;			/* Eccentricity of the ellipsoid being
							   used. */
	double e_sq;			/* Sqaure of the eccentricity. */
	double one_o_2e;		/* One divided by (o) two times
							   the eccentricity. */
	double one_esq;			/* One minus eccentricity squared. */
	double two_ka;			/* Two times the scaled radius. */

	double cos_org_lat;		/* Cosine of the origin latitude. */
	double sin_org_lat;		/* Sine of the origin latitude. */
	double cos_Az;			/* Cosine of the northing azimuth. */
	double sin_Az;			/* Sine of the northing azimuth. */

	double beta1;			/* Authalic latitude of the origin. */
	double sin_beta1;		/* Sine of beta1. */
	double cos_beta1;		/* Cosine of beta1. */
	double D;				/* The D value for this coordinate system. */
	double Rq;				/* Rq value for this coordinate system. */
	double qp;				/* Q at the projection pole. */
	double Rsq_ovr_a;		/* Rq squared divided by equatorial radius */
	double D_cos_b1;		/* D times the cosine of beta1. */
	double Dsq_sin_b1;		/* D squared times the sine of beta1. */
	double two_Rq;			/* 2 times Rq. */
	double asq_qconv;		/* ka squared times a bunch of eccentricity
							   stuff. */
	double two_asq;			/* 2 * asq_qconv */
	struct cs_BtcofI_ btcoefI;
							/* Coefficients for the inverse
							   calculation (i.e. power series) of
							   geographic latitude from the authalic
							   latitude Beta. */
	short polar;			/* Set to indicate the aspect:
								cs_AZMEA_NORTH,
								cs_AZMEA_SOUTH,
								cs_AZMEA_EQUATOR,
								cs_AZMEA_OBLIQUE. */
	short quad;				/* Non-zero specifies a cartesian system
							   other than the standard right handed
							   system. */
};

/*
	The following structure is defines the Equidistant
	Conic Projection.
*/

struct cs_Edcnc_
{
	double ref_lat1;		/* Latitude of the first of two
							   standard parallels, the essence
							   of the Lambert Conformal Conic
							   projection.  Value is in radians. */ 
	double ref_lat2;		/* Latitude of the second standard
							   parallel, usually the most
							   northerly of the two.  Value is
							   in radians. */
	double org_lng;			/* The longitude of the origin
							   of the coordinate system, i.e. the
							   location where x == 0.  Value is
							   in radians. */
	double org_lat;			/* The latitude of the origin.  Value is
							   in radians. */
	double k;				/* The scale factor in use. */
	double ka;				/* Scaled equatorial radius. */
	double x_off;			/* A value used to preclude negative
							   numbers in the resulting map. */
	double y_off;			/* A value used to preclude negative
							   numbers in the map. */
	double ecent;			/* The ecentricity of the datum in
							   use. */
	double e_sq;			/* The square of the ecentricity of the
							   datum in use. */

	double n;				/* The cone constant. */
	double G;
	double kaG;
	double rho0;			/* Radius of org_lat in the cartesian
							   system. */
	double min_rho;			/* Radius of the arc depicting, for example,
							   the north pole. */
	double max_rho;			/* Radius of the arc depicting, for example,
							   the south pole. */
	double max_theta;		/* The maximum polar angle. */
	struct cs_MmcofF_ mmcofF;
							/* The coefficients necessary to compute
							   meridonal distance from the equator. */
	struct cs_MmcofI_ mmcofI;
							/* Coeffieients neceaary to compute the
							   foot print lataitude. */
	short quad;				/* Non-zero specifies a cartesian system
							   other than the standard right handed
							   system. */
};

/*
	The following structure supports the calculations associated
	with the Miller Cylindrical projection.
*/

struct cs_Millr_
{
	double cent_lng;		/* Longitude of the central meridian.
							   It represents the longitude origin.
							   The value is in radians. */
	double k;				/* Scale factor on the central meridian. */
	double x_off;			/* The offset to be added to the
							   calculated x values.  This can
							   be used to cause points to the
							   west of the central meridian to
							   have positive values.  Note, the
							   offset is applied after scaling. */
	double y_off;			/* The y offset, applied after
							   scaling. */
	double ka;				/* Scaled radius of the sphere. */
	double ka_16;			/* Scaled radius of the sphere, divided
							   by 1.6 */
	double ka_8;			/* Scaled radius of the sphere, multiplied
							   by 0.8 */
	double max_xx;			/* The maximum magnitude of the x coordinate,
							   sans the false easting. */
	double max_yy;			/* The maximum magnitude of the y coordinate,
							   sans the false northing. */
	short quad;				/* Non-zero specifies a cartesian system
							   other than the standard right handed
							   system. */
};

/*
	The following supports the Sterographic projection.
*/

#define cs_STERO_NORTH       1
#define cs_STERO_SOUTH       2
#define cs_STERO_EQUATOR     4
#define cs_STERO_OBLIQUE     5

struct cs_Pstro_
{
	double org_lng;			/* Origin longitude, in radians. */
	double org_lat;			/* Origin latitude, in radians. */
	double k;				/* The mapping scale factor, includes
							   map scale and units of measure. */
	double x_off;			/* The coordinate system X value for the
							   origin point, also known as the
							   False Easting. */
	double y_off;			/* The coordinate system Y value for the
							   origin point, also known as the
							   False Northing. */
	double ka;				/* The scaled equitorial radius.  Used
							   as the radius of the sphere, i.e.
							   R in the reference. */
	double one_mm;			/* Actually, one tenth of a millimeter
							   in terms of the cartesian system
							   units.  Used to trap zero coordinate
							   values. */
	double ecent;			/* Eccentricity of the ellipsoid being
							   used. */
	double e_rad;			/* Equatorial radius of the ellipsoid. */
	double e_sq;			/* Eccentricity squared. */
	double e_o_2;			/* Eccentricity divided by 2.0 */
	double e_term;			/* Complex term in eccentricity. */
	double two_k0;			/* Two times the scale reduction factor. */
	double two_ka;			/* Two times the scaled radius. */

	double cos_org_lat;		/* Cosine of the origin latitude. */
	double sin_org_lat;		/* Sine of the origin latitude. */

	struct cs_ChicofI_ chicofI;
							/* Coefficients of the isometric
							   latitude series. */

	short aspect;			/* Set to indicate the aspect:
								cs_STERO_NORTH,
								cs_STERO_SOUTH */
	short quad;				/* Non-zero specifies a cartesian system
							   other than the standard right handed
							   system. */
};

struct cs_Ostro_
{
	double org_lng;			/* Origin longitude, in radians. */
	double org_lat;			/* Origin latitude, in radians. */
	double k;				/* The mapping scale factor, includes
							   map scale and units of measure. */
	double x_off;			/* The coordinate system X value for the
							   origin point, also known as the
							   False Easting. */
	double y_off;			/* The coordinate system Y value for the
							   origin point, also known as the
							   False Northing. */
	double ka;				/* The scaled equitorial radius.  Used
							   as the radius of the sphere, i.e.
							   R in the reference. */
	double one_mm;			/* Actually, one tenth of a millimeter
							   in terms of the cartesian system
							   units.  Used to trap zero coordinate
							   values. */
	double ecent;			/* Eccentricity of the ellipsoid being
							   used. */
	double e_rad;			/* Equatorial radius of the ellipsoid. */
	double e_sq;			/* Eccentricity squared. */
	double e_o_2;			/* Eccentricity divided by 2.0 */
	double one_esq;			/* One minus eccentricity squared. */
	double two_k0;			/* Two times the scale reduction factor. */
	double two_ka;			/* Two times the scaled radius. */

	double cos_org_lat;		/* Cosine of the origin latitude. */
	double sin_org_lat;		/* Sine of the origin latitude. */

	double nu0;				/* Radius of curvature in the prime
							   vertical section at the origin. */
	double rho0;			/* Radius of curvature in the meridian
							   plane at the origin. */
	double nn;				/* Conformal longitude conversion factor. */
	double cc;				/* Integration constant for conformal
							   latitude. */
	double gg;				/* term used in the inverse */
	double hh;				/* term used in the inverse */

	double chi0;			/* Conformal latitude of the origin. */
	double sin_chi0;		/* Sine of the isometric latitude of the
							   origin latitude. */
	double cos_chi0;		/* Cosine of the isometric latitude of the
							   origin latitude. */
	double cnvrg;			/* Epsilon used to terminate the international
							   variation inverse iteration. */

	short aspect;			/* Set to indicate the aspect:
								cs_STERO_EQUATOR,
								cs_STERO_OBLIQUE. */
	short quad;				/* Non-zero specifies a cartesian system
							   other than the standard right handed
							   system. */
};

struct cs_Sstro_
{
	double org_lng;			/* Origin longitude, in radians. */
	double org_lat;			/* Origin latitude, in radians. */
	double k;				/* The mapping scale factor, includes
							   map scale and units of measure. */
	double x_off;			/* The coordinate system X value for the
							   origin point, also known as the
							   False Easting. */
	double y_off;			/* The coordinate system Y value for the
							   origin point, also known as the
							   False Northing. */
	double Az;				/* The azimuth, east of north, of the
							   rotation implied by the northing
							   point, or specified by the user,
							   in radians. */
	double ka;				/* The scaled equitorial radius.  Used
							   as the radius of the sphere, i.e.
							   R in the reference. */
	double one_mm;			/* Actually, one tenth of a millimeter
							   in terms of the cartesian system
							   units.  Used to trap zero coordinate
							   values. */
	double ecent;			/* Eccentricity of the ellipsoid being
							   used. */
	double e_rad;			/* Equatorial radius of the ellipsoid. */
	double e_sq;			/* Eccentricity squared. */
	double e_o_2;			/* Eccentricity divided by 2.0 */
	double two_k0;			/* Two times the scale reduction factor. */
	double two_ka;			/* Two times the scaled radius. */

	double cos_org_lat;		/* Cosine of the origin latitude. */
	double sin_org_lat;		/* Sine of the origin latitude. */
	double cos_Az;			/* Cosine of the northing azimuth. */
	double sin_Az;			/* Sine of the northing azimuth. */

	double sin_chi1;		/* Sine of the isometric form of the
							   origin latitude. */
	double cos_chi1;		/* Cosine of the isometric form of the
							   origin latitude. */
	double m1;				/* The m factor for the origin latitude. */
	double two_kam1;		/* Two times the scaled radius, times
							   the m1 factor. */
	struct cs_ChicofI_ chicofI;
							/* Coefficients of the isometric
							   latitude series. */

	short aspect;			/* Set to indicate the aspect:
								cs_STERO_EQUATOR,
								cs_STERO_OBLIQUE. */
	short quad;				/* Non-zero specifies a cartesian system
							   other than the standard right handed
							   system. */
};

/*
	The following structure supports the Modified
	Sterographic Projection.
*/

struct cs_Mstro_
{
	double org_lng;			/* Origin longitude, in radians. */
	double org_lat;			/* Origin latitude, in radians. */
	double k;				/* The mapping scale factor, includes
							   map scale and units of measure. */
	double x_off;			/* The coordinate system X value for the
							   origin point, also known as the
							   False Easting. */
	double y_off;			/* The coordinate system Y value for the
							   origin point, also known as the
							   False Northing. */
	double ka;				/* The scaled equitorial radius.  Used
							   as the radius of the sphere, i.e.
							   R in the reference. */
	double one_mm;			/* Actually, one tenth of a millimeter
							   in terms of the cartesian system
							   units.  Used to trap zero coordinate
							   values. */
	double ecent;			/* Eccentricity of the ellipsoid being
							   used. */
	double e_rad;			/* Equatorial radius of the ellipsoid. */
	double e_sq;			/* Eccentricity squared. */
	double e_o_2;			/* Eccentricity divided by 2.0 */

	double cos_org_lat;		/* Cosine of the origin latitude. */
	double sin_org_lat;		/* Sine of the origin latitude. */
	double sin_chi1;		/* Sine of the isometric form of the
							   origin latitude. */
	double cos_chi1;		/* Cosine of the isometric form of the
							   origin latitude. */
	struct cs_Cmplx_ ABary [13];
							/* Coefficients for the complex series
							   expansion, up to 12th order.
							   NOTE: need one more than order,
							   the 0 element must be set to zero.
							   Don't know of any projection
							   which is greater than 10, so
							   we have some room for the
							   unexpected. */
	struct cs_ChicofI_ chicofI;
							/* Coefficients of the isometric
							   latitude series. */
	short order;			/* The order of the complex series used
							   for this projection. Usually
							   computed by the setup function. */
	short quad;				/* Non-zero specifies a cartesian system
							   other than the standard right handed
							   system. */
};

/*
	The following structure supports the New Zealand
	National Grid.
*/

struct cs_Nzlnd_
{
	double org_lng;			/* Origin longitude, in degrees. */
	double org_lat;			/* Origin latitude, in degrees. */
	double k;				/* The mapping scale factor, includes
							   map scale and units of measure. */
	double x_off;			/* The coordinate system X value for the
							   origin point, also known as the
							   False Easting. */
	double y_off;			/* The coordinate system Y value for the
							   origin point, also known as the
							   False Northing. */
	double ka;				/* The scaled equitorial radius.  Used
							   as the radius of the sphere, i.e.
							   R in the reference. */
	double one_mm;			/* Actually, one tenth of a millimeter
							   in terms of the cartesian system
							   units. */
	double ecent;			/* Eccentricity of the ellipsoid being
							   used. */
	double e_rad;			/* Equatorial radius of the ellipsoid. */
	double e_sq;			/* Eccentricity squared. */
	double lat_kk;			/* The 3600 * 1.0E-05 multiplier used
							   to normalize delta latitude for the
							   series expansion. */

	double A_ary [10];		/* Coefficients for the series which
							   computes delta psi. */
	struct cs_Cmplx_ B_ary [7];
							/* Coefficients for the complex series
							   used to compute X and Y values
							   (normalized).  Note, to use our
							   Horner implementation of the
							   series calculation, this array
							   is dimensioned one greater than
							   necessary, and the setup function
							   forces the first element to
							   complex zero. */
	struct cs_Cmplx_ C_ary [7];
							/* Inverse of B_ary. */
	double D_ary [10];		/* Inverse of A_ary. */
	short quad;				/* Non-zero specifies a cartesian system
							   other than the standard right handed
							   system. */
};

/*
	The following structure supports the calculations associated
	with the Sinusoidal projection.
*/

struct cs_Sinus_
{
	double cent_lng;		/* Longitude of the central meridian.
							   It represents the longitude origin.
							   The value is in radians. */
	double k;				/* The map scale factor, also includes
							   any units change. */
	double ka;				/* Scaled equatorial radius.  Includes
							   map scale, but there is no scale
							   reduction. */
	double x_off;			/* The offset to be added to the
							   calculated x values.  This can
							   be used to cause points to the
							   west of the central meridian to
							   have positive values.  Note, the
							   offset is applied after scaling. */
	double y_off;			/* The y offset, applied after
							   scaling. */
	double e_rad;			/* Equatorial radius of the sphere or
							   ellipsoid. */
	double ecent;			/* Ellipsoid eccentricity. */
	double e_sq;			/* Ecentricity squared. */
	double max_xx;			/* Maximum magnitude of X for the current
							   coordinate system. */
	double max_yy;			/* Maximum magnitude of Y for the current
							   coordinate system. */
	double one_mm;			/* Equivalent of one millimeter in the
							   system unit. */

	struct cs_MmcofF_ mmcofF;
							/* Series coeficients for calculating
							   the M value. */
	struct cs_MmcofI_ mmcofI;
							/* Series coeficients for calculating
							   the inverse of M. */
	struct cs_Zone_ zones [8];
	short zone_cnt;
	short quad;			/* Non-zero specifies a cartesian system
						   other than the standard right handed
						   system. */
};

/*
	The following supports the Orthographic projection.
*/

struct cs_Ortho_
{
	double org_lng;			/* Origin longitude, in radians. */
	double org_lat;			/* Origin latitude, in radians. */
	double k;				/* The mapping scale factor, includes
							   map scale and units of measure. */
	double x_off;			/* The coordinate system X value for the
							   origin point, also known as the
							   False Easting. */
	double y_off;			/* The coordinate system Y value for the
							   origin point, also known as the
							   False Northing. */
	double ka;				/* The scaled equitorial radius.  Used
							   as the radius of the sphere, i.e.
							   R in the reference. */
	double one_mm;			/* Actually, one tenth of a millimeter
							   in terms of the cartesian system
							   units.  Used to trap zero coordinate
							   values. */
	double e_rad;			/* Equatorial radius of the ellipsoid. */
	double cos_org_lat;		/* Cosine of the origin latitude. */
	double sin_org_lat;		/* Sine of the origin latitude. */
	short quad;				/* Non-zero specifies a cartesian system
							   other than the standard right handed
							   system. */
};

/*
	The following supports the Gnomonic projection.
*/

struct cs_Gnomc_
{
	double org_lng;			/* Origin longitude, in radians. */
	double org_lat;			/* Origin latitude, in radians. */
	double k;				/* The mapping scale factor, includes
							   map scale and units of measure. */
	double x_off;			/* The coordinate system X value for the
							   origin point, also known as the
							   False Easting. */
	double y_off;			/* The coordinate system Y value for the
							   origin point, also known as the
							   False Northing. */
	double ka;				/* The scaled equitorial radius.  Used
							   as the radius of the sphere, i.e.
							   R in the reference. */
	double one_mm;			/* Actually, one tenth of a millimeter
							   in terms of the cartesian system
							   units.  Used to trap zero coordinate
							   values. */
	double e_rad;			/* Equatorial radius of the ellipsoid. */
	double cos_org_lat;		/* Cosine of the origin latitude. */
	double sin_org_lat;		/* Sine of the origin latitude. */
	double cos_limit;		/* Cosine of the largest angular
							   distance from the origin.  That is,
							   lat/longs where the cosine of the
							   angular distance is less than this
							   value are treated specially. */
	double rho_limit;		/* Actual distance limit from the
							   origin.  I.e. X's and Y's which are
							   further away from the origin than
							   this value are treated specially. */
	short quad;				/* Non-zero specifies a cartesian system
							   other than the standard right handed
							   system. */
};

/*
	The following supports the Equidistant Cylindrical projection.
*/

struct cs_Edcyl_
{
	double org_lng;			/* Origin longitude, in radians. */
	double org_lat;			/* Origin latitude, in radians. */
	double ref_lat;			/* Reference latitude, in radians.  Can
							   be either positive or negative. */
	double k;				/* The mapping scale factor, includes
							   map scale and units of measure. */
	double x_off;			/* The coordinate system X value for the
							   origin point, also known as the
							   False Easting. */
	double y_off;			/* The coordinate system Y value for the
							   origin point, also known as the
							   False Northing. */
	double ka;				/* The scaled equitorial radius.  Used
							   as the radius of the sphere, i.e.
							   R in the reference. */
	double one_mm;			/* Actually, one tenth of a millimeter
							   in terms of the cartesian system
							   units.  Used to trap zero coordinate
							   values. */
	double e_rad;			/* Equatorial radius of the ellipsoid. */
	double ecent;			/* The ecentricity of the datum in
							   use. */
	double e_sq;			/* The square of the ecentricity of the
							   datum in use. */

	double nu0;				/* radius of curvatire of the first vertical
							   section of the ellipsoid at the reference
							   latitude. */
	double cos_ref_lat;		/* Cosine of the reference latitude. */
	double Rcos_ref_lat;	/* Cosine of the reference latitude times
							   the scaled equatorial radius. */
	double M0;				/* Meridonal distance from the equator
							   to the origin latitude. */
	struct cs_MmcofF_ mmcofF;
							/* The coefficients necessary to compute
							   meridonal distance from the equator. */
	struct cs_MmcofI_ mmcofI;
							/* Coeffieients necessary to compute the
							   foot print latitude. */
	short quad;				/* Non-zero specifies a cartesian system
							   other than the standard right handed
							   system. */
};

/*
	The following supports the Van Der Grinten projection.
*/

struct cs_Vdgrn_
{
	double org_lng;			/* Origin longitude, in radians. */
	double k;				/* The mapping scale factor, includes
							   map scale and units of measure. */
	double x_off;			/* The coordinate system X value for the
							   origin point, also known as the
							   False Easting. */
	double y_off;			/* The coordinate system Y value for the
							   origin point, also known as the
							   False Northing. */
	double ka;				/* The scaled equitorial radius.  Used
							   as the radius of the sphere, i.e.
							   R in the reference. */
	double one_mm;			/* Actually, one tenth of a millimeter
							   in terms of the cartesian system
							   units.  Used to trap zero coordinate
							   values. */
	double e_rad;			/* Equatorial radius of the ellipsoid. */

	double piR;				/* PI times ka. */
	double two_ovr_pi;		/* 2.0 / PI */
	short quad;				/* Non-zero specifies a cartesian system
							   other than the standard right handed
							   system. */
};

/*
	The following supports the Cassini projection.
*/

struct cs_Csini_
{
	double cent_lng;		/* Central longitude of the system, in
							   radians, negative is west longitude. */
	double org_lat;			/* Origin latitude of the system, usually
							   zero, in radians, negative is south
							   latitude. */
	double k;				/* Scale factor on the central meridian.
							   This includes the map scale factor and
							   the units conversion. There is no
							   scale reduction factor for this
							   projection. */
	double x_off;			/* The false easting, in coordinate
							   system units. */
	double y_off;			/* The false northing, in coordinate
							   system units. */
	double ecent;
	double e_sq;
	double e_rad;
	double ka;				/* Scaled radius.  Radius of the sphere
							   for a spherical system, equatorial
							   radius for an elliptical system. */
	double max_xx;			/* Maximum magnitude of X coordinates,
							   sans false origin. */
	double min_yy;			/* Minimum magnitude of Y coordinates,
							   sans false origin. */
	double max_yy;			/* Maximum magnitude of Y coordinates,
							   sans false origin. */
	double C_term;			/* e_sq / (1.0 - e_sq) */
	double R_term;			/* ka / (1.0 - e_sq) */
	double s_term;			/* 2.0 * (ka * ka) * (1.0 - e_sq) */

	double M0;				/* The meridonal distance from the equator
							   to the origin latitude. */
	struct cs_MmcofF_ mmcofF;
							/* The coefficients necessary to compute
							   the distance from the equator to a
							   parallel on the ellipsoid along a
							   meridian, i.e. meridonal distance. */
	struct cs_MmcofI_ mmcofI;
							/* The coefficients of the series expansion
							   for converting the recifying latitude,
							   Mu, to the ellipsoidal geodetic
							   latitude.  See MmuIsu and MmuIcal. */
	short quad;				/* Non-zero specifies a cartesian system
							   other than the standard right handed
							   system. */
};

/*
	The following structure supports the Robinson projection.
*/

struct cs_Robin_
{
	double org_lng;			/* Longitude of the central meridian.
							   It represents the longitude origin.
							   The value is in radians. */
	double k;				/* The map scale factor, also includes
							   any units change. */
	double e_rad;			/* Equatorial radius of the sphere or
							   ellipsoid. */
	double ka;				/* Scaled equatorial radius.  Includes
							   map scale, but there is no scale
							   reduction. */
	double kax;				/* Adjusted radius used in the X calculations. */
	double kay;				/* Adjusted radius used in the Y calculations. */
	double inv_kax;			/* Inverted agument used in the inverse
							   Robinson X calculation. */
	double inv_kay;			/* Inverted argument used in the inverse
							   Robinson Y calculation. */
	double x_off;			/* The offset to be added to the
							   calculated x values.  This can
							   be used to cause points to the
							   west of the central meridian to
							   have positive values.  Note, the
							   offset is applied after scaling. */
	double y_off;			/* The y offset, applied after
							   scaling. */
	double xx_max;			/* Maximum magnitude of the X value for
							   this coordinate system, sans false
							   easting. */
	double yy_max;			/* Maximum magnitude of the Y value for
							   this coordinate system, sans false
							   northing. */
	double one_cm;			/* The equivalent of one centiimeter in
							   the units of the coordinate system.
							   Close enough for a projection which
							   is only supported in the sphereical
							   form. */
	short quad;				/* Non-zero specifies a cartesian system
							   other than the standard right handed
							   system. */
};

/*
	The following structure supports the Bonne projection.  Note,
	if the origin latitude is either pole, we actually have a
	limiting form known as the Werner projection.  If the origin
	is the equator, the limiting form is the Sinusoidal.
*/

#define cs_BONNE_NONE    0
#define cs_BONNE_NRML    1
#define cs_BONNE_WERNER  2
#define cs_BONNE_SINUS   3

struct cs_Bonne_
{
	double org_lng;			/* Origin longitude in radians. */
	double org_lat;			/* Origin latitude in radians. */
	double x_off;			/* The offset to be added to the
							   calculated x values.  This can
							   be used to cause points to the
							   west of the central meridian to
							   have positive values.  Note, the
							   offset is applied after scaling. */
	double y_off;			/* The y offset, applied after
							   scaling. */
			/* Datum values captured from the
			   active dataum, repeated here
			   to prevent recalculation. */
	double ecent;			/* Ellipsoid eccentricity. */
	double e_sq;			/* Ellipsoid eccentricity squared. */
	double e_rad;			/* Usually referred to as "a", the
							   equatorial radius of the ellipsoid. */
	double k;				/* The scale of the coordinate system,
							   includes unit scaling (from meters
							   to the coordinate system unit) and
							   the map scale factor. */
	double ka;				/* Scaled equitorial radius of the earth,
							   in the units of the coordinate system. */
	double small_xy;		/* Equivalent of 0.001 seconds of arc at
							   the north pole, in cartesian system
							   units. */
	double max_rho;			/* The absolute value of the maximum rho
							   value supported by the coordinate
							   system. */
	double sin_org_lat;		/* Sine of the origin latitude. */
	double m1;				/* ka times m1 is the length of a radian
							   of longitude at the specified latitude.
							   In this case, at the origin latitude. */
	double M1;				/* Distance from the equator to the
							   the origin latitude in scaled coordinate
							   system units.  This will be negative
							   if the origin latitude is in the
							   southern hemisphere. */

	double cot_lat1;		/* Cotangent of the origin latitude. */
	double Rcot_lat1;		/* Scaled equatorial radius times the
							   cotangent of the origin latitude. */
	double cot_lat1_lat1;	/* Cotangent of the origin latitude plus
							   the origin latitude. */

	double kam1term;		/* ka * m1 / sin_org_lat */
	double kam1Mterm;		/* ka * m1 / sin_org_lat + M1 */
	struct cs_MmcofF_ mmcofF;
	struct cs_MmcofI_ mmcofI;
							/* The above two structures carry the
							   coefficients of the power series for
							   computing M. */

	short mode;				/* Use basically as an enum to distinguish
							   between the normal and two limiting
							   cases.  We have:
							   cs_BONNE_NRML   ==> the normal case
							   cs_BONNE_WERNER ==> org_lat == a pole
							   cs_BONNE_SINUS  ==> org_lat == equator */
	short south;			/* Set to TRUE if the origin latitude
							   is less than zero by more than a
							   trivial amount. */
	short quad;				/* Non-zero specifies a cartesian system
							   other than the standard right handed
							   system. */
};

/*
	The following structures are used for the Ekert 4 and 6 projections.
	Ellipsoidal versions of these projections don't exist.
*/

struct cs_Ekrt4_
{
	double org_lng;			/* Origin longitude in radians. */
	double x_off;			/* The offset to be added to the
							   calculated x values.  This can
							   be used to cause points to the
							   west of the central meridian to
							   have positive values.  Note, the
							   offset is applied after scaling. */
	double y_off;			/* The y offset, applied after
							   scaling. */
				/* Datum values captured from the
				   active dataum, repeated here
				   to prevent recalculation. */
	double e_rad;			/* Usually referred to as "a", the
							   equatorial radius of the ellipsoid. */
	double k;				/* The scale of the coordinate system,
							   includes unit scaling (from meters
							   to the coordinate system unit) and
							   the map scale factor. */
	double ka;				/* Scaled equitorial radius of the earth,
							   in the units of the coordinate system. */
	double one_cm;			/* Equivalent of one centimeter on the
							   ellipsoid in the units of the scaled
							   coordinate system. */
	double cnvrg_val;		/* Small value used to determine when the
							   theta Newton/Rhapson has converged. */
	double max_xx;			/* Magnitude of the largest valid X
							   value. */
	double max_yy;			/* Magnitude of the largest valid Y
							   value. */
	double kF_theta;		/* Constant used in the forward
							   calculation of theta. */
	double kF_x;			/* Constant used in the forward
							   calculation of X. */
	double kF_y;			/* Constant used in the forward
							   calculation of Y. */
	double kI_theta;		/* Constant used in the inverse
							   calculation of theta. */
	double kI_lat;			/* Constant used in the inverse
							   calculation of latitude. */
	double kI_lng;			/* Constant used in the inverse
							   calculation of longitude. */
	short quad;				/* Non-zero specifies a cartesian system
							   other than the standard right handed
							   system. */
};

struct cs_Ekrt6_
{
	double org_lng;			/* Origin longitude in radians. */
	double x_off;			/* The offset to be added to the
							   calculated x values.  This can
							   be used to cause points to the
							   west of the central meridian to
							   have positive values.  Note, the
							   offset is applied after scaling. */
	double y_off;			/* The y offset, applied after
							   scaling. */
				/* Datum values captured from the
				   active dataum, repeated here
				   to prevent recalculation. */
	double e_rad;			/* Usually referred to as "a", the
							   equatorial radius of the ellipsoid. */
	double k;				/* The scale of the coordinate system,
							   includes unit scaling (from meters
							   to the coordinate system unit) and
							   the map scale factor. */
	double ka;				/* Scaled equitorial radius of the earth,
							   in the units of the coordinate system. */
	double one_cm;			/* Equivalent of one centimeter on the
							   ellipsoid in the units of the scaled
							   coordinate system. */
	double cnvrg_val;		/* Small value used to determine when the
							   theta Newton/Rhapson has converged. */
	double max_xx;			/* Maximum magnitude of legitimate X
							   coordinates. */
	double max_yy;			/* Maximum magnitude of legitimate Y
							   coordinates. */
	double kF_theta;		/* Constant used in the forward
							   calculation of theta. */
	double kF_x;			/* Constant used in the forward
							   calculation of X. */
	double kF_y;			/* Constant used in the forward
							   calculation of Y. */
	double kI_theta;		/* Constant used in the inverse
							   calulation of theta. */
	double kI_lat;			/* Constant used in the inverse
							   calculation of latitude. */
	double kI_lng;			/* Constant used in the inverse
							   calculation of longitude. */
	short quad;				/* Non-zero specifies a cartesian system
							   other than the standard right handed
							   system. */
};

/*
	The following structure is used for the Mollweide Projection.
*/

struct cs_Molwd_
{
	double org_lng;			/* Origin longitude in radians. */
	double x_off;			/* The offset to be added to the
							   calculated x values.  This can
							   be used to cause points to the
							   west of the central meridian to
							   have positive values.  Note, the
							   offset is applied after scaling. */
	double y_off;			/* The y offset, applied after
							   scaling. */
				/* Datum values captured from the
				   active dataum, repeated here
				   to prevent recalculation. */
	double e_rad;			/* Usually referred to as "a", the
							   equatorial radius of the ellipsoid. */
	double k;				/* The scale of the coordinate system,
							   includes unit scaling (from meters
							   to the coordinate system unit) and
							   the map scale factor. */
	double ka;				/* Scaled equitorial radius of the earth,
							   in the units of the coordinate system. */
	double one_cm;			/* Equivalent of one centimeter on the
							   ellipsoid in the units of the scaled
							   coordinate system. */
	double cnvrg_val;		/* Small value used to determine when the
							   theta Newton/Rhapson has converged. */
	double max_xx;			/* Maximum magnitude of legitimate X's. */
	double max_yy;			/* Maximum magnitude of legitimate Y's. */
	double kF_x;			/* Constant used in the forward
							   calculation of X. */
	double kF_y;			/* Constant used in the forward
							   calculation of Y. */
	double kI_theta;		/* Constant used in the inverse
							   calulation of theta. */
	double kI_lng;			/* Constant used in the inverse
							   calculation of longitude. */
	struct cs_Zone_ zones [8];
							/* Definition of any interupted zones
							   if zone_cnt is not zero. */
	short zone_cnt;
	short quad;				/* Non-zero specifies a cartesian system
							   other than the standard right handed
							   system. */
};

/*
	The following structure is used for the Goode
	Homolosine projection.  Actually a mixture of the
	Mollweide and the Sinusoidal.  Almost always used
	with six or more zones.
*/

struct cs_Hmlsn_
{
	double org_lng;			/* Origin longitude in radians. */
	double x_off;			/* The offset to be added to the
							   calculated x values.  This can
							   be used to cause points to the
							   west of the central meridian to
							   have positive values.  Note, the
							   offset is applied after scaling. */
	double y_off;			/* The y offset, applied after
							   scaling. */
				/* Datum values captured from the
				   active dataum, repeated here
				   to prevent recalculation. */
	double e_rad;			/* Usually referred to as "a", the
							   equatorial radius of the ellipsoid. */
	double k;				/* The scale of the coordinate system,
							   includes unit scaling (from meters
							   to the coordinate system unit) and
							   the map scale factor. */
	double ka;				/* Scaled equitorial radius of the earth,
							   in the units of the coordinate system. */
	double one_cm;			/* Equivalent of one centimeter on the
							   ellipsoid in the units of the scaled
							   coordinate system. */
	double cnvrg_val;		/* Small value used to determine when the
							   theta Newton/Rhapson has converged. */

	double sw_lat;			/* The latitude at which the switch from
							   Sinusoidal to Mollweide is made. */
	double sw_yy;			/* Similar to the above, but in terms of
							   cartesian Y coordinate for the inverse. */
	double max_xx;			/* Maximum magnitude of legitimate X's. */
	double max_yy;			/* Maximum magnitude of legitimate Y's. */

			/* Mollweide stuff. */

	double kF_x;			/* Constant used in the forward
							   calculation of Mollweide X. */
	double kF_y;			/* Constant used in the forward
							   calculation of Mollweide Y. */
	double kI_theta;		/* Constant used in the inverse
							   calulation of Mollweide theta. */
	double kI_lng;			/* Constant used in the inverse
							   calculation of Mollweide longitude. */
	double yy_crct;			/* Y correction factor to bring
							   Molleweide portion down to
							   the Sinusoidal portion. */

			/* Spherical Sinusoidal is so simple, we
			   don't really need anything here. */

			/* Zone stuff. */

	struct cs_Zone_ zones [8];
							/* Definition of any interupted zones
							   if zone_cnt is not zero. */
	short zone_cnt;
	short quad;				/* Non-zero specifies a cartesian system
							   other than the standard right handed
							   system. */
};

/*
	The following structure is ued for the Normal Aspect
	of the Equal Area Cylindrical projection.  The Transverse
	Aspect (and sometime even the oblique aspect) are treated
	as separate projections since the parameters are significantly
	different.

	Note the naming convention here is Normal Authalic CYLindrical.
	Authalic essentially is synonomous with equal area.  The other
	aspects will be named TACYL (Transverse Authalic CYLindrical
	and OACYL (Oblique Authalic CYLindrical).
*/

struct cs_Nacyl_
{
	double org_lng;			/* Origin longitude in radians. */
	double std_lat;			/* Standard latitude in radians.  This
							   has an effect in this projection
							   similar to the scale reduction factor
							   of the Transverse aspect. AKA
							   standard latiude.  This is the
							   latitude at which the K scale
							   factor (scale along a parallel) is
							   1.0. */
	double x_off;			/* The offset to be added to the
							   calculated x values.  This can
							   be used to cause points to the
							   west of the central meridian to
							   have positive values.  Note, the
							   offset is applied after scaling. */
	double y_off;			/* The y offset, applied after
							   scaling. */
				/* Datum values captured from the
				   active dataum, repeated here
				   to prevent recalculation. */
	double e_rad;			/* Usually referred to as "a", the
							   equatorial radius of the ellipsoid. */
	double k;				/* The scale of the coordinate system,
							   includes unit scaling (from meters
							   to the coordinate system unit) and
							   the map scale factor. */
	double ka;				/* Scaled equitorial radius of the earth,
							   in the units of the coordinate system. */
	double ecent;			/* Ellipsoid eccentricity. */
	double e_sq;			/* Ellipsoid eccentricity squared. */
	double one_m_esq;		/* One minus the eccentricity squared. */
	double one_mm;			/* Equivalent of one millimeter on the
							   ellipsoid in the units of the scaled
							   coordinate system. */
	double max_xx;			/* Max magnitude of legitimate X's. */
	double max_yy;			/* Max magnitude of legitimate Y's. */

	double cos_std_lat;		/* Cosine of the standard latitude. */
	double ka_cosst;		/* Scaled equatorial radius times the
							   cosine of the standard latitude. */
	double ka_o_cosst;		/* Scaled equatorial radius divided by
							   (over) the cosine of the standard
							   latitude. */
	double cosst_o_ka;		/* Cosine of the standard latitude
							   divided by (over) scaled equatorial
							   radius. */
	double k0;				/* Ellipsoidal version of the cosine of
							   the standard latitude. */
	double kak0;
	double ka_o_2k0;
	double one_o_kak0;		/* The inverse of k0 times ka, for the
							   ellipsoidal inverse of longitude. */
	double one_o_2e;		/* One divided by (over) two times the
							   eccentricity. */
	double beta_term;		/* This term includes all factors
							   necessary to compute the sine of
							   Beta, given Y, by multiplication.
							   Using Synder's notation: 2*k0/a*qp */
	struct cs_BtcofI_ btcoefI;
							/* Coefficients for the inverse
							   calculation (i.e. power series) of
							   geographic latitude from the authalic
							   latitude Beta. */
	short quad;				/* Non-zero specifies a cartesian system
							   other than the standard right handed
							   system. */
};

/*
	Transverse aspect of the Authalic (equal area) CYLindircal
	projeciton.
*/

struct cs_Tacyl_
{
	double org_lng;			/* Origin longitude in radians. */
	double org_lat;			/* Origin lattiude in radians. */
	double h0;				/* This is the actual scale reduction
							   factor.  We need this unencumbered
							   by other scale factors because of the
							   authalic nature of the projection. */
	double x_off;			/* The offset to be added to the
							   calculated x values.  This can
							   be used to cause points to the
							   west of the central meridian to
							   have positive values.  Note, the
							   offset is applied after scaling. */
	double y_off;			/* The y offset, applied after
							   scaling. */
				/* Datum values captured from the
				   active dataum, repeated here
				   to prevent recalculation. */
	double e_rad;			/* Usually referred to as "a" or "R", the
							   equatorial radius of the ellipsoid. */
	double k;				/* The scale of the coordinate system,
							   includes unit scaling (from meters
							   to the coordinate system unit) and
							   the map scale factor. */
	double ka;				/* Scaled equitorial radius of the earth,
							   in the units of the coordinate system.
							   Note, for this projection this does
							   not include the scale reduction factor. */
	double ecent;			/* Ellipsoid eccentricity. */
	double e_sq;			/* Ellipsoid eccentricity squared. */
	double one_m_esq;		/* One minus the eccentricity squared. */
	double max_xx;			/* Maximum magnitude of the X coordinate,
							   sans false origin. */
	double max_yy;			/* Maximum magnitude of the X coordinate,
							   sans false origin. */

	double one_o_2e;		/* One divided by (over) two times the
							   eccentricity. */
	double kah0;			/* Scaled equatorial radius times the
							   scale reduction factor. */
	double ka_o_h0;			/* Scaled equatorial radius divided by the
							   scale reduction factor (ka / h0). */
	double h0_o_ka;			/* Reciprocal of ka_o_h0. */
	double aterm;			/* aterm */
	double qp;				/* The value of q evaluated at the north
							   pole. */
	double M0;				/* Meridonal distance along the central
							   meridian from the equator to the
							   origin latitude. */

	struct cs_BtcofI_ btcofI;
							/* Coefficients for the inverse
							   calculation (i.e. power series) of
							   geographic latitude from the authalic
							   latitude Beta. */
	struct cs_MmcofF_ mmcofF;
							/* The coefficients of the power series for
							   computing M. */
	struct cs_MmcofI_ mmcofI;
							/* The coefficients of the power series for
							   computing M. */
	short quad;				/* Non-zero specifies a cartesian system
							   other than the standard right handed
							   system. */
};

/*
	The following structure is used for the Bipolar Oblique
	Conformal Conic.
*/

struct cs_Bpcnc_
{
			/* All angular values are in radians. */

	double lng_A;			/* Longitude of pole A */
	double lat_A;			/* Latitude of pole A */
	double lng_B;			/* Longitude of pole B */
	double lat_B;			/* Latitude of pole B */
	double pole_dd;			/* Angular distance between the
							   two poles. */
	double sp_1;			/* Arc distance from either pole
							   to the first standard parallel. */
	double sp_2;			/* Arc distance from either pole
							   to the second standard parallel. */
	double x_off;			/* False easting. */
	double y_off;			/* False northing. */

	double ka;
	double one_cm;			/* Equivalent of one centimeter in the
							   system unit. */
	double sin_lat_A;
	double cos_lat_A;
	double sin_lat_B;
	double cos_lat_B;
	double nn;				/* The cone constant, same for
							   both cones. */
	double Az_AB;			/* Azimuth from pole A to pole B. */
	double Az_BA;			/* Azimuth from pole B to pole A. */
	double Az_BAc;			/* Complement of Az_BAc. Not the same
							   as Az_AB. This is spherical trig. */
	double k0;				/* Calculated scale reduction factor. */
	double F0;
	double F;
	double T;
	double rho_c;			/* Radius of the center point from
							   either pole. */
	double z_c;				/* Angular distance from the center
							   point to either pole. */
	double lng_c;			/* Longitude of the central point. */
	double lat_c;			/* Latitude of the central point. */
	double Az_c;			/* Azimuth of the axis at the central
							   point. */
	double sin_Az_c;
	double cos_Az_c;
	double max_xx;			/* Maximum magnitude of the skew rectangle
							   X coordinate, based on a maximum angular
							   from the central great circle of Pi. */
	double Az_max;			/* The maximum polar angle for the conics.
							   I.e. an XY pair which produces an
							   azimuth relative to the appropriate pole
							   greater than this is in the back side
							   cut-out of the conic section. */
	short quad;				/* Non-zero specifies a cartesian system
							   other than the standard right handed
							   system. */
};

/*
	The Swiss projection. A double projection where the ellipsoid
	is projected onto a conformal sphere, ala Gauss, where integration
	constants are choosen to produce grid scale of 1 at the
	central point. The pole of the conformal sphere is then tilted
	such that the equator of the tilted system passes through the
	central point. The tilted sphere is then projected on to a
	cylinder ala Mercator.

	*******************************************************************
	*******************************************************************
	**                                                               **
	**  The "Swiss" projection has now been generalized to be a      **
	**  Conformal Oblique Cylindrial Projection.  This enables the   **
	**  implementation here to function as a Hungarian EOV           **
	**  as well.  This was accomplished by adding a scale reduction  **
	**  factor as well as a "Normall Parallel".                      **
	**                                                               **
	*******************************************************************
	*******************************************************************
*/

#if defined(B0)
#undef B0
#endif

struct cs_Swiss_
{
	double L0;				/* Longitude of central point, in radians */
	double B0;				/* Latitude of central point, in radians */
	double Bn;				/* Latitude of the Normal Parallel, in radians */
	double k;				/* The scale factor in use.  This includes
							   the map scale and the units conversion
							   from the meters in which ellipsoids
							   are defined to whatever the
							   coordinate system units are. */
	double x_off;			/* The false easting in coordinate system
							   units. */
	double y_off;			/* The false northing in coordinate system
							   units. */
	double ecent;			/* The ecentricity of the ellipsoid in use. */
	double e_sq;			/* The square of the ecentricity of the
							   ellipsoid in use. */
	double e_ovr_2;			/* One half of the ecentricity. */

		/* The following are parameters which can be computed once
		   given the above information. Nomenclature follows that
		   used in the USGS publication. A digit following a name
		   should (usually) be read as a subscript. */

	double kR;				/* Equatorial radius scaled for map scale
							   and units. */
	double R;				/* Radius of the intermediary sphere. */
	double M0;				/* Radius of curvature of a meridian at the
							   central point. In units of the cartesian
							   system. */
	double N0;				/* Radius of curvature of the first prime
							   vertical at the central point. In units of
							   the cartesian system.  */
	double m0;				/* Scale reduction factor. */				
	double alpha;			/* Differential equation adjustment factor to
							   obtain zero scale distortion at the central
							   point. */
	double alpha_e2;		/* alpha * ecent / 2.0; a commonly used
							   constant. */
	double K;				/* Second adjustment factor. */
	double bb0;				/* Latitude, on the intermediary conformal
							   sphere, of the central point. */
	double sin_bb0;
	double cos_bb0;
	double one_mm;			/* A quantity in system units which equals
							   approximately one millimeter on the
							   surface of the earth. */
	double resolve;			/* The delta value used by the inverse function
							   to terminate the iteration. */
	short quad;				/* Non-zero specifies a cartesian system
							   other than the standard right handed
							   system. */
};

/*
	The following structure defines the construction of the Krovak
	projection, a form of Oblique Conic Projection and officialy
	adopted in 1927 for use in defining the coordinate system for
	(at that time) Cechoslovakia.

	One might think that this would just be a variation of a more
	general Oblique Conformal Conic implementation, but there were
	just too many variations between this and standard cartographic
	processes.  Thus, this guy lives as a projection on its own.

	As such, there are virtually no parameters for this projection.
	The basic parameters are hard coded.  The fact that all of these
	(rho0 in particular) could not be related to standard cartographic
	expressions is why this projection lives on its own.
*/

struct cs_Krovk_
{
		/* The next five values could be considered parameters,
		   but are hard coded in the setup function for Cechoslovakia. */
	double orgLng;			/* Longitude of central point, on the ellipsoid
							   (Bessel 1841), in radians */
	double orgLat;			/* Latitude of central point, on the ellipsoid
							   (Bessel 1841), in radians */
	double poleLng;			/* Longitude of oblique pole, on the ellisoid
							   in radians. */			
	double poleLat;			/* Latitude of oblique pole, on the ellipsoid
							   in radians */
	double stdLat;			/* Latitude, on the oblique sphere, of the
							   single standard parallel for the conic
							   projection. */
	double k0;				/* Scale reduction of the oblique standard
							   parallel. */
	double x_off;			/* false easting */
	double y_off;			/* false northing */

		/* The following are extracted from the dictionary. */
	double k;				/* The scale factor in use.  This includes
							   the map scale and the units conversion
							   from the meters in which ellipsoids
							   are defined to whatever the coordinate
							   system units are. */
	double e_rad;			/* Equatorial radius of the ellipsoid in use,
							   IN METERS!!!. */
	double ecent;			/* The ecentricity of the ellipsoid in use. */

		/* The rest of this stuff is calculated by the set up function. */
	double e_sq;			/* The square of the ecentricity of the
					   ellipsoid in use. */
	double e_ovr_2;			/* One half of the ecentricity. */
	double N0;				/* Basic radius used in several places.  Don't
							   actually know what this radius is. */
	double kR;				/* used in the scale calculations. */
	double K;				/* Second adjustment factor. */
	double logK;			/* Performance is improved by using this value
							   instead of K. */
	double alpha;			/* Differential equation adjustment factor to
							   obtain zero scale distortion at the central
							   point. */
	double alpha_eo2;		/* alpha * ecent / 2.0; a commonly used
							   constant. */
	double nn;				/* The cone constant for the spherical
							   conic calculation, sin of stdLat. */
	double rho0;			/* radius of the circle on the conic which
							   represents the origin latiude for the
							   conic. */
	double resolve;			/* The delta value used by the inverse function
							   to terminate the iteration.  Essentially
							   0.001 seconds of arc. */
	double one_o_nn;		/* One over nn. */
	double tanTermF;		/* constant term used in the foward function. */
	double tanTermI;		/* constant term used in the inverse function */ 

	double orgLngS;			/* Origin longitude converted to the
							   gausian surface. */
	double orgLatS;			/* Origin latitude converted to the
							   gausian surface. */
	double sinOrgLatS;
	double cosOrgLatS;

	double lngQ;			/* Longitude of the oblique pole transformed
							   to the gausian surface. */
	double latQ;			/* Latitude of the oblique pole transformed
							   to the gausian surface. */
	double sinLatQ;
	double cosLatQ;

		/* The following are computed to facilitate boundary checking
		   and handling of absurd coordinate values without crashing. */
	double infinity;		/* Y values are limited to this value during
							   the inverse. */
	double pole_test;		/* Used to make sure latitudes are rational
							   before inverting them. */
	double theta_max;		/* Max legal theta value, used to prevent
							   inverting an XY coordinate in the conic crack. */
	double one_mm;			/* A quantity in system units which equals
							   approximately one millimeter on the
							   surface of the earth. */
	short apply95;			/* Non-zero indicates that the 95 adjustment should
							   be applied. */
	short quad;				/* Non-zero specifies a cartesian system
							   other than the standard right handed
							   system. */
};

/*
	The following structure closely resembles the cs_Trmer_ structure.
	This is true as one of the primary functions of the Sys34
	projection technique is the conversion of geogrpahic coordinates
	to UTM coordinates and vice versa.

	In addition to the UTM conversion, we also add a conversion
	to the Danish System 34 coartesian coordinates, which is
	carried out by a polynomial.  This is necessary as the
	System 34 coordinates are defined only by way of the polynomial
	expansion which converts directly from UTM's to System 34
	cartesian and vice versa.
*/

struct cs_Sys34_
{
	/* Much to the commentary here applies solely to the UTM
	   portion of this conversion.  There are only two UTM
	   zones involved, UTM 32 Noerthm and UTM 33 North.  All
	   of this is expected to be referenced to the Eurpean
	   Datum of 1950, and thus the ellipsoid is the
	   International 1924. */

	double cent_lng;		/* Longitude of the central meridian,
							   in radians. */
	double org_lat;			/* Latitude oif the origin, in radians.
							   always zero in this case. */
	double k;				/* Scale factor on the central
							   meridian.  This includes the
							   map scale factor, the units
							   conversion, and the scale
							   reduction factor. */
	double k0;				/* This is the actual scale reduction
							   factor, the scale function needs
							   this unencumbered by other
							   scale factors. */
				/* Datum values captured from the
				   active datum, repeated here
				   to prevent recalculation. */
	double ecent;
	double e_sq;
	double e_rad;

	double Rk;				/* Scaled radius of the earth. */
	double Rk_ovr_2;		/* Scaled radius of the earth divided
							   by two. */
	double eprim_sq;		/* The ecenticity prime squared
							   value required by the transformation
							   which can be computed once, once
							   the datum has been specified. */
	double xx_max;			/* The X coordinate value, in user units,
							   but not includeing false origin, which
							   we use to approximate the eastern and
							   western extents of the projectionable
							   region. */
	double one_mm;			/* equivalent of one millimeter in
							   system units. */
	struct cs_MmcofF_ mmcofF;
							/* Coefficients necessary to compute
							   meridonal distance from the equator. */
	struct cs_MmcofI_ mmcofI;
							/* Coeffieients necessary to compute the
							   foot print lataitude. */
	short region;			/* a value in the range of 1 through 3 inclusive,
							   which indicates the specific Danish region
							   which this conversion applies to. */
	short quad;				/* Non-zero specifies a cartesian system
							   other than the standard right handed
							   system.  Can also carry bits concerning
							   specific variations. This bundling is
							   done for performance reasons. */
	short vintage;			/* A value which specifies which set of
							   of polynomials are to be used.
							   Currently, we support the original
							   polynomials and polynomials of 1999. */
};

/*
	The following structure implements the Winkel-Tripel projection.
	This is usually used for whole world maps.  The inverse is of
	our own devise.
*/
struct cs_Winkl_
{
	double org_lng;
	double ref_lat;				/* Std Lat == 50.467 for Winkel, 40.0
								   for Bartholomew. */
	double e_rad;
	double cos_ref_lat;
	double k;
	double ka;
	double twoKa;
	double Rcos_ref_lat;
	double yy_max;			/* The Y value of the poles. */

	double x_off;
	double y_off;
	double one_mm;
	short quad;				/* Non-zero specifies a cartesian system
							   other than the standard right handed
							   system. */
};

/*
	The following defines the Unity projection.  Actually, not
	a projection in a real sense, but a vehicle by which we
	effectively turn lat/longs into a coordinate system and
	associate units, origin, datums, etc. with them.
*/

struct cs_Unity_
{
	double gwo_lng;			/* The longitude of the location,
							   relative to Greenwich, of the
							   prime meridian of the system
							   being defined.  This value is
							   in degrees. */
	double unit_s;			/* Scale factor by which lat/longs
							   in degrees are to be multiplied
							   in order to acheive the desired
							   result. */
	double k;				/* Value returned by CSunityK. */


	double usr_min;			/* The minimum value of longitude,
							   in user units.  Used to
							   normalize longitude per the
							   user's desire. */
	double usr_max;			/* The maximum value of longitude. */
	double usr_rng;			/* The complete range from the
							   minimum to the maximum in user
							   units.  Not allowed to be less
							   than the equivalent of 360
							   degrees. */
	double usr_2pi;			/* The equivalent of 2 pi in user's
							   units, used for normalization
							   to user's desired range. */
	short quad;				/* Non-zero specifies a non-conventional
							   geographic coordinate system. Typically
							   positive longitude is west longitude. */
};

/*
	The following defines the Non-Earth system.  This is another system,
	which is not really a projection.  This thing is used to pass
	cartesian coordinates through the system and perform some simple
	operations such as change units.

	This fits into the system only if we have some assurance that
	both the input system and the output system are based on the
	same system.  To accomplish this, we do the following:

	1> All of these systems must be cartographically referenced.
	   That is, these things are not allowed by the Q function to
	   reference a datum.  Exactly which ellipsoid is referenced
	   does not mean anything; but a valid ellipsoid must be
	   referenced.

	2> The intermediate coordinates produced by the Inverse, and
	   processed by the Forward function, are incredibly small
	   numbers.  Thus, if the input system is funneled into a
	   normal cartesian system, the results will be all zeros.
	   Similarly, if the output of a cartesian system is funneled
	   into a nerth, the results will be absolutely huge.  Thus,
	   it is unlikely that an erroneous conversion could be made
	   and not noticed.
*/

struct cs_Nerth_
{
	double k;				/* Unit, mapping, and user scale. */
	double factor;			/* The factor used to produce intermediary
							   coordinates, includes units and scaling. */
	double x_off;			/* false easting */
	double y_off;			/* false northing */

	double rotOrgX;			/* origin of scale/rotation point */
	double rotOrgY;
	double scale;			/* scale */
	double rotation;		/* rotation angle, in degrees */
	double sinRot;
	double cosRot;

	unsigned short prjCode;
	short quad;				/* Non-zero specifies a cartesian system
							   other than the standard right handed
							   system. */
};

/*
	The following defines a structure used to contruct Gaussian
	Shperes, a common need.  This is NOT a projection.  It is
	often used in the construction of a projection.  It is also
	an after thought, so there are several places where this object
	and the related code could be used, but it isn't yet.
	
	A Gaussian Sphere is essentially a surface to which the ellipsoid
	can be mapped conformally.  Once mapped to the Gaussian Surface,
	the geographic coordinates can be projected using spherical
	techniques.
	
	The essence of this technique is referred to as the Gaussian
	Curvature, which is the radius of the Gaussian Sphere.  This
	is defined as the geometric mean of the meridional and first
	vertical section radii of curvature at the standard latitude.
*/
struct cs_Gauss_
{
	double e_rad;			/* equatorial radius of the ellipsoid
							   for which a Gaussian Sphere is to
							   be created; (usually) in meters;
							   may be scaled by a scale reduction
							   factor */
	double ecent;			/* Ecentricity of the ellipsoid for which
							   a Gaussian SPhere is to be created */
	double e_sq;			/* ecent squared */
	double orgLng;			/* origin longitude, in radians */
	double stdLat;			/* latitude at which the Gaussian
							   curvature is calculated; in
							   radians */
	double N0;				/* The gaussian curvature in meters. */
	double alpha;			/* Used to convert longitudes to and
							   from the gaussian sphere */
	double logK;			/* Used to convert geographic coordinates
							   to and from the Gaussian sphere */
	double K;				/* helpful during test and debug */
	double e_ovr_2;			/* ecent / 2.0 */
	double one_esq;
	double alpha_eo2;		/* alpha * (ecent / 2) */
	double resolve;			/* the value used to terminate the
							   iterative inverse latitude calculation */
};

/*
	The following structure contains everything an upper
	level module needs to know about converting lat/longs
	to a specific coordinate system and vice-versa.  The
	idea here is that the upper level module is completely
	independent of the coordinate system in use.  It
	just needs to call CS_csloc to get one of these
	created and then call CS_ll2cs or CS_cs2ll as
	appropriate.
*/

struct cs_Csprm_
{
	struct cs_Csdef_ csdef;	/* The coordinate system definition
							   as obtained from the coordinate
							   system dictionary. */
	struct cs_Datum_ datum;	/* The datum definition as obtained
							   from the datum dictionary. */

	/* Information for the following is supplied by the
	   setup function.  If the coordinate system definition
	   carries values for the useful range of the coordinate
	   system, the supplied values are placed here without
	   checking or limitation.  All checks (i.e. CS_xychk
	   and CS_llchk) always check the mathematical domain
	   which is independent of this check.  Therefore,
	   specifying large values can essentially disable this
	   checking feature on a coordinate system by
	   coordinate system basis.

	   If the coordinate system definition does not carry
	   values for these limits, the setup function will
	   calculate some values appropriate for the coordinate
	   system and its underlying projection.

	   The lat/long values are in degrees, and longitude
	   values are relative to the cent_mer element.  The
	   cent_mer element is required to solve the -180
	   degree longitude crack problem. */

	double cent_mer;		/* The longitude upon which the min and
							   max longitudes are based; in degrees. */
	double min_ll [2];		/* The delta longitude and absolute latitude
							   of the southwestern extent of the useful
							   range of this coordinate system; in
							   degrees. */
	double max_ll [2];		/* The delta longitude and absolute latitude
							   of the north eastern extent of the useful
							   range of this coordinate system; in
							   degrees. */
	double min_xy [2];		/* The minimum X and Y coordinates of the
							   useful range of this coordinate system.
							   Values and units are per coordinate
							   system definition and include the false
							   origin.  For a coordinate system in a
							   quadrant other than the first quad,
							   these values may not be the southwestern
							   extent as would normally be expected. */
	double max_xy [2];		/* The minimum X and Y coordinates of the
							   useful range of this coordinate system. */
	unsigned short prj_code;
							/* The projection code extracted from the
							   projection table for the active
							   projection. */
	ulong32_t prj_flags;
							/* The projection flags as extracted from
							   the projection table for the active
							   projection. */

		/* The setup function for each projection places
		   a pointer to the appropriate function in the
		   following slots. */

	int (*ll2cs)(Const void *prj_prms,double *ll,Const double *xy);
							/* Pointer to the function which
							   converts lat/longs to the
							   coordinate system values. */
	int (*cs2ll)(Const void *prj_prms,double *xy,Const double *ll);
							/* Pointer to the function which
							   converts coordinate system
							   values to lat/longs. */
	double (*cs_scale)(Const void *prj_prms,Const double *ll);
							/* Pointer to the function which
							   returns the scale of the
							   coordinate system at the
							   referenced lat/long point. */
	double (*cs_sclk)(Const void *prj_prms,Const double *ll);
							/* Pointer to the function which
							   returns the scale of the
							   coordinate system at the
							   referenced lat/long point. */
	double (*cs_sclh)(Const void *prj_prms,Const double *ll);
							/* Pointer to the function which
							   returns the scale of the
							   coordinate system at the
							   referenced lat/long point. */
	double (*cs_cnvrg)(Const void *prj_prms,Const double *ll);
							/* Pointer to the function which
							   returns the scale of the
							   coordinate system at the
							   referenced lat/long point. */
	int (*llchk)(Const void *prj_prms,int cnt,Const double pnts [][3]);
							/* Pointer to the function which
							   checks if the geographic domain
							   of the coordinate system
							   includes the provided point or line
							   segment. */
	int (*xychk)(Const void *prj_prms,int cnt,Const double pnts [][3]);
							/* Pointer to the function which
							   checks if the cartesian domain
							   of the coordinate system
							   includes the provided point or line
							   segment. */
	union
	{
				/* This union contains one structure
				   for each of the supported
				   projections. */

		struct cs_Lmbrt_ lmbrt;
		struct cs_Trmer_ trmer;
		struct cs_Trmrs_ trmrs;
		struct cs_Alber_ alber;
		struct cs_Unity_ unity;
		struct cs_Oblqm_ oblqm;
		struct cs_Mrcat_ mrcat;
		struct cs_Azmed_ azmed;
		struct cs_Lmtan_ lmtan;
		struct cs_Plycn_ plycn;
		struct cs_Modpc_ modpc;
		struct cs_Azmea_ azmea;
		struct cs_Edcnc_ edcnc;
		struct cs_Millr_ millr;
		struct cs_Pstro_ pstro;
		struct cs_Ostro_ ostro;
		struct cs_Sstro_ sstro;
		struct cs_Mstro_ mstro;
		struct cs_Nzlnd_ nzlnd;
		struct cs_Sinus_ sinus;
		struct cs_Ortho_ ortho;
		struct cs_Gnomc_ gnomc;
		struct cs_Edcyl_ edcyl;
		struct cs_Vdgrn_ vdgrn;
		struct cs_Csini_ csini;
		struct cs_Robin_ robin;
		struct cs_Bonne_ bonne;
		struct cs_Ekrt4_ ekrt4;
		struct cs_Ekrt6_ ekrt6;
		struct cs_Molwd_ molwd;
		struct cs_Hmlsn_ hmlsn;
		struct cs_Nacyl_ nacyl;
		struct cs_Tacyl_ tacyl;
		struct cs_Bpcnc_ bpcnc;
		struct cs_Swiss_ swiss;
		struct cs_Krovk_ krovk;
		struct cs_Nerth_ nerth;
		struct cs_Sys34_ sys34;
		struct cs_Winkl_ winkl;
	} proj_prms;
};

/*
	A table of the following structure defines the known
	projection types.
*/

struct cs_Prjtab_
{
	char key_nm [24];
	void (EXP_LVL9 * setup)(struct cs_Csprm_ *cs_prm);
	int  (EXP_LVL9 * check)(Const struct cs_Csdef_ *cs_def,unsigned short prj_code,
														   int err_list [],
														   int list_sz);
	unsigned short code;
	ulong32_t flags;
	ulong32_t epsg;				/* EPSG code, zero if none or not one to one */
	char descr [64];
};

/*
	Function CS_prjprm will populate a structure of the following
	form.  Intended to be used to make programming Coordinate
	System Editors a sane project.

	CS_prjprm uses a table defined in CSdataPJ which application
	programmers may wish to edit.

	The sprf_typ argument is used to indicate that the label
	contains sprintf format specifications, and indicates what
	type of processing is to be completed.  I.e. a kludge to
	handle such things as Modified Sterographic complex
	parameters in an efficient manner.

*/

struct cs_Prjprm_
{
	double min_val;			/* Minimum value. */
	double max_val;			/* Maximum value. */
	double deflt;			/* A suitable default value; zero
							   whenever zero is a valid value. */
	long32_t format;			/* A format specification is in the
							   form expected by CS_ftoa which
							   is the most suitable. E.g.
							   ddmmss for angles, lat's and
							   longs. */
	long32_t help_id;			/* Not used by CS-MAP.  Reserved for
							   use by users to stash a context
							   sensitive help ID value. */
	long32_t labl_id;			/* Not used by CS-MAP.  Reserved for
							   use by Windows programmers to
							   stash a string resource ID for
							   the parameter label if so
							   desired. */
	char label [48];		/* Screen label, in ASCII (ANSI code)
							   English. */
	short phys_type;		/* Physical type, classifies the type
							   of the parameter.  Currently, all
							   parameters are doubles, maybe not
							   so in the future. */
	short log_type;			/* Logical type, classifies the type
							   of the parameter: e.g.
							   cs_PRMTYP_LONG for longitude. */
	ulong32_t epsg;			/* The EPSG code number of the equivalent
							   parameter; zero if there is no
							   EPSG equivalent. */
	short prj_code;			/* The projection code for which this
							   structure was generated. */
	short parm_nbr;			/* The parameter number for which this
							   structure was generated. */
	short sprf_type;		/* Indicates the type of sprintf
							   processing which is to be done. */
};

/*
	An array of the following structure is used to map a projection
	code into as many as 24 projection parameter types.
*/

struct cs_PrjprmMap_
{
	short prj_code;					/* Projection code.  Array is
									   searched on this value, thus
									   projection code numbers need
									   not be sequential. */
	unsigned char prm_types [24];	/* For each of 24 parameters, an
									   index into the cs_Prjprm array.
									   Must be one of the cs_PRMCOD
									   values defined above. */
};

/*
	An array of the following structure is used to maintain the list
	of supported Geodetic Data Catalogs (i.e. .gdc files).  The table
	is accessed by the manifest constant used to represent the datum
	shift technique which implements access to the specific file.
	See CSdata.c for the definition of the table with content.

	The commented outnumbers to the right are included only as
	a debugging aid.
*/
enum cs_GdcCatalogType
{
	gdcTypeNone = 0,
	gdcTypeVertical = 3,					/* hard set to preserve legacy values */
	gdcTypeSeparation
};
enum cs_GdcCatalogs
{
	gdcFileNone = 0,						/* for Debugging */
	gdcFileGeoidHeight = 13,				/*  13, hard set to preserve legacy value */
	gdcFileVertcon,							/*  14 */
	gdcFileDisabled = 9999
};

struct cs_GdcCatalogTable_
{
	enum cs_GdcCatalogs ident;		/* A positive value unique to each
									   supported datum catalog file. */
	char *gdcName;					/* Pointer to a global variable which
									   contains the name to be used.
									   We use a pointer here to remain
									   consistent with previous code. */

	enum cs_GdcCatalogType type;	/* Type of catalog. */
	ulong32_t optionMask;			/* A mask which indicates which bits
									   of a datum catalog option value
									   are recognized by this technique.
									   Currently, all are zero. */
	char filterU [32];				/* A string which lists the file
									   extensions which are supported
									   by this technique.  The extensions
									   are given with the "dot" character,
									   and this can be used to separate one
									   extension from another. */
	char filterW [128];				/* A string which can be used as a 
									   file name filter in a Windows
									   OpenFile sequence. */
};

/*
	A table of the following structure defines the conversion
	between different unit systems.  In the length case, factor is
	the multiplicative factor for conversion to meters.  In the
	angular case, factor is the multiplicative factor for conversion
	to degrees.  Only two types are currently supported, others
	may be added in the future.
*/

#define cs_UTYP_END 0
#define cs_UTYP_LEN 'L'
#define cs_UTYP_ANG 'R'
#define cs_UTYP_OFF 'z'

#define cs_USYS_None 0
#define cs_USYS_Metric 1
#define cs_USYS_English 2

struct cs_Unittab_
{
	short type;
	char name [32];
	char pluralName [48];
	char abrv [8];
	short system;
	double factor;
	long32_t epsgCode;
};

#include "cs_Geodetic.h"

/*
	The following structures are used for group maintenance.
*/

struct cs_Csgrplst_
{
	struct cs_Csgrplst_ *next;
	char key_nm [24];
	char descr [64];
	char source [64];
	char ref_typ [10];
	char ref_to [24];
	char unit [16];
};

#define cs_GRPTBL_ACTIVE    0x01
#define cs_GRPTBL_END	    0x80

struct cs_Grptbl_
{
	char group [24];
	char descr [64];
	unsigned short flags;
};

/*
	The following structures define the form of the
	coordinate system and datum conversion caches which
	are developed and maintained by CS_basic, the one
	function interface to CS_MAP.

	The define specifies how many cache entries are
	allocated.  That is, how many different coordinate
	system definitions are cached before existing ones
	are free'ed to make room for new ones.  You may change
	it here and recompile, or simply change the value of the
	csCscachI variable at run time (before any of the
	functions in the CS_basic module are called).

	Note: these structures are doubly linked lists so that
	the most recently accessed item can be easily forced
	to the top of the list.  Thus, the last item in the
	list is the least recently accessed item and is the
	one which is always free'ed when such is necessary.
*/

#define cs_CSCACH_MAX 8
struct csCscach_
{
	struct csCscach_ *next;
	struct csCscach_ *prev;
	struct cs_Csprm_ *cs_ptr;
	char cs_nam [24];
};

#define cs_DTCACH_MAX 8
struct csDtcach_
{
	struct csDtcach_ *next;
	struct csDtcach_ *prev;
	struct cs_Dtcprm_ *dtc_ptr;
	char src_cs [24];
	char dst_cs [24];
};

/* MGRS object definition.  (Military Grid Reference System) */
struct cs_Mgrs_
{
	short Bessel;					/* 0 say standard, 1 says Bessel alternative */
	double E_rad;					/* Equatorial radius in meters. */
	double E_sq;					/* Eccentricity Squared */
	struct cs_Trmer_ NorthHemi;		/* TM parameters, northern hemisphere */
	struct cs_Trmer_ SouthHemi;		/* TM parameters, southern hemisphere */
	struct cs_Pstro_ NorthPole;		/* UPS parameters, north pole */
	struct cs_Pstro_ SouthPole;		/* UPS parameters, south pole */
};

/* The following enumerates the various possibilities of coodinate
   values with regard to a specific MGRS grid square.  The default
   value is cs_MGRS_GRDSQR_CENTER.
   
   Please note that the code referencing this enumerator often relies
   on the fact that cs_MGRS_GRDSQR_NONE is less than cs_MGRS_GRDSQR_CENTER,
   and that cs_MGRS_GRDSQR_CENTER is less than all other values.  Also,
   the code assumes that the cs_MGRS_GRDSQR_UNKNOWN value is higher than
   all other values. */
#define cs_MGRS_GRDSQR_NONE       0
#define cs_MGRS_GRDSQR_CENTER     1
#define cs_MGRS_GRDSQR_SOUTHWEST  2
#define cs_MGRS_GRDSQR_WEST       3
#define cs_MGRS_GRDSQR_NORTHWEST  4
#define cs_MGRS_GRDSQR_NORTH      5
#define cs_MGRS_GRDSQR_NORTHEAST  6
#define cs_MGRS_GRDSQR_EAST       7
#define cs_MGRS_GRDSQR_SOUTHEAST  8
#define cs_MGRS_GRDSQR_SOUTH      9
#define cs_MGRS_GRDSQR_UNKNOWN    10

/* A static array of the following structure is used to build a table
   which will map all of the above defined enumerations into a unit
   vector representing the direction in which a coordinate must be
   adjusted to convert from the default "CENETER" position to the
   requested position. */
struct csMgrsGrdSqrVector_
{
	short enumValue;            /* Redundant, but helpful in test and debug. */
	double xx;
	double yy;
};

/* Three separate constructors.  In C++ they would be simple overloads.  In C,
   we need separate names. */

// Forward declarations
struct cs_Csdef_;
struct cs_Dtdef_;
struct cs_Eldef_;

#ifdef __cplusplus
extern "C" {
#endif
struct cs_Mgrs_ *CSnewMgrs (double e_rad,double e_sq,short bessel);
struct cs_Mgrs_ *CSnewMgrsE (Const char *ellipsoid,short bessel);
struct cs_Mgrs_ *CSnewMgrsD (Const char *datum,short bessel);
void CSdeleteMgrs (struct cs_Mgrs_ *__This);
int CScalcUtmUps (struct cs_Mgrs_ *__This,double utmUps [2],double latLng [2]);
int CScalcLatLng (struct cs_Mgrs_ *__This,double latLng [2],double utmUps [2],int utmZone);
int CScalcMgrsFromLl (struct cs_Mgrs_ *__This,char *result,int size,double latLngl [2],int prec);
int CScalcMgrsFromLlUtm (struct cs_Mgrs_ *__This,char *result,int size,double latLng [2],double utmUps [2],int prec);
int CScalcLlFromMgrs (struct cs_Mgrs_ *__This,double ll [2],Const char *mgrsString);
int CScalcLlFromMgrsEx (struct cs_Mgrs_ *__This,double ll [2],Const char *mgrsString,short grdSqrPos);
int CScalcRegnFromMgrs (struct cs_Mgrs_ *_This,double sw [2],double ne [2],Const char *mgrs);
#ifdef __cplusplus
}
#endif


/*
	The following define the magic numbers which appear
	on the front of the Coordinate System Dictionary and
	the Datum Dictionary.
*/

/* #define cs_MAGIC_BASE 0x800D0000L       previous value, */
#define cs_MAGIC_BASE -0x7FF30000L		/* PClint likes this much better */

/*
	Release 3.0 Magic Numbers

#define cs_CSDEF_MAGIC (cs_MAGIC_BASE | 1)
#define cs_DTDEF_MAGIC (cs_MAGIC_BASE | 2)
*/

/*
	Release 4.0 magic numbers.

#define cs_CSDEF_MAGIC (cs_MAGIC_BASE | 3)
#define cs_DTDEF_MAGIC (cs_MAGIC_BASE | 4)
#define cs_ELDEF_MAGIC (cs_MAGIC_BASE | 5)
*/

/*
	Release 5.0 Magic numbers.

#define cs_CSDEF_MAGIC (cs_MAGIC_BASE | 5)
#define cs_DTDEF_MAGIC (cs_MAGIC_BASE | 6)
#define cs_ELDEF_MAGIC (cs_MAGIC_BASE | 9)
*/

/*
	Release 6.0 Magic Numbers


#define cs_MULREG_MAGIC (cs_MAGIC_BASE | 24)
*/

/*
	 Release 7.0 Magic Numbers.

#define cs_CSDEF_MAGIC (cs_MAGIC_BASE | 14)
#define cs_DTDEF_MAGIC (cs_MAGIC_BASE | 15)
#define cs_ELDEF_MAGIC (cs_MAGIC_BASE | 13)
*/

/* Release 8.0 Magic Numbers. */

#define cs_CSDEF_MAGIC05 (cs_MAGIC_BASE | 5)	/* Rls 5 Coordsys files. */
#define cs_DTDEF_MAGIC05 (cs_MAGIC_BASE | 6)	/* Rls 5 datum files. */
#define cs_ELDEF_MAGIC05 (cs_MAGIC_BASE | 9)	/* Rls 5 Ellipsoid files. */
#define cs_CSDEF_MAGIC06 (cs_MAGIC_BASE | 12)	/* Rls 6 Coordsys files. */
#define cs_DTDEF_MAGIC06 (cs_MAGIC_BASE | 11)	/* Rls 6 Datum files. */
#define cs_MULREG_MAGIC06 (cs_MAGIC_BASE | 24)	/* Rls 6 Multiple Regression
												   Transformation files. */
#define cs_CSDEF_MAGIC07 (cs_MAGIC_BASE | 14)	/* Rls 7 Coordsys files. */
#define cs_DTDEF_MAGIC07 (cs_MAGIC_BASE | 15)	/* Rls 7 Datum files. */
#define cs_ELDEF_MAGIC07 (cs_MAGIC_BASE | 13)	/* Rls 7 Ellipsoid files. */

#define cs_GPDEF_MAGIC   (cs_MAGIC_BASE | 20)	/* Current Geodetic Path files. */
#define cs_GXDEF_MAGIC   (cs_MAGIC_BASE | 19)	/* Current Geodetic Transformation files. */
#define cs_CSDEF_MAGIC   (cs_MAGIC_BASE | 18)	/* Current Coordsys files. */
#define cs_DTDEF_MAGIC   (cs_MAGIC_BASE | 17)	/* Current Datum files. */
#define cs_ELDEF_MAGIC   (cs_MAGIC_BASE | 16)	/* Current Ellipsoid files. */
#define cs_CTDEF_MAGIC   (0x3372ED6CL)			/* Current Category files. */
#define cs_MULREG_MAGIC  (cs_MAGIC_BASE | 25)	/* Current Multiple Regression
												   Transformation files. */
#define cs_FIPS_MAGIC    (cs_MAGIC_BASE | 32)	/* FIPS Code data file. */



/*
	CS_MAP errors are now classified into specific groups to
	enable selective suppression of error messages.  This is
	used primarily to enable basic functions, such as CS_csdef,
	to be used as validation functions without causing an
	error report.  Currently, the groups are defined below.

	These groups are assigned a specific bit, and the global
	variable cs_ErrSup is examined to see if reporting of a
	specific group is to be suppressed.  Setting cs_ErrSup
	to zero (the normal setting) enables all messages.  Setting
	cs_ErrSup to 0xFFFF suppresses the reporting of all messages.

	For example, to simply verify the existance of a coordinate
	system, one could:

	err_sup_save = cs_ErrSup;
	cs_ErrSup |= cs_ERSUP_OPR;
	cs_def = CS_csdef (cs_name);
	cs_ErrSup = ewrr_sup_save;
	if (cs_def != NULL)
	{
		free (cs_def);
		valid = TRUE;
	}
	else
	{
		valid = FALSE;
	}

	The grouping, therefore, will allow reporting of conditions
	such as "Couln't open the coordinate system dictionary", but
	will disable the "Coordinate system not found" type of message.

	Remember, these values are used in a biot map fashion.
*/

#define cs_ERSUP_SOFT   1		/* Software problems; problems which are not
								   supposed to happen if all is working per
								   design. */
#define cs_ERSUP_SYS	2		/* System problems, e.g. malloc failed,
								   I/O error, disk full. */
#define cs_ERSUP_CONFIG	4		/* Installation, configuration, manufacturing
								   problems; e.g. couln't open COORDSYS */
#define cs_ERSUP_OPR	8		/* Operational problems; e.g. invalid name,
								   bogus coordinate system name.  Conditions
								   ordinarily caused by an erroneous user
								   input might cause. */
#define cs_ERSUP_DATA	16		/* A problem with supplied data; i.e.
								   convergence failures, etc. Not really
								   a bug in CS_MAP, although the user
								   might perceive it as such. */
#define cs_ERSUP_RNG	32		/* Converting data out of normal range,
								   e.g. converting a European lat/long to
								   NAD27. */

/*
	The following define the numeric codes for each
	of the possible error conditions detected by
	the CS_MAP library.
*/

#define cs_CSDICT       102		/* Open of the Coordinate System
								   Dictionary failed. */
#define cs_CS_NOT_FND   103		/* Coordinate system not found. */
#define cs_NO_MEM       104		/* Insufficient memory. */
#define cs_UNKWN_PROJ   105		/* Unknown projection code
								   encountered in the Coordinate
								   System Dictionary. */
#define cs_CS_BAD_MAGIC 106		/* Magic number (first four bytes)
								   of Coordinate System Dictionary
								   indicate that the file is not
								   a coordinate system dictionary. */
#define cs_IOERR        107		/* Physical I/O error. */
#define cs_DTDICT       109		/* Open of Datum Dictionary failed. */
#define cs_DT_BAD_MAGIC 110		/* Magic number (first four bytes)
								   of Datum Dictionary indicate that
								   the file is not a Datum
								   Dictionary. */
#define cs_DT_NOT_FND   111		/* Datum definition not found in
								   Datum Dictionary. */
#define cs_INV_FILE     113		/* File is corrupted, specifically
								   a read of n bytes which was
								   supposed to produce n bytes,
								   didn't produce n bytes. */
#define cs_TMP_CRT      114		/* Creation of a temporary file
								   failed. */
#define cs_DISK_FULL    115		/* Disk is full, or for some other
								   reason a disk write failed. */
#define cs_UNLINK       116		/* Couldn't unlink (i.e. delete)
								   a file. */
#define cs_RENAME       117		/* Couldn't rename a file. */
#define cs_INV_NAME     118		/* Coordinate System or Datum key
								   name is invalid.  Must start
								   with an alphabetic character. */
#define cs_INV_UNIT     119		/* Invalid unit name. */
#define cs_DTC_NO_FILE  120		/* Couldn't open a required datum
								   conversion file.  Not currently
								   used; new datum conversion software
								   may require such. */
#define cs_DTC_MAGIC    122		/* Magic number (i.e. first four bytes)
								   of datum conversion data file were
								   not correct.  Not currently used.
								   New datum conversion software may
								   require such in the future. */
#define cs_DTC_NO_SETUP 124		/* Datum conversion function was
								   called bu the datum conversion
								   was not properly initialized. */
#define cs_NADCON_ICNT  126		/* The NADCON iteration count (9)
								   failed to resolve the NAD83 to
								   NAD27 conversion within the
								   required tolerance. */
#define cs_NADCON_CONS  127		/* Properly named NADCON database
								   files are inconsistent, e.g.
								   don't cover the same region. */
#define cs_DTC_FILE     128		/* Couldn't open a required datum
								   conversion file. */
#define cs_DTC_DAT_F    129		/* FATAL: Requested datum conversion
								   not supported in its most precise
								   form. */
#define cs_DTC_DAT_W    130		/* WARNING: Optional software would
								   produce more precise datum
								   conversion results. */
#define cs_DTC_BLK_F    131		/* FATAL: Data required to convert
								   specific coordinate is not
								   available on this system. */
#define cs_DTC_BLK_W    132		/* WARNING: Data required to convert
								   specific coordinate is not
								   available on this system. */
#define cs_DTC_BLK_Q    133		/* WARNING: System has reported 20
								   cs_DTC_BLK_W error conditions,
								   additional occurences will be
								   suppressed. */
#define cs_WGS_CNVRG    134		/* The iterative inverse WGS-84
								   calculations failed to converge. */
#define cs_EL_NOT_FND   135		/* Ellipsoid definition not found in
								   Ellipsoid Dictionary. */
#define cs_ELDICT       136		/* Open of Ellipsoid Dictionary
								   failed. */
#define cs_EL_BAD_MAGIC 137		/* Magic number (first four bytes)
								   of Ellipsoid Dictionary indicate
								   that the file is not a Ellipsoid
								   Dictionary. */
#define cs_NAD_NO_DATA  138		/* Couldn't locate and NADCON data
								   when initializing NADCON
								   alogorithm. */
#define cs_ISER         139		/* CS_MAP internal software error. */
#define cs_HPGN_ICNT    140		/* The NADCON iteration count (9)
								   failed to resolve the NAD91 to
								   NAD83 conversion within the
								   required tolerance. */
#define cs_HPGN_CONS    141		/* Properly named HPGN database
								   files are inconsistent, e.g.
								   don't cover the same region. */
#define cs_HPG_NO_DATA  142		/* Couldn't locate and HPGN data
								   when initializing HPGN
								   alogorithm. */
#define cs_HPG_BLK_F    143		/* FATAL: Data required to convert
								   specific HPGN coordinate is not
								   available on this system. */
#define cs_HPG_BLK_W    144		/* WARNING: Data required to convert
								   specific HPGN coordinate is not
								   available on this system. */
#define cs_HPG_BLK_Q    145		/* WARNING: System has reported 10
								   cs_HPG_BLK_W error conditions,
								   additional occurences will be
								   suppressed. */
#define cs_MREG_RANGE   146		/* WARNING: Conversion outside of
								   normal range of Multiple Regression
								   formulas. */
#define cs_MREG_BADMAG  147		/* Invalid magic number on Multiple
								   Regression data file. */
#define cs_NWDT_READ	148		/* Error reading old datum file when
								   updating to new format. */
#define cs_NWDT_WRIT	149		/* Error writing new datum file when
								   updating to new format. */
#define cs_TMPFN_MAXED  150		/* Algorithm to generate a temporary
								   file name is max'ed out. */
#define cs_BSWP_UNKWN   151		/* Byte ordering of this system is
								   unsupported. */
#define cs_NWCS_READ	152		/* Error reading old datum file when
								   updating to new format. */
#define cs_NWCS_WRIT	153		/* Error writing new datum file when
								   updating to new format. */
#define cs_NWEL_READ	154		/* Error reading old datum file when
								   updating to new format. */
#define cs_NWEL_WRIT	155		/* Error writing new datum file when
								   updating to new format. */
#define cs_NO_REFERNCE	156		/* CS_csloc was asked to activate a
								   coordinate system which had
								   neither a cartographic reference
								   (i.e. an ellipsoid) or a geodetic
								   reference (i.e. a datum). */
#define cs_CSGRP_INVKEY 157 	/* Invalid group name provided to
								   group list function. */
#define cs_FL_OPEN      158		/* General file open failure. */
#define cs_SWP_TYPE     159		/* Unknown file type passed to swapper (Cs_swpfl). */
#define cs_ELDEF_INV    160		/* Ellipsoid definition is invalid. */
#define cs_DTDEF_INV    161		/* Datum definition is invalid. */
#define cs_RGN_PNTCNT   162		/* Invalid point count argument to
								   a coordinate checker function. */
#define cs_CNTV2_MULT   163		/* Multiple Version 2 Canadian
								   National Transformation files
								   detected. */
#define cs_CS_PROT      164		/* Distribution coordinate system is
								   protected. */
#define cs_DT_PROT      165		/* Distribution datum definition is
								   protected. */
#define cs_EL_PROT      166		/* Distribution ellipsoid is
								   protected. */
#define cs_CS_UPROT     167		/* User defined  coordinate system is
								   now protected. */
#define cs_DT_UPROT     168		/* User defined datum is now
								   protected. */
#define cs_EL_UPROT     169		/* User defined ellipsoid is now
								   protected. */
#define cs_UNIQUE       170		/* User defined dictionary items
								   require unique character. */
#define cs_A2F_MLTSGN   171		/* Multiple sign indications in
								   numeric value. */
#define cs_A2F_ERRCMA   172		/* Inconsistent use of the comma. */
#define cs_A2F_MLTPNT   173		/* Multiple decimal point indications
								   in numeric value. */
#define cs_A2F_SECS60   174		/* Value greater than 60 encountered
								   where seconds were expected. */
#define cs_A2F_MINS60   175		/* Value greater than 60 encountered
								   where minutes were expected. */
#define cs_A2F_FMTERR   176		/* General format error encountered,
								   too many spaces. */
#define cs_INV_INDX     177		/* Invalid enumeration index. */
#define cs_INV_ARG1     178		/* Pointer argument 1 is invalid. */
#define cs_INV_ARG2     179		/* Pointer argument 2 is invalid. */
#define cs_INV_ARG3     180		/* Pointer argument 3 is invalid. */
#define cs_INV_ARG4     181		/* Pointer argument 4 is invalid. */
#define cs_INV_ARG5     182		/* Pointer argument 5 is invalid. */
#define cs_INV_UTYP     183		/* Invalid unit type. */

#define cs_GEOID_NO_DATA 184	/* Couldn't locate any Geoid data
								   when initializing Geoid
								   alogorithm. */
#define cs_GEOID_FILE	185		/* Error opening a Geoid file. */
#define cs_GEOID_NO_SETUP 186	/* Geoid algorithm has not been set up. */
#define cs_DYMUTM		187		/* Dynamic UTM operation requested on
								   a non UTM coordinate system. */
#define cs_MIF_UNIT		188		/* CS-MAP definition uses a unit not
								   supported by MapInfo. */
#define cs_MIF_ELREF	189		/* Mapinfo doesn't support direct ellipsoid
								   references. */
#define cs_MIF_DATUM	190		/* CS-MAP definition references a datum
								   which is not supported by MapInfo. */
#define cs_MIF_FEATR	191		/* CS-MAP definition uses a features not
								   supported by MapInfo. */
#define cs_MIF_PROJ		192		/* CS-MAP definition uses a projection not
								   supported by MapInfo. */
#define cs_DBL_SPACE	193		/* Double space encountered in key name. */
#define cs_MRT_NTFND	194		/* Multiple Regression file not found. */

#define cs_DTC_FIL_F    195		/* FATAL: Generally available NADCON
								   data file was not found. */
#define cs_DTC_FIL_W    196		/* WARNING: Generally available
								   NADCON data file was not found. */
#define cs_DTC_RNG_F    197		/* FATAL: Data point outside range
								   of NAD27/83 conversion. */
#define cs_DTC_RNG_W    198		/* WARNING: Data point outside range
								   of NAD27/83 conversion. */
#define cs_SPZ_INVALID  199		/* State plane zone number is invalid. */

	/* The following error conditions are reported by the
	   CS_cschk function, or its sub-components. They may be
	   somewhat redundant with the above, but are given unique
	   identifiers and error codes to support different
	   error messages/help messages, etc. */

#define cs_CSQ_AZM		201
#define cs_CSQ_AZMTH	202
#define cs_CSQ_LAT		203
#define cs_CSQ_LATEQU	204
#define cs_CSQ_LNG		205
#define cs_CSQ_LNGEQU	206
#define cs_CSQ_MAPSCL	207
#define cs_CSQ_MEREQU	208
#define cs_CSQ_MRCAT	209
#define cs_CSQ_MSCOEF	210
#define cs_CSQ_NOREF	211
#define cs_CSQ_NOTNRTH	212
#define cs_CSQ_NRTHLAT	213
#define cs_CSQ_NRTHPNT	214
#define cs_CSQ_ORGLAT	215
#define cs_CSQ_ORGLNG	216
#define cs_CSQ_PLL90	217
#define cs_CSQ_PLLEQU	218
#define cs_CSQ_PLLLRG	219
#define cs_CSQ_PLLREV	220
#define cs_CSQ_PLLZERO	221
#define cs_CSQ_POLDD	222
#define cs_CSQ_POLDUP	223
#define cs_CSQ_POLLAT	224
#define cs_CSQ_POLLNG	225
#define cs_CSQ_QUAD		226
#define cs_CSQ_SCLRED	227
#define cs_CSQ_SOTHLAT	228
#define cs_CSQ_STDLAT	229
#define cs_CSQ_STDLNG	230
#define cs_CSQ_STDPLL	231
#define cs_CSQ_STDSOU	232
#define cs_CSQ_STDWEST	233
#define cs_CSQ_UNIT		234
#define cs_CSQ_INVPRJ	235		/* Invalid projection key name
								   specification */
#define cs_CSQ_INVDTM	236		/* Invalid Datum Name. */
#define cs_CSQ_INVELP	237		/* Invalid ellipsoid name. */
#define cs_CSQ_LLRNG	238		/* Lat/Long range error. */
#define cs_CSQ_RNGORD	239		/* Lat/Long range order. */
#define cs_CSQ_INVQUAD	240		/* Invalid Quadrant specification. */
#define cs_CSQ_GEOMM	241		/* Invalid geographic useful range. */
#define cs_CSQ_CRTMM	242		/* Invalid cartesian useful range. */
#define cs_CSQ_PLLED	243
#define cs_CSQ_PLRLAT	244		/* Org Lat is not polar. */
#define cs_CSQ_USEPLR	245		/* Polr org lat on oblique projection. */
#define cs_CSQ_UTMZON	246		/* Invalid UTM zone number provided. */
#define cs_CSQ_HMISPHR	247		/* Invalid hemisphere specifiction. */		
#define cs_CSQ_USESW	248		/* Az == 90, use Swiss Oblique Mercator */
#define cs_CSQ_MAX15	249		/* Eastern meridian cannot be more than
								   15 degrees away from the central
								   meridian. */
#define cs_CSQ_OBLQPOLE 250		/* Invalid oblique pole specification. */		
#define cs_CSQ_AFFZERO	251		/* Denominator of affine is zero. */
#define cs_CSQ_NRDATUM	252		/* Non-earth referecned to a datum. */
#define cs_CSQ_OSTN97	253		/* OSTN97 grid shift file not present. */
#define cs_CSQ_ELEVEL	254		/* Elevated ellipsoid value is questionable */
#define cs_CSQ_OSTN02	255		/* OSTN02 grid shift file not present. */
#define cs_CSQ_TMKRG0	256		/* Kruger Formulation, non-zero origin latitude */
#define cs_CSQ_DENRGN	257		/* Invalid region code for Danish 34/45 */

#define cs_DTQ_FILE		258		/* Grid file not found. */
#define cs_DTQ_FORMAT	259		/* Grid file of a format different from that specified. */
#define cs_DTQ_FLSPEC	260		/* Grid file specification inappropriate for the indicated format. */
#define cs_DTQ_INVDIR	261		/* Invalid direction specififation in geodetic transformation definition. */

#define cs_DTQ_DELTAX   262	 	/* Delta X transformation parameter is suspiciously large. */
#define cs_DTQ_DELTAY   263		/* Delta Y transformation parameter is suspiciously large. */
#define cs_DTQ_DELTAZ   264		/* Delta Z transformation parameter is suspiciously large. */
#define cs_DTQ_ROTATEX  265		/* X Rotate transformation parameter is suspiciously large. */
#define cs_DTQ_ROTATEY  266		/* Y Rotate transformation parameter is suspiciously large. */
#define cs_DTQ_ROTATEZ  267		/* Z Rotate transformation parameter is suspiciously large. */
#define cs_DTQ_BWSCALE  268		/* Datum transformation scale parameter is suspiciously large. */
#define cs_DTQ_WGS72SRC 269		/* Source datum to the WGS72 transformation method is not WGS72. */
#define cs_DTQ_WGS72TRG 270		/* Target datum to the WGS72 transformation method is not WGS84. */
#define cs_DTQ_MTHCODE  271		/* Geodetic transformation method code is not recognized. */
#define cs_DTQ_XFRMNM   272		/* Geodetic transformation name is not valid. */
#define cs_DTQ_SRCDTNM  273		/* Geodetic transformation source datum name is invalid. */
#define cs_DTQ_TRGDTNM  274		/* Geodetic transformation target datum name is invalid. */
#define cs_DTQ_MAXITR   275		/* Geodetic transformation max iterations value is suspicious. */
#define cs_DTQ_CNVRGV   276		/* Geodetic transformation convergence value is suspicious. */
#define cs_DTQ_ERRORV   277		/* Geodetic transformation convergence error value is suspicious. */
#define cs_DTQ_ACCRCY   278		/* Geodetic transformation accuracy value is suspicious. */
#define cs_DTQ_FBKDIR   279		/* Fallback direction is invalid */
#define cs_DTQ_FBKNAME  280		/* Fallback name is invalid. */
#define cs_DTQ_FBKMTH   281		/* Fallback name is invalid. */
#define cs_DTQ_ISERFBK  282     /* Internal software error in the check function. */

#define cs_GPQ_NAME     283     /* Invalid path definition name. */
#define cs_GPQ_GPDTM    284     /* Invalid source or target datum name. */
#define cs_GPQ_COUNT    285     /* Invalid path element count. */
#define cs_GPQ_GXDIR    286     /* Invalid path element direction */
#define cs_GPQ_INVNM    287     /* Invalid transformation name */
#define cs_GPQ_NOXFRM   288     /* Transformation name is not that of an existing transformation */

#define cs_CSQ_ELPSCL   289     /* Invalid value for Ellipsoid Scale parameter */

	/* End coordinate system definition checker specific stuff. */

#define cs_DLM_CSIDX_FULL 301		/* Coordindate System index is full. */
#define cs_DLM_DTIDX_FULL 302		/* Datum conversion index is full. */
#define cs_A2F_RATIO	  303		/* Invalid ratio input format. */
#define cs_AGD66_RNG_F	  304		/* FATAL: Data point outside range
									   of AGD66/GDA94 conversion. */
#define cs_AGD66_RNG_W	  305		/* WARNING: Data point outside range
									   of AGD66/GDA94 conversion. */
#define cs_XYZ_ITR		  306		/* Geocentric inverse convergence failure. */
#define cs_MO_CNVRG		  307		/* The iterative inverse Molodensky
									   calculations failed to converge. */
#define cs_BW_CNVRG		  308		/* The iterative inverse Bursa/Wolf
									   calculations failed to converge. */
#define cs_7P_CNVRG		  309		/* The iterative inverse 7 Parameter
									   calculations failed to converge. */
#define cs_3P_CNVRG		  310		/* The iterative inverse 3 Parameter
									   calculations failed to converge. */
#define cs_6P_CNVRG		  311		/* The iterative inverse 6 Parameter
									   calculations failed to converge. */
#define cs_4P_CNVRG		  312		/* The iterative inverse 4 parameter
									   calculations failed to converge. */
#define cs_DTC_PATH		  313		/* Datum file specification format
									   error. */
#define cs_NAD_HPGN		  314		/* Datum file specification format
									   error. */
#define cs_NAD_EXT		  315		/* Datum file specification format
									   error. */
#define cs_NAD_LAS		  316		/* Datum file specification format
									   error. */
#define cs_NAD_LOS		  317		/* Datum file specification format
									   error. */
#define cs_HPGN_NAD		  318		/* Datum file specification format
									   error. */
#define cs_HPGN_EXTA	  319		/* Datum file specification format
									   error. */
#define cs_HPGN_EXTO	  320		/* Datum file specification format
									   error. */
#define cs_HPGN_EXTX	  321		/* Datum file specification format
									   error. */
#define cs_GHGT_GEO		  322		/* Datum file specification format
									   error. */
#define cs_GHGT_EXT		  323		/* Datum file specification format
									   error. */
#define cs_VCON_94		  324		/* Datum file specification format
									   error. */
#define cs_VCON_EXT		  325		/* Datum file specification format
									   error. */
#define cs_DTC_CATFILE	  326		/* Datum catalog file not found, open
									   failed. */
#define cs_DTC_EXT		  327		/* Datum catalog path entries without
									   extension or full path. */
#define cs_AGD84_RNG_F	  328		/* FATAL: Data point outside range
									   of AGD84/GDA94 conversion. */
#define cs_AGD84_RNG_W	  329		/* WARNING: Data point outside range
									   of AGD84/GDA94 conversion. */
#define cs_NZGD49_RNG_F	  330		/* FATAL: Data point outside range
									   of NZGD49/NZGD2K conversion. */
#define cs_NZGD49_RNG_W	  331		/* WARNING: Data point outside range
									   of NZGD49/NZGD2K conversion. */
#define cs_ATS77_RNG_F	  332		/* FATAL: Data point outside range
									   of ATS77/WGS84 conversion. */
#define cs_ATS77_RNG_W	  333		/* WARNING: Data point outside range
									   of ATS77/WGS84 conversion. */
#define cs_NAD_RNG_F      334		/* FATAL: Data point outside range
									   of NAD27/83 conversion. */
#define cs_NAD_RNG_W      335		/* WARNING: Data point outside range
									   of NAD27/83 conversion. */
#define cs_HARN_RNG_F     336		/* FATAL: Data point outside range
									   of HARN/83 conversion. */
#define cs_HARN_RNG_W     337		/* WARNING: Data point outside range
									   of HARN/83 conversion. */
#define cs_MGRS_LL        338		/* Invalid lat/long for MGRS conversion. */
#define cs_MGRS_XY        339		/* Invalid Utm/Ups presented for conversion */
#define cs_MGRS_ZONE      340       /* Invalid zone number for MGRS inversion */
#define cs_MGRS_UTM       341       /* Invalid UtmUps values */
#define cs_MGRS_STR1      342       /* MGRS string provided was inconsistent */
#define cs_MGRS_STR2      343       /* MGRS string provided was inconsistent */
#define cs_MGRS_STR3      344       /* MGRS string provided was inconsistent */
#define cs_MGRS_STR4      345       /* MGRS string provided was inconsistent */
#define cs_MGRS_STR5      346       /* MGRS string provided was inconsistent */
#define cs_MGRS_STR6      347       /* MGRS string provided was inconsistent */
#define cs_MGRS_STR7      348       /* MGRS string provided was inconsistent */
#define cs_MGRS_STR8      349       /* MGRS string provided was inconsistent */
#define cs_MGRS_STR9      350       /* MGRS string provided was inconsistent */
#define cs_MGRS_STRA      351       /* MGRS string provided was inconsistent */
#define cs_MGRS_STRB      352       /* MGRS string provided was inconsistent */
#define cs_MGRS_STRC      353       /* MGRS string provided was inconsistent */
#define cs_MGRS_NOSET     354       /* High Level MGRS interface not set up. */
#define cs_FLBK_NOSET     355       /* No fallback datum name specified. */
#define cs_FLBK_NTFND     356       /* Fallback datum not found. */
#define cs_FLBK_WRNGT     357       /* Fallback type is invalid. */
#define cs_NOT_NERTH      358       /* Conversion from Nerth to something else, or vice versa */
#define cs_NAD_RNG_A      359       /* Data outside coverage, fallback used successfully */
#define cs_AGD66_RNG_A    360       /* Data outside coverage, fallback used successfully */
#define cs_AGD84_RNG_A    361       /* Data outside coverage, fallback used successfully */
#define cs_NZGD49_RNG_A   362       /* Data outside coverage, fallback used successfully */
#define cs_ATS77_RNG_A    363       /* Data outside coverage, fallback used successfully */
#define cs_HARN_RNG_A     364       /* Data outside coverage, fallback used successfully */
#define cs_CSRS_RNG_F     365       /* FATAL: Data point outside range of CSRS conversion. */
#define cs_CSRS_RNG_W     366       /* WARNING: Data outside coverage, coordinates unshifted. */
#define cs_CSRS_RNG_A     367       /* WARNING: Data outside coverage, fallback used successfully. */
#define cs_DTC_SOFTMAX    368       /* Ten or more block errors (at different geographical locations) in the same datum. */
#define cs_MREG_RANGEF    369       /* Conversion outside of normal range of Multiple Regression formulas, no fallback specified. */
#define cs_MREG_CNVRG     370       /* The iterative inverse Multiple regression calculation failed to converge. */
#define cs_DTC_SOFTIGNR   371       /* Ten or more block errors found, ignoring the remainder. */
#define cs_GEOID_INIT     373       /* Geoid height object not initiailized. */
#define cs_TOKYO_ICNT     374       /* Japanese inverse datum calculation failed to converge. */
#define cs_TOKYO_RNG_F    375       /* FATAL: Data point outside range of Tokyo conversion. */
#define cs_TOKYO_RNG_W    376       /* WARNING: Data outside coverage, coordinates unshifted. */
#define cs_TOKYO_RNG_A    377       /* WARNING: Data outside coverage, fallback used successfully. */
#define cs_TOKYO_EXT	  378		/* Datum file specification file extension error. */
#define cs_RGF93_ICNT     379       /* RGF93 inverse datum calculation failed to converge. */
#define cs_RGF93_RNG_F    380       /* FATAL: Data point outside range of RGF93 conversion. */
#define cs_RGF93_RNG_W    381       /* WARNING: Data outside coverage, coordinates unshifted. */
#define cs_RGF93_RNG_A    382       /* Data outside coverage, fallback used successfully */
#define cs_CSRS27_RNG_F   383       /* FATAL: Data point outside range of CSRS27 conversion. */
#define cs_CSRS27_RNG_W   384       /* WARNING: Data outside coverage, coordinates unshifted. */
#define cs_CSRS27_RNG_A   385       /* WARNING: Data outside coverage, fallback used successfully. */
#define cs_ED50_RNG_F     386       /* FATAL: Data point outside range of ED50 conversion. */
#define cs_ED50_RNG_W     387       /* WARNING: Data outside coverage, coordinates unshifted. */
#define cs_ED50_RNG_A     388       /* WARNING: Data outside coverage, fallback used successfully. */
#define cs_UADD_TYPE      389		/* Invalid unit type on unit add. */
#define cs_UADD_DUP	      390		/* Unit to be added already exists. */		
#define cs_UADD_FULL      391		/* on unit add, table is full */
#define cs_UDEL_NONE      392		/* on unit del, not there. */
#define cs_N27A77_RNG_F   393       /* FATAL: Data point outside range of N27A77 conversion. */
#define cs_N27A77_RNG_W   394       /* WARNING: Data outside coverage, coordinates unshifted. */
#define cs_N27A77_RNG_A   395       /* WARNING: Data outside coverage, fallback used successfully. */
#define cs_THRD_LCK       396		/* Request to obtain lock on restricted resource failed. */
#define cs_GDC_DENSITY    397		/* Suspect density value in GDC file. */
#define cs_LLENUM_NOSU    398		/* Useful Range enumerator has not been setup. */

#define cs_WKT_WRNGTYP    399		/* Wrong type presented to be converted */
#define cs_WKT_NOELLIP    400		/* WKT SPHEROID child object was not present */
#define cs_WKT_NODATUM    401		/* WKT DATUM child object was not present */
#define cs_WKT_NOUNIT     402		/* WKT UNIT child object was not present */
#define cs_WKT_INVUNIT    403		/* WKT UNIT name was not one currently recognized */
#define cs_WKT_NOGEOCS    404		/* No GEOGCS child element was present */
#define cs_WKT_NOGUNIT    405		/* UNIT child element was not present in GEOGCS */
#define cs_WKT_INVGUNIT   406		/* GEOGCS unit name was not recognized */
#define cs_WKT_NOPROJ     407		/* Couldn't locate a PROJECTION element */
#define cs_WKT_INVPROJ    408		/* Value of the PROJECTION element was not recognized */
#define cs_WKT_GEOGCNT    409		/* Did not find two GEOGCS elements in a GEOGTRAN */
#define cs_WKT_NOSRCDT    410		/* Couldn't locate the source datum in a GEOGTRAN */
#define cs_WKT_NOMETH     411		/* Couldn't find a METHOD element in a GEOGTRAN */
#define cs_WKT_MTHERR     412		/* The PARAMETERS and METHOD of a GEOGTRAN were inconsistent */
#define cs_WKT_UKMETH     413		/* Unrecognized method in GEOGTRAN */
#define cs_WKT_WRNGTRG    414		/* Target of GEOGTRAN is not WGS84 */
#define cs_WKT_PRMSUPRT   415		/* Parameter arrangement not supported by CS-MAP. */
#define cs_WKT_UNITMAP    416       /* Don't know WKT equivalent to MSI unit. */
#define cs_WKT_PRJSUPRT   417		/* Projection variation arrangement not supported WKT */
#define cs_WKT_NODTMNM    418		/* Unamed datum specification */
#define cs_WKT_NODTMSPC   419		/* No TOWGS84 specification and name not found in dictionary. */
#define cs_WKT_NOLLBASE   420		/* Reference datum did not produce a geographic coordinate system. */
#define cs_WKT_NODTREF    421		/* WKT does not support direct ellipsoid references. */
#define cs_WLD_NOSOL      422		/* No solution to workd (.wld) file problem. */
#define cs_WKT_BADFORM    423		/* Poorly formed WKT structure. */
#define cs_WKT_DTMAP      424		/* Datum name mapping failed */
#define cs_WKT_FLAVOR     425		/* Couldn't determine flavor of provided WKT string */
#define cs_WKT_INCNSIST   426		/* Parameters provided to CS_cs2WktEx are inconsistent */

#define cs_DHDN_RNG_F     427       /* FATAL: Data point outside range of DHDN conversion. */
#define cs_DHDN_RNG_W     428       /* WARNING: Data outside coverage, coordinates unshifted. */
#define cs_DHDN_RNG_A     429       /* WARNING: Data outside coverage, fallback used successfully. */

#define cs_NMMAP_INIT     430       /* Name mapper has not been initialized. */
#define cs_NMMAP_FAIL1    431       /* Name mapper initialization failed, no file */
#define cs_NMMAP_FAIL2    432       /* Name mapper initialization failed, file format */
#define cs_NMMAP_NONAME   433       /* No record of given name with the specified flavor */
#define cs_NMMAP_NONBR    434       /* No record of given number with the specified flavor */

#define cs_INV_ARG6       435		/* Pointer argument 6 is invalid. */
#define cs_INV_ARG7       436		/* Pointer argument 7 is invalid. */
#define cs_INV_ARG8       437		/* Pointer argument 8 is invalid. */
#define cs_INV_ARG9       438		/* Pointer argument 9 is invalid. */

#define cs_SYS34_NOSRC    439		/* Danish System 34 source code unavailable.  */

#define cs_CHENYX_RNG_F   440       /* FATAL: Data point outside range of CHENYX conversion. */
#define cs_CHENYX_RNG_W   441       /* WARNING: Data outside CHENYX coverage, coordinates unshifted. */
#define cs_CHENYX_RNG_A   442       /* WARNING: Data outside CHENYX coverage, fallback used successfully. */
#define cs_MGRS_GRDSQR    443       /* Invalid grid square position provided */

#define cs_GP_BAD_MAGIC   444		/* Magic number (first four bytes) of Geodetic Path
									   Dictionary indicate that the file is not a Geodetic
									   Path Dictionary. */
#define cs_GPDICT         445		/* Open of the Geodetic Path Dictionary failed. */
#define cs_GP_PROT        446		/* Attempt to change a distribution Geodetic Path dictionary entry. */
#define cs_GP_UPROT       447		/* Attempt to change a protected user Geodetic Path dictionary entry. */
#define cs_GP_NOT_FND     448		/* Geodetic path not found. */
#define cs_GP_NOPATH      449		/* Geodetic path not found. */
#define cs_GEOPATH_DUP    450		/* Duplicate entries in the Geodetic Path dictionary */

#define cs_GX_BAD_MAGIC   451		/* Magic number (first four bytes) of Geodetic Transformation
									   Dictionary indicate that the file is not a Geodetic
									   Path Dictionary. */
#define cs_GXDICT         452		/* Open of the Geodetic Transformation Dictionary failed. */
#define cs_GX_PROT        453		/* Attempt to change a distribution Geodetic Transformation dictionary entry. */
#define cs_GX_UPROT       454		/* Attempt to change a protected user Geodetic Transformation dictionary entry. */
#define cs_GX_NOT_FND     455		/* Geodetic transformation not found. */
#define cs_NULLX_CNVRG	  456		/* The iterative inverse Null Transformation
									   calculation failed to converge. */
#define cs_GEOCT_CNVRG	  457		/* The iterative inverse Geocentric Transformation
									   calculation failed to converge. */
#define cs_MULRG_CNVRG	  458		/* The iterative inverse multiple regression
									   calculations failed to converge. */
#define cs_UNKWN_DTCMTH   459		/* Unknown datum transformation method encountered in the
									   Geodetic Transformation dictionary. */
#define cs_GRD_RNG_FLBK	  460		/* Out of coverage of grid files, fallback used. */
#define cs_GRD_RNG_WRN	  461		/* Out of coverage of grid files, warning only. */
#define cs_GX_TOOMANY	  462		/* Too many transformations required to do datum transformation. */
#define cs_GEOXFRM_DUP	  463		/* Duplicate geodetic transformation definitions. */
#define cs_DT_NOPATH	  464		/* Couldn't locate or construct a path between datums. */
#define cs_ATS77_INV	  465		/* Attempt to perform an inverse calculation on the ATS77 datum. */

#define cs_CT_NOT_FND	  466		/* Category not found. */
#define cs_CT_CS_NOT_IN	  467		/* CS name not contained in category. */
#define cs_CT_PROT		  468		/* CT is (partially) protected. */
#define cs_CT_CS_ADD_DUP  469		/* CT already contains a CS with that name. */
#define cs_CT_DICT		  470		/* CT dictionary open failed. */

#define cs_DICT_INV		  471		/* Dictionary contains at least 1 invalid entry */
#define cs_DICT_DUP_IDS   472		/* Dictionary contains duplicate IDs */

#define cs_ENV_TOOLONG    473		/* String presented for environmental subsitution is too long. */
#define cs_ENV_NOVAR      474		/* A referenced environmental variable did not exist. */
#define cs_ENV_FORMAT     475		/* The format of the string presented fro environmental
									   variable subsitution is improperly formatted. */


#define cs_ERROR_MAX	  cs_DICT_DUP_IDS

/*
	The following casts are used to eliminate warnings from
	ANSI compilers.  I don't understand why they are necessary,
`	but it appears that they are.  Perhaps this is because
	we use a union in the cs_Csprm_ structure and pass around
	pointers to specific elements in the union.

	Note, the defines do not include the surrounding
	parenthesis so they look something like a cast in
	the code.
*/

#define cs_CS2LL_CAST int(*)(Const void *,double *,Const double *)
#define cs_LL2CS_CAST int(*)(Const void *,double *,Const double *)
#define cs_SCALE_CAST double(*)(Const void *,Const double *)
#define cs_CNVRG_CAST double(*)(Const void *,Const double *)
#define cs_SCALK_CAST double(*)(Const void *,Const double *)
#define cs_SCALH_CAST double(*)(Const void *,Const double *)
#define cs_LLCHK_CAST int(*)(Const void *,int,Const double [][3])
#define cs_XYCHK_CAST int(*)(Const void *,int,Const double [][3])

/*
	For sorting and searching comparison functions.
*/

#if !defined (CMPFUNC_CAST)
#	define CMPFUNC_CAST int(*)(Const void *,Const void *)
#endif

/*
	Now for the function prototypes.
*/

#ifdef __cplusplus
extern "C" {
#endif

int			EXP_LVL9	CS_access (Const char *path,int mode);
void		EXP_LVL1	CS_agd66Name (Const char *newName);
void		EXP_LVL1	CS_agd84Name (Const char *newName);
double		EXP_LVL5	CS_adj90  (double az_in);
double		EXP_LVL5	CS_adj180 (double az_in);
double		EXP_LVL5	CS_adj270 (double az_in);
double		EXP_LVL5	CS_adj1pi (double az_in);
double		EXP_LVL5	CS_adj2pi (double az_in);
double		EXP_LVL5	CS_adj2piI (double az_in);
int			EXP_LVL5	CS_adjll (double ll [2]);
int			EXP_LVL1	CS_altdr (Const char *alt_dir);
int			EXP_LVL1	CS_usrdr (Const char *usr_dir);
double		EXP_LVL1	CS_ansiAtof (Const char *string);
void		EXP_LVL1	CS_ats77Name (Const char *newName);
long32_t	EXP_LVL1	CS_atof (double *result,Const char *value);
char*		EXP_LVL1	CS_audflt (Const char *dflt_au);
int			EXP_LVL1	CS_azddll (double e_rad,double e_sq,Const double from_ll [2],double az,double dd,double to_ll [2]);
double		EXP_LVL1	CS_azsphr (Const double ll0 [2],Const double ll1 [2]);

int			EXP_LVL7	CS_bins (csFILE *strm,long32_t start,long32_t eof,int rs,Const void *rec,int (*comp)(const void *elem1,const void *elem2));
void*		EXP_LVL9	CS_bsearch (Const void *key,Const void *base,size_t num,size_t width,int (*compare )(const void *elem1,const void *elem2));
int			EXP_LVL5	CS_bswap (void *rec,Const char *frmt);

double		EXP_LVL1	CS_cnvrg (Const char *cs_nam,Const double ll [3]);
int			EXP_LVL1	CS_cnvrt (Const char *src_cs,Const char *dst_cs,double coord [3]);
int			EXP_LVL1	CS_cnvrt3D (Const char *src_cs,Const char *dst_cs,double coord [3]);
const char*	EXP_LVL3	CS_countyEnum (int stateFips,int index);
unsigned short EXP_LVL9 CS_crc16 (unsigned short crc16,unsigned char *cp,int count);

int			EXP_LVL3	CS_cs2ll (Const struct cs_Csprm_ *csprm,double ll [3],Const double xy [3]);
int			EXP_LVL3	CS_cs3ll (Const struct cs_Csprm_ *csprm,double ll [3],Const double xy [3]);
int			EXP_LVL3	CS_cschk (Const struct cs_Csdef_ *cs_def,unsigned short chk_flg,int err_list [],int list_sz);
void		EXP_LVL7	CS_csDictCls (csFILE* stream);
int			EXP_LVL7	CS_cscmp (Const struct cs_Csdef_ *pp,Const struct cs_Csdef_ *qq);
double		EXP_LVL3	CS_cscnv (Const struct cs_Csprm_ *csprm,Const double ll [3]);
struct cs_Csdef_* EXP_LVL3	CS_csdef (Const char *cs_nam);
struct cs_Csdef_ * EXP_LVL3 CS_csdef2 (Const char *cs_nam, char* pszDirPath);
int			EXP_LVL3	CS_csdefAll (struct cs_Csdef_ **pDefArray[]);
int			EXP_LVL1	CS_csDefCmp (Const struct cs_Csdef_ *original,Const struct cs_Csdef_ *revised,char* message,size_t messageSize);
int			EXP_LVL1	CS_csDefCmpEx (double* qValuePtr,Const struct cs_Csdef_ *original,Const struct cs_Csdef_ *revised,char* message,size_t msgSize);
int			EXP_LVL3	CS_csdel (struct cs_Csdef_ *csdef);
int			EXP_LVL5	CS_csDiff (FILE *rptStrm,struct cs_Csdef_ *was,struct cs_Csdef_ *is);
int			EXP_LVL1	CS_csEnum (int index,char *key_name,int size);
int 		EXP_LVL1 	CS_csEnumByGroup (int index,Const char *grp_name,struct cs_Csgrplst_ *cs_descr);
void		EXP_LVL1	CS_csfnm (Const char *new_name);
int			EXP_LVL1	CS_csGrpEnum (int index,char *grp_name,int name_sz,char *grp_dscr,int dscr_sz);
void		EXP_LVL3	CS_csgrpf (struct cs_Csgrplst_ *grp_list);
int			EXP_LVL3	CS_csgrp (Const char *grp_key,struct cs_Csgrplst_ **grp_list);
int			EXP_LVL1	CS_csIsValid (Const char *key_name);
struct cs_Csprm_* EXP_LVL3	CS_cslcl (Const double min_ll [2],Const double max_ll [2],Const char *units,Const struct cs_Datum_ *datum,double map_scl);
struct cs_Csprm_* EXP_LVL3	CS_csloc (Const char *cs_nam);

csFILE *	EXP_LVL3	CS_csopn (Const char *mode);
int			EXP_LVL1	CS_csRangeEnum (int index,char *key_name,int size);
int			EXP_LVL1	CS_csRangeEnumSetup (double longitude,double latitude);
int			EXP_LVL3	CS_csrd (csFILE *strm,struct cs_Csdef_ *cs_def,int *crypt);
int			EXP_LVL3	CS_csrup (Const char *distrb,Const char *bkupnm);
double		EXP_LVL3	CS_cssch (Const struct cs_Csprm_ *csprm,Const double ll [2]);
double		EXP_LVL3	CS_cssck (Const struct cs_Csprm_ *csprm,Const double ll [2]);
double		EXP_LVL3	CS_csscl (Const struct cs_Csprm_ *csprm,Const double ll [2]);
int			EXP_LVL3	CS_csupd (struct cs_Csdef_ *csdef,int crypt);
int			EXP_LVL3	CS_cswr (csFILE *strm,Const struct cs_Csdef_ *cs_def,int crypt);

int			EXP_LVL1	CS_cmpDbls (double first,double second);
int			EXP_LVL3	CS_defCmpPrjPrm (struct cs_Prjtab_* pp,int prmNbr,double orgValue,double revValue,char *message,size_t messageSize);
int			EXP_LVL3	CS_defCmpPrjPrmEx (struct cs_Prjtab_* pp,int prmNbr,double orgValue,double revValue,char *message,size_t messageSize,double* qValue);
long32_t	EXP_LVL5	CS_degToSec (double degrees);
double		EXP_LVL3	CS_deltaLongitude (double baseMeridian,double relativeLongitude);
void		EXP_LVL3	CS_dtcls (struct cs_Dtcprm_ *dtc_ptr);
int			EXP_LVL7	CS_dtcmp (Const struct cs_Dtdef_ *pp,Const struct cs_Dtdef_ *qq);
struct cs_Dtcprm_* EXP_LVL3	CS_dtcsu (Const struct cs_Csprm_ *src_cs,Const struct cs_Csprm_ *dest_cs,int dat_erf,int blk_erf);
int			EXP_LVL3	CS_dtcvt (struct cs_Dtcprm_ *dtc_ptr,Const double ll_in [2],double ll_out [2]);
int			EXP_LVL3	CS_dtcvt3D (struct cs_Dtcprm_ *dtc_ptr,Const double ll_in [3],double ll_out [3]);
struct cs_Dtdef_* EXP_LVL3	CS_dtdef (Const char *dat_nam);
struct cs_Dtdef_ * EXP_LVL3	CS_dtdef2 (Const char *dat_nam, char* pszDirPath);
int			EXP_LVL3	CS_dtdefAll	(struct cs_Dtdef_ **pDefArray[]);
int			EXP_LVL3	CS_dtDefCmp (Const struct cs_Dtdef_ *original,Const struct cs_Dtdef_ *revised,char* message,size_t messageSize);
int			EXP_LVL3	CS_dtDefCmpEx (double *qValuePtr,Const struct cs_Dtdef_ *original,Const struct cs_Dtdef_ *revised,char* message,size_t msgSize);
int			EXP_LVL3	CS_dtdel (struct cs_Dtdef_ *dtdef);
char*		EXP_LVL1	CS_dtdflt (Const char *dflt_dt);
void		EXP_LVL7	CS_dtDictCls (csFILE* stream);
int			EXP_LVL5	CS_dtDiff (FILE *rptStrm,struct cs_Dtdef_ *was,struct cs_Dtdef_ *is);
int			EXP_LVL1	CS_dtEnum (int index,char *key_name,int size);
void		EXP_LVL1	CS_dtfnm (Const char *new_name);
int			EXP_LVL1	CS_dtIsValid (Const char *key_name);
struct cs_Datum_* EXP_LVL5	CS_dtloc (Const char *dat_nam);
csFILE *	EXP_LVL3	CS_dtopn (Const char *mode);
int			EXP_LVL3	CS_dtrd (csFILE *strm,struct cs_Dtdef_ *dt_def,int *crypt);
int			EXP_LVL3	CS_dtrup (Const char *distrb,Const char *bkupnm);
void		EXP_LVL3	CS_dtTrail (struct cs_Dtcprm_ *dtc_ptr,char* auditTrail,int size,Const double ll_in [3]);
int			EXP_LVL3	CS_dtupd (struct cs_Dtdef_ *dtdef,int crypt);
int			EXP_LVL3	CS_dtwr (csFILE *strm,Const struct cs_Dtdef_ *dt_def,int crypt);
int			EXP_LVL3	CS_dynutm (struct cs_Csprm_ *csprm,int zone);

Const char*	EXP_LVL3	CS_ecvt (double value,int count,int *dec,int *sign);
int			EXP_LVL7	CS_elcmp (Const struct cs_Eldef_ *pp,Const struct cs_Eldef_ *qq);
struct cs_Eldef_* EXP_LVL3     CS_eldef (Const char *el_nam);
struct cs_Eldef_* EXP_LVL3     CS_eldef2 (Const char *el_nam, char* pszFileDirPath);
int			EXP_LVL3	CS_eldefAll (struct cs_Eldef_ **pDefArray[]);
int			EXP_LVL3	CS_elDefCmp (Const struct cs_Eldef_ *original,Const struct cs_Eldef_ *revised,char* message,size_t messageSize);
int			EXP_LVL3	CS_elDefCmpEx (double* qValuePtr,Const struct cs_Eldef_ *original,Const struct cs_Eldef_ *revised,char* message,size_t msgSize);
int			EXP_LVL3	CS_eldel (struct cs_Eldef_ *eldef);
char*		EXP_LVL1	CS_eldflt (Const char *dflt_el);
void		EXP_LVL7	CS_elDictCls (csFILE* stream);
int			EXP_LVL5	CS_elDiff (FILE *rptStrm,struct cs_Eldef_ *was,struct cs_Eldef_ *is);
int			EXP_LVL1	CS_elEnum (int index,char *key_name,int size);
void		EXP_LVL1	CS_elfnm (Const char *new_name);
int			EXP_LVL1	CS_elIsValid (Const char *key_name);
csFILE *	EXP_LVL3	CS_elopn (Const char *mode);
int			EXP_LVL3	CS_elrd (csFILE *strm,struct cs_Eldef_ *el_def,int *crypt);
int			EXP_LVL3	CS_elrup (Const char *distrb,Const char *bkupnm);
int			EXP_LVL3	CS_elupd (struct cs_Eldef_ *eldef,int crypt);
int			EXP_LVL3	CS_elwr (csFILE *strm,Const struct cs_Eldef_ *el_def,int crypt);
int			EXP_LVL3	CS_envsub (char* stringBufr,size_t bufrSize);
int			EXP_LVL3	CS_envsubWc (wchar_t* stringBufr,size_t bufrSize);	/* bufrSize === # of characters */
void		EXP_LVL3	CS_erpt (int err_num);
void		EXP_LVL1	CS_errmsg (char *user_bufr,int buf_size);

void		EXP_LVL1	CS_fast (int fast);
void		EXP_LVL3	CS_fillIn (struct cs_Csdef_ *cs_def);
cs_Time_	EXP_LVL7	CS_fileModTime (Const char *filePath);
void		EXP_LVL1	CS_free (void *ptr);
long32_t	EXP_LVL1	CS_ftoa (char *bufr,int size,double value,long32_t frmt);

int			EXP_LVL3	CS_gdcDisable (enum cs_GdcCatalogs ident);
Const char *EXP_LVL1	CS_gdcEnum (int index,int *ident);
enum cs_GdcCatalogs EXP_LVL3 CS_gdcGetIdent (Const char *catalogName);
Const char *EXP_LVL3	CS_gdcGetName (enum cs_GdcCatalogs ident);
int			EXP_LVL3	CS_gdcSetName (enum cs_GdcCatalogs ident,Const char *newName);
Const struct cs_GdcCatalogTable_* EXP_LVL3 CS_gdcTable (enum cs_GdcCatalogs ident);
int			EXP_LVL1	CS_geoctrSetUp (const char* ellipsoid);
int			EXP_LVL1	CS_geoctrGetLlh (double llh [3],double xyz [3]);
int			EXP_LVL1	CS_geoctrGetXyz (double xyz [3],double llh [3]);

int			EXP_LVL1	CS_getcs (Const char *cs_nam,struct cs_Csdef_ *cs_ptr);
int			EXP_LVL3	CS_getCountyFips (int stateFips,Const char* countyName);
int			EXP_LVL3	CS_getCountyNad27 (int state,int county);
int			EXP_LVL3	CS_getCountyNad83 (int state,int county);
double		EXP_LVL1	CS_getCurvatureAt (Const char *csKeyName,double lat);
int			EXP_LVL1	CS_getDataDirectory (char *data_dir,int dir_sz);
int			EXP_LVL1	CS_getDatumOf (Const char *csKeyName,char *datumName,int size);
int			EXP_LVL1	CS_getDescriptionOf (Const char *csKeyName,char *description,int size);
int			EXP_LVL1	CS_getdt (Const char *dt_nam,struct cs_Dtdef_ *dt_ptr);
int			EXP_LVL1	CS_getel (Const char *el_nam,struct cs_Eldef_ *el_ptr);
int			EXP_LVL1	CS_getEllipsoidOf (Const char *csKeyName,char *ellipsoidName,int size);
int			EXP_LVL1	CS_getElValues (Const char *el_name,double *radius,double *e_Sq);
char *		EXP_LVL9	CS_getenv (Const char *varname);

int			EXP_LVL1	CS_getReferenceOf (Const char *csKeyName,char *reference,int size);
int			EXP_LVL1	CS_getSourceOf (Const char *csKeyName,char *source,int size);
int			EXP_LVL3	CS_getStateFips (Const char* stateName);
int			EXP_LVL1	CS_getUnitsOf (Const char *csKeyName,char *unitName,int size);

int			EXP_LVL1	CS_gpchk (Const struct cs_GeodeticPath_ *gpPath,unsigned short gpChkFlg,int err_list [],int list_sz);
int			EXP_LVL7	CS_gpcmp (Const struct cs_GeodeticPath_ *pp,Const struct cs_GeodeticPath_ *qq);
struct cs_GeodeticPath_*
			EXP_LVL3	CS_gpdef (Const char *pathName);
struct cs_GeodeticPath_ *
			EXP_LVL3	CS_gpdef2 (Const char *pathName, char* pszDirPath);
struct cs_GeodeticPath_*
			EXP_LVL3	CS_gpdefEx (int *direction,Const char *srcDatum,Const char *trgDatum);
int			EXP_LVL3	CS_gpdefAll (struct cs_GeodeticPath_ **pDefArray[]);
int			EXP_LVL3	CS_gpdel (struct cs_GeodeticPath_ *gpdef);
void		EXP_LVL1	CS_gpfnm (Const char *new_name);
csFILE *	EXP_LVL3	CS_gpopn (Const char *mode);
int			EXP_LVL3	CS_gprd (csFILE *strm,struct cs_GeodeticPath_ *gp_def);
int			EXP_LVL3	CS_gpupd (struct cs_GeodeticPath_ *gp_def);
int			EXP_LVL3	CS_gpwr (csFILE *strm,Const struct cs_GeodeticPath_ *gp_def);

int			EXP_LVL1	CS_gxchk (Const struct cs_GeodeticTransform_ *gxXform,unsigned short gxChkFlg,int err_list [],int list_sz);
int			EXP_LVL7	CS_gxcmp (Const struct cs_GeodeticTransform_ *pp,Const struct cs_GeodeticTransform_ *qq);
struct cs_GeodeticTransform_*
			EXP_LVL3	CS_gxdef (Const char *xfrmName);
struct cs_GeodeticTransform_*
			EXP_LVL3	CS_gxdef2 (Const char *xfrmName, char* pszDirPath);
struct cs_GeodeticTransform_*
			EXP_LVL3	CS_gxdefEx (Const char *srcDatum,Const char *trgDatum);
int			EXP_LVL3	CS_gxdefAll (struct cs_GeodeticTransform_ **pDefArray[]);
int			EXP_LVL3	CS_gxdel (struct cs_GeodeticTransform_ *gpdef);
void		EXP_LVL1	CS_gxfnm (Const char *new_name);
csFILE *	EXP_LVL3	CS_gxopn (Const char *mode);
int			EXP_LVL3	CS_gxrd (csFILE *strm,struct cs_GeodeticTransform_ *gp_def);
void		EXP_LVL3	CS_gxsep (struct cs_GeodeticTransform_* gx_def);
int			EXP_LVL5	CS_gxswp (struct cs_GeodeticTransform_* gx_def,int writeFlag);
int			EXP_LVL5	CS_gxswpRd (struct cs_GeodeticTransform_* gx_def);
int			EXP_LVL5	CS_gxswpWr (struct cs_GeodeticTransform_* gx_def);
int			EXP_LVL3	CS_gxupd (struct cs_GeodeticTransform_ *gp_def);
int			EXP_LVL3	CS_gxwr (csFILE *strm,Const struct cs_GeodeticTransform_ *gp_def);

void		EXP_LVL5	CS_iicpy (const struct cs_Cmplx_ *aa,struct cs_Cmplx_ *bb);
void		EXP_LVL5	CS_iicrt (struct cs_Cmplx_ *aa,double rVal,double iVal);
void		EXP_LVL5	CS_iiadd (Const struct cs_Cmplx_ *aa,Const struct cs_Cmplx_ *bb,struct cs_Cmplx_ *cc);
void		EXP_LVL5	CS_iisub (Const struct cs_Cmplx_ *aa,Const struct cs_Cmplx_ *bb,struct cs_Cmplx_ *cc);
void		EXP_LVL5	CS_iikmul (Const struct cs_Cmplx_ *aa,double kk,struct cs_Cmplx_ *cc);
void		EXP_LVL5	CS_iimul (Const struct cs_Cmplx_ *aa,Const struct cs_Cmplx_ *bb,struct cs_Cmplx_ *cc);
void		EXP_LVL5	CS_iidiv (Const struct cs_Cmplx_ *aa,Const struct cs_Cmplx_ *bb,struct cs_Cmplx_ *cc);
double		EXP_LVL5	CS_iiabs (Const struct cs_Cmplx_ *aa);
void		EXP_LVL5	CS_iicnj (Const struct cs_Cmplx_ *aa,struct cs_Cmplx_ *cc);
void		EXP_LVL5	CS_iisrs (Const struct cs_Cmplx_ *xy,
								  Const struct cs_Cmplx_ *AB,
								  int nn,
								  struct cs_Cmplx_ *new_xy);
void		EXP_LVL5	CS_iisrs1 (Const struct cs_Cmplx_ *xy,
								   Const struct cs_Cmplx_ *AB,
								   int nn,
								   struct cs_Cmplx_ *new_xy);
void		EXP_LVL5	CS_iisrs0 (Const struct cs_Cmplx_ *xy,
								   Const struct cs_Cmplx_ *AB,
								   int nn,
								   struct cs_Cmplx_ *new_xy);
void		EXP_LVL1	CS_init (int keepers);
int			EXP_LVL7	CS_ips (csFILE *strm,short rs,long32_t eof,int (*comp)(Const void *pp,Const void *qq));
int			EXP_LVL5	CS_isBigEndian (void);
int			EXP_LVL1	CS_isgeo (Const char *csys);
int			EXP_LVL1	CS_isHlpAvailable (void);
int			EXP_LVL5	CS_isLittleEndian (void);
int			EXP_LVL1	CS_isCsPrmReentrant (Const struct cs_Csprm_ *prjConversion);
int			EXP_LVL1	CS_isCsReentrant (Const char *csys);
int			EXP_LVL1	CS_isDtXfrmReentrant (Const struct cs_Dtcprm_ *dtc_ptr);
int			EXP_LVL1	CS_isGxDefReentrant (Const struct cs_GeodeticTransform_ *gxDef);
int			EXP_LVL1	CS_isGxfrmReentrant (Const struct cs_GxXform_ *gxXform);

int			EXP_LVL3	CS_isalpha (int chr);
int			EXP_LVL3	CS_isupper (int chr);
int			EXP_LVL3	CS_islower (int chr);
int			EXP_LVL3	CS_isdigit (int chr);
int			EXP_LVL3	CS_isnan (double xxx);
int			EXP_LVL3	CS_isxdigit (int chr);
int			EXP_LVL3	CS_isspace (int chr);
int			EXP_LVL3	CS_isalnum (int chr);

char*		EXP_LVL7	CS_lget (char *str,Const char *fld,int size,char fill);
int			EXP_LVL3	CS_ll2cs (Const struct cs_Csprm_ *csprm,double xy [3],Const double ll [3]);
int			EXP_LVL3	CS_ll3cs (Const struct cs_Csprm_ *csprm,double xy [3],Const double ll [3]);
double		EXP_LVL1	CS_llazdd (double e_rad,double e_sq,Const double ll_from [2],Const double ll_to [2],double *dist);
int			EXP_LVL3	CS_llchk (Const struct cs_Csprm_ *csprm,int cnt,Const double pnts [][3]);
int			EXP_LVL1	CS_llFromMgrs (double latLng [2],const char* mgrsString);
void		EXP_LVL7	CS_lput (char *fld,Const char *str,int size,char fill);
char*		EXP_LVL1	CS_ludflt (Const char *dflt_lu);

void*		EXP_LVL1	CS_malc (size_t blk_size);
int			EXP_LVL1	CS_mgrsSetUp (const char* ellipsoid,short bessel);
int			EXP_LVL1	CS_mgrsFromLl (char *result,double latLng [2],int prec);
Const char*	EXP_LVL3	CS_mifcs (Const struct cs_Csdef_ *cs_def);
int			EXP_LVL1	CS_msiCs2Wkt (char *wktBufr,size_t bufrSize,Const char* msiCsName,enum ErcWktFlavor flavor);
int			EXP_LVL5	CS_nampp (char *name);
int			EXP_LVL5	CS_nampp64 (char *name);

int			EXP_LVL1	CS_prchk (short prot_val);
int			EXP_LVL1	CS_prjEnum (int index,ulong32_t *prj_flags,char *prj_keynm,int keynm_sz,
											  char *prj_descr,
											  int descr_sz);
int			EXP_LVL3	CS_prjprm (struct cs_Prjprm_ *result,unsigned short prj_code,int parm_nbr);
double		EXP_LVL3	CS_prmValue (Const struct cs_Csdef_ *csDefPtr,int parm_nbr);
int			EXP_LVL1	CS_putcs (Const struct cs_Csdef_ *csdef,int crypt);
int			EXP_LVL1	CS_putdt (Const struct cs_Dtdef_ *dtdef,int crypt);
int			EXP_LVL1	CS_putel (Const struct cs_Eldef_ *eldef,int crypt);

int			EXP_LVL1	CS_quadEnum (int index,char *descr,int size,int *ident);
void		EXP_LVL9	CS_quadF (double xy [2],double xx,double yy,double x_off,double y_off,short quad);
void		EXP_LVL9	CS_quadI (double *xx,double *yy,Const double xy [2],double x_off,double y_off,short quad);
void		EXP_LVL9	CS_quadMM (double min_xy [2],double max_xy [2],double x_off,double y_off,short quad);

void*		EXP_LVL1	CS_ralc (void *ptr,size_t blk_size);
void		EXP_LVL1	CS_recvr (void);
int			EXP_LVL9	CS_remove (Const char *path);
void        EXP_LVL9    CS_removeRedundantWhiteSpace (char *string);
int			EXP_LVL9	CS_rename (Const char *prev,Const char *current);

double		EXP_LVL1	CS_scale (Const char *cs_nam,double ll [2]);
double		EXP_LVL1	CS_scalh (Const char *cs_nam,double ll [2]);
double		EXP_LVL1	CS_scalk (Const char *cs_nam,double ll [2]);
int			EXP_LVL1	CS_setHelpPath (const char *path);
int			EXP_LVL9	CS_setvbuf (csFILE *stream,char *buffer,int mode,size_t size );
unsigned    EXP_LVL9    CS_spaceParse (char *lineBuffer,char *ptrs [],unsigned maxPtrs);
int			EXP_LVL1	CS_spZoneNbrMap (char *zoneNbr,int is83);

const char*	EXP_LVL3	CS_stateEnum (int index);
char *		EXP_LVL3	CS_stncat (char *dest,Const char *source,int count);
char *		EXP_LVL3	CS_stcpy (char *dest,Const char *source);
char *		EXP_LVL3	CS_stncp (char *dest,Const char *source,int count);
int			EXP_LVL3	CS_stricmp (Const char *cp1,Const char *cp2);
Const char*	EXP_LVL3	CS_stristr (Const char *str1,Const char *str2);
int			EXP_LVL9	CS_strnicmp (Const char *cp1,Const char *cp2,size_t count);
char*		EXP_LVL3	CS_strrchr (Const char *cPtr, int chr);
double		EXP_LVL9	CS_strtod (Const char *nptr,char **endptr);
long32_t	EXP_LVL9	CS_strtol (Const char *nptr,char **endptr,int base);
ulong32_t	EXP_LVL9	CS_strtoul (Const char *nptr,char **endptr,int base);
int			EXP_LVL7	CS_swpal (void (*prog)(Const char *name));
char*		EXP_LVL7	CS_swpfl (Const char *file_name);

cs_Time_	EXP_LVL9	CS_time (cs_Time_ *timer);
int			EXP_LVL7	CS_tmpfn (char *tmp_fnm);
int			EXP_LVL3	CS_tolower (int chr);
int			EXP_LVL3	CS_toupper (int chr);
Const void*	EXP_LVL9	CS_tpars (char **pntr,Const void *table,int tab_size);
int			EXP_LVL7	CS_trim (char *string);
int			EXP_LVL7	CS_trimWc (wchar_t *string);

double		EXP_LVL9	CS_un2d (Const char *name);
int			EXP_LVL1	CS_unEnum (int index,int type,char *un_name,int un_size);
int			EXP_LVL1	CS_unEnumPlural (int index,int type,char *un_name,int un_size);
int			EXP_LVL1	CS_unEnumSystem (int index,int type);
int			EXP_LVL3	CS_unitAdd (struct cs_Unittab_ *unitPtr);
double		EXP_LVL1	CS_unitlu (short type,Const char *name);
Const char*	EXP_LVL3	CS_unitluByFactor (short type,double factor);
int			EXP_LVL3	CS_unitDel (short type,Const char *name);

void		EXP_LVL1	CS_usrDtfnm (Const char *new_name);
void		EXP_LVL1	CS_usrElfnm (Const char *new_name);
int			EXP_LVL3	CS_utmzon (double lng);

int			EXP_LVL1	CS_viaEnum (int index,char *descr,int size,int *ident);

int			EXP_LVL3	CS_wcsicmp (Const wchar_t *cp1,Const wchar_t *cp2);
int			EXP_LVL9	CS_wktCsDefFunc (struct cs_Csdef_* csDefPtr,Const char* wktString);
int			EXP_LVL9	CS_wktDtDefFunc (struct cs_Dtdef_* dtDefPtr,Const char* dtKeyName);
int			EXP_LVL9	CS_wktElDefFunc (struct cs_Eldef_* elDefPtr,Const char* elKeyName);

int			EXP_LVL3	CS_xychk (Const struct cs_Csprm_ *csprm,int cnt,Const double pnts [][3]);

Const struct cs_Zone_* EXP_LVL9	CS_znlocF (Const struct cs_Zone_ zones [8],
										   int zone_cnt,
										   double lng,
										   double lat);
Const struct cs_Zone_* EXP_LVL9	CS_znlocI (Const struct cs_Zone_ zones [8],
										   int zone_cnt,
										   double xx,
										   double yy);
int			EXP_LVL9	CS_zones (Const struct cs_Csdef_ *csdef,struct cs_Zone_ *zp);

double		EXP_LVL9	CSalberC (Const struct cs_Alber_ *alber,Const double ll [2]);
int			EXP_LVL9	CSalberF (Const struct cs_Alber_ *alber,double xy [2],Const double ll [2]);
double		EXP_LVL9	CSalberH (Const struct cs_Alber_ *alber,Const double ll [2]);
int			EXP_LVL9	CSalberI (Const struct cs_Alber_ *alber,double ll [2],Const double xy [2]);
double		EXP_LVL9	CSalberK (Const struct cs_Alber_ *alber,Const double ll [2]);
int			EXP_LVL9	CSalberL (Const struct cs_Alber_ *alber,int cnt,Const double pnts [][3]);
int			EXP_LVL9	CSalberQ (Const struct cs_Csdef_ *csdef,unsigned short prj_code,int err_list [],int list_sz);
void		EXP_LVL9	CSalberS (struct cs_Csprm_ *csprm);
int			EXP_LVL9	CSalberX (Const struct cs_Alber_ *alber,int cnt,Const double pnts [][3]);

void		EXP_LVL5	CSasciiToXml (char *xml,const char *ascii);

long32_t	EXP_LVL7	CSatof (double *result,Const char *value,char rdxChr,char sepChr,char ratioChr);

double		EXP_LVL9	CSazmeaqq (Const struct cs_Azmea_ *azmea,double sin_lat,double *sin_sq);
double		EXP_LVL9	CSazmeaC (Const struct cs_Azmea_ *azmea,Const double ll [2]);
int			EXP_LVL9	CSazmeaF (Const struct cs_Azmea_ *azmea,double xy [2],Const double ll [2]);
double		EXP_LVL9	CSazmeaH (Const struct cs_Azmea_ *azmea,Const double ll [2]);
int			EXP_LVL9	CSazmeaI (Const struct cs_Azmea_ *azmea,double ll [2],Const double xy [2]);
double		EXP_LVL9	CSazmeaK (Const struct cs_Azmea_ *azmea,Const double ll [2]);
int			EXP_LVL9	CSazmeaL (Const struct cs_Azmea_ *azmea,int cnt,Const double pnts [][3]);
int			EXP_LVL9	CSazmeaQ (Const struct cs_Csdef_ *csdef,unsigned short prj_code,int err_list [],int list_sz);
void		EXP_LVL9	CSazmeaS (struct cs_Csprm_ *csprm);
int			EXP_LVL9	CSazmeaX (Const struct cs_Azmea_ *azmea,int cnt,Const double pnts [][3]);

double		EXP_LVL9	CSazmedC (Const struct cs_Azmed_ *azmed,Const double ll [2]);
int			EXP_LVL9	CSazmedF (Const struct cs_Azmed_ *azmed,double xy [2],Const double ll [2]);
double		EXP_LVL9	CSazmedH (Const struct cs_Azmed_ *azmed,Const double ll [2]);
int			EXP_LVL9	CSazmedI (Const struct cs_Azmed_ *azmed,double ll [2],Const double xy [2]);
double		EXP_LVL9	CSazmedK (Const struct cs_Azmed_ *azmed,Const double ll [2]);
int			EXP_LVL9	CSazmedL (Const struct cs_Azmed_ *azmed,int cnt,Const double pnts [][3]);
int			EXP_LVL9	CSazmedQ (Const struct cs_Csdef_ *csdef,unsigned short prj_code,int err_list [],int list_sz);
void		EXP_LVL9	CSazmedS (struct cs_Csprm_ *csprm);
int			EXP_LVL9	CSazmedX (Const struct cs_Azmed_ *azmed,int cnt,Const double pnts [][3]);

void		EXP_LVL1	CSbccDbg (char *results,int rsltSz);
struct cs_Csprm_* EXP_LVL9	CSbcclu (Const char *cs_name);
void		EXP_LVL1	CSbdcDbg (char *results,int rsltSz);
struct cs_Dtcprm_* EXP_LVL9	CSbdclu (Const struct cs_Csprm_ *src_cs,Const struct cs_Csprm_ *dst_cs,int dat_err,int blk_err);

int			EXP_LVL9	CSbswap (void *rec,Const char *frmt);

double		EXP_LVL5	CSbtFcal (Const struct cs_BtcofF_ *btF_ptr,double lat);
double		EXP_LVL5	CSbtFcalPrec (Const struct cs_BtcofF_ *btF_ptr,double lat);
void		EXP_LVL5	CSbtFsu  (struct cs_BtcofF_ *btF_ptr,double e_sq);
double		EXP_LVL5	CSbtIcal (Const struct cs_BtcofI_ *btI_ptr,double beta);
double		EXP_LVL5	CSbtIcalPrec (Const struct cs_BtcofI_ *btI_ptr,double beta);
void		EXP_LVL5	CSbtIsu  (struct cs_BtcofI_ *btI_ptr,double e_sq);

double		EXP_LVL9	CSbonneC (Const struct cs_Bonne_ *bonne,Const double ll [2]);
int			EXP_LVL9	CSbonneF (Const struct cs_Bonne_ *bonne,double xy [2],Const double ll [2]);
double		EXP_LVL9	CSbonneH (Const struct cs_Bonne_ *bonne,Const double ll [2]);
int			EXP_LVL9	CSbonneI (Const struct cs_Bonne_ *bonne,double ll [2],Const double xy [2]);
double		EXP_LVL9	CSbonneK (Const struct cs_Bonne_ *bonne,Const double ll [2]);
int			EXP_LVL9	CSbonneL (Const struct cs_Bonne_ *bonne,int cnt,Const double pnts [][3]);
int			EXP_LVL9	CSbonneQ (Const struct cs_Csdef_ *csdef,unsigned short prj_code,int err_list [],int list_sz);
void		EXP_LVL9	CSbonneS (struct cs_Csprm_ *csprm);
int			EXP_LVL9	CSbonneX (Const struct cs_Bonne_ *bonne,int cnt,Const double pnts [][3]);

double		EXP_LVL9	CSbpcncC (Const struct cs_Bpcnc_ *bonne,Const double ll [2]);
int			EXP_LVL9	CSbpcncF (Const struct cs_Bpcnc_ *bpcnc,double xy [2],Const double ll [2]);
int			EXP_LVL9	CSbpcncI (Const struct cs_Bpcnc_ *bpcnc,double ll [2],Const double xy [2]);
double		EXP_LVL9	CSbpcncK (Const struct cs_Bpcnc_ *bpcnc,Const double ll [2]);
int			EXP_LVL9	CSbpcncL (Const struct cs_Bpcnc_ *bpcnc,int cnt,Const double pnts [][3]);
int			EXP_LVL9	CSbpcncQ (Const struct cs_Csdef_ *csdef,unsigned short prj_code,int err_list [],int list_sz);
void		EXP_LVL9	CSbpcncS (struct cs_Csprm_ *csprm);
int			EXP_LVL9	CSbpcncX (Const struct cs_Bpcnc_ *bpcnc,int cnt,Const double pnts [][3]);

double		EXP_LVL5	CSccsphrD (Const double ll0 [2],Const double ll1 [2]);
double		EXP_LVL5	CSccsphrR (Const double ll0 [2],Const double ll1 [2]);
double		EXP_LVL5	CSchiFcal (Const struct cs_ChicofF_ *chiF_ptr,double lat);
void		EXP_LVL5	CSchiFsu (struct cs_ChicofF_ *chiF_ptr,double e_sq);
double		EXP_LVL5	CSchiIcal (Const struct cs_ChicofI_ *chiI_ptr,double chi);
void		EXP_LVL5	CSchiIsu (struct cs_ChicofI_ *chiI_ptr,double e_sq);

int			EXP_LVL9	CScscomp (Const char *inpt,Const char *outp,
												   int flags,
												   Const char *elpath,
												   Const char *dtpath,
												   int (*err_func)(char *mesg));

double		EXP_LVL9	CScsiniC (Const struct cs_Csini_ *csini,Const double ll [2]);
int			EXP_LVL9	CScsiniF (Const struct cs_Csini_ *csini,double xy [2],Const double ll [2]);
double		EXP_LVL9	CScsiniH (Const struct cs_Csini_ *csini,Const double ll [2]);
int			EXP_LVL9	CScsiniI (Const struct cs_Csini_ *csini,double ll [2],Const double xy [2]);
double		EXP_LVL9	CScsiniK (Const struct cs_Csini_ *csini,Const double ll [2]);
int			EXP_LVL9	CScsiniL (Const struct cs_Csini_ *csini,int cnt,Const double pnts [][3]);
int			EXP_LVL9	CScsiniQ (Const struct cs_Csdef_ *csdef,unsigned short prj_code,int err_list [],int list_sz);
void		EXP_LVL9	CScsiniS (struct cs_Csprm_ *csprm);
int			EXP_LVL9	CScsiniX (Const struct cs_Csini_ *csini,int cnt,Const double pnts [][3]);
char*		EXP_LVL9	CScsKeyNames (void);
struct cs_Csprm_* EXP_LVL3	CScsloc (struct cs_Csdef_ *cs_ptr,struct cs_Datum_ *dt_ptr);
struct cs_Csprm_* EXP_LVL3	CScsloc1 (struct cs_Csdef_ *cs_ptr);
struct cs_Csprm_* EXP_LVL3	CScsloc2 (struct cs_Csdef_ *cs_ptr,
									  struct cs_Dtdef_ *dt_ptr,
									  struct cs_Eldef_ *el_ptr);

double		EXP_LVL3	CSdefCmpPrjPrm (struct cs_Prjtab_* pp,int prmNbr,double orgValue,double revValue,char *message,size_t messageSize);
int			EXP_LVL9	CSdfltpro (int type,char *name,int size);

int			EXP_LVL3	CSdtcvt (struct cs_Dtcprm_ *dtcPrm,short flag3D,Const double ll_in [3],double ll_out [3]);
int			EXP_LVL9	CSdtcomp (Const char *inpt,Const char *outp,
												   int flags,
												   Const char *elpath,
												   int (*err_func)(char *mesg));
struct cs_Dtcprm_* EXP_LVL3	CSdtcsu (Const struct cs_Datum_ *src_dt,Const struct cs_Datum_ *dst_dt,int dat_erf,int blk_erf);
struct cs_Dtcprm_* EXP_LVL3	CSdtcsu1 (Const char* gxName,short direction,int blk_erf);
struct cs_Dtcprm_* EXP_LVL3	CSdtcsu2 (Const struct cs_GeodeticTransform_ *xfrmDefPtr, short direction, int blk_erf);

char*		EXP_LVL9	CSdtKeyNames (void);
struct cs_Datum_* EXP_LVL5	CSdtloc1 (Const struct cs_Dtdef_ *dtdef_p);
struct cs_Datum_* EXP_LVL5	CSdtloc2 (Const struct cs_Dtdef_ *dtdef_p,Const struct cs_Eldef_ *eldef_p);

double		EXP_LVL9	CSedcncC (Const struct cs_Edcnc_ *edcnc,Const double ll [2]);
int			EXP_LVL9	CSedcncF (Const struct cs_Edcnc_ *edcnc,double xy [2],Const double ll [2]);
double		EXP_LVL9	CSedcncH (Const struct cs_Edcnc_ *edcnc,Const double ll [2]);
int			EXP_LVL9	CSedcncI (Const struct cs_Edcnc_ *edcnc,double ll [2],Const double xy [2]);
double		EXP_LVL9	CSedcncK (Const struct cs_Edcnc_ *edcnc,Const double ll [2]);
int			EXP_LVL9	CSedcncL (Const struct cs_Edcnc_ *edcnc,int cnt,Const double pnts [][3]);
int			EXP_LVL9	CSedcncQ (Const struct cs_Csdef_ *csdef,unsigned short prj_code,int err_list [],int list_sz);
void		EXP_LVL9	CSedcncS (struct cs_Csprm_ *csprm);
int			EXP_LVL9	CSedcncX (Const struct cs_Edcnc_ *edcnc,int cnt,Const double pnts [][3]);

double		EXP_LVL9	CSedcylC (Const struct cs_Edcyl_ *edcyl,Const double ll [2]);
int			EXP_LVL9	CSedcylF (Const struct cs_Edcyl_ *edcyl,double xy [2],Const double ll [2]);
double		EXP_LVL9	CSedcylH (Const struct cs_Edcyl_ *edcyl,Const double ll [2]);
int			EXP_LVL9	CSedcylI (Const struct cs_Edcyl_ *edcyl,double ll [2],Const double xy [2]);
double		EXP_LVL9	CSedcylK (Const struct cs_Edcyl_ *edcyl,Const double ll [2]);
int			EXP_LVL9	CSedcylL (Const struct cs_Edcyl_ *edcyl,int cnt,Const double pnts [][3]);
int			EXP_LVL9	CSedcylQ (Const struct cs_Csdef_ *csdef,unsigned short prj_code,int err_list [],int list_sz);
void		EXP_LVL9	CSedcylS (struct cs_Csprm_ *csprm);
int			EXP_LVL9	CSedcylX (Const struct cs_Edcyl_ *edcyl,int cnt,Const double pnts [][3]);

double		EXP_LVL9	CSekrt4C (Const struct cs_Ekrt4_ *ekrt4,Const double ll [2]);
int			EXP_LVL9	CSekrt4F (Const struct cs_Ekrt4_ *ekrt4,double xy [2],Const double ll [2]);
double		EXP_LVL9	CSekrt4H (Const struct cs_Ekrt4_ *ekrt4,Const double ll [2]);
int			EXP_LVL9	CSekrt4I (Const struct cs_Ekrt4_ *ekrt4,double ll [2],Const double xy [2]);
double		EXP_LVL9	CSekrt4K (Const struct cs_Ekrt4_ *ekrt4,Const double ll [2]);
int			EXP_LVL9	CSekrt4L (Const struct cs_Ekrt4_ *ekrt4,int cnt,Const double pnts [][3]);
int			EXP_LVL9	CSekrt4Q (Const struct cs_Csdef_ *csdef,unsigned short prj_code,int err_list [],int list_sz);
void		EXP_LVL9	CSekrt4S (struct cs_Csprm_ *csprm);
int			EXP_LVL9	CSekrt4X (Const struct cs_Ekrt4_ *ekrt4,int cnt,Const double pnts [][3]);

double		EXP_LVL9	CSekrt6C (Const struct cs_Ekrt6_ *ekrt6,Const double ll [2]);
int			EXP_LVL9	CSekrt6F (Const struct cs_Ekrt6_ *ekrt6,double xy [2],Const double ll [2]);
double		EXP_LVL9	CSekrt6H (Const struct cs_Ekrt6_ *ekrt6,Const double ll [2]);
int			EXP_LVL9	CSekrt6I (Const struct cs_Ekrt6_ *ekrt6,double ll [2],Const double xy [2]);
double		EXP_LVL9	CSekrt6K (Const struct cs_Ekrt6_ *ekrt6,Const double ll [2]);
int			EXP_LVL9	CSekrt6L (Const struct cs_Ekrt6_ *ekrt6,int cnt,Const double pnts [][3]);
int			EXP_LVL9	CSekrt6Q (Const struct cs_Csdef_ *csdef,unsigned short prj_code,int err_list [],int list_sz);
void		EXP_LVL9	CSekrt6S (struct cs_Csprm_ *csprm);
int			EXP_LVL9	CSekrt6X (Const struct cs_Ekrt6_ *ekrt6,int cnt,Const double pnts [][3]);

int			EXP_LVL9	CSelcomp (Const char *inpt,Const char *outp,
												   int flags,
												   int (*err_func)(char *mesg));
char*		EXP_LVL9	CSelKeyNames (void);
unsigned short	EXP_LVL7	CSerpt (char *mesg,int size,int err_num);
int			EXP_LVL5	CSextractDbl (csFILE *aStrm,double* result);

const char*	EXP_LVL3	CSfips2a (int state,int county);

int			EXP_LVL3	CSgaussF (struct cs_Gauss_* gauss,double gaussian [2],double ellipsoidal [2]);
int			EXP_LVL3	CSgaussI (struct cs_Gauss_* gauss,double ellipsoidal [2],double gaussian [2]);
struct cs_Gauss_* EXP_LVL3	CSgaussS (double e_rad,double e_sq,double orgLng,double stdLat);

int			EXP_LVL7	CSgdcGenerate (Const char *directory);

int			EXP_LVL1	CS_geoidHgt (Const double ll_84 [2],double *height);
void		EXP_LVL1	CS_geoidCls (void);

double		EXP_LVL9	CSgnomcC (Const struct cs_Gnomc_ *gnomc,Const double ll [2]);
int			EXP_LVL9	CSgnomcF (Const struct cs_Gnomc_ *gnomc,double xy [2],Const double ll [2]);
double		EXP_LVL9	CSgnomcH (Const struct cs_Gnomc_ *gnomc,Const double ll [2]);
int			EXP_LVL9	CSgnomcI (Const struct cs_Gnomc_ *gnomc,double ll [2],Const double xy [2]);
double		EXP_LVL9	CSgnomcK (Const struct cs_Gnomc_ *gnomc,Const double ll [2]);
int			EXP_LVL9	CSgnomcL (Const struct cs_Gnomc_ *gnomc,int cnt,Const double pnts [][3]);
int			EXP_LVL9	CSgnomcQ (Const struct cs_Csdef_ *csdef,unsigned short prj_code,int err_list [],int list_sz);
void		EXP_LVL9	CSgnomcS (struct cs_Csprm_ *csprm);
int			EXP_LVL9	CSgnomcX (Const struct cs_Gnomc_ *gnomc,int cnt,Const double pnts [][3]);

int			EXP_LVL9	CSgnricQ (struct cs_Csdef_ *csdef,int err_list [],int list_sz);

int			EXP_LVL9	CSgpcomp (Const char *inpt,Const char *outp,int flags,char *datum,char* xforms,int (*err_func)(char *mesg));
int			EXP_LVL9	CSgxcomp (Const char *inpt,Const char *outp,int flags,char *datum,int (*err_func)(char *mesg));

double		EXP_LVL9	CShmlsnC (Const struct cs_Hmlsn_ *hmlsn,Const double ll [2]);
int			EXP_LVL9	CShmlsnF (Const struct cs_Hmlsn_ *hmlsn,double xy [2],Const double ll [2]);
double		EXP_LVL9	CShmlsnH (Const struct cs_Hmlsn_ *hmlsn,Const double ll [2]);
int			EXP_LVL9	CShmlsnI (Const struct cs_Hmlsn_ *hmlsn,double ll [2],Const double xy [2]);
double		EXP_LVL9	CShmlsnK (Const struct cs_Hmlsn_ *hmlsn,Const double ll [2]);
int			EXP_LVL9	CShmlsnL (Const struct cs_Hmlsn_ *hmlsn,int cnt,Const double pnts [][3]);
int			EXP_LVL9	CShmlsnQ (Const struct cs_Csdef_ *csdef,unsigned short prj_code,int err_list [],int list_sz);
void		EXP_LVL9	CShmlsnS (struct cs_Csprm_ *csprm);
int			EXP_LVL9	CShmlsnX (Const struct cs_Hmlsn_ *hmlsn,int cnt,Const double pnts [][3]);

double		EXP_LVL9	CSkrovkC (Const struct cs_Krovk_ *krovk,Const double ll [2]);
double		EXP_LVL9	CSkrovkCsMapParam (double e_rad,double e_sq,double orgLng,double stdLat,double epsgParm);
double		EXP_LVL9	CSkrovkEpsgParam (double e_rad,double e_sq,double orgLng,double stdLat,double csMapParm);
int			EXP_LVL9	CSkrovkF (Const struct cs_Krovk_ *krovk,double xy [2],Const double lnglat [2]);
int			EXP_LVL9	CSkrovkI (Const struct cs_Krovk_ *krovk,double lnglat [2],Const double xy [2]);
double		EXP_LVL9	CSkrovkK (Const struct cs_Krovk_ *krovk,Const double ll [2]);
int			EXP_LVL9	CSkrovkL (Const struct cs_Krovk_ *krovk,int cnt,Const double pnts [][3]);
int			EXP_LVL9	CSkrovkQ (Const struct cs_Csdef_ *cs_def,unsigned short prj_code,int err_list [],int list_sz);
void		EXP_LVL9	CSkrovkS (struct cs_Csprm_ *csprm);
int			EXP_LVL9	CSkrovkX (Const struct cs_Krovk_ *krovk,int cnt,Const double pnts [][3]);
void		EXP_LVL9	CSkrovk95 (double deltaXY [2],Const double xy [2]);

int			EXP_LVL9	CSllCsFromDt (char* csKeyName,int csKeySize,Const char* dtKeyName);
void		EXP_LVL7	CSllnrml (Const double oll [2],Const double ll  [2],double ll1 [2],double ll2 [2]);

double		EXP_LVL9	CSlmbrtC (Const struct cs_Lmbrt_ *lmbrt,Const double ll [2]);
int			EXP_LVL9	CSlmbrtF (Const struct cs_Lmbrt_ *lmbrt,double xy [2],Const double ll [2]);
int			EXP_LVL9	CSlmbrtI (Const struct cs_Lmbrt_ *lmbrt,double ll [2],Const double xy [2]);
double		EXP_LVL9	CSlmbrtK (Const struct cs_Lmbrt_ *lmbrt,Const double ll [2]);
int			EXP_LVL9	CSlmbrtL (Const struct cs_Lmbrt_ *lmbrt,int cnt,Const double pnts [][3]);
int			EXP_LVL9	CSlmbrtQ (Const struct cs_Csdef_ *csdef,unsigned short prj_code,int err_list [],int list_sz);
void		EXP_LVL9	CSlmbrtS (struct cs_Csprm_ *csprm);
int			EXP_LVL9	CSlmbrtX (Const struct cs_Lmbrt_ *lmbrt,int cnt,Const double pnts [][3]);

double		EXP_LVL9	CSlmtanC (Const struct cs_Lmtan_ *lmtan,Const double ll [2]);
int			EXP_LVL9	CSlmtanF (Const struct cs_Lmtan_ *lmtan,double xy [2],Const double ll [2]);
double		EXP_LVL9	CSlmtanH (Const struct cs_Lmtan_ *lmtan,Const double ll [2]);
int			EXP_LVL9	CSlmtanI (Const struct cs_Lmtan_ *lmtan,double ll [2],Const double xy [2]);
double		EXP_LVL9	CSlmtanK (Const struct cs_Lmtan_ *lmtan,Const double ll [2]);
int			EXP_LVL9	CSlmtanL (Const struct cs_Lmtan_ *lmtan,int cnt,Const double pnts [][3]);
int			EXP_LVL9	CSlmtanQ (Const struct cs_Csdef_ *csdef,unsigned short prj_code,int err_list [],int list_sz);
void		EXP_LVL9	CSlmtanS (struct cs_Csprm_ *csprm);
int			EXP_LVL9	CSlmtanX (Const struct cs_Lmtan_ *lmtan,int cnt,Const double pnts [][3]);

double		EXP_LVL9	CSmillrC (Const struct cs_Millr_ *millr,Const double ll [2]);
int			EXP_LVL9	CSmillrF (Const struct cs_Millr_ *millr,double xy [2],Const double ll [2]);
double		EXP_LVL9	CSmillrH (Const struct cs_Millr_ *millr,Const double ll [2]);
int			EXP_LVL9	CSmillrI (Const struct cs_Millr_ *millr,double ll [2],Const double xy [2]);
double		EXP_LVL9	CSmillrK (Const struct cs_Millr_ *millr,Const double ll [2]);
int			EXP_LVL9	CSmillrL (Const struct cs_Millr_ *millr,int cnt,Const double pnts [][3]);
int			EXP_LVL9	CSmillrQ (Const struct cs_Csdef_ *csdef,unsigned short prj_code,int err_list [],int list_sz);
void		EXP_LVL9	CSmillrS (struct cs_Csprm_ *csprm);
int			EXP_LVL9	CSmillrX (Const struct cs_Millr_ *millr,int cnt,Const double pnts [][3]);

double		EXP_LVL5	CSmmFcal (Const struct cs_MmcofF_ *mmF_ptr,double lat,double sin_lat,double cos_lat);
void		EXP_LVL5	CSmmFsu (struct cs_MmcofF_ *mmF_ptr,double ka,double e_sq);
double		EXP_LVL5	CSmmIcal (Const struct cs_MmcofI_ *mmI_ptr,double mm);
void		EXP_LVL5	CSmmIsu (struct cs_MmcofI_ *mmI_ptr,double ka,double e_sq);
double		EXP_LVL5	CSmpFcal (Const struct cs_MpcofF_ *mmF_ptr,double lat);
void		EXP_LVL5	CSmpFsu (struct cs_MpcofF_ *mmF_ptr,double e_sq);



int			EXP_LVL9	CSmodpcB (Const struct cs_Modpc_ *modpc,Const double ll [2],double xy [2],double *their_yc);
double		EXP_LVL9	CSmodpcC (Const struct cs_Modpc_ *modpc,Const double ll [2]);
int			EXP_LVL9	CSmodpcF (Const struct cs_Modpc_ *modpc,double xy [2],Const double ll [2]);
double		EXP_LVL9	CSmodpcH (Const struct cs_Modpc_ *modpc,Const double ll [2]);
int			EXP_LVL9	CSmodpcI (Const struct cs_Modpc_ *modpc,double ll [2],Const double xy [2]);
double		EXP_LVL9	CSmodpcK (Const struct cs_Modpc_ *modpc,Const double ll [2]);
int			EXP_LVL9	CSmodpcL (Const struct cs_Modpc_ *modpc,int cnt,Const double pnts [][3]);
int			EXP_LVL9	CSmodpcQ (Const struct cs_Csdef_ *csdef,unsigned short prj_code,int err_list [],int list_sz);
void		EXP_LVL9	CSmodpcS (struct cs_Csprm_ *csprm);
int			EXP_LVL9	CSmodpcX (Const struct cs_Modpc_ *modpc,int cnt,Const double pnts [][3]);

double		EXP_LVL9	CSmolwdC (Const struct cs_Molwd_ *molwd,Const double ll [2]);
int			EXP_LVL9	CSmolwdF (Const struct cs_Molwd_ *molwd,double xy [2],Const double ll [2]);
double		EXP_LVL9	CSmolwdH (Const struct cs_Molwd_ *molwd,Const double ll [2]);
int			EXP_LVL9	CSmolwdI (Const struct cs_Molwd_ *molwd,double ll [2],Const double xy [2]);
double		EXP_LVL9	CSmolwdK (Const struct cs_Molwd_ *molwd,Const double ll [2]);
int			EXP_LVL9	CSmolwdL (Const struct cs_Molwd_ *molwd,int cnt,Const double pnts [][3]);
int			EXP_LVL9	CSmolwdQ (Const struct cs_Csdef_ *csdef,unsigned short prj_code,int err_list [],int list_sz);
void		EXP_LVL9	CSmolwdS (struct cs_Csprm_ *csprm);
int			EXP_LVL9	CSmolwdX (Const struct cs_Molwd_ *molwd,int cnt,Const double pnts [][3]);

double		EXP_LVL9	CSmrcatC (Const struct cs_Mrcat_ *mrcat,Const double ll [2]);
int			EXP_LVL9	CSmrcatF (Const struct cs_Mrcat_ *mrcat,double xy [2],Const double ll [2]);
double		EXP_LVL9	CSmrcatH (Const struct cs_Mrcat_ *mrcat,Const double ll [2]);
int			EXP_LVL9	CSmrcatI (Const struct cs_Mrcat_ *mrcat,double ll [2],Const double xy [2]);
double		EXP_LVL9	CSmrcatK (Const struct cs_Mrcat_ *mrcat,Const double ll [2]);
int			EXP_LVL9	CSmrcatL (Const struct cs_Mrcat_ *mrcat,int cnt,Const double pnts [][3]);
int			EXP_LVL9	CSmrcatQ (Const struct cs_Csdef_ *csdef,unsigned short prj_code,int err_list [],int list_sz);
void		EXP_LVL9	CSmrcatS (struct cs_Csprm_ *csprm);
int			EXP_LVL9	CSmrcatX (Const struct cs_Mrcat_ *mrcat,int cnt,Const double pnts [][3]);
double		EXP_LVL5	CSmrcatPhiFromK (double e_sq,double scl_red);

double		EXP_LVL9	CSmstroC (Const struct cs_Mstro_ *mstro,Const double ll [2]);
int			EXP_LVL9	CSmstroF (Const struct cs_Mstro_ *mstro,double xy [2],Const double ll [2]);
int			EXP_LVL9	CSmstroI (Const struct cs_Mstro_ *mstro,double ll [2],Const double xy [2]);
double		EXP_LVL9	CSmstroK (Const struct cs_Mstro_ *mstro,Const double ll [2]);
int			EXP_LVL9	CSmstroL (Const struct cs_Mstro_ *mstro,int cnt,Const double pnts [][3]);
int			EXP_LVL9	CSmstroQ (Const struct cs_Csdef_ *csdef,unsigned short prj_code,int err_list [],int list_sz);
void		EXP_LVL9	CSmstroS (struct cs_Csprm_ *csprm);
int			EXP_LVL9	CSmstroX (Const struct cs_Mstro_ *mstro,int cnt,Const double pnts [][3]);

int			EXP_LVL9	CSmrcomp (Const char *inpt,Const char *outp,int flags,
												   Const char *dt_path,
												   int (*err_func)(char *mesg));

double		EXP_LVL9	CSnacylC (Const struct cs_Nacyl_ *nacyl,Const double ll [2]);
int			EXP_LVL9	CSnacylF (Const struct cs_Nacyl_ *nacyl,double xy [2],Const double ll [2]);
double		EXP_LVL9	CSnacylH (Const struct cs_Nacyl_ *nacyl,Const double ll [2]);
int			EXP_LVL9	CSnacylI (Const struct cs_Nacyl_ *nacyl,double ll [2],Const double xy [2]);
double		EXP_LVL9	CSnacylK (Const struct cs_Nacyl_ *nacyl,Const double ll [2]);
int			EXP_LVL9	CSnacylL (Const struct cs_Nacyl_ *nacyl,int cnt,Const double pnts [][3]);
int			EXP_LVL9	CSnacylQ (Const struct cs_Csdef_ *csdef,unsigned short prj_code,int err_list [],int list_sz);
void		EXP_LVL9	CSnacylS (struct cs_Csprm_ *csprm);
int			EXP_LVL9	CSnacylX (Const struct cs_Nacyl_ *nacyl,int cnt,Const double pnts [][3]);

struct cs_Dtcprm_ * EXP_LVL9	CSnulinit (Const char *src_name,Const char *dst_name);

double		EXP_LVL9	CSnerthC (Const struct cs_Nerth_ *nerth,Const double ll [2]);
int			EXP_LVL9	CSnerthF (Const struct cs_Nerth_ *nerth,double xy [2],Const double ll [2]);
int			EXP_LVL9	CSnerthI (Const struct cs_Nerth_ *nerth,double ll [2],Const double xy [2]);
double		EXP_LVL9	CSnerthK (Const struct cs_Nerth_ *nerth,Const double ll [2]);
int			EXP_LVL9	CSnerthL (Const struct cs_Nerth_ *nerth,int cnt,Const double pnts [][3]);
int			EXP_LVL9	CSnerthQ (Const struct cs_Csdef_ *csdef,unsigned short prj_code,int err_list [],int list_sz);
void		EXP_LVL9	CSnerthS (struct cs_Csprm_ *csprm);
int			EXP_LVL9	CSnerthX (Const struct cs_Nerth_ *nerth,int cnt,Const double pnts [][3]);

int			EXP_LVL7	CSnampp (char *name,size_t nameSize);

double		EXP_LVL9	CSnzlndC (Const struct cs_Nzlnd_ *nzlnd,Const double ll [2]);
int			EXP_LVL9	CSnzlndF (Const struct cs_Nzlnd_ *nzlnd,double xy [2],Const double ll [2]);
double		EXP_LVL9	CSnzlndH (Const struct cs_Nzlnd_ *nzlnd,Const double ll [2]);
int			EXP_LVL9	CSnzlndI (Const struct cs_Nzlnd_ *nzlnd,double ll [2],Const double xy [2]);
double		EXP_LVL9	CSnzlndK (Const struct cs_Nzlnd_ *nzlnd,Const double ll [2]);
int			EXP_LVL9	CSnzlndL (Const struct cs_Nzlnd_ *nzlnd,int cnt,Const double pnts [][3]);
int			EXP_LVL9	CSnzlndQ (Const struct cs_Csdef_ *csdef,unsigned short prj_code,int err_list [],int list_sz);
void		EXP_LVL9	CSnzlndS (struct cs_Csprm_ *csprm);
int			EXP_LVL9	CSnzlndX (Const struct cs_Nzlnd_ *nzlnd,int cnt,Const double pnts [][3]);

double		EXP_LVL9	CSoblqmC (Const struct cs_Oblqm_ *oblqm,Const double ll [2]);
int			EXP_LVL9	CSoblqmF (Const struct cs_Oblqm_ *oblqm,double xy [2],Const double ll [2]);
int			EXP_LVL9	CSoblqmFuv (Const struct cs_Oblqm_ *oblqm,double lng,double lat,double del_lng,double* uu, double *vv);
int			EXP_LVL9	CSoblqmI (Const struct cs_Oblqm_ *oblqm,double ll [2],Const double xy [2]);
double		EXP_LVL9	CSoblqmK (Const struct cs_Oblqm_ *oblqm,Const double ll [2]);
int			EXP_LVL9	CSoblqmL (Const struct cs_Oblqm_ *oblqm,int cnt,Const double pnts [][3]);
int			EXP_LVL9	CSoblqmQ (Const struct cs_Csdef_ *csdef,unsigned short prj_code,int err_list [],int list_sz);
void		EXP_LVL9	CSoblqmS (struct cs_Csprm_ *csprm);
int			EXP_LVL9	CSoblqmX (Const struct cs_Oblqm_ *oblqm,int cnt,Const double pnts [][3]);

double		EXP_LVL9	CSorthoC (Const struct cs_Ortho_ *ortho,Const double ll [2]);
int			EXP_LVL9	CSorthoF (Const struct cs_Ortho_ *ortho,double xy [2],Const double ll [2]);
double		EXP_LVL9	CSorthoH (Const struct cs_Ortho_ *ortho,Const double ll [2]);
int			EXP_LVL9	CSorthoI (Const struct cs_Ortho_ *ortho,double ll [2],Const double xy [2]);
double		EXP_LVL9	CSorthoK (Const struct cs_Ortho_ *ortho,Const double ll [2]);
int			EXP_LVL9	CSorthoL (Const struct cs_Ortho_ *ortho,int cnt,Const double pnts [][3]);
int			EXP_LVL9	CSorthoQ (Const struct cs_Csdef_ *csdef,unsigned short prj_code,int err_list [],int list_sz);
void		EXP_LVL9	CSorthoS (struct cs_Csprm_ *csprm);
int			EXP_LVL9	CSorthoX (Const struct cs_Ortho_ *ortho,int cnt,Const double pnts [][3]);

double		EXP_LVL9	CSostroC (Const struct cs_Ostro_ *ostro,Const double ll [2]);
int			EXP_LVL9	CSostroF (Const struct cs_Ostro_ *ostro,double xy [2],Const double ll [2]);
int			EXP_LVL9	CSostroI (Const struct cs_Ostro_ *ostro,double ll [2],Const double xy [2]);
double		EXP_LVL9	CSostroK (Const struct cs_Ostro_ *ostro,Const double ll [2]);
int			EXP_LVL9	CSostroL (Const struct cs_Ostro_ *ostro,int cnt,Const double pnts [][3]);
int			EXP_LVL9	CSostroQ (Const struct cs_Csdef_ *csdef,unsigned short prj_code,int err_list [],int list_sz);
void		EXP_LVL9	CSostroS (struct cs_Csprm_ *csprm);
int			EXP_LVL9	CSostroX (Const struct cs_Ostro_ *stero,int cnt,Const double pnts [][3]);

double		EXP_LVL9	CSplycnC (Const struct cs_Plycn_ *plycn,Const double ll [2]);
int			EXP_LVL9	CSplycnF (Const struct cs_Plycn_ *plycn,double xy [2],Const double ll [2]);
double		EXP_LVL9	CSplycnH (Const struct cs_Plycn_ *plycn,Const double ll [2]);
int			EXP_LVL9	CSplycnI (Const struct cs_Plycn_ *plycn,double ll [2],Const double xy [2]);
double		EXP_LVL9	CSplycnK (Const struct cs_Plycn_ *plycn,Const double ll [2]);
int			EXP_LVL9	CSplycnL (Const struct cs_Plycn_ *plycn,int cnt,Const double pnts [][3]);
int			EXP_LVL9	CSplycnQ (Const struct cs_Csdef_ *csdef,unsigned short prj_code,int err_list [],int list_sz);
void		EXP_LVL9	CSplycnS (struct cs_Csprm_ *csprm);
int			EXP_LVL9	CSplycnX (Const struct cs_Plycn_ *plycn,int cnt,Const double pnts [][3]);

int			EXP_LVL3	CSprjPrmCmp (double* qValue,short prmNbr,Const struct cs_Csdef_* original,
																 Const struct cs_Csdef_* revised,
																 char* message,
																 size_t msgSize);

double		EXP_LVL9	CSpstroC (Const struct cs_Pstro_ *pstro,Const double ll [2]);
int			EXP_LVL9	CSpstroF (Const struct cs_Pstro_ *pstro,double xy [2],Const double ll [2]);
int			EXP_LVL9	CSpstroI (Const struct cs_Pstro_ *pstro,double ll [2],Const double xy [2]);
double		EXP_LVL9	CSpstroK (Const struct cs_Pstro_ *pstro,Const double ll [2]);
int			EXP_LVL9	CSpstroL (Const struct cs_Pstro_ *pstro,int cnt,Const double pnts [][3]);
int			EXP_LVL9	CSpstroQ (Const struct cs_Csdef_ *csdef,unsigned short prj_code,int err_list [],int list_sz);
void		EXP_LVL9	CSpstroS (struct cs_Csprm_ *csprm);
int			EXP_LVL9	CSpstroX (Const struct cs_Pstro_ *pstro,int cnt,Const double pnts [][3]);

double		EXP_LVL9	CSrobinC (Const struct cs_Robin_ *robin,Const double ll [2]);
int			EXP_LVL9	CSrobinF (Const struct cs_Robin_ *robin,double xy [2],Const double ll [2]);
int			EXP_LVL9	CSrobinI (Const struct cs_Robin_ *robin,double ll [2],Const double xy [2]);
double		EXP_LVL9	CSrobinH (Const struct cs_Robin_ *robin,Const double ll [2]);
double		EXP_LVL9	CSrobinK (Const struct cs_Robin_ *robin,Const double ll [2]);
int			EXP_LVL9	CSrobinL (Const struct cs_Robin_ *robin,int cnt,Const double onts [][3]);
int			EXP_LVL9	CSrobinQ (Const struct cs_Csdef_ *csdef,unsigned short prj_code,int err_list [],int list_sz);
void		EXP_LVL9	CSrobinS (struct cs_Csprm_ *csprm);
int			EXP_LVL9	CSrobinX (Const struct cs_Robin_ *robin,int cnt,Const double pnts [][3]);

void		EXP_LVL5	CSrplDirSep (char *path);

double		EXP_LVL9	CSsinusC (Const struct cs_Sinus_ *sinus,Const double ll [2]);
int			EXP_LVL9	CSsinusF (Const struct cs_Sinus_ *sinus,double xy [2],Const double ll [2]);
double		EXP_LVL9	CSsinusH (Const struct cs_Sinus_ *sinus,Const double ll [2]);
int			EXP_LVL9	CSsinusI (Const struct cs_Sinus_ *sinus,double ll [2],Const double xy [2]);
double		EXP_LVL9	CSsinusK (Const struct cs_Sinus_ *sinus,Const double ll [2]);
int			EXP_LVL9	CSsinusL (Const struct cs_Sinus_ *sinus,int cnt,Const double pnts [] [3]);
int			EXP_LVL9	CSsinusQ (Const struct cs_Csdef_ *csdef,unsigned short prj_code,int err_list [],int list_sz);
void		EXP_LVL9	CSsinusS (struct cs_Csprm_ *csprm);
int			EXP_LVL9	CSsinusX (Const struct cs_Sinus_ *sinus,int cnt,Const double pnts [] [3]);

double		EXP_LVL9	CSsstroC (Const struct cs_Sstro_ *sstro,Const double ll [2]);
int			EXP_LVL9	CSsstroF (Const struct cs_Sstro_ *sstro,double xy [2],Const double ll [2]);
int			EXP_LVL9	CSsstroI (Const struct cs_Sstro_ *sstro,double ll [2],Const double xy [2]);
double		EXP_LVL9	CSsstroK (Const struct cs_Sstro_ *sstro,Const double ll [2]);
int			EXP_LVL9	CSsstroL (Const struct cs_Sstro_ *sstro,int cnt,Const double pnts [][3]);
int			EXP_LVL9	CSsstroQ (Const struct cs_Csdef_ *csdef,unsigned short prj_code,int err_list [],int list_sz);
void		EXP_LVL9	CSsstroS (struct cs_Csprm_ *csprm);
int			EXP_LVL9	CSsstroX (Const struct cs_Sstro_ *sstro,int cnt,Const double pnts [][3]);

double		EXP_LVL9	CSswissC (Const struct cs_Swiss_ *swiss,Const double ll [2]);
int			EXP_LVL9	CSswissF (Const struct cs_Swiss_ *swiss,double xy [2],Const double ll [2]);
int			EXP_LVL9	CSswissI (Const struct cs_Swiss_ *swiss,double ll [2],Const double xy [2]);
double		EXP_LVL9	CSswissK (Const struct cs_Swiss_ *swiss,Const double ll [2]);
int			EXP_LVL9	CSswissL (Const struct cs_Swiss_ *swiss,int cnt,Const double pnts [][3]);
int			EXP_LVL9	CSswissQ (Const struct cs_Csdef_ *csdef,unsigned short prj_code,int err_list [],int list_sz);
void		EXP_LVL9	CSswissS (struct cs_Csprm_ *csprm);
int			EXP_LVL9	CSswissX (Const struct cs_Swiss_ *swiss,int cnt,Const double pnts [][3]);

double		EXP_LVL9	CSsys34C (Const struct cs_Sys34_ *sys34,Const double ll [2]);
int			EXP_LVL9	CSsys34F (Const struct cs_Sys34_ *sys34,double xy [2],Const double ll [2]);
int			EXP_LVL9	CSsys34I (Const struct cs_Sys34_ *sys34,double ll [2],Const double xy [2]);
double		EXP_LVL9	CSsys34K (Const struct cs_Sys34_ *sys34,Const double ll [2]);
int			EXP_LVL9	CSsys34L (Const struct cs_Sys34_ *sys34,int cnt,Const double pnts [][3]);
int			EXP_LVL9	CSsys34Q (Const struct cs_Csdef_ *csdef,unsigned short prj_code,int err_list [],int list_sz);
void		EXP_LVL9	CSsys34S (struct cs_Csprm_ *csprm);
int			EXP_LVL9	CSsys34X (Const struct cs_Sys34_ *sys34,int cnt,Const double pnts [][3]);

double		EXP_LVL9	CStacylC (Const struct cs_Tacyl_ *tacyl,Const double ll [2]);
int			EXP_LVL9	CStacylF (Const struct cs_Tacyl_ *tacyl,double xy [2],Const double ll [2]);
int			EXP_LVL9	CStacylI (Const struct cs_Tacyl_ *tacyl,double ll [2],Const double xy [2]);
double		EXP_LVL9	CStacylH (Const struct cs_Tacyl_ *tacyl,Const double ll [2]);
double		EXP_LVL9	CStacylK (Const struct cs_Tacyl_ *tacyl,Const double ll [2]);
int			EXP_LVL9	CStacylL (Const struct cs_Tacyl_ *tacyl,int cnt,Const double pnts [][3]);
int			EXP_LVL9	CStacylQ (Const struct cs_Csdef_ *csdef,unsigned short prj_code,int err_list [],int list_sz);
void		EXP_LVL9	CStacylS (struct cs_Csprm_ *csprm);
int			EXP_LVL9	CStacylX (Const struct cs_Tacyl_ *tacyl,int cnt,Const double pnts [][3]);

double		EXP_LVL9	CStrmerC (Const struct cs_Trmer_ *trmer,Const double ll [2]);
int			EXP_LVL9	CStrmerF (Const struct cs_Trmer_ *trmer,double xy [2],Const double ll [2]);
int			EXP_LVL9	CStrmerI (Const struct cs_Trmer_ *trmer,double ll [2],Const double xy [2]);
double		EXP_LVL9	CStrmerK (Const struct cs_Trmer_ *trmer,Const double ll [2]);
int			EXP_LVL9	CStrmerL (Const struct cs_Trmer_ *trmer,int cnt,Const double pnts [][3]);
int			EXP_LVL9	CStrmerQ (Const struct cs_Csdef_ *csdef,unsigned short prj_code,int err_list [],int list_sz);
void		EXP_LVL9	CStrmerS (struct cs_Csprm_ *csprm);
int			EXP_LVL9	CStrmerX (Const struct cs_Trmer_ *trmer,int cnt,Const double pnts [][3]);

double		EXP_LVL9	CStrmrsC (Const struct cs_Trmrs_ *trmrs,Const double ll [2]);
int			EXP_LVL9	CStrmrsF (Const struct cs_Trmrs_ *trmrs,double xy [2],Const double ll [2]);
int			EXP_LVL9	CStrmrsI (Const struct cs_Trmrs_ *trmrs,double ll [2],Const double xy [2]);
double		EXP_LVL9	CStrmrsK (Const struct cs_Trmrs_ *trmrs,Const double ll [2]);
int			EXP_LVL9	CStrmrsL (Const struct cs_Trmrs_ *trmrs,int cnt,Const double pnts [][3]);
int			EXP_LVL9	CStrmrsQ (Const struct cs_Csdef_ *csdef,unsigned short prj_code,int err_list [],int list_sz);
void		EXP_LVL9	CStrmrsS (struct cs_Csprm_ *csprm);
int			EXP_LVL9	CStrmrsX (Const struct cs_Trmrs_ *trmrs,int cnt,Const double pnts [][3]);

double		EXP_LVL9	CSunityC (Const struct cs_Unity_ *unity,Const double ll [2]);
int			EXP_LVL9	CSunityF (Const struct cs_Unity_ *unity,double xy [2],Const double ll [2]);
int			EXP_LVL9	CSunityI (Const struct cs_Unity_ *unity,double ll [2],Const double xy [2]);
double		EXP_LVL9	CSunityK (Const struct cs_Unity_ *unity,Const double ll [2]);
int			EXP_LVL9	CSunityL (Const struct cs_Unity_ *unity,int cnt,Const double pnts [][3]);
int			EXP_LVL9	CSunityQ (Const struct cs_Csdef_ *csdef,unsigned short prj_code,int err_list [],int list_sz);
void		EXP_LVL9	CSunityS (struct cs_Csprm_ *csprm);
int			EXP_LVL9	CSunityX (Const struct cs_Unity_ *unity,int cnt,Const double pnts [][3]);

double		EXP_LVL9	CSvdgrnC (Const struct cs_Vdgrn_ *vdgrn,Const double ll [2]);
int			EXP_LVL9	CSvdgrnF (Const struct cs_Vdgrn_ *vdgrn,double xy [2],Const double ll [2]);
double		EXP_LVL9	CSvdgrnH (Const struct cs_Vdgrn_ *vdgrn,Const double ll [2]);
int			EXP_LVL9	CSvdgrnI (Const struct cs_Vdgrn_ *vdgrn,double ll [2],Const double xy [2]);
double		EXP_LVL9	CSvdgrnK (Const struct cs_Vdgrn_ *vdgrn,Const double ll [2]);
int			EXP_LVL9	CSvdgrnL (Const struct cs_Vdgrn_ *vdgrn,int cnt,Const double pnts [][3]);
int			EXP_LVL9	CSvdgrnQ (Const struct cs_Csdef_ *csdef,unsigned short prj_code,int err_list [],int list_sz);
void		EXP_LVL9	CSvdgrnS (struct cs_Csprm_ *csprm);
int			EXP_LVL9	CSvdgrnX (Const struct cs_Vdgrn_ *vdgrn,int cnt,Const double pnts [][3]);

int			EXP_LVL9	CSwinklB (Const struct cs_Winkl_ *winkl,double* xx,double* yy,double del_lng,double lat);
double		EXP_LVL9	CSwinklC (Const struct cs_Winkl_ *winkl,Const double ll [2]);
int			EXP_LVL9	CSwinklF (Const struct cs_Winkl_ *winkl,double xy [2],Const double ll [2]);
double		EXP_LVL9	CSwinklH (Const struct cs_Winkl_ *winkl,Const double ll [2]);
int			EXP_LVL9	CSwinklI (Const struct cs_Winkl_ *winkl,double ll [2],Const double xy [2]);
double		EXP_LVL9	CSwinklK (Const struct cs_Winkl_ *winkl,Const double ll [2]);
int			EXP_LVL9	CSwinklL (Const struct cs_Winkl_ *winkl,int cnt,Const double pnts [][3]);
int			EXP_LVL9	CSwinklQ (Const struct cs_Csdef_ *csdef,unsigned short prj_code,int err_list [],int list_sz);
void		EXP_LVL9	CSwinklS (struct cs_Csprm_ *csprm);
int			EXP_LVL9	CSwinklX (Const struct cs_Winkl_ *winkl,int cnt,Const double pnts [][3]);

int			EXP_LVL7	CSvrtconInit (void);
void		EXP_LVL9	CSvrtconCls (void);
int			EXP_LVL7	CSvrtcon29To88 (double* deltaHgt,Const double ll_83 [3]);

int			EXP_LVL9	CSzone (struct cs_Zone_ *zp,double west,double cent,double east);

int			EXP_LVL3	CSel2Wkt (char *bufr,size_t bufrSize,enum ErcWktFlavor flavor,const struct cs_Eldef_ *el_def);
int			EXP_LVL3	CSdt2Wkt (char *datum,size_t datumSize,char *geoTran,size_t geoTranSize,enum ErcWktFlavor flavor,const struct cs_Dtdef_ *dt_def,const struct cs_Eldef_ *el_def);
int			EXP_LVL3	CScs2Wkt (char *projCs,size_t projCsSize,enum ErcWktFlavor flavor,const struct cs_Csdef_ *cs_def,const struct cs_Dtdef_ *dt_def,const struct cs_Eldef_ *el_def);

int			EXP_LVL3	CSel2WktEx (char *bufr,size_t bufrSize,enum ErcWktFlavor flavor,const struct cs_Eldef_ *el_def,unsigned short flags);
int			EXP_LVL3	CSdt2WktEx (char *datum,size_t datumSize,char *geoTran,size_t geoTranSize,enum ErcWktFlavor flavor,const struct cs_Dtdef_ *dt_def,const struct cs_Eldef_ *el_def,unsigned short flags);
int			EXP_LVL3	CScs2WktEx (char *projCs,size_t projCsSize,enum ErcWktFlavor flavor,const struct cs_Csdef_ *cs_def,const struct cs_Dtdef_ *dt_def,const struct cs_Eldef_ *el_def,unsigned short flags);

int			EXP_LVL1	CS_el2Wkt (char *bufr,size_t bufrSize,const char *elKeyName,int flavor);
int			EXP_LVL1	CS_dt2Wkt (char *bufr,size_t bufrSize,const char *dtKeyName,int flavor);
int			EXP_LVL1	CS_cs2Wkt (char *bufr,size_t bufrSize,const char *csKeyName,int flavor);

int			EXP_LVL1	CS_el2WktEx (char *bufr,size_t bufrSize,const char *elKeyName,int flavor,unsigned short flags);
int			EXP_LVL1	CS_dt2WktEx (char *bufr,size_t bufrSize,const char *dtKeyName,int flavor,unsigned short flags);
int			EXP_LVL1	CS_cs2WktEx (char *bufr,size_t bufrSize,const char *csKeyName,int flavor,unsigned short flags);

int			EXP_LVL1	CS_wktToDict (const char *csKeyName,const char *dtKeyName,const char *elKeyName,const char *wellKnownText,int flavor);
int			EXP_LVL1	CS_wldCalc (double* AA, double* BB, double* CC, double* DD,double from1 [2],double to1 [2],double from2 [2],double to2 [2]);

int			EXP_LVL1 CS_protect (int newValue);
int			EXP_LVL1 CS_unique (int newValue);


int			EXP_LVL1	CS_vldCtName (const char* catName);
int			EXP_LVL1	CS_vldCtNameEx (const char* catName, struct cs_Ctdef_* pCtDef);
Const char*	EXP_LVL1	CS_getCatName (unsigned idx);
Const char*	EXP_LVL1	CS_getItmName (const char* catName,unsigned idx);
int			EXP_LVL1	CS_getItmNameCount (const char* catName);

void		EXP_LVL3	CS_ctfnm (const char* new_name);
int			EXP_LVL3	CSrplItmName (Const char* catName,unsigned idx,Const char* newName);
int			EXP_LVL3	CSrmvItmName (Const char* catName,unsigned idx);
int			EXP_LVL3	CSrmvItmNameEx (struct cs_Ctdef_ *pCategoryIn, Const char* name);
int			EXP_LVL3	CSrmvItmNames (Const char* catName);
int			EXP_LVL3	CSrmvItmNamesEx (struct cs_Ctdef_ *pCategoryIn);
int			EXP_LVL3	CSaddItmName (Const char* catName,Const char* newName);
int			EXP_LVL3	CSaddItmNameEx (struct cs_Ctdef_ *pCategoryIn, Const char* newName);
int			EXP_LVL3	CSrplCatName (Const char* newCtName,unsigned idx);
int			EXP_LVL3	CSrplCatNameEx (Const char* oldCtName, Const char* newCtName);
int			EXP_LVL3	CSaddCategory (Const char* catName);

struct	cs_Ctdef_*	EXP_LVL3	CSrdCategory (csFILE* stream);
int					EXP_LVL3	CSrdCategoryEx (csFILE* stream, struct cs_Ctdef_ *pCategoryIn);
struct	cs_Ctdef_*	EXP_LVL3	CSrdCatFile (void);
struct	cs_Ctdef_*	EXP_LVL3	CSnewCategory (Const char* ctName);
struct	cs_Ctdef_*	EXP_LVL3	CSnewCategoryEx (Const char* ctName, int preAllocate);
struct	cs_Ctdef_*	EXP_LVL3	CSgetCtDef(const char* catName);
int		            EXP_LVL3	CSgetCtDefAll(struct cs_Ctdef_ **pDefArray[]);
struct	cs_Ctdef_*	EXP_LVL3	CScpyCategory(Const struct cs_Ctdef_ * pCategoryIn);
struct	cs_Ctdef_*	EXP_LVL3	CScpyCategoryEx(struct cs_Ctdef_* pDstCategory, Const struct cs_Ctdef_ * pSrcCategory, int setProtectFlag);
int					EXP_LVL3	CSclnCategory(struct cs_Ctdef_ * pCategoryIn);
int					EXP_LVL3	CSdelCategory(Const char* catName);
int					EXP_LVL3	CSupdCategory(Const struct cs_Ctdef_*ctDefPtr);
int					EXP_LVL3	CSupdCategories (Const struct cs_Ctdef_*ctDefPtr);
int					EXP_LVL3	CSwrtCategory (csFILE* stream,Const struct cs_Ctdef_*ctDefPtr);
int					EXP_LVL3	CSwrtCatFile (csFILE* stream,Const struct cs_Ctdef_*ctDefPtr);
void				EXP_LVL3	CSrlsCategory (struct cs_Ctdef_ *ctDefPtr);
void				EXP_LVL3	CSrlsCategoryList (struct cs_Ctdef_ *ctDefHead);
void				EXP_LVL3	CSrlsCategories(void);
int					EXP_LVL9	CSctcomp (Const char *inpt,Const char *outp,int flags,Const char *coordsys,
																			  int (*err_func)(char *mesg));

csFILE*				EXP_LVL3	CS_ctopn (Const char *mode);
struct	cs_Ctdef_*	EXP_LVL3	CS_ctdef (Const char *ct_nam);
int					EXP_LVL3	CS_ctdefAll (struct cs_Ctdef_ **pDefArray[]);
int					EXP_LVL3	CS_ctdel (struct cs_Ctdef_ *ctdef);
int					EXP_LVL3	CS_ctupd (Const struct cs_Ctdef_ *ctdef);
int					EXP_LVL3	CS_ctrd (csFILE *strm, struct cs_Ctdef_ *ct_def);
int					EXP_LVL3	CS_ctwr (csFILE *strm,Const struct cs_Ctdef_ *ct_def);

#ifdef __cplusplus
}
#endif

/* The following is a one dimensional variation of csLLGridCell. */
struct csZGridCell_
{
	struct csZGridCell_ *next;
	enum csDtCvtType type;
	double southWest [2];
	double northEast [2];
	double deltaLng, deltaLat;
	double AA, BB, CC, DD;
	char sourceId [32];
};
struct csZGridCellCache_
{
	int maxCount;
	int usedCount;
	struct csZGridCell_ *listHead;
};

/* Set the maximum packing available situation for the following
   structures.  We do not write them to disk, but we read them and
   use sizeof to determine the size of the entire structure on disk.

   Code should be rewritten to read each individual element from the
   binary file and not rely on reading an entire structure from the
   file; especially if one is relying on sizeof to determine how much
   data to read. */
#if   _RUN_TIME == _rt_BORLAND16 || _RUN_TIME == _rt_BORLAND32
#	pragma option -a1
#elif _RUN_TIME == _rt_METAWARE
#	pragma Align_members (1)
#elif _RUN_TIME == _rt_MSC16 || _RUN_TIME == _rt_MSVC16 || _RUN_TIME == _rt_MSDOTNET || \
														   _RUN_TIME == _rt_MSWIN64 ||  \
														   _RUN_TIME == _rt_MSVC32
#	pragma pack (1)
#elif _RUN_TIME == _rt_HPUX
#	pragma pack 1
#elif _RUN_TIME == _rt_AIX
#	pragma options align=packed
#elif _RUN_TIME >= _rt_UNIXPCC
#	pragma pack (1)
#else
	/* I don't know what would work for WATCOM. */
#endif

struct csGeoid99Hdr_
{
	double latMin;			/* Lowest latitude covered in the file,
							   in degrees, assumed to be WGS84. */
	double lngMin;			/* Lowest longitude covered in the file,
							   0 thru 360, positive east from Greenwich.
							   Again, assumed to be WGS84. */
	double latDelta;		/* Grid cell size, in degrees. */
	double lngDelta;		/* Grid cell size, in degrees. */
	long32_t latCount;		/* Size of the grid, in rows of latitude. */
	long32_t lngCount;		/* Size of the grid, in columns of longitude. */
	long32_t iKind;			/* I don't know either!!! */
};
#define cs_BSWP_Geoid99Hdr "ddddlll"

/* The following is the header on a .byn file.  We define it here to
   take advanatge of whatever packing is currently in place.
   This file format is used for various data purposes, geoid
   height only being the most useful. */
struct csBynGridFileHdr_
{
	long32_t southBoundary;		/* latitude, in arcseconds, of southern boundary
							   of the region covered by data segment. */
	long32_t northBoundary;		/* latitude, in arcseconds, of northern boundary
							   of the region covered by data segment. */
	long32_t westBoundary;		/* longitude, in arcseconds, of western boundary
							   of the region covered by data segment.
							   Negative == west longitude. */
	long32_t eastBoundary;		/* longitude, in arcseconds, of eastern boundary
							   of the region covered by data segment.
							   Negative == west longitude. */
	short nsSpacing;		/* north/south size of a grid cell in arcseconds. */
	short ewSpacing;		/* east/west size of a grid cell in arcseconds. */
	short global;			/* 0 == local model; 1 == Global model */
	short dataType;			/* 0 == undefined
							   1 == geoid heights
							   2 == NS deflections
							   3 == EW deflections
							   4 == Gravity
							   5 == Elevations
							   6 == Sea Surface Heights
							   7 == Sea Surface Topography
							   8 == Other (???) */
	double factor;			/* Use this value to convert data from integer
							   form to real form. */
	short sizeOfData;		/* size of each data element in bytes,
								i.e. 2 or 4 (data elements are always
								signed integers). */
	short stdDeviation;		/* 0  Supposedly not used. */
	double stdDevFactor;	/* Use this value to convert standard deviation
							   data from integer form to real form. */
	short datum;			/* 0 == ITRF, 1 == NAD83 (CSRS) */
	short ellipsoid;		/* 0 == GRS1980,
							   1 == WGS84,
							   2 == TOPEX/EGM96,
							   3 == GRS67 */
	short byteOrder;		/* 0 == Big Endian, 1 == Little Endian. */
	short scaleForBoundaries;
							/* 0 == No Scale (???) */	
	char unused [28];		/* Fill out to 80 bytes. */
};
#define cs_BSWP_BynFileHDR "llllssssdssdssss28c"

/*
	We've finished defining all structures which get read from
	disk. We return packing to whatever the user needs.  Thus,
	hopefully, you can use whatever packing you need for your
	application, while I've specified what I need for mine.
*/
#if   _RUN_TIME == _rt_BORLAND16 || _RUN_TIME == _rt_BORLAND32
#	pragma option -a.
#elif _RUN_TIME == _rt_METAWARE
#	pragma Align_members ()
#elif _RUN_TIME == _rt_MSC16 || _RUN_TIME == _rt_MSVC16 || _RUN_TIME == _rt_MSDOTNET || \
														   _RUN_TIME == _rt_MSWIN64 ||  \
														   _RUN_TIME == _rt_MSVC32
#	pragma pack ()
#elif _RUN_TIME == _rt_HPUX
#	pragma pack
#elif _RUN_TIME == _rt_AIX
#	pragma options align=reset
#elif _RUN_TIME >= _rt_UNIXPCC
#	pragma pack ()
#else
	/* I don't know what would work for WATCOM. */
#endif

/******************************************************************************
	US Geoid 96 file object.  Essentially the same as a csGridFileUS.  However,
	the Geoid 96 algorithm uses the biquadratic calculation techique.  This is
	rather incompatible with the grid cell/caching technique used in the
	standard csGridFileUS capability.  So, we use a very much more direct
	approach here.  There are no "grid cells" and remote equivalent of
	grid cell buffering is accomplished through larger file data buffers.
*/
struct csGeoid96GridFile_
{
	/* These first elements are the same as the csGridFileUS structure,
	   as the file is essentially in the NADCON format. */
	struct csGridCoverage_ coverage;
	long32_t elementCount;
	long32_t recordCount;
	long32_t recordSize;
	double deltaLng;
	double deltaLat;
	long32_t fileSize;
	csFILE* strm;						/* file is not opened until required,
										   may be closed if entire contents
										   have been buffered. */
	long32_t bufferSize;				/* size of buffer allocated */
	long32_t bufferBeginPosition;		/* file position of 1st byte in buffer,
										   -1 says nothing in buffer */
	long32_t bufferEndPosition;				/* file position of last byte in buffer,
										   -1 says nothing in buffer */
	void *dataBuffer;					/* not allocated until required, i.e.
										   file is actually opened. */
	char filePath [MAXPATH];
	char fileName [16];
};
/******************************************************************************
	US Geoid 99 file object.  A variation on the basic theme used in Geoid96
	and elsewhere.  In this case, the file has the same data structure as a
	typical NADCON file, but the format is slightly different.  Note, that
	this structure is virtually identical to the csGeoid96GridFile structure.
	This is possible as the data structure is virtually identical, only the
	format and the access code is different.
*/
struct csGeoid99GridFile_
{
	struct csGridCoverage_ coverage;
	long32_t elementCount;
	long32_t recordCount;
	long32_t recordSize;
	double deltaLng;
	double deltaLat;
	long32_t fileSize;
	csFILE* strm;						/* file is not opened until required,
										   may be closed if entire contents
										   have been buffered. */
	long32_t bufferSize;
	long32_t bufferBeginPosition;		/* file position of 1st byte in buffer,
										   -1 says nothing in buffer */
	long32_t bufferEndPosition;			/* file position of last byte in buffer,
										   -2 says nothing in buffer */
	void *dataBuffer;					/* not allocated until required, i.e.
										   file is actually opened. */
	long32_t iKind;						/* The value extracted from the header
										   prior to swapping.  Generally, a
										   value of one means no swapping
										   required.  A value other than one,
										   specifically 0x01000000 means
										   swapping is required. */
	char filePath [MAXPATH];
	char fileName [32];
};

/*
	The following object is used to access files of the OSTN97.txt type.
	That is, the UK equivalent of NAD83, although it is actually
	ETRS89.  (OSTN97 is the Ordnance Survet National Transformation of 1997.)

	Note, that the coverage element is superfluous and is not used.
	We keep it around, and maintain it with valid data, in case
	it becomes useful in the future.
*/
struct cs_Ostn97_
{
	struct csGridCoverage_ coverage;
	long32_t elementCount;
	long32_t recordCount;
	long32_t recordSize;
	long32_t elementSize;
	csFILE* strm;						/* file is not opened until required,
										   may be closed if entire contents
										   have been buffered. */
	long32_t bufferSize;
	long32_t bufferBeginPosition;		/* file position of 1st byte in buffer,
										   -1 says nothing in buffer */
	long32_t bufferEndPosition;			/* file position of last byte in buffer,
										   -2 says nothing in buffer */
	void *dataBuffer;					/* not allocated until required, i.e.
										   file is actually opened. */
	char filePath [MAXPATH];
	char fileName [32];
};

/*
	The following object is the equivalent Geoid data file companion for the
	OSTN97 object defined above.  Since we integrate this into our generalized
	Geoid Height object, the coverage element is indeed used and must remain
	valid.
*/
struct cs_Osgm91_
{
	struct csGridCoverage_ coverage;
	long32_t elementCount;
	long32_t recordCount;
	long32_t recordSize;
	long32_t elementSize;
	csFILE* strm;						/* file is not opened until required,
										   may be closed if entire contents
										   have been buffered. */
	long32_t bufferSize;
	long32_t bufferBeginPosition;		/* file position of 1st byte in buffer,
										   -1 says nothing in buffer */
	long32_t bufferEndPosition;			/* file position of last byte in buffer,
										   -2 says nothing in buffer */
	void *dataBuffer;					/* not allocated until required, i.e.
										   file is actually opened. */
	char filePath [MAXPATH];
	char fileName [32];
	struct cs_Trmer_ osgb36Trmer;
};

/*
	The following object is used to access files of the OSTN02.txt type.
	That is, the UK equivalent of NAD83, although it is actually
	ETRS89.  (OSTN02 is the Ordnance Survey National Transformation of 2002,
	an update to the OSTN97 transformation implemented above.)
	
	Please note that the format of the file has changed significantly.
	Thus, a whole new set of code had to developed, even though marginally
	different from the previous implementation.  In this case, duplicate
	code was chosen over spaghetti code.

	As we did with the 97 version, we assume that there is one file, and
	that it won't change (famous last words).  Since the file does not
	have any header on it, we hard code the values which would normally
	be coded in a file header if the file had one.
*/
struct cs_Ostn02_
{
	struct csGridCoverage_ coverage;
	long32_t elementCount;
	long32_t recordCount;
	long32_t recordSize;
	long32_t elementSize;
	csFILE* strm;					/* file is not opened until required,
									   may be closed if entire contents
									   have been buffered. */
	long32_t bufferSize;
	long32_t bufferBeginPosition;		/* file position of 1st byte in buffer,
										   -1 says nothing in buffer */
	long32_t bufferEndPosition;			/* file position of last byte in buffer,
										   -2 says nothing in buffer */
	void *dataBuffer;					/* not allocated until required, i.e.
										   file is actually opened. */
	char filePath [MAXPATH];
	char fileName [32];
};

/*
	The following object is the equivalent Geoid data file companion for the
	Earth Gravity Model of 1996 (Egm96).  This data is often supplied as a
	(often) large text file.  It is also distributed as a binary file.  The
	conversion between the two is rather simple, as the text file is a long
	list of doubles, and the binary version is also a long list of doubles
	in binary format.  In the binary case, there is the possibility of a 
	byte swapping problem.  We assume all binary distributions are in the
	little endian form.
	
	Essentially, the data file consists of a six double header which defines
	the coverage of the file and the density of (cell size) of the grid.
	From that we compute the number of elements, number of records, and the
	rest if fairly consistent with the algorithms used elsewhere in CS-MAP.
*/
struct cs_Egm96_
{
	double southWest [2];
	double northEast [2];
	double density [2];
	double searchDensity;
	long32_t headerCount;
	long32_t elementCount;
	long32_t recordCount;
	long32_t recordSize;
	long32_t elementSize;
	csFILE* strm;						/* file is not opened until required,
										   may be closed if entire contents
										   have been buffered. */
	long32_t bufferSize;
	long32_t bufferBeginPosition;		/* file position of 1st byte in buffer,
										   -1 says nothing in buffer */
	long32_t bufferEndPosition;			/* file position of last byte in buffer,
										   -2 says nothing in buffer */
	void *dataBuffer;					/* not allocated until required, i.e.
										   file is actually opened. */
	char filePath [MAXPATH];
	char fileName [32];
};

/* This object represents an implementation of the Byn file
   format.  While used primarily for geoid heights, it is
   also used for things like deflections, elevations, and
   gravitation field strength.
   
   Note, these files can be quite large. */
struct csBynGridFile_
{
	struct csGridCoverage_ coverage;
									/* defines the region covered by this file. */
	long32_t elementCount;
	long32_t recordCount;
	long32_t recordSize;
	double deltaLng;
	double deltaLat;
	double dataFactor;					/* used to convert the integer data to real
										   form. */
	long32_t fileSize;
	csFILE* strm;						/* file is not opened until required,
										   may be closed if entire contents
										   have been buffered. */
	long32_t bufferSize;
	long32_t bufferBeginPosition;		/* file position of 1st byte in buffer,
										   -1 says nothing in buffer */
	long32_t bufferEndPosition;			/* file position of last byte in buffer,
										   -2 says nothing in buffer */
	void *dataBuffer;					/* not allocated until required, i.e.
										   file is actually opened. */
	short fileType;						/* file type from the file header. */
	short elementSize;
	short byteOrder;					/* 0 == Big Endian, 1 == Little Endian,
										   again, from the header file. */
	short swapFlag;						/* Non-zero says the data elements, whatever
										   size they are, need to be swapped. */
	char filePath [MAXPATH];
	char fileName [32];
};

/******************************************************************************
	US Geoid Height Object
	Encspsulates the functionality of converting a WGS84 latitude/longitude
	to a Geoid Height.  Currently, only the US data files are supported.  We
	will have to add several more types in the future. At such time, we will
	adopt a scheme similar to that used for Nad27ToNad83 (in that case we
	support three different file types).
*/
enum csGeoidHeightType {csGeoidHgtTypeNone = 0,
						csGeoidHgtTypeGeoid96,
						csGeoidHgtTypeGeoid99,
						csGeoidHgtTypeOsgm91,
						csGeoidHgtTypeWorld,
						csGeoidHgtTypeBynGridFile,
						csGeoidHgtTypeAustralia,
						csGeoidHgtTypeEgm96
					   };
struct csGeoidHeight_
{
	struct csZGridCellCache_ *cachePtr;
	struct csGeoidHeightEntry_ *listHead;
};

struct csGeoidGridWrld_;		/* dummy for right now. */
struct csGeoidGridAu_;			/* dummy for right now. */

struct csGeoidHeightEntry_
{
	struct csGeoidHeightEntry_ *next;
	enum csGeoidHeightType type;
	union
	{
		struct csGeoid96GridFile_ *geoid96Ptr;
		struct csGeoid99GridFile_ *geoid99Ptr;
		struct cs_Osgm91_ *osgm91Ptr;
		struct csGeoidGridWrld_ *wrldGrid;
		struct csBynGridFile_ *bynGridFilePtr;
		struct csGeoidGridAu_ *australiaGrid;
		struct cs_Egm96_ *egm96Ptr;
	} pointers;
};
/******************************************************************************
	US VertCon Object
	Encspsulates the functionality of converting a NADV29 orthometric height
	to a NADV88 orthometric height.  I'm currently assuming that there is
	only a US version of this stuff.  If I'm wrong, extending to include
	a Canadian version will not be too tough.  Add a type element, and a
	union of pointers, and we're home free (knock on wood).
*/
struct csVertconUS_
{
	struct csVertconUSEntry_ *listHead;
};
struct csVertconUSEntry_
{
	struct csVertconUSEntry_ *next;
	struct cs_NadconFile_ *usGridPtr;
};

/******************************************************************************
	Catalog object
	Supporting datum files are access through a list of files known as a
	catalog.  The catalog is, of course, a text file itself.  This object
	encapsulates the functionality of a catalog file. */
struct csDatumCatalog_
{
	char fileFolder [520];
	char fallback [cs_KEYNM_DEF];
	struct csDatumCatalogEntry_ *listHead;
	char *initialComment;
	char *middleComment;
	char *trailingComment;
};
struct csDatumCatalogEntry_
{
	struct csDatumCatalogEntry_ *next;
	char pathName [520];
	double density;
	long32_t bufferSize;
	ulong32_t flags;
	short relative;
};


#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus  */
/******************************************************************************
	P R O T O T Y P E S
******************************************************************************/
struct csDatumCatalog_* CSnewDatumCatalog (Const char* pathName);
void CSdeleteDatumCatalog (struct csDatumCatalog_* __This);
int CSwriteDatumCatalog (struct csDatumCatalog_* __This,Const char *path);
Const char *CSgetFallbackName (struct csDatumCatalog_ *__This);
int CSaddEntryDataumCatalog (struct csDatumCatalog_ *__This,struct csDatumCatalogEntry_ *entPtr);
int CSdeleteEntryDatumCatalog (struct csDatumCatalog_ *__This,size_t index);
int CSmoveUpDatumCatalog (struct csDatumCatalog_ *__This,size_t index);
int CSmoveDownDatumCatalog (struct csDatumCatalog_ *__This,size_t index);
int CSmakeFirstDatumCatalog (struct csDatumCatalog_ *__This,size_t index);
int CSmakeLastDatumCatalog (struct csDatumCatalog_ *__This,size_t index);
struct csDatumCatalogEntry_* CSgetDatumCatalogEntry (struct csDatumCatalog_ *__This,int index);
struct csDatumCatalogEntry_* CSnewDatumCatalogEntry (Const char* path,short relative,long32_t bufferSize,ulong32_t flags,double density);
void CSdeleteDatumCatalogEntry (struct csDatumCatalogEntry_* __This);
void CSwriteDatumCatalogEntry (struct csDatumCatalogEntry_* __This,csFILE *fstr,Const char *baseDir);

double EXP_LVL9 CSgeoidQterp (double sourceDelta [2],float array [9]);
double EXP_LVL9 CSgeoidQterp1 (double delta,double f0,double f1,double f2);

void CSinitGeoid96 (struct csGeoid96GridFile_ *__This);
void CSdeleteGeoid96GridFile (struct csGeoid96GridFile_* __This);
struct csGeoid96GridFile_* CSnewGeoid96GridFile (Const char *path,long32_t bufferSize,ulong32_t flags,double density);
void CSreleaseGeoid96GridFile (struct csGeoid96GridFile_* __This);
double CStestGeoid96GridFile (struct csGeoid96GridFile_* __This,Const double *sourceLL);
int CSextractGeoid96GridCell (struct csGeoid96GridFile_* __This,Const double* sourceLL);
int CScalcGeoid96GridFile (struct csGeoid96GridFile_* __This,double* result,Const double* sourceLL);

void CSinitGeoid99 (struct csGeoid99GridFile_ *__This);
void CSdeleteGeoid99GridFile (struct csGeoid99GridFile_* __This);
struct csGeoid99GridFile_* CSnewGeoid99GridFile (Const char *path,long32_t bufferSize,ulong32_t flags,double density);
void CSreleaseGeoid99GridFile (struct csGeoid99GridFile_* __This);
double CStestGeoid99GridFile (struct csGeoid99GridFile_* __This,Const double *sourceLL);
int CSextractGeoid99GridCell (struct csGeoid99GridFile_* __This,Const double* sourceLL);
int CScalcGeoid99GridFile (struct csGeoid99GridFile_* __This,double* result,Const double* sourceLL);

void CSinitBynGridFile (struct csBynGridFile_ *__This);
struct csBynGridFile_* CSnewBynGridFile (Const char *path,long32_t bufferSize,ulong32_t flags,double density);
void CSdeleteBynGridFile (struct csBynGridFile_* __This);
void CSreleaseBynGridFile (struct csBynGridFile_* __This);
double CStestBynGridFile (struct csBynGridFile_* __This,Const double *sourceLL);
int CScalcBynGridFile (struct csBynGridFile_* __This,double* result,Const double* sourceLL);

struct cs_DtcXform_ *CSnewFallback (Const char *dtKeyName,Const char* catalog);
void CSdeleteFallback (struct cs_DtcXform_ *__This);
Const char *CSsourceFallback (struct cs_DtcXform_ *__This);
int CScalcFallbackForward (struct cs_DtcXform_ *__This,double trg [3],Const double src [3]);
int CScalcFallbackInverse (struct cs_DtcXform_ *__This,double trg [3],Const double src [3]);

struct csGeoidHeight_* CSnewGeoidHeight (Const char *catalog);
void CSdeleteGeoidHeight (struct csGeoidHeight_* __This);
struct csGeoidHeightEntry_* CSselectGeoidHeight (struct csGeoidHeight_* __This,Const double ll84 [2]);
void CSfirstGeoidHeight (struct csGeoidHeight_* __This,struct csGeoidHeightEntry_* ghEntryPtr);
int CScalcGeoidHeight (struct csGeoidHeight_* __This,double *geoidHgt,Const double ll84 [2]);
void CSreleaseGeoidHeight (struct csGeoidHeight_* __This);
struct csGeoidHeightEntry_* CSnewGeoidHeightEntry (struct csDatumCatalogEntry_* catPtr);
void CSdeleteGeoidHeightEntry (struct csGeoidHeightEntry_* __This);
void CSreleaseGeoidHeightEntry (struct csGeoidHeightEntry_* __This);
double CStestGeoidHeightEntry (struct csGeoidHeightEntry_* __This,Const double ll84 [2]);
int CScalcGeoidHeightEntry (struct csGeoidHeightEntry_* __This,double* geoidHgt,Const double ll84 [2]);

struct csVertconUS_* CSnewVertconUS (Const char *catalog);
void CSdeleteVertconUS (struct csVertconUS_* __This);
struct csVertconUSEntry_* CSselectVertconUS (struct csVertconUS_* __This,Const double ll83 [2]);
void CSfirstVertconUS (struct csVertconUS_* __This,struct csVertconUSEntry_* vcEntryPtr);
int CScalcVertconUS (struct csVertconUS_* __This,double *v88nv29,Const double ll83 [2]);
void CSreleaseVertconUS (struct csVertconUS_* __This);
struct csVertconUSEntry_* CSnewVertconUSEntry (struct csDatumCatalogEntry_* catPtr);
void CSdeleteVertconUSEntry (struct csVertconUSEntry_* __This);
void CSreleaseVertconUSEntry (struct csVertconUSEntry_* __This);
double CStestVertconUSEntry (struct csVertconUSEntry_* __This,Const double ll83 [2]);
int CScalcVertconUSEntry (struct csVertconUSEntry_* __This,double* v88Mv29,Const double ll83 [2]);

struct cs_Ostn97_ *CSnewOstn97 (const char *filePath);
void CSdeleteOstn97 (struct cs_Ostn97_ *__This);
void CSreleaseOstn97 (struct cs_Ostn97_ *__This);
int CSprivateOstn97 (struct cs_Ostn97_ *__This,double result [2],const double etrs89 [2]);
int CSforwardOstn97 (struct cs_Ostn97_ *__This,double osgb36 [2],const double etrs89 [2]);
int CSinverseOstn97 (struct cs_Ostn97_ *__This,double etrs89 [2],const double osgb36 [2]);
int CSmkBinaryOstn97 (struct cs_Ostn97_ *__This);
#ifdef _DEBUG
double CStestOstn97 (struct cs_Ostn97_ *__This);
#endif


struct cs_Ostn02_ *CSnewOstn02 (const char *filePath);
void CSdeleteOstn02 (struct cs_Ostn02_ *__This);
void CSreleaseOstn02 (struct cs_Ostn02_ *__This);
int CSprivateOstn02 (struct cs_Ostn02_ *__This,double result [2],const double etrs89 [2]);
int CSforwardOstn02 (struct cs_Ostn02_ *__This,double osgb36 [2],const double etrs89 [2]);
int CSinverseOstn02 (struct cs_Ostn02_ *__This,double etrs89 [2],const double osgb36 [2]);
int CSmkBinaryOstn02 (struct cs_Ostn02_ *__This);
#ifdef _DEBUG
double CStestOstn02 (struct cs_Ostn02_ *__This);
#endif

struct cs_Osgm91_ *CSnewOsgm91 (const char *filePath,long32_t bufferSize,ulong32_t flags,double density);
void CSdeleteOsgm91 (struct cs_Osgm91_ *__This);
void CSreleaseOsgm91 (struct cs_Osgm91_ *__This);
double CStestOsgm91 (struct cs_Osgm91_ *__This,const double etrs89 [2]);
int CScalcOsgm91 (struct cs_Osgm91_ *__This,double *geoidHgt,const double etrs89 [2]);
int CSmkBinaryOsgm91 (struct cs_Osgm91_ *__This);
#ifdef _DEBUG
double CSdebugOsgm91 (struct cs_Osgm91_ *__This);
#endif

struct cs_Egm96_ *CSnewEgm96 (const char *filePath,long32_t bufferSize,ulong32_t flags,double density);
void CSdeleteEgm96 (struct cs_Egm96_ *__This);
void CSreleaseEgm96 (struct cs_Egm96_ *__This);
double CStestEgm96 (struct cs_Egm96_ *__This,const double wgs84 [2]);
int CScalcEgm96 (struct cs_Egm96_ *__This,double *geoidHgt,const double wgs84 [2]);
int CSmkBinaryEgm96 (struct cs_Egm96_ *__This);
int CSopnBinaryEgm96 (struct cs_Egm96_ *__This,long32_t bufrSize);
double CSdebugEgm96 (struct cs_Egm96_ *__This);

void EXP_LVL5 CS_llhToXyz (double xyz [3],Const double llh [3],double e_rad,double e_sq);
int EXP_LVL5 CS_xyzToLlh (double llh [3],Const double xyz [3],double e_rad,double e_sq);

#if _FILE_SYSTEM == _fs_UNIX
csFILE *CS_fopen (const char *filename,const char *mode);
#endif
/* Note: ftw is only used in the CS_swpal function in the CS_rlsUpdt module.
	 The CS_swpal function is only used in the CStestS.c  function of the CS_Test
	(console test) module.  Since it is not used in the library portion of the product,
	It should be moved to be a function in the CStestSupport.c source  module,
	With a prototype added to the cs_Test.h module.

   Alternatively, the ‘S’ test of the CS_Test module could be removed and
   all references to CS_ftw could then be removed from the product.
*/
#if _RUN_TIME == _rt_MOSXUNIX
#  define FTW_F    0
#  define FTW_D    1
int CS_ftw (const char *path, int (*fn) (const char *,const struct stat *, int), int depth);
#else
#  define CS_ftw   ftw
#endif

#ifdef __cplusplus
}
#endif  /* __cplusplus  */

/*
	CS-MAP uses the define constant here for the indicated values.  This is done
	to enable users to easily replace the normal C run-time library function
	with their own as circumstances require, even though all of these are
	ANSI standard functions.  Of course, CS-MAP uses the ANSI standard calling
	sequences to all of these functions.

	Note, that we need to define the function prototypes before the define,
	otherwise the compiler gets all confused.  Note also, that in CS_ansi,
	where all of these functions are defined (if implemented in CS-MAP),
	we need to undef these defines.
*/

#if !defined (_DEBUG)
#	define CS_getenv  getenv
#	define CS_rename  rename
#	define CS_rewind  rewind
#	define CS_strtod  strtod
#	define CS_strtol  strtol
#	define CS_strtoul strtoul
#endif

/* We do this after everything else. */
#include "cs_clientEnd.h"

#endif	/*__CS_MAPH__ */
