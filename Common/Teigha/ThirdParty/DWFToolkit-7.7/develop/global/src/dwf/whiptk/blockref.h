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

#if !defined BLOCKREF_HEADER
#define BLOCKREF_HEADER

///
///\file        dwf/whiptk/blockref.h
///

#include "whiptk/whipcore.h"
#include "whiptk/file.h"
#include "whiptk/blockref_defs.h"
#include "whiptk/filetime.h"
#include "whiptk/matrix.h"


//The following macro tells you whether a WT_BlockRef field is applicable for a particular
//blockref kind or not by looking at the BLOCK_VARIABLE_RELATION table.
#define Verify(Variable, BlockType)\
    BLOCK_VARIABLE_RELATION[WT_BlockRef::n_##Variable][BlockType - WD_EXBO_GRAPHICS_HDR]

//The following macro just defines a variable
//if specified a variable name and variable type.
#define DEFINEVARIABLE(VARIABLETYPE, VARIABLENAME)\
    VARIABLETYPE m_##VARIABLENAME;

//Macro defines the get/set methods based on the variable name and type.
#define DEFINEGETSETMETHODS(VARIABLETYPE, METHODNAME)\
    /** \brief Returns the value of the local VARIABLETYPE variable METHODNAME. */ const VARIABLETYPE& get_##METHODNAME() const {\
   return m_##METHODNAME;\
}\
/** \brief Sets the value of the local VARIABLETYPE variable METHODNAME. */ void set_##METHODNAME(const VARIABLETYPE& local_variable_name) {\
   m_##METHODNAME = local_variable_name;\
}

//The following macro just defines a variable
//as an array if specified a name and type.
#define DEFINEVARIABLEARRAY(VARIABLETYPE, VARIABLENAME, ARRAYLENGTH)\
    VARIABLETYPE m_##VARIABLENAME[ARRAYLENGTH];

//For array variable fields we define get/set methods little bit differently.
#define DEFINEARRAYGETSETMETHODS(VARIABLETYPE, METHODNAME, ARRAYLENGTH)\
    /** \brief Returns the value of the local VARIABLETYPE array METHODNAME. */ VARIABLETYPE * get_##METHODNAME() {\
   return (VARIABLETYPE *) &m_##METHODNAME;\
}\
/** \brief Returns the number of items in the local VARIABLETYPE array METHODNAME. */ WT_Integer32 get_##METHODNAME##_length() {\
   return ARRAYLENGTH;\
}\
/** \brief Sets the contents of the local METHODNAME VARIABLETYPE array. */ void set_##METHODNAME(VARIABLETYPE *local_variable_name) {\
   for(int zz=0; zz < ARRAYLENGTH; zz++) m_##METHODNAME[zz] = local_variable_name[zz];\
}

/** \addtogroup groupRenditionAttributes
 *  @{
 */

/// Rendition attribute describing the currrent block reference.
/** \deprecated BlockRefs were only available in vesion 00.55, they were a
 *  short-lived architectural solution that has been deprecated in deference
 *  to the %DWF 6 package format structure.
 *
 *  For version 00.55, drawables, attributes and other objects can be grouped
 *  as blocks in %DWF files with some metadata (in uncompressed form)
 *  represented as an opcode represented by a WT_BlockRef object.  Different
 *  kinds of blockref objects would be specified as rendition attributes
 *  based on the kind of geometry or data that follows it. As they were
 *  serialized, each of those blockref's would be accumulated in the
 *  WT_Directory object instance maintained in the WT_File object.
 *
 *  Since certain fields are common to different kinds of blockref's,
 *  it was decided to limit the number of WT_Objects that would be
 *  added to the toolkit by creating the gigantic three-ton angry
 *  gorilla of an object called WT_BlockRef which is capable of
 *  serializing all kinds of blockrefs based its format field as well as
 *  serving as a lay functionary in various local, state and federal
 *  government positions.  WT_BlockRef will guarantee that your tax
 *  dollars will not get spent unwisely as long as you do not remind it
 *  of its true nature: Gorilla - the largest of the anthropoid apes native
 *  to the forests of equatorial Africa, having a stocky body and coarse,
 *  dark brown or black hair.  \warning Do not taunt or attempt to feed WT_BlockRef.
 *
 *  \internal The BLOCK_VARIABLE_RELATION global static boolean table stores the
 *  relevant (applicable) fields for relevant blockref kind. This
 *  table essentially decides 'which field gets serialized/materialized
 *  for a particular blockref kind'.  Modifying this table in future
 *  (after a WHIP! toolkit release) would break backward/forward
 *  compatibility of dwf file format. Columns and rows could be
 *  added at the end without affecting backward or forward compatibility,
 *  but they won't be... as this object has been deprecated.
 *  \sa WT_Rendition
 */
