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

#if !defined XAML_FONT_HEADER
#define XAML_FONT_HEADER

///
///\file        XAML/XamlFont.h
///

#include "XAML/XamlCore.h"

#include "whiptk/font.h"
#include "XAML/XamlXML.h"


/** \addtogroup groupRenditionAttributes
 *  @{
 */

/// Rendition attribute describing the current text font.
/** \sa WT_Rendition \sa WT_Text \sa \ref groupFontOptions "Font options"
 *
 *
 *  \sa See the embedded fonts example provided in the sample app \em W2DTest accompanying the WHIP!
 *  Toolkit source distribution.
 */
class XAMLTK_API WT_XAML_Font 
    : public WT_Font
    , public XamlDrawableAttributes::FontRenderingEmSize::Provider
    , public XamlDrawableAttributes::FontUri::Provider
	, public XamlDrawableAttributes::IsSideways::Provider
    , public XamlDrawableAttributes::StyleSimulations::Provider
    , public XamlDrawableAttributes::GlyphsAttributeProvider
{
	friend class WT_XAML_Class_Factory;
	friend class WT_XAML_Rendition;

protected:
    //@{ \name Construction
    /// Constructs a WT_XAML_Font object.
    WT_XAML_Font()
        : WT_Font( ) 
        , _pSerializeFile( NULL )
	{}

    /// Constructs a WT_XAML_Font object from the given data.
    WT_XAML_Font (
        WT_String             name, /**< Font name. */
        WT_Boolean            bold, /**< Boldface option. */
        WT_Boolean            italic, /**< Italics option. */
        WT_Boolean            underline, /**< Underline option. */
        WT_Byte               charset, /**< Character set code as defined in WT_Font_Option_Charset::Font_Charset_Code. */
        WT_Byte               pitch, /**< Pitch bitmask as defined in WT_Font_Option_Pitch::Font_Pitch_Flags. */
        WT_Byte               family, /**< Font family as defined in WT_Font_Option_Pitch::Font_Family_Flags. */
        WT_Integer32          height, /**< Font height in drawing units. */
        WT_Unsigned_Integer16 rotation, /**< Font rotation in 360/65636ths of a degree. */
        /** The desired width scaling (multiplied by 1024) to be applied to the font.  A value of 1024 indicates
         *  no scaling, whereas 2048 indicates a font twice as wide as normal, and 512 indicates a font 1/2 as
         *  wide as normal. May range from a value of one (1/1024 of normal) to 65535 (64 times normal). */
        WT_Unsigned_Integer16 width_scale,
        /** The desired inter-character spacing scale (multiplied by 1024) to be applied to the font.  A value
         *  of 1024 indicates normal inter-character spacing, whereas 2048 indicates spacing that is twice as
         *  wide as normal, and 512 indicates spacing that is 1/2 as wide as normal. May range from a value of
         *  one (1/1024 of normal) to 65535 (64 times normal). */
        WT_Unsigned_Integer16 spacing,
        /** The desired rotation of the individual characters within the font in 360/65636ths of a degree.  Note
         *  that \a rotation describes how the entire string is rotated, whereas \a oblique rotates the characters
         *  in place. */
        WT_Unsigned_Integer16 oblique,
        WT_Integer32          flags = 0 /**< \em RESERVED.  The font flags are rarely used. */
        )
		:WT_Font(name,bold,italic,underline,charset,pitch,family,height,rotation,width_scale,spacing,oblique,flags)
        , _pSerializeFile( NULL )
	{}

	WT_XAML_Font(WT_Font const& font)
		:WT_Font(font)
	{}

	virtual ~WT_XAML_Font()
	{}
    //@}
public:

	 /// Assignment operator.  Copies the given font settings to this one.
    WT_XAML_Font const & operator=  (WT_XAML_Font const & font)
	{
		WT_Font::operator = (font);
		return *this;
	}

	 //@{ \name WT_Object and WT_Attribute virtual methods
    WT_Result        serialize (WT_File & /*file*/) const;

	/// partial materialization of shell from W2X
	WT_Result		 parseAttributeList(XamlXML::tAttributeMap& rMap, WT_XAML_File& rFile);

    //Font utility functions
    static WT_Result obfuscate(DWFInputStream* in, DWFOutputStream* out, const char* key);
    static WT_Result parseKey( char* keyOut, const wchar_t* zFontUriIn);

protected:
    virtual WT_Result provideFontRenderingEmSize( XamlDrawableAttributes::FontRenderingEmSize*& );
    virtual WT_Result provideFontUri( XamlDrawableAttributes::FontUri*& );
	virtual WT_Result provideIsSideways( XamlDrawableAttributes::IsSideways*& );
    virtual WT_Result provideStyleSimulations( XamlDrawableAttributes::StyleSimulations*& );
    virtual WT_Result provideGlyphsAttributes( XamlDrawableAttributes::GlyphsAttributeConsumer* p, WT_XAML_File& r) const;

private:
    WT_XAML_File *_pSerializeFile;

public:
    //This is the TTF file header
    typedef struct _tagTT_OFFSET_TABLE{
        unsigned short  uMajorVersion;
        unsigned short  uMinorVersion;
        unsigned short  uNumOfTables;
        unsigned short  uSearchRange;
        unsigned short  uEntrySelector;
        unsigned short  uRangeShift;
    }TT_OFFSET_TABLE;

    //Tables in the TTF file and their placement and name (tag)
    typedef struct _tagTT_TABLE_DIRECTORY{
        char  szTag[4];      //table name
        unsigned long uCheckSum;     //Check sum
        unsigned long uOffset;       //Offset from beginning of file
        unsigned long uLength;       //length of the table in bytes
    }TT_TABLE_DIRECTORY;

    //TTF HEAD table
    typedef struct _tagTT_HEAD_TABLE
    {
        unsigned long nTable;               //version number 0x00010000 for version 1.0. 
        unsigned long nFfontRevision;        //Set by font manufacturer. 
        unsigned long nCheckSumAdjustment;   //To compute: set it to 0, sum the entire font as unsigned long, then store 0xB1B0AFBA - sum. 
        unsigned long nMagicNumber;          //Set to 0x5F0F3CF5. 
        unsigned short nFlags;               //Bit 0: Baseline for font at y=0;
        //Bit 1: Left sidebearing point at x=0;
        //Bit 2: Instructions may depend on point size; 
        //Bit 3: Force ppem to integer values for all internal scaler math; may use fractional ppem sizes if this bit is clear; 
        //Bit 4: Instructions may alter advance width (the advance widths might not scale linearly); 
        //Bits 5-10: These should be set according to Apple's specification . However, they are not implemented in OpenType. 
        //Bit 11: Font data is 'lossless,' as a result of having been compressed and decompressed with the Agfa MicroType Express engine.
        //Bit 12: Font converted (produce compatible metrics)
        //Bit 13: Font optimised for ClearType
        //Bit 14: Reserved, set to 0
        //Bit 15: Reserved, set to 0 
        unsigned short nUnitsPerEm;          //Valid range is from 16 to 16384. This value should be a power of 2 for fonts that have TrueType outlines. 
        long long nCreated;                  //Number of seconds since 12:00 midnight, January 1, 1904. 64-bit integer 
        long long nModified;                 //Number of seconds since 12:00 midnight, January 1, 1904. 64-bit integer 
        short nXMin;                         //For all glyph bounding boxes. 
        short nYMin;                         //For all glyph bounding boxes. 
        short nXMax;                         //For all glyph bounding boxes. 
        short nYMax;                         //For all glyph bounding boxes. 
        unsigned short nMacStyle;            //Bit 0: Bold (if set to 1); 
        //Bit 1: Italic (if set to 1) 
        //Bit 2: Underline (if set to 1) 
        //Bit 3: Outline (if set to 1) 
        //Bit 4: Shadow (if set to 1) 
        //Bit 5: Condensed (if set to 1) 
        //Bit 6: Extended (if set to 1) 
        //Bits 7-15: Reserved (set to 0).  
        unsigned short nLowestRecPPEM;       //Smallest readable size in pixels. 
        short nFontDirectionHint;            //0: Fully mixed directional glyphs; 
        //1: Only strongly left to right; 
        //2: Like 1 but also contains neutrals; 
        //-1: Only strongly right to left; 
        //-2: Like -1 but also contains neutrals. 1 
        short nIndexToLocFormat;             //0 for short offsets, 1 for long. 
        short nGlyphDataFormat;              //
    } TT_HEAD_TABLE;

    //Header of the horizontal header table
    typedef struct _tagTT_HHEA_TABLE_HEADER{
        unsigned long nVersion;  //Table version number
        short nAscender;         //Typographic ascent. (Distance from baseline of highest ascender)  
        short nDescender;        //Typographic descent. (Distance from baseline of lowest descender)  
        short nLineGap;          //Typographic line gap. Negative LineGap values are treated as zero in Windows 3.1, System 6, and System 7. 
        unsigned short nAdvanceWidthMax; //Maximum advance width value in 'hmtx' table. 
        short nMinLeftSideBearing;  //Minimum left side bearing value in 'hmtx' table. 
        short nMinRightSideBearing; //Minimum right side bearing value; calculated as Min(aw - lsb - (xMax - xMin)). 
        short nXMaxExtent;      //Max(lsb + (xMax - xMin)). 
        short nCaretSlopeRise;  //Used to calculate the slope of the cursor (rise/run); 1 for vertical. 
        short nCaretSlopeRun;   // 0 for vertical. 
        short nCaretOffset;     // The amount by which a slanted highlight on a glyph needs to be shifted to produce the best appearance. Set to 0 for non-slanted fonts 
        short nReserved0;       //(reserved) set to 0 
        short nReserved1;       //(reserved) set to 0 
        short nReserved2;       //(reserved) set to 0 
        short nReserved3;       //(reserved) set to 0 
        short nMetricDataFormat; // 0 for current format. 
        unsigned short nNumberOfHMetrics; //Number of hMetric entries in 'hmtx' table 
    } TT_HHEA_TABLE_HEADER;

    typedef struct _tagTT_HORZ_METRIC {
        unsigned short advanceWidth;
        short lsb;
    }  TT_HORZ_METRIC;

    typedef struct _tagTT_CMAP_TABLE_HEADER{
        unsigned short nVersion;
        unsigned short nNumTables;
    } TT_CMAP_TABLE_HEADER;

    typedef struct _tagTT_CMAP_TABLE_RECORD{
        unsigned short nPlatformID;
        unsigned short nEncodingID;
        unsigned long nOffset;
    } TT_CMAP_TABLE_RECORD;

    typedef struct _tagTT_CMAP_ENCODING_HEADER{
        unsigned short nFormat;
        unsigned short nLength;
        unsigned short nLanguage;
    } TT_CMAP_ENCODING_HEADER;

    typedef struct _tagTT_CMAP_ENCODING_0{
        TT_CMAP_ENCODING_HEADER header;
        unsigned char * pGlyphIdArray;
    } TT_CMAP_ENCODING_0;

    typedef struct _tagTT_CMAP_ENCODING_2_SUBHEADER{
        unsigned short nFirstCode;
        unsigned short nEntryCount;
        short nIdDelta;
        unsigned short nIdRangeOffset;
    } TT_CMAP_ENCODING_2_SUBHEADER;

    typedef struct _tagTT_CMAP_ENCODING_2{
        TT_CMAP_ENCODING_HEADER header;
        unsigned short subHeaderKeys[256];
        TT_CMAP_ENCODING_2_SUBHEADER* pSubHeaders; //[]
        //unsigned short[] pGlyphIndexArray;
    } TT_CMAP_ENCODING_2;

    typedef struct _tagTT_CMAP_ENCODING_4{
        TT_CMAP_ENCODING_HEADER header;
        unsigned short nSegCountX2;                 //2 x segCount
        unsigned short nSearchRange;                //2 x (2**floor(log2(segCount)))
        unsigned short nEntrySelector;              //log2(searchRange/2)
        unsigned short nRangeShift;                 //2 x segCount - searchRange
        //unsigned short endCount[segCount];        //End character code for each segment, last=0xFFFF
        //unsigned short reservePad;                //Set to 0
        //unsigned short startCount[segCount];      //Start character code for each segment
        //short idDelta[segCount]                   //Delta for all character codes in segment
        //unsigned short idrangeOffset[segCount];   //Offsets into glyphIdArray or 0
        //unsigned short glyphIdArray[]             //Glyph index array (arbitrary length)
    } TT_CMAP_ENCODING_4;

    typedef struct _tagTT_CMAP_ENCODING_6{
        TT_CMAP_ENCODING_HEADER header;
    } TT_CMAP_ENCODING_6;

    typedef struct _tagTT_CMAP_ENCODING_8{
        TT_CMAP_ENCODING_HEADER header;
    } TT_CMAP_ENCODING_8;

    typedef struct _tagTT_CMAP_ENCODING_10{
        TT_CMAP_ENCODING_HEADER header;
    } TT_CMAP_ENCODING_10;

    typedef struct _tagTT_CMAP_ENCODING_12{
        TT_CMAP_ENCODING_HEADER header;
    } TT_CMAP_ENCODING_12;

    class CachedTTFFontData
    {
    public:
        typedef std::map <unsigned short, unsigned short > tGlyphIndexMap;

        unsigned short nUnitsPerEM;
        TT_HHEA_TABLE_HEADER ttTblHHEA;
        tGlyphIndexMap oGlyphIndexMap;
        TT_HORZ_METRIC **ppHorzMetrics;
        TT_CMAP_ENCODING_HEADER **ppCMapTable;
		int nEncodingID;//1  Unicode; 0  Symbol.

        CachedTTFFontData()
            : nUnitsPerEM(0)
            , ppHorzMetrics(NULL)
            , ppCMapTable(NULL)
            , nEncodingID(-1)
        {
            ttTblHHEA.nNumberOfHMetrics = 0;
        }

        ~CachedTTFFontData()
        {
            if (ppHorzMetrics)
            {
                DWFCORE_FREE_MEMORY( *ppHorzMetrics );
                DWFCORE_FREE_MEMORY( ppHorzMetrics );
            }
            if (ppCMapTable)
            {
                DWFCORE_FREE_MEMORY( *ppCMapTable );
                DWFCORE_FREE_MEMORY( ppCMapTable );
            }
        }
    };
};
//@}


#endif //XAML_FONT_HEADER
