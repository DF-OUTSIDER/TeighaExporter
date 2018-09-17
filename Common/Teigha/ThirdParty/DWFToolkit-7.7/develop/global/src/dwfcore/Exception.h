//
//  Copyright (c) 2003-2006 by Autodesk, Inc.
//
//  By using this code, you are agreeing to the terms and conditions of
//  the License Agreement included in the documentation for this code.
//
//  AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED,
//  AS TO THE CORRECTNESS OF THIS CODE OR ANY DERIVATIVE
//  WORKS WHICH INCORPORATE IT.
//
//  AUTODESK PROVIDES THE CODE ON AN "AS-IS" BASIS
//  AND EXPLICITLY DISCLAIMS ANY LIABILITY, INCLUDING
//  CONSEQUENTIAL AND INCIDENTAL DAMAGES FOR ERRORS,
//  OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
//
//  Use, duplication, or disclosure by the U.S. Government is subject to
//  restrictions set forth in FAR 52.227-19 (Commercial Computer Software
//  Restricted Rights) and DFAR 252.227-7013(c)(1)(ii) (Rights in Technical
//  Data and Computer Software), as applicable.
//


#ifndef _DWFCORE_EXCEPTION_H
#define _DWFCORE_EXCEPTION_H


///
///\file        dwfcore/Exception.h
///\brief       This header contains the declaration for the DWF exception classes.
///

#include "dwfcore/Core.h"


namespace DWFCore
{

///
///\class       DWFException    dwfcore/Exception.h  "dwfcore/Exception.h"
///\brief       Base class for all exceptions.
///\since       1.0.1
///
///             An exception class that contains information detailing
///             the error event.  
///
class DWFException  : virtual public DWFCoreMemory
{

public:

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFCORE_API
    virtual ~DWFException()
        throw();

    ///
    ///         Copy Constructor
    ///         This constructor will only make a copy of the \e message string member.
    ///         Other member pointers will be copied by reference since they should
    ///         should be string literals.
    ///
    ///\throw   None
    ///
    _DWFCORE_API
    DWFException( const DWFException& )
        throw();

    ///
    ///         Assignment Operator
    ///         This operator will only make a copy of the \e message string member.
    ///         Other member pointers will be copied by reference since they should
    ///         should be string literals.
    ///
    ///\throw   None
    ///
    _DWFCORE_API
    DWFException& operator=( const DWFException& )
        throw();

    ///
    ///         Returns a wide character string describing the exception type,
    ///         usually the class name.
    ///
    ///\return  The type string.
    ///\throw   None
    ///
#if defined(__hpux)
    _DWFCORE_API virtual const wchar_t* type() const throw() { return 0; } // required for compilation on hp
#else
    _DWFCORE_API virtual const wchar_t* type() const throw() = 0;
#endif

    ///
    ///         Returns a wide character string describing the exception event.
    ///
    ///\return  The message string.
    ///\throw   None
    ///
    _DWFCORE_API
    const wchar_t* message() const
        throw();

    ///
    ///         Returns a character string describing the function in which
    ///         the exception occured.
    ///
    ///\return  The function name.
    ///\throw   None
    ///
    _DWFCORE_API
    const char* function() const
        throw();

    ///
    ///         Returns a character string describing the file in which
    ///         the exception occured.
    ///
    ///\return  The file name.
    ///\throw   None
    ///
    _DWFCORE_API
    const wchar_t* file() const
        throw();

    ///
    ///         Returns the line in the file in which the exception occured.
    ///
    ///\return  The line number.
    ///\throw   None
    ///
    _DWFCORE_API
    unsigned int line() const
        throw();

protected:

