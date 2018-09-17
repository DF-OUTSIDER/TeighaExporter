/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2018, Open Design Alliance (the "Alliance"). 
// All rights reserved. 
// 
// This software and its documentation and related materials are owned by 
// the Alliance. The software may only be incorporated into application 
// programs owned by members of the Alliance, subject to a signed 
// Membership Agreement and Supplemental Software License Agreement with the
// Alliance. The structure and organization of this software are the valuable  
// trade secrets of the Alliance and its suppliers. The software is also 
// protected by copyright law and international treaty provisions. Application  
// programs incorporating this software must include the following statement 
// with their copyright notices:
//   
//   This application incorporates Teigha(R) software pursuant to a license 
//   agreement with Open Design Alliance.
//   Teigha(R) Copyright (C) 2002-2018 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 
// acknowledge and accept the above terms.
///////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// Purpose: Declarations for using STL without warning noise.
//
// Usage: Include this file and define at least one of the STL_USING_xxx
//        macros. Currently supported data types from the STL:
//
//        // defines for using the STL
//        #define STL_USING_ALL
//        #define STL_USING_MAP
//        #define STL_USING_VECTOR
//        #define STL_USING_LIST
//        #define STL_USING_STRING
//        #define STL_USING_STREAM
//        #define STL_USING_IOSTREAM
//        #define STL_USING_MEMORY
//        #define STL_USING_STACK
//        #define STL_USING_ALGORITHM
//        #define STL_USING_FUNCTIONAL
//        #define STL_USING_UTILITY
//        #define STL_USING_SET
//        #define STL_USING_LIMITS
//
// Sample:
//        #define STL_USING_ALL
//        #include "OdaSTL.h"
//
//   this file based on Oskar Wieland (oskar.wieland@gmx.de)  STL.H
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// include guards
//////////////////////////////////////////////////////////////////////

//   !!! Do not prevent the multiple include for this file

//#ifndef _ODA_STLHELPER_INCLUDED_
//#define _ODA_STLHELPER_INCLUDED_


//////////////////////////////////////////////////////////////////////
// handy define to include all stuff
//////////////////////////////////////////////////////////////////////

#ifdef STL_USING_ALL

#define STL_USING_MAP
#define STL_USING_VECTOR
#define STL_USING_LIST
#define STL_USING_STRING
#define STL_USING_STREAM
#define STL_USING_IOSTREAM
#define STL_USING_MEMORY
#define STL_USING_STACK
#define STL_USING_ALGORITHM
#define STL_USING_FUNCTIONAL
#define STL_USING_UTILITY
#define STL_USING_SET
#define STL_USING_LIMITS

#endif


//////////////////////////////////////////////////////////////////////
// STL neccessary declaration for map
//////////////////////////////////////////////////////////////////////

#ifdef STL_USING_MAP

#ifdef _MSC_VER
#pragma warning(push)
#endif // _MSC_VER

#ifdef _MSC_VER
#include <yvals.h>              // warning numbers get enabled in yvals.h 
#endif

#ifdef _MSC_VER
#pragma warning(disable: 4018)  // signed/unsigned mismatch
#pragma warning(disable: 4097)  // typedef-name 'identifier1' used as synonym for class-name 'identifier2'
#pragma warning(disable: 4100)  // unreferenced formal parameter
#pragma warning(disable: 4127)  // conditional expression is constant
#pragma warning(disable: 4146)  // unary minus operator applied to unsigned type, result still unsigned
#pragma warning(disable: 4244)  // 'conversion' conversion from 'type1' to 'type2', possible loss of data
#pragma warning(disable: 4245)  // conversion from 'type1' to 'type2', signed/unsigned mismatch
#pragma warning(disable: 4284)  // return type for 'identifier::operator ->' is not a UDT or reference 
                                // to a UDT. Will produce errors if applied using infix notation
#pragma warning(disable: 4511)  // 'class' : copy constructor could not be generated
#pragma warning(disable: 4512)  // 'class' : assignment operator could not be generated
#pragma warning(disable: 4663)  // C++ language change: to explicitly specialize class template 'vector'
#pragma warning(disable: 4702)  // unreachable code
#pragma warning(disable: 4710)  // 'function' : function not inlined
#pragma warning(disable: 4786)  // identifier was truncated to 'number' characters in the debug information

