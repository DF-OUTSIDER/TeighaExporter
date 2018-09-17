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


#ifndef _ODFILEDEPMGR_INCLUDED_
#define _ODFILEDEPMGR_INCLUDED_

#include "TD_PackPush.h"

#include "OdString.h"
#include "RxDictionary.h"
#include "DbExport.h"

/** \details
    This class represents entries in the DWG section of File Dependency List.
    Corresponding C++ library: TD_Db
    <group Other_Classes> 
*/
class TOOLKIT_EXPORT OdFileDependencyInfo : public OdRxObject
{
protected:
  OdFileDependencyInfo();

public:
  ODRX_DECLARE_MEMBERS(OdFileDependencyInfo);
  virtual void copyFrom(
    const OdRxObject* pSource);
  OdString m_FullFileName;      // Full name of file.
  OdString m_FileName;          // Base name of file.
  OdString m_FoundPath;         // Path at which file was found.
  OdString m_FingerprintGuid;   // Fingerprint GUID.
  OdString m_VersionGuid;       // Version GUID.
  OdString m_Feature;           // Name of the application or *feature* that created this entry.
  bool m_bIsModified;           // Database was modified since last save.
  bool m_bAffectsGraphics;      // Entry affects the screen view.
  OdInt32 m_nIndex;             // Unique File Dependency List *index*.
  OdInt32 m_nTimeStamp;         // Seconds since 1/1/1980.
  OdInt32 m_nFileSize;          // Current File size.
  OdRefCounter m_nReferenceCount;    // Current reference count for *database*.
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdFileDependencyInfo object pointers.
*/
typedef OdSmartPtr<OdFileDependencyInfo> OdFileDependencyInfoPtr;

/** \details
    This class allows management of entries in the DWG section of the File Dependency List. 

    Corresponding C++ library: TD_Db
    
    \remarks
    Every OdDbDatabase instance has permanently associated with it an instance of the this class.
    <group Other_Classes> 
*/
class TOOLKIT_EXPORT OdFileDependencyManager : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdFileDependencyManager);

  OdFileDependencyManager() {};

  /** \details
    Creates an entry in the DWG section of the File Dependency List.

    \param feature [in]  Name of the application or feature creating the entry.
    \param fullFileName [in]  Full name of file for the entry.
    \param affectsGraphics [in]  True if and only if the entry affects the screen view.
    \param noIncrement [in]  If true, duplicate records are created instead of 
                    incrementing the reference count of the existing entry.
    
    \returns
    Returns the Unique File Dependency List ID for the new entry, or 0 if not successful.
  */
  virtual OdUInt32 createEntry(
    const OdString& feature,
    const OdString& fullFileName,
    const bool affectsGraphics = false,
    const bool noIncrement = false) = 0;


  /** \details
    Returns the specified entry in the DWG section of the File Dependency List.
    
    \param feature [in]  Name of the application or feature that created the entry.
    \param fullFileName [in]  Full name of file for the entry.
    \param fileInfo [out]  Receives the file dependency information
    \param useCachedInfo [in]  True to use cached information, false to use updated information.
    
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult getEntry(
    const OdString& feature,
    const OdString& fullFileName,
    OdFileDependencyInfoPtr& fileInfo, 
    const bool useCachedInfo = false) = 0;

  /** \details
    Returns the specified entry in the DWG section of the File Dependency List.
    
    
    \param fdlIndex [in]  File Dependency List index.
    \param fileInfo [out]  Receives the file dependency information
    \param useCachedInfo [in]  True to use cached information, false to use updated information.
    
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult getEntry(
    OdUInt32 fdlIndex,
    OdFileDependencyInfoPtr& fileInfo, 
    const bool useCachedInfo = false) = 0;

  /** \details
    Updates the specified entry in the DWG section of the File Dependency List.
    
    \param feature [in]  Name of the application or feature that created the entry.
    \param fullFileName [in]  Full name of file for the entry.
    \param index [in]  File Dependency List index.

    \remarks
    This function resets the mIsModified, mTimestamp, and mFileSize members of the entry.    
  */
  virtual OdResult updateEntry(
    const OdString& feature,
    const OdString& fullFileName) = 0;
  virtual OdResult updateEntry(
    OdUInt32 index) = 0;

  /** \details
    Erases the specified entry from the DWG section of the File Dependency List.
    
    \param feature [in]  Name of the application or feature that created the entry.
    \param fullFileName [in]  Full name of file for the entry.
    \param forceRemove [in]  True to erases the entry regardless of its reference count.

    \remarks
    If the reference count of the entry is 1, or forceRemove is true, the entry is erased. Otherwise,
    this function merely decrements the reference count.
  */
  virtual OdResult eraseEntry(
    const OdString& feature,
    const OdString& fullFileName,
    const bool forceRemove = false) = 0;

  /** \details
    Erases the specified entry from the DWG section of the File Dependency List.
    
    \param fdlIndex [in]  File Dependency List index.
    \param forceRemove [in]  True to erases the entry regardless of its reference count.

    \remarks
    If the reference count of the entry is 1, or forceRemove is true, the entry is erased. Otherwise,
    this function merely decrements the reference count.
  */
  virtual OdResult eraseEntry(
    OdUInt32 fdlIndex,
    const bool forceRemove = false) = 0;

  /** \details
    Returns the number of unique entries in the DWG section of the File Dependency List.
  */
  virtual OdUInt32 countEntries() = 0;

  /** \details
    Initializes an iterator for the DWG section of the File Dependency List.
    
    \param feature [in]  Name of the application or feature creating the entry.
    \param modifiedOnly [in]  True to return only modified entries.
    \param affectsGraphicsOnly [in]  True to return only  entries that effect the screen view.
    \param walkXRefTree [in]  True to traverse the File Dependency Lists of Xref'd drawings.    
    
    \remarks
    If no arguments are specified, all entries in the local list will be traversed. 
    Specifying feature, modifiedOnly true, and/or affectsGraphicsOnly true will restrict the entries traversed.
  */
  virtual void iteratorInitialize(
    const OdString& feature = OdString::kEmpty, 
    const bool modifiedOnly = false,
    const bool affectsGraphicsOnly = false,
    const bool walkXRefTree = false) = 0;

  /** \details
    Returns the index of the next entry in the DWG section of the File Dependency List.
    
    \remarks
    The returned value may be used in the getEntry() and eraseEntry() functions.
  */
  virtual OdUInt32 iteratorNext() = 0;

  // get feature list for saving

  /** \details
    Returns the dictionary containing the feature list in the DWG section of the File Dependency List.
  */
  virtual void getFeatures(
    OdRxDictionaryPtr& features) = 0;
  
  /** \details
    Removes the Xref entries added to the DWG section of the File Dependency List
    by iteratorInitialize.
  */
  virtual void clearXRefEntries() = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdFileDependencyManager object pointers.
*/
typedef OdSmartPtr<OdFileDependencyManager> OdFileDependencyManagerPtr;

#include "TD_PackPop.h"

#endif
