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
#if !defined XAML_CLASSFACTORY_HEADER
#define XAML_CLASSFACTORY_HEADER

///
/// \file       XAML/XamlClassFactory.h
///


#include "XAML/XamlCore.h"

#include "whiptk/whip_toolkit.h"
#include "whiptk/class_factory.h"
//#include "XAML/pch.h"

/// \class WT_XAML_Class_Factory
/// Class factory for creating DWFx 2D graphics streams.  We use 
/// the XAML moniker here, but technically our DWFx 2D streams are
/// based on the WPF subset of XAML, not actually "full XAML".
///
/// \see WT_Class_Factory
class XAMLTK_API WT_XAML_Class_Factory : public WT_Class_Factory
{
public:
    WT_XAML_Class_Factory(void);
public:
    virtual ~WT_XAML_Class_Factory(void);

    //Create__XAML_Class_Factoryhere

    ///
    ///\copydoc WT_Class_Factory::Create_File()
    ///
    virtual WT_File*        Create_File() 
        throw(DWFException);

	///
    ///\copydoc WT_Class_Factory::Create_Attribute_URL()
    ///
	virtual WT_Attribute_URL*		Create_Attribute_URL() throw(DWFException);	
	///
    ///\copydoc WT_Class_Factory::Create_Attribute_URL(WT_Attribute &attribute)
    ///
	virtual WT_Attribute_URL*		Create_Attribute_URL(WT_Attribute &attribute) throw(DWFException);
	///
    ///\copydoc WT_Class_Factory::Create_Attribute_URL(WT_Attribute &attribute, WT_URL_Item &item)
    ///
	virtual WT_Attribute_URL*		Create_Attribute_URL(WT_Attribute &attribute, WT_URL_Item &item) 
		throw(DWFException);
	///
    ///\copydoc WT_Class_Factory::Create_Attribute_URL(WT_Attribute &attribute,WT_Integer32 index,WT_Unsigned_Integer16 const * address, WT_Unsigned_Integer16 const * friendly_name )
    ///
	virtual WT_Attribute_URL*		Create_Attribute_URL(WT_Attribute &attribute,
												WT_Integer32 index, 
												WT_Unsigned_Integer16 const * address, 
												WT_Unsigned_Integer16 const * friendly_name ) 
												throw(DWFException);

	///
    ///\copydoc WT_Class_Factory::Create_Attribute_URL(const WT_Attribute_URL& url )
    ///
	virtual WT_Attribute_URL*		Create_Attribute_URL( WT_Attribute_URL const& url)
        throw(DWFException);

    //Code Page
    ///
    ///\copydoc WT_Class_Factory::Create_Code_Page()
    ///
    virtual WT_Code_Page*    Create_Code_Page() 
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Code_Page( WT_Integer32 page_number )
    ///
    virtual WT_Code_Page*    Create_Code_Page( WT_Integer32 page_number ) 
        throw(DWFException);

	///
    ///\copydoc WT_Class_Factory::Create_Code_Page( const WT_Code_Page& code_page ) 
    ///
	virtual WT_Code_Page*    Create_Code_Page( WT_Code_Page const& code_page )  
        throw(DWFException);

    //Color
    ///
    ///\copydoc WT_Class_Factory::Create_Color()
    ///
    virtual WT_Color*        Create_Color() 
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Color(WT_RGBA32 color, WT_Color::WT_Color_Map_Mode m=WT_Color::No_Mapping)
    ///
    virtual WT_Color*        Create_Color(WT_RGBA32 color, WT_Color::WT_Color_Map_Mode m=WT_Color::No_Mapping) 
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Color(int red, int green, int blue, WT_Color::WT_Color_Map_Mode m=WT_Color::No_Mapping)
    ///
    virtual WT_Color*        Create_Color(int red, int green, int blue, WT_Color::WT_Color_Map_Mode m=WT_Color::No_Mapping) 
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Color(int red, int green, int blue, int alpha, WT_Color::WT_Color_Map_Mode m=WT_Color::No_Mapping)
    ///
    virtual WT_Color*        Create_Color(int red, int green, int blue, int alpha, WT_Color::WT_Color_Map_Mode m=WT_Color::No_Mapping) 
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Color(WT_Color_Index index, WT_Color_Map const &color_map)
    ///
    virtual WT_Color*        Create_Color(WT_Color_Index index, WT_Color_Map const &color_map) 
        throw(DWFException);
	///
    ///\copydoc WT_Class_Factory::Create_Color(WT_Color const &color) 
    ///
    virtual WT_Color*		Create_Color(WT_Color const & /*color*/)
        throw(DWFException);

    //Color_Map    
    ///
    ///\copydoc WT_Class_Factory::Create_Color_Map(int file_revision_being_used=REVISION_WHEN_DEFAULT_COLORMAP_WAS_CHANGED)
    ///
    virtual WT_Color_Map*    Create_Color_Map(int file_revision_being_used=REVISION_WHEN_DEFAULT_COLORMAP_WAS_CHANGED) 
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Color_Map(int count, WT_RGBA32 const *map, WT_File &file)
    ///
    virtual WT_Color_Map*    Create_Color_Map(int count, WT_RGBA32 const *map, WT_File &file) 
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Color_Map(int count, WT_RGB const *map, WT_File &file)
    ///
    virtual WT_Color_Map*    Create_Color_Map(int count, WT_RGB const *map, WT_File &file) 
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Color_Map(WT_Color_Map const &cmap)
    ///
    virtual WT_Color_Map*    Create_Color_Map(WT_Color_Map const &cmap) 
        throw(DWFException);

    //WT_Contour_Set
    ///
    ///\copydoc WT_Class_Factory::Create_Contour_Set() 
    ///
    virtual WT_Contour_Set*         Create_Contour_Set() 
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Contour_Set(WT_File &file, WT_Integer32 contours, WT_Integer32 const *counts, WT_Integer32 point_count, WT_Logical_Point const *points, WT_Boolean copy) 
    ///
    virtual WT_Contour_Set*         Create_Contour_Set(WT_File &file, WT_Integer32 contours, WT_Integer32 const *counts, WT_Integer32 point_count, WT_Logical_Point const *points, WT_Boolean copy)
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Contour_Set(WT_File &file, WT_Integer32 point_count, WT_Logical_Point const *points)
    ///
     virtual WT_Contour_Set*        Create_Contour_Set(WT_File &file, WT_Integer32 point_count, WT_Logical_Point const *points)
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Contour_Set(WT_Contour_Set const &source, WT_Boolean copy=WD_True)
    ///
     virtual WT_Contour_Set*        Create_Contour_Set(WT_Contour_Set const &source, WT_Boolean copy=WD_True)
        throw(DWFException);