// BUG: C4786 Warning Is Not Disabled with #pragma Warning
// STATUS: Microsoft has confirmed this to be a bug in the Microsoft product. 
// This warning can be ignored. This occured only in the <map> container.

// MSVC8 /analyze warnings
#if _MSC_VER > 1310
#pragma warning( disable : 6011 )
#pragma warning( disable : 6219 )
#pragma warning( disable : 6221 )
#pragma warning( disable : 6255 )
#pragma warning( disable : 6308 )
#pragma warning( disable : 6320 )
#pragma warning( disable : 6321 )
#pragma warning( disable : 6387 )
#endif // /analyze
#endif // _MSC_VER

#include <map>

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER

#endif  //STL_USING_MAP


//////////////////////////////////////////////////////////////////////
// STL neccessary declaration for vector
//////////////////////////////////////////////////////////////////////

#ifdef STL_USING_VECTOR

#ifdef _MSC_VER
#pragma warning(push)
#endif // _MSC_VER

#ifdef _MSC_VER
#include <yvals.h>              // warning numbers get enabled in yvals.h 
#endif

#ifdef _MSC_VER
#pragma warning(disable: 4018)  // signed/unsigned mismatch
#pragma warning(disable: 4097)  // typedef-name 'identifier1' used as synonym for class-name 'identifier2'
#pragma warning(disable: 4100)  // unreferenced formal parameter
#pragma warning(disable: 4127)  // conditional expression is constant
#pragma warning(disable: 4146)  // unary minus operator applied to unsigned type, result still unsigned
#pragma warning(disable: 4244)  // 'conversion' conversion from 'type1' to 'type2', possible loss of data
#pragma warning(disable: 4245)  // conversion from 'type1' to 'type2', signed/unsigned mismatch
#pragma warning(disable: 4284)  // return type for 'identifier::operator ->' is not a UDT or reference 
                                // to a UDT. Will produce errors if applied using infix notation
#pragma warning(disable: 4511)  // 'class' : copy constructor could not be generated
#pragma warning(disable: 4512)  // 'class' : assignment operator could not be generated
#pragma warning(disable: 4663)  // C++ language change: to explicitly specialize class template 'vector'
#pragma warning(disable: 4702)  // unreachable code
#pragma warning(disable: 4710)  // 'function' : function not inlined
#pragma warning(disable: 4786)  // identifier was truncated to 'number' characters in the debug information

// MSVC8 /analyze warnings
#if _MSC_VER > 1310
#pragma warning( disable : 6011 )
#pragma warning( disable : 6219 )
#pragma warning( disable : 6221 )
#pragma warning( disable : 6255 )
#pragma warning( disable : 6308 )
#pragma warning( disable : 6320 )
#pragma warning( disable : 6321 )
#pragma warning( disable : 6387 )
#endif // /analyze
#endif 

#include <vector>

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER

#endif  // STL_USING_VECTOR


//////////////////////////////////////////////////////////////////////
// STL neccessary declaration for list
//////////////////////////////////////////////////////////////////////

#ifdef STL_USING_LIST

#ifdef _MSC_VER
#pragma warning(push)
#endif // _MSC_VER

#ifdef _MSC_VER
#include <yvals.h>              // warning numbers get enabled in yvals.h 
#endif

#ifdef _MSC_VER
#pragma warning(disable: 4018)  // signed/unsigned mismatch
#pragma warning(disable: 4097)  // typedef-name 'identifier1' used as synonym for class-name 'identifier2'
#pragma warning(disable: 4100)  // unreferenced formal parameter
#pragma warning(disable: 4127)  // conditional expression is constant
#pragma warning(disable: 4146)  // unary minus operator applied to unsigned type, result still unsigned
#pragma warning(disable: 4244)  // 'conversion' conversion from 'type1' to 'type2', possible loss of data
#pragma warning(disable: 4245)  // conversion from 'type1' to 'type2', signed/unsigned mismatch
#pragma warning(disable: 4284)  // return type for 'identifier::operator ->' is not a UDT or reference 
                                // to a UDT. Will produce errors if applied using infix notation
