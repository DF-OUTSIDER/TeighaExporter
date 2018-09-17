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

#if !defined WHIPERRS_HEADER
#define WHIPERRS_HEADER

///
///\file        dwf/whiptk/whiperrs.h
///

#include "whiptk/whipcore.h"

#define alter
#define WT_ENUM_CLASS_FUNCTIONS(Classname)                                                              \
  protected:                                                                                            \
    /** \brief The result value. */                                                                     \
    Enum            m_value;                                                                            \
  public:                                                                                               \
    /** \brief Constructs the object. */                                                                \
    Classname() : m_value ((Enum) 0) {}                                                                 \
    /** \brief Constructs the object.  Copy constructor. */                                             \
    Classname(Classname const & in) : m_value (in.m_value) {}                                           \
    /** \brief Constructs the object using the given value. */                                          \
    Classname(Enum in) : m_value (in) {}                                                                \
    /** \brief Int type cast operator. */                                                               \
    operator int() const {return (int) m_value;}                                                        \
    /** \brief Assignment operator (assigns value from the same type of object.) */                     \
    Classname const & operator=  (Classname const & in) alter {m_value = in.m_value; return *this;}     \
    /** \brief Assignment operator (assigns value from an Enum.) */                                     \
    Classname const & operator=  (Classname::Enum in)   alter {m_value = in; return *this;}             \
    /** \brief Equality operator (compare with the same type of object.) */                             \
    WT_Boolean        operator== (Classname const & in) const {return m_value == in.m_value;}           \
    /** \brief Equality operator (compare with an Enum value.) */                                       \
    WT_Boolean        operator== (Classname::Enum in)   const {return m_value == in;}                   \
    /** \brief Inequality operator (compare with the same type of object.) */                           \
    WT_Boolean        operator!= (Classname const & in) const {return m_value != in.m_value;}           \
    /** \brief Inequality operator (compare with an Enum value.) */                                     \
    WT_Boolean        operator!= (Classname::Enum in)   const {return m_value != in;}                  //

#define WT_ENUM_BIT_CLASS_FUNCTIONS(Classname)                                                          \
    WT_ENUM_CLASS_FUNCTIONS (Classname)                                                                 \
    /** \brief Bitwise OR assignment operator.  Bitwise ORs the value with the current one and sets the current value with the result. */ \
    Classname const & operator|= (Classname const & in)  alter {((int &)m_value) |= in.m_value;         \
                                                                return *this;}                          \
    /** \brief Bitwise OR assignment operator.  Bitwise ORs the value with the current one and sets the current value with the result. */ \
    Classname const & operator|= (Classname::Enum in)    alter {((int &)m_value) |= in; return *this;}  \
    /** \brief Bitwise AND assignment operator.  Bitwise ANDs the value with the current one and sets the current value with the result. */ \
    Classname const & operator&= (Classname const & in)  alter {((int &)m_value) &= in.m_value;         \
                                                                return *this;}                          \
    /** \brief Bitwise AND assignment operator.  Bitwise ANDs the value with the current one and sets the current value with the result. */ \
    Classname const & operator&= (Classname::Enum in)    alter {((int &)m_value) &= in; return *this;}  \
    /** \brief Bitwise XOR assignment operator.  Bitwise XORs the value with the current one and sets the current value with the result. */ \
    Classname const & operator^= (Classname const & in)  alter {((int &)m_value) ^= in.m_value;         \
                                                                return *this;}                          \
    /** \brief Bitwise XOR assignment operator.  Bitwise XORs the value with the current one and sets the current value with the result. */ \
    Classname const & operator^= (Classname::Enum in)    alter {((int &)m_value) ^= in; return *this;}  \
    /** \brief Bitwise OR operator.  Bitwise ORs the value with the current one and returns the resultant value. */ \
    Classname::Enum   operator|  (Classname const & in)  const {return (Enum)(m_value | in.m_value);}   \
    /** \brief Bitwise OR operator.  Bitwise ORs the value with the current one and returns the resultant value. */ \
    Classname::Enum   operator|  (Classname::Enum in)    const {return (Enum)(m_value | in);}           \
    /** \brief Bitwise AND operator.  Bitwise ANDs the value with the current one and returns the resultant value. */ \
    Classname::Enum   operator&  (Classname const & in)  const {return (Enum)(m_value & in.m_value);}   \
    /** \brief Bitwise AND operator.  Bitwise ANDs the value with the current one and returns the resultant value. */ \
    Classname::Enum   operator&  (Classname::Enum in)    const {return (Enum)(m_value & in);}           \
    /** \brief Bitwise XOR operator.  Bitwise XORs the value with the current one and returns the resultant value. */ \
    Classname::Enum   operator^  (Classname const & in)  const {return (Enum)(m_value ^ in.m_value);}   \
    /** \brief Bitwise XOR operator.  Bitwise XORs the value with the current one and returns the resultant value. */ \
    Classname::Enum   operator^  (Classname::Enum in)    const {return (Enum)(m_value ^ in);}          //


/// A return value and exception class
/**
 *  The extension of C to C++ left enums in a half-baked state.  The scope of the enum
 *  contents is the same as that of the enum definition, so it is necessary to either
 *  provide very long-winded names for each element, or to encapsulate each enum within
 *  a class and require qualification of each element by the class name (the resulting
 *  names look similar, but the second method allows us to keep the simpler names
 *  inside the definition, and is a more object-oriented approach).  In addition,
 *  we can provide methods on the class to access elements using the internal enum.
 *  Additional methods are available for those enums which also act as bitmasks for
 *  multiple options.  We can't use any form of template to do this as it would require
 *  defining the enum before the class, which defeats the whole purpose.
 *
 *  Use of the form Classname::Enum should only be required in special cases such as
 *  when a value stored as an integer is to be converted back into the internal enum,
 *  such as when catching the value as an exception in a try / catch block.
 *
 *  Essentially, we are recreating enums with the name scoping we desire.
 */
class WHIPTK_API WT_Result
{
    public:
        /// An enumeration of result/exception values.
        enum Enum {
            Success,
            Waiting_For_Data,
            Corrupt_File_Error,
            End_Of_File_Error,
            Unknown_File_Read_Error,
            Out_Of_Memory_Error,
            File_Already_Open_Error,
            No_File_Open_Error,
            File_Write_Error,
            File_Open_Error,
            Internal_Error,
            Not_A_DWF_File_Error,
            User_Requested_Abort,
            DWF_Version_Higher_Than_Toolkit,
            Unsupported_DWF_Opcode,
            Unsupported_DWF_Extension_Error,
            End_Of_DWF_Opcode_Found,
            File_Inconsistency_Warning,
            Toolkit_Usage_Error,
            Decompression_Terminated,
            File_Close_Error,
            Opcode_Not_Valid_For_This_Object,
            DWF_Package_Format,
            Minor_Version_Warning,
            Undefined = -1
        };

    WT_ENUM_CLASS_FUNCTIONS (WT_Result)
};

#endif // WHIPERRS_HEADER
