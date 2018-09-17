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
#if !defined WT_XAML_POLYLMARKER
#define WT_XAML_POLYMARKER

///
///\file        XAML/XamlPolymarker.h
///

#include "whiptk/polymark.h"
#include "XAML/XamlPath.h"
#include "XAML/XamlPointset.h"
#include "XAML/XamlXML.h"

class WT_XAML_Polymarker : public WT_Polymarker, public WT_XAML_Point_Set_Data
{
    friend class WT_XAML_Class_Factory;
    friend class XamlPath;
	friend class WT_XAML_File;

protected:
    WT_XAML_Polymarker(void) 
		: WT_Polymarker()
	{}

    virtual ~WT_XAML_Polymarker(void) 
	{}

    /// Constructs a WT_Polymarker object from the given data.
    WT_XAML_Polymarker(
        int                         count, /**< The number of points in the array. */
        WT_Logical_Point const *    points, /**< Pointer to the array of points. */
        WT_Boolean                  copy /**< Whether the points should be copied or if their addresses should be used directly from the array. */
        ) throw(WT_Result)
        : WT_Polymarker(count, points, copy)
    { }

    /// Constructs a WT_Polymarker object. Copy constructor.
    WT_XAML_Polymarker(const WT_Polymarker & pMarker) throw(WT_Result)
        : WT_Polymarker (pMarker)
    { }

private:
    WT_Result _createSegments( WT_XAML_File& rFile, XamlPath* rPath, WT_XAML_Point_Set_Data& rPointSet ) const;

public:

	/// Assignment operator.  Assigns the data from the given polymarker to this one.
    /** \note If the incoming polymarker's points are not copied, i.e. it is directly
     *  using client memory, then the new polymarker will do the same.
     */
    WT_XAML_Polymarker const & operator= (const WT_XAML_Polymarker & pMarker) throw(WT_Result)
    {
		WT_Polymarker::operator = (pMarker);
        return *this;
    }

    virtual WT_Result   serialize(WT_File & file) const;
	
};

#endif //WT_XAML_POLYMARKER
