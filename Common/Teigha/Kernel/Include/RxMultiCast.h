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

#ifndef _ODRXMULTICAST_H_
#define _ODRXMULTICAST_H_

#include "RxObject.h"

#include "TD_PackPush.h"

/** details 
  Provides ability to cast RxObject into set of classes as one step.

   - Basic (investigate entire classes structure and set all pointers)
   odrxMultiCast - Check all classes include protocol extensions
   odrxMultiCastObject - Check all classes without protocol extensions check
   odrxMultiCastX - Check all protocol extensions only
   - Single shot (investigate classes structure until any first doesn't found and stop check)
   odrxMultiCastFwd - scan forward (from OdRxObject class) with protocol extensions with second priority
   odrxMultiCastFwdXPrior - scan forward (from OdRxObject class) with protocol extensions with first priority
   odrxMultiCastFwdObject - scan forward (from OdRxObject class) without protocol extensions check
   odrxMultiCastFwdX - scan forward (from OdRxObject class) protocol extensions only
   odrxMultiCastBk - scan backward (from last inherited class) with protocol extensions with second priority
   odrxMultiCastBkXPrior - scan backward (from last inherited class) with protocol extensions with first priority
   odrxMultiCastBkObject - scan backward (from last inherited class) without protocol extensions check
   odrxMultiCastBkX - scan backward (from last inherited class) protocol extensions only

   \param pObject [in] - object for which check will be processed
   \param pPointers [out] - array of pointers which will be set to found classes (use static_cast or c-style cast to convert pointers)
   \param pClasses [in] - array of classes into which object cast must be processed
   \param nClasses [in] - count of classes in pClasses and pointer in pPointers arrays
*/

inline // Returns count of available casts, pPointers must be non-zero
OdUInt32 odrxMultiCast(const OdRxObject *pObject, OdRxObjectPtr *pPointers, const OdRxClass **pClasses, OdUInt32 nClasses)
{
  OdUInt32 nCasts = 0;
  if (!pObject || !pPointers || !pClasses || !nClasses)
    return nCasts;
  OdRxClass *pClass = pObject->isA();
  while (pClass)
  {
    for (OdUInt32 nClass = 0; nClass < nClasses; nClass++)
    {
      if (pPointers[nClass].isNull())
      {
        if (pClass == pClasses[nClass])
        {
          pPointers[nClass] = pObject;
          nCasts++;
          if (nCasts == nClasses)
            return nCasts;
        }
        else
        {
          pPointers[nClass] = pClass->getX(pClasses[nClass]);
          if (!pPointers[nClass].isNull())
          {
            nCasts++;
            if (nCasts == nClasses)
              return nCasts;
          }
        }
      }
    }
    pClass = pClass->myParent();
  }
  return nCasts;
}

inline // If pCasts null returns bit mask, elsewhere count of casts
OdUInt32 odrxMultiCastObject(const OdRxObject *pObject, bool *pCasts, const OdRxClass **pClasses, OdUInt32 nClasses)
{
  OdUInt32 nCasts = 0, nBits = 0;
  if (!pObject || !pClasses || !nClasses)
    return nCasts;
  OdRxClass *pClass = pObject->isA();
  while (pClass)
  {
    for (OdUInt32 nClass = 0; nClass < nClasses; nClass++)
    {
      if ((pCasts) ? !pCasts[nClass] : ((nBits & (1 << nClass)) == 0))
      {
        if (pClass == pClasses[nClass])
        {
          if (pCasts)
            pCasts[nClass] = true;
          else
            nBits |= 1 << nClass;
          nCasts++;
          if (nCasts == nClasses)
            return nCasts;
        }
      }
    }
    pClass = pClass->myParent();
  }
  return (pCasts) ? nCasts : nBits;
}

