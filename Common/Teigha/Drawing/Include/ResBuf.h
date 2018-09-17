/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2018, Open Design Alliance (the "Alliance"). 
// All rights reserved. 
// 
// This software and its documentation and related materials are owned by 
// the Alliance. The software may only be incorporated into application 
// programs owned by members of the Alliance, subject to a signed 
// Membership Agreement and Supplemental Software License Agreement with the
// Alliance. The structure and organization of this software are the valuable  
// trade secrets of the Alliance and its suppliers. The software is also 
// protected by copyright law and international treaty provisions. Application  
// programs incorporating this software must include the following statement 
// with their copyright notices:
//   
//   This application incorporates Teigha(R) software pursuant to a license 
//   agreement with Open Design Alliance.
//   Teigha(R) Copyright (C) 2002-2018 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 
// acknowledge and accept the above terms.
///////////////////////////////////////////////////////////////////////////////




#if !defined(_ODRESBUF_H___INCLUDED_)
#define _ODRESBUF_H___INCLUDED_

#include "TD_PackPush.h"

#include "DbExport.h"
#include "RxObject.h"
#include "OdString.h"
#include "DbHandle.h"
#include "OdError.h"

class OdBinaryData;
class OdGePoint2d;
class OdGePoint3d;
class OdGeVector2d;
class OdGeVector3d;
class OdDbObjectId;
class OdDbDatabase;
class OdCmColor;
class OdResBuf;
class OdDbSelectionSet;

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbSelectionSet object pointers.
*/
typedef OdSmartPtr<OdDbSelectionSet> OdDbSelectionSetPtr;


/** \details
  <group Error_Classes>

  This class implements the error object which indicates the result of an operation with the 
  ResBuf object when it modifies the tagged data. An instance stores the eInvalidResBuf code.

  \sa
  TD_Db

  \sa
  <link db_rb.html, Working with Tagged Data>

  OdResBuf class
*/
class TOOLKIT_EXPORT OdError_InvalidResBuf : public OdError
{
public:
  OdError_InvalidResBuf() : OdError(eInvalidResBuf){}
};

/** \details
The typified smart pointer for the ResBuf object (tagged data). This template 
class is specialization of the OdSmartPtr class for the OdResBuf class.

\sa
<link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdResBuf> OdResBufPtr;

/** \details
  <group Other_Classes>

  This class implements ResBuf object which represents the structure for storing and passing 
  the tagged data of all basic data types between various objects using different technologies.

  \remarks
  Each ResBuf object contains three data members:
  * ResType: An integer which specifies the type of data stored in the ResBuf instance.
  * ResVal:  A container for the data stored in the ResBuf instance.
  * RbNext:  A smart pointer to the ResBuf instance to be attached in a linked list.

  \sa
  TD_Db

  \sa
  <link db_rb_sample_codes.html, Example of Entering and Displaying Tagged Data>

  <link db_rb_sample_sequence.html, Example of Working with the Sequence of Tagged Data>

  \sa
  <link db_rb.html, Working with Tagged Data>

  <link db_xdata.html, Working with Extended Data>

  <link db_sysvar.html, Working with System Variables>
*/
class TOOLKIT_EXPORT OdResBuf : public OdRxObject
{
public:

  ODRX_DECLARE_MEMBERS(OdResBuf);

  /** \details
    Destroys an instance of the ResBuf object.
  */
  ~OdResBuf();

  /** \details
    Copies the contents of the specified ResBuf object into this ResBuf object.

    \param pRb [in]  Pointer to the specified source ResBuf object to be copied.

    \remarks
    This method generates the eInvalidGroupCode exeption when the data types of both 
    ResBuf object are diffenrent or specified ResBuf value is invalid.

    \sa
    <link db_rb.html, Working with Tagged Data>
  */
  void copyFrom( const OdRxObject* pRb );


  /** \details
    Compares the specified ResBuf object with this ResBuf object whether object are equal. 

    \param Rb [in]  Reference to the specified ResBuf object to be compared.

    \remarks
    Rreturns True when group codes and data values are equal, or False when group codes 
    are different or data values are different.

    \sa
    <link db_rb.html, Working with Tagged Data>
  */
  bool operator==( const OdResBuf& Rb ) const;


  /** \details
    Compares the specified ResBuf object with this ResBuf object whether object are not equal. 

    \param Rb [in]  Reference to the specified ResBuf object to be compared.

    \returns
    Returns true when group codes are different or data values are different, or false when 
    group codes and data values are equal.

    \sa
    <link db_rb.html, Working with Tagged Data>
  */
  bool operator!=( const OdResBuf& Rb ) const;


  /** \details
    Defines the group codes for ResBuf instances.
  */
  enum ValueType
  {
    kRtNone                     = 5000,
    kRtDouble                   = 5001,
    kRtPoint2d                  = 5002,
    kRtInt16                    = 5003,
    kRtAngle                    = 5004,
    kRtString                   = 5005,

    kRtEntName                  = 5006, // setObjectId()/getEntName()/getObjectId(0)
    kRtPickSet                  = 5007,

    kRtOrient                   = 5008,

    kRtPoint3d                  = 5009,
    kRtInt32                    = 5010,
    kRtColor                    = 5011,

    kRtVoid                     = 5014,
    kRtListBeg                  = 5016, // list begin
    kRtListEnd                  = 5017, // list end
    kRtDote                     = 5018, // dot
    kRtNil                      = 5019, // nil



    kRtDXF0                     = 5020,
    kRtT                        = 5021,
    kRtResBuf                   = 5023,
//  kRtModeless                 = 5027,

    kRtBool                     =  290,
    kRtInt8                     =  280,
    kRtVector2d                 = kRtPoint2d,
    kRtVector3d                 = kRtPoint3d,
    kRtBinaryChunk              =  310,
    kRtHandle                   =  320,
    kRtObjectId                 =  330,
    kRtSoftPointerId            =  330,
    kRtHardPointerId            =  340,
    kRtSoftOwnershipId          =  350,
    kRtHardOwnershipId          =  360,

    kDxfInvalid                 = -9999,

