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


#ifndef _DWFTK_CONTENT_PRESENTATION_H
#define _DWFTK_CONTENT_PRESENTATION_H


///
///\file        dwf/presentation/ContentPresentation.h
///\brief       This file contains the DWFContentPresentation class declaration.
///

#include "dwfcore/Owner.h"
#include "dwfcore/DWFXMLSerializer.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"
#include "dwf/presentation/utility/ContentPresentationViewContainer.h"

namespace DWFToolkit
{

//
// fwd decl
//
class DWFContentPresentationContainer;

///
///\ingroup     dwfpresentation
///
///\class       DWFContentPresentation   dwf/package/ContentPresentation.h     "dwf/package/ContentPresentation.h"
///\brief       Content presentations are publisher defined means of allowing access to meta data (content) published
///             in a dwf.
///\since       7.2
///
///             Content presentations are publisher defined means of allowing access to meta data (content) published
///             in a dwf. Pre-defined ids can be used to publish "known" presentations.
///
class DWFContentPresentation : public DWFContentPresentationViewContainer
                             , public DWFXMLBuildable
#ifndef DWFTK_READ_ONLY
                             , public DWFXMLSerializable
#endif
                             , public DWFOwnable
                             _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

    friend class DWFContentPresentationContainer;

public:
    ///
    ///\brief   This type defines a list of DWFContentPresentation pointers.
    ///
    typedef DWFOrderedVector<DWFContentPresentation*>            tList;

public:

    ///
    ///\brief   Pre-defined ID for package contents presentation
    ///
    static const char* const kzID_PackageContents;
    ///
    ///\brief   Pre-defined ID for content navigation presentation
    ///
    static const char* const kzID_ContentNavigation;
    ///
    ///\brief   Pre-defined ID for views presentation
    ///
    static const char* const kzID_Views;

	static const char* const kzID_DefinedViews;
    ///
    ///\brief   Pre-defined ID for layers presentation
    ///
    static const char* const kzID_Layers;
    ///
    ///\brief   Pre-defined ID for tabular presentation
    ///
    static const char* const kzID_Table;
    ///
    ///\brief   Pre-defined ID for animation presentation
    ///
    static const char* const kzID_Animations;
	///
    ///\brief   Pre-defined ID for animation presentation
    ///
    static const char* const kzID_Showmotions;

    ///
    ///         Constructor
    ///
    ///\param   zID     An optional identifier used when referencing this presentation.
    ///\param   zLabel  An optional user-friendly label for this presentation.
    ///\throw   None
    ///
    _DWFTK_API
    DWFContentPresentation( const DWFString& zLabel = /*NOXLATE*/L"",
                            const DWFString& zID = /*NOXLATE*/L"" )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFContentPresentation()
        throw();

    ///
    ///         Sets an id for this presentation
    ///
    ///\param   zID             A string that specifies the id
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void setID( const DWFString& zID )
        throw( DWFException );

    ///
    ///         Returns the ID for this presentation
    ///         If no label has been set, returns the null string
    ///
    ///\return  The label for this presentation
    ///\throw   None
    ///
    _DWFTK_API
    virtual const DWFString& id() const
        throw();

    ///
    ///         Sets a user-friendly label for this presentation that 
    ///
    ///\param   zLabel       A string that specifies the label
    ///\throw   None
    ///
    _DWFTK_API
    virtual void setLabel( const DWFString& zLabel )
        throw();

    ///
    ///         Returns the label for this presentation
    ///         If no label has been set, returns the null string
    ///
    ///\return  The label for this presentation
    ///\throw   None
    ///
    _DWFTK_API
    virtual const DWFString& label() const
        throw();

    ///
    ///         Returns the parent of this presentation - the container which holds it.
    ///
    ///\return  The parent, or NULL if one has not yet been set.
    ///\throw   None
    ///
    virtual DWFContentPresentationContainer* parent() const
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

    DWFContentPresentation( const DWFContentPresentation& );
    DWFContentPresentation& operator=( const DWFContentPresentation& );

    void setParent( DWFContentPresentationContainer* pParent )
    {
        _pParent = pParent;
    }

private:

    DWFString                               _zID;
    DWFString                               _zLabel;
    DWFContentPresentationContainer*        _pParent;
};

    //
    // required for win32 dll external linkage
    //
#ifdef  _DWFCORE_WIN32_SYSTEM
#ifndef DWFTK_STATIC

_declspec(selectany) const char* const DWFContentPresentation::kzID_PackageContents        = "mXbIKdssFUOrLj3K7vZWHg";
_declspec(selectany) const char* const DWFContentPresentation::kzID_ContentNavigation      = "mnbIKdssFUOrLj3K7vZWHg";
_declspec(selectany) const char* const DWFContentPresentation::kzID_Views                  = "m3bIKdssFUOrLj3K7vZWHg";
_declspec(selectany) const char* const DWFContentPresentation::kzID_Layers                 = "nHbIKdssFUOrLj3K7vZWHg";
_declspec(selectany) const char* const DWFContentPresentation::kzID_Table                  = "nXbIKdssFUOrLj3K7vZWHg";
_declspec(selectany) const char* const DWFContentPresentation::kzID_Animations             = "nYbIKdssFUOrLj3K7vZWHg";
_declspec(selectany) const char* const DWFContentPresentation::kzID_Showmotions            = "nZbIKdssFUOrLj3K7vZWHg";
_declspec(selectany) const char* const DWFContentPresentation::kzID_DefinedViews		   = "m0bIKdssFUOrLj3K7vZWHg";

#endif
#endif

}

#endif
