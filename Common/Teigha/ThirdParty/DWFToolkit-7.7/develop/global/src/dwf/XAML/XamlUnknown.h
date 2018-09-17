//
//  Copyright (c) 2006 by Autodesk, Inc.
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

#if !defined XAML_UNKNOWN_HEADER
#define XAML_UNKNOWN_HEADER

///
///\file        XAML/XamlUnknown.h
///

#include "XAML/XamlCore.h"

#include "whiptk/unknown.h"

/// An internal class used for unknown objects.
/** Note, the data buffer contained in the object is allocated and populated in the materialize() method. */
class XAMLTK_API WT_XAML_Unknown : public WT_Unknown
{
    friend class WT_XAML_Class_Factory;
protected:
	//@{ \name Construction / destruction
    /// Constructs a WT_XAML_Unknown object.
    WT_XAML_Unknown()
		: WT_Unknown()
	{}
    
    /// Destroys a v object.
    virtual ~WT_XAML_Unknown()
	{}
public:
	 //@{ \name WT_Object virtual methods
	WT_Result        serialize (WT_File & file) const;
    /// partial materialization of shell from W2X
	WT_Result		 parseAttributeList(XamlXML::tAttributeMap& rMap, WT_XAML_File& rFile);
    //@}
};

#endif //XAML_UNKNOWN_HEADER