    kDxfXDictionary             = -6,
    kDxfPReactors               = -5,
    kDxfOperator                = -4,
    kDxfXDataStart              = -3,
    kDxfHeaderId                = -2,
    kDxfFirstEntId              = -2,
    kDxfEnd                     = -1,
    kDxfStart                   = 0,
    kDxfText                    = 1,
    kDxfXRefPath                = 1,
    kDxfShapeName               = 2,
    kDxfBlockName               = 2,
    kDxfAttributeTag            = 2,
    kDxfSymbolTableName         = 2,
    kDxfMstyleName              = 2,
    kDxfSymTableRecName         = 2,
    kDxfAttributePrompt         = 3,
    kDxfDimStyleName            = 3,
    kDxfLinetypeProse           = 3,
    kDxfTextFontFile            = 3,
    kDxfDescription             = 3,
    kDxfDimPostStr              = 3,
    kDxfTextBigFontFile         = 4,
    kDxfDimAPostStr             = 4,
    kDxfCLShapeName             = 4,
    kDxfSymTableRecComments     = 4,
    kDxfHandle                  = 5,
    kDxfDimBlk                  = 5,
    kDxfDimBlk1                 = 6,
    kDxfLinetypeName            = 6,
    kDxfDimBlk2                 = 7,
    kDxfTextStyleName           = 7,
    kDxfLayerName               = 8,
    kDxfCLShapeText             = 9,
    kDxfXCoord                 = 10,
    kDxfYCoord                 = 20,
    kDxfZCoord                 = 30,
    kDxfElevation              = 38,
    kDxfThickness              = 39,
    kDxfReal                   = 40,
    kDxfViewportHeight         = 40,
    kDxfTxtSize                = 40,
    kDxfTxtStyleXScale         = 41,
    kDxfViewWidth              = 41,
    kDxfViewportAspect         = 41,
    kDxfTxtStylePSize          = 42,
    kDxfViewLensLength         = 42,
    kDxfViewFrontClip          = 43,
    kDxfViewBackClip           = 44,
    kDxfShapeXOffset           = 44,
    kDxfShapeYOffset           = 45,
    kDxfViewHeight             = 45,
    kDxfShapeScale             = 46,
    kDxfPixelScale             = 47,
    kDxfLinetypeScale          = 48,
    kDxfDashLength             = 49,
    kDxfMlineOffset            = 49,
    kDxfLinetypeElement        = 49,
    kDxfAngle                  = 50,
    kDxfViewportSnapAngle      = 50,
    kDxfViewportTwist          = 51,
    kDxfVisibility             = 60,
    kDxfLayerLinetype          = 61,
    kDxfColor                  = 62,
    kDxfHasSubentities         = 66,
    kDxfViewportVisibility     = 67,
    kDxfViewportActive         = 68,
    kDxfViewportNumber         = 69,
    kDxfInt16                  = 70,
    kDxfViewMode               = 71,
    kDxfCircleSides            = 72,
    kDxfViewportZoom           = 73,
    kDxfViewportIcon           = 74,
    kDxfViewportSnap           = 75,
    kDxfViewportGrid           = 76,
    kDxfViewportSnapStyle      = 77,
    kDxfViewportSnapPair       = 78,
    kDxfRegAppFlags            = 71,
    kDxfTxtStyleFlags          = 71,
    kDxfLinetypeAlign          = 72,
    kDxfLinetypePDC            = 73,
    kDxfInt32                  = 90,
    kDxfSubclass               = 100,
    kDxfEmbeddedObjectStart    = 101,
    kDxfControlString          = 102,
    kDxfDimVarHandle           = 105,
    kDxfUCSOrg                 = 110,
    kDxfUCSOriX                = 111,
    kDxfUCSOriY                = 112,
    kDxfXReal                  = 140,
    // 64-bit integers can only be used with version R24 and higher
    kDxfInt64                  = 160,

    kDxfXInt16                 = 170,
    kDxfNormalX                = 210,
    kDxfNormalY                = 220,
    kDxfNormalZ                = 230,
    kDxfXXInt16                = 270,
    kDxfInt8                   = 280,
    kDxfRenderMode             = 281,
    kDxfBool                   = 290,
    kDxfXTextString            = 300,
    kDxfBinaryChunk            = 310,
    kDxfArbHandle              = 320,
    kDxfSoftPointerId          = 330,
    kDxfHardPointerId          = 340,
    kDxfSoftOwnershipId        = 350,
    kDxfHardOwnershipId        = 360,  
    kDxfLineWeight             = 370,
    kDxfPlotStyleNameType      = 380,
    kDxfPlotStyleNameId        = 390,
    kDxfXXXInt16               = 400,
    kDxfLayoutName             = 410,
    kDxfComment                = 999,
    kDxfXdAsciiString          = 1000,
    kDxfRegAppName             = 1001,
    kDxfXdControlString        = 1002,
    kDxfXdLayerName            = 1003,
    kDxfXdBinaryChunk          = 1004,
    kDxfXdHandle               = 1005,
    kDxfXdXCoord               = 1010,
    kDxfXdYCoord               = 1020,
    kDxfXdZCoord               = 1030,
    kDxfXdWorldXCoord          = 1011,
    kDxfXdWorldYCoord          = 1021,
    kDxfXdWorldZCoord          = 1031,
    kDxfXdWorldXDisp           = 1012,
    kDxfXdWorldYDisp           = 1022,
    kDxfXdWorldZDisp           = 1032,
    kDxfXdWorldXDir            = 1013,
    kDxfXdWorldYDir            = 1023,
    kDxfXdWorldZDir            = 1033,
    kDxfXdReal                 = 1040,
    kDxfXdDist                 = 1041,
    kDxfXdScale                = 1042,
    kDxfXdInteger16            = 1070,
    kDxfXdInteger32            = 1071
  };


  /** \details
    Returns the group code as an Integer value which indicates the data type stored in
    the ResBuf object.
    
    \remarks
    Use the OdDxfCode::_getType() static method to determine the type of data associated 
    with the returned group code.
    
    \sa
    <link db_rb_sample_codes.html, Example of Entering and Displaying Tagged Data>

    \sa
    <link db_rb_type.html, Getting and Setting Tagged Data>

    <link db_rb_verify.html, Determining the Data Type by Group Code>

    OdResBuf::ValueType enumerator

    OdResBuf::setRestype() method
  */
  int restype() const;

  
  /** \details
    Sets the group code for the ResBuf object.
    
    \param resType [in]  Group code as an Integer value.
        
    \sa
    <link db_rb_sample_codes.html, Example of Entering and Displaying Tagged Data>

    \sa
    <link db_rb_type.html, Getting and Setting Tagged Data>

    <link db_rb_verify.html, Determining the Data Type by Group Code>

    OdResBuf::ValueType enumerator

    OdResBuf::restype() method
  */
  void setRestype( int resType );


  /** \details
    Returns a smart pointer to the next ResBuf object in the sequence of tagged data.

    \sa
    <link db_rb_sample_sequence.html, Example of Working with the Sequence of Tagged Data>

    \sa
    <link db_rb_sequence.html, Sequence of Tagged Data>

    OdResBuf::last(), OdResBuf::setNext() methods
  */
  OdResBufPtr next() const;


  /** \details
    Returns a smart pointer to the last ResBuf object in the sequence of tagged data.

    \sa
    <link db_rb_sample_sequence.html, Example of Working with the Sequence of Tagged Data>

    \sa
    <link db_rb_sequence.html, Sequence of Tagged Data>

    OdResBuf::next(), OdResBuf::setNext() methods
  */
  OdResBufPtr last() const;


  /** \details
    Inserts the specified ResBuf object before the next ResBuf object in the sequence of 
    tagged data after the current ResBuf object that calls this method.
    
    \param pRb [in]  Pointer to the existing ResBuf instance to be inserted.
    
    \returns
    Returns a smart pointer to the ResBuf object before which it was inserted.

    \sa
    <link db_rb_sample_sequence.html, Example of Working with the Sequence of Tagged Data>

    \sa
    <link db_rb_sequence.html, Sequence of Tagged Data>

    OdResBuf::next(), OdResBuf::last(), OdResBuf::setNext() methods
  */
  OdResBufPtr insert( OdResBuf* pRb );


