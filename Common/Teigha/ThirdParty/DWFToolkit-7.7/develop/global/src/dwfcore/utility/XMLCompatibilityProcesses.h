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
#ifndef _DWFTK_XML_COMPATIBILITY_PROCESSES_H
#define _DWFTK_XML_COMPATIBILITY_PROCESSES_H

#include "dwfcore/utility/XMLCompatibilityInterfaces.h"

namespace DWFCore
{

///
///\class   DWFXMLCompatibilityProcessIgnore dwfcore/utility/XMLCompatibilityProcesses.h
///\since   7.4
///\brief   Process that completely ignores all incoming elements
///
class DWFXMLCompatibilityProcessIgnore : public DWFXMLCompatibilityProcessTransferable
{

public:

    ///
    ///\copydoc DWFXMLCompatibilityProcessTransferable::elementBegin()
    ///
    virtual void elementBegin( const DWFCore::DWFString& zName, 
                               tAttributesList& rAttributes) 
        throw();

    ///
    ///\copydoc DWFXMLCompatibilityProcessTransferable::elementEnd()
    ///
    virtual void elementEnd( const DWFCore::DWFString& zName )
        throw();

protected:

private:

};

///
///\class   DWFXMLCompatibilityProcessConsider dwfcore/utility/XMLCompatibilityProcesses.h
///\since   7.4
///\brief   Common functionality for processes that inspect incoming element
///
class DWFXMLCompatibilityProcessConsider : public DWFXMLCompatibilityProcessTransferable
{

public:

    ///
    ///\copydoc DWFXMLCompatibilityProcessTransferable::elementBegin()
    ///
    virtual void elementBegin( const DWFCore::DWFString& zName, 
                               tAttributesList& rAttributes) 
        throw();

    ///
    ///\copydoc DWFXMLCompatibilityProcessTransferable::elementEnd()
    ///
    virtual void elementEnd( const DWFCore::DWFString& zName )
        throw();

protected:

private:

};

///
///\class   DWFXMLCompatibilityProcessStandard dwfcore/utility/XMLCompatibilityProcesses.h
///\since   7.4
///\brief   Standard processing rules assuming, that context hasn't changed them.
///
class DWFXMLCompatibilityProcessStandard : public DWFXMLCompatibilityProcessConsider
{

public:

    ///
    ///\copydoc DWFXMLCompatibilityProcessTransferable::elementBegin()
    ///
    virtual void elementBegin( const DWFCore::DWFString& zName, 
                               tAttributesList& rAttributes) 
        throw();

    ///
    ///\copydoc DWFXMLCompatibilityProcessTransferable::elementEnd()
    ///
    virtual void elementEnd( const DWFCore::DWFString& zName )
        throw();

protected:

private:

};

///
///\class   DWFXMLCompatibilityProcessChoices dwfcore/utility/XMLCompatibilityProcesses.h
///\since   7.4
///\brief   Processing rules in effect while processing an \e AlternateContent element.
///
class DWFXMLCompatibilityProcessChoices : public DWFXMLCompatibilityProcessConsider
{

public:

    ///
    ///\copydoc DWFXMLCompatibilityProcessTransferable::elementBegin()
    ///
    virtual void elementBegin( const DWFCore::DWFString& zName, 
                               tAttributesList& rAttributes) 
        throw();

    ///
    ///\copydoc DWFXMLCompatibilityProcessTransferable::elementEnd()
    ///
    virtual void elementEnd( const DWFCore::DWFString& zName )
        throw();

protected:

private:

};

}

#endif
