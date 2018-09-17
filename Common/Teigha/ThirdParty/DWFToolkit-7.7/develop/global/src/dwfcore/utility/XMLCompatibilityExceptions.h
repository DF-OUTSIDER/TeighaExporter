//
//  Copyright (c) 2004-2006 by Autodesk, Inc. 
// 
//  By using this code, you are agreeing to the terms and conditions of 
//  the License Agreement included in the documentation for this code. 
// 
//  AUTODESK MAKES NO WARRANTIES, EXPRESSED OR IMPLIED, 
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
#ifndef _DWFTK_XML_COMPATIBILITY_EXCEPTIONS_H
#define _DWFTK_XML_COMPATIBILITY_EXCEPTIONS_H

///
///\file    dwfcore/utility/XMLCompatibilityExceptions.h
///\brief   Exceptions for reporting XML Compatibility preprocessor error 
///         conditions
///
///         All of the exceptions defined in this file signal some violation of 
///         the Open XML Markup Compatibility specification (otherwise known as 
///         Part 5 of Open Office XML: "Markup Compatibility").  Therefore,
///         exceptions may reference at least one instance in the Markup 
///         Compatibility specification that declares the need for the error.
///         The document used for these reference will be Ecma TC45 Working 
///         Draft 1.4 which can be accessed at
///         http://www.ecma-international.org/news/TC45_current_work/tc45-2006-332.pdf
///         at the time of this writing.
///

//////////////////////////////////////////////////////////////////////
#include "dwfcore/Exception.h"

