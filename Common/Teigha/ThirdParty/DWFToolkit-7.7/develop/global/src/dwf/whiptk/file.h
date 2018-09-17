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

#if !defined FILE_HEADER
#define FILE_HEADER

///
///\file        dwf/whiptk/file.h
///

#include "whiptk/allclass.h"
#include "whiptk/wversion.h"
#include "whiptk/whipcore.h"
#include "whiptk/rendition.h"
#include "whiptk/opcode.h"
#include "whiptk/drawable.h"
#include "whiptk/fifo.h"
#include "whiptk/heuristics.h"
#include "whiptk/layer_list.h"
#include "whiptk/dpat_list.h"
#include "whiptk/object_node_list.h"
#include "whiptk/group_begin.h"
#include "whiptk/group_end.h"
#include "whiptk/blockref_defs.h"
#include "whiptk/filetime.h"
#include "whiptk/inked_area.h"
#include "whiptk/directory.h"
#include "whiptk/penpat_options.h"

#define CALLBACK_LIST\
    CALLBACK_MACRO(Alignment,alignment)\
    CALLBACK_MACRO(Attribute_URL,attribute_url)\
    CALLBACK_MACRO(Author,author)\
    CALLBACK_MACRO(Background,background)\
    CALLBACK_MACRO(Block_Meaning,block_meaning)\
    CALLBACK_MACRO(BlockRef,blockref)\
    CALLBACK_MACRO(Code_Page,code_page)\
    CALLBACK_MACRO(Color,color)\
    CALLBACK_MACRO(Color_Map,color_map)\
    CALLBACK_MACRO(Comments,comments)\
    CALLBACK_MACRO(Compressed_Data_Moniker,compressed_data_moniker)\
    CALLBACK_MACRO(Contour_Set,contour_set)\
    CALLBACK_MACRO(Contrast_Color,contrast_color)\
    CALLBACK_MACRO(Copyright,copyright)\
    CALLBACK_MACRO(Creation_Time,creation_time)\
    CALLBACK_MACRO(Creator,creator)\
    CALLBACK_MACRO(Dash_Pattern,dash_pattern)\
    CALLBACK_MACRO(Delineate,delineate)\
    CALLBACK_MACRO(Description,description)\
    CALLBACK_MACRO(Directory,directory)\
    CALLBACK_MACRO(Drawing_Info,drawing_info)\
    CALLBACK_MACRO(DWF_Header,dwf_header)\
    CALLBACK_MACRO(Embed,embed)\
    CALLBACK_MACRO(Embedded_Font,embedded_font)\
    CALLBACK_MACRO(Encryption,encryption)\
    CALLBACK_MACRO(End_Of_DWF,end_of_dwf)\
    CALLBACK_MACRO(FileTime,filetime)\
    CALLBACK_MACRO(Fill,fill)\
    CALLBACK_MACRO(Fill_Pattern,fill_pattern)\
    CALLBACK_MACRO(Filled_Ellipse,filled_ellipse)\
    CALLBACK_MACRO(Font,font)\
    CALLBACK_MACRO(Font_Extension,font_extension)\
    CALLBACK_MACRO(Gouraud_Polyline,gouraud_polyline)\
    CALLBACK_MACRO(Gouraud_Polytriangle,gouraud_polytriangle)\
    CALLBACK_MACRO(Group_Begin,group_begin)\
    CALLBACK_MACRO(Group_End,group_end)\
    CALLBACK_MACRO(Guid,guid)\
    CALLBACK_MACRO(Guid_List,guid_list)\
    CALLBACK_MACRO(Image,image)\
    CALLBACK_MACRO(Inked_Area,inked_area)\
    CALLBACK_MACRO(Keywords,keywords)\
    CALLBACK_MACRO(Layer,layer)\
    CALLBACK_MACRO(Line_Pattern,line_pattern)\
    CALLBACK_MACRO(Line_Style,line_style)\
    CALLBACK_MACRO(Line_Weight,line_weight)\
    CALLBACK_MACRO(Macro_Definition,macro_definition)\
    CALLBACK_MACRO(Macro_Draw,macro_draw)\
    CALLBACK_MACRO(Macro_Scale,macro_scale)\
    CALLBACK_MACRO(Macro_Index,macro_index)\
    CALLBACK_MACRO(Marker_Size,marker_size)\
    CALLBACK_MACRO(Marker_Symbol,marker_symbol)\
    CALLBACK_MACRO(Merge_Control,merge_control)\
    CALLBACK_MACRO(Modification_Time,modification_time)\
    CALLBACK_MACRO(Named_View,named_view)\
    CALLBACK_MACRO(Named_View_List,named_view_list)\
    CALLBACK_MACRO(Object_Node,object_node)\
    CALLBACK_MACRO(Orientation,orientation)\
    CALLBACK_MACRO(Origin,origin)\
    CALLBACK_MACRO(Outline_Ellipse,outline_ellipse)\
    CALLBACK_MACRO(Overpost,overpost)\
    CALLBACK_MACRO(Password,password)\
    CALLBACK_MACRO(Pen_Pattern,pen_pattern)\
    CALLBACK_MACRO(PenPat_Options,penpat_options)\
    CALLBACK_MACRO(Plot_Info,plot_info)\
    CALLBACK_MACRO(Plot_Optimized,plot_optimized)\
    CALLBACK_MACRO(PNG_Group4_Image,png_group4_image)\
    CALLBACK_MACRO(Polygon,polygon)\
    CALLBACK_MACRO(Polyline,polyline)\
    CALLBACK_MACRO(Polymarker,polymarker)\
    CALLBACK_MACRO(Polytriangle,polytriangle)\
    CALLBACK_MACRO(Projection,projection)\
    CALLBACK_MACRO(SignData,signdata)\
    CALLBACK_MACRO(Source_Creation_Time,source_creation_time)\
    CALLBACK_MACRO(Source_Filename,source_filename)\
    CALLBACK_MACRO(Source_Modification_Time,source_modification_time)\
    CALLBACK_MACRO(Subject,subject)\
    CALLBACK_MACRO(Text,text)\
    CALLBACK_MACRO(Text_Background,text_background)\
    CALLBACK_MACRO(Text_HAlign,text_halign)\
    CALLBACK_MACRO(Text_VAlign,text_valign)\
    CALLBACK_MACRO(Title,title)\
    CALLBACK_MACRO(Trusted_Font_List,trusted_font_list)\
    CALLBACK_MACRO(Units,units)\
    CALLBACK_MACRO(Unknown,unknown)\
    CALLBACK_MACRO(URL,url)\
    CALLBACK_MACRO(UserData,userdata)\
    CALLBACK_MACRO(User_Fill_Pattern,user_fill_pattern)\
    CALLBACK_MACRO(User_Hatch_Pattern,user_hatch_pattern)\
    CALLBACK_MACRO(View,view)\
    CALLBACK_MACRO(Viewport,viewport)\
    CALLBACK_MACRO(Visibility,visibility)

