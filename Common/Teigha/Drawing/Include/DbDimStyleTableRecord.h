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




#ifndef _ODDBDIMSTYLETABLERECORD_INCLUDED
#define _ODDBDIMSTYLETABLERECORD_INCLUDED

#include "TD_PackPush.h"

#include "DbSymbolTableRecord.h"
#include "DwgDeclareMembers.h"

/** \details
    This namespace contains utility functions for Dimension Style processing.

    <group TD_Namespaces>
*/
namespace OdDmUtil
{
  /** \details
    Returns the R14 DIMFIT value corresponding to the specified DIMATFIT and DIMTMOVE values.

    \param dimatfit [in]  DIMATFIT value.
    \param dimtmove [in]  DIMTMOVE value.
    
    \remarks
    The complete mapping is as follows:
    
    <table>
    DIMTMOVE    DIMAFIT   DIMFIT
    0           0         0
    0           1         1
    0           2         2
    0           3         3
    1           0         4
    1           1         4
    1           2         4
    1           3         4
    2           0         5
    2           1         5
    2           2         5
    2           3         5
    </table>
    
    All other input combinations return 3.
  */
  int TOOLKIT_EXPORT dimfit(
    int dimatfit, 
    int dimtmove);
  
  /** \details
    Returns the R14 DIMUNIT value corresponding to the specified DIMLUNIT and DIMFRAC values.

    \param dimlunit [in]  DIMLUNIT value.
    \param dimfrac [in]  DIMFRAC value.    

    \remarks
    The complete mapping is as follows:
    
    <table>
    DIMFRAC   DIMLUNIT  DIMUNIT
    0         1         1 
    0         2         2
    0         3         3
    0         4         4
    0         5         5
    0         6         8
    1         1         1 
    1         2         2
    1         3         3
    1         4         4
    1         5         5
    1         6         8
    2         1         1 
    2         2         2
    2         3         3
    2         4         6
    2         5         7
    2         6         8
    </table>
    
    All other input combinations return 2.
  */
  int TOOLKIT_EXPORT dimunit(
    int dimlunit, 
    int dimfrac);
  
  /** \details
    Returns the R15 DIMATFIT value corresponding to the specified R14 DIMFIT value.

    \param dimfit [in]  DIMFIT value.
    
    \remarks
    The complete mapping is as follows:
    
    <table>
    dimfit  dimatfit
    0       0
    1       1
    2       2
    3       3
    4       3
    5       3
    other   3
    </table>
    
  */
  int TOOLKIT_EXPORT dimatfit(
    int dimfit);
  
  /** \details
    Returns the R15 DIMTMOVE value corresponding to the specified R14 DIMFIT value.
    
    \param dimfit [in]  DIMFIT value.
    
    \remarks
    The complete mapping is as follows:
    
    <table>
    dimfit  dimtmove
    0       0
    1       0
    2       0
    3       0
    4       1
    5       2
    other   0
    </table>
  */
  int TOOLKIT_EXPORT dimtmove(
    int dimfit);
  
  /** \details
    Returns the R15 DIMLUNIT value corresponding to the specified R14 DUMUNIT value.
    
    \param dimunit [in]  DIMUNIT value.
    
    \remarks
    The complete mapping is as follows:
    
    <table>
    dimunit    dimlunit
    1          1
    2          2
    3          3
    4          4
    5          5
    6          4
    7          5
    8          6
    other      2
    </table>
  */
  int TOOLKIT_EXPORT dimlunit(
    int dimunit);
  
  /** \details
    Returns the R15 DIMFRAC value corresponding to the specified R14 DUMUNIT value.
    
    \param dimunit [in]  DIMUNIT value.
    
    \remarks
    The complete mapping is as follows:
    
    <table>
    dimunit    dimfrac
    1          0
    2          0
    3          0
    4          0
    5          0
    6          2
    7          2
    8          0
    other      0
    </table>
  */
  int TOOLKIT_EXPORT dimfrac(
    int dimunit);
    
  /** \details
    Returns the local name of the specified dimension arrowhead.
    \param blockId [in]  Object ID of the dimension arrowhead.
  */
  OdString TOOLKIT_EXPORT arrowName(
    OdDbObjectId blockId);
  
