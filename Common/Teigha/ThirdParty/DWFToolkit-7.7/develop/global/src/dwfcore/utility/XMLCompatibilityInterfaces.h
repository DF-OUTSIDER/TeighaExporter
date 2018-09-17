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
#ifndef _DWFTK_XML_COMPATIBILITY_INTERFACES_H
#define _DWFTK_XML_COMPATIBILITY_INTERFACES_H

///
///\file    dwfcore/utility/XMLCompatibilityInterfaces.h
///\brief   Various interface declarations for use by the XML Compatibility PreProcessor
///

#include "dwfcore/Iterator.h"
#include "dwfcore/String.h"

namespace DWFCore
{

///
///\interface   DWFXMLCompatibilityErrorReportable dwfcore/utility/XMLCompatibilityInterfaces.h
///\since       7.4
///\brief       Interface through which notification of error conditions are made.
///
class DWFXMLCompatibilityErrorReportable
{

public:

    ///
    ///\brief   The various errors that can be reported
    ///
    ///         Each one of these errors is to be understood
    ///         by the implementor of the interface.  Since there
    ///         should be an id for each exception in 
    ///         dwfcore/utility/XMLCompatibilityExceptions.h, these
    ///         id's can be converted to exceptions.
    ///
    typedef enum
    {
        ///
        ///\brief   General error encountered while processing markup.
        ///\see     DWFXMLCompatibilityException
        ///
        eGeneralError,
        ///
        ///\brief   Generic error witht the structure of the markup document.
        ///\see     DWFXMLCompatibilityFormatException
        ///
        eFormatError,
        ///
        ///\brief   A namespace which has not been declared if referenced in the markup.
        ///\see     DWFXMLCompatibilityInvalidNamespaceReferenceException
        ///
        eInvalidNamespaceReferenceError,
        ///
        ///\brief   A \e Choice element was found after a \e Fallback element.
        ///\see     DWFXMLCompatibilityLateChoiceException
        ///
        eLateChoiceError,
        ///
        ///\brief   An attribute which should not be in an element was encountered.
        ///\see     DWFXMLCompatibilityIllegalAttributeException
        ///
        eIllegalAttributeError,
        ///
        ///\brief   An attribute which shouldn't be prefixed is.
        ///\see     DWFXMLCompatibilityPrefixedAttributeException
        ///
        ePrefixedAttributeError,
        ///
        ///\brief   An attribute which should be prefixed isn't.
        ///\see     DWFXMLCompatibilityUnprefixedAttributeException
        ///
        eUnprefixedAttributeError,
        ///
        ///\brief   An attribute which should be present in an element isn't.
        ///\see     DWFXMLCompatibilityMissingAttributeException
        ///
        eMissingAttributeError,
        ///
        ///\brief   An \e AlternateContent element has more than one \e Fallback child element.
        ///\see     DWFXMLCompatibilityMultipleFallbacksException
        ///
        eMultipleFallbacksError,
        ///
        ///\brief   An \e AlternateContent element was discovered as a child of another 
        ///         \e AlternateContent element.
        ///\see     DWFXMLCompatibilityNestedAlternateContentException
        ///
        eNestedAlternateContentError,
        ///
        ///\brief   A required namespace is not recognized by the client.
        ///\see     DWFXMLCompatibilityInsufficientSupportException
        ///
        eInsufficientSupportError,
        ///
        ///\brief   An absolutely required namespace is not recognized by the client>
        ///\see     DWFXMLCompatibilityMustUnderstandException
        ///
        eMustUnderstandError,
        ///
        ///\brief   An unknown, non-ignorable markup element was encountered.
        ///\see     DWFXMLCompatibilityUnknownItemException
        ///
        eUnknownItemError,
        ///
        ///\brief   An unknown, non-ignorable attribute was encountered
        ///\see     DWFXMLCompatibilityUnknownAttributeException
        ///
        eUnknownAttributeError,
        ///
        ///\brief   An unknown, non-ignorable element was encountered
        ///\see     DWFXMLCompatibilityUnknownElementException
        ///
        eUnknownElementError
    } teErrorId;

    virtual ~DWFXMLCompatibilityErrorReportable() {}

    ///
    ///\brief   Notify this object that an error occurred
    ///\param   eId Identify the error
    ///\param   zDescription    A human-readable description of the error
    ///\throw   none
    ///
    virtual void reportError( teErrorId eId, const DWFCore::DWFString &zDescription )
        throw() = 0;

    ///
    ///\brief   Check to see if this object is aware of any errors
    ///\return  \e True if an error has been reported to this object.  \e False otherwise.
    ///\throw   none
    ///
    virtual bool errorReported()
        throw() = 0;
};

///
///\interface   DWFXMLCompatibilityProcessTransferable dwfcore/utility/XMLCompatibilityInterfaces.h
///\since       7.4
///\brief       Interfaces for handling certain portions of XML compatibility processing logic.
///
///             An interface is used so that different classes may implement markup processing
///             depending on the context of the current markup elements being examind.  This
///             way processing logic can be pushed on to and popped off of the stack.
///
class DWFXMLCompatibilityProcessTransferable
{

public:

    ///
    ///\brief   A list of strings 
    ///
    ///         Used to report the attributes that belong to the current element
    ///
    typedef DWFCore::DWFIterator<DWFCore::DWFString> tAttributesList;

    virtual ~DWFXMLCompatibilityProcessTransferable() {}

    ///
    ///\brief   Notify the process of the presence element open tag.
    ///\param   zName   the element name
    ///\param   rAttributes the attributes declared on this element
    ///\throw   none
    ///
    virtual void elementBegin( const DWFCore::DWFString& zName, 
                               tAttributesList& rAttributes )
        throw() = 0;

    ///
    ///\brief   Notify the process that the specified element is being closed.
    ///\param   zName   the element name
    ///\throw   none
    ///
    virtual void elementEnd( const DWFCore::DWFString& zName )
        throw() = 0;

};

}

#endif