  /** \details
    Sets the specified ResBuf object as the next ResBuf Object in the sequence of 
    tagged data after the current ResBuf object that calls this method.
    
    \param pRb [in]  Pointer to the existing ResBuf instance to be set.

    \returns
    Returns a smart pointer to the ResBuf object before which it was set.

    \sa
    <link db_rb_sample_sequence.html, Example of Working with the Sequence of Tagged Data>

    \sa
    <link db_rb_sequence.html, Sequence of Tagged Data>

    OdResBuf::next(), OdResBuf::last(), OdResBuf::insert() methods
  */
  OdResBufPtr setNext( OdResBuf* pRb );


  /** \details
    Returns the data content of the ResBuf object as a String value.

    \remarks
    This method generates the OdError_InvalidResBuf exeption when the stored data type does not 
    correspond to the String type or Name type.

    \sa
    <link db_rb_sample_codes.html, Example of Entering and Displaying Tagged Data>

    \sa
    <link db_rb_data_string.html, Getting and Setting Strings>

    <link db_rb_verify.html, Determining the Data Type by Group Code>

    OdResBuf::setString(), OdResBuf::restype(), OdDxfCode::_getType() methods
  */
  OdString getString() const;


  /** \details
    Sets the data content of the ResBuf object using the specified String value only if
    the stored group code has a String type.
      
    \param sValue [in]  String value to be set.

    \remarks
    This method generates the OdError_InvalidResBuf exeption when the stored data type does not 
    correspond to the String type or Name type.

    \sa
    <link db_rb_sample_codes.html, Example of Entering and Displaying Tagged Data>

    \sa
    <link db_rb_data_string.html, Getting and Setting Strings>

    <link db_rb_verify.html, Determining the Data Type by Group Code>

    OdResBuf::getString(), OdResBuf::setRestype(), OdDxfCode::_getType() methods
  */
  void setString( const OdString& sValue );


  /** \details
    Returns the data content of the ResBuf object as a Boolean value.

    \remarks
    This method generates the OdError_InvalidResBuf exeption when the stored data type does not 
    correspond to the Bool type. If the stored value is an Integer type, this method 
    casts it to a Boolean type and returns True when the value is non zero, or False when the
    value is zero. 

    \sa
    <link db_rb_sample_codes.html, Example of Entering and Displaying Tagged Data>

    \sa
    <link db_rb_data_typical.html, Getting and Setting Typical Data (integer, double, boolean)>

    <link db_rb_verify.html, Determining the Data Type by Group Code>

    OdResBuf::setBool(), OdResBuf::restype(), OdDxfCode::_getType() methods
  */
  bool getBool() const;


  /** \details
    Sets the data content of the ResBuf object using the specified Boolean value only if
    the stored group code has a Boolean type.
      
    \param bValue [in]  Boolean value to be set.

    \remarks
    This method generates the OdError_InvalidResBuf exeption when the stored data type does not 
    correspond to the Bool type.

    \sa
    <link db_rb_sample_codes.html, Example of Entering and Displaying Tagged Data>

    \sa
    <link db_rb_data_typical.html, Getting and Setting Typical Data (integer, double, boolean)>

    <link db_rb_verify.html, Determining the Data Type by Group Code>

    OdResBuf::getBool(), OdResBuf::setRestype(), OdDxfCode::_getType() methods
  */
  void setBool( bool bValue );


  /** \details
    Returns the data content of the ResBuf object as 8-bit Integer value.

    \remarks
    This method generates the OdError_InvalidResBuf exeption when the stored data type does not 
    correspond to the Integer8 or Integer16 type.

    \sa
    <link db_rb_sample_codes.html, Example of Entering and Displaying Tagged Data>

    \sa
    <link db_rb_data_typical.html, Getting and Setting Typical Data (integer, double, boolean)>

    <link db_rb_verify.html, Determining the Data Type by Group Code>

    OdResBuf::setInt8(), OdResBuf::restype(), OdDxfCode::_getType() methods
  */
  OdInt8 getInt8() const;


  /** \details
    Sets the data content of the ResBuf object using the specified 8-bit Integer value only if
    the stored group code has an Integer type.
      
    \param iValue [in]  8-bit Integer value to be set.

    \remarks
    This method generates the OdError_InvalidResBuf exeption when the stored data type does not 
    correspond to the Integer8 type. Use the ((OdInt8)value) instruction to convert  
    the passed value.

    \sa
    <link db_rb_sample_codes.html, Example of Entering and Displaying Tagged Data>

    \sa
    <link db_rb_data_typical.html, Getting and Setting Typical Data (integer, double, boolean)>

    <link db_rb_verify.html, Determining the Data Type by Group Code>

    OdResBuf::getInt8(), OdResBuf::setRestype(), OdDxfCode::_getType() methods
  */
  void setInt8( OdInt8 iValue );


  /** \details
    Returns the data content of the ResBuf object as 16-bit Integer value.

    \remarks
    This method generates the OdError_InvalidResBuf exeption when the stored data type does not 
    correspond to the Integer8 or Integer16 type.

    \sa
    <link db_rb_sample_codes.html, Example of Entering and Displaying Tagged Data>

    \sa
    <link db_rb_data_typical.html, Getting and Setting Typical Data (integer, double, boolean)>

    <link db_rb_verify.html, Determining the Data Type by Group Code>

    OdResBuf::setInt16(), OdResBuf::restype(), OdDxfCode::_getType() methods
  */
  OdInt16 getInt16() const;


  /** \details
    Sets the data content of the ResBuf object using the specified 16-bit Integer value only if
    the stored group code has an Integer type.
      
    \param iValue [in]  16-bit Integer value to be set.

    \remarks
    This method generates the OdError_InvalidResBuf exeption when the stored data type does not 
    correspond to the Integer16 type. Use the ((OdInt16)value) instruction to convert  
    the passed value.

    \sa
    <link db_rb_sample_codes.html, Example of Entering and Displaying Tagged Data>

    \sa
    <link db_rb_data_typical.html, Getting and Setting Typical Data (integer, double, boolean)>

    <link db_rb_verify.html, Determining the Data Type by Group Code>

    OdResBuf::getInt16(), OdResBuf::setRestype(), OdDxfCode::_getType() methods
  */
  void setInt16( OdInt16 iValue );



  /** \details
    Returns the data content of the ResBuf object as 32-bit Integer value.

    \remarks
    This method generates the OdError_InvalidResBuf exeption when the stored data type does not 
    correspond to the Integer8, Integer16, or Integer32 type.

    \sa
    <link db_rb_sample_codes.html, Example of Entering and Displaying Tagged Data>

    \sa
    <link db_rb_data_typical.html, Getting and Setting Typical Data (integer, double, boolean)>

    <link db_rb_verify.html, Determining the Data Type by Group Code>

    OdResBuf::setInt32(), OdResBuf::restype(), OdDxfCode::_getType() methods
  */
  OdInt32 getInt32() const;


