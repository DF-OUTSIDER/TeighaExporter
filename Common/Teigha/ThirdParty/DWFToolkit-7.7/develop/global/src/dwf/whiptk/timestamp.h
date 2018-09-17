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

#if !defined TIMESTAMP_HEADER
#define TIMESTAMP_HEADER

///
///\file        dwf/whiptk/timestamp.h
///

#include "whiptk/whipcore.h"
#include "whiptk/object.h"

/// Metadata describing a time instant (in seconds) with an associated GUID.
class WHIPTK_API WT_Timestamp : public WT_Object
{
protected:

    // Don't need to document this
    #ifndef DOXYGEN_IGNORE
    WT_String               m_string;
    WT_Integer32            m_seconds;
    WT_String               m_guid;

    enum WT_Materialize_Stage
    {
        Eating_Initial_Whitespace,
        Gathering_Seconds,
        Eating_Middle_Whitespace,
        Gathering_String,
        Eating_Next_Whitespace,
        Gathering_GUID,
        Eating_End_Whitespace
    } m_stage;
    #endif

public:

    //@{ \name Construction
    /// Constructs a WT_Timestamp object.
    WT_Timestamp ()
        : m_seconds (0)
        , m_stage (Eating_Initial_Whitespace)
    {}

    /// Constructs a WT_Timestamp object.
    WT_Timestamp(
        WT_Integer32 seconds, /**< Seconds since midnight 00:00:00 January 1, 1970. */
        char const * exAsciiOpcode /**< The extended ASCII opcode to use when seriailizing. */
        );

    /// Constructs a WT_Timestamp object. Copy constructor.
    WT_Timestamp(WT_Timestamp const & ts)
        : WT_Object()
        , m_seconds (ts.seconds())
        , m_stage (Eating_Initial_Whitespace)
    {
        set(ts.string());
    }
	virtual ~WT_Timestamp()
	{}
    //@}

    //@{ \name Data access methods
    /// Returns the extended ASCII opcode to use when seriailizing.
    WT_String const & string(void) const {   return m_string;    }

    /// Returns the seconds value (seconds since midnight 00:00:00 January 1, 1970.)
    WT_Integer32 seconds(void) const    {   return m_seconds;   }

    /// Returns the GUID string associated with this timestamp.
    WT_String const & guid(void) const {   return m_guid;      }

    /// Sets the extended ASCII opcode to use when seriailizing.
    void       set(char const * exAsciiOpcode);
    /// Sets the extended ASCII opcode to use when seriailizing.
    void       set(WT_String const & exAsciiOpcode);
    /// Sets the extended ASCII opcode to use when seriailizing.
    void       set(WT_Unsigned_Integer16 const * exAsciiOpcode);
    /// Sets the value of this timestamp.
    void       set(
        WT_Integer32 seconds, /**< Seconds since midnight 00:00:00 January 1, 1970. */
        char const * exAsciiOpcode /**< The extended ASCII opcode to use when seriailizing. */
        );
    /// Sets the value of this timestamp.
    void       set(
        WT_Integer32 seconds, /**< Seconds since midnight 00:00:00 January 1, 1970. */
        WT_String const & string /**< The extended ASCII opcode to use when seriailizing. */
        );
    /// Sets the value of this timestamp.
    void       set(
        WT_Integer32 seconds, /**< Seconds since midnight 00:00:00 January 1, 1970. */
        WT_Unsigned_Integer16 const * string /**< The extended ASCII opcode to use when seriailizing. */
        );
    /// Copies the value of the given timestamp to this one.
    void       set(WT_Timestamp const & ts);
    /// Sets the GUID associated with this timestamp.
    void       set_guid(WT_String const & guid);

    /// Assignment operator.  Assigns the value of the given timestamp to this one.
    WT_Timestamp const & operator=(WT_Timestamp const & ts)
    {
        set(ts);
        return *this;
    }
    //@}

    //@{ \name WT_Object virtual methods
    WT_Result    materialize(WT_Opcode const & opcode, WT_File & file);
    WT_Type      object_type() const;
    WT_Result    skip_operand(WT_Opcode const & opcode, WT_File & file);
    //@}

    /// Causes the serialization of the object to the file.
    /** \retval WT_Result::Success The operation was successful. */
    WT_Result    serialize(
        WT_File & file, /**< The file being written. */
        char const * exAsciiOpcode /**< The extended ASCII opcode to use when seriailizing. */
        ) const;
};


