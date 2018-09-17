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
#if !defined XAML_GRAPHICS_OBJECT
#define XAML_GRAPHICS_OBJECT

#include "XAML/MemoryBuffer.h"

class XamlGraphicsObject : public DWFOwnable
{
public:
    enum teGraphicsObjectType
    {
        PolylineSegment,
        ArcSegment
    };

    XamlGraphicsObject(void);
    virtual ~XamlGraphicsObject(void) throw();

    virtual void setWriteStart(bool /*b*/) = 0;
    virtual teGraphicsObjectType type() = 0;

    virtual    WT_Result        serializeAttributeValue( WT_XAML_File &, tMemoryBuffer*& ) const { return WT_Result::Success; }
};

#endif //XAML_GRAPHICS_OBJECT
