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
#if !defined XAML_ARCSEGMENT
#define XAML_ARCSEGMENT

#include "XAML/XamlCore.h"

#include "XAML/XamlDrawable.h"
#include "XAML/XamlGraphicsObject.h"
#include "XAML/MemoryBuffer.h"

class WT_Ellipse;

class XamlArcSegment : public XamlGraphicsObject
{
public:
    enum teSweepDirection
    {
        Clockwise,
        Counterclockwise
    };

public:
    XamlArcSegment(void);
	XamlArcSegment(const XamlArcSegment& s);

    explicit XamlArcSegment(const WT_Outline_Ellipse& rEllipse);
    explicit XamlArcSegment(const WT_Filled_Ellipse& rEllipse);
    virtual ~XamlArcSegment(void) throw();

    teGraphicsObjectType type() { return XamlGraphicsObject::ArcSegment; }
    virtual void setWriteStart(bool b) { _bIsWriteStart = b; }

    virtual WT_Result   serializeAttributeValue( WT_XAML_File &rFile, tMemoryBuffer*& rpBuffer ) const;
#ifdef EMCC
    const WT_Integer32& major_version() const           { return _nMajor; }
          WT_Integer32& major_version()                 { return _nMajor; }
    const WT_Integer32& minor_version() const           { return _nMinor; }
          WT_Integer32& minor_version()                 { return _nMinor; }
#else
    const WT_Integer32& major() const           { return _nMajor; }
          WT_Integer32& major()                 { return _nMajor; }
    const WT_Integer32& minor() const           { return _nMinor; }
          WT_Integer32& minor()                 { return _nMinor; }
#endif
    const WT_Logical_Point& position() const    { return _oPosition; }
          WT_Logical_Point& position()          { return _oPosition; }
    const float& start_radian() const           { return _fStart; }
          float& start_radian()                 { return _fStart; }
    const float& end_radian() const             { return _fEnd; }
          float& end_radian()                   { return _fEnd; }
    const float& tilt_radian() const            { return _fRotate; }
          float& tilt_radian()                  { return _fRotate; }

private:
    bool                _bIsWriteStart;
    WT_Integer32        _nMajor;
    WT_Integer32        _nMinor;
    WT_Logical_Point    _oPosition;
    float               _fStart; 
    float               _fEnd;
    float               _fRotate;
    bool                _bIsFilled;
    bool                _bIsClosed;
};

#endif //XAML_ARCSEGMENT
