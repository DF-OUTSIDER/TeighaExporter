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



#ifndef BLKVAR_DEF
#define BLKVAR_DEF  VAR_DEF
#endif

#ifndef ANNOVAR_DEF
#define ANNOVAR_DEF  VAR_DEF
#endif

#ifndef ODTDB_LINEWEIGHT
#define ODTDB_LINEWEIGHT OdDb::LineWeight /*!DOM*/
#define DOUNDEF_ODTDB_LINEWEIGHT
#endif

#ifndef ODTDIST
#define ODTDIST  double /*!DOM*/
#define DOUNDEF_ODTDIST
#endif

#ifndef ODTORIENT
#define ODTORIENT  double /*!DOM*/
#define DOUNDEF_ODTORIENT
#endif

#ifndef ODTUNITLESS
#define ODTUNITLESS  double /*!DOM*/
#define DOUNDEF_ODTUNITLESS
#endif

#ifndef ODTDECSEP
#define ODTDECSEP  OdInt16 /*!DOM*/
#define DOUNDEF_ODTDECSEP
#endif

#ifndef ODTARROWBLK
#define ODTARROWBLK  OdDbHardPointerId /*!DOM*/
#define DOUNDEF_ODTARROWBLK
#endif

#ifndef ODTTEXTSTYLEID
#define ODTTEXTSTYLEID  OdDbHardPointerId /*!DOM*/
#define DOUNDEF_ODTTEXTSTYLEID
#endif


#ifndef ODTLINETYPEID
#define ODTLINETYPEID  OdDbHardPointerId /*!DOM*/
#define DOUNDEF_ODTLINETYPEID
#endif

#ifndef NullIdAllowed
#define NullIdAllowed true
#define DOUNDEF_NullIdAllowed
#endif


/** **********************************************
 *
 * Dimension variables definitions
*/

//          Type              Name    DXF     Default value         Metric Def Value      Reserve1   Reserve2erve2

/**
  <title DIMADEC>
  <toctitle DIMADEC>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   0
   <b>Range</b>           –1 ... 8
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R14+
  </table>
   
  DIMADEC specifies the number of decimal places in angular dimensions. If DIMADEC == –1, the number of decimal places is
  determined by DIMDEC.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimadec()
   OdDbDatabase::setDimadec()
   &nbsp; 
   OdDbDimension::dimadec()
   OdDbDimension::setDimadec()
   &nbsp; 
   OdDbDimStyleTableRecord::dimadec()
   OdDbDimStyleTableRecord::setDimadec()
  </table>
  
  \sa
  <link DIMDEC, DIMDEC>

<group TD_SysVars_D>
  */
//<dom_var_def>OdInt16 DIMADEC;
VAR_DEF(OdInt16,              adec,   179,    (0),                  (0),                  (),        ValidateRange(0, 8))

/**
  <title DIMALT>
  <toctitle DIMALT>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   false
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
  
  DIMALT controls the display of alternate measurements in dimensions.
   
  <table>
   <b>Value</b>   <b>Description</b>
   false          Disabled
   true           Enabled
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimalt()
   OdDbDatabase::setDimalt()
   &nbsp; 
   OdDbDimension::dimalt()
   OdDbDimension::setDimalt()
   &nbsp; 
   OdDbDimStyleTableRecord::dimalt()
   OdDbDimStyleTableRecord::setDimalt()
   </table>
   
 <group TD_SysVars_D>
 */
//<dom_var_def>bool DIMALT;
VAR_DEF(bool,                 alt,    170,    (false),              (false),              (),        ValidateNone())

/**
  <title DIMALTD>
  <toctitle DIMALTD>
   
  <table>
   <b>Type</b>            OdUInt16
   <b>Initial value</b>   2 | 3
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R13+
  </table>
   
   DIMALTD specifies the number of decimal places in alternate measurements in dimensions.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimaltd()
   OdDbDatabase::setDimaltd()
   &nbsp; 
   OdDbDimension::dimaltd()
   OdDbDimension::setDimaltd()
   &nbsp;
   OdDbDimStyleTableRecord::dimaltd()
   OdDbDimStyleTableRecord::setDimaltd()
  </table>
 
<group TD_SysVars_D>
*/
//<dom_var_def>OdUInt16 DIMALTD;
VAR_DEF(OdUInt16,             altd,   171,    (2),                  (3),                  (),        ValidateRange(0,8))

/**
  <title DIMALTF>
   <toctitle DIMALTF>
   
   <table>
   <b>Type</b>            double
   <b>Initial value</b>   25.4 | (1.0 / 25.4)
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
   </table>
   
   DIMALTF specifies the distance multiplier for alternate
   measurements in dimensions.
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimaltf()
   OdDbDatabase::setDimaltf()
   &nbsp; 
   OdDbDimension::dimaltf()
   OdDbDimension::setDimaltf()
   &nbsp; 
   OdDbDimStyleTableRecord::dimaltf()
   OdDbDimStyleTableRecord::setDimaltf()
   </table>
   
   <group TD_SysVars_D>
*/
//<dom_var_def>double DIMALTF;
VAR_DEF(double,               altf,   143,    (25.4),               (0.03937007874016),   (),        ValidateRange(DBL_MIN, DBL_MAX))

/**
  <title DIMALTRND>
  <toctitle DIMALTRND>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   0.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2000+
  </table>
   
   DIMALTRND specifies the rounding of alternate measurements in dimensions. If DIMALTRND == 0.0, no rounding is performed.
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimaltrnd()
   OdDbDatabase::setDimaltrnd()
   &nbsp; 
   OdDbDimension::dimaltrnd()
   OdDbDimension::setDimaltrnd()
   &nbsp; 
   OdDbDimStyleTableRecord::dimaltrnd()
   OdDbDimStyleTableRecord::setDimaltrnd()
   </table>
   
  <group TD_SysVars_D>
*/
//<dom_var_def>double DIMALTRND;
VAR_DEF(ODTDIST,              altrnd, 148,    (0.0),                (0.0),                (),        ValidateRange(0,DBL_MAX))

/**
  <title DIMALTTD>
  <toctitle DIMALTTD>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   2(imperial) | 3(metric)
   <b>Range</b>           0 ... 8
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R13+
   </table>
   
   DIMALTTD specifies the number of decimal places in tolerance values in alternate measurements in dimensions.
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimalttd()
   OdDbDatabase::setDimalttd()
   &nbsp; 
   OdDbDimension::dimalttd()
   OdDbDimension::setDimalttd()
   &nbsp; 
   OdDbDimStyleTableRecord::dimalttd()
   OdDbDimStyleTableRecord::setDimalttd()
   </table>
   
  <group TD_SysVars_D>
*/
//<dom_var_def>OdInt16 DIMALTTD;
VAR_DEF(OdInt16,              alttd,  274,    (2),                  (3),                  (),        ValidateRange(0,8))

/**
  <title DIMALTTZ>
  <toctitle DIMALTTZ>
   
   <table>
   <b>Type</b>            OdUInt8
   <b>Initial value</b>   0
   <b>Range</b>           0 ... 15
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R13+
   </table>
   
   DIMALTTZ controls zero suppression in tolerance values in alternate measurements in dimensions.
   
   <table>
   <b>Value</b>   <b>Description</b>
   0              Suppress zero feet and exactly zero inches.
   1              Include zero feet and exactly zero inches.
   2              Include zero feet and suppress exactly zero inches.
   3              Suppress zero feet and include exactly zero inches.
   4              Suppress leading decimal zeros.
   8              Suppress trailing decimal zeros.
   12             Suppress leading and trailing decimal zeros.
   </table>
   
   Values 0-3 affect only feet-and-inch dimensions, and may be combined with values 4-12.
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimalttz()
   OdDbDatabase::setDimalttz()
   &nbsp; 
   OdDbDimension::dimalttz()
   OdDbDimension::setDimalttz()
   &nbsp; 
   OdDbDimStyleTableRecord::dimalttz()
   OdDbDimStyleTableRecord::setDimalttz()
   </table>

  <group TD_SysVars_D>
*/
//<dom_var_def>OdUInt8 DIMALTTZ;
VAR_DEF(OdUInt8,              alttz,  286,    (0),                  (0),                  (),        ValidateRange(0,15))

/**
  <title DIMALTU>
  <toctitle DIMALTU>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   2
   <b>Range</b>           1 ... 8
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R13+
  </table>
   
  DIMALTU controls the formatting for alternate units in dimensions.
   
  <table>
   <b>Value</b>   <b>Description</b>
   1              Scientific
   2              Decimal
   3              Engineering
   4              Architectural (stacked)
   5              Fractional (stacked)
   6              Architectural
   7              Fractional
   8              Microsoft Windows Desktop
  </table>
   
   Values 0-3 affect only feet-and-inch dimensions, and may be combined with values 4-12.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimaltu()
   OdDbDatabase::setDimaltu()
   &nbsp; 
   OdDbDimension::dimaltu()
   OdDbDimension::setDimaltu()
   &nbsp; 
   OdDbDimStyleTableRecord::dimaltu()
   OdDbDimStyleTableRecord::setDimaltu()
  </table>

  <group TD_SysVars_D>
   */
//<dom_var_def>OdInt16 DIMALTU;
VAR_DEF(OdInt16,              altu,   273,    (2),                  (2),                  (),        ValidateRange(1,8))