  /** \details
    Returns true if and only if the specified arrowhead name is that of a built-in arrowhead.

    \param arrowheadName [in]  Arrowhead name.

    \remarks
    The built-in arrowheads are as follows:
    
    <table>
    BLOCK NAME    UI STRING
    ArchTick      "Architectural tick"
    BoxBlank      "Box"
    BoxFilled     "Box filled"
    Closed        "Closed"
    ClosedBlank   "Closed blank"
    DatumBlank    "Datum triangle"
    DatumFilled   "Datum triangle filled"
    Dot           "Dot"
    DotBlank      "Dot blank"
    DotSmall      "Dot small"
    Integral      "Integral"
    None          "None"
    Oblique       "Oblique"
    Open          "Open"
    Open30        "Open 30"
    Open90        "Right angle"
    Origin        "Origin indicator"
    Origin2       "Origin indicator 2"
    Small         "Dot small blank"
    </table>
  */
  bool TOOLKIT_EXPORT isBuiltInArrow(
    const OdString& arrowheadName);
  
  /** \details
    Returns true if and only if the specified arrowhead name is that of a built-in arrowhead 
    that is treated as having zero length.  
      
    \param arrowheadName [in]  Arrowhead name.

    \remarks
    The built-in zero-length arrowheads are as follows:

    <table>
    BLOCK NAME
    ArchTick
    DotSmall
    Integral
    None
    Oblique
    Small
    </table>
  */
  bool TOOLKIT_EXPORT isZeroLengthArrow(
    const OdString& arrowheadName);
  
  /** \details
    Returns the Object ID of the arrowhead block with the specified arrowhead name.

    \param arrowheadName [in]  Arrowhead name.
    \param pDb [in]  Database containing the arrowhead.
  */
  OdDbObjectId TOOLKIT_EXPORT findArrowId(
    const OdString& arrowheadName, 
    const OdDbDatabase* pDb);
  
  /** \details
    Returns the Object ID of the arrowhead block with the specified arrowhead name, creating
    this block if necessary.

    \param arrowheadName [in]  Arrowhead name.
    \param pDb [in]  Database containing the arrowhead.
  */
  OdDbObjectId TOOLKIT_EXPORT getArrowId(
    const OdString& arrowheadName, 
    OdDbDatabase* pDb);

  /*
       OdString globalArrowName(const OdChar* pName);  
       OdString globalArrowName(OdDbObjectId blockId);
       OdString arrowName(const OdChar* pName);
  */

}

/** \details

    <group TD_Namespaces>
*/
namespace OdDb
{
  enum DimArrowFlags
  {
    kFirstArrow  = 0, // First arrowhead
    kSecondArrow = 1  // Second arrowhead
  };
}

/** \details
    This class represents Dimension Style records in the OdDbDimStyleTable in an OdDbDatabase instance.

    \sa
    TD_Db

    <group OdDb_Classes>
*/
class DBDIM_EXPORT OdDbDimStyleTableRecord: public OdDbSymbolTableRecord
{
public:
  DWGMAP_DECLARE_MEMBERS(OdDbDimStyleTableRecord);
  
  OdDbDimStyleTableRecord();

/*DOM-IGNORE-BEGIN*/  
#define VAR_DEF(type, name, dxf, def_value, metric_def_value, reserve1, reserve2)\
  /** Description: Returns the DIM##NAME value of this Dimension Style. \
    Remarks: ##remarks \
    Notes: ##notes \
  */ \
  virtual type dim##name() const; \
  \
  /** Description: Sets the DIM##NAME value for this Dimension Style.  \
    Arguments: val (I) New value for DIM##NAME.  ##RANGE \
    Remarks: ##remarks \
    Notes: ##notes \
    See Also: ##see_also \
  */ \
  virtual void setDim##name(type val);

#include "DimVarDefs.h"
#undef VAR_DEF
/*DOM-IGNORE-END*/
  
  void setDimblk(
    const OdString& val);

  void setDimblk1(
    const OdString& val);
  void setDimblk2(
  const OdString& val);
  void setDimldrblk(
    const OdString& val);

  // 2005-2006 support
  
  /** \details
    Returns the background text color and flags for this Dimension Style.
    \param bgrndTxtColor [out]  Receives the background text color.
  */
  OdUInt16 getBgrndTxtColor(OdCmColor& bgrndTxtColor) const;
  
  /** \details
    Sets the background text color and flags for this Dimension Style.
    \param bgrndTxtColor [in]  Background text color.
    \param bgrndTxtFlags [in]  Background text flags.
  */
  void setBgrndTxtColor(const OdCmColor& bgrndTxtColor, OdUInt16 bgrndTxtFlags);

  /** \details
    Returns the extension line fixed length enable flag for this Dimension Style.
  */
  bool getExtLineFixLenEnable() const;

