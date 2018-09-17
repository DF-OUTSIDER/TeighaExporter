//
//  Copyright (c) 2007 by Autodesk, Inc.
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

#if !defined XAML_END_OF_DWF
#define XAML_END_OF_DWF

///
///\file        XAML/XamlEndOfDwf.h
///

#include "XAML/XamlCore.h"

#include "whiptk/endofdwf.h"

/// An implementation for the End of Dwf object, used by clients to signify the end of the stream.
class XAMLTK_API WT_XAML_End_Of_DWF : public WT_End_Of_DWF
{
    friend class WT_XAML_Class_Factory;
protected:
	//@{ \name Construction / destruction
    /// Constructs a WT_XAML_End_Of_DWF object.
    WT_XAML_End_Of_DWF()
		: WT_End_Of_DWF()
	{}
    
    /// Destroys the object.
    virtual ~WT_XAML_End_Of_DWF()
	{}
public:
	 //@{ \name WT_Object virtual methods
    WT_Result        serialize (WT_File &) const { return WT_Result::Toolkit_Usage_Error; }
    //@}
};

#endif //XAML_END_OF_DWF
