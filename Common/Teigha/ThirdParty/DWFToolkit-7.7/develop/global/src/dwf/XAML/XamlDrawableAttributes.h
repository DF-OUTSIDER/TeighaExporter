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
#ifndef XAML_PATH_ATTRIBUTES
#define XAML_PATH_ATTRIBUTES

#include "XAML/XamlPathGeometry.h"
#include "XAML/XamlBrushes.h"
#include "dwfcore/Owner.h"
#include "dwfcore/String.h"
#include "whiptk/matrix.h"

namespace XamlDrawableAttributes
{
    static const char* const kpzEmpty_String = 0;

    //////////////////////////////////////////////////////////// - utility
    class BrushRef: public DWFOwner
    {
    public:
        BrushRef();
        BrushRef( const XamlBrush::Brush* p);
        const BrushRef& operator=( const XamlBrush::Brush* );
        const BrushRef& operator=( const BrushRef& );
        bool operator==(const BrushRef& s) const;
        bool operator==(const XamlBrush::Brush* p) const;
        virtual ~BrushRef() throw();

        virtual void notifyOwnableDeletion( DWFOwnable& rOwnable ) throw( DWFException );

    public:
        operator const XamlBrush::Brush*() const { return _pBrush; }
        const XamlBrush::Brush* operator->() const { return _pBrush; }

    private:
        XamlBrush::Brush* _pBrush;
    };

    //////////////////////////////////////////////////////////// - utility
    class IndexArrayRef
    {
    public:
        IndexArrayRef();
        IndexArrayRef( int* pnArray, size_t nCount ) throw(DWFException);
        const IndexArrayRef& operator=( const IndexArrayRef& ) throw(DWFException);
        const IndexArrayRef& operator=( vector<int> ) throw(DWFException);
        bool operator==(const IndexArrayRef& s) const;
        virtual ~IndexArrayRef();

    public:
        operator const int*() const { return _pnArray; }
        const int operator[]( unsigned int nIndex ) const throw(DWFException);
        const size_t count() const { return _nCount; }

    private:
        size_t _nCount;
		int* _pnArray;
    };

    //////////////////////////////////////////////////////////// - utility
    class MatrixRef
    {
    public:
        MatrixRef();
        MatrixRef( const MatrixRef& r);
        const MatrixRef& operator=( const WT_Matrix2D& r );
        const MatrixRef& operator=( const WT_Matrix& r );  //converts to 3x3
        const MatrixRef& operator=( const MatrixRef& r );
        const MatrixRef& operator=( const char* p );
        bool operator==(const MatrixRef& r) const;
        bool operator==(const WT_Matrix2D& r) const;
        bool operator!=(const WT_Matrix2D& r) const;
        virtual ~MatrixRef();

    public:
        operator const WT_Matrix2D&() const { return _oMatrix; }
        operator const wchar_t*() const { return _szMatrixString; }
        operator const DWFString&() const { return _szMatrixString; }

    private:
        WT_Matrix2D _oMatrix;
        DWFString _szMatrixString;
    };

    //////////////////////////////////////////////////////////// - utility
    class StringRef
    {
    public:
        StringRef();
        StringRef( const wchar_t* p);
        StringRef( const char* p);
        const StringRef& operator=( const wchar_t* );
        const StringRef& operator=( const char* );
        bool operator==(const StringRef& s) const;
        bool operator==(const wchar_t* p) const;
        virtual ~StringRef();

    public:
        operator const wchar_t*() const { return _szString; }
        operator const DWFString&() const { return _szString; }

    private:
        DWFString _szString;
    };

    ////////////////////////////////////////////////////////////    
    class Data : public XamlAttribute<Data>
    {
    public:
        Data() : _oGeometry() {}
        Data(const Data& s);
        Data& operator=(const Data& s);
        bool operator==(const Data& s) const;
        virtual ~Data(void) {}

