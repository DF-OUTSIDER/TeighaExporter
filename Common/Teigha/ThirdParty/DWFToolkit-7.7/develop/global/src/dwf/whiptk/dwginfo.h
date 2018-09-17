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

#if !defined DWGINFO_HEADER
#define DWGINFO_HEADER

///
///\file        dwf/whiptk/dwginfo.h
///

#include "whiptk/whipcore.h"
#include "whiptk/timestamp.h"
#include "whiptk/informational.h"
#include "whiptk/named_view_list.h"
#include "whiptk/units.h"

#define WHIP_DRAWING_INFO_LIST\
    WHIP_DRAWING_INFO_MACRO(Author,author)\
    WHIP_DRAWING_INFO_MACRO(Comments,comments)\
    WHIP_DRAWING_INFO_MACRO(Copyright,copyright)\
    WHIP_DRAWING_INFO_MACRO(Creation_Time,creation_time)\
    WHIP_DRAWING_INFO_MACRO(Creator,creator)\
    WHIP_DRAWING_INFO_MACRO(Description,description)\
    WHIP_DRAWING_INFO_MACRO(Keywords,keywords)\
    WHIP_DRAWING_INFO_MACRO(Modification_Time,modification_time)\
    WHIP_DRAWING_INFO_MACRO(Named_View_List,named_view_list)\
    WHIP_DRAWING_INFO_MACRO(Source_Creation_Time,source_creation_time)\
    WHIP_DRAWING_INFO_MACRO(Source_Filename,source_filename)\
    WHIP_DRAWING_INFO_MACRO(Source_Modification_Time,source_modification_time)\
    WHIP_DRAWING_INFO_MACRO(Subject,subject)\
    WHIP_DRAWING_INFO_MACRO(Title,title)\
    WHIP_DRAWING_INFO_MACRO(Units,units)

/** \addtogroup groupFileIO
 *  @{
 */

/// Contains drawing metadata.
/** Note, most of the drawing info metadata objects have been deprecated.
 *  The remaining data useful for versions 06.00 and beyond are the
 *  drawing revision and the WT_Units object.
 *  \sa WT_Rendition
 *
 */
class WHIPTK_API WT_Drawing_Info
{
    friend class WT_DWF_Header;
	friend class WT_Rendition;
	friend class WT_W2D_Class_Factory;

protected:
    WT_Integer32    m_changed;
    int             m_major_revision;
    int             m_minor_revision;

    void set_major_revision(int rev)
    {
        m_major_revision = rev;
        m_changed = WD_True;
    }

    void set_minor_revision(int rev)
    {
        m_minor_revision = rev;
        m_changed = WD_True;
    }

private:
    #define WHIP_DRAWING_INFO_MACRO(class_type, class_lower) \
        WT_##class_type            m_##class_lower;
        WHIP_DRAWING_INFO_LIST
    #undef WHIP_DRAWING_INFO_MACRO

public:
    /// Used in identifying which drawing info data have changed and need to be serialized.
    enum WT_Drawing_Info_Bits
    {
        Description_Bit                 = 0x00000001,
        Author_Bit                      = 0x00000002,
        Comments_Bit                    = 0x00000004,
        Creator_Bit                     = 0x00000008,
        Creation_Time_Bit               = 0x00000010,
        Modification_Time_Bit           = 0x00000020,
        Source_Filename_Bit             = 0x00000040,
        Source_Creation_Time_Bit        = 0x00000080,
        Source_Modification_Time_Bit    = 0x00000100,
        Named_View_List_Bit             = 0x00000200,
        Units_Bit                       = 0x00000400,
        Copyright_Bit                   = 0x00000800,
        Keywords_Bit                    = 0x00001000,
        Title_Bit                       = 0x00002000,
        Subject_Bit                     = 0x00004000
    };

    /// Constructs a WT_Drawing_Info object.
    WT_Drawing_Info()
        : m_changed(0)
        , m_major_revision(0)
        , m_minor_revision(0)
    { }

    /// Destroys a WT_Drawing_Info object.
    virtual ~WT_Drawing_Info()
    { }

    WT_Drawing_Info (WT_Drawing_Info const & other)
        : m_changed         (other.m_changed)
        , m_major_revision  (other.m_major_revision)
        , m_minor_revision  (other.m_minor_revision)
        #define WHIP_DRAWING_INFO_MACRO(class_type, class_lower) \
            , m_##class_lower (other.m_##class_lower)
            WHIP_DRAWING_INFO_LIST
        #undef WHIP_DRAWING_INFO_MACRO
    {  }

    WT_Drawing_Info & operator= (WT_Drawing_Info const & other)
    {
        m_changed         = other.m_changed;
        m_major_revision  = other.m_major_revision;
        m_minor_revision  = other.m_minor_revision;
        #define WHIP_DRAWING_INFO_MACRO(class_type, class_lower) \
            m_##class_lower = other.m_##class_lower;
            WHIP_DRAWING_INFO_LIST
        #undef WHIP_DRAWING_INFO_MACRO

        return *this;
    }

    //@{ \name File revision methods

    /// Returns the decimal revision of the file being read.
    /** The revision is calculated as follows: (major_version * 100) + minor_version.
     *  For example, if the file version is 00.55, the decimal revision will be 55;
     *  for version 06.00, the decimal revision will be 600.
     *  \note This is only used when reading a file.  This method is not applicable in a write mode.
     *  When writing a file, clients should call %WT_File::heuristics().set_target_version()
     *  before opening the file to set the desired version/revision.
     *  \sa WT_File_Heuristics::set_target_version()
     */
    int decimal_revision() const
    {    return (major_revision() * 100) + minor_revision();    }

    /// Returns the major revision of the file being read.
    /** For example, if the file version is 00.55, the major revision is 0.
     *  For version 06.00, the major revision is 6.
     */
    int major_revision() const
    {    return m_major_revision;    }

    /// Returns the minor revision of the file being read.
    /** For example, if the file version is 00.55, the minor revision is 55.
     *  For version 06.00, the minor revision is 0.
     */
    int minor_revision() const
    {    return m_minor_revision;    }
    //@}

    /** \name Metadata read-only accessor methods
     *  The following methods provide read-only access to the drawing info metadata objects.
     *  @{
     */
    #define WHIP_DRAWING_INFO_MACRO(class_type, class_lower)\
        WT_##class_type const & class_lower() const  { return m_##class_lower; }
        WHIP_DRAWING_INFO_LIST
    #undef WHIP_DRAWING_INFO_MACRO
    //@}

    /** \name Metadata accessor methods
     *  The following methods provide writable access to the drawing info metadata objects.
     *  @{
     */
    #define WHIP_DRAWING_INFO_MACRO(class_type, class_lower)\
        WT_##class_type & class_lower() { m_changed |=  class_type##_Bit ; return m_##class_lower; }
        WHIP_DRAWING_INFO_LIST
    #undef WHIP_DRAWING_INFO_MACRO
    //@}

    /// Serializes pending changed drawing info settings to the file.
    /** Only outputs changed desired drawing info changes to the file.
     *  \warning This is used by the framework and should not be called by client code.
     *  \retval WT_Result::Success The operation was successful.
     */
    virtual WT_Result sync(WT_File & file);

    /// Provides a default action handler for this object.
    /** \warning This is used by the framework and should not be called by client code.
     */
    static WT_Result default_process(
        WT_Drawing_Info & item, /**< The object to process. */
        WT_File & file /**< The file being read. */
        );
};

//@}

#endif // DWGINFO_HEADER