#pragma warning(disable: 4511)  // 'class' : copy constructor could not be generated
#pragma warning(disable: 4512)  // 'class' : assignment operator could not be generated
#pragma warning(disable: 4663)  // C++ language change: to explicitly specialize class template 'vector'
#pragma warning(disable: 4702)  // unreachable code
#pragma warning(disable: 4710)  // 'function' : function not inlined
#pragma warning(disable: 4786)  // identifier was truncated to 'number' characters in the debug information
// MSVC8 /analyze warnings
#if _MSC_VER > 1310
#pragma warning( disable : 6011 )
#pragma warning( disable : 6219 )
#pragma warning( disable : 6221 )
#pragma warning( disable : 6255 )
#pragma warning( disable : 6308 )
#pragma warning( disable : 6320 )
#pragma warning( disable : 6321 )
#pragma warning( disable : 6387 )
#endif // /analyze
#endif

#include <list>

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER

#endif  // STL_USING_LIST


//////////////////////////////////////////////////////////////////////
// STL neccessary declaration for string
//////////////////////////////////////////////////////////////////////

#ifdef STL_USING_STRING

#ifdef _MSC_VER
#pragma warning(push)
#endif // _MSC_VER

#ifdef _MSC_VER
#include <yvals.h>              // warning numbers get enabled in yvals.h 
#endif

#ifdef _MSC_VER
#pragma warning(disable: 4018)  // signed/unsigned mismatch
#pragma warning(disable: 4097)  // typedef-name 'identifier1' used as synonym for class-name 'identifier2'
#pragma warning(disable: 4100)  // unreferenced formal parameter
#pragma warning(disable: 4127)  // conditional expression is constant
#pragma warning(disable: 4146)  // unary minus operator applied to unsigned type, result still unsigned
#pragma warning(disable: 4244)  // 'conversion' conversion from 'type1' to 'type2', possible loss of data
#pragma warning(disable: 4245)  // conversion from 'type1' to 'type2', signed/unsigned mismatch
#pragma warning(disable: 4284)  // return type for 'identifier::operator ->' is not a UDT or reference 
                                // to a UDT. Will produce errors if applied using infix notation
#pragma warning(disable: 4511)  // 'class' : copy constructor could not be generated
#pragma warning(disable: 4512)  // 'class' : assignment operator could not be generated
#pragma warning(disable: 4663)  // C++ language change: to explicitly specialize class template 'vector'
#pragma warning(disable: 4702)  // unreachable code
#pragma warning(disable: 4710)  // 'function' : function not inlined
#pragma warning(disable: 4786)  // identifier was truncated to 'number' characters in the debug information
// MSVC8 /analyze warnings
#if _MSC_VER > 1310
#pragma warning( disable : 6011 )
#pragma warning( disable : 6219 )
#pragma warning( disable : 6221 )
#pragma warning( disable : 6255 )
#pragma warning( disable : 6308 )
#pragma warning( disable : 6320 )
#pragma warning( disable : 6321 )
#pragma warning( disable : 6387 )
#endif // /analyze
#endif 

#include <string>

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER

#ifdef _MSC_VER
#pragma warning(disable: 4514)  // unreferenced inline/local function has been removed
#pragma warning(disable: 4710)  // 'function' : function not inlined
#pragma warning(disable: 4786)  // identifier was truncated to 'number' characters 
                                // in the debug information
#endif 

#endif  // STL_USING_STRING


//////////////////////////////////////////////////////////////////////
// STL neccessary declaration for streams
//////////////////////////////////////////////////////////////////////

#ifdef STL_USING_STREAM

#ifdef _MSC_VER
#pragma warning(push)
#endif // _MSC_VER

#ifdef _MSC_VER
#include <yvals.h>              // warning numbers get enabled in yvals.h 
#endif