namespace DWFCore
{

///
///\class   DWFXMLCompatibilityException dwfcore/utility/XMLCompatibilityExceptions.h
///\brief   General error while processing a "Markup Compatibility" document.
///\since   7.4
///
///         This exception is the base to all "Markup Compatibility" exceptions
///         and can be used as a catch-all exception (more specfic than
///         DWFException) when a client does not care about specifics.
///
_DWFCORE_DECLARE_EXCEPTION_CLASS( XMLCompatibility, DWFCore::DWFException )

///
///\class   DWFXMLCompatibilityFormatException dwfcore/utility/XMLCompatibilityExceptions.h
///\brief   Error indicating problems with format of the processed markup
///\see     DWFXMLCompatibilityInsufficientSupportException
///\since   7.4
///
///         This exception classifies the various errors caused by invalid
///         conditions in the processed "Markup Compatibility" document.
///         It is a companion and balance to 
///         DWFXMLCompatibilityInsufficientSupportException.
///
_DWFCORE_DECLARE_EXCEPTION_CLASS( XMLCompatibilityFormat,
                                  DWFXMLCompatibilityException )

///
///\class   DWFXMLCompatibilityInvalidNamespaceReferenceException dwfcore/utility/XMLCompatibilityExceptions.h
///\brief   Error indicating a non-ignored namespace in a "ProcessContent" attribute
///\since   7.4
///
///         Page 17, line 23 states that all elements referenced in a ProcessContent
///         attribute must belong to a namespace that has been identified as ignorable.
///
_DWFCORE_DECLARE_EXCEPTION_CLASS( XMLCompatibilityInvalidNamespaceReference,
                                  DWFXMLCompatibilityFormatException )

///
///\class   DWFXMLCompatibilityLateChoiceException dwfcore/utility/XMLCompatibilityExceptions.h
///\brief   Error indicating a "Choice" element was found after a "Fallback" element.
///\since   7.4
///
///         Page 21, line 16 of the "Markup Compatibility" specification does not allow
///         "Choice" elements to proceed a "Fallback" element within a parent 
///         "AlternateContent" element.
///
_DWFCORE_DECLARE_EXCEPTION_CLASS( XMLCompatibilityLateChoice,
                                  DWFXMLCompatibilityFormatException )

///
///\class   DWFXMLCompatibilityIllegalAttributeException dwfcore/utility/XMLCompatibilityExceptions.h
///\brief   Indicates an error with an attribute applied to an element.
///\see	    DWFXMLCompatibilityPrefixedAttributeException
///\see	    DWFXMLCompatibilityUnprefixedAttributeException
///\since	7.4
///
///         The "Markup Compatibility" specification places certain restrictions
///         on the attributes allowed on certain elements.  The most common restriction
///         is on the use of the "xml:lang" and "xml:space" attributes.  Page 17, Line 27
///         of the specification mandates this requirement, but this exception is
///         also a base class of other, more specific exceptions.
///
_DWFCORE_DECLARE_EXCEPTION_CLASS( XMLCompatibilityIllegalAttribute,
                                  DWFXMLCompatibilityFormatException )

///
///\class   DWFXMLCompatibilityPrefixedAttributeException dwfcore/utility/XMLCompatibilityExceptions.h
///\brief   An illegal attribute prefix has been specified.
///\since   7.4
///
///         Some attributes are not allowed to specify a prefix. Page 22, line 26 
///         of the "Markup Compatibility" specification indicates that the
///         "Requires" attribute of the "Choice" element is one such attribute.
///
_DWFCORE_DECLARE_EXCEPTION_CLASS( XMLCompatibilityPrefixedAttribute,
                                  DWFXMLCompatibilityIllegalAttributeException )

///
///\class   DWFXMLCompatibilityUnprefixedAttributeException dwfcore/utility/XMLCompatibilityExceptions.h
///\brief   A required attribute prefix has not been specified.
///\since   7.4
///
///         Several portions of the "Markup Compatibility" specification indicate
///         one or all of an element's attributes must have a namespace prefix.
///         - Page 21, line 24
///         - Page 22, line 24
///         - Page 23, line 1 
///
_DWFCORE_DECLARE_EXCEPTION_CLASS( XMLCompatibilityUnprefixedAttribute,
                                  DWFXMLCompatibilityIllegalAttributeException )

///
///\class   DWFXMLCompatibilityMissingAttributeException dwfcore/utility/XMLCompatibilityExceptions.h
///\brief   A required attribute is missing from the element.
///\since   7.4
///
_DWFCORE_DECLARE_EXCEPTION_CLASS( XMLCompatibilityMissingAttribute,
                                  DWFXMLCompatibilityFormatException )

///
///\class   DWFXMLCompatibilityMultipleFallbacksException dwfcore/utility/XMLCompatibilityExceptions.h
///\brief   Error indicating that an "AlternateContent" element has more than one 
///         "Fallback" element.
///\since   7.4
///
///         Page 21, line 16 of the "Markup Compatibility" specification does not allow
///         "AlternateContent" elements to contain more than one "Fallback"  child
///         element.
///			
_DWFCORE_DECLARE_EXCEPTION_CLASS( XMLCompatibilityMultipleFallbacks,
                                  DWFXMLCompatibilityFormatException )

///
///\class   DWFXMLCompatibilityNestedAlternateContentException dwfcore/utility/XMLCompatibilityExceptions.h
///\brief
///\since   7.4
///
_DWFCORE_DECLARE_EXCEPTION_CLASS( XMLNestedAlternateContent, 
                                  DWFXMLCompatibilityFormatException )

///
///\class   DWFXMLCompatibilityInsufficientSupportException dwfcore/utility/XMLCompatibilityExceptions.h
///\brief   Error indicating that the client is not feature-rich enough to
///         support the processed document.
///\see     DWFXMLCompatibilityFormatException
///\since   7.4
///
///         This exception classifies errors in which there is not a problem
///         with the format of the processed markup, but the markup indicates
///         features that are not supported by the client.  It is a companion
///         and a balance to DWFXMLCompatibilityFormatException.
///
_DWFCORE_DECLARE_EXCEPTION_CLASS( XMLCompatibilityInsufficientSupport,
                                  DWFXMLCompatibilityException )

///
///\class   DWFXMLCompatibilityMustUnderstandException dwfcore/utility/XMLCompatibilityExceptions.h
///\brief   Error indicating that the markup requires a namespace to be
///         supported which is not supported by the client.
///\since   7.4
///
///         Page 20, line 2 of the "Markup Compatibility" specification states
///         that all namespaces mentioned in a "MustUnderstand" attribute
///         must be supported by the client or an error will be generated.
///
_DWFCORE_DECLARE_EXCEPTION_CLASS( XMLCompatibilityMustUnderstand,
                                  DWFXMLCompatibilityInsufficientSupportException )

///
///\class   DWFXMLCompatibilityUnknownItemException dwfcore/utility/XMLCompatibilityExceptions.h
///\brief   An unknown, non-ignorable markup element was encountered.
///\since   7.4
///
///         Page 14, line 9 of the "Markup Compatibility" specification states
///         that if an unknown element or attribute is encountered in the
///         marku, an error is generated unless somthing in the document 
///         specifies otherwise.
///
_DWFCORE_DECLARE_EXCEPTION_CLASS( XMLCompatibilityUnknownItem,
                                  DWFXMLCompatibilityInsufficientSupportException )

///
///\class   DWFXMLCompatibilityUnknownAttributeException dwfcore/utility/XMLCompatibilityExceptions.h
///\brief   An unknown, non-ignorable attribute was encountered.
///\see	    DWFXMLCompatibilityUnknownItemException
///\since   7.4
///
_DWFCORE_DECLARE_EXCEPTION_CLASS( XMLCompatibilityUnknownAttribute,
                                  DWFXMLCompatibilityUnknownItemException )

///
///\class   DWFXMLCompatibilityUnknownElementException dwfcore/utility/XMLCompatibilityExceptions.h
///\brief   An unknown, non-ignorable element was encountered.
///\see	    DWFXMLComptibilityUnknownItemException
///\since   7.4
///
_DWFCORE_DECLARE_EXCEPTION_CLASS( XMLCompatibilityUnknownElement,
                                  DWFXMLCompatibilityUnknownItemException )
}

#endif
