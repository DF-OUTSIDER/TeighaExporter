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

///
///\file        XAML/XamlOverpost.h
///

#if !defined XAML_OVERPOST_HEADER
#define XAML_OVERPOST_HEADER

#include "XAML/XamlCore.h"

#include "whiptk/overpost.h"
#include "XAML/XamlXML.h"

class XAMLTK_API WT_XAML_Overpost : public WT_Overpost
{
	friend class WT_XAML_Class_Factory;
protected:
    // Constructors and destructor
    WT_XAML_Overpost()
		: WT_Overpost()
	{}
    WT_XAML_Overpost( WT_AcceptMode  acceptMode,
                 WT_Boolean     renderEntities,
                 WT_Boolean     addExtents)
		: WT_Overpost(acceptMode,renderEntities,addExtents)
	{}

    virtual ~WT_XAML_Overpost()
	{}

public:
	/// Implementation of the WT_Object interface
    WT_Result           serialize(WT_File & /*file*/) const;
    WT_Result           serialize_stream(WT_File & file) const;
	/// partial materialization of shell from W2X
	WT_Result		    parseAttributeList(XamlXML::tAttributeMap& rMap, WT_XAML_File& rFile);
    WT_Result           parseCData(int nLen,const char* pCData);
};

#endif //XAML_OVERPOST_HEADER