#ifdef _MSC_VER
#pragma warning(disable: 4018)  // signed/unsigned mismatch
#pragma warning(disable: 4097)  // typedef-name 'identifier1' used as synonym for class-name 'identifier2'
#pragma warning(disable: 4100)  // unreferenced formal parameter
#pragma warning(disable: 4127)  // conditional expression is constant
#pragma warning(disable: 4146)  // unary minus operator applied to unsigned type, result still unsigned
#pragma warning(disable: 4244)  // 'conversion' conversion from 'type1' to 'type2', possible loss of data
#pragma warning(disable: 4245)  // conversion from 'type1' to 'type2', signed/unsigned mismatch
#pragma warning(disable: 4284)  // return type for 'identifier::operator ->' is not a UDT or reference 
                                // to a UDT. Will produce errors if applied using infix notation
#pragma warning(disable: 4511)  // 'class' : copy constructor could not be generated
#pragma warning(disable: 4512)  // 'class' : assignment operator could not be generated
#pragma warning(disable: 4663)  // C++ language change: to explicitly specialize class template 'vector'
#pragma warning(disable: 4702)  // unreachable code
#pragma warning(disable: 4710)  // 'function' : function not inlined
#pragma warning(disable: 4786)  // identifier was truncated to 'number' characters in the debug information
// MSVC8 /analyze warnings
#if _MSC_VER > 1310
#pragma warning( disable : 6011 )
#pragma warning( disable : 6219 )
#pragma warning( disable : 6221 )
#pragma warning( disable : 6255 )
#pragma warning( disable : 6308 )
#pragma warning( disable : 6320 )
#pragma warning( disable : 6321 )
#pragma warning( disable : 6387 )
#endif // /analyze
#endif 

#ifdef OD_HAVE_SSTREAM_FILE
#include <sstream>
#endif

#ifdef OD_HAVE_FSTREAM_FILE
#include <fstream>
#endif

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER

#endif //STL_USING_STREAM


//////////////////////////////////////////////////////////////////////
// STL neccessary declaration for input/output streams
//////////////////////////////////////////////////////////////////////

#ifdef STL_USING_IOSTREAM

#ifdef _MSC_VER
#pragma warning(push)
#endif // _MSC_VER

#ifdef _MSC_VER
#include <yvals.h>              // warning numbers get enabled in yvals.h 
#endif

#ifdef _MSC_VER
#pragma warning(disable: 4018)  // signed/unsigned mismatch
#pragma warning(disable: 4097)  // typedef-name 'identifier1' used as synonym for class-name 'identifier2'
#pragma warning(disable: 4100)  // unreferenced formal parameter
#pragma warning(disable: 4127)  // conditional expression is constant
#pragma warning(disable: 4146)  // unary minus operator applied to unsigned type, result still unsigned
#pragma warning(disable: 4244)  // 'conversion' conversion from 'type1' to 'type2', possible loss of data
#pragma warning(disable: 4245)  // conversion from 'type1' to 'type2', signed/unsigned mismatch
#pragma warning(disable: 4284)  // return type for 'identifier::operator ->' is not a UDT or reference 
                                // to a UDT. Will produce errors if applied using infix notation
#pragma warning(disable: 4511)  // 'class' : copy constructor could not be generated
#pragma warning(disable: 4512)  // 'class' : assignment operator could not be generated
#pragma warning(disable: 4663)  // C++ language change: to explicitly specialize class template 'vector'
#pragma warning(disable: 4702)  // unreachable code
#pragma warning(disable: 4710)  // 'function' : function not inlined
#pragma warning(disable: 4786)  // identifier was truncated to 'number' characters in the debug information
// MSVC8 /analyze warnings
#if _MSC_VER > 1310
#pragma warning( disable : 6011 )
#pragma warning( disable : 6219 )
#pragma warning( disable : 6221 )
#pragma warning( disable : 6255 )
#pragma warning( disable : 6308 )
#pragma warning( disable : 6320 )
#pragma warning( disable : 6321 )
#pragma warning( disable : 6387 )
#endif // /analyze
#endif 

#include <iostream>
#ifdef OD_HAVE_IOMANIP_FILE
#include <iomanip>
#endif

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER

#endif //STL_USING_IOSTREAM

//////////////////////////////////////////////////////////////////////
// STL neccessary declaration for memory
//////////////////////////////////////////////////////////////////////

