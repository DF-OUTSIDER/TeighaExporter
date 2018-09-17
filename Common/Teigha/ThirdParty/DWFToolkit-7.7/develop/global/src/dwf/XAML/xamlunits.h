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
#if !defined XAML_UNITS
#define XAML_UNITS


#include "whiptk/units.h"

class WT_XAML_Units :
    public WT_Units
{
public:
    WT_XAML_Units(void) : WT_Units() {}
    WT_XAML_Units(WT_Matrix const & xform, WT_String units) : WT_Units(xform, units) {}
public:
    virtual ~WT_XAML_Units(void);

    WT_Result   serialize(WT_File & file) const;
	/// partial materialization of shell from W2X
	WT_Result   parseAttributeList(XamlXML::tAttributeMap& rMap, WT_XAML_File& rFile);
};

#endif //XAML_UNITS
