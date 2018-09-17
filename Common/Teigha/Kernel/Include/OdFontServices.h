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

#ifndef _OdFontServices_h_Included_
#define _OdFontServices_h_Included_

#include "RxObject.h"
#include "DbBaseDatabase.h"
#include "Gi/GiTextStyle.h"
#include "Gi/GiExport.h"

class OdDbBaseHostAppServices;
/** \details
    This class is the base class for classes that provide platform-dependent font services for Teigha.

    Corresponding C++ library: TD_Gi
    <group Other_Classes>
*/
class FIRSTDLL_EXPORT OdFontServices : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdFontServices);
  /** \details
    Returns the specified OdGiTextStyle record for the specified OdDbDatabase object.
    \param textStyle [out]  Receives the TextStyle record.
    \param pDb [in]  Pointer to the database.
  */
  virtual void loadStyleRec(OdGiTextStyle& textStyle, OdDbBaseDatabase* pDb) const = 0;
  /** \details
    Returns the FontFilePath for the specified OdGiTextStyle and OdDbDatabase objects.
    \param textStyle [in]  TextStyle object.
    \param pDb [in]  Pointer to the database.
  */
  virtual OdString getFontFilePath(OdGiTextStyle& textStyle, OdDbBaseDatabase* pDb) const = 0;
  /** \details
    Returns the BigFontFilePath for the specified OdGiTextStyle and OdDbDatabase objects.
    \param textStyle [in]  TextStyle object.
    \param pDb [in]  Pointer to the database.
  */
  virtual OdString getBigFontFilePath(OdGiTextStyle& textStyle, OdDbBaseDatabase* pDb) const = 0;
  /** \details
    Returns the default OdFont object associated with this FontServices object.
  */
  virtual OdFontPtr defaultFont() const = 0;
  /** \details
    Retrieval OdTtfDescriptor data from file.
  */
  virtual bool getTTFParamFromFile(OdString fileName, OdTtfDescriptor& descr) const = 0;
  /** \details
    Locates the TTF or TTC file containing the specified font description.
  */
  virtual bool ttfFileNameByDescriptor(const OdTtfDescriptor& descr, OdString& fileName, OdDbBaseHostAppServices* pHost) = 0;

  virtual bool getSystemFontFolders(OdStringArray& aDirs, OdDbBaseHostAppServices* pHost) = 0;

  virtual void collectFilePathsInDirectory(OdStringArray& res, const OdString& sPath, const OdString& sFilter = L"*.*") = 0;

};

#define ODDB_FONT_SERVICES OD_T("OdDbFontServices")

/** \details
    This template class is a specialization of the OdSmartPtr class for OdFontServices object pointers.
*/
typedef OdSmartPtr<OdFontServices> OdFontServicesPtr;

#endif