	///
	 ///
    ///\copydoc WT_Class_Factory::Create_Contrast_Color() 
    ///
	virtual WT_Contrast_Color*		Create_Contrast_Color() 
		throw(DWFException);
	  
	///
    ///\copydoc WT_Class_Factory::Create_Contrast_Color(WT_RGBA32 color) 
    ///
	virtual WT_Contrast_Color*		Create_Contrast_Color(WT_RGBA32 color)
		throw(DWFException);
	
	///
    ///\copydoc WT_Class_Factory::Create_Contrast_Color(int red,int green,int blue,int alpha = 255) 
    ///
    virtual WT_Contrast_Color*		Create_Contrast_Color(int red,int green,int blue,int alpha = 255) 
		throw(DWFException);
	
	///
    ///\copydoc WT_Class_Factory::Create_Contrast_Color(const WT_Contrast_Color& color ) 
    ///
	virtual WT_Contrast_Color* Create_Contrast_Color(WT_Contrast_Color const& color  ) 
        throw(DWFException);

    //Dash_Pattern
    ///
    ///\copydoc WT_Class_Factory::Create_Dash_Pattern()
    ///
    virtual WT_Dash_Pattern*    Create_Dash_Pattern() 
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Dash_Pattern(WT_Integer32 id_number, WT_Integer16 length, WT_Integer16 const *pArray)
    ///
    virtual WT_Dash_Pattern*    Create_Dash_Pattern(WT_Integer32 id_number, WT_Integer16 length, WT_Integer16 const *pArray)
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Dash_Pattern(WT_Dash_Pattern const &r)
    ///
    virtual WT_Dash_Pattern*    Create_Dash_Pattern(WT_Dash_Pattern const &r)
        throw(DWFException);

	//Delineate
	///
    ///\copydoc WT_Class_Factory::Create_Delineate() 
    ///
	virtual WT_Delineate*		Create_Delineate() 
        throw(DWFException);
	///
    ///\copydoc WT_Class_Factory::Create_Delineate(WT_Boolean delineate_state) 
    ///
    virtual WT_Delineate*		Create_Delineate(WT_Boolean delineate_state) 
        throw(DWFException);
	///
    ///\copydoc WT_Class_Factory::Create_Delineate(const WT_Delineate& delineate) 
    ///
	virtual WT_Delineate*        Create_Delineate(WT_Delineate const& delineate) 
        throw(DWFException);

	//DWF header
	///
    ///\copydoc WT_Class_Factory::Create_DWF_Header() 
    ///
	virtual WT_DWF_Header*				Create_DWF_Header() 
        throw(DWFException);

	//End of DWF
	///
    ///\copydoc WT_Class_Factory::Create_End_Of_DWF() 
    ///
	virtual	WT_End_Of_DWF*				Create_End_Of_DWF() 
		throw(DWFException);

    //Ellipse
    ///
    ///\copydoc WT_Class_Factory::Create_Filled_Ellipse() 
    ///
    virtual WT_Filled_Ellipse*    Create_Filled_Ellipse() 
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Filled_Ellipse(WT_Integer32 x, WT_Integer32 y, WT_Integer32 major, WT_Integer32 minor, WT_Unsigned_Integer16 start=0, WT_Unsigned_Integer16 end=0, WT_Unsigned_Integer16 tilt=0) 
    ///
    virtual WT_Filled_Ellipse*    Create_Filled_Ellipse(WT_Integer32 x, WT_Integer32 y, WT_Integer32 major, WT_Integer32 minor, WT_Unsigned_Integer16 start=0, WT_Unsigned_Integer16 end=0, WT_Unsigned_Integer16 tilt=0)
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Filled_Ellipse(WT_Logical_Point const &pos, WT_Integer32 major, WT_Integer32 minor, WT_Unsigned_Integer16 start=0, WT_Unsigned_Integer16 end=0, WT_Unsigned_Integer16 tilt=0)
    ///
    virtual WT_Filled_Ellipse*    Create_Filled_Ellipse(WT_Logical_Point const &pos, WT_Integer32 major, WT_Integer32 minor, WT_Unsigned_Integer16 start=0, WT_Unsigned_Integer16 end=0, WT_Unsigned_Integer16 tilt=0)
        throw(DWFException);
	///
    ///\copydoc WT_Class_Factory::Create_Filled_Ellipse(const WT_Filled_Ellipse& e)
    ///
    virtual WT_Filled_Ellipse*    Create_Filled_Ellipse(const WT_Filled_Ellipse& e)
        throw(DWFException);
    
    //Outline Ellipse
    ///
    ///\copydoc WT_Class_Factory::Create_Outline_Ellipse()
    ///
    virtual WT_Outline_Ellipse*            Create_Outline_Ellipse()
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Outline_Ellipse(WT_Integer32 x, WT_Integer32 y, WT_Integer32 major, WT_Integer32 minor, WT_Unsigned_Integer16 start=0, WT_Unsigned_Integer16 end=0, WT_Unsigned_Integer16 tilt=0)
    ///
     virtual WT_Outline_Ellipse*            Create_Outline_Ellipse(WT_Integer32 x, WT_Integer32 y, WT_Integer32 major, WT_Integer32 minor, WT_Unsigned_Integer16 start=0, WT_Unsigned_Integer16 end=0, WT_Unsigned_Integer16 tilt=0)
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Outline_Ellipse(WT_Logical_Point const &pos, WT_Integer32 major, WT_Integer32 minor, WT_Unsigned_Integer16 start=0, WT_Unsigned_Integer16 end=0, WT_Unsigned_Integer16 tilt=0)
    ///
     virtual WT_Outline_Ellipse*            Create_Outline_Ellipse(WT_Logical_Point const &pos, WT_Integer32 major, WT_Integer32 minor, WT_Unsigned_Integer16 start=0, WT_Unsigned_Integer16 end=0, WT_Unsigned_Integer16 tilt=0)
        throw(DWFException);

    ///
    ///\copydoc WT_Class_Factory::Create_Outline_Ellipse(const WT_Outline_Ellipse& e)
    ///
     virtual WT_Outline_Ellipse*            Create_Outline_Ellipse(const WT_Outline_Ellipse& e) 
		 throw(DWFException);

    //Embed
    ///
    ///\copydoc WT_Class_Factory::Create_Embed() 
    ///
    virtual WT_Embed*                    Create_Embed() 
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Embed(WT_Embed const &embed)  
    ///
    virtual WT_Embed*                    Create_Embed(WT_Embed const &embed) 
        throw(DWFException); 

    ///
    ///\copydoc WT_Class_Factory::Create_Embedded_Font()
    ///    
    virtual WT_Embedded_Font*           Create_Embedded_Font()
        throw(DWFException);