#ifdef STL_USING_MEMORY

// The STL library provides a type called auto_ptr for managing pointers.  
// This template class acts as a stack variable for dynamically allocated 
// memory.  When the variable goes out of scope, its destructor gets called.  
// In its destructor, it calls delete on the contained pointer, making sure 
// that the memory is returned to the heap.

#ifdef _MSC_VER
#pragma warning(push)
#endif // _MSC_VER

#ifdef _MSC_VER
#include <yvals.h>              // warning numbers get enabled in yvals.h 
#endif

#ifdef _MSC_VER
#pragma warning(disable: 4018)  // signed/unsigned mismatch
#pragma warning(disable: 4097)  // typedef-name 'identifier1' used as synonym for class-name 'identifier2'
#pragma warning(disable: 4100)  // unreferenced formal parameter
#pragma warning(disable: 4127)  // conditional expression is constant
#pragma warning(disable: 4146)  // unary minus operator applied to unsigned type, result still unsigned
#pragma warning(disable: 4244)  // 'conversion' conversion from 'type1' to 'type2', possible loss of data
#pragma warning(disable: 4245)  // conversion from 'type1' to 'type2', signed/unsigned mismatch
#pragma warning(disable: 4284)  // return type for 'identifier::operator ->' is not a UDT or reference 
                                // to a UDT. Will produce errors if applied using infix notation
#pragma warning(disable: 4511)  // 'class' : copy constructor could not be generated
#pragma warning(disable: 4512)  // 'class' : assignment operator could not be generated
#pragma warning(disable: 4663)  // C++ language change: to explicitly specialize class template 'vector'
#pragma warning(disable: 4702)  // unreachable code
#pragma warning(disable: 4710)  // 'function' : function not inlined
#pragma warning(disable: 4786)  // identifier was truncated to 'number' characters in the debug information
// MSVC8 /analyze warnings
#if _MSC_VER > 1310
#pragma warning( disable : 6011 )
#pragma warning( disable : 6219 )
#pragma warning( disable : 6221 )
#pragma warning( disable : 6255 )
#pragma warning( disable : 6308 )
#pragma warning( disable : 6320 )
#pragma warning( disable : 6321 )
#pragma warning( disable : 6387 )
#endif // /analyze
#endif 

#include <memory>

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER

#endif //STL_USING_MEMORY


//////////////////////////////////////////////////////////////////////
// STL neccessary declaration for stack
//////////////////////////////////////////////////////////////////////

#ifdef STL_USING_STACK

#ifdef _MSC_VER
#pragma warning(push)
#endif // _MSC_VER

#ifdef _MSC_VER
#include <yvals.h>              // warning numbers get enabled in yvals.h 
#endif

#ifdef _MSC_VER
#pragma warning(disable: 4018)  // signed/unsigned mismatch
#pragma warning(disable: 4097)  // typedef-name 'identifier1' used as synonym for class-name 'identifier2'
#pragma warning(disable: 4100)  // unreferenced formal parameter
#pragma warning(disable: 4127)  // conditional expression is constant
#pragma warning(disable: 4146)  // unary minus operator applied to unsigned type, result still unsigned
#pragma warning(disable: 4244)  // 'conversion' conversion from 'type1' to 'type2', possible loss of data
#pragma warning(disable: 4245)  // conversion from 'type1' to 'type2', signed/unsigned mismatch
#pragma warning(disable: 4284)  // return type for 'identifier::operator ->' is not a UDT or reference 
                                // to a UDT. Will produce errors if applied using infix notation
#pragma warning(disable: 4511)  // 'class' : copy constructor could not be generated
#pragma warning(disable: 4512)  // 'class' : assignment operator could not be generated
#pragma warning(disable: 4663)  // C++ language change: to explicitly specialize class template 'vector'
#pragma warning(disable: 4702)  // unreachable code
#pragma warning(disable: 4710)  // 'function' : function not inlined
#pragma warning(disable: 4786)  // identifier was truncated to 'number' characters in the debug information
// MSVC8 /analyze warnings
#if _MSC_VER > 1310
#pragma warning( disable : 6011 )
#pragma warning( disable : 6219 )
#pragma warning( disable : 6221 )
#pragma warning( disable : 6255 )
#pragma warning( disable : 6308 )
#pragma warning( disable : 6320 )
#pragma warning( disable : 6321 )
#pragma warning( disable : 6387 )
#endif // /analyze
#endif 