/**
  <title DIMALTZ>
  <toctitle DIMALTZ>
   
   <table>
   <b>Type</b>            OdUInt8
   <b>Initial value</b>   0
   <b>Range</b>           0 ... 15
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R13+
   </table>
   
   DIMALTZ controls zero suppression in alternate measurements in dimensions.
   
   <table>
   <b>Value</b>   <b>Description</b>
   0              Suppress zero feet and exactly zero inches.
   1              Include zero feet and exactly zero inches.
   2              Include zero feet and suppress exactly zero inches.
   3              Suppress zero feet and include exactly zero inches.
   4              Suppress leading decimal zeros.
   8              Suppress trailing decimal zeros.
   12             Suppress leading and trailing decimal zeros.
   </table>
   
   Values 0-3 affect only feet-and-inch dimensions, and may be combined with values 4-12.
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimaltz()
   OdDbDatabase::setDimaltz()
   &nbsp; 
   OdDbDimension::dimaltz()
   OdDbDimension::setDimaltz()
   &nbsp; 
   OdDbDimStyleTableRecord::dimaltz()
   OdDbDimStyleTableRecord::setDimaltz()
   </table>  

   <group TD_SysVars_D>
   */
//<dom_var_def>OdUInt8 DIMALTZ;
VAR_DEF(OdUInt8,              altz,   285,    (0),                  (0),                  (),        ValidateRange(0,15))

/**
  <title DIMAPOST>
  <toctitle DIMAPOST>
   
   <table>
   <b>Type</b>            OdString
   <b>Initial value</b>   ""
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
   </table>
   
   DIMAPOST specifies a prefix and/or suffix for the alternate
   measurements text in dimensions. DIMAPOST is in the form
   prefix\<\>suffix. The '\<\>' in DIMAPOST is replaced by the
   alternate measurements text.
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimapost()
   OdDbDatabase::setDimapost()
   &nbsp; 
   OdDbDimension::dimapost()
   OdDbDimension::setDimapost()
   &nbsp; 
   OdDbDimStyleTableRecord::dimapost()
   OdDbDimStyleTableRecord::setDimapost()
   </table>

   <group TD_SysVars_D>
   */
//<dom_var_def>OdString DIMAPOST;
VAR_DEF(OdString,             apost,  4,      (OD_T("")),                 (OD_T("")),                 (),        ValidateNone())

/**
  <title DIMASZ>
  <toctitle DIMASZ>
   
   <table>
   <b>Type</b>            double
   <b>Initial value</b>   0.18 (imperial) | 2.5 (metric)
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
   </table>
   
   DIMASZ specifies the size of dimension and leader arrowheads.
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimasz()
   OdDbDatabase::setDimasz()
   &nbsp; 
   OdDbDimension::dimasz()
   OdDbDimension::setDimasz()
   &nbsp; 
   OdDbDimStyleTableRecord::dimasz()
   OdDbDimStyleTableRecord::setDimasz()
   &nbsp; 
   OdDbLeader::dimasz()
   OdDbLeader::setDimasz()
   </table> 

<group TD_SysVars_D>
   */
//<dom_var_def>double DIMASZ;
VAR_DEF(ODTDIST,              asz,    41,     (0.18),               (2.5),                (),        ValidateRange(0,DBL_MAX))

/**
  <title DIMATFIT>
  <toctitle DIMATFIT>
   
   <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   3
   <b>Range</b>           0 ... 3
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2000+
   </table>
   
   DIMATFIT specifies how arrows and text are placed when they do not fit within the extension lines of dimensions.
   
   <table>
   <b>Value</b>   <b>Description</b>
   0              Moves text and arrows outside extension lines.
   1              Moves arrows, then text, outside extension lines. When DIMTMOVE == 1, a leader is added to moved text
   2              Moves text, then arrows, outside extension lines.
   3              Moves text or arrows for best fit.
   </table>
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimatfit()
   OdDbDatabase::setDimatfit()
   &nbsp; 
   OdDbDimension::dimatfit()
   OdDbDimension::setDimatfit()
   &nbsp; 
   OdDbDimStyleTableRecord::dimatfit()
   OdDbDimStyleTableRecord::setDimatfit()
   </table>

   <group TD_SysVars_D>
   */
//<dom_var_def>OdInt16 DIMATFIT;
ANNOVAR_DEF(OdInt16,          atfit,  289,    (3),                  (3),                  (),        ValidateRange(0,3))

/**
  <title DIMAUNIT>
  <toctitle DIMAUNIT>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   0
   <b>Range</b>           0 ... 4
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R13+
  </table>
   
   DIMAUNIT specifies the formatting for units in angular dimensions.
   
   <table>
   <b>Value</b>   <b>Description</b>
   0              Decimal Degrees
   1              Degrees-Minutes-Seconds
   2              Gradians
   3              Radians
   4              Surveyor's Units
   </table>
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimaunit()
   OdDbDatabase::setDimaunit()
   &nbsp; 
   OdDbDimension::dimaunit()
   OdDbDimension::setDimaunit()
   &nbsp; 
   OdDbDimStyleTableRecord::dimaunit()
   OdDbDimStyleTableRecord::setDimaunit()
   </table> 

   <group TD_SysVars_D>
   */
//<dom_var_def>OdInt16 DIMAUNIT;
VAR_DEF(OdInt16,              aunit,  275,    (0),                  (0),                  (),        ValidateRange(0,4))

/**
  <title DIMAZIN>
  <toctitle DIMAZIN>
   
   <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   0
   <b>Range</b>           0 ... 3
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2000+
   </table>
   
   DIMAZIN controls zero suppression in angular dimensions.
   
   <table>
   <b>Value</b>   <b>Description</b>
   0              Display leading and trailing decimal zeros.
   1              Suppress leading decimal zeros.
   2              Suppress trailing decimal zeros.
   3              Suppress leading and trailing decimal zeros.
   </table>
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimazin()
   OdDbDatabase::setDimazin()
   &nbsp; 
   OdDbDimension::dimazin()
   OdDbDimension::setDimazin()
   &nbsp; 
   OdDbDimStyleTableRecord::dimazin()
   OdDbDimStyleTableRecord::setDimazin()
   </table> 

   <group TD_SysVars_D>
   */
//<dom_var_def>OdInt16 DIMAZIN;
VAR_DEF(OdInt16,              azin,   79,     (0),                  (0),                  (),        ValidateRange(0,3))

/**
  <title DIMBLK>
  <toctitle DIMBLK>
   
   <table>
   <b>Type</b>            OdDbObjectId
   <b>Initial value</b>   OdDbObjectId::kNull
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
   </table>
   
   DIMBLK specifies the arrowhead block at the ends of dimension lines and leader lines. A null value 
   specifies the default arrowhead is to be used.
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimblk()
   OdDbDatabase::setDimblk()
   &nbsp; 
   OdDbDimension::dimblk()
   OdDbDimension::setDimblk()
   &nbsp; 
   OdDbDimStyleTableRecord::dimblk()
   OdDbDimStyleTableRecord::setDimblk()
   </table>

   <group TD_SysVars_D>
   */
//<dom_var_def>OdDbObjectId DIMBLK;
BLKVAR_DEF(ODTARROWBLK,       blk,    342,    (OdDbObjectId::kNull),(OdDbObjectId::kNull),(),        ValidateDimBlock())

/**
  <title DIMBLK1>
  <toctitle DIMBLK1>
   
  <table>
   <b>Type</b>            OdDbObjectId
   <b>Initial value</b>   OdDbObjectId::kNull
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  DIMBLK1 specifies the first arrowhead block at the ends of dimension lines when DIMSAH == true. 
  A null value specifies the default arrowhead is to be used.
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimblk1()
   OdDbDatabase::setDimblk1()
   &nbsp; 
   OdDbDimension::dimblk1()
   OdDbDimension::setDimblk1()
   &nbsp; 
   OdDbDimStyleTableRecord::dimblk1()
   OdDbDimStyleTableRecord::setDimblk1()
   </table>

   <group TD_SysVars_D>
   */
//<dom_var_def>OdDbObjectId DIMBLK1;
BLKVAR_DEF(ODTARROWBLK,       blk1,   343,    (OdDbObjectId::kNull),(OdDbObjectId::kNull),(),        ValidateDimBlock())

/**
 <title DIMBLK2>
   <toctitle DIMBLK2>
   
   <table>
   <b>Type</b>            OdDbObjectId
   <b>Initial value</b>   OdDbObjectId::kNull
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
   </table>
   
   DIMBLK2 specifies the second arrowhead block at the ends of dimension lines when DIMSAH == true. 
   A null value specifies the default arrowhead is to be used.
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;
   OdDbDatabase::dimblk2()
   OdDbDatabase::setDimblk2()
   &nbsp; 
   OdDbDimension::dimblk2()
   OdDbDimension::setDimblk2()
   &nbsp; 
   OdDbDimStyleTableRecord::dimblk2()
   OdDbDimStyleTableRecord::setDimblk2()
   </table>       

   <group TD_SysVars_D>
   */
//<dom_var_def>OdDbObjectId DIMBLK2;
BLKVAR_DEF(ODTARROWBLK,       blk2,   344,    (OdDbObjectId::kNull),(OdDbObjectId::kNull),(),        ValidateDimBlock())

