//
//  Copyright (c) 2005-2006 by Autodesk, Inc.
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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/package/Instance.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//

#ifndef _DWFTK_INSTANCE_H
#define _DWFTK_INSTANCE_H

///
///\file        dwf/package/Instance.h
///\brief       This file contains the DWFInstance class declaration.
///

#include "dwfcore/STL.h"
#include "dwfcore/Vector.h"
#include "dwfcore/SkipList.h"

#include "dwf/Toolkit.h"
#include "dwf/package/XML.h"

namespace DWFToolkit
{

//
// fwd declarations
//
class DWFRenderable;


///
///\ingroup     dwfpackage
///
///\class       DWFInstance     dwf/package/Instance.h      "dwf/package/Instance.h"
///\brief       An DWFInstance corresponds to a rendition of a DWFObject or a DWFFeaturable,
///             i.e. a renderable element of the model.
///\since       7.2.0
///
///             A DWFInstance corresponds to a rendition of a DWFObject or a DWFFeaturable,
///             i.e. a renderable element of the model. It provides the mapping between the
///             rendered element and its graphical representation.
///
class DWFInstance : public DWFXMLBuildable
#ifndef DWFTK_READ_ONLY
                  , public DWFXMLSerializable
#endif
                  _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///\brief   This type defines a list of DWFInstance pointers.
    ///
    typedef DWFOrderedVector<DWFInstance*>          tList;
    ///
    ///\brief   This type defines a mapped collection of DWFInstance pointers.
    ///
    typedef DWFStringKeySkipList<DWFInstance*>      tMap;
    ///
    ///\brief   This type defines a caching iterator on a collection of DWFInstance pointers,
    ///         that caches the pointers in its own container.
    ///
    typedef DWFCachingIterator<DWFInstance*>        tCachingIterator;
    ///
    ///\brief   This type defines a basic iterator on a collection of DWFInstance pointers.
    ///
    typedef DWFIterator<DWFInstance*>               tIterator;

    ///
    ///\brief   This enumerates attributes of the graphics that the instance can store.
    ///
    //
    // These are bit flags
    //
    typedef enum teAttributes {

        ///
        ///\brief   This determines whether the instance is visible or not.
        ///
        eVisible        = 0x01,

        ///
        ///\brief   This determines whether the instance is transparent or not.
        ///
        eTransparent    = 0x02

    } teAttributes;

public:

    ///
    ///         Constructor. This constructor can be used if there is only
    ///         graphical node corresponding to an element.
    ///         The constructor is mainly for use by the DWFContent and the DWFXMLElementBuilder,
    ///         during publishing and reading respectively. 
    ///
    ///\param   zInstanceID         The UUID assigned to the object.
    ///\param   pRenderedElement    Pointer to the element that this instance renders.
    ///\param   iNodeID             The integer ID of the graphical nodes.
    ///\param   nAttributes         The visibility and transparency of the instance in terms of bitflags given by DWFInstance::teAttributes.
    ///\param   nGeometricVariationIndex    The index of the geometric variation to show. The default, -1, indicates there are none.
    ///\throw   DWFException
    ///
    _DWFTK_API
    DWFInstance( const DWFString&   zInstanceID,
                 DWFRenderable*     pRenderedElement,
                 int                iNodeID,
                 unsigned int       nAttributes = DWFInstance::eVisible,
                 int                nGeometricVariationIndex = -1 )
        throw( DWFException );

    ///
    ///         Constructor. 
    ///
    ///         The constructor is for use by the the DWFXMLElementBuilder,
    ///         during reading and should not be used directly.
    ///
    ///\throw   None
    ///
    _DWFTK_API
    DWFInstance()
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFInstance()
        throw();

    ///
    ///         Returns the identifier for this instance.
    ///
    ///\return  The ID.
    ///\throw   None
    ///
    const DWFString& id() const
        throw()
    {
        return _zID;
    }

    ///
    ///         Returns a pointer to the element being rendered.
    ///
    ///\return  Pointer to the rendered element.
    ///\throw   None
    ///
    DWFRenderable* getRenderedElement() const
        throw()
    {
        return _pRenderedElement;
    }