    ///
    ///\copydoc WT_Class_Factory::Create_Embedded_Font(int,int,int,WT_Integer32,WT_Byte*,WT_Integer32,WT_Byte*,WT_Integer32,WT_Byte*,WT_Boolean)
    ///    
    virtual WT_Embedded_Font*           Create_Embedded_Font(
        int /*request_type*/,
        int /*privilege_type*/,
        int /*character_set_type*/,
        WT_Integer32 /*data_size*/,
        WT_Byte * /*data*/,
        WT_Integer32 /*font_type_face_name_length*/,
        WT_Byte * /*font_type_face_name_string*/,
        WT_Integer32 /*font_logfont_name_length*/,
        WT_Byte * /*font_logfont_name_string*/,
        WT_Boolean /*copy*/)
        throw(DWFException);

	//Fill
	///
    ///\copydoc WT_Class_Factory::Create_Fill() 
    ///
	virtual WT_Fill*					Create_Fill() 
        throw(DWFException);
	///
    ///\copydoc WT_Class_Factory::Create_Fill(WT_Boolean fill) 
    ///
	virtual WT_Fill*					Create_Fill(WT_Boolean fill) 
        throw(DWFException);

	///
    ///\copydoc WT_Class_Factory::Create_Fill(WT_Fill const& fill) 
    ///
	virtual WT_Fill*					Create_Fill(WT_Fill const& fill) 
        throw(DWFException);

    //Fill Pattern
    ///
    ///\copydoc WT_Class_Factory::Create_Fill_Pattern() 
    ///
    virtual WT_Fill_Pattern*            Create_Fill_Pattern() 
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Fill_Pattern(WT_Fill_Pattern::WT_Pattern_ID n)
    ///
    virtual WT_Fill_Pattern*            Create_Fill_Pattern(WT_Fill_Pattern::WT_Pattern_ID n)
        throw(DWFException);

	///
    ///\copydoc WT_Class_Factory::Create_Fill_Pattern(WT_Fill_Pattern const& pattern) 
    ///
	 virtual WT_Fill_Pattern*            Create_Fill_Pattern(WT_Fill_Pattern const& pattern) 
        throw(DWFException);