  /** \details
    Sets the data content of the ResBuf object using the specified 32-bit Integer value only if
    the stored group code has an Integer type.
      
    \param iValue [in]  32-bit Integer value to be set.

    \remarks
    This method generates the OdError_InvalidResBuf exeption when the stored data type does not 
    correspond to the Integer32 type. Use the ((OdInt32)value) instruction to convert  
    the passed value.

    \sa
    <link db_rb_sample_codes.html, Example of Entering and Displaying Tagged Data>

    \sa
    <link db_rb_data_typical.html, Getting and Setting Typical Data (integer, double, boolean)>

    <link db_rb_verify.html, Determining the Data Type by Group Code>

    OdResBuf::getInt32(), OdResBuf::setRestype(), OdDxfCode::_getType() methods
  */
  void setInt32( OdInt32 iValue );


  /** \details
    Returns the data content of the ResBuf object as 64-bit Integer value.

    \remarks
    This method generates the OdError_InvalidResBuf exeption when the stored data type does not 
    correspond to the Integer8, Integer16, Integer32, or Integer64 type.

    \sa
    <link db_rb_sample_codes.html, Example of Entering and Displaying Tagged Data>

    \sa
    <link db_rb_data_typical.html, Getting and Setting Typical Data (integer, double, boolean)>

    <link db_rb_verify.html, Determining the Data Type by Group Code>

    OdResBuf::setInt64(), OdResBuf::restype(), OdDxfCode::_getType() methods
  */
  OdInt64 getInt64() const;


  /** \details
    Sets the data content of the ResBuf object using the specified 64-bit Integer value only if
    the stored group code has an Integer type.
      
    \param iValue [in]  64-bit Integer value to be set.

    \remarks
    This method generates the OdError_InvalidResBuf exeption when the stored data type does not 
    correspond to the Integer64 type. Use the ((OdInt64)value) instruction to convert  
    the passed value.

    \sa
    <link db_rb_sample_codes.html, Example of Entering and Displaying Tagged Data>

    \sa
    <link db_rb_data_typical.html, Getting and Setting Typical Data (integer, double, boolean)>

    <link db_rb_verify.html, Determining the Data Type by Group Code>

    OdResBuf::getInt64(), OdResBuf::setRestype(), OdDxfCode::_getType() methods
  */
  void setInt64( OdInt64 iValue );


  /** \details
    Returns the data content of the ResBuf object as a double-precision floating-point value.

    \remarks
    This method generates the OdError_InvalidResBuf exeption when the stored data type does not 
    correspond to the Double or Angle type. The returned value can be 
    converted to an Integer value truncating the fractional part.

    \sa
    <link db_rb_sample_codes.html, Example of Entering and Displaying Tagged Data>

    \sa
    <link db_rb_data_typical.html, Getting and Setting Typical Data (integer, double, boolean)>

    <link db_rb_verify.html, Determining the Data Type by Group Code>

    OdResBuf::setDouble(), OdResBuf::restype(), OdDxfCode::_getType() methods
  */
  double getDouble() const;


  /** \details
    Sets the data content of the ResBuf object using the specified double-precision 
    floating-point value only if the stored group code has a Double or Angle type.
      
    \param realValue [in]  Double value to be set.

    \remarks
    This method generates the OdError_InvalidResBuf exeption when the stored data type does not 
    correspond to the Double or Angle type. Use the ((double)value) 
    instruction to convert the passed value.

    \sa
    <link db_rb_sample_codes.html, Example of Entering and Displaying Tagged Data>

    \sa
    <link db_rb_data_typical.html, Getting and Setting Typical Data (integer, double, boolean)>

    <link db_rb_verify.html, Determining the Data Type by Group Code>

    OdResBuf::getDouble(), OdResBuf::setRestype(), OdDxfCode::_getType() methods
  */
  void setDouble( double realValue );


  /** \details
    Returns the data content of the ResBuf object as two-dimesional point instance.

    \remarks
    This method generates the OdError_InvalidResBuf exeption when the stored data type does not 
    correspond to the Point type. When the ResBuf object stores three-dimensional
    point or vector, this method converts it to the two-dimesional point truncating Z-coordinate.

    \sa
    <link db_rb_sample_codes.html, Example of Entering and Displaying Tagged Data>

    \sa
    <link db_rb_data_point.html, Getting and Setting Points and Vectors>

    <link db_rb_verify.html, Determining the Data Type by Group Code>

    OdResBuf::setPoint2d(), OdResBuf::restype(), OdDxfCode::_getType() methods
  */
  const OdGePoint2d& getPoint2d() const;


  /** \details
    Sets the data content of the ResBuf object using the specified two-dimesional point instance 
    only if the stored group code has a Point type.
      
    \param gePoint [in]  Two-dimesional point instance to be set.

    \remarks
    This method generates the OdError_InvalidResBuf exeption when the stored data type does not 
    correspond to the Point type. The passed value must be the OdGePoint2d instance.

    \sa
    <link db_rb_sample_codes.html, Example of Entering and Displaying Tagged Data>

    \sa
    <link db_rb_data_point.html, Getting and Setting Points and Vectors>

    <link db_rb_verify.html, Determining the Data Type by Group Code>

    OdResBuf::getPoint2d(), OdResBuf::setRestype(), OdDxfCode::_getType() methods
  */
  void setPoint2d( const OdGePoint2d& gePoint );


  /** \details
    Returns the data content of the ResBuf object as three-dimesional point instance.

    \remarks
    This method generates the OdError_InvalidResBuf exeption when the stored data type does not 
    correspond to the Point type. When the ResBuf object stores two-dimensional
    point or vector, this method converts it to the three-dimesional point adding zero Z-coordinate.

    \sa
    <link db_rb_sample_codes.html, Example of Entering and Displaying Tagged Data>

    \sa
    <link db_rb_data_point.html, Getting and Setting Points and Vectors>

    <link db_rb_verify.html, Determining the Data Type by Group Code>

    OdResBuf::setPoint3d(), OdResBuf::restype(), OdDxfCode::_getType() methods
  */
  const OdGePoint3d& getPoint3d() const;


  /** \details
    Sets the data content of the ResBuf object using the specified three-dimesional point  
    instance only if the stored group code has a Point type.
      
    \param gePoint [in]  Three-dimesional point instance to be set.

    \remarks
    This method generates the OdError_InvalidResBuf exeption when the stored data type does not 
    correspond to the Point type. The passed value must be the OdGePoint3d instance.

    \sa
    <link db_rb_sample_codes.html, Example of Entering and Displaying Tagged Data>

    \sa
    <link db_rb_data_point.html, Getting and Setting Points and Vectors>

    <link db_rb_verify.html, Determining the Data Type by Group Code>

    OdResBuf::getPoint3d(), OdResBuf::setRestype(), OdDxfCode::_getType() methods
  */
  void setPoint3d( const OdGePoint3d& gePoint );


