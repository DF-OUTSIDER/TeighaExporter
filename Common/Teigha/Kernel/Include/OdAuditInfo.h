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




#ifndef _OD_AUDITINFO_INCLUDED_
#define _OD_AUDITINFO_INCLUDED_

#include "TD_PackPush.h"

#include "DbRootExport.h"
#include "OdString.h"
#include "RxObject.h"

class OdAuditInfoImpl;
//class OdRxObject;

/** \details
  This class tracks audit information during a database audit.

  \sa
  TD_Db

  <group OdDb_Classes>
*/
class DBROOT_EXPORT OdAuditInfo
{
public:

  enum PrintDest
  {
    kSilent  = 0, // None.
    kCmdLine = 1, // Command line.
    kFile    = 2, // File.
    kBoth    = 3  // Both command line and file.
  };
  struct MsgInfo
  {
    OdString strName;
    OdString strValue;
    OdString strValidation;
    OdString strDefaultValue;
    bool     bIsError;
  };

  OdAuditInfo();

  virtual ~OdAuditInfo();

  /** \details
    Returns true if and only if errors are to be fixed during the
    audit that uses this AuditInfo object.
  */
  bool fixErrors() const;

  /** \details
    Controls the fixing of errors are to be fixed during the
    audit that uses this AuditInfo object.

    \param fixErrors [in]  Fix errors.
  */
  void setFixErrors(
    bool fixErrors);

  /** \details
    Returns the number of errors reported to this AuditInfo object.
  */
  int numErrors() const;

  /** \details
    Returns the number of errors reported as fixed to this AuditInfo object.
  */
  int numFixes() const;

  /** \details
    Increments the count of errors reported to this AuditInfo object.
    \param increment [in]  Increment.
  */
  void errorsFound(
    int count);

  /** \details
    Increments the count of errors reported as fixed to this AuditInfo object.
    \param increment [in]  Increment.
  */
  void errorsFixed(
    int count);

  /** \details
  Increments the count of errors which should be interpreted as skipped.
  \param increment [in]  Increment.
  */
  void errorsSkip(
    int count);

  /** \details
    Notification function called to print error information.

    \remarks
    Prints log information about an error detected or fixed during an audit.

    \param name [in] Type of erroneous data found.
    \param value [in] Value of the bad data.
    \param validation [in] Reason the data were bad.
    \param defaultValue [in] Default value to which the were set.

    \remarks
    As implemented, this functions does nothing.
  */
  virtual void printError(
    const OdString& name,
    const OdString& value,
    const OdString& validation = OdString::kEmpty,
    const OdString& defaultValue = OdString::kEmpty );

  /** \details
    Notification function called to print error information.

    \param pObject [in]  Pointer to the object which generated the error.
    \param value [in] Value of the bad data.
    \param validation [in] Reason the data were bad.
    \param defaultValue [in] Default value to which the were set.

   \remarks
    pObject->isA()->name() is used to generate the name.
  */
  virtual void printError(
    const OdRxObject* pObject,
    const OdString& value,
    const OdString& validation = OdString::kEmpty,
    const OdString& defaultValue = OdString::kEmpty);

  /** \details
    Notification function called to print log information.

    \param logInfo [in]  Log information.
  */
  virtual void printInfo (
    const OdString& logInfo);

  /** \details
    Allows fixed objects to specify a regen is required.
  */
  void requestRegen();

  /** \details
    Resets the number of errors reported to this AuditInfo object.
  */
  void resetNumEntities();

  /** \details
    Increments by one the number of errors reported as processed to this AuditInfo object.
  */
  void incNumEntities();

  /** \details
    Returns the number of entities reported as processed to this AuditInfo object.
  */
  int numEntities();

  /** \details
    Returns the last error information that is intended for printing.
  */
  virtual const MsgInfo& getLastInfo();

  /** \details
    Sets the last error information that is intended for printing.
    \param lastInfo [in]  Last error information.
  */
  virtual void setLastInfo(
    MsgInfo &lastInfo);

  /*  If the current count of entities being maintained in the instance
      of OdAuditInfo is a multiple of 100, and msg is not NULL,
      then this function will print the string pointed to by msg
      followed by the current entity count out to the audit log file.
     void printNumEntities(const OdChar* msg);
  */

  /** \details
    Sets the destination for log printing.
    \param printDest [in]  Print destination.

    \remarks
    printDest must be one of the following:

    <table>
    Name        Value  Description
    kSilent     0      None.
    kCmdLine    1      Command line.
    kFile       2      File.
    kBoth       3      Both command line and file.
    </table>
  */
  void setPrintDest(
    PrintDest printDest);

  /** \details
    Returns the destination for log printing.

    \remarks
    getPrintDest() returns one of the following:

    <table>
    Name        Value  Description
    kSilent     0      None.
    kCmdLine    1      Command line.
    kFile       2      File.
    kBoth       3      Both command line and file.
    </table>
  */
  PrintDest  getPrintDest();

private:
  OdAuditInfoImpl* m_pImpl;
};

/** \details
    This registration allows you to catch the invoking of each audit info destructor.
*/
typedef void (*ODAUDITINFO_CALLBACK)(const OdAuditInfo*);
DBROOT_EXPORT void odrxRegisterAuditInfoDestructorCallback(const ODAUDITINFO_CALLBACK callbackFunc);
DBROOT_EXPORT void odrxUnregisterAuditInfoDestructorCallback();

#include "TD_PackPop.h"

#endif // _OD_AUDITINFO_INCLUDED_

