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
#if !defined WT_XAML_POLYGON
#define WT_XAML_POLYGON

///
///\file        XAML/XamlPolygon.h
///

#include "whiptk/polygon.h"
#include "XAML/XamlPointset.h"

class WT_XAML_Polygon : public WT_Polygon, public WT_XAML_Point_Set_Data
{
	friend class WT_XAML_Class_Factory;
	friend class WT_XAML_File;

protected:
    WT_XAML_Polygon(void)
		:WT_Polygon()
	{}
    WT_XAML_Polygon(int count, WT_Logical_Point const *points, WT_Boolean copy) throw(WT_Result)
		: WT_Polygon(count, points, copy) 
	{}
    
	WT_XAML_Polygon(const WT_Polygon& polygon) throw(WT_Result)
		: WT_Polygon(polygon) 
	{}
    
	virtual ~WT_XAML_Polygon(void)
	{}

public:

	/// Assignment operator.  Assigns the data from the given polygon to this one.
    /** \note If the incoming polygon's points are not copied, i.e. it is directly
     *  using client memory, then the new polygon will do the same.
     */
    WT_XAML_Polygon const & operator= (const WT_XAML_Polygon & polygon) throw(WT_Result)
    {
		WT_Polygon::operator = (polygon);
        return *this;


    }
	//@{ \name WT_Object virtual methods
    virtual WT_Result   serialize(WT_File & file) const;
    //@}
};

#endif //WT_XAML_POLYGON
