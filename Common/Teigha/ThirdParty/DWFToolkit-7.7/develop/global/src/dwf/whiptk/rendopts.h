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

#if !defined RENDOPTS_HEADER
#define RENDOPTS_HEADER

///
///\file        dwf/whiptk/rendopts.h
///

#include "whiptk/whipcore.h"
#include "whiptk/backgrnd.h"
#include "whiptk/view.h"
#include "whiptk/plot_optimized.h"
#include "whiptk/inked_area.h"
#include "whiptk/penpat_options.h"

#define WHIP_RENDERING_OPTION_LIST\
    WHIP_RENDERING_OPTION(Background,background)\
    WHIP_RENDERING_OPTION(Inked_Area,inked_area)\
    WHIP_RENDERING_OPTION(PenPat_Options,penpat_options)\
    WHIP_RENDERING_OPTION(Plot_Optimized,plot_optimized)\
    WHIP_RENDERING_OPTION(View,view)

/** \addtogroup groupRenderingAttrobutes Rendering attributes
 *  Contains objects which describe drawing details of interest to a rendering application.
 *  \sa WT_Rendition, WT_Rendering_Options
 *  @{
 *  @}
 */

/** \addtogroup groupFileIO
 *  @{
 */

/// Options stored in the rendition which describe drawing details of interest to a rendering application.
/** \sa WT_Rendition, \ref groupRenderingAttrobutes "Rendering attributes" */
class WHIPTK_API WT_Rendering_Options
{
protected:
    WT_Integer32            m_changed_flags;

private:
#define WHIP_RENDERING_OPTION(class_type, class_lower) \
    WT_##class_type            m_##class_lower;
    WHIP_RENDERING_OPTION_LIST
#undef WHIP_RENDERING_OPTION

    WT_Result sync_parts(WT_File & file, WT_Integer32 needed);

public:

    /// Used in identifying which rendering options are applicable to a given object and/or which have changed and need to be serialized if needed.
    enum WT_Rendering_Option_Bits
    {
        // For speed's sake, these should be ordered from most oftenly changed
        // to least oftenly changed...
        View_Bit            = 0x00000001,
        Background_Bit      = 0x00000002,
        Plot_Optimized_Bit  = 0x00000004,
        Inked_Area_Bit      = 0x00000008,
        PenPat_Options_Bit  = 0x00000010
    };

    //@{ \name Construction
    /// Constructs a WT_Rendering_Options object.
    WT_Rendering_Options()
        : m_changed_flags(0)
    { }
    virtual ~WT_Rendering_Options()
    {}

    /// Copies a WT_Rendering_Options object from another.
    WT_Rendering_Options (WT_Rendering_Options const & other)
        : m_changed_flags     (other.m_changed_flags)
        #define WHIP_RENDERING_OPTION(class_name, class_lower) \
            , m_##class_lower (other.m_##class_lower)
            WHIP_RENDERING_OPTION_LIST
        #undef WHIP_RENDERING_OPTION
    { }
    //@}

    /// Assigns the contents of a WT_Rendering_Options from another.
    WT_Rendering_Options & operator= (WT_Rendering_Options const & other)
    {
        m_changed_flags     = 0x7FFFFF; //

        #define WHIP_RENDERING_OPTION(class_name, class_lower) \
            m_##class_lower = other.m_##class_lower;
            WHIP_RENDERING_OPTION_LIST
        #undef WHIP_RENDERING_OPTION

        return *this;
    }


    /** \name Rendering option read-only accessor methods
     *  The following methods provide read-only access to the rendering options.
     *  \sa \ref groupRenderingAttrobutes "Rendering attributes"
     *  @{
     */
    #define WHIP_RENDERING_OPTION(class_type, class_lower) \
        WT_##class_type const & class_lower() const  { return m_##class_lower; }
        WHIP_RENDERING_OPTION_LIST
    #undef WHIP_RENDERING_OPTION
    //@}

    /** \name Rendering option accessor methods
     *  The following methods provide writable access to the rendering options.
     *  \sa \ref groupRenderingAttrobutes "Rendering attributes"
     *  @{
     */
    #define WHIP_RENDERING_OPTION(class_type, class_lower) \
        WT_##class_type & class_lower() { m_changed_flags |= class_type##_Bit; return m_##class_lower; }
        WHIP_RENDERING_OPTION_LIST
    #undef WHIP_RENDERING_OPTION
    //@}

    /// Serializes pending changed rendering option settings to the file, if necessary.
    /** Only outputs relevant desired rendering option changes to the file.
     *  \warning This is used by the framework and should not be called by client code.
     *  \retval WT_Result::Success The operation was successful.
     */
    virtual WT_Result sync(WT_File & file)
    {
        if (m_changed_flags)
        {
            WT_Integer32    needed = m_changed_flags;

            m_changed_flags = 0;
            return sync_parts(file, needed);
        }

        return WT_Result::Success;
    }
};

//@}

#endif // RENDOPTS_HEADER
