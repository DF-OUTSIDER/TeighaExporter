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

#ifndef _DWFTK_PUBLISHER_H
#define _DWFTK_PUBLISHER_H


///
///\file        dwf/publisher/Publisher.h
///\brief       This file contains the DWFPublishedObject class declaration.
///

#ifndef DWFTK_READ_ONLY



#include "dwfcore/STL.h"
#include "dwfcore/Iterator.h"
#include "dwfcore/Exception.h"
#include "dwfcore/InputStream.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"
#include "dwf/publisher/PublishableResource.h"
#include "dwf/publisher/PublishableSection.h"
#include "dwf/publisher/PropertyVisitor.h"
#include "dwf/publisher/EmbeddedFontVisitor.h"
#include "dwf/publisher/PublishedObject.h"



namespace DWFToolkit
{



///
///\ingroup         dwfpublish
///
///\class       DWFPublisher   dwf/publisher/Publisher.h    "dwf/publisher/Publisher.h"
///\brief       Base implementation and interface defintion for objects (publishers) that
///             can process and publish known content.
///\since       7.0.1
///
///             Publisher objects are used to extract data from publishable structural constructs (Model, etc.)
///             A publisher can be configured with strong type visitors that will be called upon
///             as the structure is traversed.  Complex and compound visitors can be created
///             for additional functionality.  For example, a compound DWFPublishedObject visitor could be used
///             to insert a record in a database for every "object" in the structure. 
///             Another is as a base class for a aggregate object that additionally
///             implements whatever visitor interfaces it needs.  This solution is preferrable if common data
///             must be shared across visitors.  
///
///             The basic publishing workflow is as follows:
///             -# Create an instance of a [derived type] DWFPublisher (DWFPackagePublisher is an example implementation.)
///             -# Create an instance of a [derived type] DWFPublishableSection (DWFModel is an example implementation.)
///             -# Add content (DWFPublishable types) to the section.
///             -# Pass the publisher to the \a publish() method of the section.  The publisher will process the section
///                content according to its implementation details.
///             -# Invoke the \a publish() method on the publisher.  This will complete the process.
///
///             For a more advanced workflow, repeat steps 2, 3 and 4 above to add multiple sections to the final publication.
///
///\note        It should be obvious but there is nothing requiring a DWFPublisher to actually produce a DWF package file.
///             A reference implementation (DWFPackagePublisher) is provided with the toolkit.
///
class DWFPublisher _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE
{

public:


    ///
    ///\brief   The metadata version for publishing. Use this to choose between 
    ///         content definition and object definition
    ///
    typedef enum teMetadataVersion
    {
        ePublishContentDefinition,
        ePublishObjectDefinition

    } teMetadataVersion;

public:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    DWFPublisher()
        throw()
        : _pPublishedObjectVisitor( NULL )
        , _pPropertyVisitor( NULL )
        , _pEmbeddedFontVisitor( NULL )
    {;}

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    virtual ~DWFPublisher()
        throw()
    {;}

    ///
    ///         Invokes the final publication process.
    ///
    ///\throw   None
    ///
    virtual void publish()
        throw( DWFException )
    {;}

    ///
    ///         Returns a visitor that can process published objects
    ///         in order to retrieve or characterize their structure.
    ///
    ///\return  A published object visitor.  This pointer must not be deleted by the caller.
    ///\throw   DWFException
    ///
    virtual DWFPublishedObject::Visitor* getPublishedObjectVisitor()
        throw( DWFException )
    {
        return _pPublishedObjectVisitor;
    }

    ///
    ///         Sets the visitor that can process published objects
    ///         in order to retrieve or characterize their structure.
    ///
    ///\param   pVisitor    The published object visitor. This pointer will not
    ///                     be deleted by the publisher.
    ///\throw   None
    ///
    void setPublishedObjectVisitor( DWFPublishedObject::Visitor* pVisitor )
        throw()
    {
        _pPublishedObjectVisitor = pVisitor;
    }

    ///
    ///         Returns a visitor that can process a property and/or property container.
    ///
    ///\return  A property visitor.  This pointer must not be deleted by the caller.
    ///\throw   DWFException
    ///
    virtual DWFPropertyVisitor* getPropertyVisitor()
        throw( DWFException )
    {
        return _pPropertyVisitor;
    }

    ///
    ///         Sets the visitor that can process a property and/or property container.
    ///
    ///\param   pVisitor    The property visitor. This pointer will not
    ///                     be deleted by the publisher.
    ///\throw   None
    ///
    void setPropertyVisitor( DWFPropertyVisitor* pVisitor )
        throw()
    {
        _pPropertyVisitor = pVisitor;
    }

    ///
    ///         Returns a visitor that can process a font.
    ///
    ///\return  A font visitor.  This pointer must not be deleted by the caller.
    ///\throw   DWFException
    ///
    virtual DWFEmbeddedFontVisitor* getEmbeddedFontVisitor()
        throw( DWFException )
    {
        return _pEmbeddedFontVisitor;
    }

    ///
    ///         Sets the visitor that can process a font.
    ///
    ///\param   pVisitor    The font visitor. This pointer will not
    ///                     be deleted by the publisher.
    ///\throw   None
    ///
    void setEmbeddedFontVisitor( DWFEmbeddedFontVisitor* pVisitor )
        throw()
    {
        _pEmbeddedFontVisitor = pVisitor;
    }

    ///
    ///         This callback should be invoked by \a pPublishable
    ///         as soon as it starts its \a publish() processing
    ///         passing itself back as the parameter.
    ///
    ///\param   pPublishable    The object being published.
    ///\throw   DWFException
    ///
    virtual void preprocess( DWFPublishable* )
        throw( DWFException )
    {;}

    ///
    ///         This callback should be invoked by \a pPublishable
    ///         when it has completed its \a publish() processing
    ///         passing itself back as the parameter.
    ///
    ///\param   pPublishable    The object being published.
    ///\throw   DWFException
    ///
    virtual void postprocess( DWFPublishable* )
        throw( DWFException )
    {;}

private:

    DWFPublishedObject::Visitor*    _pPublishedObjectVisitor;
    DWFPropertyVisitor*             _pPropertyVisitor;
    DWFEmbeddedFontVisitor*         _pEmbeddedFontVisitor;

private:

    DWFPublisher( const DWFPublisher& );
    DWFPublisher& operator=( const DWFPublisher& );
};


}





#endif  
#endif

