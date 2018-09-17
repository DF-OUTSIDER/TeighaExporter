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




#ifndef _SUMMINFO_H_
#define _SUMMINFO_H_

#include "RxObject.h"
#include "DbDatabase.h"
#include "DbLibraryInfo.h"

#include "TD_PackPush.h"

/** \details
    This class encapsulates a set of character strings containing 
    additional information for an OdDbDatabase as Summary Information.

    \remarks
    In addition to the predefined fields, you add create any number of custom fields to the Summary Information.
    Corresponding C++ library: TD_Db
    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbDatabaseSummaryInfo : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbDatabaseSummaryInfo);

  OdDbDatabaseSummaryInfo();
  
  /** \details
    Returns the value of the Title field of the Summary Information.
  */
  virtual OdString getTitle() const = 0;

  /** \details
    Sets the value of the Title field of the Summary Information.
    \param title [in]  Title.
  */
  virtual void setTitle(
    const OdString& title) = 0;
  
  /** \details
    Returns the value of the Subject field of the Summary Information.
  */
  virtual OdString getSubject() const = 0;

  /** \details
    Sets the value of the Subject field of the Summary Information.
    \param subject [in]  Subject.
  */
  virtual void setSubject(
    const OdString& subject) = 0;
  
  /** \details
    Returns the value of the Author field of the Summary Information.
  */
  virtual OdString getAuthor() const = 0;

  /** \details
    Sets the value of the Author field of the Summary Information.
    \param author [in]  Author.
  */
  virtual void setAuthor(
    const OdString& author) = 0;

  /** \details
    Returns the value of the Keywords field of the Summary Information.
    
    \remarks
    Keywords are delimited by spaces.
  */
  virtual OdString getKeywords() const = 0;

  /** \details
    Sets the value of the Keywords field of the Summary Information.
    \param keywords [in]  Keywords.
    \remarks
    Keywords are delimited by spaces.
  */
  virtual void setKeywords(
    const OdString& keywords) = 0;

  /** \details
    Returns the value of the Comments field of the Summary Information.
  */
  virtual OdString getComments() const = 0;

  /** \details
    Sets the value of the Comments field of the Summary Information.
    \param comments [in]  Comments.
  */
  virtual void setComments(
    const OdString& comments) = 0;


  /** \details
    Returns the value of the LastSavedBy field of the Summary Information.
  */
  virtual OdString getLastSavedBy() const = 0;

  /** \details
    Sets the value of the LastSavedBy field of the Summary Information.
    \param lastSavedBy [in]  Last Saved By.
  */
  virtual void setLastSavedBy(
    const OdString& lastSavedBy) = 0;

  /** \details
    Returns the value of the RevisionNumber field of the Summary Information.
    
    \remarks
    The RevisionNumber field is returned as a string.
  */
  virtual OdString getRevisionNumber() const = 0;

  /** \details
    Sets the value of the RevisionNumber field of the Summary Information.
    \param revisionNumber [in]  Revision number.
    \remarks
    The RevisionNumber field is set as a string.
  */
  virtual void setRevisionNumber(
    const OdString& revisionNumber) = 0;
    
  /** \details
    Returns the value of the HyperlinkBase field of the Summary Information.
  */
  virtual OdString getHyperlinkBase() const = 0;

  /** \details
    Sets the value of the HyperlinkBase field of the Summary Information.
    \param hyperlinkBase [in]  HyperlinkBase.
  */
  virtual void setHyperlinkBase(
    const OdString& hyperlinkBase) = 0;
  /** \details
    Returns the number of custom fields in the Summary Information  
  */
  virtual int numCustomInfo() const = 0;
  
  /** \details
    Appends a custom field to the Summary Information.
    \param key [in]  Name of the custom field.
    \param value [in]  Value of the custom field.
  */
  virtual void addCustomSummaryInfo(
    const OdString& key, 
    const OdString& value ) = 0;
  
  /** \details
    Deletes the specified custom field from the Summary Information.
    \param fieldIndex [in]  Field index [1..numCustomInfo()].
  */
  virtual void deleteCustomSummaryInfo(
    int fieldIndex) = 0;
  
  /** \details
    Deletes the specified custom field from the Summary Information.
    \param key [in]  Name of the field.
  */
  virtual bool deleteCustomSummaryInfo(
    const OdString& key) = 0;
    
  /** \details
    Returns the value of the specified custom field of the Summary Information.
    \param fieldIndex [in]  Field index [1..numCustomInfo()].
    \param key [out]  Receives the name.
    \param rvalue [out]  Receives the value.
  */
    virtual void getCustomSummaryInfo(
    int fieldIndex, 
    OdString& key, 
    OdString& rvalue) const = 0;

  /** \details
    Sets the value of the specified custom field of the Summary Information.
    \param fieldIndex [in]  Field index [1..numCustomInfo()].
    \param key [in]  Name of the field.
    \param value [in]  Value for the field.
  */
  virtual void setCustomSummaryInfo(
    int fieldIndex, const OdString& key, 
    const OdString& value) = 0;
  
  /** \details
    Returns the value of the specified custom field of the Summary Information.
    \param customInfoKey [in]  Name of the field to retrieve.
    \param rvalue [out]  Receives the value.
  */
  virtual bool getCustomSummaryInfo(
    const OdString& customInfoKey, 
    OdString& rvalue ) const = 0;
  
  /** \details
    Sets the value of the specified custom field of the Summary Information.
    \param customInfoKey [in]  Name of the field to set.
    \param value [in]  Value for the field.
  */
  virtual void setCustomSummaryInfo(
    const OdString& customInfoKey, 
    const OdString& value) = 0;
  
  /** \details
    Returns the OdDbDatabase associated with this Summary Information.
  */
  virtual OdDbDatabase* database() const = 0; 
  
  /** \details
    Sets the OdDbDatabase associated with this Summary Information.
    \param pDb [in]  Pointer to the database.
  */
  virtual void setDatabase(
    OdDbDatabase *pDb) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbDatabaseSummaryInfo object pointers.
*/
typedef OdSmartPtr<OdDbDatabaseSummaryInfo> OdDbDatabaseSummaryInfoPtr;


TOOLKIT_EXPORT OdDbLibraryInfo*           oddbGetLibraryInfo(void);
TOOLKIT_EXPORT OdDbDatabaseSummaryInfoPtr oddbGetSummaryInfo(OdDbDatabase* pDb);
TOOLKIT_EXPORT void                       oddbPutSummaryInfo(const OdDbDatabaseSummaryInfo* pInfo);


#include "TD_PackPop.h"

#endif // _SUMMINFO_H_
