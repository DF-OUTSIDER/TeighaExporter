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

#if !defined COMPDATA_HEADER
#define COMPDATA_HEADER

///
///\file        dwf/whiptk/compdata.h
///

#include "whiptk/whipcore.h"
#include "whiptk/object.h"
#include "whiptk/wversion.h"

class WT_File;

/** \addtogroup groupSupportObjects Support objects
 *
 * @{
 */

/// Indicates the start of a compressed block of data.  For use by the compression classes.
/** \note Internal use only - not intended for direct use by client code. */
class WHIPTK_API WT_Compressed_Data_Moniker : public WT_Object
{
	friend class WT_W2D_Class_Factory;
	friend class WT_PDFClassFactory;
	friend class WT_LZ_Compressor;
	friend class WT_ZLib_Compressor;
	friend class WT_Opcode;

private:
    int m_compression_format;

public:


    //@{ \name Construction
    /// Constructs a WT_Compressed_Data_Moniker object.
    WT_Compressed_Data_Moniker(int format = REVISION_WHEN_ZLIB_COMPRESSION_IS_SUPPORTED)
        : m_compression_format(format)
    { }
    /// Destroys a WT_Compressed_Data_Moniker object.
    virtual ~WT_Compressed_Data_Moniker()
    { }
    //@}
public:
    //@{ \name Data access methods
    /// Returns the compression format, either WD_ZLIB_COMPRESSION_EXT_OPCODE or WD_LZ_COMPRESSION_EXT_OPCODE.
    const int compression_format() const { return m_compression_format; }
    //@}

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
        WT_Compressed_Data_Moniker & item, /**< The object to process. */
        WT_File & file /**< The file being read. */
        );

};

//@}

#endif // COMPDATA_HEADER