  /** \details
    Returns the data content of the ResBuf object as two-dimesional vector instance.

    \remarks
    This method generates the OdError_InvalidResBuf exeption when the stored data type does not 
    correspond to the Point type. When the ResBuf object stores three-dimensional
    point or vector, this method converts it to the two-dimesional vector truncating Z-coordinate.

    \sa
    <link db_rb_sample_codes.html, Example of Entering and Displaying Tagged Data>

    \sa
    <link db_rb_data_point.html, Getting and Setting Points and Vectors>

    <link db_rb_verify.html, Determining the Data Type by Group Code>

    OdResBuf::setVector2d(), OdResBuf::restype(), OdDxfCode::_getType() methods
  */
  const OdGeVector2d& getVector2d() const;


  /** \details
    Sets the data content of the ResBuf object using the specified two-dimesional vector  
    instance only if the stored group code has a Point type.
      
    \param geVector [in]  Two-dimesional vector instance to be set.

    \remarks
    This method generates the OdError_InvalidResBuf exeption when the stored data type does not 
    correspond to the Point type. The passed value must be the OdGeVector2d instance.

    \sa
    <link db_rb_sample_codes.html, Example of Entering and Displaying Tagged Data>

    \sa
    <link db_rb_data_point.html, Getting and Setting Points and Vectors>

    <link db_rb_verify.html, Determining the Data Type by Group Code>

    OdResBuf::getVector2d(), OdResBuf::setRestype(), OdDxfCode::_getType() methods
  */
  void setVector2d( const OdGeVector2d& geVector );


  /** \details
    Returns the data content of the ResBuf object as three-dimesional vector instance.

    \remarks
    This method generates the OdError_InvalidResBuf exeption when the stored data type does not 
    correspond to the Point type. When the ResBuf object stores two-dimensional
    point or vector, this method converts it to the three-dimesional vector adding zero Z-coordinate.

    \sa
    <link db_rb_sample_codes.html, Example of Entering and Displaying Tagged Data>

    \sa
    <link db_rb_data_point.html, Getting and Setting Points and Vectors>

    <link db_rb_verify.html, Determining the Data Type by Group Code>

    OdResBuf::setVector3d(), OdResBuf::restype(), OdDxfCode::_getType() methods
  */
  const OdGeVector3d& getVector3d() const;


  /** \details
    Sets the data content of the ResBuf object using the specified three-dimesional vector  
    instance only if the stored group code has a Point type.
      
    \param geVector [in]  Three-dimesional vector instance to be set.

    \remarks
    This method generates the OdError_InvalidResBuf exeption when the stored data type does not 
    correspond to the Point type. The passed value must be the OdGeVector3d instance.

    \sa
    <link db_rb_sample_codes.html, Example of Entering and Displaying Tagged Data>

    \sa
    <link db_rb_data_point.html, Getting and Setting Points and Vectors>

    <link db_rb_verify.html, Determining the Data Type by Group Code>

    OdResBuf::getVector3d(), OdResBuf::setRestype(), OdDxfCode::_getType() methods
  */
  void setVector3d(const OdGeVector3d& val);


  /** \details
    Returns the data content of the ResBuf object as a Binary chunk.

    \remarks
    This method generates the OdError_InvalidResBuf exeption when the stored data type does not 
    correspond to the BinaryChunk type.

    \sa
    <link db_rb_sample_codes.html, Example of Entering and Displaying Tagged Data>

    \sa
    <link db_rb_data_other.html, Getting and Setting Specific Data (color, binary, resbuf)>

    <link db_rb_verify.html, Determining the Data Type by Group Code>

    OdResBuf::setBinaryChunk(), OdResBuf::restype(), OdDxfCode::_getType() methods

    OdBinaryData, OdUInt8 classes
  */
  const OdBinaryData& getBinaryChunk() const;


  /** \details
    Sets the data content of the ResBuf object using the specified binary chunk instance 
    only if the stored group code has the BinaryChunk type.
      
    \param bChunk [in]  Binary chunk instance to be set.

    \remarks
    This method generates the OdError_InvalidResBuf exeption when the stored data type does not 
    correspond to the BinaryChunk type. The passed value must be the OdBinaryChunk instance.

    \sa
    <link db_rb_sample_codes.html, Example of Entering and Displaying Tagged Data>

    \sa
    <link db_rb_data_other.html, Getting and Setting Specific Data (color, binary, resbuf)>

    <link db_rb_verify.html, Determining the Data Type by Group Code>

    OdResBuf::getBinaryChunk(), OdResBuf::setRestype(), OdDxfCode::_getType() methods

    OdBinaryData, OdUInt8 classes
  */
  void setBinaryChunk( const OdBinaryData& bChunk );


  /** \details
    Returns the data content of the ResBuf object as a Color instance.

    \remarks
    This method generates the OdError_InvalidResBuf exeption when the stored group code is not set 
    to kRtColor. The OdDxfCode::_getType() method returns Unknown type for it.

    \sa
    <link db_rb_sample_codes.html, Example of Entering and Displaying Tagged Data>

    \sa
    <link db_rb_data_other.html, Getting and Setting Specific Data (color, binary, resbuf)>

    <link db_rb_verify.html, Determining the Data Type by Group Code>

    <link cm.html, Colors and Transparencies>

    OdResBuf::setColor(), OdResBuf::restype() methods
  */
  const OdCmColor& getColor() const;


  /** \details
    Sets the data content of the ResBuf object using the specified Color instance 
    only if the group code is set to OdResBuf::kRtColor.
      
    \param cmColor [in]  Color instance to be set.

    \remarks
    This method generates the OdError_InvalidResBuf exeption when the stored group code is not set 
    to kRtColor. The passed value must be the OdCmColor instance. The OdDxfCode::_getType() 
    method returns Unknown type for it.

    \sa
    <link db_rb_sample_codes.html, Example of Entering and Displaying Tagged Data>

    \sa
    <link db_rb_data_other.html, Getting and Setting Specific Data (color, binary, resbuf)>

    <link db_rb_verify.html, Determining the Data Type by Group Code>

    <link cm.html, Colors and Transparencies>

    OdResBuf::getColor(), OdResBuf::setRestype() methods
  */
  void setColor( const OdCmColor& cmColor );


  /** \details
    Returns a pointer to the ResBuf instance nested in the data content of the ResBuf object.

    \remarks
    This method generates the OdError_InvalidResBuf exeption when the stored group code is not set 
    to kRtResBuf. The OdDxfCode::_getType() method returns Unknown type for it.

    \sa
    <link db_rb_sample_codes.html, Example of Entering and Displaying Tagged Data>

    \sa
    <link db_rb_data_other.html, Getting and Setting Specific Data (color, binary, resbuf)>

    OdResBuf::setResBuf(), OdResBuf::restype() methods
  */
  OdResBufPtr getResBuf() const;


  /** \details
    Sets the data content of the ResBuf object using the specified ResBuf instance 
    only if the group code is set to OdResBuf::kRtResBuf.
      
    \param pResBuf [in]  Pointer to the existing ResBuf instance to be set.

    \remarks
    This method generates the OdError_InvalidResBuf exeption when the stored group code is not set 
    to kRtResBuf. The OdDxfCode::_getType() method returns Unknown type for it.

    \sa
    <link db_rb_sample_codes.html, Example of Entering and Displaying Tagged Data>

    \sa
    <link db_rb_data_other.html, Getting and Setting Specific Data (color, binary, resbuf)>

    OdResBuf::getResBuf(), OdResBuf::setRestype() methods
  */
  void setResBuf( const OdResBuf* pResBuf );


