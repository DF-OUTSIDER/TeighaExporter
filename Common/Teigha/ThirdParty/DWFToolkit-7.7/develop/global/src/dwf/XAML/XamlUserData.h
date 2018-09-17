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

#if !defined XAML_USER_DATA_HEADER
#define XAML_USER_DATA_HEADER

///
///\file        XAML/xamlUserData.h
///

#include "XAML/XamlCore.h"

#include "whiptk/userdata.h"
#include "XAML/XamlXML.h"

/** \addtogroup groupSupportObjects Support objects
 *
 * @{
 */

/// An object containing user-defined data.
/** This data is generally ignored by any rendering applications which are not
 *  specifically designed to recognize the user-defined data.
 */
class XAMLTK_API WT_XAML_UserData : public WT_UserData
{
	friend class WT_XAML_Class_Factory;
protected:

    //@{ \name Construction / destruction
    /// Constructs a WT_XAML_UserData object.
    WT_XAML_UserData ()
         : WT_UserData()
    { }
    /// Constructs a WT_XAML_UserData object with the given description.
    WT_XAML_UserData (const WT_String & data_description)
      : WT_UserData(data_description)
     { }
    /// Constructs a WT_XAML_UserData object with the given data.
    WT_XAML_UserData(
            const WT_String &           data_description, /**< Description of the user-defined data. */
            WT_Integer32                data_size, /**< Size of the \a data array in bytes. */
            WT_Byte *                   data, /**< Byte buffer containing user-defined data. */
            WT_Boolean                  copy /**< Whether the data should be copied or if its content should be used directly from the array. */
            ) throw(WT_Result)
		: WT_UserData(data_description,data_size,data,copy)
	{}
    
	WT_XAML_UserData & operator= (WT_XAML_UserData const &)
    {
        WD_Complain ("cannot assign WT_XAML_UserData");
        return *this;
    }

	/// Destroys a WT_XAML_UserData object.
    virtual ~WT_XAML_UserData()
    {}
    //@}
public:	

	 //@{ \name WT_Object virtual methods
    WT_Result        serialize(WT_File & file) const;
	/// partial materialization of shell from W2X
	WT_Result		 parseAttributeList(XamlXML::tAttributeMap& rMap, WT_XAML_File& rFile);
    WT_Result        parseCData(int nLen,const char* pCData);
    //@}
};
//@}

#endif //XAML_USER_DATA_HEADER