#include <stack>

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER

#endif // STL_USING_STACK


#ifdef STL_USING_QUEUE

#ifdef _MSC_VER
#pragma warning(push)
#endif // _MSC_VER

#ifdef _MSC_VER
#include <yvals.h>              // warning numbers get enabled in yvals.h 
#endif

#ifdef _MSC_VER
#pragma warning(disable: 4018)  // signed/unsigned mismatch
#pragma warning(disable: 4097)  // typedef-name 'identifier1' used as synonym for class-name 'identifier2'
#pragma warning(disable: 4100)  // unreferenced formal parameter
#pragma warning(disable: 4127)  // conditional expression is constant
#pragma warning(disable: 4146)  // unary minus operator applied to unsigned type, result still unsigned
#pragma warning(disable: 4244)  // 'conversion' conversion from 'type1' to 'type2', possible loss of data
#pragma warning(disable: 4245)  // conversion from 'type1' to 'type2', signed/unsigned mismatch
#pragma warning(disable: 4284)  // return type for 'identifier::operator ->' is not a UDT or reference 
// to a UDT. Will produce errors if applied using infix notation
#pragma warning(disable: 4511)  // 'class' : copy constructor could not be generated
#pragma warning(disable: 4512)  // 'class' : assignment operator could not be generated
#pragma warning(disable: 4663)  // C++ language change: to explicitly specialize class template 'vector'
#pragma warning(disable: 4702)  // unreachable code
#pragma warning(disable: 4710)  // 'function' : function not inlined
#pragma warning(disable: 4786)  // identifier was truncated to 'number' characters in the debug information
// MSVC8 /analyze warnings
#if _MSC_VER > 1310
#pragma warning( disable : 6011 )
#pragma warning( disable : 6219 )
#pragma warning( disable : 6221 )
#pragma warning( disable : 6255 )
#pragma warning( disable : 6308 )
#pragma warning( disable : 6320 )
#pragma warning( disable : 6321 )
#pragma warning( disable : 6387 )
#endif // /analyze
#endif 

#include <queue>

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER

#endif // STL_USING_STACK

//////////////////////////////////////////////////////////////////////
// STL neccessary declaration for algorithm
//////////////////////////////////////////////////////////////////////

#ifdef STL_USING_ALGORITHM

#ifdef _MSC_VER
#pragma warning(push)
#endif // _MSC_VER

#ifdef _MSC_VER
#include <yvals.h>              // warning numbers get enabled in yvals.h 
#endif

#ifdef _MSC_VER
#pragma warning(disable: 4018)  // signed/unsigned mismatch
#pragma warning(disable: 4097)  // typedef-name 'identifier1' used as synonym for class-name 'identifier2'
#pragma warning(disable: 4100)  // unreferenced formal parameter
#pragma warning(disable: 4127)  // conditional expression is constant
#pragma warning(disable: 4146)  // unary minus operator applied to unsigned type, result still unsigned
#pragma warning(disable: 4244)  // 'conversion' conversion from 'type1' to 'type2', possible loss of data
#pragma warning(disable: 4245)  // conversion from 'type1' to 'type2', signed/unsigned mismatch
#pragma warning(disable: 4284)  // return type for 'identifier::operator ->' is not a UDT or reference 
                                // to a UDT. Will produce errors if applied using infix notation
#pragma warning(disable: 4511)  // 'class' : copy constructor could not be generated
#pragma warning(disable: 4512)  // 'class' : assignment operator could not be generated
#pragma warning(disable: 4663)  // C++ language change: to explicitly specialize class template 'vector'
#pragma warning(disable: 4702)  // unreachable code
#pragma warning(disable: 4710)  // 'function' : function not inlined
#pragma warning(disable: 4786)  // identifier was truncated to 'number' characters in the debug information
// MSVC8 /analyze warnings
#if _MSC_VER > 1310
#pragma warning( disable : 6011 )
#pragma warning( disable : 6219 )
#pragma warning( disable : 6221 )
#pragma warning( disable : 6255 )
#pragma warning( disable : 6308 )
#pragma warning( disable : 6320 )
#pragma warning( disable : 6321 )
#pragma warning( disable : 6387 )
#endif // /analyze
#endif 

