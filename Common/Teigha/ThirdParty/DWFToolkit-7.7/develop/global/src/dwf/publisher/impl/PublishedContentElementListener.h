//
//  Copyright (c) 1996-2006 by Autodesk, Inc.
//
//  By using this code, you are agreeing to the terms and conditions of
//  the License Agreement included in the documentation for this code.
//
//  AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE CORRECTNESS
//  OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE IT. AUTODESK
//  PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY DISCLAIMS ANY
//  LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL DAMAGES FOR ERRORS,
//  OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
//
//  Use, duplication, or disclosure by the U.S. Government is subject to
//  restrictions set forth in FAR 52.227-19 (Commercial Computer Software
//  Restricted Rights) and DFAR 252.227-7013(c)(1)(ii) (Rights in Technical
//  Data and Computer Software), as applicable.
//
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/publisher/impl/PublishedContentElementListener.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//

#ifndef _DWFTK_PUBLISHED_CONTENT_ELEMENT_LISTENER_H
#define _DWFTK_PUBLISHED_CONTENT_ELEMENT_LISTENER_H

///
///\file        dwf/publisher/impl/PublishedContentElementListener.h
///\brief       This file contains the DWFPublishedContentElementListener class declaration.
///

#ifndef DWFTK_READ_ONLY

#include "dwf/package/Instance.h"
#include "dwf/publisher/impl/PublishedContentElement.h"

namespace DWFToolkit
{

///
///\ingroup     dwfpublish
///
///\class       DWFPublishedContentElementListener   dwf/publisher/impl/PublishedContentElementListener.h    "dwf/publisher/impl/PublishedContentElementListener.h"
///\brief       The listener used by the DWFPackagePublisher to track, in order, the creation of the instances to 
///             generate the default model view navigation tree.
///\since       7.2.0
///

class DWFPublishedContentElementListener : public DWFPublishedContentElement::NotificationSink
                                           _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    DWFPublishedContentElementListener()
        throw()
        : _oInstances()
    {;}

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFPublishedContentElementListener()
        throw()
    {;}

    ///
    ///         Clear all stored instance pointers
    ///
    ///\throw   None
    ///
    _DWFTK_API
    void clear()
        throw()
    {
        _oInstances.clear();
    }

    ///
    ///         Access the instance pointers collected by the notifications
    ///
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///\throw   None
    _DWFTK_API
    DWFInstance::tIterator* getInstances()
        throw()
    {
        return _oInstances.iterator();
    }

public:

    ///
    ///         Use this to notify the listener about instance creation events.
    ///
    ///\throw   None
    ///
    _DWFTK_API
    void notifyInstanceCreated( DWFPublishedContentElement::tKey /*nKey*/, DWFInstance* pInstance )
        throw()
    {
        _oInstances.push_back( pInstance );
    }

private:

    DWFInstance::tList      _oInstances;
};

}


#endif

#endif