/**
  <title DIMCEN>
   <toctitle DIMCEN>
   
   <table>
   <b>Type</b>            double
   <b>Initial value</b>   0.09 | 2.5
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
   </table>
   
   DIMCEN controls the generation of arc and circle center marks
   and lines. The absolute value of DIMCEN specifies the size of
   the center mark, which half the length of the center mark,
   which is also the length of the portion of a center line
   beyond the arc or circle, as well as the length of the gap
   between the center mark and line. The sign of DIMCEN controls
   the generation of center marks and center lines:
   
   <table>
   <b>Value</b>   <b>Description</b>
   &lt;0            Center marks and center lines
   0                No center marks or center lines
   &gt;0            Center lines
   </table>
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimcen()
   OdDbDatabase::setDimcen()
   &nbsp; 
   OdDbDimension::dimcen()
   OdDbDimension::setDimcen()
   &nbsp; 
   OdDbDimStyleTableRecord::dimcen()
   OdDbDimStyleTableRecord::setDimcen()
   </table> 

<group TD_SysVars_D>
   */
//<dom_var_def>double DIMCEN;
VAR_DEF(ODTDIST,              cen,    141,    (0.09),               (2.5),                (),        ValidateNone())

/**
  <title DIMCLRD>
  <toctitle DIMCLRD>
   
   <table>
   <b>Type</b>            OdCmColor
   <b>Initial value</b>   OdCmEntityColor::kByBlock
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
   </table>
   
   DIMCLRD specifies the color of dimension lines, leader lines,
   frames, and arrowheads.
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimclrd()
   OdDbDatabase::setDimclrd()
   &nbsp; 
   OdDbDimension::dimclrd()
   OdDbDimension::setDimclrd()
   &nbsp; 
   OdDbDimStyleTableRecord::dimclrd()
   OdDbDimStyleTableRecord::setDimclrd()
   &nbsp; 
   OdDbFcf::dimclrd()
   OdDbFcf::setDimclrd()
   &nbsp; 
   OdDbLeader::dimclrd()
   OdDbLeader::setDimclrd()
   </table> 

<group TD_SysVars_D>
   */
//<dom_var_def>OdCmColor DIMCLRD;
VAR_DEF(OdCmColor,            clrd,   176,(OdCmEntityColor::kByBlock),(OdCmEntityColor::kByBlock),(),ValidateColor())

/**
  <title DIMCLRE>
   <toctitle DIMCLRE>
   
   <table>
   <b>Type</b>            OdCmColor
   <b>Initial value</b>   OdCmEntityColor::kByBlock
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
   </table>
   
   DIMCLRE specifies the color of extension lines, enter marks,
   and centerlines.
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimclre()
   OdDbDatabase::setDimclre()
   &nbsp; 
   OdDbDimension::dimclre()
   OdDbDimension::setDimclre()
   &nbsp; 
   OdDbDimStyleTableRecord::dimclre()
   OdDbDimStyleTableRecord::setDimclre()
   </table>  

<group TD_SysVars_D>
   */
//<dom_var_def>OdCmColor DIMCLRE;
VAR_DEF(OdCmColor,            clre,   177,(OdCmEntityColor::kByBlock),(OdCmEntityColor::kByBlock),(),ValidateColor())

/**
  <title DIMCLRT>
   <toctitle DIMCLRT>
   
   <table>
   <b>Type</b>            OdCmColor
   <b>Initial value</b>   OdCmEntityColor::kByBlock
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
   </table>
   
   DIMCLRT specifies the color of dimension text.
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimclrt()
   OdDbDatabase::setDimclrt()
   &nbsp; 
   OdDbDimension::dimclrt()
   OdDbDimension::setDimclrt()
   &nbsp; 
   OdDbDimStyleTableRecord::dimclrt()
   OdDbDimStyleTableRecord::setDimclrt()
   &nbsp; 
   OdDbFcf::dimclrt()
   OdDbFcf::setDimclrt()
   </table> 

<group TD_SysVars_D>
   */
//<dom_var_def>OdCmColor DIMCLRT;
VAR_DEF(OdCmColor,            clrt,   178,(OdCmEntityColor::kByBlock),(OdCmEntityColor::kByBlock),(),ValidateColor())

/**
  <title DIMDEC>
  <toctitle DIMDEC>
   
   <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   4 | 2
   <b>Range</b>           0 ... 8
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
   DIMDEC specifies the number of decimal places in primary units in dimensions.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimdec()
   OdDbDatabase::setDimdec()
   &nbsp; 
   OdDbDimension::dimdec()
   OdDbDimension::setDimdec()
   &nbsp; 
   OdDbDimStyleTableRecord::dimdec()
   OdDbDimStyleTableRecord::setDimdec()
  </table> 

<group TD_SysVars_D>
  */
//<dom_var_def>OdInt16 DIMDEC;
VAR_DEF(OdInt16,              dec,    271,    (4),                  (2),                  (),        ValidateRange(0,8))

/**
  <title DIMDLE>
   <toctitle DIMDLE>
   
   <table>
   <b>Type</b>            double
   <b>Initial value</b>   0.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
   </table>
   
   DIMDLE specifies the distance dimension lines extend beyond extension lines when arrowheads are architectural, 
   integral, oblique, tick, or none.
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimdle()
   OdDbDatabase::setDimdle()
   &nbsp; 
   OdDbDimension::dimdle()
   OdDbDimension::setDimdle()
   &nbsp; 
   OdDbDimStyleTableRecord::dimdle()
   OdDbDimStyleTableRecord::setDimdle()
   </table>    

<group TD_SysVars_D>
   */
//<dom_var_def>double DIMDLE;
VAR_DEF(ODTDIST,              dle,    46,     (0.0),                (0.0),                (),        ValidateRange(0,DBL_MAX))

/**
  <title DIMDLI>
  <toctitle DIMDLI>
   
   <table>
   <b>Type</b>            double
   <b>Initial value</b>   0.38 | 3.75
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
   </table>
   
   DIMDLI specifies distance between dimension lines for
   baseline dimensions.
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimdli()
   OdDbDatabase::setDimdli()
   &nbsp; 
   OdDbDimension::dimdli()
   OdDbDimension::setDimdli()
   &nbsp; 
   OdDbDimStyleTableRecord::dimdli()
   OdDbDimStyleTableRecord::setDimdli()
   </table>  

<group TD_SysVars_D>
   */
//<dom_var_def>double DIMDLI;
VAR_DEF(ODTDIST,              dli,    43,     (0.38),               (3.75),               (),        ValidatePositive())

/**
  <title DIMDSEP>
   <toctitle DIMDSEP>
   
   <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   '.' | ','
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R14+
   </table>
   
   DIMDSEP specifies the decimal separator for dimensions.
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimdsep()
   OdDbDatabase::setDimdsep()
   &nbsp; 
   OdDbDimension::dimdsep()
   OdDbDimension::setDimdsep()
   &nbsp; 
   OdDbDimStyleTableRecord::dimdsep()
   OdDbDimStyleTableRecord::setDimdsep()
   </table> 

<group TD_SysVars_D>
   */
//<dom_var_def>OdInt16 DIMDSEP;
VAR_DEF(ODTDECSEP,            dsep,   278,    ('.'),                (','),                (),        ValidateDecSep())


/**
  <title DIMEXE>
  <toctitle DIMEXE>
   
   <table>
   <b>Type</b>            double
   <b>Initial value</b>   0.18 | 1.25
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
   </table>
   
   DIMEXE specifies the distance extension lines extend beyond dimension lines.
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimexe()
   OdDbDatabase::setDimexe()
   &nbsp; 
   OdDbDimension::dimexe()
   OdDbDimension::setDimexe()
   &nbsp; 
   OdDbDimStyleTableRecord::dimexe()
   OdDbDimStyleTableRecord::setDimexe()
   </table>
   
   \sa
   <link DIMTSZ, DIMTSZ>
   
<group TD_SysVars_D>
   */
//<dom_var_def>double DIMEXE;
VAR_DEF(ODTDIST,              exe,    44,     (0.18),               (1.25),               (),        ValidatePositive())

/**
  <title DIMEXO>
  <toctitle DIMEXO>
   
   <table>
   <b>Type</b>            double
   <b>Initial value</b>   0.0625 | 0.625
   <b>Range</b>           >=0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
   </table>
   
   DIMEXO specifies the distance extensions are offset from
   their origin points. DIMEXO specifies a minimum value when
   fixed-length extension lines are used.
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimexo()
   OdDbDatabase::setDimexo()
   &nbsp; 
   OdDbDimension::dimexo()
   OdDbDimension::setDimexo()
   &nbsp; 
   OdDbDimStyleTableRecord::dimexo()
   OdDbDimStyleTableRecord::setDimexo()
   </table> 

<group TD_SysVars_D>
   */
//<dom_var_def>double DIMEXO;
VAR_DEF(ODTDIST,              exo,    42,     (0.0625),             (0.625),              (),        ValidatePositive())

/**
  <title DIMFRAC>
  <toctitle DIMFRAC>
   
   <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   0
   <b>Range</b>           0 ... 2
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2000+
   </table>
   
   DIMFRAC specifies the fraction format when DIMLUNIT == 4 or 5.
   
   <table>
   <b>Value</b>   <b>Description</b>
   0              Horizontal Stacked
   1              Diagonal Stacked
   2              Not Stacked
   </table>
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimfrac()
   OdDbDatabase::setDimfrac()
   &nbsp; 
   OdDbDimension::dimfrac()
   OdDbDimension::setDimfrac()
   &nbsp; 
   OdDbDimStyleTableRecord::dimfrac()
   OdDbDimStyleTableRecord::setDimfrac()
   </table>
   
   \sa
   <link DIMLUNIT, DIMLUNIT> 

<group TD_SysVars_D>
   */
//<dom_var_def>OdInt16 DIMFRAC;
VAR_DEF(OdInt16,              frac,   276,    (0),                  (0),                  (),        ValidateRange(0,2))

