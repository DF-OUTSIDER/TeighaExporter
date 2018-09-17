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

#ifndef MODELERMARKERS_H
#define MODELERMARKERS_H /*!DOM*/

#define kSubentTypeStep 4
#define kFirstFaceMarker (OdDb::kFaceSubentType + kSubentTypeStep)
#define kFirstEdgeMarker (OdDb::kEdgeSubentType + kSubentTypeStep)
#define kFirstVertexMarker (OdDb::kVertexSubentType + kSubentTypeStep)

#define getFaceIndex(gsMarker) (((gsMarker) - OdDb::kFaceSubentType) / kSubentTypeStep)
#define getEdgeIndex(gsMarker) (((gsMarker) - OdDb::kEdgeSubentType) / kSubentTypeStep)
#define getVertexIndex(gsMarker) (((gsMarker) - OdDb::kVertexSubentType) / kSubentTypeStep)

#define getTypedIndex(gsMarker, type) (((gsMarker) - (type)) / kSubentTypeStep)

#define getFaceMarker(nIndex) ((nIndex) * kSubentTypeStep + OdDb::kFaceSubentType)
#define getEdgeMarker(nIndex) ((nIndex) * kSubentTypeStep + OdDb::kEdgeSubentType)
#define getVertexMarker(nIndex) ((nIndex) * kSubentTypeStep + OdDb::kVertexSubentType)

#define getTypedMarker(nIndex, type) ((nIndex) * kSubentTypeStep + (type))

#define isIndexValid(nIndex) ((nIndex)>0)
#define isMarkerInvalid(gsMarker) ((gsMarker) < kFirstFaceMarker || 0 == (gsMarker)%kSubentTypeStep)

#define isValidFaceMarker(gsMarker) ((gsMarker) >= kFirstFaceMarker && ((((gsMarker) - kFirstFaceMarker) % kSubentTypeStep) == 0))
#define isValidEdgeMarker(gsMarker) ((gsMarker) >= kFirstEdgeMarker && ((((gsMarker) - kFirstEdgeMarker) % kSubentTypeStep) == 0))
#define isValidVertexMarker(gsMarker) ((gsMarker) >= kFirstVertexMarker && ((((gsMarker) - kFirstVertexMarker) % kSubentTypeStep) == 0))

#endif //MODELERMARKERS_H
