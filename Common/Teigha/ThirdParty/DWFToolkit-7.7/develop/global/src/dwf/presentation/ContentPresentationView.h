//
//  Copyright (c) 2006 by Autodesk, Inc.
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

//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/presentation/ContentPresentationView.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $

#ifndef _DWFTK_CONTENT_PRESENTATION_VIEW_H
#define _DWFTK_CONTENT_PRESENTATION_VIEW_H


///
///\file        dwf/presentation/ContentPresentationView.h
///\brief       This file contains the DWFPresentationView class declaration.
///

#include "dwfcore/STL.h"
#include "dwfcore/DWFXMLSerializer.h"
#include "dwfcore/Vector.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"
#include "dwf/presentation/ContentPresentationNode.h"
#include "dwf/presentation/utility/PropertyReferenceContainer.h"
#include "dwf/presentation/utility/ContentPresentationNodeContainer.h"

namespace DWFToolkit
{

//
// fwd declarations
//
class DWFContentPresentationViewContainer;

///
///\ingroup     dwfpresentation
///
///\class       DWFContentPresentationView     dwf/package/ContentPresentationView.h      "dwf/package/ContentPresentationView.h"
///\brief       The DWFContentPresentationView class supports alternate views of a presentation.
///\since       7.2
///
///             The DWFContentPresentationView class supports alternate views of a presentation.
///

class DWFContentPresentationView : public DWFPropertyReferenceContainer
                                 , public DWFContentPresentationNodeContainer
                                 , public DWFXMLBuildable
#ifndef DWFTK_READ_ONLY
                                 , public DWFXMLSerializable
#endif
                                   _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{
    friend class DWFContentPresentationViewContainer;

public:
    ///
    ///\brief   This type defines a list of DWFContentPresentationView pointers.
    ///
    typedef DWFOrderedVector<DWFContentPresentationView*>            tList;
    ///
    ///\brief   This type defines a mapped collection of DWFContentPresentationView pointers.
    ///
    typedef DWFStringKeySkipList<DWFContentPresentationView*>         tMap;

public:

    ///
    ///         Constructor
    ///
    ///\param   zID     An optional identifier used when referencing this view.
    ///\param   zLabel  An optional user-friendly label for this view.
    ///\throw   None
    ///
    _DWFTK_API
    DWFContentPresentationView( const DWFString& zLabel = /*NOXLATE*/L"",
                                const DWFString& zID = /*NOXLATE*/L"" )
        throw();
    ///
    ///         Destructor
    ///
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFContentPresentationView()
        throw();

    ///
    ///         Sets an id for this view.
    ///
    ///\param   zID             A string that specifies the id.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void setID( const DWFString& zID )
        throw( DWFException );

    ///
    ///         Returns the ID for this view.
    ///         If no label has been set, returns the null string.
    ///
    ///\return  The label for this view
    ///\throw   None
    ///
    _DWFTK_API
    virtual const DWFString& id() const
        throw();

    ///
    ///         Sets a user-friendly label for this view. 
    ///
    ///\param   zLabel       A string that specifies the label.
    ///\throw   None
    ///
    _DWFTK_API
    virtual void setLabel( const DWFString& zLabel )
        throw();

    ///
    ///         Returns the label for this view
    ///         If no label has been set, returns the null string
    ///
    ///\return  The label for this view
    ///\throw   None
    ///
    _DWFTK_API
    virtual const DWFString& label() const
        throw();

    ///
    ///         Sets a property set id to be used to scope property references in this view.
    ///
    ///\param   zPropertySetID          The property set id.
    ///\param   bUseDefaultIfNotFound   If \e true and the property referred to in a property reference
    ///                                 is missing, the content element will be queried for the property
    ///                                 directly.
    ///\throw   None
    ///
    _DWFTK_API
    virtual void setPropertySetID( const DWFString& zPropertySetID,
                                   bool             bUseDefaultIfNotFound = true )
        throw( DWFException );

    ///
    ///         Returns the property set id to be used to scope property references in this view.
    ///
    ///\return  The property set id.
    ///\throw   None
    ///
    _DWFTK_API
    virtual const DWFString& propertySetID() const
        throw();

    ///
    ///         Sets a property schema id to be used to scope property references in this view 
    ///
    ///\param   zPropertySchemaID       The property schema id.
    ///\param   bUseDefaultIfNotFound   If \e true and the property referred to in a property reference
    ///                                 is missing, the content element will be queried for the property.
    ///\throw   None
    ///
    _DWFTK_API
    virtual void setPropertySchemaID( const DWFString& zPropertySchemaID,
                                      bool             bUseDefaultIfNotFound = false )
        throw( DWFException );

    ///
    ///         Returns the schema id of the property set to be used to scope property references in this view.
    ///
    ///\return  The schema id of the property set.
    ///\throw   None
    ///
    _DWFTK_API
    virtual const DWFString& propertySchemaID() const
        throw();

    ///
    ///         Returns whether the default property must be used if it does 
    ///         not exist in the specified set.
    ///
    ///\return  If \e true and the property referred to in a property reference
    ///         is missing, the content element will be queried for the property.
    ///\throw   None
    ///
    virtual bool useDefaultPropertyIfNotFoundInSet() const
        throw()
    {
        return _bUseDefaultIfNotFound;
    }

    ///
    ///         Specifies whether this view is hidden or not.
    ///
    ///\param   bHidden       If \e true, this view will be marked as hidden.
    ///\throw   None
    ///
    virtual void setHidden( bool bHidden )
        throw()
    {
        _bHidden = bHidden;
    }

    ///
    ///         Returns whether this view is hidden or not.
    ///
    ///\return  Returns whether this view is hidden or not.
    ///\throw   None
    ///
    virtual bool hidden() const
        throw()
    {
        return _bHidden;
    }

    ///
    ///         Returns the parent of this view - the container which holds it.
    ///
    ///\return  The parent, or NULL if one has not yet been set.
    ///\throw   None
    ///
    virtual DWFContentPresentationViewContainer* parent() const
        throw()
    {
        return _pParent;
    }

    ///
    ///\copydoc DWFCore::DWFXMLBuildable::parseAttributeList()
    ///
    _DWFTK_API
    virtual void parseAttributeList( const char** ppAttributeList )
        throw( DWFException );

#ifndef DWFTK_READ_ONLY

    ///
    ///\copydoc DWFXMLSerializable::serializeXML()
    ///
    _DWFTK_API
    virtual void serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
        throw( DWFException );

#endif

private:

    DWFContentPresentationView( const DWFContentPresentationView& );
    DWFContentPresentationView& operator=( const DWFContentPresentationView& );

    void setParent( DWFContentPresentationViewContainer* pParent )
    {
        _pParent = pParent;
    }

private:

    DWFString                               _zID;
    DWFString                               _zLabel;
    DWFString                               _zPropertySetID;
    DWFString                               _zPropertySchemaID;
    bool                                    _bUseDefaultIfNotFound;
    bool                                    _bHidden;
    DWFContentPresentationViewContainer*    _pParent;
};

}

#endif

