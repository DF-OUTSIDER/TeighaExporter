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
///\file        dwf/whiptk/opcode.h
///

#if !defined OPCODE_HEADER
#define OPCODE_HEADER

#include "whiptk/whipcore.h"
#include "whiptk/object.h"

class WT_Rendition;

/// A class representing an WHIP! opcode, also the base class for a WT_Optioncode.
class WHIPTK_API WT_Opcode
{
	friend class WT_Font_Option_Style;
	friend class WT_Pen_Pattern;
	friend class WT_File;
	friend class WT_Image;
	friend class WT_PNG_Group4_Image;
	friend class WT_Viewport;
	friend class WT_SignData;
	friend class WT_Object_Stream;
	friend class WT_Guid_List;
	friend class WT_Directory;
	friend class WT_BlockRef;
	friend class WT_W2D_Class_Factory;

public:
    /// Opcode type
    enum WT_Type
    {
        Single_Byte,
        Extended_ASCII,
        Extended_Binary,
        Unary_Optioncode,
        Null_Optioncode
    };
private:
    enum WT_Status
    {
        Starting,
        Finished,
        Eating_Whitespace,
        Determining_Opcode_Type,
        Accumulating_Extended_Opcode
    };

    int            m_size;
    WT_Byte        m_token[WD_MAX_OPCODE_TOKEN_SIZE + 2];
    WT_Type        m_type;
    WT_Status      m_status;

    int            m_prior_paren_level;

public:
    //@{ \name Construction
    WT_Opcode()
        :m_size (0)
        ,m_type (Single_Byte)
        ,m_status (Starting)
        ,m_prior_paren_level(0)
    { }
    //@}

    virtual ~WT_Opcode()
    { }

	virtual WT_Result        serialize (WT_File & /*file*/) const
	{
		return WT_Result::Success;
	}

    /// Returns the previous parenthesis level.
    virtual int matching_paren_level() const  {  return m_prior_paren_level; }
    /// Returns the opcode size.
    virtual int size() const  { return m_size; }
    /// Returns a pointer to the opcode token.
    virtual WT_Byte const * token() const {  return &m_token[0]; }
	void	set_token(const char cToken);
    /// Returns the opcode type one of WT_Type "enum WT_Type".
    virtual WT_Type type() const { return m_type; }
    /// Causes the WT_File object to move its file pointer to te byte preceeding the closing parenthesis of this opcode.
    /** \retval WT_Result::Success The operation was successful. */
    virtual WT_Result skip_past_matching_paren(WT_File & file) const;
    /// Causes the file to read the next opcode from the data stream into this object.
    /** \retval WT_Result::Success The operation was successful. */
    WT_Result      get_opcode(WT_File & file, WT_Boolean allow_sub_options = WD_False);
    /// Constructs an empty object from the current opcode.
    /** \retval WT_Object* A default object of the type specified by the curren topcode.
     *  \retval NULL The object could not be created.  Check the \a result parameter.
     */
    WT_Object *    object_from_opcode(
        WT_Rendition & rend, /**< Input parameter: the current file rendition, typically file.rendition() */
        WT_Result & result, /**< Output parameter: the result of the object creation effort. */
        WT_File & file /**< Input parameter: the file being read. */
        ) const;
    /// Returns WD_Trye if (byte >= '!' && byte <= 'z' && byte != '(' && byte != ')')
    WT_Boolean     is_legal_opcode_character(WT_Byte byte) const;
    /// Returns WD_True if byte is a ( or a ) or a NULL, space, tab, linefeed or carriage return.
    WT_Boolean     is_opcode_terminator(WT_Byte byte) const;
};

/// A base class for option classes described within parent opcodes.
class WHIPTK_API WT_Optioncode : public WT_Opcode
{
protected:

    int m_option_id; /**< An ID value for the option. */

public:

    /// Constructs a WT_Optioncode object.
    WT_Optioncode()
        : m_option_id(-1)
    { }

    /// Causes the file to read the next optioncode from the data stream into this object.
    /** \retval WT_Result::Success The operation was successful. */
    WT_Result get_optioncode(WT_File & file)
    {
        m_option_id = -1;                   // Get rid of any previous option's id
        return WT_Opcode::get_opcode(file, WD_True);
    }

    /// Returns the option ID value.
    int option_id()
    {
        if (m_option_id == -1)
            m_option_id = option_id_from_optioncode();
        return m_option_id;
    }

    /// Returns the option ID value.
    virtual int option_id_from_optioncode() = 0;
};

#endif // OPCODE_HEADER
