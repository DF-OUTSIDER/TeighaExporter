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
///\file        dwf/whiptk/object.h
///

#if !defined OBJECT_HEADER
#define OBJECT_HEADER

#include "whiptk/whipcore.h"

//class WT_File;
class WT_Opcode;

/// Base class for all WHIP! objects and attributes that can be serialized to or materialized from a file.
class WHIPTK_API WT_Object
{
	friend class WT_Viewport;
	friend class WT_Object_Stream;
	friend class WT_File;
	friend class WT_Class_Factory;
	friend class WT_DeprecatedClassFactory;

public:
    /// Object ID.  Allows object class identification without having RTTI enabled.
    enum WT_ID
    {
         Unknown_ID
        ,Adapt_Line_Patterns_ID
        ,Author_ID
        ,Background_ID
        ,Code_Page_ID
        ,Color_ID
        ,Color_Map_ID
        ,Comments_ID
        ,Compressed_Data_Moniker_ID
        ,Contour_Set_ID
        ,Copyright_ID
        ,Creator_ID
        ,Creation_Time_ID
        ,Dash_Pattern_ID
        ,Description_ID
        ,Drawing_Info_ID
        ,DWF_Header_ID
        ,Embed_ID
        ,Embedded_Font_ID
        ,End_Of_DWF_ID
        ,Fill_ID
        ,Fill_Pattern_ID
        ,Filled_Ellipse_ID
        ,Gouraud_Polyline_ID
        ,Gouraud_Polytriangle_ID
        ,Image_ID
        ,Inked_Area_ID
        ,Keywords_ID
        ,Layer_ID
        ,Layer_List_ID
        ,Line_Caps_ID
        ,Line_Join_ID
        ,Line_Pattern_ID
        ,Line_Style_ID
        ,Line_Pattern_Scale_ID
        ,Line_Weight_ID
        ,Marker_Size_ID
        ,Marker_Symbol_ID
        ,Merge_Control_ID
        ,Miter_Angle_ID
        ,Miter_Length_ID
        ,Modification_Time_ID
        ,Named_View_ID
        ,Named_View_List_ID
        ,Trusted_Font_List_ID
        ,Object_Node_ID
        ,Object_Node_List_ID
        ,Option_ID
        ,Origin_ID
        ,Outline_Ellipse_ID
        ,Plot_Info_ID
        ,Polygon_ID
        ,Polyline_ID
        ,Polymarker_ID
        ,Polytriangle_ID
        ,Projection_ID
        ,Source_Creation_Time_ID
        ,Source_Filename_ID
        ,Source_Modification_Time_ID
        ,Subject_ID
        ,Text_ID
        ,Title_ID
        ,Units_ID
        ,URL_ID
        ,URL_List_ID
        ,View_ID
        ,Viewport_ID
        ,Viewport_Option_Viewport_Units_ID
        ,Visibility_ID
        ,Font_ID
        ,Font_Option_Font_Name_ID
        ,Font_Option_Charset_ID
        ,Font_Option_Pitch_ID
        ,Font_Option_Family_ID
        ,Font_Option_Style_ID
        ,Font_Option_Height_ID
        ,Font_Option_Rotation_ID
        ,Font_Option_Width_Scale_ID
        ,Font_Option_Spacing_ID
        ,Font_Option_Oblique_ID
        ,Font_Option_Flags_ID
        ,Text_Option_Overscore_ID
        ,Text_Option_Underscore_ID
        ,Text_Option_Bounds_ID
        ,Text_Option_Reserved_ID
        ,PNG_Group4_Image_ID
        ,Optimized_For_Plot_ID
        ,Group_Begin_ID
        ,Group_End_ID
        ,Block_Meaning_ID
        ,Encryption_ID
        ,Orientation_ID
        ,Alignment_ID
        ,Password_ID
        ,Guid_ID
        ,FileTime_ID
        ,BlockRef_ID
        ,Directory_ID
        ,UserData_ID
        ,Pen_Pattern_ID
        ,SignData_ID
        ,Guid_List_ID
        ,Font_Extension_ID
        ,PenPat_Options_ID
        ,Macro_Definition_ID
        ,Macro_Draw_ID
        ,Macro_Scale_ID
        ,Macro_Index_ID
        ,Text_Background_ID
        ,Text_HAlign_ID
        ,Text_VAlign_ID
        ,Overpost_ID
        ,Delineate_ID
        ,User_Fill_Pattern_ID
        ,User_Hatch_Pattern_ID
        ,Contrast_Color_ID
        ,Attribute_URL_ID
    };

    /// General type of an object.
    enum WT_Type
    {
         Unknown
        ,Attribute
        ,Drawable
        ,Definition
        ,Compression
        ,Comment
        ,Wrapper
        ,Option
        ,Informational
    };

protected:
    WT_Boolean    m_materialized; /**< \brief WD_True if the instance has been materialized from a WT_File. */

    /// Constructs a WT_Object object.
    WT_Object ()
        : m_materialized(WD_False)
    { }

