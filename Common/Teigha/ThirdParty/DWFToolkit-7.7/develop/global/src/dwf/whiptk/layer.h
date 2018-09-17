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
#if !defined LAYER_HEADER
#define LAYER_HEADER

///
///\file        dwf/whiptk/layer.h
///

#include "whiptk/whipcore.h"
#include "whiptk/list.h"
#include "whiptk/attribute.h"

class WT_File;

/** \addtogroup groupRenditionAttributes
 *  @{
 */

/// Rendition attribute describing a layer - a relation to a group of objects.
/** \sa WT_Rendition
 *
 */
class WHIPTK_API WT_Layer : public WT_Item, public WT_Attribute
{
	friend class WT_Rendition;
	friend class WT_Class_Factory;
	friend class WT_W2D_Class_Factory;
	friend class WT_Opcode;
	friend class WT_Layer_List;


private:
    WT_String               m_layer_name;
    WT_Integer32            m_layer_num;
    WT_Boolean              m_visibility;
    int                     m_incarnation;
    enum WT_Materialize_Stage
    {
        Gathering_Layer_Number,
        Gathering_Layer_Name,
        Eating_End_Whitespace
    } m_stage;

    void _deleteObject(void *object)
    {
        delete (WT_Layer*)object;
    }


public:

    //@{ \name Construction
    /// Constructs a WT_Layer object.
    WT_Layer()
        : m_layer_num(0)
        , m_visibility(WD_True)
        , m_incarnation(-1)
        , m_stage(Gathering_Layer_Number)
    { }
    /// Constructs a WT_Layer object wth the given data.
    WT_Layer(
        WT_File &file, /**< The file being written. */
        WT_Integer32 layer_num, /**< The layer number. */
        char const * layer_name=WD_Null /**< The optional layer name. */
        );
    /// Constructs a WT_Layer object wth the given data.
    WT_Layer(
        WT_File &file, /**< The file being written. */
        WT_Integer32 layer_num, /**< The layer number. */
        WT_Unsigned_Integer16 const * layer_name /**< The layer name. */
        );
    /// Constructs a WT_Layer object.  Copy constructor
    WT_Layer (WT_Layer const & layer);

	virtual ~WT_Layer()
	{}
    //@}
public:
    //@{ \name Data access methods
    /// Returns the layer name.
    WT_String const & layer_name() {    return m_layer_name;    }
    /// Returns the layer number.
    WT_Integer32 const& layer_num() const {    return m_layer_num; } 
	WT_Integer32&	layer_num() {    return m_layer_num; }

    /// Sets the layer name.
    void set(
        char const * layer_name, /**< The new layer name. */
        WT_File & file /**< The file being written. */
        );
    /// Sets the layer name.
    void set(
        WT_Unsigned_Integer16 const * layer_name, /**< The new layer name. */
        WT_File & file  /**< The file being written. */
        );
#ifdef WHIP_USE_WCHAR_STRINGS
    /// Sets the layer name.
    void set(
      wchar_t const * layer_name, /**< The new layer name. */
      WT_File & file  /**< The file being written. */
      );
#endif // WHIP_USE_WCHAR_STRINGS
    /// Sets the layer contents from another (copies its contents.)
    void set(WT_Layer const &layer);
    /// Sets the layer visibility.
    /** \bug The layer visibility is neither serialized nor materialized.  This option was not
     *  part of the original WHIP! spec.  Changing at this stage would be prohibitively difficult.
     *  Another workaround exists, the layer materialize / serialize code could use the rendition
     *  visibility to determine its state; however existing WHIP! data was written assuming that
     *  the layer didn't pay attention to the rendition visibility.  Were we to start doing so,
     *  many legacy drawings would be materialized has having invisible layers, not the author's
     *  intention.
     */
    void set_visibility(WT_Boolean vis) {m_visibility = vis;}
    /// Returns the layer visibility.
    WT_Boolean  visibility() {    return m_visibility; }
    /// Assignment operator.  Sets the layer contents from another (copies its contents.)
    WT_Layer const & operator=(WT_Layer const & layer);
    //@}

    //@{ \name WT_Object and WT_Attribute virtual methods
    WT_ID            object_id() const;
    WT_Result        materialize(WT_Opcode const & opcode, WT_File & file);
    WT_Result        process(WT_File & file);
    WT_Result        skip_operand(WT_Opcode const & opcode, WT_File & file);
    WT_Result        serialize (WT_File & file) const;
    WT_Result        sync (WT_File & file) const;
    WT_Boolean       operator== (WT_Attribute const & attrib) const;
    //@}

    /// Provides a default action handler for this attribute (e.g. sets the current rendition's corresponding attribute.)
    /** \warning This is used by the framework and should not be called by client code.
     */
    static WT_Result default_process(
        WT_Layer & item, /**< The attribute to process. */
        WT_File & file /**< The file being read. */
        );
};

//@}

#endif // LAYER_HEADER
