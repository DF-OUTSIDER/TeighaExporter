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


#ifndef _DWFCORE_COMPARATOR_H
#define _DWFCORE_COMPARATOR_H


///
///\file        dwfcore/Comparator.h
///\brief       This header contains the declarations for the parameterized equals, 
///             less than and empty comparator functions.
///



#include "dwfcore/Core.h"
#include "dwfcore/String.h"



namespace DWFCore
{

///
///\struct      tDWFCompareEqual    dwfcore/Comparator.h    "dwfcore/Comparator.h"
///\brief       Equals comparator template.
///\since       1.0.1
///
///             This template defines the equality of two objects
///             of the same type using \e operator==() method.  
///
template<class T>
struct tDWFCompareEqual
{
    ///
    ///         Equality comparator using \e operator==() for type \e T.
    ///
    ///\param   rLHS    The first object to compare.
    ///\param   rRHS    The second object to compare.
    ///\return  \e true if \a rLHS is equal to \a rRHS,
    ///         \e false if \a rLHS is not equal to \a rLHS.
    ///
    virtual bool operator()( const T& rLHS, const T& rRHS ) const
    {
        return static_cast<bool>(rLHS == rRHS);
    }

    virtual ~tDWFCompareEqual()
    {}
};

///
///\struct      tDWFCharCompareEqual    dwfcore/Comparator.h    "dwfcore/Comparator.h"
///\brief       Equals comparator for ASCII character strings.
///\since       1.0.1
///
///             This defines the equality of two ASCII strings. 
///
struct tDWFCharCompareEqual : tDWFCompareEqual<const char*>
{
    ///
    ///         Equality comparator using the \b DWFCORE_COMPARE_ASCII_STRINGS macro.
    ///
    ///\param   rLHS    The first string to compare.
    ///\param   rRHS    The second string to compare.
    ///\return  \e true if \a rLHS is equal to \a rRHS,
    ///         \e false if \a rLHS is not equal to \a rLHS.
    ///
    bool operator()( const char* rLHS, const char* rRHS ) const
    {
        return (DWFCORE_COMPARE_ASCII_STRINGS(rLHS, rRHS) == 0);
    }
};

///
///\struct      tDWFWCharCompareEqual    dwfcore/Comparator.h    "dwfcore/Comparator.h"
///\brief       Equals comparator for wide character strings.
///\since       1.0.1
///
///             This defines the equality of two wide strings. 
///
struct tDWFWCharCompareEqual : tDWFCompareEqual<const wchar_t*>
{
    ///
    ///         Equality comparator using the \b DWFCORE_COMPARE_WIDE_STRINGS macro.
    ///
    ///\param   rLHS    The first string to compare.
    ///\param   rRHS    The second string to compare.
    ///\return  \e true if \a rLHS is equal to \a rRHS,
    ///         \e false if \a rLHS is not equal to \a rLHS.
    ///
    bool operator()( const wchar_t* rLHS, const wchar_t* rRHS ) const
    {
        return (DWFCORE_COMPARE_WIDE_STRINGS(rLHS, rRHS) == 0);
    }
};

///
///\struct      tDWFCompareLess    dwfcore/Comparator.h    "dwfcore/Comparator.h"
///\brief       Less Than comparator template.
///\since       1.0.1
///
///             This template defines the ordering of two objects
///             of the same type using \e operator<() method.  
///
template<class T>
struct tDWFCompareLess
{
    ///
    ///         Less Than comparator using \e operator<() for type \e T.
    ///
    ///\param   rLHS    The first object to compare.
    ///\param   rRHS    The second object to compare.
    ///\return  \e true if \a rLHS is less than to \a rRHS,
    ///         \e false if \a rLHS is not less than to \a rLHS.
    ///
    virtual bool operator()( const T& rLHS, const T& rRHS ) const
    {
        return (rLHS < rRHS);
    }

    virtual ~tDWFCompareLess()
    {}
};

///
///\struct      tDWFCharCompareLess    dwfcore/Comparator.h    "dwfcore/Comparator.h"
///\brief       Less Than comparator for ASCII character strings.
///\since       1.0.1
///
///             This defines the ordering by less than of two ASCII strings.
///
struct tDWFCharCompareLess : tDWFCompareLess<const char*>
{
    ///
    ///         Less Than comparator using the \b DWFCORE_COMPARE_ASCII_STRINGS macro.
    ///
    ///\param   rLHS    The first string to compare.
    ///\param   rRHS    The second string to compare.
    ///\return  \e true if \a rLHS is less than to \a rRHS,
    ///         \e false if \a rLHS is not less than to \a rLHS.
    ///
    bool operator()( const char* rLHS, const char* rRHS ) const
    {
        return (DWFCORE_COMPARE_ASCII_STRINGS(rLHS, rRHS) < 0);
    }
};

///
///\struct      tDWFWCharCompareLess    dwfcore/Comparator.h    "dwfcore/Comparator.h"
///\brief       Less Than comparator for wide character strings.
///\since       1.0.1
///
///             This defines the ordering by less than of two wide strings.
///
struct tDWFWCharCompareLess : tDWFCompareLess<const wchar_t*>
{
    ///
    ///         Less Than comparator using the \b DWFCORE_COMPARE_WIDE_STRINGS macro.
    ///
    ///\param   rLHS    The first string to compare.
    ///\param   rRHS    The second string to compare.
    ///\return  \e true if \a rLHS is less than to \a rRHS,
    ///         \e false if \a rLHS is not less than to \a rLHS.
    ///
    bool operator()( const wchar_t* rLHS, const wchar_t* rRHS ) const
    {
        return (DWFCORE_COMPARE_WIDE_STRINGS(rLHS, rRHS) < 0);
    }
};

///
///\struct      tDWFDefinedEmpty    dwfcore/Comparator.h    "dwfcore/Comparator.h"
///\brief       Empty value definition template.
///\since       1.0.1
///
///             This template defines the empty value of an object of type \e T.
///
template<class T>
struct tDWFDefinedEmpty
{
    ///
    ///         Defines \b 0 as the empty value for type \e T.
    ///
    ///\return  \b 0.
    ///
    T operator()() const
    {
        return 0;
    }
};

///
///\struct      tDWFCharDefinedEmpty    dwfcore/Comparator.h    "dwfcore/Comparator.h"
///\brief       Empty value definition for ASCII character strings.
///\since       1.0.1
///
///             This template defines the empty value of an ASCII string.
///
struct tDWFCharDefinedEmpty : tDWFDefinedEmpty<const char*>
{
    ///
    ///         Defines \b "" as the empty ASCII character string.
    ///
    ///\return  \b "".
    ///
    const char* operator()() const
    {
        return "";
    }
};

///
///\struct      tDWFWCharDefinedEmpty    dwfcore/Comparator.h    "dwfcore/Comparator.h"
///\brief       Empty value definition for wide character strings.
///\since       1.0.1
///
///             This template defines the empty value of a wide string.
///
struct tDWFWCharDefinedEmpty : tDWFDefinedEmpty<const wchar_t*>
{
    ///
    ///         Defines \b "" as the empty wide character string.
    ///
    ///\return  \b L"".
    ///
    const wchar_t* operator()() const
    {
        return L"";
    }
};

///
///\struct      tDWFStringDefinedEmpty    dwfcore/Comparator.h    "dwfcore/Comparator.h"
///\brief       Empty value definition for DWFString objects.
///\since       1.0.1
///
///             This template defines the empty value of DWFString.
///
struct tDWFStringDefinedEmpty : tDWFDefinedEmpty<DWFString>
{
    DWFString operator()() const
    {
        return L"";
    }
};

}


#endif