inline // Returns count of available casts, pPointers must be non-zero
OdUInt32 odrxMultiCastX(const OdRxObject *pObject, OdRxObjectPtr *pPointers, const OdRxClass **pClasses, OdUInt32 nClasses)
{
  OdUInt32 nCasts = 0;
  if (!pObject || !pPointers || !pClasses || !nClasses)
    return nCasts;
  OdRxClass *pClass = pObject->isA();
  while (pClass)
  {
    for (OdUInt32 nClass = 0; nClass < nClasses; nClass++)
    {
      if (pPointers[nClass].isNull())
      {
        pPointers[nClass] = pClass->getX(pClasses[nClass]);
        if (!pPointers[nClass].isNull())
        {
          nCasts++;
          if (nCasts == nClasses)
            return nCasts;
        }
      }
    }
    pClass = pClass->myParent();
  }
  return nCasts;
}

inline
OdUInt32 odrxMultiCastFwd_(OdRxClass *pClass, const OdRxObject *pObject, OdRxObjectPtr &pPointer, const OdRxClass **pClasses, OdUInt32 nClasses)
{
  OdRxClass *pParent = pClass->myParent();
  if (pParent)
  {
    OdUInt32 nIndex = odrxMultiCastFwd_(pParent, pObject, pPointer, pClasses, nClasses);
    if (nIndex < nClasses)
      return nIndex;
  }
  for (OdUInt32 nClass = 0; nClass < nClasses; nClass++)
  {
    if (pClass == pClasses[nClass])
    {
      pPointer = pObject;
      return nClass;
    }
    else
    {
      pPointer = pClass->getX(pClasses[nClass]);
      if (!pPointer.isNull())
        return nClass;
    }
  }
  return nClasses;
}
inline
OdUInt32 odrxMultiCastFwd(const OdRxObject *pObject, OdRxObjectPtr &pPointer, const OdRxClass **pClasses, OdUInt32 nClasses)
{
  if (!pObject || !pClasses || !nClasses)
    return nClasses;
  OdRxClass *pClass = pObject->isA();
  return odrxMultiCastFwd_(pClass, pObject, pPointer, pClasses, nClasses);
}

inline
OdUInt32 odrxMultiCastFwdXPrior_(OdRxClass *pClass, const OdRxObject *pObject, OdRxObjectPtr &pPointer, const OdRxClass **pClasses, OdUInt32 nClasses)
{
  OdRxClass *pParent = pClass->myParent();
  if (pParent)
  {
    OdUInt32 nIndex = odrxMultiCastFwdXPrior_(pParent, pObject, pPointer, pClasses, nClasses);
    if (nIndex < nClasses)
      return nIndex;
  }
  for (OdUInt32 nClass = 0; nClass < nClasses; nClass++)
  {
    pPointer = pClass->getX(pClasses[nClass]);
    if (!pPointer.isNull())
      return nClass;
    if (pClass == pClasses[nClass])
    {
      pPointer = pObject;
      return nClass;
    }
  }
  return nClasses;
}
inline
OdUInt32 odrxMultiCastFwdXPrior(const OdRxObject *pObject, OdRxObjectPtr &pPointer, const OdRxClass **pClasses, OdUInt32 nClasses)
{
  if (!pObject || !pClasses || !nClasses)
    return nClasses;
  OdRxClass *pClass = pObject->isA();
  return odrxMultiCastFwdXPrior_(pClass, pObject, pPointer, pClasses, nClasses);
}

inline
OdUInt32 odrxMultiCastFwdObject_(OdRxClass *pClass, const OdRxObject *pObject, const OdRxClass **pClasses, OdUInt32 nClasses)
{
  OdRxClass *pParent = pClass->myParent();
  if (pParent)
  {
    OdUInt32 nIndex = odrxMultiCastFwdObject_(pParent, pObject, pClasses, nClasses);
    if (nIndex < nClasses)
      return nIndex;
  }
  for (OdUInt32 nClass = 0; nClass < nClasses; nClass++)
  {
    if (pClass == pClasses[nClass])
      return nClass;
  }
  return nClasses;
}
inline
OdUInt32 odrxMultiCastFwdObject(const OdRxObject *pObject, const OdRxClass **pClasses, OdUInt32 nClasses)
{
  if (!pObject || !pClasses || !nClasses)
    return nClasses;
  OdRxClass *pClass = pObject->isA();
  return odrxMultiCastFwdObject_(pClass, pObject, pClasses, nClasses);
}

