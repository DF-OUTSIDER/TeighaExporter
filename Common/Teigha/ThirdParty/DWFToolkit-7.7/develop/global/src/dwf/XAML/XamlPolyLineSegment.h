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
#if !defined XAML_POLYLINESEGMENT
#define XAML_POLYLINESEGMENT

#include "XAML/XamlDrawable.h"
#include "XAML/XamlGraphicsObject.h"
#include "XAML/XamlPointset.h"
#include "XAML/MemoryBuffer.h"

class XamlPolylineSegment : public XamlGraphicsObject
{
public:
    XamlPolylineSegment(const WT_XAML_Point_Set_Data& pointSet, bool bIsClosed = false);
    virtual ~XamlPolylineSegment(void) throw();

    teGraphicsObjectType type() { return XamlGraphicsObject::PolylineSegment; }

    virtual void setWriteStart(bool b) { _bIsWriteStart = b; }
    virtual WT_Result   serializeAttributeValue( WT_XAML_File &rFile, tMemoryBuffer*& rpBuffer ) const;

    bool& closed()                                  { return _bIsClosed; }
    const bool& closed() const                      { return _bIsClosed; }
    const WT_XAML_Point_Set_Data& pointSet() const  { return _oPointSet; }

private:
    WT_XAML_Point_Set_Data    _oPointSet;
    bool                      _bIsWriteStart;
    bool                      _bIsClosed;

};

#endif //XAML_POLYLINESEGMENT
