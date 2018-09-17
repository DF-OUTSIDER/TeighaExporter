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

#if !defined XAML_DWF_HEADER
#define XAML_DWF_HEADER

///
///\file        XAML/XamlDwfHeader.h
///

#include "XAML/XamlCore.h"

#include "whiptk/dwfhead.h"

/// An implementation for the DWF header object, used by clients to signify the beginning of the DWF stream.
class XAMLTK_API WT_XAML_DWF_Header : public WT_DWF_Header
{
    friend class WT_XAML_Class_Factory;

protected:
	//@{ \name Construction / destruction
    /// Constructs a WT_XAML_DWF_Header object.
    WT_XAML_DWF_Header()
		: WT_DWF_Header()
	{}
    
    /// Destroys the object.
    virtual ~WT_XAML_DWF_Header()
	{}
public:
	 //@{ \name WT_Object virtual methods
    WT_Result        serialize (WT_File &) const;
    //@}

    WT_Result        parseAttributeList(XamlXML::tAttributeMap& rMap, WT_XAML_File& /*rFile*/);


};

#endif //XAML_DWF_HEADER
