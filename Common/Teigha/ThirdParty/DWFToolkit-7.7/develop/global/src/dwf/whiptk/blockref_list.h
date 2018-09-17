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

#if !defined BLOCKREF_LIST_HEADER
#define BLOCKREF_LIST_HEADER

///
///\file        dwf/whiptk/blockref_list.h
///

#include "whiptk/whipcore.h"
#include "whiptk/attribute.h"

/// A linked list of WT_BlockRef objects.
class WHIPTK_API WT_BlockRef_List : public WT_Item_List
{
	friend class WT_SignData;
protected:
    /// Constructs a WT_BlockRef_List object.
    WT_BlockRef_List()
    { }

    /// Constructs a WT_BlockRef_List object.  Copy constructor.
    WT_BlockRef_List(
        const WT_BlockRef_List & list
        )
        : WT_Item_List()
    {
        *this = list;
    }

    /// Destroys a WT_BlockRef_List object.
    virtual ~WT_BlockRef_List();

public:
    /// Adds a WT_BlockRef item to the list.
    void add (WT_BlockRef &item);

    /// Assignment operator.  Assigns the given list contents to this one.
    WT_BlockRef_List const & operator=(WT_BlockRef_List const & list);

    /// Equality operator.  Returns WD_True if the given list's contents is equal to this one.
    WT_Boolean operator== (WT_BlockRef_List const & list) const;

};

#endif // BLOCKREF_LIST_HEADER