  /** \details
    Returns the data content of the ResBuf object as an OdDbHandle instance.

    \remarks
    This method generates the OdError_InvalidResBuf exeption when the stored data type does not 
    correspond to the Handle type.

    \sa
    <link db_rb_sample_codes.html, Example of Entering and Displaying Tagged Data>

    \sa
    <link db_rb_data_object.html, Getting and Setting Objects>

    <link db_rb_verify.html, Determining the Data Type by Group Code>

    OdResBuf::setHandle(), OdResBuf::restype(), OdDxfCode::_getType() methods

    OdDbHandle, OdUInt64 classes
  */
  OdDbHandle getHandle() const;


  /** \details
    Sets the data content of the ResBuf object using the specified Handle instance 
    only if the stored group code has the Handle type.
      
    \param vHandle [in]  Handle instance to be set.

    \remarks
    This method generates the OdError_InvalidResBuf exeption when the stored data type does not 
    correspond to the Handle type. The passed value must be an instance of the OdDbHandle 
    class or OdUInt64 class to be converted to Handle type.

    \sa
    <link db_rb_sample_codes.html, Example of Entering and Displaying Tagged Data>

    \sa
    <link db_rb_data_object.html, Getting and Setting Objects>

    <link db_rb_verify.html, Determining the Data Type by Group Code>

    OdResBuf::getHandle(), OdResBuf::setRestype(), OdDxfCode::_getType() methods

    OdDbHandle, OdUInt64 classes
  */
  void setHandle( const OdDbHandle& vHandle );


  /** \details
    Returns the data content of the ResBuf object as an OdDbObjectId instance.

    \remarks
    This method generates the OdError_InvalidResBuf exeption when the stored data type does not 
    correspond to the ObjectId type. Only group codes kRtEntName and 
    kDxfEnd store the object ID, other group codes associated with ObjectId,
    SoftPointerId, HardPointerId, SoftOwnershipId, HardOwnershipId types store the handle.

    \sa
    <link db_rb_sample_codes.html, Example of Entering and Displaying Tagged Data>

    \sa
    <link db_rb_data_object.html, Getting and Setting Objects>

    <link db_rb_verify.html, Determining the Data Type by Group Code>

    OdResBuf::setObjectId(), OdResBuf::restype(), OdDxfCode::_getType() methods

    OdDbObjectId, OdDbHandle classes
  */
  const OdDbObjectId& getEntName() const;


  /** \details
    Sets the data content of the ResBuf object using the specified object ID instance 
    only if the stored group code has the ObjectId, SoftPointerId, 
    HardPointerId, SoftOwnershipId, or HardOwnershipId type.
      
    \param idObject [in]  Object ID instance to be set.

    \remarks
    This method generates the OdError_InvalidResBuf exeption when the stored data type does not 
    correspond to the ObjectId type. The passed value must be the OdDbObjectId instance.
    Only group codes kRtEntName and kDxfEnd store the object ID, other group 
    codes store the handle.

    \sa
    <link db_rb_sample_codes.html, Example of Entering and Displaying Tagged Data>

    \sa
    <link db_rb_data_object.html, Getting and Setting Objects>

    <link db_rb_verify.html, Determining the Data Type by Group Code>

    OdResBuf::getEntName(), OdResBuf::setRestype(), OdDxfCode::_getType() methods

    OdDbObjectId, OdDbHandle classes
  */
  void setObjectId( const OdDbObjectId& idObject );


  /** \details
    Returns the data content of the ResBuf object as an OdDbObjectId instance using the 
    specified database instance.
    
    \param pDb [in]  Pointer to the database object.

    \sa
    <link db_rb_sample_codes.html, Example of Entering and Displaying Tagged Data>

    \sa
    <link db_rb_data_object.html, Getting and Setting Objects>

    OdResBuf::setObjectId(), OdResBuf::restype(), OdDxfCode::_getType() methods
  */
  OdDbObjectId getObjectId( const OdDbDatabase* pDb ) const;


  /** \details
    Returns the ResVal in this ResBuf object.
  */
  OdDbSelectionSetPtr getPickSet() const;

  /** \details
    Sets the ResVal in this ResBuf object.
      
    \param pSSet [in]  selection set.

    \remarks
    OdError_InvalidResBuf if ResType does not correspond to the type of ResVal.
  */
  void setPickSet(const OdDbSelectionSet* pSSet);


  /** \details
    Create an instance of the ResBuf object of the specified data type and initialized
    the specified data value.
    
    \param resType [in]  Group code as an Integer value.   
 
    \remarks
    This method generate the OdError_InvalidResBuf exeption when the specified data value 
    does not correspond to the specified data type.

    \sa
    <link db_rb_sequence.html, Sequence of Tagged Data>

    \sa
    <link db_rb_data_object.html, Getting and Setting Objects>
  */
  static OdResBufPtr newRb(int resType = OdResBuf::kRtNone);

  /** \details
    Create an instance of the ResBuf object of the specified data type and initialized
    the specified data value.
    
    \param resType [in]  Group code as an Integer value.
    \param resVal  [in] Value of the corresponding type.    
 
    \remarks
    This method generate the OdError_InvalidResBuf exeption when the specified data value 
    does not correspond to the specified data type.

    \sa
    <link db_rb_sequence.html, Sequence of Tagged Data>

    \sa
    <link db_rb_data_object.html, Getting and Setting Objects>
  */
  static OdResBufPtr newRb(int resType, bool resVal)
  {
    OdResBufPtr pRes = newRb(resType);
    pRes->setBool(resVal);
    return pRes;
  }

  /** \details
    Create an instance of the ResBuf object of the specified data type and initialized
    the specified data value.
    
    \param resType [in]  Group code as an Integer value.
    \param resVal  [in] Value of the corresponding type.    
 
    \remarks
    This method generate the OdError_InvalidResBuf exeption when the specified data value 
    does not correspond to the specified data type.

    \sa
    <link db_rb_sequence.html, Sequence of Tagged Data>

    \sa
    <link db_rb_data_object.html, Getting and Setting Objects>
  */
  static OdResBufPtr newRb(int resType, OdInt8 resVal)
  {
    OdResBufPtr pRes = newRb(resType);
    pRes->setInt8(resVal);
    return pRes;
  }

  /** \details
    Create an instance of the ResBuf object of the specified data type and initialized
    the specified data value.
    
    \param resType [in]  Group code as an Integer value.
    \param resVal  [in] Value of the corresponding type.    
 
    \remarks
    This method generate the OdError_InvalidResBuf exeption when the specified data value 
    does not correspond to the specified data type.

    \sa
    <link db_rb_sequence.html, Sequence of Tagged Data>

    \sa
    <link db_rb_data_object.html, Getting and Setting Objects>
  */
  static OdResBufPtr newRb(int resType, OdUInt8 resVal)
  {
    OdResBufPtr pRes = newRb(resType);
    pRes->setInt8(resVal);
    return pRes;
  }