inline
OdUInt32 odrxMultiCastFwdX_(OdRxClass *pClass, const OdRxObject *pObject, OdRxObjectPtr &pPointer, const OdRxClass **pClasses, OdUInt32 nClasses)
{
  OdRxClass *pParent = pClass->myParent();
  if (pParent)
  {
    OdUInt32 nIndex = odrxMultiCastFwdX_(pParent, pObject, pPointer, pClasses, nClasses);
    if (nIndex < nClasses)
      return nIndex;
  }
  for (OdUInt32 nClass = 0; nClass < nClasses; nClass++)
  {
    pPointer = pClass->getX(pClasses[nClass]);
    if (!pPointer.isNull())
      return nClass;
  }
  return nClasses;
}
inline
OdUInt32 odrxMultiCastFwdX(const OdRxObject *pObject, OdRxObjectPtr &pPointer, const OdRxClass **pClasses, OdUInt32 nClasses)
{
  if (!pObject || !pClasses || !nClasses)
    return nClasses;
  OdRxClass *pClass = pObject->isA();
  return odrxMultiCastFwdX_(pClass, pObject, pPointer, pClasses, nClasses);
}

inline
OdUInt32 odrxMultiCastBk(const OdRxObject *pObject, OdRxObjectPtr &pPointer, const OdRxClass **pClasses, OdUInt32 nClasses)
{
  if (!pObject || !pClasses || !nClasses)
    return nClasses;
  OdRxClass *pClass = pObject->isA();
  while (pClass)
  {
    for (OdUInt32 nClass = 0; nClass < nClasses; nClass++)
    {
      if (pClass == pClasses[nClass])
      {
        pPointer = pObject;
        return nClass;
      }
      else
      {
        pPointer = pClass->getX(pClasses[nClass]);
        if (!pPointer.isNull())
          return nClass;
      }
    }
    pClass = pClass->myParent();
  }
  return nClasses;
}

inline
OdUInt32 odrxMultiCastBkXPrior(const OdRxObject *pObject, OdRxObjectPtr &pPointer, const OdRxClass **pClasses, OdUInt32 nClasses)
{
  if (!pObject || !pClasses || !nClasses)
    return nClasses;
  OdRxClass *pClass = pObject->isA();
  while (pClass)
  {
    for (OdUInt32 nClass = 0; nClass < nClasses; nClass++)
    {
      pPointer = pClass->getX(pClasses[nClass]);
      if (!pPointer.isNull())
        return nClass;
      if (pClass == pClasses[nClass])
      {
        pPointer = pObject;
        return nClass;
      }
    }
    pClass = pClass->myParent();
  }
  return nClasses;
}

inline
OdUInt32 odrxMultiCastBkObject(const OdRxObject *pObject, const OdRxClass **pClasses, OdUInt32 nClasses)
{
  if (!pObject || !pClasses || !nClasses)
    return nClasses;
  OdRxClass *pClass = pObject->isA();
  while (pClass)
  {
    for (OdUInt32 nClass = 0; nClass < nClasses; nClass++)
    {
      if (pClass == pClasses[nClass])
        return nClass;
    }
    pClass = pClass->myParent();
  }
  return nClasses;
}

inline
OdUInt32 odrxMultiCastBkX(const OdRxObject *pObject, OdRxObjectPtr &pPointer, const OdRxClass **pClasses, OdUInt32 nClasses)
{
  if (!pObject || !pClasses || !nClasses)
    return nClasses;
  OdRxClass *pClass = pObject->isA();
  while (pClass)
  {
    for (OdUInt32 nClass = 0; nClass < nClasses; nClass++)
    {
      pPointer = pClass->getX(pClasses[nClass]);
      if (!pPointer.isNull())
        return nClass;
    }
    pClass = pClass->myParent();
  }
  return nClasses;
}

#include "TD_PackPop.h"

#endif // _ODRXMULTICAST_H_
