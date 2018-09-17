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

#if !defined VIEWPORT_OPTIONS_HEADER
#define VIEWPORT_OPTIONS_HEADER

///
///\file        dwf/whiptk/viewport_options.h
///

#include "whiptk/units.h"
#include "whiptk/opcode.h"

/// Provides an ID for viewport option objects.
class WHIPTK_API WT_Viewport_Optioncode : public WT_Optioncode
{
    friend class WT_Viewport;
    
protected:
    WT_Viewport_Optioncode() 
    {}

public:
    /// Viewport option identifiers.
    enum
    {   Viewport_Unknown_Option,
        Viewport_Units_Option
    };

    /// Returns the option ID of the viewport option.
    int option_id_from_optioncode();
};

/// Contains units information corresponding to a viewport
class WHIPTK_API WT_Viewport_Option_Viewport_Units : public WT_Option, public WT_Units
{
	friend class WT_Viewport;
	friend class WT_W2D_Class_Factory;
	friend class WT_Class_Factory;

protected:

    /// Constructs a WT_Viewport_Option_Viewport_Units object.
    WT_Viewport_Option_Viewport_Units() 
    {}
	virtual ~WT_Viewport_Option_Viewport_Units()
	{}
public:
    //@{ \name WT_Option virtual methods
    WT_ID          object_id() const;
    WT_Result      serialize  (WT_Object const & parent, WT_File & file) const;
    WT_Result      materialize(WT_Object       & parent, WT_Optioncode const & optioncode, WT_File & file);
    //@}

    /// Returns WD_True if the given viewport units object is equal to this one.
    WT_Boolean     operator==(WT_Viewport_Option_Viewport_Units const &) const;
    /// Returns WD_True if the given viewport units object is NOT equal to this one.
    WT_Boolean     operator!=(WT_Viewport_Option_Viewport_Units const &) const;

};

#endif // VIEWPORT_OPTIONS_HEADER