        virtual WT_Result materializeAttribute( WT_XAML_File &, const char* );
        virtual WT_Result serializeAttribute( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual WT_Result serializeElement( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual bool validAsAttribute( void ) const;

    public:
        virtual const XamlPathGeometry& geometry() const { return _oGeometry; }
        virtual XamlPathGeometry& geometry() { return _oGeometry; }

    private:
        XamlPathGeometry _oGeometry;

        //
        // optimizations :
        // since 50% of the time is usually spent into re-allocating
        // string buffers while serializing a Path.Data, let's track
        // buffer sizes and reuse buffers as appropriate.
        //
        static size_t _nBufferSize;

    public:
        class Provider
        {
        public:
            virtual WT_Result provideData( Data *& ) = 0;
        };
        class Consumer
        {
        public:
            virtual WT_Result consumeData( Provider* p ) = 0;
        };
        
    };

    ////////////////////////////////////////////////////////////
    class Name : public XamlAttribute<Name>
    {
    public:
        Name() {}
        Name( const Name& r);
        Name& operator=( const Name& r);
        bool operator==(const Name& s) const;
        
        virtual WT_Result materializeAttribute( WT_XAML_File &, const char* );
        virtual WT_Result serializeAttribute( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual WT_Result serializeElement( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual bool validAsAttribute( void ) const;

    public:
        virtual const StringRef& name() const { return _oString; }
        virtual StringRef& name() { return _oString; }

    private:
        StringRef _oString;

    public:
        class Provider
        {
        public:
            virtual WT_Result provideName( Name *& ) = 0;
        };
        class Consumer
        {
        public:
            virtual WT_Result consumeName( Provider* p ) = 0;
        };
        
    };

    ////////////////////////////////////////////////////////////
    class Fill : public XamlAttribute<Fill>
    {
    public:
        Fill();
        Fill( const Fill& r);
        Fill& operator=( const Fill& r);
        bool operator==(const Fill& s) const;
        virtual ~Fill();

        virtual WT_Result materializeAttribute( WT_XAML_File &, const char* );
        virtual WT_Result serializeAttribute( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual WT_Result serializeElement( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual bool validAsAttribute( void ) const;

    public:
        virtual const BrushRef& brush() const { return _oBrush; }
        virtual BrushRef& brush() { return _oBrush; }

    private:
        BrushRef _oBrush;

    public:
        class Provider
        {
        public:
            virtual WT_Result provideFill( Fill *& ) = 0;
        };
        class Consumer
        {
        public:
            virtual WT_Result consumeFill( Provider* p ) = 0;
        };

    };

    ////////////////////////////////////////////////////////////
    class RenderTransform : public XamlAttribute<RenderTransform>
    {
    public:
        RenderTransform() {}
        RenderTransform( const RenderTransform& r);
        RenderTransform& operator=( const RenderTransform& r);
        bool operator==(const RenderTransform& s) const;
        
        virtual WT_Result materializeAttribute( WT_XAML_File &, const char* );
        virtual WT_Result serializeAttribute( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual WT_Result serializeElement( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual bool validAsAttribute( void ) const;

    public:
        virtual const MatrixRef& matrix() const { return _oMatrix; }
        virtual MatrixRef& matrix() { return _oMatrix; }

    private:
        MatrixRef _oMatrix;

    public:
        class Provider
        {
        public:
            virtual WT_Result provideRenderTransform( RenderTransform *& ) = 0;
        };
        class Consumer
        {
        public:
            virtual WT_Result consumeRenderTransform( Provider* p ) = 0;
        };
    };

    ////////////////////////////////////////////////////////////
    class Clip : public XamlAttribute<Clip>
    {
    public:
        Clip() : _oGeometry() {}
        Clip( const Clip& r);
        Clip& operator=( const Clip& r);
        bool operator==(const Clip& s) const;

        virtual WT_Result materializeAttribute( WT_XAML_File &, const char* );
        virtual WT_Result serializeAttribute( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual WT_Result serializeElement( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual bool validAsAttribute( void ) const;

    public:
        virtual const XamlPathGeometry& geometry() const { return _oGeometry; }
        virtual XamlPathGeometry& geometry() { return _oGeometry; }

    private:
        XamlPathGeometry _oGeometry;

    public:
        class Provider
        {
        public:
            virtual WT_Result provideClip( Clip *& ) = 0;
        };
        class Consumer
        {
        public:
            virtual WT_Result consumeClip( Provider* p ) = 0;
        };
    };

    ////////////////////////////////////////////////////////////
    class Opacity : public XamlAttribute<Opacity>
    {
    public:
        Opacity();
        Opacity( const Opacity& r);
        Opacity& operator=( const Opacity& r);
        bool operator==(const Opacity& s) const;

        virtual WT_Result materializeAttribute( WT_XAML_File &, const char* );
        virtual WT_Result serializeAttribute( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual WT_Result serializeElement( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual bool validAsAttribute( void ) const;

    public:
        virtual const float& value() const { return _fValue; }
        virtual float& value() { return _fValue; }

    private:
        float _fValue;

    public:
        class Provider
        {
        public:
            virtual WT_Result provideOpacity( Opacity *& ) = 0;
        };
        class Consumer
        {
        public:
            virtual WT_Result consumeOpacity( Provider* p ) = 0;
        };
    };
 
    ////////////////////////////////////////////////////////////
    class OpacityMask : public XamlAttribute<OpacityMask>
    {
    public:
        OpacityMask();
        OpacityMask( const OpacityMask& r);
        OpacityMask& operator=( const OpacityMask& r);
        bool operator==(const OpacityMask& s) const;
        ~OpacityMask();

        virtual WT_Result materializeAttribute( WT_XAML_File &, const char* );
        virtual WT_Result serializeAttribute( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual WT_Result serializeElement( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual bool validAsAttribute( void ) const;
        
    public:
        virtual const BrushRef& brush() const { return _oBrush; }
        virtual BrushRef& brush() { return _oBrush; }

    private:
        BrushRef _oBrush;

    public:
        class Provider
        {
        public:
            virtual WT_Result provideOpacityMask( OpacityMask *& ) = 0;
        };
        class Consumer
        {
        public:
            virtual WT_Result consumeOpacityMask( Provider* p ) = 0;
        };
    };
    
    ////////////////////////////////////////////////////////////
    class Stroke : public XamlAttribute<Stroke>
    {
    public:
        Stroke();
        Stroke( const Stroke& r);
        Stroke& operator=( const Stroke& r);
        bool operator==(const Stroke& s) const;
        ~Stroke();

        virtual WT_Result materializeAttribute( WT_XAML_File &, const char* );
        virtual WT_Result serializeAttribute( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual WT_Result serializeElement( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual bool validAsAttribute( void ) const;

    public:
        virtual const BrushRef& brush() const { return _oBrush; }
        virtual BrushRef& brush() { return _oBrush; }

    private:
        BrushRef _oBrush;


    public:
        class Provider
        {
        public:
            virtual WT_Result provideStroke( Stroke *& ) = 0;
        };
        class Consumer
        {
        public:
            virtual WT_Result consumeStroke( Provider* p ) = 0;
        };
    };
    
    ////////////////////////////////////////////////////////////
    class StrokeDashArray : public XamlAttribute<StrokeDashArray>
    {
    public:
        StrokeDashArray() : _vfDashGap() {}
        StrokeDashArray( const StrokeDashArray& r);
        StrokeDashArray& operator=( const StrokeDashArray& r);
        bool operator==(const StrokeDashArray& s) const;

        virtual WT_Result materializeAttribute( WT_XAML_File &, const char* );
        virtual WT_Result serializeAttribute( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual WT_Result serializeElement( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual bool validAsAttribute( void ) const;

    public:
        typedef pair< float,float > tDashGapPair;
        typedef vector< tDashGapPair > tDashGapVector;

    public:
        virtual const tDashGapVector& dashGap() const { return _vfDashGap; }
        virtual tDashGapVector& dashGap() { return _vfDashGap; }

    private:
        tDashGapVector _vfDashGap;

    public:
        class Provider
        {
        public:
            virtual WT_Result provideStrokeDashArray( StrokeDashArray *& ) = 0;
        };
        class Consumer
        {
        public:
            virtual WT_Result consumeStrokeDashArray( Provider* p ) = 0;
        };
    };
    
    ////////////////////////////////////////////////////////////
    class StrokeDashCap : public XamlAttribute<StrokeDashCap>
    {
    public:
        StrokeDashCap();
        StrokeDashCap( const StrokeDashCap& r);
        StrokeDashCap& operator=( const StrokeDashCap& r);
        bool operator==(const StrokeDashCap& s) const;

        virtual WT_Result materializeAttribute( WT_XAML_File &, const char* );
        virtual WT_Result serializeAttribute( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual WT_Result serializeElement( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual bool validAsAttribute( void ) const;

    public:
        enum eCapStyle
        {
            Flat,
            Square,
            Round,
            Triangle
        };

    public:
        const eCapStyle& style() const { return _eCapStyle; }
        eCapStyle& style() { return _eCapStyle; }

    private: 
        eCapStyle _eCapStyle;

    public:
        class Provider
        {
        public:
            virtual WT_Result provideStrokeDashCap( StrokeDashCap *& ) = 0;
        };
        class Consumer
        {
        public:
            virtual WT_Result consumeStrokeDashCap( Provider* p ) = 0;
        };
    };
 
    ////////////////////////////////////////////////////////////
    class StrokeDashOffset : public XamlAttribute<StrokeDashOffset>
    {
    public:
        StrokeDashOffset();
        StrokeDashOffset( const StrokeDashOffset& r);
        StrokeDashOffset& operator=( const StrokeDashOffset& r);
        bool operator==(const StrokeDashOffset& s) const;

        virtual WT_Result materializeAttribute( WT_XAML_File &, const char* );
        virtual WT_Result serializeAttribute( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual WT_Result serializeElement( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual bool validAsAttribute( void ) const;

    public:
        const float& value() const { return _fValue; }
        float& value() { return _fValue; }

    private:
        float _fValue;

    public:
        class Provider
        {
        public:
            virtual WT_Result provideStrokeDashOffset( StrokeDashOffset *& ) = 0;
        };
        class Consumer
        {
        public:
            virtual WT_Result consumeStrokeDashOffset( Provider* p ) = 0;
        };
    };
 
    ////////////////////////////////////////////////////////////
    class StrokeEndLineCap : public XamlAttribute<StrokeEndLineCap>
    {
    public:
        StrokeEndLineCap();
        StrokeEndLineCap( const StrokeEndLineCap& r);
        StrokeEndLineCap& operator=( const StrokeEndLineCap& r);
        bool operator==(const StrokeEndLineCap& s) const;

        virtual WT_Result materializeAttribute( WT_XAML_File &, const char* );
        virtual WT_Result serializeAttribute( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual WT_Result serializeElement( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual bool validAsAttribute( void ) const;

    public:
        enum eCapStyle
        {
            Flat,
            Square,
            Round,
            Triangle
        };

    public:
        const eCapStyle& style() const { return _eCapStyle; }
        eCapStyle& style() { return _eCapStyle; }

    private: 
        eCapStyle _eCapStyle;

    public:
        class Provider
        {
        public:
            virtual WT_Result provideStrokeEndLineCap( StrokeEndLineCap *& ) = 0;
        };
        class Consumer
        {
        public:
            virtual WT_Result consumeStrokeEndLineCap( Provider* p ) = 0;
        };
    };

    ////////////////////////////////////////////////////////////
    class StrokeStartLineCap : public XamlAttribute<StrokeStartLineCap>
    {
    public:
        StrokeStartLineCap();
        StrokeStartLineCap( const StrokeStartLineCap& r);
        StrokeStartLineCap& operator=( const StrokeStartLineCap& r);
        bool operator==(const StrokeStartLineCap& s) const;

        virtual WT_Result materializeAttribute( WT_XAML_File &, const char* );
        virtual WT_Result serializeAttribute( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual WT_Result serializeElement( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual bool validAsAttribute( void ) const;

    public:
        enum eCapStyle
        {
            Flat,
            Square,
            Round,
            Triangle
        };

    public:
        const eCapStyle& style() const { return _eCapStyle; }
        eCapStyle& style() { return _eCapStyle; }

    private: 
        eCapStyle _eCapStyle;

    public:
        class Provider
        {
        public:
            virtual WT_Result provideStrokeStartLineCap( StrokeStartLineCap *& ) = 0;
        };
        class Consumer
        {
        public:
            virtual WT_Result consumeStrokeStartLineCap( Provider* p ) = 0;
        };
    };

    ////////////////////////////////////////////////////////////
    class StrokeLineJoin : public XamlAttribute<StrokeLineJoin>
    {
    public:
        StrokeLineJoin();
        StrokeLineJoin( const StrokeLineJoin& r);
        StrokeLineJoin& operator=( const StrokeLineJoin& r);
        bool operator==(const StrokeLineJoin& s) const;

        virtual WT_Result materializeAttribute( WT_XAML_File &, const char* );
        virtual WT_Result serializeAttribute( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual WT_Result serializeElement( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual bool validAsAttribute( void ) const;

    public:
        enum eJoinStyle
        {
            None,
            Miter,
            Bevel,
            Round
        };

    public:
        const eJoinStyle& style() const { return _eJoinStyle; }
        eJoinStyle& style() { return _eJoinStyle; }

    private: 
        eJoinStyle _eJoinStyle;

    public:
        class Provider
        {
        public:
            virtual WT_Result provideStrokeLineJoin( StrokeLineJoin *& ) = 0;
        };
        class Consumer
        {
        public:
            virtual WT_Result consumeStrokeLineJoin( Provider* p ) = 0;
        };
    };

    ////////////////////////////////////////////////////////////
    class StrokeMiterLimit : public XamlAttribute<StrokeMiterLimit>
    {
    public:
        StrokeMiterLimit();
        StrokeMiterLimit( const StrokeMiterLimit& r);
        StrokeMiterLimit& operator=( const StrokeMiterLimit& r);
        bool operator==(const StrokeMiterLimit& s) const;

        virtual WT_Result materializeAttribute( WT_XAML_File &, const char* );
        virtual WT_Result serializeAttribute( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual WT_Result serializeElement( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual bool validAsAttribute( void ) const;

    public:
        const float& value() const { return _fValue; }
        float& value() { return _fValue; }

    private:
        float _fValue;

    public:
        class Provider
        {
        public:
            virtual WT_Result provideStrokeMiterLimit( StrokeMiterLimit *& ) = 0;
        };
        class Consumer
        {
        public:
            virtual WT_Result consumeStrokeMiterLimit( Provider* p ) = 0;
        };
    };
 
    ////////////////////////////////////////////////////////////
    class StrokeThickness : public XamlAttribute<StrokeThickness>
    {
    public:
        StrokeThickness();
        StrokeThickness( const StrokeThickness& r);
        StrokeThickness& operator=( const StrokeThickness& r);
        bool operator==(const StrokeThickness& s) const;

        virtual WT_Result materializeAttribute( WT_XAML_File &, const char* );
        virtual WT_Result serializeAttribute( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual WT_Result serializeElement( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual bool validAsAttribute( void ) const;

    public:
        const unsigned int& weight() const { return _nWeight; }
        unsigned int& weight() { return _nWeight; }

    private:
        unsigned int _nWeight;

    public:
        class Provider
        {
        public:
            virtual WT_Result provideStrokeThickness( StrokeThickness *& ) = 0;
        };
        class Consumer
        {
        public:
            virtual WT_Result consumeStrokeThickness( Provider* p ) = 0;
        };
    };
     
    ////////////////////////////////////////////////////////////
    class NavigateUri : public XamlAttribute<NavigateUri>
    {
    public:
        NavigateUri() {}
        NavigateUri( const NavigateUri& r);
        NavigateUri& operator=( const NavigateUri& r);
        bool operator==(const NavigateUri& s) const;

        virtual WT_Result materializeAttribute( WT_XAML_File &, const char* );
        virtual WT_Result serializeAttribute( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual WT_Result serializeElement( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual bool validAsAttribute( void ) const;

    public:
        virtual const StringRef& uri() const { return _oString; }
        virtual StringRef& uri() { return _oString; }

    private:
        StringRef _oString;

    public:
        class Provider
        {
        public:
            virtual WT_Result provideNavigateUri( NavigateUri *& ) = 0;
        };
        class Consumer
        {
        public:
            virtual WT_Result consumeNavigateUri( Provider* p ) = 0;
        };
    };

    ////////////////////////////////////////////////////////////
    class BidiLevel : public XamlAttribute<BidiLevel>
    {
    public:
        BidiLevel() : _nLevel(0) {}
        BidiLevel( const BidiLevel& r);
        BidiLevel& operator=( const BidiLevel& r);
        bool operator==(const BidiLevel& s) const;

        virtual WT_Result materializeAttribute( WT_XAML_File &, const char* );
        virtual WT_Result serializeAttribute( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual WT_Result serializeElement( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual bool validAsAttribute( void ) const;

    public:
        virtual const short& level() const { return _nLevel; }
        virtual short& level() { return _nLevel; }

    private:
        short _nLevel;

    public:
        class Provider
        {
        public:
            virtual WT_Result provideBidiLevel( BidiLevel *& ) = 0;
        };
        class Consumer
        {
        public:
            virtual WT_Result consumeBidiLevel( Provider* p ) = 0;
        };
    };

    ////////////////////////////////////////////////////////////
    class CaretStops : public XamlAttribute<CaretStops>
    {
    public:
        CaretStops() : _oStops() {}
        CaretStops( const CaretStops& r);
        CaretStops& operator=( const CaretStops& r);
        bool operator==(const CaretStops& r) const;

        virtual WT_Result materializeAttribute( WT_XAML_File &, const char* );
        virtual WT_Result serializeAttribute( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual WT_Result serializeElement( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual bool validAsAttribute( void ) const;

    public:
        virtual const IndexArrayRef& stops() const { return _oStops; }
        virtual IndexArrayRef& stops() { return _oStops; }

    private:
        IndexArrayRef _oStops;

    public:
        class Provider
        {
        public:
            virtual WT_Result provideCaretStops( CaretStops *& ) = 0;
        };
        class Consumer
        {
        public:
            virtual WT_Result consumeCaretStops( Provider* p ) = 0;
        };
    };

    ////////////////////////////////////////////////////////////
    class DeviceFontName : public XamlAttribute<DeviceFontName>
    {
    public:
        DeviceFontName() {}
        DeviceFontName( const DeviceFontName& r);
        DeviceFontName& operator=( const DeviceFontName& r);
        bool operator==(const DeviceFontName& r) const;

        virtual WT_Result materializeAttribute( WT_XAML_File &, const char* );
        virtual WT_Result serializeAttribute( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual WT_Result serializeElement( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual bool validAsAttribute( void ) const;

    public:
        virtual const StringRef& name() const { return _oString; }
        virtual StringRef& name() { return _oString; }

    private:
        StringRef _oString;

    public:
        class Provider
        {
        public:
            virtual WT_Result provideDeviceFontName( DeviceFontName *& ) = 0;
        };
        class Consumer
        {
        public:
            virtual WT_Result consumeDeviceFontName( Provider* p ) = 0;
        };
    };

    ////////////////////////////////////////////////////////////
    class FontRenderingEmSize : public XamlAttribute<FontRenderingEmSize>
    {
    public:
        FontRenderingEmSize();
        FontRenderingEmSize( const FontRenderingEmSize& r);
        FontRenderingEmSize& operator=( const FontRenderingEmSize& r);
        bool operator==(const FontRenderingEmSize& s) const;

        virtual WT_Result materializeAttribute( WT_XAML_File &, const char* );
        virtual WT_Result serializeAttribute( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual WT_Result serializeElement( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual bool validAsAttribute( void ) const;

    public:
        const float& value() const { return _fValue; }
        float& value() { return _fValue; }

    private:
        float _fValue;

    public:
        class Provider
        {
        public:
            virtual WT_Result provideFontRenderingEmSize( FontRenderingEmSize *& ) = 0;
        };
        class Consumer
        {
        public:
            virtual WT_Result consumeFontRenderingEmSize( Provider* p ) = 0;
        };
    };

    ////////////////////////////////////////////////////////////
    class FontUri : public XamlAttribute<FontUri>
    {
    public:
        FontUri() {}
        FontUri( const FontUri& r);
        FontUri& operator=( const FontUri& r);
        bool operator==(const FontUri& s) const;

        virtual WT_Result materializeAttribute( WT_XAML_File &, const char* );
        virtual WT_Result serializeAttribute( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual WT_Result serializeElement( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual bool validAsAttribute( void ) const;

    public:
        virtual const StringRef& uri() const { return _oString; }
        virtual StringRef& uri() { return _oString; }

    private:
        StringRef _oString;

    public:
        class Provider
        {
        public:
            virtual WT_Result provideFontUri( FontUri *& ) = 0;
        };
        class Consumer
        {
        public:
            virtual WT_Result consumeFontUri( Provider* p ) = 0;
        };
    };

    ////////////////////////////////////////////////////////////
    class OriginX : public XamlAttribute<OriginX>
    {
    public:
        OriginX();
        OriginX( const OriginX& r);
        OriginX& operator=( const OriginX& r);
        bool operator==(const OriginX& s) const;

        virtual WT_Result materializeAttribute( WT_XAML_File &, const char* );
        virtual WT_Result serializeAttribute( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual WT_Result serializeElement( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual bool validAsAttribute( void ) const;

    public:
        const float& value() const { return _fValue; }
        float& value() { return _fValue; }

    private:
        float _fValue;

    public:
        class Provider
        {
        public:
            virtual WT_Result provideOriginX( OriginX *& ) = 0;
        };
        class Consumer
        {
        public:
            virtual WT_Result consumeOriginX( Provider* p ) = 0;
        };
    };

    ////////////////////////////////////////////////////////////
    class OriginY : public XamlAttribute<OriginY>
    {
    public:
        OriginY();
        OriginY( const OriginY& r);
        OriginY& operator=( const OriginY& r);
        bool operator==(const OriginY& s) const;

        virtual WT_Result materializeAttribute( WT_XAML_File &, const char* );
        virtual WT_Result serializeAttribute( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual WT_Result serializeElement( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual bool validAsAttribute( void ) const;

    public:
        const float& value() const { return _fValue; }
        float& value() { return _fValue; }

    private:
        float _fValue;

    public:
        class Provider
        {
        public:
            virtual WT_Result provideOriginY( OriginY *& ) = 0;
        };
        class Consumer
        {
        public:
            virtual WT_Result consumeOriginY( Provider* p ) = 0;
        };
    };
 
    ////////////////////////////////////////////////////////////
    class IsSideways : public XamlAttribute<IsSideways>
    {
    public:
        IsSideways();
        IsSideways( const IsSideways& r);
        IsSideways& operator=( const IsSideways& r);
        bool operator==(const IsSideways& s) const;

        virtual WT_Result materializeAttribute( WT_XAML_File &, const char* );
        virtual WT_Result serializeAttribute( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual WT_Result serializeElement( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual bool validAsAttribute( void ) const;

    public:
        const bool& value() const { return _bValue; }
        bool& value() { return _bValue; }

    private:
        bool _bValue;

    public:
        class Provider
        {
        public:
            virtual WT_Result provideIsSideways( IsSideways *& ) = 0;
        };
        class Consumer
        {
        public:
            virtual WT_Result consumeIsSideways( Provider* p ) = 0;
        };
    };

    ////////////////////////////////////////////////////////////
    class Indices : public XamlAttribute<Indices>
    {
    public:
        Indices() {}
        Indices( const Indices& r);
        Indices& operator=( const Indices& r);
        bool operator==(const Indices& s) const;
        
        virtual WT_Result materializeAttribute( WT_XAML_File &, const char* );
        virtual WT_Result serializeAttribute( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual WT_Result serializeElement( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual bool validAsAttribute( void ) const;

    public:
        virtual const StringRef& string() const { return _oString; }
        virtual StringRef& string() { return _oString; }

    private:
        StringRef _oString;

    public:
        class Provider
        {
        public:
            virtual WT_Result provideIndices( Indices *& ) = 0;
        };
        class Consumer
        {
        public:
            virtual WT_Result consumeIndices( Provider* p ) = 0;
        };
        
    };

    ////////////////////////////////////////////////////////////
    class Resources : public XamlAttribute<Resources>
    {
    public:
        Resources() {}
        Resources( const Resources& r);
        Resources& operator=( const Resources& r);
        bool operator==(const Resources& s) const;
        
        virtual WT_Result materializeAttribute( WT_XAML_File &, const char* );
        virtual WT_Result serializeAttribute( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual WT_Result serializeElement( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual bool validAsAttribute( void ) const;

    public:
        virtual const StringRef& sourceRef() const { return _oString; }
        virtual StringRef& sourceRef() { return _oString; }

    private:
        StringRef _oString;

    public:
        class Provider
        {
        public:
            virtual WT_Result provideResources( Resources *& ) = 0;
        };
        class Consumer
        {
        public:
            virtual WT_Result consumeResources( Provider* p ) = 0;
        };
        
    };

    ////////////////////////////////////////////////////////////
    class UnicodeString : public XamlAttribute<UnicodeString>
    {
    public:
        UnicodeString() {}
        UnicodeString( const UnicodeString& r);
        UnicodeString& operator=( const UnicodeString& r);
        bool operator==(const UnicodeString& s) const;
        
        virtual WT_Result materializeAttribute( WT_XAML_File &, const char* );
        virtual WT_Result serializeAttribute( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual WT_Result serializeElement( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual bool validAsAttribute( void ) const;

    public:
        virtual const StringRef& string() const { return _oString; }
        virtual StringRef& string() { return _oString; }

    private:
        StringRef _oString;

    public:
        class Provider
        {
        public:
            virtual WT_Result provideUnicodeString( UnicodeString *& ) = 0;
        };
        class Consumer
        {
        public:
            virtual WT_Result consumeUnicodeString( Provider* p ) = 0;
        };
    };

    ////////////////////////////////////////////////////////////
    //StyleSimulations
    class StyleSimulations : public XamlAttribute<StyleSimulations>
    {
    public:
        StyleSimulations();
        StyleSimulations( const StyleSimulations& r);
        StyleSimulations& operator=( const StyleSimulations& r);
        bool operator==(const StyleSimulations& s) const;

        virtual WT_Result materializeAttribute( WT_XAML_File &, const char* );
        virtual WT_Result serializeAttribute( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual WT_Result serializeElement( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const;
        virtual bool validAsAttribute( void ) const;

    public:
        enum eSimulationStyle
        {
            None,
            Italic,
            Bold,
            BoldItalic
        };

    public:
        const eSimulationStyle& style() const { return _eSimulationStyle; }
        eSimulationStyle& style() { return _eSimulationStyle; }

    private: 
        eSimulationStyle _eSimulationStyle;

    public:
        class Provider
        {
        public:
            virtual WT_Result provideStyleSimulations( StyleSimulations *& ) = 0;
        };
        class Consumer
        {
        public:
            virtual WT_Result consumeStyleSimulations( Provider* p ) = 0;
        };
    };

    ////////////////////////////////////////////////////////////    
    class CanvasAttributeConsumer :
                 public XamlDrawableAttributes::Name::Consumer
               , public XamlDrawableAttributes::RenderTransform::Consumer
               , public XamlDrawableAttributes::Clip::Consumer
               , public XamlDrawableAttributes::Opacity::Consumer
               , public XamlDrawableAttributes::OpacityMask::Consumer
			   , public XamlDrawableAttributes::NavigateUri::Consumer
			   , public XamlDrawableAttributes::Resources::Consumer
    {
    };

    class PathAttributeConsumer : 
                 public XamlDrawableAttributes::Data::Consumer
               , public XamlDrawableAttributes::Name::Consumer
               , public XamlDrawableAttributes::Fill::Consumer
               , public XamlDrawableAttributes::RenderTransform::Consumer
               , public XamlDrawableAttributes::Clip::Consumer
               , public XamlDrawableAttributes::Opacity::Consumer
               , public XamlDrawableAttributes::OpacityMask::Consumer
               , public XamlDrawableAttributes::Stroke::Consumer
               , public XamlDrawableAttributes::StrokeDashArray::Consumer
               , public XamlDrawableAttributes::StrokeDashCap::Consumer
               , public XamlDrawableAttributes::StrokeDashOffset::Consumer
               , public XamlDrawableAttributes::StrokeEndLineCap::Consumer
               , public XamlDrawableAttributes::StrokeStartLineCap::Consumer
               , public XamlDrawableAttributes::StrokeLineJoin::Consumer
               , public XamlDrawableAttributes::StrokeMiterLimit::Consumer
               , public XamlDrawableAttributes::StrokeThickness::Consumer
               , public XamlDrawableAttributes::NavigateUri::Consumer
    {
    };

    class GlyphsAttributeConsumer :
                  public XamlDrawableAttributes::BidiLevel::Consumer
                , public XamlDrawableAttributes::CaretStops::Consumer
                , public XamlDrawableAttributes::DeviceFontName::Consumer
                , public XamlDrawableAttributes::Fill::Consumer
                , public XamlDrawableAttributes::FontRenderingEmSize::Consumer
                , public XamlDrawableAttributes::FontUri::Consumer
                , public XamlDrawableAttributes::OriginX::Consumer
                , public XamlDrawableAttributes::OriginY::Consumer
                , public XamlDrawableAttributes::IsSideways::Consumer
                , public XamlDrawableAttributes::Indices::Consumer
                , public XamlDrawableAttributes::UnicodeString::Consumer
                , public XamlDrawableAttributes::StyleSimulations::Consumer
                , public XamlDrawableAttributes::RenderTransform::Consumer
                , public XamlDrawableAttributes::Clip::Consumer
                , public XamlDrawableAttributes::Opacity::Consumer
                , public XamlDrawableAttributes::OpacityMask::Consumer
                , public XamlDrawableAttributes::Name::Consumer
                , public XamlDrawableAttributes::NavigateUri::Consumer
    {
    };

    //////////////////////////////////////////////////////////// - utility
    class FillProvider : public Fill::Provider 
    {
    public:
        FillProvider( XamlBrush::Brush* pBrush) 
            : _pBrush( pBrush )
        { }

        WT_Result provideFill( XamlDrawableAttributes::Fill *& rpFill )
        {
            if (rpFill == NULL)
            {
                rpFill = DWFCORE_ALLOC_OBJECT( XamlDrawableAttributes::Fill() );
                if (rpFill == NULL)
                {
                    return WT_Result::Out_Of_Memory_Error;
                }
            }

            if (_pBrush == NULL)
            {
                return WT_Result::Toolkit_Usage_Error;
            }

            rpFill->brush() = _pBrush;
            return WT_Result::Success;
        }

    private:
        XamlBrush::Brush* _pBrush;
    };

    //////////////////////////////////////////////////////////// - utility
    class ResourcesProvider : public Resources::Provider 
    {
    public:
        ResourcesProvider( const char* pSourceRef ) 
            : _oString( pSourceRef )
        { }

        WT_Result provideResources( XamlDrawableAttributes::Resources *& rpResources )
        {
            if (rpResources == NULL)
            {
                rpResources = DWFCORE_ALLOC_OBJECT( XamlDrawableAttributes::Resources() );
                if (rpResources == NULL)
                {
                    return WT_Result::Out_Of_Memory_Error;
                }
            }

            rpResources->sourceRef() = _oString;
            return WT_Result::Success;
        }
        virtual ~ResourcesProvider()
        {}

    private:
        StringRef _oString;
    };

    //////////////////////////////////////////////////////////// - utility
    class StrokeProvider : public Stroke::Provider 
    {
    public:
        StrokeProvider( XamlBrush::Brush * pBrush) 
            : _pBrush( pBrush )
        { }

        WT_Result provideStroke( XamlDrawableAttributes::Stroke *& rpStroke )
        {
            if (rpStroke == NULL)
            {
                rpStroke = DWFCORE_ALLOC_OBJECT( XamlDrawableAttributes::Stroke() );
                if (rpStroke == NULL)
                {
                    return WT_Result::Out_Of_Memory_Error;
                }
            }

            if (_pBrush == NULL)
            {
                return WT_Result::Toolkit_Usage_Error;
            }

            rpStroke->brush() = _pBrush;
            return WT_Result::Success;
        }

    private:
        XamlBrush::Brush* _pBrush;
    };

    //////////////////////////////////////////////////////////// - utility
    class RenderTransformProvider : public RenderTransform::Provider 
    {
    public:
        RenderTransformProvider( const WT_Matrix2D& rMatrix ) 
        { _oMatrix = rMatrix; }

		virtual ~RenderTransformProvider(void) {}

        WT_Result provideRenderTransform( XamlDrawableAttributes::RenderTransform *& rpRenderTransform )
        {
            if (rpRenderTransform == NULL)
            {
                rpRenderTransform = DWFCORE_ALLOC_OBJECT( XamlDrawableAttributes::RenderTransform() );
                if (rpRenderTransform == NULL)
                {
                    return WT_Result::Out_Of_Memory_Error;
                }
            }

            rpRenderTransform->matrix() = _oMatrix;
            return WT_Result::Success;
        }

    private:
        MatrixRef _oMatrix;
    };

    ////////////////////////////////////////////////////////////
    class CanvasAttributeProvider
    {
    public:
        virtual WT_Result provideCanvasAttributes( XamlDrawableAttributes::CanvasAttributeConsumer* p, WT_XAML_File& r) const = 0;
    };
    
    class PathAttributeProvider
    {
    public:
        virtual WT_Result providePathAttributes( XamlDrawableAttributes::PathAttributeConsumer* p, WT_XAML_File& r) const = 0;
    };

    class GlyphsAttributeProvider
    {
    public:
        virtual WT_Result provideGlyphsAttributes( XamlDrawableAttributes::GlyphsAttributeConsumer* p, WT_XAML_File& r) const = 0;
    };
    
}

#endif