/**
  <title DIMGAP>
   <toctitle DIMGAP>
   
   <table>
   <b>Type</b>            double
   <b>Initial value</b>   0.09 | 0.625
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
   </table>
   
   DIMGAP specifies the gap between dimension text and dimension
   lines.
     * A negative value for DIMGAP draws a reference box round
       the dimension text.
     * Text is placed inside a dimension line only if there will
       be two line segments each as long as abs(DIMGAP).
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimgap()
   OdDbDatabase::setDimgap()
   &nbsp; 
   OdDbDimension::dimgap()
   OdDbDimension::setDimgap()
   &nbsp; 
   OdDbDimStyleTableRecord::dimgap()
   OdDbDimStyleTableRecord::setDimgap()
   &nbsp; 
   OdDbFcf::dimgap()
   OdDbFcf::setDimgap()
   &nbsp;
   OdDbLeader::dimgap()
   OdDbLeader::setDimgap()
   </table>

<group TD_SysVars_D>
   */
//<dom_var_def>double DIMGAP;
VAR_DEF(ODTDIST,              gap,    147,    (0.09),               (0.625),              (),        ValidateNone())

/**
  <title DIMJUST>
   <toctitle DIMJUST>
   
   <table>
   <b>Type</b>            OdUInt16
   <b>Initial value</b>   0
   <b>Range</b>           0 ... 4
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R13+
   </table>
   
   DIMJUST specifies the dimension text horizontal position.
   
   <table>
   <b>Value</b>   <b>Description</b>
   0              Centered above the dimension line.
   1              By the first extension line.
   2              By the second extension line.
   3              Above the dimension line, parallel to the first extension line.
   4              Above the dimension line, parallel to the second extension line.
   </table>
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimjust()
   OdDbDatabase::setDimjust()
   &nbsp; 
   OdDbDimension::dimjust()
   OdDbDimension::setDimjust()
   &nbsp; 
   OdDbDimStyleTableRecord::dimjust()
   OdDbDimStyleTableRecord::setDimjust()
   </table>

<group TD_SysVars_D>
   */
//<dom_var_def>OdUInt16 DIMJUST;
VAR_DEF(OdUInt16,             just,   280,    (0),                  (0),                  (),        ValidateRange(0,4))

/**
  <title DIMLDRBLK>
   <toctitle DIMLDRBLK>
   
   <table>
   <b>Type</b>            OdDbObjectId
   <b>Initial value</b>   OdDbObjectId::kNull
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2000+
   </table>
   
   DIMLDRBLK specifies the arrowhead block at the ends of leader
   lines. A null value specifies the default arrowhead is to be
   used. Some predefined arrowhead types:
   
   <image DIMLDRBLK.gif>
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimldrblk()
   OdDbDatabase::setDimldrblk()
   &nbsp; 
   OdDbDimension::dimldrblk()
   OdDbDimension::setDimldrblk()
   &nbsp; 
   OdDbDimStyleTableRecord::dimldrblk()
   OdDbDimStyleTableRecord::setDimldrblk()
   &nbsp; 
   OdDbLeader::dimldrblk()
   OdDbLeader::setDimldrblk()
   </table>

<group TD_SysVars_D>
   */
//<dom_var_def>OdDbObjectId DIMLDRBLK;
VAR_DEF(ODTARROWBLK,          ldrblk, 341,    (OdDbObjectId::kNull),(OdDbObjectId::kNull),(),        ValidateDimBlock())

/**
  <title DIMLFAC>
   <toctitle DIMLFAC>
   
   <table>
   <b>Type</b>            double
   <b>Initial value</b>   1.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
   </table>
   
   DIMLFAC specifies the distance multiplier for measurements in
   dimensions.
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimlfac()
   OdDbDatabase::setDimlfac()
   &nbsp; 
   OdDbDimension::dimlfac()
   OdDbDimension::setDimlfac()
   &nbsp;
   OdDbDimStyleTableRecord::dimlfac()
   OdDbDimStyleTableRecord::setDimlfac()
   </table> 

<group TD_SysVars_D>
   */
//<dom_var_def>double DIMLFAC;
VAR_DEF(double,               lfac,   144,    (1.0),                (1.0),                (),        ValidateNone())

/**
  <title DIMLIM>
   <toctitle DIMLIM>
   
   <table>
   <b>Type</b>            bool
   <b>Initial value</b>   false
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
   </table>
   
   DIMLIM controls the display of dimension limits.
   
   <table>
   <b>Value</b>   <b>Description</b>
   false          Disabled
   true           Enabled
   </table>
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimlim()
   OdDbDatabase::setDimlim()
   &nbsp; 
   OdDbDimension::dimlim()
   OdDbDimension::setDimlim()
   &nbsp; 
   OdDbDimStyleTableRecord::dimlim()
   OdDbDimStyleTableRecord::setDimlim()
   </table>

<group TD_SysVars_D>
   */
//<dom_var_def>bool DIMLIM;
VAR_DEF(bool,                 lim,    72,     (false),              (false),              (),        ValidateNone())

/**
  <title DIMLUNIT>
  <toctitle DIMLUNIT>
   
   <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   2
   <b>Range</b>           1 ... 6
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R13+
   </table>
   
   DIMLUNIT controls the formatting for units in non-angular dimensions.
   
   <table>
   <b>Value</b>   <b>Description</b>
   1              Scientific
   2              Decimal
   3              Engineering
   4              Architectural (stacked)
   5              Fractional (stacked)
   6              Microsoft Windows Desktop
   </table>
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimlunit()
   OdDbDatabase::setDimlunit()
   &nbsp; 
   OdDbDimension::dimlunit()
   OdDbDimension::setDimlunit()
   &nbsp; 
   OdDbDimStyleTableRecord::dimlunit()
   OdDbDimStyleTableRecord::setDimlunit()
   </table>

<group TD_SysVars_D>
*/
//<dom_var_def>OdInt16 DIMLUNIT;
VAR_DEF(OdInt16,              lunit,  277,    (2),                  (2),                  (),        ValidateRange(1,6))

/** 
  <title DIMLWD>
  <toctitle DIMLWD>
   
   <table>
   <b>Type</b>            OdDb::LineWeight
   <b>Initial value</b>   OdDb::kLnWtByBlock
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2000+
   </table>
   
   DIMLWD specifies the lineweight for dimension lines. Some
   specific values:
   
   <table>
   <b>Value</b>             <b>Description</b>
   OdDb::kLnWtByLayer       Lightweight is set to "ByLayer"
   OdDb::kLnWtByBlock       Lightweight is set to "ByBlock"
   OdDb::kLnWtByLwDefault   Lightweight is set to "Default", which is controlled by LWDEFAULT system variable
   </table>
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;
   OdDbDatabase::dimlwd()
   OdDbDatabase::setDimlwd()
   &nbsp; 
   OdDbDimension::dimlwd()
   OdDbDimension::setDimlwd()
   &nbsp; 
   OdDbDimStyleTableRecord::dimldrblk()
   OdDbDimStyleTableRecord::setDimldrblk()
   &nbsp; 
   OdDbLeader::dimldrblk()
   OdDbLeader::setDimldrblk()
   </table>
   
   \sa
   <link LWDEFAULT, LWDEFAULT>

   <group TD_SysVars_D>
   */
//<dom_var_def>OdDb::LineWeight DIMLWD;
VAR_DEF(ODTDB_LINEWEIGHT,     lwd,    371,    (OdDb::kLnWtByBlock), (OdDb::kLnWtByBlock), (),        ValidateLineWeight())

/**
  <title DIMLWE>
  <toctitle DIMLWE>
   
   <table>
   <b>Type</b>            OdDb::LineWeight
   <b>Initial value</b>   OdDb::kLnWtByBlock
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2000+
   </table>
   
   DIMLWE specifies the lineweight for extension lines. Some
   specific values:
   
   <table>
   <b>Value</b>             <b>Description</b>
   OdDb::kLnWtByLayer       Lightweight is set to "ByLayer"
   OdDb::kLnWtByBlock       Lightweight is set to "ByBlock"
   OdDb::kLnWtByLwDefault   Lightweight is set to "Default", which is controlled by LWDEFAULT system variable
   </table>
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimlwe()
   OdDbDatabase::setDimlwe()
   &nbsp; 
   OdDbDimension::dimlwe()
   OdDbDimension::setDimlwe()
   &nbsp; 
   OdDbDimStyleTableRecord::dimlwe()
   OdDbDimStyleTableRecord::setDimlwe()
   </table>
   
   \sa
   <link LWDEFAULT, LWDEFAULT>
   
   <group TD_SysVars_D>
*/
//<dom_var_def>OdDb::LineWeight DIMLWE;
VAR_DEF(ODTDB_LINEWEIGHT,     lwe,    372,    (OdDb::kLnWtByBlock), (OdDb::kLnWtByBlock), (),        ValidateLineWeight())

/**
  <title DIMPOST>
  <toctitle DIMPOST>
   
   <table>
   <b>Type</b>            OdString
   <b>Initial value</b>   ""
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
   </table>
   
   DIMPOST specifies a prefix and/or suffix for the measurement
   text in dimensions. DIMPOST is in the form prefix\<\>suffix.
   The '\<\>' in DIMPOST is replaced by the alternate
   measurements text.
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimpost()
   OdDbDatabase::setDimpost()
   &nbsp; 
   OdDbDimension::dimpost()
   OdDbDimension::setDimpost()
   &nbsp; 
   OdDbDimStyleTableRecord::dimpost()
   OdDbDimStyleTableRecord::setDimpost()
   </table> 

<group TD_SysVars_D>
   */