class WHIPTK_API WT_BlockRef : public WT_Item, public WT_Attribute
{
    friend class WT_File;
    friend class WT_Directory;
	friend class WT_Rendition;
	friend class WT_Opcode;
	friend class WT_BlockRef_List;
    
public:

    ///Based on this and BLOCK_VARIABLE_RELATION table, different member variables of WT_BlockRef class can be serialized.
    enum WT_BlockRef_Format
    {
        Graphics_Hdr       = WD_EXBO_GRAPHICS_HDR           ,
        Overlay_Hdr        = WD_EXBO_OVERLAY_HDR            ,
        Redline_Hdr        = WD_EXBO_REDLINE_HDR            ,
        Thumbnail          = WD_EXBO_THUMBNAIL              ,
        Preview            = WD_EXBO_PREVIEW                ,
        Overlay_Preview    = WD_EXBO_OVERLAY_PREVIEW        ,
        EmbedFont          = WD_EXBO_FONT                   ,
        Graphics           = WD_EXBO_GRAPHICS               ,
        Overlay            = WD_EXBO_OVERLAY                ,
        Redline            = WD_EXBO_REDLINE                ,
        User               = WD_EXBO_USER                   ,
        Null               = WD_EXBO_NULL                   ,
        Global_Sheet       = WD_EXBO_GLOBAL_SHEET           ,
        Global             = WD_EXBO_GLOBAL                 ,
        Signature          = WD_EXBO_SIGNATURE
    };

    /// Constructs a WT_BlockRef object.
    WT_BlockRef ();
    /// Constructs a WT_BlockRef object with the given format.
    WT_BlockRef (WT_BlockRef_Format format);
    /// Constructs a WT_BlockRef object.  Copy constructor.
    WT_BlockRef (WT_BlockRef const & blockref);
    /// Destroys a WT_BlockRef object.
    virtual ~WT_BlockRef()
    {
    }

    /// Inequality operator.  Returns WD_True if the given blockref is NOT equal to this one.
    WT_Boolean    operator!= (WT_BlockRef const & attrib) const;
    /// Equality operator.  Returns WD_True if the given blockref is equal to this one.
    WT_Boolean    operator== (WT_BlockRef const & attrib) const;
    /// Assignment operator.  Assigns the given blockref's contents to this one.
    WT_BlockRef   const &   operator=(WT_BlockRef const & blockref);

    //@{ \name Data access methods

    DEFINEGETSETMETHODS(WT_Alignment            ,alignment);
    DEFINEGETSETMETHODS(WT_Guid                 ,block_guid);
    DEFINEGETSETMETHODS(WT_Block_Meaning        ,block_meaning);
    DEFINEARRAYGETSETMETHODS(WT_Logical_Point   ,clip_rectangle, 2);
    DEFINEGETSETMETHODS(WT_FileTime             ,container_modified_time);
    DEFINEGETSETMETHODS(WT_FileTime             ,creation_time);
    DEFINEGETSETMETHODS(WT_Integer16            ,dpi_resolution);
    DEFINEGETSETMETHODS(WT_Guid                 ,dwf_container_guid);
    DEFINEGETSETMETHODS(WT_Guid                 ,dwf_discipline_guid);
    DEFINEGETSETMETHODS(WT_FileTime             ,dwf_discipline_modified_time);
    DEFINEGETSETMETHODS(WT_Encryption           ,encryption);
    DEFINEGETSETMETHODS(WT_Boolean              ,flag_mini_dwf);

    /// Returns the blockref size (in bytes).
    WT_Unsigned_Integer32 get_block_size() const {
       return m_block_size;
    }

    /// Return the file offset for this object.
    /** \note There is no set_file_offset method; it is
     * left to the toolkit to figure out the begining
     * blockref opcode file offset when it gets serialized.
     */
    WT_Unsigned_Integer32 get_file_offset() const {
       return m_file_offset;
    }

    /// Returns the blockref format, one of \ref WT_BlockRef_Format "enum WT_BlockRef_Format."
    WT_BlockRef_Format get_format() const {
        return m_format;
    }

    /// Returns the blockref index (in the set of blockrefs.)
    WT_Integer32 index() const {
        return m_index;
    }

