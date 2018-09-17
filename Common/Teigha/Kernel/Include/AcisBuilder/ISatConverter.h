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


#ifndef __ISATCONVERTER_H__
#define __ISATCONVERTER_H__

#include "ABExport.h"
#include "OdStreamBuf.h"
#include "ModelerDefs.h"
#include "OdArray.h"

/** \details
    This class is the base class for classes that provide platform-dependent loading and saving
    of SAT files for Teigha.

    Corresponding C++ library: TD_AcisBuilder
    <group AcisBuilder_Classes>
*/
class AB_EXPORT ISATConverter
{
public:
  /** \details
      Loads the specified SAT file. 
      \param pStreamBuf [in]  Pointer to the StreamBuf object from which the data are to be read.
      \param typeVer [out]  Receives the type and version of the SAT file.
      \param standardSaveFlag [in]  Controls the saving and restoring of use count data in the save file.
      \returns
      Returns true if and only if successful
  */
  virtual bool SatIn ( OdStreamBuf *pStreamBuf, AfTypeVer *typeVer, bool standardSaveFlag = true) = 0;
  /** \details
      Saves the specified SAT file. 
      \param pStreamBuf [in]  Pointer to the StreamBuf object to which the data are to be written.
      \param typeVer [in]  Type and version of the SAT file.
      \param standardSaveFlag [in]  Controls the saving and restoring of use count data in the save file.
      \returns
      Returns true if and only if successful
  */
  virtual bool SatOut( OdStreamBuf *pStreamBuf, AfTypeVer  typeVer, bool standardSaveFlag = true) = 0;

  virtual void *getIBr( ) = 0;

  virtual ~ISATConverter();
};

AB_EXPORT bool ABBreakToSingleBody(OdStreamBuf *pStreamBuf, AfTypeVer *typeVer, bool standardSaveFlag, OdArray<ISATConverter*>& entitySet);

AB_EXPORT ISATConverter* ABCreateConverter();

AB_EXPORT void ABDeleteConverter(ISATConverter* pIC);

#endif // __ISATCONVERTER_H__