//<dom_var_def>OdString DIMPOST;
VAR_DEF(OdString,             post,   3,      (OD_T("")),                 (OD_T("")),                 (),        ValidateNone())

/**
  <title DIMRND>
  <toctitle DIMRND>
   
   <table>
   <b>Type</b>            double
   <b>Initial value</b>   0.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
   </table>
   
   DIMRND specifies the rounding of measurements in dimensions.
   If DIMRND == 0.0, no rounding is performed.
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimrnd()
   OdDbDatabase::setDimrnd()
   &nbsp; 
   OdDbDimension::dimrnd()
   OdDbDimension::setDimrnd()
   &nbsp; 
   OdDbDimStyleTableRecord::dimrnd()
   OdDbDimStyleTableRecord::setDimrnd()
   </table> 

<group TD_SysVars_D>
   */
//<dom_var_def>double DIMRND;
VAR_DEF(ODTDIST,              rnd,    45,     (0.0),                (0.0),                (),        ValidatePositive())

/**
  <title DIMSAH>
  <toctitle DIMSAH>
   
   <table>
   <b>Type</b>            bool
   <b>Initial value</b>   false
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
   </table>
   
   DIMSAH controls the generation of separate arrowheads for
   dimension lines.
   
   <table>
   <b>Value</b>   <b>Arrowhead Blocks</b>
   false          DIMBLK
   true           DIMBLK1 and DIMBLK2
   </table>
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;
   OdDbDatabase::dimsah()
   OdDbDatabase::setDimsah()
   &nbsp;
   OdDbDimension::dimsah()
   OdDbDimension::setDimsah()
   &nbsp; 
   OdDbDimStyleTableRecord::dimsah()
   OdDbDimStyleTableRecord::setDimsah()
   &nbsp; 
   OdDbLeader::dimsah()
   OdDbLeader::setDimsah()
   </table>  

<group TD_SysVars_D>
   */
//<dom_var_def>bool DIMSAH;
VAR_DEF(bool,                 sah,    173,    (false),              (false),              (),        ValidateNone())

/**
  <title DIMSCALE>
  <toctitle DIMSCALE>
   
   <table>
   <b>Type</b>            double
   <b>Initial value</b>   1.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
   </table>
   
   DIMSCALE is an scale factor applied to all dimension
   variables that regulate sizes, offsets, and distances.
   DIMSCALE serves as an overall "volume control" for dimensions
   and leaders.
   
   <table>
   <b>Value</b>   <b>Description</b>
   0.0            A "reasonable" value is computed for model space viewports.
   &gt;0.0        If set to the inverse of the plot scale, plotted dimensions will be at the sizes specified by the other dimension variables.
   </table>
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;
   OdDbDatabase::dimscale()
   OdDbDatabase::setDimscale()
   &nbsp; 
   OdDbDimension::dimscale()
   OdDbDimension::setDimscale()
   &nbsp; 
   OdDbDimStyleTableRecord::dimscale()
   OdDbDimStyleTableRecord::setDimscale()
   &nbsp; 
   OdDbFcf::dimscale()
   OdDbFcf::setDimscale()
   &nbsp; 
   OdDbLeader::dimscale()
   OdDbLeader::setDimscale()
   </table> 

<group TD_SysVars_D>
   */
//<dom_var_def>double DIMSCALE;
VAR_DEF(double,               scale,  40,     (1.0),                (1.0),                (),        ValidatePositive())

/**
  <title DIMSD1>
   <toctitle DIMSD1>
   
   <table>
   <b>Type</b>            bool
   <b>Initial value</b>   false
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R13+
   </table>
   
   DIMSD1 controls the suppression of the first dimension line
   and arrowhead in dimensions.
   
   <table>
   <b>Value</b>   <b>Description</b>
   false          Not suppressed
   true           Suppressed
   </table>
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;
   OdDbDatabase::dimsd1()
   OdDbDatabase::setDimsd1()
   &nbsp; 
   OdDbDimension::dimsd1()
   OdDbDimension::setDimsd1()
   &nbsp; 
   OdDbDimStyleTableRecord::dimsd1()
   OdDbDimStyleTableRecord::setDimsd1()
   </table> 

<group TD_SysVars_D>
   */
//<dom_var_def>bool DIMSD1;
VAR_DEF(bool,                 sd1,    281,    (false),              (false),              (),        ValidateNone())

/**
  <title DIMSD2>
   <toctitle DIMSD2>
   
   <table>
   <b>Type</b>            bool
   <b>Initial value</b>   false
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R13+
   </table>
   
   DIMSD2 controls the suppression of the second dimension line
   and arrowhead in dimensions.
   
   <table>
   <b>Value</b>   <b>Description</b>
   false          Not suppressed
   true           Suppressed
   </table>
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;
   OdDbDatabase::dimsd2
   OdDbDatabase::setDimsd2()
   &nbsp; 
   OdDbDimension::dimsd2()
   OdDbDimension::setDimsd2()
   &nbsp; 
   OdDbDimStyleTableRecord::dimsd2()
   OdDbDimStyleTableRecord::setDimsd2()
   </table> 

<group TD_SysVars_D>
   */
//<dom_var_def>bool DIMSD2;
VAR_DEF(bool,                 sd2,    282,    (false),              (false),              (),        ValidateNone())

/**
  <title DIMSE1>
   <toctitle DIMSE1>
   
   <table>
   <b>Type</b>            bool
   <b>Initial value</b>   false
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
   </table>
   
   DIMSE1 controls the suppression of the first extension line
   in dimensions.
   
   <table>
   <b>Value</b>   <b>Description</b>
   false          Not suppressed
   true           Suppressed
   </table>
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimse1()
   OdDbDatabase::setDimse1()
   &nbsp; 
   OdDbDimension::dimse1()
   OdDbDimension::setDimse1()
   &nbsp; 
   OdDbDimStyleTableRecord::dimse1()
   OdDbDimStyleTableRecord::setDimse1()
   </table> 

<group TD_SysVars_D>
   */
//<dom_var_def>bool DIMSE1;
VAR_DEF(bool,                 se1,    75,     (false),              (false),              (),        ValidateNone())

/**
  <title DIMSE2>
   <toctitle DIMSE2>
   
   <table>
   <b>Type</b>            bool
   <b>Initial value</b>   false
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
   </table>
   
   DIMSE2 controls the suppression of the second extension line in dimensions.
   
   <table>
   <b>Value</b>   <b>Description</b>
   false          Not suppressed
   true           Suppressed
   </table>
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimse2()
   OdDbDatabase::setDimse2()
   &nbsp; 
   OdDbDimension::dimse2()
   OdDbDimension::setDimse2()
   &nbsp; 
   OdDbDimStyleTableRecord::dimse2()
   OdDbDimStyleTableRecord::setDimse2()
   </table> 

<group TD_SysVars_D>
   */
//<dom_var_def>bool DIMSE2;
VAR_DEF(bool,                 se2,    76,     (false),              (false),              (),        ValidateNone())

/**
  <title DIMSOXD>
  <toctitle DIMSOXD>
   
   <table>
   <b>Type</b>            bool
   <b>Initial value</b>   false
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
   </table>
   
   DIMSOXD controls the suppression of dimension lines outside
   the extension lines.
   
   <table>
   <b>Value</b>   <b>Description</b>
   false          Not suppressed
   true           Suppressed
   </table>
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimsoxd()
   OdDbDatabase::setDimsoxd()
   &nbsp; 
   OdDbDimension::dimsoxd()
   OdDbDimension::setDimsoxd()
   &nbsp; 
   OdDbDimStyleTableRecord::dimsoxd()
   OdDbDimStyleTableRecord::setDimsoxd()
   </table> 

<group TD_SysVars_D>
   */
//<dom_var_def>bool DIMSOXD;
ANNOVAR_DEF(bool,             soxd,   175,    (false),              (false),              (),        ValidateNone())

/**
  <title DIMTAD>
   <toctitle DIMTAD>
   
   <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   0 | 1
   <b>Range</b>           0 ... 4
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
   </table>
   
   DIMTAD specifies the dimension text vertical position.
   
   <table>
   <b>Value</b>   <b>Description</b>
   0              Centered about the dimension line.
   1              Above the dimension line, unless DIMTIH==1 and the dimension line is not horizontal.
   2              Side of the dimension line farthest from the defining points.
   3              JIS standard.
   4              Text below the dimension line.
   </table>
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimtad()
   OdDbDatabase::setDimtad()
   &nbsp;
   OdDbDimension::dimtad()
   OdDbDimension::setDimtad()
   &nbsp;
   OdDbDimStyleTableRecord::dimtad()
   OdDbDimStyleTableRecord::setDimtad()
   &nbsp; 
   OdDbLeader::dimtad()
   OdDbLeader::setDimtad()
   </table>  
   
<group TD_SysVars_D>
   */
//<dom_var_def>OdInt16 DIMTAD;
VAR_DEF(OdInt16,              tad,    77,     (0),                  (1),                  (),        ValidateRange(0,4))

/**
  <title DIMTDEC>
   <toctitle DIMTDEC>
   
   <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   4 | 2
   <b>Range</b>           0 ... 8
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R13+
   </table>
   
   DIMTDEC specifies the number of decimal places in tolerance
   values for primary units in dimensions.
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimtdec()
   OdDbDatabase::setDimtdec()
   &nbsp; 
   OdDbDimension::dimtdec()
   OdDbDimension::setDimtdec()
   &nbsp; 
   OdDbDimStyleTableRecord::dimtdec()
   OdDbDimStyleTableRecord::setDimtdec()
   </table>

<group TD_SysVars_D>
   */
