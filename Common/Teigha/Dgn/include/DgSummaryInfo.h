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
#ifndef __DG_SUMMARYINFO_H__
#define __DG_SUMMARYINFO_H__


#include "DgExport.h"
#include "RxObject.h"
#include "RxVariant.h"
#include "OdTimeStamp.h"
#include "OdBinaryData.h"

#include "TD_PackPush.h"


class OdDgPropertyValue;
/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgPropertyValue object pointers.
*/
typedef OdSmartPtr<OdDgPropertyValue> OdDgPropertyValuePtr;

/** \details
    This class encapsulates a UserDefined Property value.

    Corresponding C++ library: TG_Db
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgPropertyValue : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDgPropertyValue);

  enum CustomType
  {
    kCustomNumber =  3,
    kCustomBool   = 11,
    kCustomText   = 30,
    kCustomDate   = 64
  };
  static OdDgPropertyValuePtr createObject(OdUInt16 vtype, const OdVariant& value);

  virtual OdUInt32 getId() const = 0;
  virtual void setId(OdUInt32 val) = 0;

  virtual OdUInt16 getType() const = 0;
  virtual void setType(OdUInt16 type) = 0;

  virtual const OdVariant& getValue() const = 0;
  virtual void setValue(const OdVariant& val) = 0;
};


/** \details
    This class encapsulates a Summary Information.

    Corresponding C++ library: TG_Db
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgSummaryInformation : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDgSummaryInformation);

  virtual OdString getTitle() const = 0;
  virtual void setTitle(const OdString& str) = 0;

  virtual OdString getSubject() const = 0;
  virtual void setSubject(const OdString& str) = 0;

  virtual OdString getAuthor() const = 0;
  virtual void setAuthor(const OdString& str) = 0;

  virtual OdString getKeywords() const = 0;
  virtual void setKeywords(const OdString& str) = 0;

  virtual OdString getComments() const = 0;
  virtual void setComments(const OdString& str) = 0;

  virtual OdString getTemplate() const = 0;
  virtual void setTemplate(const OdString& str) = 0;

  virtual OdString getLastSavedBy() const = 0;
  virtual void setLastSavedBy(const OdString& str) = 0;

  virtual OdString getRevisionNumber() const = 0;
  virtual void setRevisionNumber(const OdString& str) = 0;

  virtual OdString getApplicationName() const = 0;
  virtual void setApplicationName(const OdString& str) = 0;

  /** \details
    Returns the thumbnail bitmap data.

    \remarks
    The thumbnail is in Windows BITMAPINFO* format.
  */
  virtual void getThumbnailBitmap(OdBinaryData& dibData) const = 0;

  /** \details
    Sets the thumbnail bitmap data.

    \remarks
    The thumbnail is in Windows BITMAPINFO* format.
  */
  virtual void setThumbnailBitmap(const OdBinaryData& dibData) = 0;

  virtual bool getRetainOriginalThumbnailBitmapFlag() const = 0;
  virtual void setRetainOriginalThumbnailBitmapFlag(bool retain) = 0;

  virtual OdTimeStamp getTotalEditingTime() const = 0;

  virtual OdTimeStamp getLastPrintedTime() const = 0;

  virtual OdTimeStamp getCreatedTime() const = 0;

  virtual OdTimeStamp getLastSavedTime() const = 0;

  virtual OdUInt32 getSecurity() const = 0;

  virtual OdDgPropertyValuePtr getProperty(OdUInt32 pid) const = 0;
  virtual void setProperty(OdUInt32 pid, OdDgPropertyValuePtr pProp) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgSummaryInformation object pointers.
*/
typedef OdSmartPtr<OdDgSummaryInformation> OdDgSummaryInformationPtr;


/** \details
    This class encapsulates a Document Summary Information.

    Corresponding C++ library: TG_Db
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDocumentSummaryInformation : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDgDocumentSummaryInformation);

  virtual OdString getCategory() const = 0;
  virtual void setCategory(const OdString& str) = 0;

  virtual OdString getManager() const = 0;
  virtual void setManager(const OdString& str) = 0;

  virtual OdString getCompany() const = 0;
  virtual void setCompany(const OdString& str) = 0;

  virtual OdDgPropertyValuePtr getProperty(OdUInt32 pid) const = 0;
  virtual void setProperty(OdUInt32 pid, OdDgPropertyValuePtr pProp) = 0;

  virtual OdDgPropertyValuePtr getCustomProperty(const OdString& name) const = 0;
  virtual void setCustomProperty(const OdString& name, OdDgPropertyValuePtr pProp) = 0;

  virtual OdRxDictionaryPtr getCustomProperties() const = 0;
  virtual void setCustomProperties(OdRxDictionaryPtr props) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgDocumentSummaryInformation object pointers.
*/
typedef OdSmartPtr<OdDgDocumentSummaryInformation> OdDgDocumentSummaryInformationPtr;


/** \details
    Returns the Summary Information object for the specified OdDgDatabase instance.

    \param pDb [in]  Database.
*/
TG_EXPORT OdDgSummaryInformationPtr oddgGetSummaryInformation(OdDgDatabase* pDb);

/** \details
    Returns the Document Summary Information object for the specified OdDgDatabase instance.

    \param pDb [in]  Database.
*/
TG_EXPORT OdDgDocumentSummaryInformationPtr oddgGetDocumentSummaryInformation(OdDgDatabase* pDb);

/** \details
    Retutns the Summary Information object for the specified dgn file.

    \param pDb [in]  Dgn file name.
*/
TG_EXPORT OdDgSummaryInformationPtr oddgGetSummaryInformation(const OdString& fileName);

/** \details
    Retutns the Document Summary Information object for the specified dgn file.

    \param pDb [in]  Dgn file name.
*/
TG_EXPORT OdDgDocumentSummaryInformationPtr oddgGetDocumentSummaryInformation(const OdString& fileName);

#include "TD_PackPop.h"

#endif // __DG_SUMMARYINFO_H__
