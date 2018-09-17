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
#if !defined XAML_BRUSHES
#define XAML_BRUSHES


#include "whiptk/whip_toolkit.h"
#include "whiptk/typedefs_defines.h"
#include "dwfcore/DWFXMLSerializer.h"
#include "dwfcore/Owner.h"
#include "XAML/XamlResource.h"

class WT_XAML_File;

namespace XamlBrush
{
    enum eType
    {
        kSolidColor,
        kImage,
        kRadialGradient,
        kLinearGradient,
        kVisual
    };

    class Brush : public DWFOwnable
    {
    public:
        // serialize as an XML element
        virtual ~Brush() throw() {}
        virtual WT_Result serializeElement( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const = 0;
        virtual const eType type() const = 0;
        virtual bool operator==( const Brush& ) const = 0;

        static size_t PrintColor( wchar_t* buf, size_t bufSize, const WT_RGBA32& rColor );
        static WT_Result ReadColor( WT_RGBA32& rColor, const char* zBuf );
    };

    //SolidColorBrush
    class SolidColor : public Brush
    {
    public:
        SolidColor();
        SolidColor( const WT_RGBA32 &r );
        ~SolidColor() throw();
        virtual bool operator==( const Brush& ) const;
        const eType type() const { return kSolidColor; }
    public:
        virtual WT_Result set( const WT_RGBA32 &r );
        virtual WT_Result materializeAttribute( WT_XAML_File &, const char * );
        virtual WT_Result serializeElement( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual const WT_RGBA32& color() const;
        virtual const DWFString& colorString() const;
    private:
        WT_RGBA32 _oColor;
        DWFString _szColorString;
    };    

    //ImageBrush
    class Image: public Brush, public XamlResource
    {
    public:
        Image();
        ~Image() throw();
        bool operator==( const Brush& ) const;
        const eType type() const { return kImage; }
    public:
        WT_Result serializeElement( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
    };    

    //RadialGradientBrush
    class RadialGradient: public Brush
    {
    public:
        RadialGradient();
        ~RadialGradient() throw();
        virtual bool operator==( const Brush& ) const;
        const eType type() const { return kRadialGradient; }
    public:
        virtual WT_Result serializeElement( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
    };    

    //LinearGradientBrush - this implementation only assumes two colors, needs work if more gradient stops are needed
    class LinearGradient: public Brush
    {
    public:
        LinearGradient();
        ~LinearGradient() throw();
        virtual bool operator==( const Brush& ) const;
        const eType type() const { return kLinearGradient; }

    public:
        virtual const WT_Point2D& startPoint() const { return _oStartPt; }
        virtual WT_Point2D& startPoint() { return _oStartPt; }
        virtual const WT_Point2D& endPoint() const { return _oEndPt; }
        virtual WT_Point2D& endPoint() { return _oEndPt; }

        virtual const WT_RGBA32& startColor() const { return _oStartColor; }
        virtual WT_RGBA32& startColor() { return _oStartColor; }
        virtual const WT_RGBA32& endColor() const { return _oEndColor; }
        virtual WT_RGBA32& endColor() { return _oEndColor; }

    private:
        WT_Point2D _oStartPt, _oEndPt;
        WT_RGBA32 _oStartColor, _oEndColor;

    public:
        virtual WT_Result serializeElement( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
    };    

    //
    // VisualBrush
    //
    // note : since <VisualBrush> almost always need a lot of drawable content,
    // it needs to be referenced as a resource
    //
    class Visual: public Brush, public XamlResource
    {
    public:

        Visual();
        ~Visual() throw();
        virtual bool operator==( const Brush& ) const;
        const eType type() const { return kVisual; }

    public:

        virtual WT_Result serializeElement( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
    };    

    //
    // image brush translator, derived from Image
    //
    class XamlImageBrush : public Image
    {
    public :

        //Need this empty constructor to create ImageBrush shell
        //while materialization
        XamlImageBrush()
            : _zName()
            , _oXfo()
            , _nDpi(-1)
            , _nWidth(0)
            , _nHeight(0)
        {}

        XamlImageBrush(
            const DWFString &,                              // name
            const WT_Matrix2D &,                            // image xfo
            WT_Unsigned_Integer16,                          // width > 0
            WT_Unsigned_Integer16);                         // height > 0

        XamlImageBrush(
            const DWFString &,                              // name
            const WT_Matrix2D &,                            // image xfo
            WT_Integer32,                                   // Scanned Resolution dpi
            WT_Unsigned_Integer16,                          // width > 0
            WT_Unsigned_Integer16);                         // height > 0

        ~XamlImageBrush() throw();

        //
        // from XamlBrush::Visual
        //
        WT_Result serializeElement(
            WT_XAML_File &,                                 // current file
            DWFCore::DWFXMLSerializer *) const;             // XML utility, !=0

        //
        // from XamlResource
        //
        bool operator==(
            const XamlResource&) const;                     // comparee

        //
        // from XamlResource
        //
        XamlResource *copy() const;
        
