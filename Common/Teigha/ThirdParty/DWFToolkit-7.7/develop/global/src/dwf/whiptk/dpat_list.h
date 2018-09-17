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

#if !defined DPAT_LIST_HEADER
#define DPAT_LIST_HEADER

///
///\file        dwf/whiptk/dpat_list.h
///

#include "whiptk/whipcore.h"
#include "whiptk/list.h"
#include "whiptk/dashpat.h"

/// A linked list of WT_Dash_Pattern objects.
class WHIPTK_API WT_Dash_Pattern_List
: public WT_Item_List
{
	friend class WT_File;
	friend class WT_W2D_Class_Factory;

protected:

	WT_Dash_Pattern_List()
		:WT_Item_List()
	{}

	WT_Dash_Pattern_List(WT_Dash_Pattern* pPat)
		:WT_Item_List(pPat)
	{}

    /// Destroys a WT_Dash_Pattern_List object.
    virtual ~WT_Dash_Pattern_List()
    {
        remove_all();
    }
public:
    /// Adds the given WT_Dash_Pattern to the list.
    WT_Dash_Pattern* add_pattern( WT_Dash_Pattern const & r );

    /// Returns the WT_Dash_Pattern with the associated identification number (NULL if not found.)
    WT_Dash_Pattern* find_pattern( WT_Integer32 id_number );


};

#endif // DPAT_LIST_HEADER