  /** \details
    Controls the extension line fixed length enable flag for this Dimension Style.
    \param extLineFixLenEnable [in]  Extension line fixed length flag.
  */
  void setExtLineFixLenEnable(bool extLineFixLenEnable);

  /** \details
    Returns the extension line fixed length for this Dimension Style.
  */
  double getExtLineFixLen() const;

  /** \details
    Sets the extension line fixed length for this Dimension Style.
    \param extLineFixLen [in]  Extension line fixed length.
  */
  void setExtLineFixLen(double extLineFixLen);

  /** \details
    Returns the arc symbol type for this Dimension Style.
    \remarks
    getArcSymbolType() returns one of the following:
    
    <table>
    Value   Description
    0       Arc symbol precedes text.
    1       Arc symbol is above text.
    2       No arc symbol.
    </table>
  */
  OdUInt16 getArcSymbolType() const;
  /** \details
    Sets the arc symbol type for this Dimension Style.
    \param arcSymbolType [in]  Arc symbol type.

    \remarks
    getArcSymbolType() must be one of the following:
    
    <table>
    Value   Description
    0       Arc symbol precedes text.
    1       Arc symbol is above text.
    2       No arc symbol.
    </table>
  */
  void setArcSymbolType(OdUInt16 arcSymbolType);

  /** \details
    Returns the jog angle for this Dimension Style.
    \remarks
    All angles are expressed in radians. 
  */
  double getJogAngle() const;
  /** \details
    Sets the jog angle for this Dimension Style.
    \param jogAngle [in]  Jog angle. 
    \remarks
    All angles are expressed in radians. 
  */
  void setJogAngle(double jogAngle);

  /** \details
    Returns the Object ID of the dimension line linetype for this Dimension Style.
  */
  OdDbObjectId getDimLinetype() const;

  /** \details
    Sets the Object ID of the dimension line linetype for this Dimension Style.

    \param linetypeId [in]  Dimension line linetype Object ID.
  */
  void setDimLinetype(const OdDbObjectId& linetypeId);

  /** \details
    Returns the Object ID of the first extension line linetype for this Dimension Style.
  */
  OdDbObjectId getDimExt1Linetype() const;

  /** \details
    Sets the Object ID of the first extension line linetype for this Dimension Style.
    
    \param linetypeId [in]  First extension line linetype Object ID.
  */
  void setDimExt1Linetype(const OdDbObjectId& linetypeId);

  /** \details
    Returns the Object ID of the second extension line linetype for this Dimension Style.
  */
  OdDbObjectId getDimExt2Linetype() const;
  /** \details
    Sets the Object ID of the second extension line linetype for this Dimension entity.
    
    \param linetypeId [in]  Second extension line linetype Object ID.
  */
  void setDimExt2Linetype(const OdDbObjectId& linetypeId);

  double jogSymbolHeight();

  void setJogSymbolHeight(double value);
  void setDIMTALN(bool value);
  bool getDIMTALN() const;

  void setDIMBREAK(double value);
  double getDIMBREAK() const;

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);
  
  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;
  
  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);
  
  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;
  
  virtual OdResult dxfInFields_R12(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields_R12(
    OdDbDxfFiler* pFiler) const;
  
  virtual OdResult subGetClassID(
    void* pClsid) const;

  /** \details
    Returns the Object ID of the arrowhead block for the specified arrowhead type.

    \param arrowType [in]  Arrow type.
    
    \remarks
    arrowType must be one of the following:
    
    <table>
    Name           Value    Description    
    kFirstArrow    0        First arrowhead
    kSecondArrow   1        Second arrowhead
    </table>
  */
  OdDbObjectId arrowId(
    OdDb::DimArrowFlags arrowType) const;

  /** \details
    Returns true if and only if this Dimension Style has been modified for recompute.
  */
  bool isModifiedForRecompute() const;

  /*
  OdString dimpost() const;
  OdString dimapost() const;
  OdString dimblk() const;
  OdString dimblk1() const;
  OdString dimblk2() const;
  
  int dimfit() const;
  int dimunit() const;
    
  void setDimfit(int fit);
  void setDimunit(int unit);
  */
  OdResult subErase( bool erasing );
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbDimStyleTableRecord object pointers.
*/
typedef OdSmartPtr<OdDbDimStyleTableRecord> OdDbDimStyleTableRecordPtr;

#include "TD_PackPop.h"

#endif // _ODDBDIMSTYLETABLERECORD_INCLUDED