#include <algorithm>
#include <iterator>

// in STLPORT used with BCC 5.6 std::remove clashes with remove(char*) from iostream
#ifdef __SGI_STL_PORT
/** \details
  
    <group TD_Namespaces>
*/
/*!DOM*/
namespace std 
{
  using _STLP_STD::remove;
}
#endif

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER

#endif // STL_USING_ALGORITHM

//////////////////////////////////////////////////////////////////////
// STL neccessary declaration for functional
//////////////////////////////////////////////////////////////////////

#ifdef STL_USING_FUNCTIONAL

#ifdef _MSC_VER
#pragma warning(push)
#endif // _MSC_VER

#ifdef _MSC_VER
#include <yvals.h>              // warning numbers get enabled in yvals.h 
#endif

#ifdef _MSC_VER
#pragma warning(disable: 4018)  // signed/unsigned mismatch
#pragma warning(disable: 4097)  // typedef-name 'identifier1' used as synonym for class-name 'identifier2'
#pragma warning(disable: 4100)  // unreferenced formal parameter
#pragma warning(disable: 4127)  // conditional expression is constant
#pragma warning(disable: 4146)  // unary minus operator applied to unsigned type, result still unsigned
#pragma warning(disable: 4244)  // 'conversion' conversion from 'type1' to 'type2', possible loss of data
#pragma warning(disable: 4245)  // conversion from 'type1' to 'type2', signed/unsigned mismatch
#pragma warning(disable: 4284)  // return type for 'identifier::operator ->' is not a UDT or reference 
                                // to a UDT. Will produce errors if applied using infix notation
#pragma warning(disable: 4511)  // 'class' : copy constructor could not be generated
#pragma warning(disable: 4512)  // 'class' : assignment operator could not be generated
#pragma warning(disable: 4663)  // C++ language change: to explicitly specialize class template 'vector'
#pragma warning(disable: 4702)  // unreachable code
#pragma warning(disable: 4710)  // 'function' : function not inlined
#pragma warning(disable: 4786)  // identifier was truncated to 'number' characters in the debug information
// MSVC8 /analyze warnings
#if _MSC_VER > 1310
#pragma warning( disable : 6011 )
#pragma warning( disable : 6219 )
#pragma warning( disable : 6221 )
#pragma warning( disable : 6255 )
#pragma warning( disable : 6308 )
#pragma warning( disable : 6320 )
#pragma warning( disable : 6321 )
#pragma warning( disable : 6387 )
#endif // /analyze
#endif 

#include <functional>

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER

#endif // STL_USING_FUNCTIONAL


//////////////////////////////////////////////////////////////////////
// STL neccessary declaration for utilities
//////////////////////////////////////////////////////////////////////

#ifdef STL_USING_UTILITY

#ifdef _MSC_VER
#pragma warning(push)
#endif // _MSC_VER

#ifdef _MSC_VER
#include <yvals.h>              // warning numbers get enabled in yvals.h 
#endif

#ifdef _MSC_VER
#pragma warning(disable: 4018)  // signed/unsigned mismatch
#pragma warning(disable: 4097)  // typedef-name 'identifier1' used as synonym for class-name 'identifier2'
#pragma warning(disable: 4100)  // unreferenced formal parameter
#pragma warning(disable: 4127)  // conditional expression is constant
#pragma warning(disable: 4146)  // unary minus operator applied to unsigned type, result still unsigned
#pragma warning(disable: 4244)  // 'conversion' conversion from 'type1' to 'type2', possible loss of data
#pragma warning(disable: 4245)  // conversion from 'type1' to 'type2', signed/unsigned mismatch
#pragma warning(disable: 4284)  // return type for 'identifier::operator ->' is not a UDT or reference 
                                // to a UDT. Will produce errors if applied using infix notation