class WT_File_Stats;
class WT_File;

typedef WT_Result (* WT_Stream_Open_Action) (WT_File & file);
typedef WT_Result (* WT_Stream_Close_Action)(WT_File & file);
typedef WT_Result (* WT_Stream_Read_Action) (WT_File & file,int bytes_desired,int & bytes_read,void * buffer);
typedef WT_Result (* WT_Stream_Write_Action)(WT_File & file,int size,void const * buffer);
typedef WT_Result (* WT_Stream_Seek_Action)(WT_File & file, int distance, int & amount_seeked);
typedef WT_Result (* WT_Stream_End_Seek_Action)(WT_File & file);
typedef WT_Result (* WT_Stream_Tell_Action)(WT_File & file, unsigned long *current_file_pointer_position);

// public typedefs
#define CALLBACK_MACRO(class_name, class_lower) \
    typedef WT_Result (* WT_##class_name##_Action)(WT_##class_name & class_lower, WT_File & file);
CALLBACK_LIST
#undef CALLBACK_MACRO

/** \addtogroup groupFileIO File I/O objects
 *  @{
 *  @}
 */

/** \addtogroup groupFileIO File I/O objects
 *  @{
 */

/// Coordinates the reading and writing of WHIP! data and classic %DWF files.
/** Opening a file for writing:
 *
 *  Opening a file for reading:
 */
class WHIPTK_API WT_File
{
	friend class WT_W2D_Class_Factory;
    friend class WT_Class_Factory;
	friend class WT_Macro_Definition;

public:

    /// The mode used for reading or writing.
    enum WT_File_mode
    {
        File_Inactive, /**< \brief Default mode - nothing can be done with the file in this mode. */
        File_Read, /**< \brief Open file for writing. */
        File_Write, /**< \brief Open file for writing. */
        Block_Write, /**< \brief \deprecated Block writing is only available for classic %DWF files of version 00.55. */
        Block_Read, /**< \brief \deprecated Block reading is only available for classic %DWF files of version 00.55. */
        Block_Append /**< \brief \deprecated Block appending is only available for classic %DWF files of version 00.55. */
    };
public:
    //@{ \name Construction / destruction.
    /// Constructs a WT_File object.
    WT_File() throw(WT_Result);

    /// Destroys a WT_File object.
    virtual ~WT_File();
    //@}
public:
    //@{ \name I/O startup / shutdown / status methods
    /// Close the file.
    virtual WT_Result            close();
    /// Returns the file mode with which the file was opened (one of \ref WT_File_mode "enum WT_File_mode".)
    WT_File_mode                 file_mode() const;
    /// Returns the filename that was opened.
    WT_String const &            filename() const;
    /// Returns an accessor to the WT_File_Heuristics object for this file.
    WT_File_Heuristics &         heuristics()    { return m_heuristics;              }
    /// Open the file based on the filename, file mode, and heuristics, which should be configured prior to calling open().
    /** \retval WT_Result::Success The operation was successful.
     *  \retval WT_Result::Out_Of_Memory_Error An internal object could not be created.
     */
    virtual WT_Result             open();
    /// Sets the file mode to be used for opening the file.
    void                         set_file_mode(WT_File_mode mode);
    /// Sets the filename which is to be opened.
    void                         set_filename(char const * name);
    /// Sets the filename which is to be opened.
    void                         set_filename(int length, WT_Unsigned_Integer16 const * name);
    /// Sets the filename which is to be opened.
    void                         set_filename(WT_Unsigned_Integer16 const * name);
    /// Returns the decimal revision of the toolkit.
    /** The version is calculated as follows: (major_version * 100) + minor_version.
     *  For example, if the toolkit is version 00.55, the decimal revision is 55;
     *  for version 06.00, the decimal revision is 600.
     */
    int                          toolkit_decimal_revision() const  {    return (toolkit_major_revision() * 100) + toolkit_minor_revision();    }
    /// Returns the major revision of the toolkit.
    /** For example, if the toolkit is version 06.00, the major revision is 6.
     */
    virtual int                  toolkit_major_revision() const    {    return WD_Toolkit_Major_Revision;    }
    /// Returns the minor revision of the toolkit.
    /** For example, if the toolkit is version 06.00, the minor revision is 0.
     */
    virtual int                  toolkit_minor_revision() const    {    return WD_Toolkit_Minor_Revision;    }
    //@}

    //@{ \name Data access methods
    /// Used when a file is opened for reading to return a read-only accessor to the current object set in get_next_object() -\> get_next_object_shell()
    virtual WT_Object const *            current_object() const    { return m_current_object;}
	virtual WT_Logical_Point			 current_point() { return m_current_point; }
	virtual void						 set_current_object( WT_Object* pObj) { m_current_object = pObj; }
    /// Returns a writable reference to the local WT_Dash_Pattern_List object for this file (contains the set of WT_Dash_Pattern objects for this file.)
    virtual WT_Dash_Pattern_List &       dash_pattern_list() { return m_dpat_list; }
    /// Returns the write-mode optimizing WT_Rendition object for this file.
    /** \warning Clients should use this when setting rendition attributes and should
     *  \b never use this during read operations (use rendition() when reading.)
     *  \sa \ref groupRenditionAttributes "Rendition attributes"
     */
    virtual WT_Rendition &               desired_rendition()    {return m_desired_rendition;}
    /// Returns a writable reference to the local WT_Layer_List object for this file (contains the set of WT_Layer objects for this file.)
    virtual WT_Layer_List &              layer_list()   { return m_layer_list; }
    /// Returns the read-mode current WT_Rendition object for this file.
    /** \warning Clients should use this during read operations when reading the current \ref groupRenditionAttributes "rendition attributes",
     *  but should \b never use this during write operations (use desired_rendition() when writing.)
     */
    virtual WT_Rendition &               rendition()    {        return *m_rendition;    }
    /// Used when a file is opened for reading to return an accessor to the current object set in get_next_object() -\> get_next_object_shell()
    virtual WT_Object *                  object() { return m_current_object; }
    /// Returns a writable reference to the local WT_Object_Node_List object for this file (contains the set of WT_Object_Node objects for this file.)
    virtual WT_Object_Node_List &        object_node_list() { return m_object_node_list; }
    /// Sets a pointer to store a user-defined data item.
    /** \warning When the default stream action handlers are used, this is used internally to
     *  store the FILE pointer used to read/write the file.  Users wishing to store a custom
     *  pointer and still use the default stream action handlers can use the WT_Heuristics::set_user_data() method.
     */
    virtual void                         set_stream_user_data(void * stream_user_data) {    m_stream_user_data = stream_user_data;    }
    /// Retrieves the pointer to a user-defined data item.
    virtual void *                       stream_user_data() {    return m_stream_user_data;    }
    //@}

    //@{ \name Reader state machine methods
    /// Reads the next object from the file stream (but does not process it.)
    /** This calls get_next_object_shell() then WT_Object::materialize() to fully
     *  create and populate the object contents from the file.  The file pointer is
     *  moved to the next object/opcode.
     *
     *  \retval WT_Result::Success The operation was successful.
     */
    virtual WT_Result                    get_next_object();
    /// Reads the next object opcode and determines the object type, creates an empty (default) object.
    /** This method does not move the file pointer beyond the opcode of the object.  If the object
     *  is not materialized through a call to the WT_Object::materialize() method (typically by the
     *  framework,) then upon retriving the \b next object shell, the current object is skipped through
     *  a call to WT_Object::skip_operand().
     *  \warning Dangerous when used by client code and especially with deferred delete.
     *  \sa WT_Object::skip_operand(), WT_File_Heuristics::set_deferred_delete().
     */
    virtual WT_Result                    get_next_object_shell();
    /// Processes the next object.
    /** This calls get_next_object(), and then calls the object's processing handler if configured
     *  or the object's default_process() method.
     *
     *  \retval WT_Result::Success The operation was successful.
     */
    virtual WT_Result                    process_next_object();
    //@}

    /** \name Object processing handler set methods
     *  Sets read-mode object processing handlers.  If set, these are called when
     *  process_next_object() is called.  Otherwise, the default_process() method
     *  of each object is called.
     *  @{
     */
    #define CALLBACK_MACRO(class_name, class_lower) \
        void set_##class_lower##_action(WT_##class_name##_Action action) \
                    {m_##class_lower##_action = action;}
    CALLBACK_LIST
    #undef CALLBACK_MACRO
    //@}

    /** \name Object processing handler get methods
     *  Gets the read-mode object processing handlers.  If set, these are called when
     *  process_next_object() is called.  Otherwise, the default_process() method
     *  of each object is called.
     *  @{
     */
    #define CALLBACK_MACRO(class_name, class_lower) \
        WT_##class_name##_Action class_lower##_action() \
            {return m_##class_lower##_action;}
    CALLBACK_LIST
    #undef CALLBACK_MACRO
    //@}

    /** \name I/O customization set/get methods
     *  These methods allow client code to override how low-level file I/O works
     *  by allowing client code to selectively implement various I/O procedures.
     *  @{
     */
    void                         set_stream_close_action(WT_Stream_Close_Action action) {    m_stream_close_action = action;    }
    void                         set_stream_end_seek_action(WT_Stream_End_Seek_Action action) {    m_stream_end_seek_action = action;    }
    void                         set_stream_open_action(WT_Stream_Open_Action action) {    m_stream_open_action = action;    }
    void                         set_stream_read_action(WT_Stream_Read_Action action) {    m_stream_read_action = action;    }
    void                         set_stream_seek_action(WT_Stream_Seek_Action action) {    m_stream_seek_action = action;    }
    void                         set_stream_tell_action(WT_Stream_Tell_Action action) {    m_stream_tell_action = action;    }
    void                         set_stream_write_action(WT_Stream_Write_Action action) {    m_stream_write_action = action;    }
    WT_Stream_Close_Action       stream_close_action() {    return m_stream_close_action;    }
    WT_Stream_End_Seek_Action    stream_end_seek_action() {    return m_stream_end_seek_action;    }
    WT_Stream_Open_Action        stream_open_action() {    return m_stream_open_action;    }
    WT_Stream_Read_Action        stream_read_action() {    return m_stream_read_action;    }
    WT_Stream_Seek_Action        stream_seek_action() {    return m_stream_seek_action;    }
    WT_Stream_Tell_Action        stream_tell_action(){    return m_stream_tell_action;    }
    WT_Stream_Write_Action       stream_write_action() {    return m_stream_write_action;    }
    //@}

    /** \name BlockRef and Directory methods
     *  \deprecated These methods are available only for backward compatibility to allow client code
     *  to continue to write %DWF 00.55 files which contain WT_BlockRef objects.
     *  @{
     */
    /// Adds a new WT_BlockRef object to the local WT_Directory.
    WT_Result                    add_blockref(WT_BlockRef & blockref) { return m_directory.add(blockref); }
    /// Sets the \a bBinaryFileType to WD_True if the file is binary and sets \a end_byte_length to the length of the last opcode (EndOfDWF)
    /** If the file mode is opened as Block_Append or Block_Read, this method will help in
     *  determining the file type (whether the file is in ascii or binary format).
     */
    WT_Result                    is_file_type_binary(WT_Boolean & bBinaryFileType, int & end_byte_length);
    /// Returns the local WT_Directory object for this file (contains the set of WT_BlockRef objects for this file.)
    WT_Directory const           get_directory() { return m_directory; }
    /// Moves the file pointer to the beginning of the directory for this file.
    /** Available when a file is opened as Block_Append or Block_Read
     */
    WT_Result                    get_to_the_directory();
    //@}

    // We don't want to document the following methods.  They're for internal use only,
    // but public so that we are not force to make every object a friend.
#ifndef DOXYGEN_IGNORE

    // Internal Reader state machine stuff that's just plain dangerous to use.
    WT_Result                    get_next_object(WT_File *p_uncompressed_file);
    WT_Result                    process_next_object(WT_File *p_uncompressed_file);

    // Internal reader / writer functions only used by WT_Object derivatives, not by client code.
    virtual WT_Result            compress_write(int count, void const * data);
    const WT_Opcode &            current_opcode() const { return m_opcode; }
    int                          data_decompression() {    return m_data_decompression;    }
    WT_Result                    decompress_read(int count, int & num_read, void * data);
    WT_Result                    decompress_seek(int count, int & num_seeked);
    int                          decrement_paren_count()    {   return --m_paren_count;}
    int                          decrement_tab_level()    {   return --m_tab_level;    }
    virtual WT_Logical_Point     de_update_current_point(WT_Logical_Point const & delta);
    WT_Drawable *                delayed_drawable() const { return m_delayed_drawable; }
    virtual WT_Result            dump_delayed_drawable();
    WT_Result                    eat_whitespace();
    WT_File_Stats *              file_stats() { return m_file_stats; }
    WT_Result                    handle_leftover_decompressor_stream_read_data(int avail_in, WT_Byte const * next_in);
    WT_Boolean                   have_read_first_opcode() const    { return m_have_read_first_opcode;    }
    int                          increment_paren_count()    { return ++m_paren_count; }
    int                          increment_tab_level()    {   return ++m_tab_level;    }
    WT_Result                    merge_or_delay_drawable(WT_Drawable const & current);
    int                          next_incarnation()    {        return m_next_incarnation++;    }
    int                          paren_count() const    {   return m_paren_count;    }
    WT_Result                    put_back(int size, WT_Byte const * buffer);
    WT_Result                    put_back(WT_Byte const & a_byte)  { return put_back(sizeof(WT_Byte), & a_byte);    }
    WT_Result                    read(char * & string, int max_size, WT_Boolean allow_initial_open_paren = WD_False); // Too big to
    WT_Result                    read(int count, WT_Logical_Point    * points);
    WT_Result                    read(int count, WT_Logical_Point_16 * points);
    WT_Result                    read(int count, WT_RGBA32 * colors);
    WT_Result                    read(int count, WT_Unsigned_Integer16 * buffer);
    WT_Result                    read(int size, char * buffer);
    WT_Result                    read(int size, WT_Byte * buffer);
    WT_Result                    read(WT_Byte & a_byte);
    WT_Result                    read(WT_Integer16 & a_word);
    WT_Result                    read(WT_Integer32 & a_word);
    WT_Result                    read(WT_RGBA32 & a_color);
    WT_Result                    read(WT_Unsigned_Integer16 & a_word);
    WT_Result                    read(WT_Unsigned_Integer32 & a_word);
    WT_Result                    read_ascii(double & a_word);
    WT_Result                    read_ascii(int count, WT_RGBA32 * colors);
    WT_Result                    read_ascii(WT_Color & a_color, WT_Boolean allow_indexed_color = WD_True);
    WT_Result                    read_ascii(WT_Integer16 & a_word);
    WT_Result                    read_ascii(WT_Integer32 & a_word);
    WT_Result                    read_ascii(WT_Logical_Box & a_box);
    WT_Result                    read_ascii(WT_Logical_Point & a_point);
    WT_Result                    read_ascii(WT_RGBA32 & a_color);
    WT_Result                    read_ascii(WT_Unsigned_Integer16 & a_word);
    WT_Result                    read_ascii(WT_Unsigned_Integer32 & a_dword);
    WT_Result                    read_block(WT_File & block_file, WT_BlockRef & blockref);
    WT_Result                    read_count (WT_Integer32 & count);
    WT_Result                    read_hex(int count, WT_Byte * data);
    WT_Result                    read_hex(WT_Byte & byte);
    WT_Result                    read_index_tree(int max_size, WT_Boolean allow_initial_open_paren = WD_False); // Too big to
    WT_Result                    set_block_size_for_tail_blockref(WT_Unsigned_Integer32 current_blockref_file_offset);
    void                         set_data_decompression(int decompression) {    m_data_decompression = decompression;    }
    void                         set_delayed_drawable(WT_Drawable * drwbl) {   WD_Assert(!m_delayed_drawable); m_delayed_drawable = drwbl; }
    void                         set_directory(WT_Directory directory) { m_directory = directory; } /* in the default_process of WT_Directory, we set the file directory member value to the directory that we might have materialized from the file opened in Block_Append, Block_Read or File_Read mode. */
    void                         set_have_read_first_opcode(WT_Boolean state)    { m_have_read_first_opcode = state;    }
    void                         set_paren_count(int count)    {   m_paren_count = count;    }
    void                         set_tab_level(int count)    {        m_tab_level = count;    }
    WT_Result                    skip(int size);
    WT_Unsigned_Integer16        skip_byte_length() const { return m_skip_byte_length; }
    WT_Result                    skip_past_matching_paren(int terminating_paren_level);
    int                          tab_level() const    {        return m_tab_level;    }
    int                          this_opcode_start_position() const { return m_this_opcode_start_position; }
    virtual WT_Logical_Point     update_current_point(WT_Logical_Point const & new_point);
    WT_Result                    write(char const * str);
    WT_Result                    write(int count, WT_Byte const * data);
    WT_Result                    write(int count, WT_Logical_Point    const * points);
    WT_Result                    write(int count, WT_Logical_Point_16 const * points);
    WT_Result                    write(int count, WT_Unsigned_Integer16 const * data);
    WT_Result                    write(WT_Byte a_byte);
    WT_Result                    write(WT_Integer16 a_word);
    WT_Result                    write(WT_Integer32 a_word);
    WT_Result                    write(WT_RGBA32 a_color);
    WT_Result                    write(WT_Unsigned_Integer16 a_word);
    WT_Result                    write(WT_Unsigned_Integer32 a_dword);
    WT_Result                    write_ascii(double val);
    WT_Result                    write_ascii(int count, WT_Logical_Point const * points);
    WT_Result                    write_ascii(WT_Integer16 a_word);
    WT_Result                    write_ascii(WT_Integer32 a_word);
    WT_Result                    write_ascii(WT_Logical_Point const & point);
    WT_Result                    write_ascii(WT_RGBA32 a_color);
    WT_Result                    write_ascii(WT_Unsigned_Integer16 a_word);
    WT_Result                    write_ascii(WT_Unsigned_Integer32 a_dword);
    WT_Result                    write_block(WT_File & file);
    WT_Result                    write_count(int count);
    WT_Result                    write_geom_tab_level();
    WT_Result                    write_hex(int count, WT_Byte const * data);
    WT_Result                    write_hex(WT_Byte data);
    WT_Result                    write_padded_ascii(double a_double);
    WT_Result                    write_padded_ascii(int count, WT_Logical_Point const * points);
    WT_Result                    write_padded_ascii(WT_Integer16 a_word);
    WT_Result                    write_padded_ascii(WT_Integer32 a_word);
    WT_Result                    write_padded_ascii(WT_Logical_Point const & point);
    WT_Result                    write_padded_ascii(WT_Unsigned_Integer16 a_word);
    WT_Result                    write_padded_ascii(WT_Unsigned_Integer32 a_word);
    WT_Result                    write_quoted_string(char const * str, WT_Boolean force_quotes = WD_False);
    WT_Result                    write_quoted_string(WT_Unsigned_Integer16 const * str);
    WT_Result                    write_tab_level();

	void						 set_write_ascii_format(char const *strFormat);
#endif

protected:
    /// Holds a pointer to a user-defined data item.
    void *                      m_stream_user_data;

    /** \name Reader processing handler properties
     *  If set, these are called when process_next_object() is called.
     *  Otherwise, the default_process() method of each object is called.
     *  @{
     */
    #define CALLBACK_MACRO(class_name, class_lower) \
        WT_##class_name##_Action    m_##class_lower##_action;
    CALLBACK_LIST
    #undef CALLBACK_MACRO
    //@}

    //@{ \name I/O customization propreties
    WT_Stream_End_Seek_Action   m_stored_stream_end_seek_action;
    WT_Stream_Read_Action       m_stored_stream_read_action;
    WT_Stream_Seek_Action       m_stored_stream_seek_action;
    WT_Stream_Close_Action      m_stream_close_action;
    WT_Stream_End_Seek_Action   m_stream_end_seek_action;
    WT_Stream_Open_Action       m_stream_open_action;
    WT_Stream_Read_Action       m_stream_read_action;
    WT_Stream_Seek_Action       m_stream_seek_action;
    WT_Stream_Tell_Action       m_stream_tell_action;
    WT_Stream_Write_Action      m_stream_write_action;
    //@}

    // Default I/O and decompression methods, document readers don't need to know about these
#ifndef DOXYGEN_IGNORE
    static WT_Result default_close (WT_File & file);
    static WT_Result default_end_seek (WT_File & file);
    static WT_Result default_open (WT_File & file);
    static WT_Result default_read (WT_File & file,    int desired_bytes, int & bytes_read, void * buffer);
    static WT_Result default_seek (WT_File & file, int distance, int & amount_seeked);
    static WT_Result default_tell (WT_File & file, unsigned long *current_file_pointer_position);
    static WT_Result default_write(WT_File & file, int size, void const * buffer);
#endif

private:

    WT_String           m_filename;
    WT_File_mode        m_mode;
    WT_File_Heuristics  m_heuristics;

    WT_Rendition        m_desired_rendition;
    WT_Rendition *      m_rendition;


    WT_Opcode            m_opcode;
    WT_Object *          m_current_object;
    WT_Layer_List        m_layer_list;
    WT_Object_Node_List  m_object_node_list;
    WT_Dash_Pattern_List m_dpat_list;


    WT_Boolean          m_have_read_first_opcode;
    WT_Logical_Point    m_current_point;
    int                 m_next_incarnation;
    int                 m_actual_file_position;
    int                 m_paren_count;
    int                 m_tab_level;
    WT_FIFO<WT_Byte>    m_read_fifo;

    WT_Byte             m_quote;
    WT_Boolean          m_processing_quoted_string;
    WT_Boolean          m_processing_escaped_literal;
    WT_FIFO<WT_Byte>    m_string_accumulator;

    WT_Compressor *     m_compressor;
    WT_DeCompressor *   m_decompressor;
    int                 m_data_decompression;
    WT_FIFO<WT_Byte>    m_decomp_leftover_data_buffer;

    WT_Drawable *       m_delayed_drawable;
    WT_Boolean          m_currently_dumping_delayed_drawable;

    int                 m_current_file_position;
    int                 m_this_opcode_start_position;
    int                 m_number_of_opcodes_read;
    WT_File_Stats *     m_file_stats;

    // Persistant scratch values in case we run out of data
    WT_Integer32        m_read_colors_state;
    WT_Integer32        m_read_ascii_value;
    int                 m_read_ascii_sign;
    double              m_read_ascii_double_value;
    double              m_read_ascii_double_sign;
    double              m_read_ascii_double_divisor;
    WT_Logical_Point    m_read_ascii_logical_point_value;
    WT_Integer32        m_temp_index;
    WT_RGBA32           m_temp_rgba;
    WT_Logical_Box      m_temp_box;
    WT_Directory        m_directory;

	char				m_write_ascii_format[0xf];

    //As we skip unknown extended ascii opcodes (possibly because, those opcodes
    //were added as part of a subsequent whip toolkit releases), we just accumulate
    //the unknown opcode's field length so that we can seek back and read them
    //when unknown opcode materializes as part of WT_Unknown::materialize.
    WT_Unsigned_Integer16 m_skip_byte_length;

    enum
    {
        Getting_Count_Byte,
        Getting_Extended_Count_Short
    }               m_read_count_state;

    enum
    {
        Eating_Initial_Whitespace,
        Getting_Sign,
        Eating_Post_Sign_Whitespace,
        Getting_First_Digit,
        Accumulating_Subsequent_Digits,
        Accumulating_Fractional_Digits,
        Accumulating_Exponent
    }               m_read_ascii_integer32_state,
                    m_read_ascii_double_state;

    enum
    {
        Getting_X_Coordinate,
        Getting_Comma,
        Getting_Y_Coordinate
    }               m_read_ascii_logical_point_state;
    enum
    {
        Getting_Min_Point,
        Eating_Middle_Whitespace,
        Getting_Max_Point
    }               m_read_ascii_logical_box_state;
    enum
    {
        Getting_First_Value,
        Getting_Separator,
        Getting_Green,
        Getting_Green_Comma,
        Getting_Blue,
        Getting_Blue_Comma,
        Getting_Alpha
    }               m_read_ascii_color_state;

    int m_read_hex_multiple_state;

    enum
    {
        Eating_Whitespace,
        Getting_Hex_Value
    }               m_read_hex_state;


                 WT_Result      shut_down_decompression();


private:
    WT_File (WT_File const &)
    {
        WD_Complain ("cannot copy WT_File");
    } // prohibited


    WT_File & operator= (WT_File const &)
    {
        WD_Complain ("cannot assign WT_File");
        return *this;
    } // prohibited

    static WT_Result temporary_decomp_leftover_end_seek (WT_File & file);
    static WT_Result temporary_decomp_leftover_read (WT_File & file,    int desired_bytes, int & bytes_read, void * buffer);
    static WT_Result temporary_decomp_leftover_seek (WT_File & file,    int desired_bytes, int & bytes_seeked);

    WT_Result                    write_uncompressed_tab_level(void);
    WT_Result                    write_uncompressed        (char const * str);
    WT_Result                    write_uncompressed        (WT_Byte a_byte);

    /*
    Parent application would typically open a dwf file in Block_Append or
    Block_Read mode to append new blocks at the end and update the
    directory info. Read_Write mode of opening dwf file feature was
    primarily added for server side programming (non-streaming users). In
    such a scenario, initialize_open_for_read_write_mode function will set
    the file heuristics format type to either binary or ascii and read
    through the directory information and set the file pointer position to
    the begining file offset of the existing directory opcode information.
    So, a subseqeunt change in rendition blockref would signal addition of
    new block at the end with new drawables following it. Whip toolkit
    will finally take care of rewriting the updated directory information.
    Also, when opened in Block_Append mode, we read through the existing
    dwf data to make sure that we have the right rendition attribute
    data set before we start appending new information. Also, to properly
    transform the appended block information in to relativized dwf data,
    we need to read through existing information. We don't have to do
    this for Block_Read mode.
    */
    WT_Result       initialize_open_for_block_read_mode();
    WT_Result       initialize_open_for_block_append_mode();

    /*Since the block size for a already serialized block is not decided
    up until the next blockref gets serialized (as mentioned above in the
    comments for set_block_size_for_tail_blockref function), we wait till
    the file close time (when the block size for all blocks is pretty much
    decided) and seek back to the blockref serialized instances and
    re-serialize them with the updated block size information. */
    WT_Result       rewrite_blockref_instance(WT_BlockRef *blockref);

    /*rewrite_blockref_instance function updates an (one) already serialized
    blockref instance (which is not part of the directory instance).
    rewrite_block_sizes_for_all_blockrefs loops through and does the rewriting
    business for all of the previously serialized blockref instances.*/
    WT_Result       rewrite_block_sizes_for_all_blockrefs();

};

//@}


/// An non-file, useful for initializations
//extern WT_File  _WT_File_g_none;


// This stuff collects statistics about the file being read into the viewer.
// The class isn't used when writing to a file.

#include "whiptk/file_stats.h"


#endif // FILE_HEADER