  /** \details
    Create an instance of the ResBuf object of the specified data type and initialized
    the specified data value.
    
    \param resType [in]  Group code as an Integer value.
    \param resVal  [in] Value of the corresponding type.    
 
    \remarks
    This method generate the OdError_InvalidResBuf exeption when the specified data value 
    does not correspond to the specified data type.

    \sa
    <link db_rb_sequence.html, Sequence of Tagged Data>

    \sa
    <link db_rb_data_object.html, Getting and Setting Objects>
  */
  static OdResBufPtr newRb(int resType, OdInt16 resVal)
  {
    OdResBufPtr pRes = newRb(resType);
    pRes->setInt16(resVal);
    return pRes;
  }

  /** \details
    Create an instance of the ResBuf object of the specified data type and initialized
    the specified data value.
    
    \param resType [in]  Group code as an Integer value.
    \param resVal  [in] Value of the corresponding type.    
 
    \remarks
    This method generate the OdError_InvalidResBuf exeption when the specified data value 
    does not correspond to the specified data type.

    \sa
    <link db_rb_sequence.html, Sequence of Tagged Data>

    \sa
    <link db_rb_data_object.html, Getting and Setting Objects>
  */
  static OdResBufPtr newRb(int resType, OdUInt16 resVal)
  {
    OdResBufPtr pRes = newRb(resType);
    pRes->setInt16(resVal);
    return pRes;
  }

  /** \details
    Create an instance of the ResBuf object of the specified data type and initialized
    the specified data value.
    
    \param resType [in]  Group code as an Integer value.
    \param resVal  [in] Value of the corresponding type.    
 
    \remarks
    This method generate the OdError_InvalidResBuf exeption when the specified data value 
    does not correspond to the specified data type.

    \sa
    <link db_rb_sequence.html, Sequence of Tagged Data>

    \sa
    <link db_rb_data_object.html, Getting and Setting Objects>
  */
  static OdResBufPtr newRb(int resType, OdInt32 resVal)
  {
    OdResBufPtr pRes = newRb(resType);
    pRes->setInt32(resVal);
    return pRes;
  }

  /** \details
    Create an instance of the ResBuf object of the specified data type and initialized
    the specified data value.
    
    \param resType [in]  Group code as an Integer value.
    \param resVal  [in] Value of the corresponding type.    
 
    \remarks
    This method generate the OdError_InvalidResBuf exeption when the specified data value 
    does not correspond to the specified data type.

    \sa
    <link db_rb_sequence.html, Sequence of Tagged Data>

    \sa
    <link db_rb_data_object.html, Getting and Setting Objects>
  */
  static OdResBufPtr newRb(int resType, OdUInt32 resVal)
  {
    OdResBufPtr pRes = newRb(resType);
    pRes->setInt32(resVal);
    return pRes;
  }

  /** \details
    Create an instance of the ResBuf object of the specified data type and initialized
    the specified data value.
    
    \param resType [in]  Group code as an Integer value.
    \param resVal  [in] Value of the corresponding type.    
 
    \remarks
    This method generate the OdError_InvalidResBuf exeption when the specified data value 
    does not correspond to the specified data type.

    \sa
    <link db_rb_sequence.html, Sequence of Tagged Data>

    \sa
    <link db_rb_data_object.html, Getting and Setting Objects>
  */
  static OdResBufPtr newRb(int resType, OdInt64 resVal)
  {
    OdResBufPtr pRes = newRb(resType);
    pRes->setInt64(resVal);
    return pRes;
  }

  /** \details
    Create an instance of the ResBuf object of the specified data type and initialized
    the specified data value.
    
    \param resType [in]  Group code as an Integer value.
    \param resVal  [in] Value of the corresponding type.    
 
    \remarks
    This method generate the OdError_InvalidResBuf exeption when the specified data value 
    does not correspond to the specified data type.

    \sa
    <link db_rb_sequence.html, Sequence of Tagged Data>

    \sa
    <link db_rb_data_object.html, Getting and Setting Objects>
  */
  static OdResBufPtr newRb(int resType, OdUInt64 resVal)
  {
    OdResBufPtr pRes = newRb(resType);
    pRes->setInt64(resVal);
    return pRes;
  }

  /** \details
    Create an instance of the ResBuf object of the specified data type and initialized
    the specified data value.
    
    \param resType [in]  Group code as an Integer value.
    \param resVal  [in] Value of the corresponding type.    
 
    \remarks
    This method generate the OdError_InvalidResBuf exeption when the specified data value 
    does not correspond to the specified data type.

    \sa
    <link db_rb_sequence.html, Sequence of Tagged Data>

    \sa
    <link db_rb_data_object.html, Getting and Setting Objects>
  */
  static OdResBufPtr newRb(int resType, double resVal)
  {
    OdResBufPtr pRes = newRb(resType);
    pRes->setDouble(resVal);
    return pRes;
  }

  /** \details
    Create an instance of the ResBuf object of the specified data type and initialized
    the specified data value.
    
    \param resType [in]  Group code as an Integer value.
    \param resVal  [in] Value of the corresponding type.    
 
    \remarks
    This method generate the OdError_InvalidResBuf exeption when the specified data value 
    does not correspond to the specified data type.

    \sa
    <link db_rb_sequence.html, Sequence of Tagged Data>

    \sa
    <link db_rb_data_object.html, Getting and Setting Objects>
  */
  static OdResBufPtr newRb(int resType, const OdGePoint2d& resVal)
  {
    OdResBufPtr pRes = newRb(resType);
    pRes->setPoint2d(resVal);
    return pRes;
  }

  /** \details
    Create an instance of the ResBuf object of the specified data type and initialized
    the specified data value.
    
    \param resType [in]  Group code as an Integer value.
    \param resVal  [in] Value of the corresponding type.    
 
    \remarks
    This method generate the OdError_InvalidResBuf exeption when the specified data value 
    does not correspond to the specified data type.

    \sa
    <link db_rb_sequence.html, Sequence of Tagged Data>

    \sa
    <link db_rb_data_object.html, Getting and Setting Objects>
  */
  static OdResBufPtr newRb(int resType, const OdGePoint3d& resVal)
  {
    OdResBufPtr pRes = newRb(resType);
    pRes->setPoint3d(resVal);
    return pRes;
  }

  /** \details
    Create an instance of the ResBuf object of the specified data type and initialized
    the specified data value.
    
    \param resType [in]  Group code as an Integer value.
    \param resVal  [in] Value of the corresponding type.    
 
    \remarks
    This method generate the OdError_InvalidResBuf exeption when the specified data value 
    does not correspond to the specified data type.

    \sa
    <link db_rb_sequence.html, Sequence of Tagged Data>

    \sa
    <link db_rb_data_object.html, Getting and Setting Objects>
  */
  static OdResBufPtr newRb(int resType, const OdGeVector2d& resVal)
  {
    OdResBufPtr pRes = newRb(resType);
    pRes->setVector2d(resVal);
    return pRes;
  }

