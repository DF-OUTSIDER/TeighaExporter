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

#if !defined INFORMATIONAL_HEADER
#define INFORMATIONAL_HEADER

///
///\file        dwf/whiptk/informational.h
///

#include "whiptk/whipcore.h"
#include "whiptk/object.h"


#define DECLARE_INFORMATIONAL_CLASS(class_name)\
    class WHIPTK_API WT_##class_name : public WT_Informational {\
    public: \
        WT_ID object_id() const;\
        WT_Type        object_type() const;\
        WT_Result      process(WT_File & file);\
        WT_Result      serialize(WT_File & file) const;\
        /** Provides a default action handler for this object. \warning This is used by the framework and should not be called by client code. */ static WT_Result default_process( /** The object to process. */ WT_##class_name & item, /** The file being read. */WT_File & file );\
    };

#define IMPLEMENT_INFORMATIONAL_CLASS_FUNCTIONS(class_name, class_lower, class_type, class_command)\
    WT_Object::WT_ID    WT_##class_name::object_id() const\
        {return class_name##_ID;}\
    WT_Object::WT_Type WT_##class_name::object_type() const\
        {return class_type;}\
    WT_Result WT_##class_name::serialize(WT_File & file) const\
        {return WT_Informational::serialize (file, class_command);}\
    WT_Result WT_##class_name::process(WT_File & file)\
        {WD_Assert (file.class_lower##_action());\
            return (file.class_lower##_action())(*this, file);}\
    WT_Result WT_##class_name::default_process(WT_##class_name & item, WT_File & file)\
        {   file.rendition().drawing_info().class_lower() = item;\
            preprocess_workarounds(item, file);\
            return WT_Result::Success;}


/** \addtogroup groupMetadata Metadata objects
 *  \note The metadata objects are all deprecated.  Users desiring to add metadata to the W2D
 *  page should create properties using DWFEPlot::EPlotPage::GetProperties()->AddProperty().
 *  Use of these objects is allowed in version 6.0 and beyond, but this practice is discouraged.
 *  \sa WT_Drawing_Info
 *  @{
 */

/// A metadata class for internal drawing metadata (also used as a base class for specific metadata objects.)
/** \deprecated This class is provided for backward compatibiility with classic %DWF
 *  files of version 00.55 and earlier.  Users desiring to add metadata to the W2D
 *  page should create properties using DWFEPlot::EPlotPage::GetProperties()->AddProperty().
 *  Use of derivatives of this class are allowed in version 6.0 and beyond, but this is discouraged.
 */
class WHIPTK_API WT_Informational : public WT_Object
{
protected:

    WT_String m_string; /**< \brief String containing the information value.*/

    /// Materialization stage.
    enum WT_Materialize_Stage
    {
        Eating_Initial_Whitespace,
        Gathering_String,
        Eating_End_Whitespace
    } m_stage;

protected:
    /// Performs some fixes to data containing known errors from specific authoring apps.
    static WT_Result preprocess_workarounds(WT_Object & item, WT_File & file);

public:

    /// Constructs a WT_Informational object.
    WT_Informational ()
        : m_stage (Eating_Initial_Whitespace)
    {}

    /// Constructs a WT_Informational object with the given string value.
    WT_Informational(char * string)
        : m_stage (Eating_Initial_Whitespace)
    {   set(string);    }

    /// Constructs a WT_Informational object with the given string value.
    WT_Informational(WT_Unsigned_Integer16 * string)
        : m_stage (Eating_Initial_Whitespace)
    {   set(string);    }

    /// Constructs a WT_Informational object.  Copy constructor.
    WT_Informational(WT_Informational const & info)
        : WT_Object()
        , m_stage (Eating_Initial_Whitespace)
    {
        set(info.string());
    }
	virtual ~WT_Informational()
	{}
public:
    //@{ \name WT_Object virtual methods
    virtual WT_Type    object_type() const { return Informational; }
    virtual WT_ID            object_id() const { return Unknown_ID; }
    virtual WT_Result        process(WT_File & /*file*/) { return WT_Result::Success; }
    virtual WT_Result        serialize (WT_File & /*file*/) const { return WT_Result::Success; }
    virtual WT_Result            materialize(WT_Opcode const & opcode, WT_File & file);
    /// Provides a means to create custom informational metadata with the given string name.
    virtual WT_Result            serialize(WT_File & file, char const * name) const;
    virtual WT_Result            skip_operand(WT_Opcode const & opcode, WT_File & file);
    //@}

    //@{ \name Data access and manipulation methods
    /// Sets the string value with the given string.
    void                set(char const * info);
    /// Sets the string value with the given string.
    void                set(WT_Unsigned_Integer16 const * info);
    /// Sets the string with characters from the given string buffer.
    void                set(int length, WT_Unsigned_Integer16 const * info);
    /// Sets the string value with the given string.
    void                set(WT_String const & info);
    /// Returns the string value.
    WT_String const &   string(void) const
    {   return m_string;    }
    /// Assignment operator.  Assigns the given object's string value to this one.
    WT_Informational const & operator=(WT_Informational const & info)
    {
        set(info.string());
        return *this;
    }
    /// Assignment operator.  Assigns the given string to this one.
    WT_Informational const & operator=(char * info)
    {
        set(info);
        return *this;
    }
    //@}
};

/// String object containing the author of this drawing.
/** \deprecated This class is provided for backward compatibiility with classic %DWF files of version 00.55 and earlier.  Users desiring to add metadata to the W2D page should create properties using DWFEPlot::EPlotPage::GetProperties()->AddProperty(). Use of this class is allowed in version 6.0 and beyond, but this is discouraged. */ \
DECLARE_INFORMATIONAL_CLASS (Author)
/// String object containing comments regarding this drawing.
/** \deprecated This class is provided for backward compatibiility with classic %DWF files of version 00.55 and earlier.  Users desiring to add metadata to the W2D page should create properties using DWFEPlot::EPlotPage::GetProperties()->AddProperty(). Use of this class is allowed in version 6.0 and beyond, but this is discouraged. */ \
DECLARE_INFORMATIONAL_CLASS (Comments)
/// String object containing copyright information.
/** \deprecated This class is provided for backward compatibiility with classic %DWF files of version 00.55 and earlier.  Users desiring to add metadata to the W2D page should create properties using DWFEPlot::EPlotPage::GetProperties()->AddProperty(). Use of this class is allowed in version 6.0 and beyond, but this is discouraged. */ \
DECLARE_INFORMATIONAL_CLASS (Copyright)
/// String object containing the name of the application which created this drawing.
/** \deprecated This class is provided for backward compatibiility with classic %DWF files of version 00.55 and earlier.  Users desiring to add metadata to the W2D page should create properties using DWFEPlot::EPlotPage::GetProperties()->AddProperty(). Use of this class is allowed in version 6.0 and beyond, but this is discouraged. */ \
DECLARE_INFORMATIONAL_CLASS (Creator)
/// String object containing a Description of this drawing.
/** \deprecated This class is provided for backward compatibiility with classic %DWF files of version 00.55 and earlier.  Users desiring to add metadata to the W2D page should create properties using DWFEPlot::EPlotPage::GetProperties()->AddProperty(). Use of this class is allowed in version 6.0 and beyond, but this is discouraged. */ \
DECLARE_INFORMATIONAL_CLASS (Description)
/// String object containing keywords to facilitate text searches upon this drawing.
/** \deprecated This class is provided for backward compatibiility with classic %DWF files of version 00.55 and earlier.  Users desiring to add metadata to the W2D page should create properties using DWFEPlot::EPlotPage::GetProperties()->AddProperty(). Use of this class is allowed in version 6.0 and beyond, but this is discouraged. */ \
DECLARE_INFORMATIONAL_CLASS (Keywords)
/// String object containing the title of this drawing.
/** \deprecated This class is provided for backward compatibiility with classic %DWF files of version 00.55 and earlier.  Users desiring to add metadata to the W2D page should create properties using DWFEPlot::EPlotPage::GetProperties()->AddProperty(). Use of this class is allowed in version 6.0 and beyond, but this is discouraged. */ \
DECLARE_INFORMATIONAL_CLASS (Title)
/// String object containing the nature of the drawing such as the discipline or type of drawing.
/** \deprecated This class is provided for backward compatibiility with classic %DWF files of version 00.55 and earlier.  Users desiring to add metadata to the W2D page should create properties using DWFEPlot::EPlotPage::GetProperties()->AddProperty(). Use of this class is allowed in version 6.0 and beyond, but this is discouraged. */ \
DECLARE_INFORMATIONAL_CLASS (Subject)
/// String object containing the filename of the source data from which this drawing was created.
/** \deprecated This class is provided for backward compatibiility with classic %DWF files of version 00.55 and earlier.  Users desiring to add metadata to the W2D page should create properties using DWFEPlot::EPlotPage::GetProperties()->AddProperty(). Use of this class is allowed in version 6.0 and beyond, but this is discouraged. */ \
DECLARE_INFORMATIONAL_CLASS (Source_Filename)

//@}

#endif // INFORMATIONAL_HEADER
