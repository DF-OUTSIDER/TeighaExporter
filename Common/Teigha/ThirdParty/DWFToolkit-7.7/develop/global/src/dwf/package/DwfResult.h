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

#ifndef DWFRESULT_H_INCLUDED
#define DWFRESULT_H_INCLUDED


///
///\file        dwf/package/DwfResult.h
///\brief       This file contains legacy enumerations. 
///
///             This file and the data that it contains have been deprecated.
///             Please do not use any DwfResult references in new code.
///             Please update legacy code and remove any references to DwfResult.
///

#include "dwf/Toolkit.h"

namespace DWFToolkit
{
    ///
    ///\brief       Deprecated.
    ///             Please do not use any DwfResult references in new code.
    ///             Please update legacy code and remove any references to DwfResult.        
    ///\deprecated  Provided for backwards compatability only.
    ///
    class _DWFTK_API DwfResult
    {
        public:
            /// An enumeration of result/exception values.
            enum Enum {
                Success                                     = 0,
                Corrupt_File_Error                          = -1,
                Building_XML_Object_Error                   = -2,
                File_Not_In_DWF                             = -3,
                File_Open_Error                             = -4,
                File_Read_Error                             = -5,
                File_Write_Error                            = -6,
                Internal_Error                              = -7,
                Invalid_Argument_Error                      = -8,
                Invalid_XML_Content_Error                   = -9,
                Invalid_DWF_Version_Error                   = -10,
                Not_A_DWF_Package_Error                     = -11,
                Open_Section_Descriptor_Error               = -12,
                Out_Of_Memory_Error                         = -13,
                Stream_Write_Error                          = -14,
                Temp_File_Error                             = -15,
                Toolkit_Usage_Error                         = -16,
                ZipLib_Error                                = -17,
                Bad_Password_Error                          = -18,
                Resource_Handler_Not_Specified_Error        = -19,
                Not_Implemented                             = -20,
                Stream_In_Progress                          = -21,
                User_Requested_Abort                        = -22,
                Unsupported_File_Type_Error                 = -23,
                Temp_Path_Error                             = -24,
                Incompatable_XML_Version_Error              = -25,
                Failure                                     = -26,
                Undefined                                   = -27
            };

    protected:
        Enum            m_value; /**< \brief The result value. */

    public:
        /// Default constructor (defaults to Success)
        DwfResult() : m_value ((Enum) 0) {}
        /// Copy constructor
        DwfResult(DwfResult const & in) : m_value (in.m_value) {}
        /// Constructs a new DwfResult from a DwfResult::Enum value
        DwfResult(Enum in) : m_value (in) {}
        /// Int type cast operator
        operator int() const {return (int) m_value;}
		#ifndef __APPLE_CC__
		/// HRESULT type cast operator
        operator HRESULT() const {return (HRESULT) m_value;}
		#endif 
        /// Assignment operator (copy value from another DwfResult)
        DwfResult const & operator=  (DwfResult const & in) {m_value = in.m_value; return *this;}
        /// Assignment operator (copy value from a DwfResult::Enum value)
        DwfResult const & operator=  (DwfResult::Enum in)   {m_value = in; return *this;}
		#ifndef __APPLE_CC__
        /// Assignment operator (copy value from an HRESULT.)
        /** \warning May overflow enumeration - use only valid DwfResult::Enum values! 
        */
        DwfResult const & operator=  (HRESULT in)   {m_value = Enum(in); return *this;}
		#endif
		/// Equality operator (compare with another DwfResult)
        bool operator== (DwfResult const & in) const {return m_value == in.m_value;}
        /// Equality operator (compare with a DwfResult::Enum value)
        bool operator== (DwfResult::Enum in)   const {return m_value == in;}
        /// Inequality operator (compare with another DwfResult)
        bool operator!= (DwfResult const & in) const {return m_value != in.m_value;}
        /// Inequality operator (compare with a DwfResult::Enum value)
        bool operator!= (DwfResult::Enum in)   const {return m_value != in;}

    }; //class DwfResult

}   //namespace DWFWriter

#endif //DWFRESULT_H_INCLUDED