  /** \details
    Create an instance of the ResBuf object of the specified data type and initialized
    the specified data value.
    
    \param resType [in]  Group code as an Integer value.
    \param resVal  [in] Value of the corresponding type.    
 
    \remarks
    This method generate the OdError_InvalidResBuf exeption when the specified data value 
    does not correspond to the specified data type.

    \sa
    <link db_rb_sequence.html, Sequence of Tagged Data>

    \sa
    <link db_rb_data_object.html, Getting and Setting Objects>
  */
  static OdResBufPtr newRb(int resType, const OdGeVector3d& resVal)
  {
    OdResBufPtr pRes = newRb(resType);
    pRes->setVector3d(resVal);
    return pRes;
  }

  /** \details
    Create an instance of the ResBuf object of the specified data type and initialized
    the specified data value.
    
    \param resType [in]  Group code as an Integer value.
    \param resVal  [in] Value of the corresponding type.    
 
    \remarks
    This method generate the OdError_InvalidResBuf exeption when the specified data value 
    does not correspond to the specified data type.

    \sa
    <link db_rb_sequence.html, Sequence of Tagged Data>

    \sa
    <link db_rb_data_object.html, Getting and Setting Objects>
  */
  static OdResBufPtr newRb(int resType, const OdString& resVal)
  {
    OdResBufPtr pRes = newRb(resType);
    pRes->setString(resVal);
    return pRes;
  }

  /** \details
    Create an instance of the ResBuf object of the specified data type and initialized
    the specified data value.
    
    \param resType [in]  Group code as an Integer value.
    \param resVal  [in] Value of the corresponding type.    
 
    \remarks
    This method generate the OdError_InvalidResBuf exeption when the specified data value 
    does not correspond to the specified data type.

    \sa
    <link db_rb_sequence.html, Sequence of Tagged Data>

    \sa
    <link db_rb_data_object.html, Getting and Setting Objects>
  */
  static OdResBufPtr newRb(int resType, const OdChar* resVal)
  {
    OdResBufPtr pRes = newRb(resType);
    pRes->setString(resVal);
    return pRes;
  }

#if defined(OD_WINDOWS_DESKTOP) && defined(_MSC_VER)
  /** \details
    Create an instance of the ResBuf object of the specified data type and initialized
    the specified data value.
    
    \param resType [in]  Group code as an Integer value.
    \param resVal  [in] Value of the corresponding type.    
 
    \remarks
    This method generate the OdError_InvalidResBuf exeption when the specified data value 
    does not correspond to the specified data type.

    \sa
    <link db_rb_sequence.html, Sequence of Tagged Data>

    \sa
    <link db_rb_data_object.html, Getting and Setting Objects>
  */
  static OdResBufPtr newRb(int resType, const __wchar_t* resVal)
  {
    OdResBufPtr pRes = newRb(resType);
    pRes->setString(resVal);
    return pRes;
  }
#endif

/** \details
    Create an instance of the ResBuf object of the specified data type and initialized
    the specified data value.
    
    \param resType [in]  Group code as an Integer value.
    \param resVal  [in] Value of the corresponding type.    
 
    \remarks
    This method generate the OdError_InvalidResBuf exeption when the specified data value 
    does not correspond to the specified data type.

    \sa
    <link db_rb_sequence.html, Sequence of Tagged Data>

    \sa
    <link db_rb_data_object.html, Getting and Setting Objects>
  */
  static OdResBufPtr newRb(int resType, const OdCmColor& resVal)
  {
    OdResBufPtr pRes = newRb(resType);
    pRes->setColor(resVal);
    return pRes;
  }

  /** \details
    Create an instance of the ResBuf object of the specified data type and initialized
    the specified data value.
    
    \param resType [in]  Group code as an Integer value.
    \param resVal  [in] Value of the corresponding type.    
 
    \remarks
    This method generate the OdError_InvalidResBuf exeption when the specified data value 
    does not correspond to the specified data type.

    \sa
    <link db_rb_sequence.html, Sequence of Tagged Data>

    \sa
    <link db_rb_data_object.html, Getting and Setting Objects>
  */
  static OdResBufPtr newRb(int resType, const OdDbObjectId& id)
  {
    OdResBufPtr pRes = newRb(resType);
    pRes->setObjectId(id);
    return pRes;
  }

  /** \details
    Create an instance of the ResBuf object of the specified data type and initialized
    the specified data value.
    
    \param resType [in]  Group code as an Integer value.
    \param resVal  [in] Value of the corresponding type.    
 
    \remarks
    This method generate the OdError_InvalidResBuf exeption when the specified data value 
    does not correspond to the specified data type.

    \sa
    <link db_rb_sequence.html, Sequence of Tagged Data>

    \sa
    <link db_rb_data_object.html, Getting and Setting Objects>
  */
  static OdResBufPtr newRb(int resType, const OdDbSelectionSet* pSSet)
  {
    OdResBufPtr pRes = newRb(resType);
    pRes->setPickSet(pSSet);
    return pRes;
  }

  /** \details
    Create an instance of the ResBuf object of the specified data type and initialized
    the specified data value.
    
    \param resType [in]  Group code as an Integer value.
    \param resVal  [in] Value of the corresponding type.    
 
    \remarks
    This method generate the OdError_InvalidResBuf exeption when the specified data value 
    does not correspond to the specified data type.

    \sa
    <link db_rb_sequence.html, Sequence of Tagged Data>

    \sa
    <link db_rb_data_object.html, Getting and Setting Objects>
  */
  static OdResBufPtr newRb(int resType, const OdResBuf* pNestedRb)
  {
    OdResBufPtr pRes = newRb(resType);
    pRes->setResBuf(pNestedRb);
    return pRes;
  }

  /*!DOM*/ // is used to get assertion with needed call stack only
  static void setAssertIndexByNewRb(int index);

  /** \details
    Defines the content of the ResBuf object.
  */
  union Data
  {
    bool    Bool;                     // Boolean content
    OdInt16 Int16;                    // 8-bit or 16-bit Integer content
    OdInt32 Int32;                    // 32-bit Integer or Color content
    OdInt64 Int64;                    // 64-bit Integer or Handle content  
    double  Double;                   // double-precision floating-point content
    void*   Pointer;                  // pointer to an object
    OdUInt8 Bytes[sizeof(OdInt64)];   // Content as an array of bytes
  };

protected:
  /** \details
    Builds an instance of the ResBuf object.
    
    \remarks
    Use the newRb() static pseudo-constructor instead it.
    See: <link db_rb_sequence.html, Sequence of Tagged Data>
  */
  OdResBuf();

  int m_nCode;           // Group code that defines the data type
  Data m_data;           // Value that defines the content
  OdResBufPtr m_pNext;   // Smart pointer to another instance for attaching in a sequence
};

inline bool
OdResBuf::operator!=( const OdResBuf& other ) const
{
  return !operator==( other );
}

#include "TD_PackPop.h"

#endif //_ODRESBUF_H___INCLUDED_