    DEFINEARRAYGETSETMETHODS(WT_Integer32       ,image_representation, 3);
    DEFINEARRAYGETSETMETHODS(double             ,inked_area, 2);
    DEFINEGETSETMETHODS(WT_Boolean              ,inversion_flag);
    DEFINEGETSETMETHODS(WT_FileTime             ,last_sync_time);
    DEFINEGETSETMETHODS(WT_Boolean              ,mirror_flag);
    DEFINEGETSETMETHODS(WT_FileTime             ,modification_time);
    DEFINEGETSETMETHODS(WT_FileTime             ,modified_block_timestamp);
    DEFINEGETSETMETHODS(WT_Orientation          ,orientation);
    DEFINEARRAYGETSETMETHODS(double             ,paper_offset, 2);
    DEFINEGETSETMETHODS(double                  ,paper_scale);
    DEFINEGETSETMETHODS(WT_Guid                 ,parent_block_guid);
    DEFINEGETSETMETHODS(WT_Password             ,password);
    DEFINEGETSETMETHODS(WT_Guid                 ,plans_and_specs_website_guid);
    DEFINEGETSETMETHODS(WT_FileTime             ,print_sequence_modified_time);
    DEFINEGETSETMETHODS(WT_Guid                 ,related_overlay_hdr_block_guid);
    DEFINEGETSETMETHODS(WT_Integer16            ,rotation);
    DEFINEGETSETMETHODS(WT_Boolean              ,scan_flag);
    /// Sets the blockref format, one of WT_BlockRef_Format "enum WT_BlockRef_Format."
    void    set_format(WT_BlockRef_Format format);
    DEFINEGETSETMETHODS(WT_Integer32            ,sheet_print_sequence);
    DEFINEGETSETMETHODS(WT_Matrix               ,targeted_matrix_rep);
    DEFINEGETSETMETHODS(WT_Boolean              ,validity);
    DEFINEGETSETMETHODS(WT_Boolean              ,visibility);
    DEFINEGETSETMETHODS(WT_Integer32            ,zValue);
    //@}

    //@{ \name WT_Object and WT_Attribute virtual methods
    WT_Result materialize (WT_Opcode const & opcode, WT_File & file);
    WT_ID     object_id() const;
    WT_Type   object_type() const;
    WT_Result process(WT_File & file);
    WT_Result serialize(WT_File & file) const;
    WT_Result skip_operand (WT_Opcode const & opcode, WT_File & file);
    WT_Result sync (WT_File & file) const;
    WT_Boolean    operator==(WT_Attribute const & attrib) const;
    //@}

    /// Provides a default action handler for this attribute (e.g. sets the current rendition's corresponding attribute.)
    /** \warning This is used by the framework and should not be called by client code.
     */
    static WT_Result default_process(
        WT_BlockRef & item, /**< The attribute to process. */
        WT_File & file /**< The file being read. */
        );

private:
    WT_BlockRef_Format      m_format;
    WT_Opcode *             m_optioncode;
    WT_Integer32            m_total_binary_opcode_size;
    WT_Integer32            m_index;

    WT_Result        serialize(WT_File & file, WT_Boolean bAsPartOfList, WT_Boolean bForBlockSizeRewrite) const;
    WT_Result        materialize(WT_Opcode const & opcode, WT_File & file, WT_Boolean bAsPartOfList);

    void _deleteObject(void *object)
    {
        delete (WT_BlockRef*)object;
    }

    /// Sets the blockref size (in bytes).
    void set_block_size(WT_Unsigned_Integer32 block_size) {
       m_block_size = block_size;
    }

    /// Determines the total length of the blockref opcode
    WT_Unsigned_Integer32 get_total_binary_opcode_size() {
       return m_total_binary_opcode_size;
    }

    //let us now use the macro and define some private variables.
    DEFINEVARIABLE(WT_Unsigned_Integer32   ,file_offset)
    DEFINEVARIABLE(WT_Unsigned_Integer32   ,block_size)
    DEFINEVARIABLE(WT_Guid                 ,block_guid)
    DEFINEVARIABLE(WT_FileTime             ,creation_time)
    DEFINEVARIABLE(WT_FileTime             ,modification_time)
    DEFINEVARIABLE(WT_Encryption           ,encryption)
    DEFINEVARIABLE(WT_Boolean              ,validity)
    DEFINEVARIABLE(WT_Boolean              ,visibility)
    DEFINEVARIABLE(WT_Block_Meaning        ,block_meaning)
    DEFINEVARIABLE(WT_Guid                 ,parent_block_guid)
    DEFINEVARIABLE(WT_Guid                 ,related_overlay_hdr_block_guid)
    DEFINEVARIABLE(WT_Integer32            ,sheet_print_sequence)
    DEFINEVARIABLE(WT_FileTime             ,print_sequence_modified_time)
    DEFINEVARIABLE(WT_Guid                 ,plans_and_specs_website_guid)
    DEFINEVARIABLE(WT_FileTime             ,last_sync_time)
    DEFINEVARIABLE(WT_Boolean              ,flag_mini_dwf)
    DEFINEVARIABLE(WT_FileTime             ,modified_block_timestamp)
    DEFINEVARIABLE(WT_Guid                 ,dwf_container_guid)
    DEFINEVARIABLE(WT_FileTime             ,container_modified_time)
    DEFINEVARIABLE(WT_Guid                 ,dwf_discipline_guid)
    DEFINEVARIABLE(WT_FileTime             ,dwf_discipline_modified_time)
    DEFINEVARIABLE(WT_Integer32            ,zValue)
    DEFINEVARIABLE(WT_Boolean              ,scan_flag)
    DEFINEVARIABLE(WT_Boolean              ,mirror_flag)
    DEFINEVARIABLE(WT_Boolean              ,inversion_flag)
    DEFINEVARIABLE(double                  ,paper_scale)
    DEFINEVARIABLE(WT_Orientation          ,orientation)
    DEFINEVARIABLE(WT_Integer16            ,rotation)
    DEFINEVARIABLE(WT_Alignment            ,alignment)
    DEFINEVARIABLEARRAY(double             ,inked_area, 2)
    DEFINEVARIABLE(WT_Integer16            ,dpi_resolution)
    DEFINEVARIABLEARRAY(double             ,paper_offset, 2)
    DEFINEVARIABLEARRAY(WT_Logical_Point   ,clip_rectangle, 2)
    DEFINEVARIABLE(WT_Password             ,password)
    DEFINEVARIABLEARRAY(WT_Integer32       ,image_representation, 3)
    DEFINEVARIABLE(WT_Matrix               ,targeted_matrix_rep)