#pragma warning(disable: 4511)  // 'class' : copy constructor could not be generated
#pragma warning(disable: 4512)  // 'class' : assignment operator could not be generated
#pragma warning(disable: 4663)  // C++ language change: to explicitly specialize class template 'vector'
#pragma warning(disable: 4702)  // unreachable code
#pragma warning(disable: 4710)  // 'function' : function not inlined
#pragma warning(disable: 4786)  // identifier was truncated to 'number' characters in the debug information
// MSVC8 /analyze warnings
#if _MSC_VER > 1310
#pragma warning( disable : 6011 )
#pragma warning( disable : 6219 )
#pragma warning( disable : 6221 )
#pragma warning( disable : 6255 )
#pragma warning( disable : 6308 )
#pragma warning( disable : 6320 )
#pragma warning( disable : 6321 )
#pragma warning( disable : 6387 )
#endif // /analyze
#endif 

#include <utility>

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER

#endif // STL_USING_FUNCTIONAL

#ifdef STL_USING_SET

#ifdef _MSC_VER
#pragma warning(push)
#endif // _MSC_VER

#ifdef _MSC_VER
#include <yvals.h>              // warning numbers get enabled in yvals.h 
#endif

#ifdef _MSC_VER
#pragma warning(disable: 4018)  // signed/unsigned mismatch
#pragma warning(disable: 4097)  // typedef-name 'identifier1' used as synonym for class-name 'identifier2'
#pragma warning(disable: 4100)  // unreferenced formal parameter
#pragma warning(disable: 4127)  // conditional expression is constant
#pragma warning(disable: 4146)  // unary minus operator applied to unsigned type, result still unsigned
#pragma warning(disable: 4244)  // 'conversion' conversion from 'type1' to 'type2', possible loss of data
#pragma warning(disable: 4245)  // conversion from 'type1' to 'type2', signed/unsigned mismatch
#pragma warning(disable: 4284)  // return type for 'identifier::operator ->' is not a UDT or reference 
                                // to a UDT. Will produce errors if applied using infix notation
#pragma warning(disable: 4511)  // 'class' : copy constructor could not be generated
#pragma warning(disable: 4512)  // 'class' : assignment operator could not be generated
#pragma warning(disable: 4702)  // unreachable code
#pragma warning(disable: 4710)  // 'function' : function not inlined
#pragma warning(disable: 4786)  // identifier was truncated to 'number' characters in the debug information
// MSVC8 /analyze warnings
#if _MSC_VER > 1310
#pragma warning( disable : 6011 )
#pragma warning( disable : 6219 )
#pragma warning( disable : 6221 )
#pragma warning( disable : 6255 )
#pragma warning( disable : 6308 )
#pragma warning( disable : 6320 )
#pragma warning( disable : 6321 )
#pragma warning( disable : 6387 )
#endif // /analyze
#endif 

#include <set>

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER

#endif  // STL_USING_SET

#ifdef STL_USING_LIMITS

#ifdef _MSC_VER
#pragma warning(push)
#include <yvals.h>              // warning numbers get enabled in yvals.h 
#pragma warning(disable: 4663)  // C++ language change: to explicitly specialize class template ...

#undef min // in windows header min & max are defined
#undef max

#endif // _MSC_VER

#include <limits>

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER

#endif  // STL_USING_LIMITS


//////////////////////////////////////////////////////////////////////
// verify proper use of macros
//////////////////////////////////////////////////////////////////////

#if defined(STL_USING_MAP)    || defined(STL_USING_VECTOR) || defined(STL_USING_LIST)      || \
    defined(STL_USING_STRING) || defined(STL_USING_STREAM) || defined STL_USING_FUNCTIONAL || \
    defined(STL_USING_MEMORY) || defined(STL_USING_STACK)  || defined STL_USING_ALGORITHM  || \
    defined(STL_USING_UTILITY) || defined(STL_USING_IOSTREAM) || defined(STL_USING_SET) || \
    defined(STL_USING_LIMITS)

#define _ODA_STL_

#else
#pragma message( "Warning: You included \"OdaSTL.H\" without using any STL features!" )

#endif

//#endif  // _ODA_STLHELPER_INCLUDED_

