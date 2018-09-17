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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/dwfx/DWFSection.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//
//

#ifndef _DWFTK_DWFXDWFSECTION_H
#define _DWFTK_DWFXDWFSECTION_H

#include "dwf/opc/XMLPart.h"
#include "dwf/dwfx/ResourcePart.h"

namespace DWFToolkit
{

//
// fwd declaration
//
class DWFSection;


class DWFXDWFSection : public OPCXMLPart
                     , protected DWFOwner
{

public:

    ///
    ///\brief   This type defines a list of DWFXDWFSection pointers.
    ///
    typedef DWFOrderedVector<DWFXDWFSection*>              tList;
    ///
    ///\brief   This type defines a mapped collection of DWFXDWFSection pointers.
    ///
    typedef DWFStringKeySkipList<DWFXDWFSection*>          tMap;
    ///
    ///\brief   This type defines a caching iterator on a collection of DWFXDWFSection
    ///         pointers. The iterator caches the pointers in its own container.
    ///
    typedef DWFCachingIterator<DWFXDWFSection*>            tCachingIterator;
    ///
    ///\brief   This type defines a basic iterator on a collection of 
    ///         DWFXDWFSection pointers.
    ///
    typedef DWFIterator<DWFXDWFSection*>                   tIterator;
    ///
    ///\brief   This type defines a basic iterator on a collection of 
    ///         DWFXDWFSection pointers.
    ///
    typedef DWFConstIterator<DWFXDWFSection*>              tConstIterator;

public:

    ///
    ///\brief   "descriptor.xml"
    ///
    static const char* const kzName;

public:

    _DWFTK_API
    DWFXDWFSection( DWFSection* pSection )
        throw( DWFException );

    _DWFTK_API
    virtual ~DWFXDWFSection()
        throw();

    _DWFTK_API
    DWFSection* const section() const
        throw()
    {
        return _pSection;
    }

    ///
    ///\copydoc OPCPart::setPath( const DWFString& zPath )
    ///
    _DWFTK_API
    virtual void setPath( const DWFString& zPath )
        throw();

    _DWFTK_API
    void addResourcePart( DWFXResourcePart* pResourcePart,
                          bool bOwn = true )
        throw();

    _DWFTK_API
    bool removeResourcePart( DWFXResourcePart* pResourcePart )
        throw();

    _DWFTK_API
    DWFXResourcePart::tConstIterator* resourceParts() const
        throw()
    {
        return _oResourceParts.constIterator();
    }

#ifndef DWFTK_READ_ONLY

    _DWFTK_API
    virtual void serializeXML( DWFXMLSerializer& rSerializer )
        throw( DWFException );

#endif

protected:

    _DWFTK_API
    void notifyOwnableDeletion( DWFOwnable& rOwnable )
        throw( DWFException );

private:

    _DWFTK_API
    void _updatePageRels()
        throw();

private:

    DWFSection*             _pSection;
    DWFXResourcePart::tList _oResourceParts;

private:

    //
    //  Not implemented
    //
    DWFXDWFSection( const DWFXDWFSection& pDWFSection );
    DWFXDWFSection& operator=( const DWFXDWFSection& pDWFSection );
};

//
// required for win32 dll external linkage
//
#ifdef  _DWFCORE_WIN32_SYSTEM
#ifndef DWFTK_STATIC

//DNT_START
_declspec(selectany) const char* const DWFXDWFSection::kzName = "descriptor.xml";
//DNT_END

#endif
#endif


}

#endif

