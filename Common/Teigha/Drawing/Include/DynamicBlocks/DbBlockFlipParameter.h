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

#ifndef _DbBlockFlipParameter_h_Included_
#define _DbBlockFlipParameter_h_Included_

#include "DbBlock2PtParameter.h"

class OdDbBlockFlipGrip;
typedef OdSmartPtr<OdDbBlockFlipGrip> OdDbBlockFlipGripPtr;

/** \details

    \sa
    TD_Db

    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlockFlipParameter : public OdDbBlock2PtParameter
{
public:
  ODDB_EVAL_DECLARE_MEMBERS(OdDbBlockFlipParameter);
  OdDbBlockFlipParameter();
  enum FlipState{ NotFlipped = 0, Flipped };

  FlipState flipState() const;
  void setFlipState(FlipState);
  FlipState updatedFlipState() const;
  void setUpdatedFlipState(FlipState);
  virtual void graphEvalEnd(bool);
  OdString flipLabel() const;
  void setFlipLabel(const OdString &);
  OdString flipLabelDescription()const;
  void setFlipLabelDescription(const OdString &);
  OdString baseStateLabel() const;
  void setBaseStateLabel(const OdString &);
  OdString flippedStateLabel()const;
  void setFlippedStateLabel(const OdString &);
  OdGePoint3d definitionLabelPoint() const;
  void setDefinitionLabelPoint(const OdGePoint3d &);
  OdDbBlockFlipGripPtr  getAssociatedFlipGrip( OdDb::OpenMode );
};

typedef OdSmartPtr<OdDbBlockFlipParameter>OdDbBlockFlipParameterPtr;

#endif // _DbBlockFlipParameter_h_Included_