    //Font
    ///
    ///\copydoc WT_Class_Factory::Create_Font()  
    ///
    virtual WT_Font*                    Create_Font() 
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Font()  
    ///
    virtual WT_Font*                    Create_Font(WT_Font const& font) 
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Font(WT_String const &name, WT_Boolean bold, WT_Boolean italic, WT_Boolean underline,WT_Byte charset, WT_Byte pitch, WT_Byte family, WT_Integer32 height, 
    ///                                                   WT_Unsigned_Integer16 rotation, WT_Unsigned_Integer16 width_scale, 
    ///                                                   WT_Unsigned_Integer16 spacing, WT_Unsigned_Integer16 oblique, 
    ///                                                   WT_Integer32 flags=0)
    ///
    virtual WT_Font*                    Create_Font(WT_String const &name, WT_Boolean bold, WT_Boolean italic, WT_Boolean underline, 
                                                   WT_Byte charset, WT_Byte pitch, WT_Byte family, WT_Integer32 height, 
                                                   WT_Unsigned_Integer16 rotation, WT_Unsigned_Integer16 width_scale, 
                                                   WT_Unsigned_Integer16 spacing, WT_Unsigned_Integer16 oblique, 
                                                   WT_Integer32 flags=0)
        throw(DWFException);

    //Font Extension											   
    ///
    ///\copydoc WT_Class_Factory::Create_Font_Extension() 
    ///
    virtual WT_Font_Extension* Create_Font_Extension() 
        throw(DWFException);

    ///
    ///\copydoc WT_Class_Factory::Create_Font_Extension(WT_String const &, WT_String const &)
    ///
    virtual WT_Font_Extension* Create_Font_Extension(WT_String const & logfont_name, WT_String const & cannonical_name) 
        throw(DWFException);

	//Font Options
	///
    ///\copydoc WT_Class_Factory::Create_Font_Option_Charset() 
    ///
	virtual WT_Font_Option_Charset*		Create_Font_Option_Charset() 
		throw(DWFException);
 	///
    ///\copydoc WT_Class_Factory::Create_Font_Option_Charset(WT_Byte charset) 
    ///
 	virtual WT_Font_Option_Charset*		Create_Font_Option_Charset(WT_Byte charset) 
		throw(DWFException);
 	///
    ///\copydoc WT_Class_Factory::Create_Font_Option_Family() 
    ///
	virtual WT_Font_Option_Family*		Create_Font_Option_Family() 
        throw(DWFException);
 	///
    ///\copydoc WT_Class_Factory::Create_Font_Option_Family(WT_Byte family) 
    ///
 	virtual WT_Font_Option_Family*		Create_Font_Option_Family(WT_Byte family) 
        throw(DWFException);
	///
    ///\copydoc WT_Class_Factory::Create_Font_Option_Flags() 
    ///
 	virtual	WT_Font_Option_Flags*		Create_Font_Option_Flags() 
        throw(DWFException);
 	///
    ///\copydoc WT_Class_Factory::Create_Font_Option_Flags(WT_Integer32 flags) 
    ///
 	virtual WT_Font_Option_Flags*		Create_Font_Option_Flags(WT_Integer32 flags) 
        throw(DWFException);
 	///
    ///\copydoc WT_Class_Factory::Create_Font_Option_Font_Name() 
    ///
	virtual WT_Font_Option_Font_Name*	Create_Font_Option_Font_Name() 
        throw(DWFException);
 	///
    ///\copydoc WT_Class_Factory::Create_Font_Option_Font_Name(WT_String const & name) 
    ///
 	virtual WT_Font_Option_Font_Name*	Create_Font_Option_Font_Name(WT_String const & name) 
        throw(DWFException);
	///
    ///\copydoc WT_Class_Factory::Create_Font_Option_Height() 
    ///
	virtual WT_Font_Option_Height*		Create_Font_Option_Height() 
        throw(DWFException);
 	///
    ///\copydoc WT_Class_Factory::Create_Font_Option_Height(WT_Integer32 height) 
    ///
 	virtual WT_Font_Option_Height*		Create_Font_Option_Height(WT_Integer32 height) 
        throw(DWFException);
	///
    ///\copydoc WT_Class_Factory::Create_Font_Option_Oblique() 
    ///
	virtual WT_Font_Option_Oblique*		Create_Font_Option_Oblique() 
        throw(DWFException);
 	///
    ///\copydoc WT_Class_Factory::Create_Font_Option_Oblique(WT_Unsigned_Integer16 oblique) 
    ///
 	virtual WT_Font_Option_Oblique*		Create_Font_Option_Oblique(WT_Unsigned_Integer16 oblique) 
		throw(DWFException);
	///
    ///\copydoc WT_Class_Factory::Create_Font_Option_Pitch() 
    ///
 	virtual WT_Font_Option_Pitch*		Create_Font_Option_Pitch() 
        throw(DWFException); 	
 	///
    ///\copydoc WT_Class_Factory::Create_Font_Option_Pitch(WT_Byte pitch) 
    ///
	virtual WT_Font_Option_Pitch*		Create_Font_Option_Pitch(WT_Byte pitch) 
        throw(DWFException);
	///
    ///\copydoc WT_Class_Factory::Create_Font_Option_Rotation() 
    ///
	virtual	WT_Font_Option_Rotation*	Create_Font_Option_Rotation() 
        throw(DWFException);
 	///
    ///\copydoc WT_Class_Factory::Create_Font_Option_Rotation(WT_Unsigned_Integer16 rotation) 
    ///
 	virtual WT_Font_Option_Rotation*	Create_Font_Option_Rotation(WT_Unsigned_Integer16 rotation) 
        throw(DWFException);
	///
    ///\copydoc WT_Class_Factory::Create_Font_Option_Spacing() 
    ///
	virtual	WT_Font_Option_Spacing*		Create_Font_Option_Spacing() 
        throw(DWFException);
 	///
    ///\copydoc WT_Class_Factory::Create_Font_Option_Spacing(WT_Unsigned_Integer16 spacing) 
    ///
 	virtual WT_Font_Option_Spacing*		Create_Font_Option_Spacing(WT_Unsigned_Integer16 spacing) 
        throw(DWFException);
	///
    ///\copydoc WT_Class_Factory::Create_Font_Option_Style(WT_Boolean bold=WD_False, WT_Boolean italic=WD_False, 
	///															WT_Boolean underlined=WD_False) 
    ///
	virtual WT_Font_Option_Style*		Create_Font_Option_Style(WT_Boolean bold=WD_False, WT_Boolean italic=WD_False, 
										WT_Boolean underlined=WD_False) 
        throw(DWFException);
	///
    ///\copydoc WT_Class_Factory::Create_Font_Option_Width_Scale() 
    ///
	virtual WT_Font_Option_Width_Scale*	Create_Font_Option_Width_Scale() 
        throw(DWFException);
 	///
    ///\copydoc WT_Class_Factory::Create_Font_Option_Width_Scale(WT_Unsigned_Integer16 width_scale) 
    ///
 	virtual WT_Font_Option_Width_Scale* Create_Font_Option_Width_Scale(WT_Unsigned_Integer16 width_scale) 
        throw(DWFException);

    //Gouraud Polyline
    ///
    ///\copydoc WT_Class_Factory::Create_Gouraud_Polyline() 
    ///
    virtual WT_Gouraud_Polyline*        Create_Gouraud_Polyline() 
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Gouraud_Polyline(int count, WT_Logical_Point const *points, 
    ///                                                          WT_RGBA32 const *colors, WT_Boolean copy)
    ///
    virtual WT_Gouraud_Polyline*        Create_Gouraud_Polyline(int count, WT_Logical_Point const *points, 
                                                              WT_RGBA32 const *colors, WT_Boolean copy)
        throw(DWFException);     

	///
    ///\copydoc WT_Class_Factory::Create_Gouraud_Polyline(WT_Gouraud_Polyline const& pline) 
    ///
	 virtual WT_Gouraud_Polyline*		Create_Gouraud_Polyline(WT_Gouraud_Polyline const& pline)
        throw(DWFException);

    //Gouraud Polytriangle
    ///
    ///\copydoc WT_Class_Factory::Create_Gouraud_Polytriangle() 
    ///
    virtual WT_Gouraud_Polytriangle*    Create_Gouraud_Polytriangle() 
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Gouraud_Polytriangle(int count, WT_Logical_Point const *points, 
    ///                                                              WT_RGBA32 const *colors, WT_Boolean copy)
    ///
    virtual WT_Gouraud_Polytriangle*    Create_Gouraud_Polytriangle(int count, WT_Logical_Point const *points, 
                                                                  WT_RGBA32 const *colors, WT_Boolean copy)
        throw(DWFException);
      
	///
    ///\copydoc WT_Class_Factory::Create_Gouraud_Polytriangle(WT_Gouraud_Polytriangle const& polytri) 
    ///
	virtual WT_Gouraud_Polytriangle*		Create_Gouraud_Polytriangle(WT_Gouraud_Polytriangle const& /*polytri*/)
        throw(DWFException);

    //Image
    ///
    ///\copydoc WT_Class_Factory::Create_Image() 
    ///
    virtual WT_Image*                    Create_Image() 
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Image(int image_type) 
    ///
    virtual WT_Image*                    Create_Image(int image_type) 
         throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Image(WT_Unsigned_Integer16 rows, 
    ///                                                 WT_Unsigned_Integer16 columns, 
    ///                                                 WT_Image::WT_Image_Format format, WT_Integer32 identifier, 
    ///                                                 WT_Color_Map const *color_map, WT_Integer32 data_size, 
    ///                                                 WT_Byte *data, WT_Logical_Point const &min_corner, 
    ///                                                 WT_Logical_Point const &max_corner, WT_Boolean copy)
    ///
    virtual WT_Image*                    Create_Image(WT_Unsigned_Integer16 rows, 
                                                     WT_Unsigned_Integer16 columns, 
                                                     WT_Image::WT_Image_Format format, WT_Integer32 identifier, 
                                                     WT_Color_Map const *color_map, WT_Integer32 data_size, 
                                                     WT_Byte *data, WT_Logical_Point const &min_corner, 
                                                     WT_Logical_Point const &max_corner, WT_Boolean copy,
                                                     WT_Integer32 dpi=-1)
        throw(DWFException);

    //Layer
    ///
    ///\copydoc WT_Class_Factory::Create_Layer() 
    ///
    virtual WT_Layer*                    Create_Layer() 
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Layer(WT_File &file, WT_Integer32 layer_num, char const *layer_name=NULL)
    ///
    virtual WT_Layer*                    Create_Layer(WT_File &file, WT_Integer32 layer_num, char const *layer_name=NULL)
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Layer(WT_File &file, WT_Integer32 layer_num, WT_Unsigned_Integer16 const *layer_name)
    ///
    virtual WT_Layer*                    Create_Layer(WT_File &file, WT_Integer32 layer_num, WT_Unsigned_Integer16 const *layer_name)
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Layer(WT_Layer const &layer)
    ///
    virtual WT_Layer*                    Create_Layer(WT_Layer const &layer)
        throw(DWFException);
     
    //Line Pattern
    ///
    ///\copydoc WT_Class_Factory::Create_Line_Pattern() 
    ///
    virtual WT_Line_Pattern*            Create_Line_Pattern() 
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Line_Pattern(WT_Line_Pattern::WT_Pattern_ID id) 
    ///
    virtual WT_Line_Pattern*            Create_Line_Pattern(WT_Line_Pattern::WT_Pattern_ID id)
        throw(DWFException);

	///
    ///\copydoc WT_Class_Factory::Create_Line_Pattern(WT_Line_Pattern const& pat) 
	///
	virtual WT_Line_Pattern*            Create_Line_Pattern(WT_Line_Pattern const& pat) 
        throw(DWFException);

    //Line Style    
    ///
    ///\copydoc WT_Class_Factory::Create_Line_Style() 
    ///
    virtual WT_Line_Style*                Create_Line_Style()
        throw(DWFException);

	///
    ///\copydoc WT_Class_Factory::Create_Line_Style(WT_Line_Style const &) 
	///
    virtual WT_Line_Style*                Create_Line_Style(WT_Line_Style const & /*style*/) 
        throw(DWFException);

	//Line Weight
	///
    ///\copydoc WT_Class_Factory::Create_Line_Weight() 
	///
	virtual WT_Line_Weight*				Create_Line_Weight() 
		throw(DWFException);
 	///
    ///\copydoc WT_Class_Factory::Create_Line_Weight(WT_Integer32 weight) 
	///
 	virtual WT_Line_Weight*				Create_Line_Weight(WT_Integer32 weight) 
		throw(DWFException);

    ///
    ///\copydoc WT_Class_Factory::Create_Line_Weight(WT_Line_Weight const &) 
    ///
    virtual WT_Line_Weight*             Create_Line_Weight(WT_Line_Weight const & /*weight*/) 
        throw(DWFException);

    //WT_Macro_Definition
    ///
    ///\copydoc WT_Class_Factory::Create_Macro_Definition()
    ///
    virtual WT_Macro_Definition*        Create_Macro_Definition()
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Macro_Definition( WT_Unsigned_Integer16 index,WT_Integer32 scale_units = 1 )
    ///
    virtual WT_Macro_Definition*        Create_Macro_Definition( WT_Unsigned_Integer16 index,WT_Integer32 scale_units = 1 )
        throw(DWFException);

    ///
    ///\copydoc WT_Class_Factory::Create_Macro_Draw()
    ///
    virtual WT_Macro_Draw*                Create_Macro_Draw() throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Macro_Draw(int count, WT_Logical_Point const * points,WT_Boolean copy )
    ///
    virtual WT_Macro_Draw*                Create_Macro_Draw(int count, WT_Logical_Point const * points,WT_Boolean copy )
        throw(DWFException); 

	///
	///\copydoc WT_Class_Factory::Create_Macro_Draw(WT_Macro_Draw const& draw) 
	///
	virtual WT_Macro_Draw*				Create_Macro_Draw(WT_Macro_Draw const& /*draw*/ )
        throw(DWFException);

    //Macro_Index
    ///
    ///\copydoc WT_Class_Factory::Create_Macro_Index()
    ///
    virtual WT_Macro_Index*                Create_Macro_Index()
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Macro_Index(WT_Integer32 macro)
    ///
    virtual WT_Macro_Index*                Create_Macro_Index(WT_Integer32 macro)
        throw(DWFException);

	///
	///\copydoc WT_Class_Factory::Create_Macro_Index(WT_Macro_Index const& draw) 
	///
	 virtual WT_Macro_Index*                Create_Macro_Index(WT_Macro_Index const& /*index*/) 
        throw(DWFException);

    //Macro_Scale
    ///
    ///\copydoc WT_Class_Factory::Create_Macro_Scale()
    ///
    virtual WT_Macro_Scale*                Create_Macro_Scale()
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Macro_Scale(WT_Integer32 macro_scale)
    ///
    virtual WT_Macro_Scale*                Create_Macro_Scale(WT_Integer32 macro_scale)
        throw(DWFException);

	///
	///\copydoc WT_Class_Factory::Create_Macro_Scale(WT_Macro_Scale const& scale) 
	///
	 virtual WT_Macro_Scale*                Create_Macro_Scale(WT_Macro_Scale const& /*scale*/) 
        throw(DWFException);

    //Merge_Control
    ///
    ///\copydoc WT_Class_Factory::Create_Merge_Control()
    ///
    virtual WT_Merge_Control*            Create_Merge_Control()
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Merge_Control(WT_Merge_Control::WT_Merge_Format merge)
    ///
    virtual WT_Merge_Control*            Create_Merge_Control(WT_Merge_Control::WT_Merge_Format merge)
        throw(DWFException);

	//Merge_Control
	///
	///\copydoc WT_Class_Factory::Create_Merge_Control(WT_Merge_Control const& merge_control) 
	///
	virtual WT_Merge_Control*            Create_Merge_Control(WT_Merge_Control const& merge_control) 
        throw(DWFException);

	//Named_View_List
	///
	///\copydoc WT_Class_Factory::Create_Named_View_List(void) 
	///
	virtual	WT_Named_View_List*				Create_Named_View_List(void) 
		throw(DWFException);
 	///
	///\copydoc WT_Class_Factory::Create_Named_View(WT_Named_View_List const& named_view_list) 
	///
 	virtual WT_Named_View_List*				Create_Named_View_List(WT_Named_View_List const &named_view_list) 
		throw(DWFException);

    //Named_View
    ///
    ///\copydoc WT_Class_Factory::Create_Named_View(void)
    ///
    virtual    WT_Named_View*                Create_Named_View(void)
        throw(DWFException);
     ///
    ///\copydoc WT_Class_Factory::Create_Named_View(WT_Named_View const &named_view)
    ///
    virtual WT_Named_View*                Create_Named_View(WT_Named_View const &named_view)
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Named_View(WT_Logical_Box const &view, const char *name=NULL)
    ///
    virtual WT_Named_View*                Create_Named_View(WT_Logical_Box const &view, const char *name=NULL)
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Named_View(WT_Logical_Box const &view, WT_Unsigned_Integer16 const *name)
    ///
    virtual WT_Named_View*                Create_Named_View(WT_Logical_Box const &view, WT_Unsigned_Integer16 const *name)
        throw(DWFException);

    //Object_Node
    ///
    ///\copydoc WT_Class_Factory::Create_Object_Node() 
    ///
    virtual WT_Object_Node*                Create_Object_Node() 
        throw(DWFException);
     ///
    ///\copydoc WT_Class_Factory::Create_Object_Node(WT_File &file, WT_Integer32 object_node_num, 
    ///                                                      char const *object_node_name=NULL)
    ///
    virtual WT_Object_Node*                Create_Object_Node(WT_File &file, WT_Integer32 object_node_num, 
                                                          char const *object_node_name=NULL)
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Object_Node(WT_File &file, WT_Integer32 object_node_num, 
    ///                                                     WT_Unsigned_Integer16 const *object_node_name)
    ///
    virtual WT_Object_Node*                Create_Object_Node(WT_File &file, WT_Integer32 object_node_num, 
                                                         WT_Unsigned_Integer16 const *object_node_name)
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Object_Node(WT_Object_Node const &object_node)
    ///
    virtual WT_Object_Node*                Create_Object_Node(WT_Object_Node const &object_node)
        throw(DWFException);

    //Overpost
    ///
    ///\copydoc WT_Class_Factory::Create_Overpost() 
    ///
    virtual WT_Overpost*                Create_Overpost() 
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Overpost( WT_Overpost::WT_AcceptMode acceptMode,WT_Boolean renderEntities,WT_Boolean addExtents)
    ///
    virtual WT_Overpost*                Create_Overpost( WT_Overpost::WT_AcceptMode acceptMode,WT_Boolean renderEntities,WT_Boolean addExtents)
        throw(DWFException);

    //Pen_Pattern
    ///
    ///\copydoc WT_Class_Factory::Create_Pen_Pattern() 
    ///
    virtual WT_Pen_Pattern*                Create_Pen_Pattern() 
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Pen_Pattern(WT_Pen_Pattern::WT_Pattern_ID pattern_id) 
    ///
    virtual WT_Pen_Pattern*                Create_Pen_Pattern(WT_Pen_Pattern::WT_Pattern_ID pattern_id) 
         throw(DWFException);
     ///
    ///\copydoc WT_Class_Factory::Create_Pen_Pattern(WT_Pen_Pattern::WT_Pattern_ID pattern_id, 
    ///                                                      WT_Unsigned_Integer32 screening_percentage, 
    ///                                                      WT_Boolean colormap_flag, WT_Color_Map const *color_map, 
    ///                                                      WT_Boolean copy) 
    ///
    virtual WT_Pen_Pattern*                Create_Pen_Pattern(WT_Pen_Pattern::WT_Pattern_ID pattern_id, 
                                                          WT_Unsigned_Integer32 screening_percentage, 
                                                          WT_Boolean colormap_flag, WT_Color_Map const *color_map, 
                                                          WT_Boolean copy) 
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Pen_Pattern(WT_Pen_Pattern const &pattern) 
    ///
    virtual WT_Pen_Pattern*                Create_Pen_Pattern(WT_Pen_Pattern const &pattern) 
         throw(DWFException);

    //PNG_Group4_Image
    ///
    ///\copydoc WT_Class_Factory::Create_PNG_Group4_Image()
    ///
    virtual WT_PNG_Group4_Image*        Create_PNG_Group4_Image() 
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_PNG_Group4_Image(int image_type)
    ///
    virtual WT_PNG_Group4_Image*        Create_PNG_Group4_Image(int image_type) 
         throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_PNG_Group4_Image(int image_typeWT_Unsigned_Integer16 columns, 
    ///                                                          WT_PNG_Group4_Image::WT_PNG_Group4_Image_Format format, 
    ///                                                          WT_Integer32 identifier, 
    ///                                                          WT_Color_Map const *color_map, 
    ///                                                          WT_Integer32 data_size, 
    ///                                                          WT_Byte *data, 
    ///                                                          WT_Logical_Point const &min_corner, 
    ///                                                          WT_Logical_Point const &max_corner,     
    ///                                                          WT_Boolean copy)
    ///
    virtual WT_PNG_Group4_Image*        Create_PNG_Group4_Image(WT_Unsigned_Integer16 rows, 
                                                              WT_Unsigned_Integer16 columns, 
                                                              WT_PNG_Group4_Image::WT_PNG_Group4_Image_Format format, 
                                                              WT_Integer32 identifier, 
                                                              WT_Color_Map const *color_map, 
                                                              WT_Integer32 data_size, 
                                                              WT_Byte *data, 
                                                              WT_Logical_Point const &min_corner, 
                                                              WT_Logical_Point const &max_corner,     
                                                              WT_Boolean copy,
                                                              WT_Integer32 dpi=-1)  
        throw(DWFException);

    //Polygon
    ///
    ///\copydoc WT_Class_Factory::Create_Polygon() 
    ///
    virtual WT_Polygon*                    Create_Polygon() 
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Polygon(int count, WT_Logical_Point const *points, WT_Boolean copy) 
    ///
    virtual WT_Polygon*                    Create_Polygon(int count, WT_Logical_Point const *points, WT_Boolean copy) 
         throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Polygon(const WT_Polygon &pline) 
    ///
    virtual WT_Polygon*                    Create_Polygon(const WT_Polygon &pline) 
         throw(DWFException);

    //Polyline
    ///
    ///\copydoc WT_Class_Factory::Create_Polyline() 
    ///
    virtual WT_Polyline*    Create_Polyline() 
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Polyline(
    ///                            int                         count, /**< The number of points in the array. */
    ///                            WT_Logical_Point const *    points, /**< Pointer to the array of points. */
    ///                            WT_Boolean                  copy /**< Whether the points should be copied or if their addresses should be used directly from the array. */
    ///                            )
    ///
    virtual WT_Polyline*    Create_Polyline(
                                int                         count, /**< The number of points in the array. */
                                WT_Logical_Point const *    points, /**< Pointer to the array of points. */
                                WT_Boolean                  copy /**< Whether the points should be copied or if their addresses should be used directly from the array. */
                                ) 
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Polyline(const WT_Polyline & pline) 
    ///
    virtual WT_Polyline*    Create_Polyline(const WT_Polyline & pline) 
        throw(DWFException);

    // Polymarker
    ///
    ///\copydoc WT_Class_Factory::Create_Polymarker()
    ///
    virtual WT_Polymarker* Create_Polymarker()
        throw(DWFException);

    ///
    ///\copydoc WT_Class_Factory::Create_Polymarker( int, WT_Logical_Point const *, WT_Boolean )
    ///
    virtual WT_Polymarker* Create_Polymarker(
        int                         count, /**< The number of points in the array. */
        WT_Logical_Point const *    points, /**< Pointer to the array of points. */
        WT_Boolean                  copy /**< Whether the points should be copied or if their addresses should be used directly from the array. */
        )
        throw(DWFException);

    ///
    ///\copydoc WT_Class_Factory::Create_Polymarker( WT_Polymarker const& )
    ///
	virtual WT_Polymarker* Create_Polymarker(WT_Polymarker const& marker) 
        throw(DWFException);

    //Polytriangle
    ///
    ///\copydoc WT_Class_Factory::Create_Polytriangle() 
    ///
    virtual WT_Polytriangle*            Create_Polytriangle() 
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Polytriangle(int count, WT_Logical_Point const *points, WT_Boolean copy)
    ///
    virtual WT_Polytriangle*            Create_Polytriangle(int count, WT_Logical_Point const *points, WT_Boolean copy)
        throw(DWFException); 

	///
	///\copydoc WT_Class_Factory::Create_Polytriangle(WT_Polytriangle const& polytriangle) 
	///
	virtual WT_Polytriangle*            Create_Polytriangle(WT_Polytriangle const& polytriangle) 
        throw(DWFException);

    //Projection
    ///
    ///\copydoc WT_Class_Factory::Create_Projection() 
    ///
    virtual WT_Projection*                Create_Projection() throw(DWFException);
     ///
    ///\copydoc WT_Class_Factory::Create_Projection(const WT_Projection::WT_Projection_Type projection_type) 
    ///
     virtual WT_Projection*                Create_Projection(const WT_Projection::WT_Projection_Type projection_type) 
        throw(DWFException);
 
	///
	///\copydoc WT_Class_Factory::Create_Projection(WT_Projection const& projection) 
	///
	 virtual WT_Projection*                Create_Projection(WT_Projection const& projection) 
        throw(DWFException);
    //Text
    ///
    ///\copydoc WT_Class_Factory::Create_Text() 
    ///
    virtual WT_Text*                    Create_Text() 
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Text(WT_Text const &) 
    ///
    virtual WT_Text*                    Create_Text(WT_Text const &) 
         throw(DWFException);
     ///
    ///\copydoc WT_Class_Factory::Create_Text(WT_Logical_Point const &position, 
    ///                                                WT_String const &string, 
    ///                                                WT_Logical_Point const *bbox, 
    ///                                                WT_Unsigned_Integer16 overscore_count, 
    ///                                                WT_Unsigned_Integer16 const *overscore_pos, 
    ///                                                WT_Unsigned_Integer16 underscore_count, 
    ///                                                WT_Unsigned_Integer16 const *underscore_pos)
    ///
    virtual WT_Text*                    Create_Text(WT_Logical_Point const &position, 
                                                    WT_String const &string, 
                                                    WT_Logical_Point const *bbox, 
                                                    WT_Unsigned_Integer16 overscore_count, 
                                                    WT_Unsigned_Integer16 const *overscore_pos, 
                                                    WT_Unsigned_Integer16 underscore_count, 
                                                    WT_Unsigned_Integer16 const *underscore_pos)
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Text(WT_Logical_Point const &position, WT_String const &string)  
    ///
    virtual WT_Text*                    Create_Text(WT_Logical_Point const &position, WT_String const &string) 
         throw(DWFException);

    //Text_Background
    ///
    ///\copydoc WT_Class_Factory::Create_Text_Background() 
    ///
    virtual WT_Text_Background*            Create_Text_Background() 
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Text_Background(WT_Text_Background::eBackground background,WT_Integer32 offset)
    ///
    virtual WT_Text_Background*            Create_Text_Background(WT_Text_Background::eBackground background,WT_Integer32 offset)
        throw(DWFException);

	///
	///\copydoc WT_Class_Factory::Create_Text_Background(WT_Text_Background const& background) 
	///
	virtual WT_Text_Background*            Create_Text_Background(WT_Text_Background const& background)  
        throw(DWFException);

    //Text_HAlign
    ///
    ///\copydoc WT_Class_Factory::Create_Text_HAlign() 
    ///
    virtual WT_Text_HAlign*                Create_Text_HAlign() 
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Text_HAlign(WT_Text_HAlign::WT_HAlign hAlign) 
    ///
    virtual WT_Text_HAlign*                Create_Text_HAlign(WT_Text_HAlign::WT_HAlign hAlign) 
        throw(DWFException);

	///
	///\copydoc WT_Class_Factory::Create_Text_HAlign(WT_Text_HAlign const& hAlign) 
	///
	virtual WT_Text_HAlign*                Create_Text_HAlign(WT_Text_HAlign const& hAlign) 
        throw(DWFException);

    //Text_VAlign
    ///
    ///\copydoc WT_Class_Factory::Create_Text_VAlign() 
    ///
    virtual WT_Text_VAlign*                Create_Text_VAlign() 
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Text_VAlign(WT_Text_VAlign::WT_VAlign vAlign) 
    ///
    virtual WT_Text_VAlign*                Create_Text_VAlign(WT_Text_VAlign::WT_VAlign vAlign) 
        throw(DWFException);

	///
	///\copydoc WT_Class_Factory::Create_Text_VAlign(WT_Text_VAlign const& vAlign) 
	///
	virtual WT_Text_VAlign*                Create_Text_VAlign(WT_Text_VAlign const& vAlign) 
        throw(DWFException);

    //WT_Units
    ///
    ///\copydoc WT_Class_Factory::Create_Units() 
    ///
    virtual WT_Units*                    Create_Units() 
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Units(const WT_Units& u) 
    ///
    virtual WT_Units*                    Create_Units(WT_Units const& u)
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Units(WT_Matrix const &xform, WT_String const &units) 
    ///
    virtual WT_Units*                    Create_Units(WT_Matrix const &xform, WT_String const &units) 
         throw(DWFException);

    //WT_URL
    ///
    ///\copydoc WT_Class_Factory::Create_URL() 
    ///
    virtual WT_URL*                        Create_URL() 
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_URL(WT_URL_Item &item) 
    ///
    virtual WT_URL*                        Create_URL(WT_URL_Item &item) 
         throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_URL(WT_Integer32 index, WT_Unsigned_Integer16 const *address, WT_Unsigned_Integer16 const *friendly_name)
    ///
    virtual WT_URL*                        Create_URL(WT_Integer32 index, WT_Unsigned_Integer16 const *address, WT_Unsigned_Integer16 const *friendly_name)
        throw(DWFException);
  
	///
	///\copydoc WT_Class_Factory::Create_URL(WT_URL const& url) 
	///
	 virtual WT_URL*                        Create_URL(WT_URL const& url) 
        throw(DWFException);

    //WT_User_Data
    ///
    ///\copydoc WT_Class_Factory::Create_UserData() 
    ///
    virtual WT_UserData*                Create_UserData() 
        throw(DWFException); 
    ///
    ///\copydoc WT_Class_Factory::Create_UserData(WT_String const &data_description)  
    ///
    virtual WT_UserData*                Create_UserData(WT_String const &data_description) 
         throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_UserData(WT_String const &data_description, WT_Integer32 data_size, 
    ///                                                    WT_Byte *data, WT_Boolean copy)  
    ///
    virtual WT_UserData*                Create_UserData(WT_String const &data_description, WT_Integer32 data_size, 
                                                        WT_Byte *data, WT_Boolean copy) 
        throw(DWFException);

    //User_Fill_Pattern
    ///
    ///\copydoc WT_Class_Factory::Create_User_Fill_Pattern()   
    ///
    virtual WT_User_Fill_Pattern*        Create_User_Fill_Pattern() 
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_User_Fill_Pattern(WT_Integer16 pattern_num)    
    ///
    virtual WT_User_Fill_Pattern*        Create_User_Fill_Pattern(WT_Integer16 pattern_num) 
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_User_Fill_Pattern(WT_Integer16 pattern_num, WT_Unsigned_Integer16 rows,WT_Unsigned_Integer16 columns,
    ///                                                          WT_Unsigned_Integer32 data_size,WT_Byte const *data)    
    ///
    virtual WT_User_Fill_Pattern*        Create_User_Fill_Pattern(WT_Integer16 pattern_num, WT_Unsigned_Integer16 rows,WT_Unsigned_Integer16 columns,
                                                              WT_Unsigned_Integer32 data_size,WT_Byte const *data)
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_User_Fill_Pattern(WT_Integer16 pattern_num,WT_User_Fill_Pattern::Fill_Pattern * fill_pattern)    
    ///
    virtual WT_User_Fill_Pattern*        Create_User_Fill_Pattern(WT_Integer16 pattern_num,WT_User_Fill_Pattern::Fill_Pattern * fill_pattern)
        throw(DWFException); 
    ///
    ///\copydoc WT_Class_Factory::Create_User_Fill_Pattern(WT_User_Fill_Pattern const &)
    ///
    virtual    WT_User_Fill_Pattern*        Create_User_Fill_Pattern(WT_User_Fill_Pattern const &)
        throw(DWFException);

    ///
    ///\copydoc WT_Class_Factory::Create_User_Hatch_Pattern()
    ///
    virtual WT_User_Hatch_Pattern*        Create_User_Hatch_Pattern()
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_User_Hatch_Pattern(WT_Integer16 hashpatnum) 
    ///
    virtual WT_User_Hatch_Pattern*        Create_User_Hatch_Pattern(WT_Integer16 hashpatnum) 
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_User_Hatch_Pattern(WT_Integer16 hashpatnum,WT_Unsigned_Integer16 xsize,WT_Unsigned_Integer16 ysize)
    ///
    virtual WT_User_Hatch_Pattern*        Create_User_Hatch_Pattern(WT_Integer16 hashpatnum,WT_Unsigned_Integer16 xsize,WT_Unsigned_Integer16 ysize)
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_User_Hatch_Pattern(WT_User_Hatch_Pattern const & pattern)  
    ///
    virtual WT_User_Hatch_Pattern*        Create_User_Hatch_Pattern(WT_User_Hatch_Pattern const & pattern) 
        throw(DWFException);

    //WT_View
    ///
    ///\copydoc WT_Class_Factory::Create_View() 
    ///
    virtual WT_View*                    Create_View() 
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_View(WT_Logical_Box view)  
    ///
    virtual WT_View*                    Create_View(WT_Logical_Box view) 
         throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_View(char *name) 
    ///
    virtual WT_View*                    Create_View(char *name) 
         throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_View(WT_View const &view) 
    ///
    virtual WT_View*                    Create_View(WT_View const &view) 
         throw(DWFException);

    //Viewport     
    ///
    ///\copydoc WT_Class_Factory::Create_Viewport() 
    ///
    virtual WT_Viewport*                Create_Viewport() 
        throw(DWFException);
     ///
    ///\copydoc WT_Class_Factory::Create_Viewport(WT_Viewport const &vport)  
    ///
    virtual WT_Viewport*                Create_Viewport(WT_Viewport const &vport) 
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Viewport(WT_File &file, char const *name, 
    ///                                                    int num_points, WT_Logical_Point const *points, 
    ///                                                    WT_Boolean copy=WD_True)  
    ///
    virtual WT_Viewport*                Create_Viewport(WT_File &file, char const *name, 
                                                        int num_points, WT_Logical_Point const *points, 
                                                        WT_Boolean copy=WD_True)
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Viewport(WT_File &file, WT_Unsigned_Integer16 const *name, 
    ///                                                    int num_points, WT_Logical_Point const *points, 
    ///                                                    WT_Boolean copy=WD_True)
    ///
    virtual WT_Viewport*                Create_Viewport(WT_File &file, WT_Unsigned_Integer16 const *name, 
                                                        int num_points, WT_Logical_Point const *points, 
                                                        WT_Boolean copy=WD_True) 
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Viewport(WT_File &file, WT_String const &name, 
    ///                                                    WT_Contour_Set const &boundary, WT_Boolean copy=WD_True) 
    ///
    virtual WT_Viewport*                Create_Viewport(WT_File &file, WT_String const &name, 
                                                        WT_Contour_Set const &boundary, WT_Boolean copy=WD_True)
        throw(DWFException);

    //Visibility
    ///
    ///\copydoc WT_Class_Factory::Create_Visibility() 
    ///
    virtual WT_Visibility*                Create_Visibility() 
        throw(DWFException);
    ///
    ///\copydoc WT_Class_Factory::Create_Visibility(WT_Boolean visible) 
    ///
    virtual WT_Visibility*                Create_Visibility(WT_Boolean visible) 
         throw(DWFException);

	///
	///\copydoc WT_Class_Factory::Create_Visibility(WT_Visibility const& visibility) 
	///
	virtual WT_Visibility*                Create_Visibility(WT_Visibility const& visibility) 
        throw(DWFException);

    ///
	///\copydoc WT_Class_Factory::Create_Unknown() 
	///
    virtual WT_Unknown*                   Create_Unknown() 
        throw(DWFException);


    //Destruction methods
    virtual void            Destroy(WT_Object*      pObject);
    virtual void            Destroy(WT_Attribute*   pAttribute);
    virtual void            Destroy(WT_File*        pFile);
    virtual void            Destroy(WT_Dash_Pattern* pDashPat);
    virtual void            Destroy(WT_Layer*       pLayer);
    virtual void            Destroy(WT_Named_View*  pNamedView);
    virtual void            Destroy(WT_Object_Node* pObjectNode);

};




#endif // XAML_CLASSFACTORY_HEADER