    /// Destroys a WT_Object object.
    virtual ~WT_Object ()
    { }
public:
    /// Returns the WT_Type for this object.
    virtual WT_Type    object_type() const = 0;

    /// Returns the WT_ID for this object.
    virtual WT_ID      object_id() const = 0;

    /// Materializes the contents of the object from the file and the given opcode.
    /** Reads in the object data and causes the file reading to proceed to the end of this object.
     *  \warning This is used by the framework and should not be called by client code.
     *  \retval WT_Result::Success The operation was successful.
     *  \retval WT_Result::Opcode_Not_Valid_For_This_Object The object does not support the opcode type.
     *  \retval WT_Result::Internal_Error Something went very wrong.
     */
    virtual WT_Result        materialize(
        WT_Opcode const & opcode, /**< The opcode which corresponds to the object. */
        WT_File & file /**< The file being read. */
        ) = 0;

    /// Calls the configured action handler for this object (passes in the file reference.)
    /** \warning This is used by the framework and should not be called by client code.
     */
    virtual WT_Result        process(WT_File & file) = 0;

    /// Causes the serialization of the object to the file.
    /** If this is a WT_Drawable derived object, this method may cause the object to be
     *  delayed (in case a coincident like-object follows) and/or merged (in case a coincident
     *  like-object preceeded) so as to optimize the output.
     *
     *  \retval WT_Result::Success The operation was successful.
     */
    virtual WT_Result        serialize (
        WT_File & file /**< The file being written. */
        ) const = 0;

    /// Causes the file reading to proceed to the end of this object.
    /** \warning This is used by the framework and should not be called by client code.
     *  \bug Not all objects perform skip_operand() correctly; this is a known problem.
     *  For best results, if client code must use WT_File::get_next_object_shell() manually,
     *  it should also call the corresponding WT_Object::materialize() method to avoid this
     *  problem.
     *  \sa WT_File::get_next_object_shell(), WT_File_Heuristics::set_deferred_delete().
     */
    virtual WT_Result        skip_operand(WT_Opcode const & opcode, WT_File & file);

    /// Returns WD_True if the instance has been materialized from a WT_File. */
    const WT_Boolean& materialized() const
    {
        return m_materialized;
    }

    /// Returns WD_True if the instance has been materialized from a WT_File. */
    WT_Boolean& materialized()
    {
        return m_materialized;
    }

};

class WT_Optioncode;

/// A dependent sub-object that always gets seralized inside of a parent object.
class WHIPTK_API WT_Option : public WT_Object
{
	friend class WT_W2D_Class_Factory;
	friend class WT_Text_Option_Scoring;
	friend class WT_Text_Option_Bounds;
	friend class WT_Text_Option_Reserved;

public:

    /// Materialization stage.
    enum
    {
        Getting_Operand,
        Getting_Close_Paren,
        Completed
    };

    /// \internal We don't use this directly as an enum since child object commandeer and overload the value.
    int m_stage; /**< \brief The current materialization stage. */

protected:
    /// Constructs a WT_Option object.
    WT_Option()
        : m_stage (Getting_Operand)
    { }

public:
	virtual ~WT_Option()
	{}

	/// Returns the WT_ID for this object.
    virtual WT_ID      object_id() const { return Option_ID; }

    /// Returns the object type WT_Type::Option.
    WT_Type    object_type() const
    {
        return Option;
    }

    /// Provided to implement WT_Object, but <b>not used</b> (returns WT_Result::Toolkit_Usage_Error).
    virtual WT_Result    materialize(WT_Opcode const & /* opcode */, WT_File & /* file */) {
        return WT_Result::Toolkit_Usage_Error;
    }

    /// Materializes the dependent WT_Option of the \a parent object.
    virtual WT_Result    materialize (
        WT_Object & /*parent*/, /**< The parent object for this dependent option. */
        WT_Optioncode const & /*optioncode*/, /**< A derivative of WT_Opcode which corresponds to this WT_Option. */
        WT_File & /*file*/ /**< The file being read. */
        ) { return WT_Result::Toolkit_Usage_Error; }

    /// Calls the configured action handler for this object (passes in the file reference.)
    /** \warning This is used by the framework and should not be called by client code.
     */
    virtual WT_Result    process(WT_File & file);

    /// Provided to implement WT_Object, but <b>not used</b> (returns WT_Result::Toolkit_Usage_Error).
    virtual WT_Result    serialize (WT_File & /* file */) const {
        return WT_Result::Toolkit_Usage_Error;
    }

    /// Serializes this dependent WT_Option of the \a parent object.
    virtual WT_Result    serialize   (
        WT_Object const & /*parent*/, /**< The parent object for this dependent option. */
        WT_File & /*file*/ /**< The file being written. */
        ) const { return WT_Result::Toolkit_Usage_Error; }

    /// Causes the file reading to proceed to the end of this option.
    /** \warning This is used by the framework and should not be called by client code.
     */
    virtual WT_Result    skip_operand(WT_Optioncode const & optioncode, WT_File & file);
};

#endif // OBJECT_HEADER
