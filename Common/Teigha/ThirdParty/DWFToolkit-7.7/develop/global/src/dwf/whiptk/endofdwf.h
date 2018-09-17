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

#if !defined ENDOFDWF_HEADER
#define ENDOFDWF_HEADER

///
///\file        dwf/whiptk/endofdwf.h
///

#include "whiptk/whipcore.h"
#include "whiptk/object.h"

class WT_File;

/** \addtogroup groupSupportObjects Support objects
 *
 * @{
 */

/// Indicates the end of the WHIP! data.
/** \note Internal use only - not intended for direct use by client code. */
class WHIPTK_API WT_End_Of_DWF : public WT_Object
{
	friend class WT_W2D_Class_Factory;
	friend class WT_Opcode;
	friend class WT_File;

public:

    //@{ \name Construction
    /// Constructs a WT_End_Of_DWF object.
    WT_End_Of_DWF()
    { }
    //@}
	virtual ~WT_End_Of_DWF()
	{}
public:
    //@{ \name WT_Object virtual methods
    WT_Result        materialize(WT_Opcode const & opcode, WT_File & file);
    WT_ID            object_id() const;
    WT_Type          object_type() const;
    WT_Result        process(WT_File & file);
    WT_Result        serialize(WT_File & file) const;
    WT_Result        skip_operand(WT_Opcode const & opcode, WT_File & file);
    //@}

    /// Provides a default action handler for this object.
    /** \warning This is used by the framework and should not be called by client code.
     */
    static WT_Result default_process(
        WT_End_Of_DWF & item, /**< The object to process. */
        WT_File & file /**< The file being read. */
        );
};

//@}

#endif // ENDOFDWF_HEADER