//<dom_var_def>OdInt16 DIMTDEC;
VAR_DEF(OdInt16,              tdec,   272,    (4),                  (2),                  (),        ValidateRange(0,8))

/**
  <title DIMTFAC>
   <toctitle DIMTFAC>
   
   <table>
   <b>Type</b>            double
   <b>Initial value</b>   1.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
   </table>
   
   DIMTFAC specifies, as a factor of DIMTXT, the size of tolerance and fraction text in dimensions.
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimtfac()
   OdDbDatabase::setDimtfac()
   &nbsp; 
   OdDbDimension::dimtfac()
   OdDbDimension::setDimtfac()
   &nbsp; 
   OdDbDimStyleTableRecord::dimtfac()
   OdDbDimStyleTableRecord::setDimtfac()
   </table>  

<group TD_SysVars_D>
   */
//<dom_var_def>double DIMTFAC;
VAR_DEF(double,               tfac,   146,    (1.0),                (1.0),                (),        ValidatePositiveNonZero())

/**
  <title DIMTIH>
   <toctitle DIMTIH>
   
   <table>
   <b>Type</b>            bool
   <b>Initial value</b>   true | false
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
   </table>
   
   DIMTIH controls the orientation of dimension text inside the
   extension lines.
   
   <table>
   <b>Value</b>   <b>Description</b>
   false          Aligned with dimension line.
   true           Horizonta
   </table>
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimtih()
   OdDbDatabase::setDimtih()
   &nbsp; 
   OdDbDimension::dimtih()
   OdDbDimension::setDimtih()
   &nbsp; 
   OdDbDimStyleTableRecord::dimtih()
   OdDbDimStyleTableRecord::setDimtih()
   </table> 

<group TD_SysVars_D>
   */
//<dom_var_def>bool DIMTIH;
VAR_DEF(bool,                 tih,    73,     (true),               (false),              (),        ValidateNone())

/**
  <title DIMTIX>
   <toctitle DIMTIX>
   
   <table>
   <b>Type</b>            bool
   <b>Initial value</b>   false
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
   </table>
   
   DIMTIX controls the placement of dimension text inside the
   extension lines.
   
   <table>
   <b>Value</b>   <b>Description</b>
   false          Inside extension lines if there is room.
   true           Inside extension lines.
   </table>
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimtix()
   OdDbDatabase::setDimtix()
   &nbsp; 
   OdDbDimension::dimtix()
   OdDbDimension::setDimtix()
   &nbsp; 
   OdDbDimStyleTableRecord::dimtix()
   OdDbDimStyleTableRecord::setDimtix()
   </table>  

<group TD_SysVars_D>
   */
//<dom_var_def>bool DIMTIX;
ANNOVAR_DEF(bool,             tix,    174,    (false),              (false),              (),        ValidateNone())

/**
  <title DIMTM>
   <toctitle DIMTM>
   
   <table>
   <b>Type</b>            double
   <b>Initial value</b>   0.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
   </table>
   
   DIMTM specifies the negative of the lower tolerance limit for
   dimensions.
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimtm()
   OdDbDatabase::setDimtm()
   &nbsp; 
   OdDbDimension::dimtm()
   OdDbDimension::setDimtm()
   &nbsp; 
   OdDbDimStyleTableRecord::dimtm()
   OdDbDimStyleTableRecord::setDimtm()
   </table>

<group TD_SysVars_D>
   */
//<dom_var_def>double DIMTM;
VAR_DEF(ODTDIST,              tm,     48,     (0.0),                (0.0),                (),        ValidateNone())

/**
  <title DIMTMOVE>
  <toctitle DIMTMOVE>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   0
   <b>Range</b>           0 ... 2
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2000+
  </table>
   
   DIMTMOVE controls the updating of associative dimension
   objects while dragging.
   
   <table>
   <b>Value</b>   <b>Description</b>
   0              Moving dimension text moves the dimension line.
   1              Moving dimension text adds a leader.
   2              Moving dimension text does not add a leader.
   </table>
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimtmove()
   OdDbDatabase::setDimtmove()
   &nbsp;
   OdDbDimension::dimtmove()
   OdDbDimension::setDimtmove()
   &nbsp; 
   OdDbDimStyleTableRecord::dimtmove()
   OdDbDimStyleTableRecord::setDimtmove()
   </table>
   
   <b>Note: </b>DIMTMOVE is obsolete, but is included for compatibility purposes.

   <group TD_SysVars_D>
   */
//<dom_var_def>OdInt16 DIMTMOVE;
ANNOVAR_DEF(OdInt16,          tmove,  279,    (0),                  (0),                  (),        ValidateRange(0,2))

/**
  <title DIMTOFL>
  <toctitle DIMTOFL>
   
   <table>
   <b>Type</b>            bool
   <b>Initial value</b>   false | true
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
   </table>
   
   DIMTOFL controls the drawing of dimension lines between extension lines when text is outside the dimension lines.
   
   <table>
   <b>Value</b>   <b>Description</b>
   false          Disabled
   true           Enabled
   </table>
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimtofl()
   OdDbDatabase::setDimtofl()
   &nbsp; 
   OdDbDimension::dimtofl()
   OdDbDimension::setDimtofl()
   &nbsp; 
   OdDbDimStyleTableRecord::dimtofl()
   OdDbDimStyleTableRecord::setDimtofl()
   </table> 
   
<group TD_SysVars_D>
   */
//<dom_var_def>bool DIMTOFL;
ANNOVAR_DEF(bool,             tofl,   172,    (false),              (true),               (),        ValidateNone())

/**
  <title DIMTOH>
  <toctitle DIMTOH>
   
   <table>
   <b>Type</b>            bool
   <b>Initial value</b>   true | false
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
   </table>
   
   DIMTOH controls the orientation of dimension text outside the extension lines.
   
   <table>
   <b>Value</b>   <b>Description</b>
   false          Aligned with dimension line.
   true           Horizontal
   </table>
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimtoh()
   OdDbDatabase::setDimtoh()
   &nbsp; 
   OdDbDimension::dimtoh()
   OdDbDimension::setDimtoh()
   &nbsp; 
   OdDbDimStyleTableRecord::dimtoh()
   OdDbDimStyleTableRecord::setDimtoh()
   </table> 

<group TD_SysVars_D>
   */
//<dom_var_def>bool DIMTOH;
VAR_DEF(bool,                 toh,    74,     (true),               (false),              (),        ValidateNone())

/**
  <title DIMTOL>
  <toctitle DIMTOL>
   
   <table>
   <b>Type</b>            bool
   <b>Initial value</b>   false
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
   </table>
   
   DIMTOL controls the generation of tolerance text in dimensions.
   
   <table>
   <b>Value</b>   <b>Description</b>
   false          Disabled
   true           Enabled
   </table>
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimtol()
   OdDbDatabase::setDimtol()
   &nbsp; 
   OdDbDimension::dimtol()
   OdDbDimension::setDimtol()
   &nbsp; 
   OdDbDimStyleTableRecord::dimtol()
   OdDbDimStyleTableRecord::setDimtol()
   </table>  

<group TD_SysVars_D>
   */
//<dom_var_def>bool DIMTOL;
VAR_DEF(bool,                 tol,    71,     (false),              (false),              (),        ValidateNone())

/**
  <title DIMTOLJ>
  <toctitle DIMTOLJ>
   
   <table>
   <b>Type</b>            OdUInt8
   <b>Initial value</b>   1 | 0
   <b>Range</b>           0 ... 2
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R13+
   </table>
   
   DIMTOLJ controls the vertical justification of tolerance values with respect to the nominal dimension text.
   
   <table>
   <b>Value</b>   <b>Description</b>
   0              Bottom
   1              Middle
   2              Top
   </table>
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimtolj()
   OdDbDatabase::setDimtolj()
   &nbsp; 
   OdDbDimension::dimtolj()
   OdDbDimension::setDimtolj()
   &nbsp; 
   OdDbDimStyleTableRecord::dimtolj()
   OdDbDimStyleTableRecord::setDimtolj()
   </table>  

<group TD_SysVars_D>
   */
//<dom_var_def>OdUInt8 DIMTOLJ;
VAR_DEF(OdUInt8,              tolj,   283,    (1),                  (0),                  (),        ValidateRange(0,2))

/**
  <title DIMTP>
  <toctitle DIMTP>
   
   <table>
   <b>Type</b>            double
   <b>Initial value</b>   0.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
   </table>
   
   DIMTP specifies the upper tolerance limit for dimensions.
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimtp()
   OdDbDatabase::setDimtp()
   &nbsp; 
   OdDbDimension::dimtp()
   OdDbDimension::setDimtp()
   &nbsp; 
   OdDbDimStyleTableRecord::dimtp()
   OdDbDimStyleTableRecord::setDimtp()
   </table>

<group TD_SysVars_D>
   */
//<dom_var_def>double DIMTP;
VAR_DEF(ODTDIST,              tp,     47,     (0.0),                (0.0),                (),        ValidateNone())

/** * VARDOCS:
  <properties>
    <remarks>
      "\n    DIMTSZ specifies the size of architectural tics are "
      "\n    to be drawn instead of arrowheads for linear, radial, and diameter dimensions."
      "\n"
      "\n    DIMTSZ is the distance, parallel to the dimension line,"
      "\n    that the tics extends beyond the extension lines."
      "\n"
      "\n    @table"
      "\n    Value    Description"
      "\n    0.0      Arrowheads"
      "\n    > 0.0    Tics"
    </remarks>
    <see_also>
      "\n    DIMDLE"
    </see_also>
  </properties>
*/