    ///
    ///         Access the graphics node ID of the instance.
    ///
    ///\return  The node ID.
    ///\throw   None
    ///
    int getNodeID() const
        throw()
    {
        return _nNodeID;
    }

    ///
    ///         Get the visibility of the instance.
    ///
    ///\return  True if the instance is set to visible.
    ///\throw   None
    ///
    bool getVisibility() const
        throw()
    {
        return ((_nAttributeFlags & eVisible) ? true : false);
    }

    ///
    ///         Set the visibility of the instance.
    ///
    ///\throw   None
    ///
    void setVisibility( bool bVisible )
        throw()
    {
        if (bVisible)
        {
            _nAttributeFlags |= eVisible;
        }
        else
        {
            _nAttributeFlags &= ~eVisible;
        }
    }

    ///
    ///         Get the transparency of the instance.
    ///
    ///\return  True if the instance is set to transparent.
    ///\throw   None
    ///
    bool getTransparency() const
        throw()
    {
        return ((_nAttributeFlags & eTransparent) ? true : false);
    }

    ///
    ///         Set the transparency of the instance.
    ///
    ///\throw   None
    ///
    void setTransparency( bool bTransparent )
        throw()
    {
        if (bTransparent)
        {
            _nAttributeFlags |= eTransparent;
        }
        else
        {
            _nAttributeFlags &= ~eTransparent;
        }
    }

    ///
    ///         Get the bit flags for the graphics attributes.
    ///
    ///\return  An unsigned int that representation the graphical state of the instance.
    ///         The possible values correspond to combinations of the enums in DWFInstance::teAttributes.
    ///\throw   None
    ///
    unsigned int getGraphicsAttributes() const
        throw()
    {
        return _nAttributeFlags;
    }

    ///
    ///         Set the graphics attributes overriding existing attributes
    ///
    ///\param   nAttributes The combination of attributes to which to set the instance to.
    ///         The possible values correspond to combinations of the enums in DWFInstance::teAttributes.
    ///\throw   DWFException
    ///
    void setGraphicsAttributes( unsigned int nAttributes)
        throw( DWFException )
    {
        if (nAttributes > (eVisible | eTransparent))
        {
            _DWFCORE_THROW( DWFInvalidArgumentException, /*NOXLATE*/L"Trying to set unknown attribute flags on the instance" );
        }

        _nAttributeFlags = nAttributes;
    }

    ///
    ///         Get the index of the geometric variation to activate. A negative value (-1) indicates
    ///         that there are no geometric variations.
    ///
    ///\return  The index of the geometric variation.
    ///\throw   None
    ///
    int getGeometricVariationIndex() const
        throw()
    {
        return _nGeometricVariationIndex;
    }

    ///
    ///         Set the index of the geometric variation to activate.
    ///
    ///\throw   None
    ///
    void setGeometricVariationIndex( int nIndex )
        throw()
    {
        _nGeometricVariationIndex = nIndex;
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
    void serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
        throw( DWFException );

#endif

    ///
    ///         Set the renderable element of the instance. This is to be used
    ///         only by the readers.
    ///
    ///\param   pRenderable The element that this instance renders.
    ///\throw   None
    ///
    void setRenderable( DWFRenderable* pRenderable )
    {
        _pRenderedElement = pRenderable;
    }

private:

    void _identify( const DWFString& zID )
        throw()
    {
        _zID = zID;
    }

private:

    //
    // The UUID of the instance.
    //
    DWFString           _zID;

    //
    // The object or feature that the instance renders.
    //
    DWFRenderable*      _pRenderedElement;

    //
    // The resource-specific ID of the graphics node - 2D or 3D, that
    // corresponds to the graphical rendition of the above object.
    //
    int                 _nNodeID;

    //
    // The graphics attributes of the instance
    //
    unsigned int        _nAttributeFlags;

    //
    // This just gives the published index of the geomtric variation to
    // activate. The publisher needs to ensure that the index is valid.
    // A -1 indicates that there are no geometric variations.
    //
    int                 _nGeometricVariationIndex;

private:

    //
    // Not Implemented
    //
    DWFInstance( const DWFInstance& );
    DWFInstance& operator=( const DWFInstance& );

};

}

#endif

