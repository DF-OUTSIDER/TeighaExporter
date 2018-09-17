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

#ifndef ODDGNLS_DBLSPOINTCOMPONENT_H
#define ODDGNLS_DBLSPOINTCOMPONENT_H

#include "DbLSComponent.h"
#include "DbLSSymbolReference.h"

#include "TD_PackPush.h"

/** \details
    Dgn Line Style Point Component.
    \sa
    TD_Db
    <group OdDbLS_Classes> 
*/
class ODDGNLS_EXPORT OdDbLSPointComponent : public OdDbLSComponent
{
  public:
    ODDB_DECLARE_MEMBERS(OdDbLSPointComponent);

    OdDbLSPointComponent();

    /** \details
      Inserts new symbol for stroke into this point component.

      \param symCompId [in]  Identifier of symbol to set.
      \param fXOffset [in]  Symbol offset in X direction.
      \param fYOffset [in]  Symbol offset in Y direction.
      \param fAngle [in]  Symbol rotation angle in radians.
      \param nStroke [in]  Index of stroke for symbol insert.
      \returns
      Returns pointer to newly added symbol reference which could be used for additional modifications.
    */
    OdDbLSSymbolReference *insertSymbolForStroke(const OdDbObjectId &symCompId, double fXOffset = 0.0, double fYOffset = 0.0, double fAngle = 0.0,
                                                 OdInt32 nStroke = 0);
    /** \details
      Inserts new symbol for stroke into this point component.

      \param symCompId [in]  Identifier of symbol to set.
      \param nStroke [in]  Index of stroke for symbol insert.
      \returns
      Returns pointer to newly added symbol reference which could be used for additional modifications.
    */
    OdDbLSSymbolReference *insertSymbolForStroke(const OdDbObjectId &symCompId, OdInt32 nStroke);
    /** \details
      Inserts new symbol for stroke into this point component as copy of existing symbol.

      \param pSymbol [in]  Symbol reference for clone.
      \param nStroke [in]  Index of stroke for symbol insert.
      \returns
      Returns pointer to newly added symbol reference which could be used for additional modifications.
    */
    OdDbLSSymbolReference *insertSymbolForStroke(const OdDbLSSymbolReference *pSymbol, OdInt32 nStroke);

    /** \details
      Inserts new symbol for vertex into this point component.

      \param symCompId [in]  Identifier of symbol to set.
      \param fXOffset [in]  Symbol offset in X direction.
      \param fYOffset [in]  Symbol offset in Y direction.
      \param fAngle [in]  Symbol rotation angle in radians.
      \param at [in]  Type of vertex for this symbol.
      \returns
      Returns pointer to newly added symbol reference which could be used for additional modifications.
    */
    OdDbLSSymbolReference *insertSymbolForVertex(const OdDbObjectId &symCompId, double fXOffset = 0.0, double fYOffset = 0.0, double fAngle = 0.0,
                                                 OdDbLSSymbolReference::VertexMask at = OdDbLSSymbolReference::kLSAtOrigin);
    /** \details
      Inserts new symbol for vertex into this point component.

      \param symCompId [in]  Identifier of symbol to set.
      \param at [in]  Type of vertex for this symbol.
      \returns
      Returns pointer to newly added symbol reference which could be used for additional modifications.
    */
    OdDbLSSymbolReference *insertSymbolForVertex(const OdDbObjectId &symCompId, OdDbLSSymbolReference::VertexMask at);
    /** \details
      Inserts new symbol for vertex into this point component as copy of existing symbol.

      \param pSymbol [in]  Symbol reference for clone.
      \param nStroke [in]  Index of stroke for symbol insert.
      \returns
      Returns pointer to newly added symbol reference which could be used for additional modifications.
    */
    OdDbLSSymbolReference *insertSymbolForVertex(const OdDbLSSymbolReference *pSymbol, OdDbLSSymbolReference::VertexMask at);

    /** \details
      Removes existing symbol for stroke from this point component.

      \param nStroke [in]  Index of stroke for remove symbol.
    */
    void deleteSymbolForStroke(OdInt32 nStroke);
    /** \details
      Removes existing symbol for vertex from this point component.

      \param at [in]  Type of vertex for remove symbol.
    */
    void deleteSymbolForVertex(OdDbLSSymbolReference::VertexMask at);

    /** \details
      Returns count of symbols in this point component symbols list.

      \remarks
      Returns count of both symbol lists for stroke and for vertex.
    */
    OdUInt32 numberSymbols() const;

    /** \details
      Returns stroke component identifier associated with this point component.
    */
    OdDbObjectId strokeComponent() const;
    /** \details
      Sets stroke component identifier associated with this point component.

      \param strokeCompId [in]  Identifier of stroke component.
    */
    void setStrokeComponent(const OdDbObjectId &strokeCompId);

    /** \details
      Returns symbol by index in point component symbols list.

      \param nSym [in]  Index of symbol.
    */
    OdDbLSSymbolReference *symbol(OdUInt32 nSym);
    /** \details
      Returns symbol by stroke index.

      \param nStroke [in]  Stroke index.
    */
    OdDbLSSymbolReference *symbolForStroke(OdInt32 nStroke);
    /** \details
      Returns symbol by vertex type.

      \param at [in]  Type of vertex.
    */
    OdDbLSSymbolReference *symbolForVertex(OdDbLSSymbolReference::VertexMask at);
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbLSPointComponent object pointers.
*/
typedef OdSmartPtr<OdDbLSPointComponent> OdDbLSPointComponentPtr;

#include "TD_PackPop.h"

#endif // ODDGNLS_DBLSPOINTCOMPONENT_H