/** <title DIMTSZ>
   <toctitle DIMTSZ>
   
   <table>
   <b>Type</b>            double
   <b>Initial value</b>   0.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
   </table>
   
   DIMTSZ specifies the size of architectural tics are to be
   drawn instead of arrowheads for linear, radial, and diameter
   dimensions. DIMTSZ is the distance, parallel to the dimension
   line, that the tics extends beyond the extension lines.
   
   <table>
   <b>Value</b>   <b>Description</b>
   0.0            Arrowheads
   &gt; 0.0       Tics
   </table>
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimtsz()
   OdDbDatabase::setDimtsz()
   &nbsp; 
   OdDbDimension::dimtsz()
   OdDbDimension::setDimtsz()
   &nbsp; 
   OdDbDimStyleTableRecord::dimtsz()
   OdDbDimStyleTableRecord::setDimtsz()
   </table> 

<group TD_SysVars_D>
   */
//<dom_var_def>double DIMTSZ;
VAR_DEF(ODTDIST,              tsz,    142,    (0.0),                (0.0),                (),        ValidatePositive())

/**
  <title DIMTVP>
  <toctitle DIMTVP>
   
   <table>
   <b>Type</b>            double
   <b>Initial value</b>   0.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
   </table>
   
   DIMTVP specifies, as a factor of DIMTXT, the vertical
   distance of the middle of the dimension text with respect to
   the dimension line.
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimtvp()
   OdDbDatabase::setDimtvp()
   &nbsp; 
   OdDbDimension::dimtvp()
   OdDbDimension::setDimtvp()
   &nbsp; 
   OdDbDimStyleTableRecord::dimtvp()
   OdDbDimStyleTableRecord::setDimtvp()
   </table>  

<group TD_SysVars_D>
   */
//<dom_var_def>double DIMTVP;
VAR_DEF(double,               tvp,    145,    (0.0),                (0.0),                (),        ValidateNone())

/**
  <title DIMTXSTY>
  <toctitle DIMTXSTY>
   
   <table>
   <b>Type</b>            OdDbObjectId
   <b>Initial value</b>   "Standard" text style object ID
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R13+
   </table>
   
   The DIMTXSTY variable specifies the text style for dimension text.
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;
   OdDbDatabase::dimtxsty()
   OdDbDatabase::setDimtxsty()
   &nbsp; 
   OdDbDimension::dimtxsty()
   OdDbDimension::setDimtxsty()
   &nbsp; 
   OdDbLeader::dimtxsty()
   OdDbLeader::setDimtxsty()
   </table> 

<group TD_SysVars_D>
   */
//<dom_var_def>OdDbObjectId DIMTXSTY;
VAR_DEF(ODTTEXTSTYLEID,       txsty,  340,    (OdDbObjectId::kNull),(OdDbObjectId::kNull),(),        ValidateTextStyle(NullIdAllowed))

/**
  <title DIMTXT>
  <toctitle DIMTXT>
   
   <table>
   <b>Type</b>            double
   <b>Initial value</b>   0.18 | 2.5
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
   </table>
   
   DIMTXT specifies the size of the dimension text.
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimtxt()
   OdDbDatabase::setDimtxt()
   &nbsp; 
   OdDbDimension::dimtxt()
   OdDbDimension::setDimtxt()
   &nbsp; 
   OdDbDimStyleTableRecord::dimtxt()
   OdDbDimStyleTableRecord::setDimtxt()
   &nbsp; 
   OdDbFcf::dimtxt()
   OdDbFcf::setDimtxt()
   &nbsp; 
   OdDbLeader::dimtxt()
   OdDbLeader::setDimtxt()
   </table> 

<group TD_SysVars_D>
   */
//<dom_var_def>double DIMTXT;
VAR_DEF(ODTDIST,              txt,    140,    (0.18),               (2.5),                (),        ValidatePositiveNonZero())

/**
  <title DIMTZIN>
  <toctitle DIMTZIN>
   
   <table>
   <b>Type</b>            OdUInt8
   <b>Initial value</b>   0 | 8
   <b>Range</b>           0 ... 15
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R13+
   </table>
   
  DIMTZIN controls zero suppression in tolerance values in linear dimensions.
   
   <table>
   <b>Value</b>   <b>Description</b>
   0              Suppress zero feet and exactly zero inches.
   1              Include zero feet and exactly zero inches.
   2              Include zero feet and suppress exactly zero inches.
   3              Suppress zero feet and include exactly zero inches.
   4              Suppress leading decimal zeros.
   8              Suppress trailing decimal zeros.
   12             Suppress leading and trailing decimal zeros.
   </table>
   
   Values 0-3 affect only feet-and-inch dimensions, and may be combined with values 4-12.
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimtzin()
   OdDbDatabase::setDimtzin()
   &nbsp; 
   OdDbDimension::dimtzin()
   OdDbDimension::setDimtzin()
   &nbsp; 
   OdDbDimStyleTableRecord::dimtzin()
   OdDbDimStyleTableRecord::setDimtzin()
   </table> 

<group TD_SysVars_D>
   */
//<dom_var_def>OdUInt8 DIMTZIN;
VAR_DEF(OdUInt8,              tzin,   284,    (0),                  (8),                  (),        ValidateRange(0,15))

/**
  <title DIMUPT>
  <toctitle DIMUPT>
   
   <table>
   <b>Type</b>            bool
   <b>Initial value</b>   false
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R13+
   </table>
   
   DIMUPT controls the movement of user-positioned text when dimension lines are moved.
   
   <table>
   <b>Value</b>   <b>Description</b>
   false          Moves only the dimension line.
   true           Moves the dimension line and the text.
   </table>
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimupt()
   OdDbDatabase::setDimupt()
   &nbsp; 
   OdDbDimension::dimupt()
   OdDbDimension::setDimupt()
   &nbsp; 
   OdDbDimStyleTableRecord::dimupt()
   OdDbDimStyleTableRecord::setDimupt()
   </table>   

<group TD_SysVars_D>
   */
//<dom_var_def>bool DIMUPT;
VAR_DEF(bool,                 upt,    288,    (false),              (false),              (),        ValidateNone())

/**
  <title DIMZIN>
  <toctitle DIMZIN>
   
   <table>
   <b>Type</b>            OdUInt8
   <b>Initial value</b>   0 | 8
   <b>Range</b>           0 ... 15
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
   </table>
   
   DIMZIN controls zero suppression in linear dimensions.
   
   <table>
   <b>Value</b>   <b>Description</b>
   0              Suppress zero feet and exactly zero inches.
   1              Include zero feet and exactly zero inches.
   2              Include zero feet and suppress exactly zero inches.
   3              Suppress zero feet and include exactly zero inches.
   4              Suppress leading decimal zeros.
   8              Suppress trailing decimal zeros.
   12             Suppress leading and trailing decimal zeros.
   </table>
   
   Values 0-3 affect only feet-and-inch dimensions, and may be combined with values 4-12.
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimzin()
   OdDbDatabase::setDimzin()
   &nbsp; 
   OdDbDimension::dimzin()
   OdDbDimension::setDimzin()
   &nbsp; 
   OdDbDimStyleTableRecord::dimzin()
   OdDbDimStyleTableRecord::setDimzin()
   </table> 

<group TD_SysVars_D>
   */
//<dom_var_def>OdUInt8 DIMZIN;
VAR_DEF(OdUInt8,              zin,    78,     (0),                  (8),                  (),        ValidateRange(0,15))


//--------------------------------------------new in R21------------------------------------------------------

/**
  <title DIMFXL>
   <toctitle DIMFXL>
   
   <table>
   <b>Type</b>            double
   <b>Initial value</b>   1.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2007+
   </table>
   
   DIMFXL specifies the fixed length of extension lines from the
   dimension line towards the dimension origin. DIMFXL is
   ignored when DIMFXLON == false.
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimfxl()
   OdDbDatabase::setDimfxl()
   &nbsp; 
   OdDbDimension::dimfxl()
   OdDbDimension::setDimfxl()
   &nbsp; 
   OdDbDimStyleTableRecord::dimfxl()
   OdDbDimStyleTableRecord::setDimfxl()
   </table>  

<group TD_SysVars_D>
   */
//<dom_var_def>double DIMFXL;
VAR_DEF(ODTDIST,              fxl,    49,     (1.0),                  (1.0),                  (),        ValidateNone())

/**
  <title DIMFXLON>
   <toctitle DIMFXLON>
   
   <table>
   <b>Type</b>            bool
   <b>Initial value</b>   false
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2007+
   </table>
   
   DIMFXLON controls the generation of fixed-length extension lines.
   
   <table>
   <b>Value</b>   <b>Description</b>
   false          Variable length.
   true           Fixed length.
   </table>
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimfxlon()
   OdDbDatabase::setDimfxlon()
   &nbsp; 
   OdDbDimension::dimfxlon()
   OdDbDimension::setDimfxlon()
   &nbsp; 
   OdDbDimStyleTableRecord::dimfxlon()
   OdDbDimStyleTableRecord::setDimfxlon()
   </table>
   
   \sa
   <link DIMFXL, DIMFXL> 

<group TD_SysVars_D>
   */
//<dom_var_def>bool DIMFXLON;
VAR_DEF(bool,              fxlon,    290,     (false),                  (false),                  (),       ValidateBOOL())