    ///
    ///         Constructor
    ///
    ///\param   zMessage    Describes the exception event.
    ///                     This string is copied into a local buffer and is limited to \b 512 wide characters.
    ///\param   zFunction   The function in which the event occured.  The \b __func__ macro is recommended.
    ///\param   zFile       The file in which the event occured. The \b __FILE__ macro is recommended.
    ///\param   nLine       The line number in the file in which the event occured.  The \b __LINE__ macro is recommended.
    ///\throw   None
    ///
    _DWFCORE_API DWFException( const wchar_t* zMessage,
                               const char*    zFunction,
                               const wchar_t* zFile,
                               unsigned int   nLine )
        throw();

private:

    wchar_t         _zMessage[512];
    const char*     _zFunction;
    const wchar_t*  _zFile;
    unsigned int    _nLine;
};



#define _DWFCORE_STRING_ECHO( s )           #s
#define _DWFCORE_FORCE_WIDE_STRING_( s )    L ## s
#define _DWFCORE_FORCE_WIDE_STRING( s )     _DWFCORE_FORCE_WIDE_STRING_( s )


///
///\ingroup     InternalMacros
///\brief       Declares a new exception class.
///
///             This macro can be used to create new exception classes.
///             It provides the definition of \a type() as well.
///
#define _DWFCORE_DECLARE_EXCEPTION_CLASS( this, super )                                             \
class DWF##this##Exception : public super                                                           \
{                                                                                                   \
public:                                                                                             \
                 DWF##this##Exception(const wchar_t* zMessage,                                      \
                                      const char*    zFunction,                                     \
                                      const wchar_t* zFile,                                         \
                                      unsigned int   nLine )                                        \
        : super(zMessage, zFunction, zFile, nLine) {;}                                              \
                 virtual ~DWF##this##Exception() throw() {;}                                        \
                 virtual const wchar_t* type() const                                                \
    throw() { return _DWFCORE_FORCE_WIDE_STRING(_DWFCORE_STRING_ECHO(DWF##this##Exception)); }      \
};

///
///\class       DWFIOException  dwfcore/Exception.h "dwfcore/Exception.h"
///\brief       Class for I/O operation exceptions.
///\since       1.0.1
///
_DWFCORE_DECLARE_EXCEPTION_CLASS( IO, DWFException )

///
///\class       DWFMemoryException  dwfcore/Exception.h "dwfcore/Exception.h"
///\brief       Class for memory allocation exceptions.
///\since       1.0.1
///
_DWFCORE_DECLARE_EXCEPTION_CLASS( Memory, DWFException )

///
///\class       DWFOverflowException  dwfcore/Exception.h "dwfcore/Exception.h"
///\brief       Class for string and buffer overflow exceptions.
///\since       1.0.1
///
_DWFCORE_DECLARE_EXCEPTION_CLASS( Overflow, DWFException )

///
///\class       DWFStringException  dwfcore/Exception.h "dwfcore/Exception.h"
///\brief       Class for general string manipulation exceptions.
///\since       1.0.1
///
_DWFCORE_DECLARE_EXCEPTION_CLASS( String, DWFException )

///
///\class       DWFUnexpectedException  dwfcore/Exception.h "dwfcore/Exception.h"
///\brief       Class for unexpected or unknown exceptions.
///\since       1.0.1
///
_DWFCORE_DECLARE_EXCEPTION_CLASS( Unexpected, DWFException )

///
///\class       DWFNullPointerException  dwfcore/Exception.h "dwfcore/Exception.h"
///\brief       Class for access violation exceptions.
///\since       1.0.1
///
_DWFCORE_DECLARE_EXCEPTION_CLASS( NullPointer, DWFException )

///
///\class       DWFInvalidTypeException  dwfcore/Exception.h "dwfcore/Exception.h"
///\brief       Class for type specification exceptions.
///\since       1.0.1
///
_DWFCORE_DECLARE_EXCEPTION_CLASS( InvalidType, DWFException )

///
///\class       DWFDoesNotExistException  dwfcore/Exception.h "dwfcore/Exception.h"
///\brief       Class for missing object exceptions.
///\since       1.0.1
///
_DWFCORE_DECLARE_EXCEPTION_CLASS( DoesNotExist, DWFException )

///
///\class       DWFTypeMismatchException  dwfcore/Exception.h "dwfcore/Exception.h"
///\brief       Class for mismatched type exceptions.
///\since       1.0.1
///
_DWFCORE_DECLARE_EXCEPTION_CLASS( TypeMismatch, DWFException )

///
///\class       DWFInvalidArgumentException  dwfcore/Exception.h "dwfcore/Exception.h"
///\brief       Class for bad function arguments exceptions.
///\since       1.0.1
///
_DWFCORE_DECLARE_EXCEPTION_CLASS( InvalidArgument, DWFException )

///
///\class       DWFNotImplementedException  dwfcore/Exception.h "dwfcore/Exception.h"
///\brief       Class for unimplemented method exceptions.
///\since       1.0.1
///
_DWFCORE_DECLARE_EXCEPTION_CLASS( NotImplemented, DWFException )

///
///\class       DWFIllegalStateException  dwfcore/Exception.h "dwfcore/Exception.h"
///\brief       Class for logical state exceptions.
///\since       1.0.1
///
_DWFCORE_DECLARE_EXCEPTION_CLASS( IllegalState, DWFException )

///
///\class       DWFInvalidPasswordException  dwfcore/Exception.h "dwfcore/Exception.h"
///\brief       Class for authentication exceptions.
///\since       1.0.1
///
_DWFCORE_DECLARE_EXCEPTION_CLASS( InvalidPassword, DWFException )

///
///\class       DWFNameCollisionException  dwfcore/Exception.h "dwfcore/Exception.h"
///\brief       Class for signalling that duplicate names were encountered where they are invalid
///\since       7.4.0
///
_DWFCORE_DECLARE_EXCEPTION_CLASS( NameCollision, DWFIllegalStateException )

///
///\class       DWFUserCancelledException  dwfcore/Exception.h "dwfcore/Exception.h"
///\brief       Class for signalling that a process has been terminated by the end user
///\since       7.4.1
///
_DWFCORE_DECLARE_EXCEPTION_CLASS( UserCancelled, DWFException )

///
///\class       DWFZlibDataException  dwfcore/Exception.h "dwfcore/Exception.h"
///\brief       Class for ZLIB data error exceptions 
///\since       7.5.0
///
///             Specific exception class that should be thrown if error occures reading ZLIB
///             Derived from DWFIOException for compatibility purposes  
///
_DWFCORE_DECLARE_EXCEPTION_CLASS( ZlibData, DWFIOException )

///
///\class       DWFXMLDuplicateAttributeException  dwfcore/Exception.h "dwfcore/Exception.h"
///\brief       Class for signalling duplicate attributes encountered during XML parsing 
///\since       7.6.0
///
///             Specific exception class that should be thrown if duplicate
///             attributes are encountered during XML parsing
///             Derived from DWFIOException for compatibility purposes  
///
_DWFCORE_DECLARE_EXCEPTION_CLASS( XMLDuplicateAttribute, DWFIOException )


}

///
///
///


    //
    // prefer ANSI C99 standard macro
    //
#ifndef    __func__
#define    __func__    __FUNCTION__
#endif


#define _DWFCORE_BUILD_EX( ex_class, message )                      \
    ex_class ex( message, __func__,                                 \
                          _DWFCORE_FORCE_WIDE_STRING(__FILE__),     \
                          __LINE__ );

#define _DWFCORE_THROW_EX    throw ex;

///
///\ingroup     InternalMacros
///\brief       Creates an exception object on the local stack
///             and throws it.
///
///             Use this macro to create and throw a typed exception object
///             pre-populated with the event details.
///
#define _DWFCORE_THROW( ex_class, message )                         \
    _DWFCORE_BUILD_EX( ex_class, message )                          \
    _DWFCORE_THROW_EX

#endif
