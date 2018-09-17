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

#ifndef OdDbEvalVariant_INCLUDED_
#define OdDbEvalVariant_INCLUDED_ /*!DOM*/

#include "DbConstraintsExports.h"
#include "ResBuf.h"
#include "DbFiler.h"

#include "TD_PackPush.h"

class OdDbEvalVariant;

typedef OdSmartPtr<OdDbEvalVariant> OdDbEvalVariantPtr;

/** \details
  Declares the data types.
*/
enum DwgDataType {
  kDwgNull = 0,
  kDwgReal = 1,
  kDwgInt32 = 2,
  kDwgInt16 = 3,
  kDwgInt8 = 4,
  kDwgText = 5,
  kDwgBChunk = 6,
  kDwgHandle = 7,
  kDwgHardOwnershipId = 8,
  kDwgSoftOwnershipId = 9,
  kDwgHardPointerId = 10,
  kDwgSoftPointerId = 11,
  kDwg3Real = 12,
  kDwgInt64 = 13,
  kDwg2Real = 14,
  kDwgNotRecognized = 19
};

/** \details
  This class represents a wrapper for a resbuf structure.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbEvalVariant : public OdResBuf
{
public:
  ODRX_DECLARE_MEMBERS(OdDbEvalVariant );

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbEvalVariant();

  /** \details
    Destructor destroys an instance of this class.
  */
  ~OdDbEvalVariant();

  /** \details
    Initializes the content of this variable.
  */
  static OdDbEvalVariantPtr init(const OdDbEvalVariant& other); 
  
  /** \details
    Initializes the content of this variable.
  */
  static OdDbEvalVariantPtr init(const OdDbEvalVariant* pOther);
  
  /** \details
    Initializes the content of this variable.
  */
  static OdDbEvalVariantPtr init(double       dVal);
  
  /** \details
    Initializes the content of this variable.
  */
  static OdDbEvalVariantPtr init(short        iVal);
//static OdDbEvalVariantPtr init(const ACHAR*  szVal);
  
  /** \details
    Initializes the content of this variable.
  */
  static OdDbEvalVariantPtr init(int         lVal);
  
  /** \details
    Initializes the content of this variable.
  */
  static OdDbEvalVariantPtr init(const OdDbObjectId& id);
  
  /** \details
    Initializes the content of this variable.
  */
  static OdDbEvalVariantPtr init(const OdGePoint2d&  pt);
  
  /** \details
    Initializes the content of this variable.
  */
  static OdDbEvalVariantPtr init(const OdGePoint3d&  pt);
  
  /** \details
    Initializes the content of this variable.
  */
  static OdDbEvalVariantPtr init(const OdResBuf& rb);
  
  /** \details
    Initializes the content of this variable.
  */
  static OdDbEvalVariantPtr init(const OdString& str);

  /** \details
    Sets the content of this variable using a given value.
  */
  OdDbEvalVariant& operator=(const OdResBuf& rb);
  OdDbEvalVariant& operator=(double      dVal);
  OdDbEvalVariant& operator=(short       iVal);
  OdDbEvalVariant& operator=(int        lVal);
//OdDbEvalVariant& operator=(const ACHAR* szVal);
  OdDbEvalVariant& operator=(const OdDbEvalVariant& other);
  OdDbEvalVariant& operator=(const OdDbObjectId& id);
  OdDbEvalVariant& operator=(const OdGePoint2d&  pt);
  OdDbEvalVariant& operator=(const OdGePoint3d&  pt);
  OdDbEvalVariant& operator=(const OdString&  str);

  /** \details
    Clears the content of this variable and frees any allocated memory.
  */
  void clear();

  /** \details
    Copies the content of the variable from another rx-object.
  */
  void copyFrom(const OdRxObject* pOther);//eWrongObjectType 

  /** \details
    Checks whether content of this variable less than content of a given variable.
  */
  bool operator <  ( const OdDbEvalVariant& val ) const;

  /** \details
    Checks whether content of this variable more than content of a given variable.
  */
  bool operator >  ( const OdDbEvalVariant& val ) const;

  /** \details
    Checks whether content of this variable equal to or less than content of a given variable.
  */
  bool operator <= ( const OdDbEvalVariant& val ) const;

  /** \details
    Checks whether content of this variable equal to or more than content of a given variable.
  */
  bool operator >= ( const OdDbEvalVariant& val ) const;

  /** \details
    Checks whether content of this variable is equal to the content of a given variable.
  */
  bool operator == ( const OdDbEvalVariant& val ) const;

  /** \details
    Checks whether content of this variable is not equal to the content of a given variable.
  */
  bool operator != ( const OdDbEvalVariant& val ) const;

  /** \details
    Reads the .dwg file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  OdResult dwgInFields(OdDbDwgFiler* pFiler);

  /** \details
    Writes the .dwg file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  void dwgOutFields(OdDbDwgFiler* pFiler) const;

  /** \details
    Reads the .dxf file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  OdResult dxfInFields(OdDbDxfFiler* pFiler);

  /** \details
    Writes the .dxf file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  void dxfOutFields(OdDbDxfFiler* pFiler) const;

  /** \details
    Returns the data type of this variable.
  */
  DwgDataType getType() const;

  /** \details
    Sets the content of this variable using a given value.
  */
  OdResult setValue(int /*AcDb::DxfCode*/ groupcode, const double value);
  
  /** \details
    Sets the content of this variable using a given value.
  */
  OdResult setValue(int /*AcDb::DxfCode*/ groupcode, const short value);
  
  /** \details
    Sets the content of this variable using a given value.
  */
  OdResult setValue(int /*AcDb::DxfCode*/ groupcode, const int value);
  
  /** \details
    Sets the content of this variable using a given value.
  */
  OdResult setValue(int /*AcDb::DxfCode*/ groupcode, const OdString& value);
  
  /** \details
    Sets the content of this variable using a given value.
  */
  OdResult setValue(int /*AcDb::DxfCode*/ groupcode, const OdDbObjectId value);
  
  /** \details
    Sets the content of this variable using a given value.
  */
  OdResult setValue(int /*AcDb::DxfCode*/ groupcode, const OdGePoint3d& value);
  
  /** \details
    Sets the content of this variable using a given value.
  */
  OdResult setValue(int /*AcDb::DxfCode*/ groupcode, const OdGePoint2d& value);

  /** \details
    Gets the content of this variable.
  */
  OdResult getValue(double& value) const;
  
  /** \details
    Gets the content of this variable.
  */
  OdResult getValue(short& value) const;
  
  /** \details
    Gets the content of this variable.
  */
  OdResult getValue(int& value) const;
  
  /** \details
    Gets the content of this variable.
  */
  OdResult getValue(OdString& value) const;
  
  /** \details
    Gets the content of this variable.
  */
  OdResult getValue(OdDbObjectId& value, OdDbDatabase *pDb = NULL) const;
  
  /** \details
    Gets the content of this variable.
  */
  OdResult getValue(OdGePoint3d& value) const;
  
  /** \details
    Gets the content of this variable.
  */
  OdResult getValue(OdGePoint2d& value) const;

protected:
  /*!DOM*/
  DwgDataType m_type;

};

#include "TD_PackPop.h"

#endif
