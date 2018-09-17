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


// Get output file name. Suggest all file extensions supported by RxRasterServices
static OdString getRasterFilePathToSave(OdRxRasterServices* pRasSvcs, OdEdUserIO* pIO,
                           const OdString& strDefName,
                           const OdString& strDefExt)
{
  OdString sFilters;
  OdString sDefExt(OdString::kEmpty); 

  OdUInt32Array types = pRasSvcs->getRasterImageTypes();
  if (types.size())
  {
    for (unsigned index = 0; index < types.size(); index++)
    {
      OdUInt32 tp = types.getAt(index);
      OdString sFilterName,
      sExt = pRasSvcs->mapTypeToExtension(tp, &sFilterName);
      if (sExt.isEmpty())
        continue;
      sExt.makeLower().remove(L'.');
      if (sExt == strDefExt)
        sDefExt = sExt;
      
      if (sFilterName.isEmpty())
      {
        sFilterName = sExt;
        sFilterName.makeUpper();
        sFilterName += L" (*." + sExt + L")";
      }

      sFilters += sFilterName + L"|*." + sExt + L"|";
      if (sExt == L"png")
        sDefExt = sExt;
    }
  }
  if (sFilters.isEmpty())
    sFilters = L"Windows Bitmap Files (*.bmp)|*.bmp|";
  sFilters += L"|";

  return pIO->getFilePath(L"Enter file name to save image:", OdEd::kGfpForSave|OdEd::kGfpOverwritePrompt,
                            L"Save image", sDefExt, strDefName, sFilters);
}
