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

///
///\file        dwf/whiptk/pattern_scale.h
///

#if !defined PATTERN_SCALE_HEADER
#define PATTERN_SCALE_HEADER

#include "whiptk/whipcore.h"

/// Used by WT_Fill_Pattern and WT_Line_Pattern as an internal WT_Option defining the scale of the respective pattern.
/** Specifies the scale factor (muliplier) for the pattern.  There are three different types of
 *  pattern scales:
 *  \li Values greater than 0.0 indicate the pattern scales with geometry (as you zoom in the
 *  pattern spreads out). The value defines the number of drawing units over which one repetition
 *  of the line pattern will occur. Thus if a pattern scale of 50.0 is applied to a line 100
 *  logical units long, then the line pattern will repeat two times over the length of the line.
 *  \li When the pattern scale is 0.0 (the default), this means that a rendering application should
 *  default to an aesthetically reasonable device-relative scale value.
 *  \li Values less than 0.0 mean that the provided scale is to be measured in device coordinates
 *  (and thus doesn't change with a viewer's zoom factor). The value defines the number of device
 *  pixels (not drawing units) over which the pattern repeats.
 */
class WHIPTK_API WT_Pattern_Scale : public WT_Option
{
    friend class WT_Fill_Pattern;
    friend class WT_Line_Pattern;
	friend class WT_Font;
	friend class WT_Line_Style;
	friend class WT_User_Fill_Pattern;
	friend class WT_W2D_Class_Factory;

protected:
    //@{ \name Constructor
    /// Constructs a WT_Pattern_Scale object.
    WT_Pattern_Scale()
    : m_stage( Eating_Initial_Whitespace )
    , m_value( ( double ) 0.0 )
    {}
	virtual ~WT_Pattern_Scale()
	{}
    //@}
public:
    //@{ \name Data access methods
    /// Returns the scale value.
    inline operator double () const { return m_value; }
    /// Assignment operator.  Sets the scale value.
    inline WT_Pattern_Scale & operator= ( double i ) { m_value = i; return *this; }
    //@}

    //@{ \name Inspection methods
    /// Returns WD_True if the given scale is equal to this one.
    inline WT_Boolean operator== ( WT_Pattern_Scale const & scale ) const { return m_value == scale.m_value; }
    /// Returns WD_True if the given scale value is equal to this one.
    inline WT_Boolean operator== ( double const & value ) const { return m_value == value; }
    /// Returns WD_True if the given scale is NOT equal to this one.
    inline WT_Boolean operator!= ( WT_Pattern_Scale const & scale ) const { return m_value != scale.m_value; }
    /// Returns WD_True if the given scale value is NOT equal to this one.
    inline WT_Boolean operator!= ( double const & value ) const { return m_value != value; }
    //@}

    //@{ \name WT_Option virtual methods
    WT_Result serialize( WT_Object const & parent, WT_File & file ) const;
    WT_Result materialize( WT_Object& parent , WT_Optioncode const & optioncode, WT_File& file);
    //@}

private:
    WT_ID     object_id() const { return Option_ID; }
    enum WT_Materialize_Stage
    {   Eating_Initial_Whitespace,
        Getting_Value,
        Eating_End_Whitespace
    } m_stage;
    double m_value;
};

#endif // PATTERN_SCALE_HEADER
