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


#ifndef OD_DBTABLETEMPLATE_H
#define OD_DBTABLETEMPLATE_H

#include "TD_PackPush.h"
#include "DbObject.h"
#include "DbTableContent.h"

class OdDbTable;

/** \details
  This class represents TableTemplate objects which are used for capturing, storing 
  and managing table templates.

  \sa
  TD_Db

	<group OdDb_Classes>
*/
class DBENT_EXPORT OdDbTableTemplate : public OdDbTableContent
{
public:
  ODDB_DECLARE_MEMBERS(OdDbTableTemplate);

  OdDbTableTemplate();
  OdDbTableTemplate(const OdDbTable* pTable, OdDb::TableCopyOption nCopyOption);

  /** \details
    Captures table template contents.

    \param pTable [in]  Table from which to capture the template.
    \param nCopyOption [in]  Specifies which contents are captured while capturing a template.
    
    \returns
    Returns eOk if successful. Otherwise, returns eInvalidInput.
  */
  virtual OdResult capture(const OdDbTable* pTable, OdDb::TableCopyOption nCopyOption);
  
  /** \details
    Creates a table from the specified template contents.

    \param pTable [out]  Pointer to created table.
    \param nCopyOption [in]  Specifies which contents are copied while creating a table.
    
    \returns
    Returns eOk if successful. Otherwise, returns eInvalidInput.
  */
  virtual OdResult createTable(OdDbTable* pTable, OdDb::TableCopyOption nCopyOption);

  virtual OdResult  dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
};

/** \details
  This template class is a specialization of the OdSmartPtr class template for OdDbTableTemplate object pointers.
*/
typedef OdSmartPtr<OdDbTableTemplate> OdDbTableTemplatePtr;

#include "TD_PackPop.h"

#endif // OD_DBTABLETEMPLATE_H