    enum
    {
        Starting,
        Getting_Format,
        Getting_File_Offset,
        Getting_Block_Size,
        Getting_Block_Guid,
        Getting_Creation_Time,
        Getting_Modification_Time,
        Getting_Encryption,
        Getting_Block_Validity_Flag,
        Getting_Block_Visibility_Flag,
        Getting_Block_Meaning,
        Getting_Parent_Block_Guid,
        Getting_Related_Overlay_Hdr_Block_Guid,
        Getting_Sheet_Print_Sequence,
        Getting_Print_Sequence_Modification_Time,
        Getting_Plans_And_Website_Guid,
        Getting_Last_Sync_Time,
        Gettinig_Mini_Dwf_Flag,
        Getting_Block_Modified_TimeStamp,
        Getting_Dwf_Container_Id,
        Getting_Dwf_Container_Modification_Time,
        Getting_Dwf_Discipline_Guid,
        Getting_Dwf_Discipline_Modification_Time,
        Getting_ZValue,
        Getting_Scan_Flag,
        Getting_Mirror_Flag,
        Getting_Inversion_Flag,
        Getting_Paper_Scale,
        Getting_Orientation,
        Getting_Paper_Rotation,
        Getting_Alignment,
        Getting_Inked_Area,
        Getting_Dpi_Resolution,
        Getting_Paper_Offset,
        Getting_Clipping_Rectangle,
        Getting_Password,
        Getting_Image_Size_Representation,
        Getting_Targeted_Matrix_Representation,
        Getting_Close
    }   m_stage;

    /// Establishes a relationship between member variables of WT_BlockRef class and rows of the BLOCK_VARIABLE_RELATION table.
    enum WT_BlockRef_Variables
    {
        n_file_offset                      = 0,
        n_block_size                       = 1,
        n_block_guid                       = 2,
        n_creation_time                    = 3,
        n_modification_time                = 4,
        n_encryption                       = 5,
        n_validity                         = 6,
        n_visibility                       = 7,
        n_block_meaning                    = 8,
        n_parent_block_guid                = 9,
        n_related_overlay_hdr_block_guid   = 10,
        n_sheet_print_sequence             = 11,
        n_print_sequence_modified_time     = 12,
        n_plans_and_specs_website_guid     = 13,
        n_last_sync_time                   = 14,
        n_flag_mini_dwf                    = 15,
        n_modified_block_timestamp         = 16,
        n_dwf_container_guid               = 17,
        n_container_modified_time          = 18,
        n_dwf_discipline_guid              = 19,
        n_dwf_discipline_modified_time     = 20,
        n_zValue                           = 21,
        n_scan_flag                        = 22,
        n_mirror_flag                      = 23,
        n_inversion_flag                   = 24,
        n_paper_scale                      = 25,
        n_orientation                      = 26,
        n_rotation                         = 27,
        n_alignment                        = 28,
        n_inked_area                       = 29,
        n_dpi_resolution                   = 30,
        n_paper_offset                     = 31,
        n_clip_rectangle                   = 32,
        n_password                         = 33,
        n_image_representation             = 34,
        n_targeted_matrix_rep              = 35
    };

    //The moment the blockref format changes, this method has to be
    //called to re-compute the binary blockref opcode size based on the
    //new blockref format.
    void InitializeTotalBinaryOpcodeSize();

    WT_Integer32 ComputeBinaryOpcodeSize(WT_BlockRef_Format format,
        WT_Boolean bAsPartOfList);

    WT_Integer32 ComputeTotalBinaryOpcodeSize(
        WT_Boolean bAsPartOfList);

};

//@}

#endif // BLOCKREF_HEADER
