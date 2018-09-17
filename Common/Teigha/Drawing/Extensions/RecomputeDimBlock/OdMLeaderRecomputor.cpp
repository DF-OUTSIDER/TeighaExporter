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


#include "OdaCommon.h"
#include "OdMLeaderRecomputor.h"
#include "DbDictionary.h"
#include "OdBreakDimEngine.h"
#include "DbLine.h"
#include "DbMLeaderBreaks.h"

extern bool oddbIsEnableDimensionBreak();

// Support BREAK DATA // CORE-11001 
OdResult OdDbMLeaderRecomputePEImpl::recomputeBreakPoints(OdDbMLeader* pMLeader)
{
  if (!pMLeader || !oddbIsEnableDimensionBreak())
    return eNotApplicable; // disabled
  OdDbDictionaryPtr pExtDic = OdDbDictionary::cast(pMLeader->extensionDictionary().openObject(OdDb::kForRead));
  if (pExtDic.isNull())
    return eNotApplicable;
  OdDbBreakDataPtr pBreakData = OdDbBreakData::cast(pExtDic->getAt(L"ACAD_BREAKDATA", OdDb::kForRead));
  if (pBreakData.isNull())
    return eNotApplicable;

  OdDbMLeaderStylePtr pMleaderStyle = pMLeader->MLeaderStyle().openObject(OdDb::kForRead);
  double dBreakSize = pMleaderStyle->breakSize();
  OdBreakDimEngine eng(dBreakSize);

  OdResult res = eOk;

  OdIntArray leaderRootIndexes;
  pMLeader->getLeaderIndexes(leaderRootIndexes);
  for (OdUInt32 idxRoot = 0, szRoot = leaderRootIndexes.size(); idxRoot < szRoot; idxRoot++)
  {
    OdGePoint3dArray ptsDogleg;
    int leaderRootIndex = leaderRootIndexes[idxRoot];
    OdIntArray leaderLineIndexes;
    pMLeader->getLeaderLineIndexes(leaderRootIndex, leaderLineIndexes); 
    
    for (OdUInt32 idxLn = 0, szLn = leaderLineIndexes.size(); idxLn < szLn; idxLn++)
    {
      OdGePoint3dArray ptsLines;
      OdGePoint3d pt;
      int leaderLineIndex = leaderLineIndexes[idxLn];
      int szVrt = 0; pMLeader->numVertices(leaderLineIndex, szVrt);
      for (int idxVrt = 0; idxVrt < szVrt; idxVrt++)
      {
        pMLeader->getVertex(leaderLineIndex, idxVrt, pt);
        ptsLines.push_back(pt);
      }
      if (szVrt)
      {
        OdGeVector3d vDoglegDir;
        pMLeader->getDoglegDirection(leaderRootIndex, vDoglegDir);
        OdGePoint3d ptConnect;
        if (pMLeader->connectionPoint(- vDoglegDir, ptConnect) == eOk && pt != ptConnect)
        {
          if (ptsDogleg.size() < 2 || ptsDogleg[ptsDogleg.size() - 2] != pt || ptsDogleg.last() != ptConnect)
          {
            ptsDogleg.push_back(pt);
            ptsDogleg.push_back(ptConnect);
          }
          else
          {
            ODA_ASSERT_ONCE_X(MLEADER, !ptsDogleg.size()); // TODO test it with debugger
          }
        }
      }

      OdArray<OdDbMLeaderBreaks::BreakInfo> breaks;
      for (OdInt32 idxSeg = 0, szSeg = (OdInt32) ptsLines.size(); idxSeg < (szSeg - 1); idxSeg++)
      {
        OdGePoint3d ptStartSeg = ptsLines[idxSeg],
                    ptEndSeg = ptsLines[idxSeg + 1];
        OdArray<OdDbEntityPtr> entsLines;
        OdDbLinePtr pLine = OdDbLine::createObject();
        pLine->setStartPoint(ptStartSeg);
        pLine->setEndPoint(ptEndSeg);
        entsLines.push_back(pLine);

        eng.setDimensionEnts(entsLines, OdGeMatrix3d::kIdentity);
        eng.breakDimension(pBreakData);

        OdDbMLeaderBreaks::BreakInfo* pBrk = NULL;
        for (OdUInt32 idxEnt = 0, szEnt = eng.m_newDimEnts.size(); idxEnt < szEnt; idxEnt++)
        {
          OdDbLinePtr pLine = OdDbLine::cast(eng.m_newDimEnts[idxEnt]);
          ODA_ASSERT_ONCE_X(MLEADER, pLine.get());

          OdGePoint3d pt0 = pLine->startPoint(),
                      pt1 = pLine->endPoint();
          if (!pBrk && ptStartSeg == pt0 && ptEndSeg == pt1)
          {
            ODA_ASSERT_ONCE_X(MLEADER, (idxEnt + 1) == szEnt);
            break;
          }
          if (!pBrk)
          {
            breaks.push_back(OdDbMLeaderBreaks::BreakInfo());
            pBrk = &breaks.last();
            pBrk->m_nSegment = idxSeg;
          }
          OdUInt32 szPairs = pBrk->m_StartPoints.size();
          ODA_ASSERT_ONCE_X(MLEADER, szPairs == pBrk->m_EndPoints.size());
          ODA_ASSERT_ONCE_X(MLEADER, !szPairs || ptEndSeg == pBrk->m_EndPoints.last());

          if (szPairs)
            pBrk->m_EndPoints.last() = pt0;
          if (pt1 != ptEndSeg)
          {
            pBrk->m_StartPoints.push_back(pt1);
            pBrk->m_EndPoints.push_back(ptEndSeg); // may be changed by next idx
          }
        }
      } // end for of idxSeg
      eng.m_newDimEnts.clear();

      res = OdDbMLeaderBreaks::setBreaks(pMLeader, leaderLineIndex, breaks);
      ODA_ASSERT_ONCE_X(MLEADER, res == eOk)
      if (res != eOk)
        return res;

    } // end for idxLn (leaderLineIndex)

    OdUInt32 sz = ptsDogleg.size();
    if ( sz != 0 )
    {
      OdGePoint3dArray startBreakPoints, endBreakPoints;
      for (OdUInt32 idx = 0; idx < (sz - 1); idx += 2)
      {
        OdArray<OdDbEntityPtr> entsDogleg;
        OdGePoint3d ptStartSeg = ptsDogleg[idx],
                    ptEndSeg = ptsDogleg[idx + 1];
        OdDbLinePtr pLine = OdDbLine::createObject();
        pLine->setStartPoint(ptStartSeg);
        pLine->setEndPoint(ptEndSeg);
        entsDogleg.push_back(pLine);

        eng.setDimensionEnts(entsDogleg, OdGeMatrix3d::kIdentity);
        eng.breakDimension(pBreakData);

        for (OdUInt32 idxEnt = 0, szEnt = eng.m_newDimEnts.size(); idxEnt < szEnt; idxEnt++)
        {
          OdDbLinePtr pLine = OdDbLine::cast(eng.m_newDimEnts[idxEnt]);
          ODA_ASSERT_ONCE_X(MLEADER, pLine.get());

          OdGePoint3d pt0 = pLine->startPoint(),
                      pt1 = pLine->endPoint();
          if (ptStartSeg == pt0 && ptEndSeg == pt1)
          {
            ODA_ASSERT_ONCE_X(MLEADER, (idxEnt + 1) == szEnt);
            break;
          }
          OdUInt32 szPairs = startBreakPoints.size();
          ODA_ASSERT_ONCE_X(MLEADER, szPairs == endBreakPoints.size());
          ODA_ASSERT_ONCE_X(MLEADER, !szPairs || ptEndSeg == endBreakPoints.last());

          if (szPairs)
            endBreakPoints.last() = pt0;
          if (pt1 != ptEndSeg)
          {
            startBreakPoints.push_back(pt1);
            endBreakPoints.push_back(ptEndSeg); // may be changed by next idx
          }
        }
        eng.m_newDimEnts.clear();

      } // end for of idx
      res = OdDbMLeaderBreaks::setDoglegBreaks(pMLeader, leaderRootIndex, startBreakPoints, endBreakPoints);
      ODA_ASSERT_ONCE_X(MLEADER, res == eOk)
      if (res != eOk)
        return res;
    }
  } // end for idxRoot

  return res;
}