/**
  <title DIMJOGANG>
   <toctitle DIMJOGANG>
   
   <table>
   <b>Type</b>            double
   <b>Initial value</b>   pi/4
   <b>Range</b>           pi/36 ... pi/2
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2007+
   </table>
   
   DIMJOGANG specifies the jog angle of dimension lines in
   jogged radius dimensions.
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimjogang()
   OdDbDatabase::setDimjogang()
   &nbsp; 
   OdDbDimension::dimjogang()
   OdDbDimension::setDimjogang()
   &nbsp; 
   OdDbDimStyleTableRecord::dimjogang()
   OdDbDimStyleTableRecord::setDimjogang()
   </table>
   
   <b>Note: </b>All angles are expressed in radians.

<group TD_SysVars_D>
   */
//<dom_var_def>double DIMJOGANG;
VAR_DEF(ODTORIENT,         jogang,    50,     (OdaPI4),                  (OdaPI4),                  (),     ValidateDouble(  5.0*OdaPI/180.0, OdaPI2))

/**
  <title DIMTFILL>
   <toctitle DIMTFILL>
   
   <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   0
   <b>Range</b>           0 ... 2
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2007+
   </table>
   
   DIMTFILL specifies the background fill of dimension text.
   
   <table>
   <b>Value</b>   <b>Description</b>
   0              No fill
   1              Drawing background color
   2              Color specified by DIMTFILLCLR
   </table>
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimtfill()
   OdDbDatabase::setDimtfill()
   &nbsp;
   OdDbDimension::dimtfill()
   OdDbDimension::setDimtfill()
   &nbsp; 
   OdDbDimStyleTableRecord::dimtfill()
   OdDbDimStyleTableRecord::setDimtfill()
   </table>
   
   \sa
   <link DIMTFILLCLR, DIMTFILLCLR>
   
<group TD_SysVars_D>   
   */
//<dom_var_def>OdInt16 DIMTFILL;
VAR_DEF(OdInt16,              tfill,    69,     (0),                  (0),                  (),        ValidateRange(0, 2))

/**
  <title DIMTFILLCLR>
  <toctitle DIMTFILLCLR>
   
  <table>
   <b>Type</b>            OdCmColor
   <b>Initial value</b>   OdCmEntityColor::kByBlock
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2007+
  </table>
   
  DIMTFILLCLR specifies the background fill color of dimension text when DIMTFILL == 2.
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimtfillclr()
   OdDbDatabase::setDimtfillclr()
   &nbsp; 
   OdDbDimension::dimtfillclr()
   OdDbDimension::setDimtfillclr()
   &nbsp; 
   OdDbDimStyleTableRecord::dimtfillclr()
   OdDbDimStyleTableRecord::setDimtfillclr()
   </table>
   
   \sa
   <link DIMTFILL, DIMTFILL> 

<group TD_SysVars_D>
   */
//<dom_var_def>OdCmColor DIMTFILLCLR;
VAR_DEF(OdCmColor,              tfillclr,    70,     (OdCmEntityColor::kByBlock),                  (OdCmEntityColor::kByBlock),                  (),        ValidateNone())

/**
  <title DIMARCSYM>
  <toctitle DIMARCSYM>
   
   <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   0
   <b>Range</b>           0 ... 2
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2007+
   </table>
   
   DIMARCSYM specifies the placement of arc symbols in arc length dimensions
   
   <table>
   <b>Value</b>   <b>Description</b>
   0              Before the dimension text.
   1              Above the dimension text.
   2              Not displayed.
   </table>
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;
   OdDbDatabase::dimarcsym()
   OdDbDatabase::setDimarcsym()
   &nbsp; 
   OdDbDimension::dimarcsym()
   OdDbDimension::setDimarcsym()
   &nbsp; 
   OdDbDimStyleTableRecord::dimarcsym()
   OdDbDimStyleTableRecord::setDimarcsym()
   </table>

<group TD_SysVars_D>
   */
//<dom_var_def>OdInt16 DIMARCSYM;
VAR_DEF(OdInt16,              arcsym,    90,     (0),                  (0),                  (),        ValidateRange(0, 2))

/**
  <title DIMLTYPE>
  <toctitle DIMLTYPE>
   
  <table>
   <b>Type</b>            OdDbObjectId
   <b>Initial value</b>   OdDbObjectId::kNull
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2007+
  </table>
   
  DIMLTYPE specifies the linetype of dimension lines.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimltype()
   OdDbDatabase::setDimltype()
   &nbsp; 
   OdDbDimension::dimltype()
   OdDbDimension::setDimltype()
   &nbsp; 
   OdDbDimStyleTableRecord::dimltype()
   OdDbDimStyleTableRecord::setDimltype()
   </table> 

<group TD_SysVars_D>
   */
//<dom_var_def>OdDbObjectId DIMLTYPE;
VAR_DEF(ODTLINETYPEID,              ltype,    345,     (OdDbObjectId::kNull),                  (OdDbObjectId::kNull),                  (),        ValidateLinetype(true))

/**
  <title DIMLTEX1>
  <toctitle DIMLTEX1>
   
   <table>
   <b>Type</b>            OdDbObjectId
   <b>Initial value</b>   OdDbObjectId::kNull
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2007+
   </table>
   
  DIMLTEX1 specifies the linetype of first extension lines.
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimltex1()
   OdDbDatabase::setDimltex1()
   &nbsp; 
   OdDbDimension::dimltex1()
   OdDbDimension::setDimltex1()
   &nbsp; 
   OdDbDimStyleTableRecord::dimltex1()
   OdDbDimStyleTableRecord::setDimltex1()
   </table> 

<group TD_SysVars_D>
   */
//<dom_var_def>OdDbObjectId DIMLTEX1;
VAR_DEF(ODTLINETYPEID,              ltex1,    346,     (OdDbObjectId::kNull),                  (OdDbObjectId::kNull),                  (),        ValidateLinetype(true))

/**
  <title DIMLTEX2>
  <toctitle DIMLTEX2>
   
   <table>
   <b>Type</b>            OdDbObjectId
   <b>Initial value</b>   OdDbObjectId::kNull
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2007+
   </table>
   
   DIMLTEX2 specifies the linetype of second extension lines.
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::dimltex2()
   OdDbDatabase::setDimltex2()
   &nbsp; 
   OdDbDimension::dimltex2()
   OdDbDimension::setDimltex2()
   &nbsp; 
   OdDbDimStyleTableRecord::dimltex2()
   OdDbDimStyleTableRecord::setDimltex2()
   </table>  

<group TD_SysVars_D>
   */
//<dom_var_def>OdDbObjectId DIMLTEX2;
VAR_DEF(ODTLINETYPEID,              ltex2,    347,     (OdDbObjectId::kNull),                  (OdDbObjectId::kNull),                  (),        ValidateLinetype(true))

//--------------------------------------------new in R24------------------------------------------------------

/** * VARDOCS:
  <properties>
    <remarks>
      "\n    DIMTXTDIRECTION controls the text direction."
      "\n"
      "\n    @table"
      "\n    Value    Description"
      "\n    false    "
      "\n    true     "
    </remarks>
      <see_also>
      "\n    DIMTXTDIRECTION"
      </see_also>
  </properties>
*/
VAR_DEF(bool,         txtdirection,   294,          (false),                  (false),          (),  ValidateBOOL())

/** * VARDOCS:
  <properties>
    <remarks>
      "\n    DIMMZF."
    </remarks>
  </properties>
*/
VAR_DEF(double,                mzf,   297,          (100.0),                  (100.0),          (),  ValidateNone())

/** * VARDOCS:
  <properties>
    <remarks>
      "\n    DIMMZS."
    </remarks>
  </properties>
*/
VAR_DEF(OdString,              mzs,   298,       (OD_T("")),               (OD_T("")),          (),  ValidateNone())

/** * VARDOCS:
  <properties>
    <remarks>
      "\n    DIMALTMZF."
    </remarks>
  </properties>
*/
VAR_DEF(double,             altmzf,   295,          (100.0),                  (100.0),          (),  ValidateNone())

/** * VARDOCS:
  <properties>
    <remarks>
      "\n    DIMALTMZS."
    </remarks>
  </properties>
*/
VAR_DEF(OdString,           altmzs,   296,       (OD_T("")),               (OD_T("")),          (),  ValidateNone())


#ifdef DOUNDEF_ODTDB_LINEWEIGHT
#undef ODTDB_LINEWEIGHT
#undef DOUNDEF_ODTDB_LINEWEIGHT
#endif

#ifdef DOUNDEF_ODTDIST
#undef ODTDIST
#undef DOUNDEF_ODTDIST
#endif

#ifdef DOUNDEF_ODTUNITLESS
#undef ODTUNITLESS
#undef DOUNDEF_ODTUNITLESS
#endif

#ifdef DOUNDEF_ODTORIENT
#undef ODTORIENT
#undef DOUNDEF_ODTORIENT
#endif

#ifdef DOUNDEF_ODTDECSEP
#undef ODTDECSEP
#undef DOUNDEF_ODTDECSEP
#endif

#ifdef DOUNDEF_ODTARROWBLK
#undef ODTARROWBLK
#undef DOUNDEF_ODTARROWBLK
#endif

#ifdef DOUNDEF_ODTTEXTSTYLEID
#undef ODTTEXTSTYLEID
#undef DOUNDEF_ODTTEXTSTYLEID
#endif

#ifdef DOUNDEF_ODTLINETYPEID
#undef ODTLINETYPEID
#undef DOUNDEF_ODTLINETYPEID
#endif

#ifdef DOUNDEF_NullIdAllowed
#undef NullIdAllowed
#undef DOUNDEF_NullIdAllowed
#endif

