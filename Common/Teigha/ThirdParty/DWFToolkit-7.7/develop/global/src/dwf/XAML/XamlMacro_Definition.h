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
///\file        XAML/XamlMacro_Definition.h
///

#if !defined XAML_MACRO_DEFINITION_HEADER
#define XAML_MACRO_DEFINITION_HEADER

#include "XAML/XamlCore.h"

#include "whiptk/macro_definition.h"
#include "XAML/XamlXML.h"
#include "dwfcore/BufferInputStream.h"
#include "dwfcore/BufferOutputStream.h"


class XAMLTK_API WT_XAML_Macro_Definition : public WT_Macro_Definition
{
	friend class WT_XAML_Class_Factory;
	friend class WT_XAML_File;
	
protected:

    /// Constructors and destructor
    // Default constructor
    WT_XAML_Macro_Definition()
        : WT_Macro_Definition (0)
    {}

    WT_XAML_Macro_Definition( const WT_XAML_Macro_Definition& rMacro );

    WT_XAML_Macro_Definition( WT_Unsigned_Integer16 index,
                         WT_Integer32 scale_units = 1 )
	 : WT_Macro_Definition(index,scale_units)
	{} 

    virtual ~WT_XAML_Macro_Definition()
	{}

public:   
	 /// Implementation of the WT_Object interface
    WT_Result       serialize(WT_File & file) const;
    WT_Result       serialize_stream(WT_File & file) const;
	/// partial materialization of shell from W2X
	WT_Result		 parseAttributeList(XamlXML::tAttributeMap& rMap, WT_XAML_File& rFile);
    WT_Result        parseCData(int nLen,const char* pCData);

};

#endif //XAML_MACRO_DEFINITION_HEADER