#define DECLARE_TIMESTAMP_CLASS(class_name) \
    class WHIPTK_API WT_##class_name : public WT_Timestamp \
    { \
    public: \
        /*@{ \name WT_Object virtual methods */ \
        WT_ID            object_id() const; \
        WT_Result        process(WT_File & file); \
        WT_Result        serialize(WT_File & file) const; \
        /*@}*/ /** \brief Provides a default action handler for this object. */ \
        /** \warning This is used by the framework and should not be called by client code. */ \
        static WT_Result default_process( /** The object to process.*/ WT_##class_name & item, /** The file being read. */ WT_File & file); \
    }                                                                                                  //

#define IMPLEMENT_TIMESTAMP_CLASS_FUNCTIONS(class_name, class_lower, class_command)                        \
    WT_Object::WT_ID    WT_##class_name::object_id() const                                              \
        {return class_name##_ID;}                                                                       \
    WT_Result WT_##class_name::serialize(WT_File & file) const                                          \
        {return WT_Timestamp::serialize (file, class_command);}                                         \
    WT_Result WT_##class_name::process(WT_File & file)                                                  \
        {WD_Assert (file.class_lower##_action());                                                       \
            return (file.class_lower##_action())(*this, file);}                                         \
    WT_Result WT_##class_name::default_process(WT_##class_name & item, WT_File & file)                  \
        {                                                                                               \
            file.rendition().drawing_info().class_lower() = item;                                       \
            return WT_Result::Success;                                                                  \
        }                                                                   //

/// WT_Timestamp derivative object containing the creation time of this drawing.
/** \deprecated This class is provided for backward compatibiility with classic %DWF files
 *   of version 00.55 and earlier.  Users desiring to add creation time metadata to the W2D
 *   resource should create an ISO dateTime string in the form "CCYY-MM-DDThh:mm:ss.sss[Z|{+,-}mm:ss]" and
 *   call the method DWFEPlot::EPlotPage::GetProperties()->AddProperty(EPlotProperty::S_PROPERTY_CREATION_TIME, time).
 *   Use of this class is allowed in version 6.0 and beyond, but this is discouraged. */
DECLARE_TIMESTAMP_CLASS (Creation_Time);
/// WT_Timestamp derivative object containing the modification time of this drawing.
/** \deprecated This class is provided for backward compatibiility with classic %DWF files
 *   of version 00.55 and earlier.  Users desiring to add creation time metadata to the W2D
 *   resource should create an ISO dateTime string in the form "CCYY-MM-DDThh:mm:ss.sss[Z|{+,-}mm:ss]" and
 *   call the method DWFEPlot::EPlotPage::GetProperties()->AddProperty(EPlotProperty::S_PROPERTY_MODIFICATION_TIME, time).
 *   Use of this class is allowed in version 6.0 and beyond, but this is discouraged. */
DECLARE_TIMESTAMP_CLASS (Modification_Time);
/// WT_Timestamp derivative object containing the modification time of this drawing.
/** \deprecated This class is provided for backward compatibiility with classic %DWF files
 *   of version 00.55 and earlier.  Users desiring to add creation time metadata to the W2D
 *   resource should create an ISO dateTime string in the form "CCYY-MM-DDThh:mm:ss.sss[Z|{+,-}mm:ss]" and
 *   call the method DWFEPlot::EPlotPage::GetProperties()->AddProperty(EPlotProperty::S_PROPERTY_SOURCE_CREATION_TIME, time).
 *   Use of this class is allowed in version 6.0 and beyond, but this is discouraged. */
DECLARE_TIMESTAMP_CLASS (Source_Creation_Time);
/// WT_Timestamp derivative object containing the modification time of this drawing.
/** \deprecated This class is provided for backward compatibiility with classic %DWF files
 *   of version 00.55 and earlier.  Users desiring to add creation time metadata to the W2D
 *   resource should create an ISO dateTime string in the form "CCYY-MM-DDThh:mm:ss.sss[Z|{+,-}mm:ss]" and
 *   call the method DWFEPlot::EPlotPage::GetProperties()->AddProperty(EPlotProperty::S_PROPERTY_SOURCE_MODIFICATION_TIME, time).
 *   Use of this class is allowed in version 6.0 and beyond, but this is discouraged. */
DECLARE_TIMESTAMP_CLASS (Source_Modification_Time);

#endif // TIMESTAMP_HEADER