        //
        // from XamlResource
        //
        eResourceType resourceType() const;
        
        //
        // from XamlResource
        //
        WT_Result serializeResource(
            const wchar_t*,                                 // key, != 0
            WT_XAML_File &,                                 // current file
            DWFXMLSerializer &) const;                      // xml serializer

    private :

        DWFString _zName;
        WT_Matrix2D _oXfo;
        WT_Integer32 _nDpi;
        WT_Unsigned_Integer16 _nWidth;
        WT_Unsigned_Integer16 _nHeight;
    };

    //
    // user hatch XAML translator, built as a visual brush
    //
    // the hatch definition comes from a WT_User_Hatch_Pattern::Hatch_Pattern
    // multi-patterns hatches will have to use multiple XamlHatchBrush
    // instances.
    //
    class XamlHatchBrush : public Visual
    {
    public :

        //
        // ctor, holding the w2d pattern definition data
        // the HatchPattern's smart pointer is incremented
        //
        XamlHatchBrush(
           WT_User_Hatch_Pattern::Hatch_Pattern *);   // ptr from w2d

        //
        // dtor
        // the HatchPattern's smart pointer is decremented
        //
        ~XamlHatchBrush() throw();

        //
        // from XamlBrush::Visual
        //
        WT_Result serializeElement(
            WT_XAML_File &,                                 // current file
            DWFCore::DWFXMLSerializer *) const;             // XML utility, !=0

        //
        // from XamlResource
        //
        bool operator==(
            const XamlResource&) const;                     // comparee

        //
        // from XamlResource
        //
        XamlResource *copy() const;
        
        //
        // from XamlResource
        //
        eResourceType resourceType() const;
        
        //
        // from XamlResource
        //
        WT_Result serializeResource(
            const wchar_t*,                                 // key, != 0
            WT_XAML_File &,                                 // current file
            DWFXMLSerializer &) const;                      // xml serializer

    private :

        WT_User_Hatch_Pattern::Hatch_Pattern *_pPat;  // ptr from w2d
    };

    //
    // fill pattern XAML translator, built as a visual brush
    //
    //
    class XamlFixedPatternBrush : public Visual
    {
    public :

        //
        // ctor, holding the w2d pattern definition data
        // the HatchPattern's smart pointer is incremented
        //
        // note : Solid is _not_ supported (use the SolidColor brush
        // instead) and UserDefined won't draw anything
        //
        XamlFixedPatternBrush(
            WT_Fill_Pattern::WT_Pattern_ID,                 // what pattern ?
            double);                                        // pattern scale

        //
        // dtor
        // the HatchPattern's smart pointer is decremented
        //
        ~XamlFixedPatternBrush() throw();

        //
        // from XamlBrush::Visual
        //
        WT_Result serializeElement(
            WT_XAML_File &,                                 // current file
            DWFCore::DWFXMLSerializer *) const;             // XML utility, !=0

        //
        // from XamlResource
        //
        bool operator==(
            const XamlResource&) const;                     // comparee

        //
        // from XamlResource
        //
        XamlResource *copy() const;
        
        //
        // from XamlResource
        //
        eResourceType resourceType() const;
        
        //
        // from XamlResource
        //
        WT_Result serializeResource(
            const wchar_t*,                                 // key, != 0
            WT_XAML_File &,                                 // current file
            DWFXMLSerializer &) const;                      // xml serializer

    private :

        WT_Fill_Pattern::WT_Pattern_ID  _eId;               // what pattern ?
        double                       _fScale;               // pattern scale
    };

    //
    // user-defined pattern XAML translator, built as a visual brush
    //
    //
    class XamlUserPatternBrush : public Visual
    {
    public :

        //
        // ctor, holding the w2d pattern definition data
        // the FillPattern's smart pointer is incremented
        //
        XamlUserPatternBrush(
            WT_User_Fill_Pattern::Fill_Pattern *,     // what pattern ?
            double);                                        // pattern scale

        //
        // dtor
        // the FillPattern's smart pointer is decremented
        //
        ~XamlUserPatternBrush() throw();

        //
        // from XamlBrush::Visual
        //
        WT_Result serializeElement(
            WT_XAML_File &,                                 // current file
            DWFCore::DWFXMLSerializer *) const;             // XML utility, !=0

        //
        // from XamlResource
        //
        bool operator==(
            const XamlResource&) const;                     // comparee

        //
        // from XamlResource
        //
        XamlResource *copy() const;
        
        //
        // from XamlResource
        //
        eResourceType resourceType() const;
        
        //
        // from XamlResource
        //
        WT_Result serializeResource(
            const wchar_t*,                                 // key, != 0
            WT_XAML_File &,                                 // current file
            DWFXMLSerializer &) const;                      // xml serializer

    private :

        WT_User_Fill_Pattern::Fill_Pattern *_pPat;          // pattern spec, != 0
        double _fScale;                                     // pattern scale
    };

}

#endif //XAML_BRUSHES

