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

//DOM-IGNORE-BEGIN

/*********************************************************/
//  Macros description:
/*********************************************************
VAR_DEF(type, VarName,...)
Variable support is generated in OdDbDatabase::getVarName/setVarName()
and OdDbDatabase::get/setSysVar()

RO_VAR_DEF(type, VarName,...)
Read-only
Variable support is generated in OdDbDatabase::getVarName()
and OdDbDatabase::getSysVar()

VAR_DEF_H(type, VarName,...)
Variable support is generated in OdDbDatabase::getVarName/setVarName() only.
If get/setSysVar() is available see QuasiVarDefs.h
Note: Such variables may work differently in OdDbDatabase::get/setXXX()and get/setSysVar()
For example:
There are get/set OdDbDatabase methods for both EXTMIN(MS) and PEXTMIN(PS) but get/setSysVar()
is for EXTMIN only. It works with extents of current space.

RO_VAR_DEF_H(type, VarName,...)
The same as VAR_DEF_H but only OdDbDatabase::getVarName() is generated.

VAR_DEF_2
Similar to VAR_DEF but some special processing in set methods is required
and OdDbDatabase::setVarName() method is implemented manually.
Example - OdDbDatabase::setTEXTSTYLE() also changes TEXTSIZE.
And OdDbDatabase::setTEXTSIZE() changes prior size in current Text Style

DICR15VAR
DICR21VAR
DICR24VAR
DICR27VAR
Similar to VAR_DEF. Difference is that in some file versions these variables
present in Database header and in minor file versions they are round-tripped
via Variables Dictionary


MLEADERSTYLE_VAR_DEF
ANNO_VAR_DEF
A special cases of DICR21VAR, used to differentiate CANNOSCALE and CMLEADERSCALE from other usual system variables in some cases.
By default is equal to DICR21VAR.


REGVAR_DEF(type, VarName,...)
Variable support is generated in OdDbHostAppServices (getVarName/setVarName)
and OdDbDatabase::get/setSysVar()

*/


#ifndef RO_VAR_DEF
#define RO_VAR_DEF  VAR_DEF /* {Secret} */
#define DOUNDEF_RO_VAR_DEF
#endif

#ifndef RO_VAR_DEF_H
#define RO_VAR_DEF_H  RO_VAR_DEF /* {Secret} */
#define DOUNDEF_RO_VAR_DEF_H
#endif

#ifndef VAR_DEF_H
#define VAR_DEF_H  VAR_DEF /* {Secret} */
#define DOUNDEF_VAR_DEF_H
#endif

#ifndef DICR15VAR
#define DOUNDEF_DICR15VAR
#define DICR15VAR  VAR_DEF /* {Secret} */
#endif

#ifndef DICR21VAR
#define DOUNDEF_DICR21VAR
#define DICR21VAR  VAR_DEF /* {Secret} */
#define DOUNDEF_DICR21VAR_H
#define DICR21VAR_H VAR_DEF_H /* {Secret} */
#else
#define DOUNDEF_DICR21VAR_H
#define DICR21VAR_H DICR21VAR /* {Secret} */
#endif

#ifndef DICR24VAR
#define DOUNDEF_DICR24VAR
#define DICR24VAR  DICR21VAR /* {Secret} */
#define DOUNDEF_DICR24VAR_H
#define DICR24VAR_H DICR21VAR_H /* {Secret} */
#else
#define DOUNDEF_DICR24VAR_H
#define DICR24VAR_H DICR24VAR /* {Secret} */
#endif

#ifndef DICR27VAR
#define DOUNDEF_DICR27VAR
#define DICR27VAR DICR21VAR /* {Secret} */
#define DOUNDEF_DICR27VAR_H
#define DICR27VAR_H DICR21VAR_H /* {Secret} */
#else
#define DOUNDEF_DICR27VAR_H
#define DICR27VAR_H DICR27VAR /* {Secret} */
#endif

#ifndef ANNO_VAR_DEF
#define DOUNDEF_ANNO_VAR_DEF
#define ANNO_VAR_DEF DICR21VAR  /* {Secret} */
#endif

#ifndef MLEADERSTYLE_VAR_DEF
#define DOUNDEF_MLEADERSTYLE_VAR_DEF
#define MLEADERSTYLE_VAR_DEF DICR21VAR  /* {Secret} */
#endif

#ifndef VAR_DEF_2
#define DOUNDEF_VAR_DEF_2
#define VAR_DEF_2  VAR_DEF /* {Secret} */
#endif

#ifndef ODTBOOL
#define ODTBOOL                 bool                    /* {Secret} */
#endif

#ifndef ODTDIST
#define ODTDIST                 double                  /* {Secret} */
#endif

#ifndef ODTORIENT
#define ODTORIENT               double                  /* {Secret} */
#endif

#ifndef ODTUNITLESS4
#define ODTUNITLESS4            double                  /* {Secret} */
#endif

#ifndef ODTUNITLESS8
#define ODTUNITLESS8            double                  /* {Secret} */
#endif

#ifndef ODTDB_MEASUREMENTVALUE
#define ODTDB_MEASUREMENTVALUE  OdDb::MeasurementValue  /* {Secret} */
#endif

#ifndef ODTDB_LINEWEIGHT
#define ODTDB_LINEWEIGHT        OdDb::LineWeight        /* {Secret} */
#endif

#ifndef ODTDB_JOINSTYLE
#define ODTDB_JOINSTYLE         OdDb::JoinStyle         /* {Secret} */
#endif

#ifndef ODTDB_UNITSVALUE
#define ODTDB_UNITSVALUE        OdDb::UnitsValue        /* {Secret} */
#endif

#ifndef ODTDB_PROXYIMAGE
#define ODTDB_PROXYIMAGE        OdDb::ProxyImage        /* {Secret} */
#endif

#ifndef ODTDB_MEASUREMENTVALUE
#define ODTDB_MEASUREMENTVALUE  OdDb::MeasurementValue  /* {Secret} */
#endif

#ifndef ODTDB_ENDCAPS
#define ODTDB_ENDCAPS           OdDb::EndCaps           /* {Secret} */
#endif

#ifndef ODTDB_JOINSTYLE
#define ODTDB_JOINSTYLE         OdDb::JoinStyle         /* {Secret} */
#endif

#ifndef ODTDB_PLOTSTYLENAMETYPE
#define ODTDB_PLOTSTYLENAMETYPE OdDb::PlotStyleNameType /* {Secret} */
#endif

#ifndef ODDBOBJECTID
#define ODDBOBJECTID            OdDbObjectId            /* {Secret} */
#endif

#ifndef ODDBHARDPOINTERID
#define ODDBHARDPOINTERID       ODDBOBJECTID            /* {Secret} */
#endif

#ifndef ODTTEXTSTYLEID
#define ODTTEXTSTYLEID          ODDBOBJECTID            /* {Secret} */
#endif

#ifndef ODTLAYERID
#define ODTLAYERID              ODDBOBJECTID            /* {Secret} */
#endif

#ifndef ODTLAYERIDDEF
#define ODTLAYERIDDEF           ODTLAYERID              /* {Secret} */
#endif

#ifndef ODTLINETYPEID
#define ODTLINETYPEID           ODDBOBJECTID            /* {Secret} */
#endif

#ifndef ODTDIMSTYLEID
#define ODTDIMSTYLEID           ODDBOBJECTID            /* {Secret} */
#endif

#ifndef ODTUCSID
#define ODTUCSID                ODDBOBJECTID            /* {Secret} */
#endif

#ifndef ODTMLSTYLEID
#define ODTMLSTYLEID            ODDBOBJECTID            /* {Secret} */
#endif

#ifndef ODTDB_TIMEZONE
#define ODTDB_TIMEZONE          OdDb::TimeZone         /* {Secret} */
#endif

#ifndef ODTDB_SHADOWFLAGS
#define ODTDB_SHADOWFLAGS       OdGiSubEntityTraits::ShadowFlags      /* {Secret} */
#endif

#ifndef ODVISTYLEID
#define ODVISTYLEID             ODDBOBJECTID            /* {Secret} */
#endif

#ifndef ODTMATERIALID
#define ODTMATERIALID           ODDBOBJECTID            /* {Secret} */
#endif

#ifndef ODTDB_LOFTPARAMTYPE
#define ODTDB_LOFTPARAMTYPE     OdDb::LoftParamType      /* {Secret} */
#endif

#ifndef ODTDB_LOFTNORMALSTYPE
#define ODTDB_LOFTNORMALSTYPE   OdDb::LoftNormalsType      /* {Secret} */
#endif

#ifndef ODTTABLESTYLEID
#define ODTTABLESTYLEID         ODDBOBJECTID            /* {Secret} */
#endif

#ifndef ODTMLEADERSTYLEID
#define ODTMLEADERSTYLEID       ODDBOBJECTID            /* {Secret} */
#endif

#ifndef ODTVIEWDETAILSTYLEID
#define ODTVIEWDETAILSTYLEID    ODDBOBJECTID            /* {Secret} */
#endif

#ifndef ODTVIEWSECTIONSTYLEID
#define ODTVIEWSECTIONSTYLEID   ODDBOBJECTID            /* {Secret} */
#endif

#ifndef ODANNOSCALE
#define ODANNOSCALE             OdDbAnnotationScalePtr /* {Secret} */
#endif

#ifndef ODTTRANSPARENCYDEF
#define ODTTRANSPARENCYDEF OdCmTransparency
#endif

#ifndef ODTCOLORDEF
#define ODTCOLORDEF OdCmColor
#endif

#ifndef VAR_DEF
/* {Secret} */
#define VAR_DEF(a,b,c,d,r1,r2)
/* {Secret} */
#define DOUNDEF_VAR_DEF
#endif


#define SVARS_FUNC_MODIFIER virtual

//         Type                    Name          Default                         Metric default                    Reserve1                    Reserve2
//DOM-IGNORE-END

/**
  <title ANGBASE>
  <toctitle ANGBASE>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   0.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2.5+
  </table>
   
   ANGBASE specifies the direction of angle 0 with respect to the UCS.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getANGBASE()
   OdDbDatabase::setANGBASE()
  </table>
   
   <b>Note:</b> All angles are expressed in radians.

  <group TD_SysVars_A>
*/
//<dom_var_def>double ANGBASE;
VAR_DEF(   ODTORIENT,              ANGBASE,      (0.0),                          (0.0),                            (),                         ValidateNone())
  
/**
  <title ANGDIR>
  <toctitle ANGDIR>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   false
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2.5+
  </table>
   
  ANGDIR controls the direction of positive angles with respect to the UCS.
   
   <table>
   <b>Value</b>   <b>Positive Angles</b>
   false          Counterclockwise
   true           Clockwise
   </table>
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;
   OdDbDatabase::getANGDIR()
   OdDbDatabase::setANGDIR()
   </table> 
   
  <group TD_SysVars_A>
*/
//<dom_var_def>bool ANGDIR;
VAR_DEF(   ODTBOOL,                ANGDIR,       (false),                        (false),                          (),                         ValidateBOOL())

/**
  <title INSBASE>
  <toctitle INSBASE>
   
  <table>
   <b>Type</b>            OdGePoint3d
   <b>Initial value</b>   (0.0, 0.0, 0.0)
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
   INSBASE specifies the WCS insertion base point of the current space or model space.
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getINSBASE()
   OdDbDatabase::setINSBASE()
   </table>   

  <group TD_SysVars_I>
*/
//<dom_var_def>OdGePoint3d INSBASE;
VAR_DEF_H( OdGePoint3d,            INSBASE,      (0.0, 0.0, 0.0),                (0.0, 0.0, 0.0),                  (),                         ValidateNone())

/**
  <title EXTMIN>
  <toctitle EXTMIN>
   
   <table>
   <b>Type</b>            OdGePoint3d
   <b>Initial value</b>   (1.E+20, 1.E+20, 1.E+20)
   <b>Read-Only</b>       Yes (for getSysVar() and setSysVar() methods)
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
   </table>
   
   EXTMIN specifies the lower-left corner of the extents of the Model Space.
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar() &#8211; returns the lower-left corner of the extents of the current space (Model or Paper).
   &nbsp;
   OdDbDatabase::getEXTMIN() &#8211; returns the lower-left corner of the extents of the Model Space.
   OdDbDatabase::setEXTMIN() &#8211; sets the lower-left corner of the extents of the Model Space.
   </table>
   
   <b>Note: </b>get(L"EXTMIN") works with values for current Layout and accesses database EXTMIN 
   or PEXTMIN.   

   \sa
   <link OdDbLayout::getEXTMIN(), OdDbLayout::getEXTMIN()>
   <link OdDbLayout::setEXTMIN(), OdDbLayout::setEXTMIN()>
  <group TD_SysVars_E>
*/
//<dom_var_def>OdGePoint3d EXTMIN;
VAR_DEF_H( OdGePoint3d,            EXTMIN,       (1.E+20, 1.E+20, 1.E+20),       (1.E+20, 1.E+20, 1.E+20),         (),                         ValidateNone())

/**
  <title EXTMAX>
  <toctitle EXTMAX>
   
   <table>
   <b>Type</b>            OdGePoint3d
   <b>Initial value</b>   (-1.0E+20, -1.0E+20, -1.0E+20)
   <b>Read-Only</b>       Yes (for getSysVar() and setSysVar() methods)
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
   </table>
   
   EXTMAX specifies the upper-right corner of the extents of the Model Space.
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar() &#8211; returns the upper-right corner of the extents of the current space (Model or Paper).
   &nbsp; 
   OdDbDatabase::getEXTMAX() &#8211; returns the upper-right corner of the extents of the Model Space.
   OdDbDatabase::setEXTMAX() &#8211; sets the upper-right corner of the extents of the Model Space.
   </table>  

   \sa
   <link OdDbLayout::getEXTMAX(), OdDbLayout::getEXTMAX()>
   <link OdDbLayout::setEXTMAX(), OdDbLayout::setEXTMAX()>
  <group TD_SysVars_E>
*/
//<dom_var_def>OdGePoint3d EXTMAX;
VAR_DEF_H( OdGePoint3d,            EXTMAX,       (-1.E+20, -1.E+20, -1.E+20),    (-1.E+20, -1.E+20, -1.E+20),      (),                         ValidateNone())

/**
  <title LIMMIN>
  <toctitle LIMMIN>
   
   <table>
   <b>Type</b>            OdGePoint2d
   <b>Initial value</b>   (0.0, 0.0)
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
   </table>
   
   The LIMMIN system variable stores the lower-left corner of the grid limits for model space or current space 
   (current layout).
   
  <table>
   <b>Access Methods</b>        
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;                           
   OdDbDatabase::getLIMMIN()
   OdDbDatabase::setLIMMIN()
  </table>
   
  For example, to get the value:
  <code>
   OdRusBufPtr pRb = pDb-\>getSysVar(L"LIMMIN");
   OdGePoint2d limmin = pRb-\>getPoint2d();
   // or //
   OdGePoint2d limmin = pDb-\>getLIMMIN();
   odPrintConsoleString(L"nLower-left limits = (%f,%f)", limmin.x, limmin.y);
   </code>
   For example, to set the value:
   <code>
   OdGePoint2d point(0.5, 0.8);
   OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtPoint2d, point);
   pDb-\>setSysVar(L"LIMMIN", pRb);
   // or //
   pDb-\>setLIMMIN( OdGePoint2d(0.5, 0.8) );
   </code>
   
  <group TD_SysVars_L>
*/
//<dom_var_def>OdGePoint2d LIMMIN;
VAR_DEF_H( OdGePoint2d,            LIMMIN,       (0.0, 0.0),                     (0.0, 0.0),                       (),                         ValidateNone())

/**
  <title LIMMAX>
  <toctitle LIMMAX>
   
   <table>
   <b>Type</b>            OdGePoint2d
   <b>Initial value</b>   (12.0, 9.0) | (420.0, 297.0)
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
   </table>
   
   The LIMMAX system variable stores the upper-right corner of the grid limits for model space or current 
   space (current layout).
   
  <table>
   <b>Access Methods</b>        
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;                            
   OdDbDatabase::getLIMMAX()
   OdDbDatabase::setLIMMAX()
  </table>
   
   For example, to get the value:
   <code>
   OdRusBufPtr pRb = pDb-\>getSysVar(L"LIMMAX");
   OdGePoint2d limmax = pRb-\>getPoint2d();
   // or //
   OdGePoint2d limmax = pDb-\>getLIMMAX();
   odPrintConsoleString(L"nUpper-right limits = (%f,%f)", limmax.x, limmax.y);
   </code>
   For example, to set the value:
   <code>
   OdGePoint2d point(10.5, 8.4);
   OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtPoint2d, point);
   pDb-\>setSysVar(L"LIMMAX", pRb);
   // or //
   pDb-\>setLIMMAX( OdGePoint2d(10.5, 8.4) );
   </code>
   
  <group TD_SysVars_L> 
*/
//<dom_var_def>OdGePoint2d LIMMAX;
VAR_DEF_H( OdGePoint2d,            LIMMAX,       (12.0, 9.0),                    (420.0, 297.0),                   (),                         ValidateNone())

/**
  <title ORTHOMODE>
  <toctitle ORTHOMODE>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   false
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
  
   ORTHOMODE controls the constraint of cursor movement to the perpendicular.
   
  <table>
   <b>Value</b>   <b>Description</b>
   false          Not constrained
   true           Constrained
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;
   OdDbDatabase::getORTHOMODE()
   OdDbDatabase::setORTHOMODE()
  </table> 

  <group TD_SysVars_O>
*/
//<dom_var_def>bool ORTHOMODE;
VAR_DEF(   ODTBOOL,                ORTHOMODE,    (false),                        (false),                          (),                         ValidateBOOL())

/**
  <title REGENMODE>
  <toctitle REGENMODE>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   true
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  REGENMODE controls automatic regeneration.
   
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
   OdDbDatabase::getREGENMODE()
   OdDbDatabase::setREGENMODE()
  </table> 

  <group TD_SysVars_R>
*/
//<dom_var_def>bool REGENMODE;
VAR_DEF(   ODTBOOL,                REGENMODE,    (true),                         (true),                           (),                         ValidateBOOL())

/**
  <title FILLMODE>
  <toctitle FILLMODE>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   true
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
   FILLMODE controls the rendering of Hatches, 2D Solids, and wide Polylines.
   
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
   OdDbDatabase::getFILLMODE()
   OdDbDatabase::setFILLMODE()
   </table>

  <group TD_SysVars_F>
*/
//<dom_var_def>bool FILLMODE;
VAR_DEF(   ODTBOOL,                FILLMODE,     (true),                         (true),                           (),                         ValidateBOOL())

/**
  <title QTEXTMODE>
  <toctitle QTEXTMODE>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   false
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  QTEXTMODE controls the display of text.
   
  <table>
   <b>Value</b>   <b>Description</b>
   false          Text is displayed
   true           Bounding box is displayed
  </table>
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getQTEXTMODE()
   OdDbDatabase::setQTEXTMODE()
   </table>  

  <group TD_SysVars_Q>
   */
//<dom_var_def>bool QTEXTMODE;
VAR_DEF(   ODTBOOL,                QTEXTMODE,    (false),                        (false),                          (),                         ValidateBOOL())

/**
  <title MIRRTEXT>
  <toctitle MIRRTEXT>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   true
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
   MIRRTEXT controls the mirroring of Text with the MIRROR command.
   
  <table>
   <b>Value</b>   <b>Description</b>
   false          Not mirrored
   true           Mirrored
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;
   OdDbDatabase::getMIRRTEXT()
   OdDbDatabase::setMIRRTEXT()
  </table> 

<group TD_SysVars_M>
  */
//<dom_var_def>bool MIRRTEXT;
VAR_DEF(   ODTBOOL,                MIRRTEXT,     (true),                         (true),                           (),                         ValidateBOOL())

/**
  <title LTSCALE>
  <toctitle LTSCALE>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   1.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  LTSCALE specifies the global linetype scale factor.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getLTSCALE()
   OdDbDatabase::setLTSCALE()
  </table>  

<group TD_SysVars_L>
*/
//<dom_var_def>double LTSCALE;
VAR_DEF(   ODTUNITLESS4,           LTSCALE,      (1.0),                          (1.0),                            (),                         ValidatePositiveNonZero())

/**
  <title ATTMODE>
  <toctitle ATTMODE>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   1
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2.5+
  </table>
   
  ATTMODE controls the display of attributes.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              All attributes are invisible
   1              Visible attributes are visible, and invisible attributes are invisible.
   2              All attributes are visible.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getATTMODE()
   OdDbDatabase::setATTMODE()
  </table>  

<group TD_SysVars_A>
*/
//<dom_var_def>OdInt16 ATTMODE;
VAR_DEF(   OdInt16,                ATTMODE,      (1),                            (1),                              (),                         ValidateRange(0,2))

/**
  <title TEXTSIZE>
  <toctitle TEXTSIZE>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   0.2 | 2.5
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  TEXTSIZE specifies the default size for new text entities in the current text style.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getTEXTSIZE()
   OdDbDatabase::setTEXTSIZE()
  </table> 

<group TD_SysVars_T>
*/
//<dom_var_def>double TEXTSIZE;
VAR_DEF_2(   ODTDIST,                TEXTSIZE,     (0.2),                          (2.5),                            (),                         ValidateGr(0.))

/**
  <title TRACEWID>
  <toctitle TRACEWID>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   0.05 | 1.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  TRACEWID is default width for new trace entities.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getTRACEWID()
   OdDbDatabase::setTRACEWID()
  </table> 

<group TD_SysVars_T>
*/
//<dom_var_def>double TRACEWID;
VAR_DEF(   ODTDIST,                TRACEWID,     (0.05),                         (1.0),                            (),                         ValidateEqGr(0.))

/**
  <title TEXTSTYLE>
  <toctitle TEXTSTYLE>
   
  <table>
   <b>Type</b>            OdDbObjectId
   <b>Initial value</b>   "Standard" text style object ID
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  TEXTSTYLE specifies the default style for new text entities.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getTEXTSTYLE()
   OdDbDatabase::setTEXTSTYLE()
  </table>  

<group TD_SysVars_T>
  */
//<dom_var_def>OdDbObjectId TEXTSTYLE;
VAR_DEF_2(   ODTTEXTSTYLEID,         TEXTSTYLE,    (OdDbObjectId::kNull),          (OdDbObjectId::kNull),            (),                         ValidateTextStyle())

/**
  <title CLAYER>
  <toctitle CLAYER>
   
  <table>
   <b>Type</b>            OdDbObjectId
   <b>Initial value</b>   ID of the predefined layer "0" (Zero Layer)
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2.5+
  </table>
   
  CLAYER specifies the default layer for new entities.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getCLAYER()
   OdDbDatabase::setCLAYER()
  </table>  

<group TD_SysVars_C>
*/
//<dom_var_def>OdDbObjectId CLAYER;
VAR_DEF(   ODTLAYERID,             CLAYER,       (OdDbObjectId::kNull),          (OdDbObjectId::kNull),            (),                         ValidateLayer())

/**
  <title CELTYPE>
  <toctitle CELTYPE>
   
  <table>
   <b>Type</b>            OdDbObjectId
   <b>Initial value</b>   "Continuous" linetype record object ID
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2.5+
  </table>
   
  CELTYPE specifies the default linetype for new entities.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getCELTYPE()
   OdDbDatabase::setCELTYPE()
  </table>  

<group TD_SysVars_C>
  */
//<dom_var_def>OdDbObjectId CELTYPE;
VAR_DEF(   ODTLINETYPEID,          CELTYPE,      (OdDbObjectId::kNull),          (OdDbObjectId::kNull),            (),                         ValidateLinetype())

/**
  <title CECOLOR>
  <toctitle CECOLOR>
   
  <table>
   <b>Type</b>            OdCmColor
   <b>Initial value</b>   OdCmEntityColor::kByLayer
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2.5+
  </table>
   
  CECOLOR specifies the default color for new entities.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getCECOLOR()
   OdDbDatabase::setCECOLOR()
  </table>

<group TD_SysVars_C>
  */
//<dom_var_def>OdCmColor CECOLOR;
VAR_DEF(   OdCmColor,              CECOLOR,      (OdCmEntityColor::kByLayer),    (OdCmEntityColor::kByLayer),      (),                         ValidateCmColor())

/**
  <title CELTSCALE>
  <toctitle CELTSCALE>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   1.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        13+
  </table>
   
  CELTSCALE specifies the default linetype scale for new entities.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getCELTSCALE()
   OdDbDatabase::setCELTSCALE()
  </table> 

<group TD_SysVars_C>
  */
//<dom_var_def>double CELTSCALE;
VAR_DEF(   ODTUNITLESS4,           CELTSCALE,    (1.0),                          (1.0),                            (),                         ValidateGr(0.))

/**
  <title CHAMFERA>
  <toctitle CHAMFERA>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   0.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        9+
  </table>
   
  The CHAMFERA variable specifies a default value of the first chamfer distance in drawing units.
   
  <table>
   <b>Access Methods</b>  
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getCHAMFERA()
   OdDbDatabase::setCHAMFERA()
  </table>
   
  For example, to get the value:
  <code>
   OdResBufPtr pRb = pDb-\>getSysVar(L"CHAMFERA");
   double distance1 = pRb-\>getDouble();
   odPrintConsoleString(L"nFirst chamfer distance = %g", distance1);
   // or //
   odPrintConsoleString(L"nFirst chamfer distance = %g", pDb-\>getCHAMFERA());
  </code>
  For example, to set the value:
  <code>
   OdResBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtDouble, 2.5);
   pDb-\>setSysVar(L"CHAMFERA", pRb);
   // or //
   pDb-\>setCHAMFERA(2.5);
  </code>
   
  <b>Note:</b> Use together with the CHAMFERB variable. 

  <group TD_SysVars_C>
*/
//<dom_var_def>double CHAMFERA;
VAR_DEF(   ODTDIST,                CHAMFERA,     (0.0),                           (0.0),                           (),                         ValidateEqGr(0.))

/** 
  <title CHAMFERB>
  <toctitle CHAMFERB>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   0.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        9+
  </table>
   
  The CHAMFERB variable specifies a default value of the second chamfer distance in drawing units.
   
  <table>
    <b>Access Methods</b>  
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getCHAMFERB()
   OdDbDatabase::setCHAMFERB()
  </table>
   
  For example, to get the value:
  <code>
   OdRusBufPtr pRb = pDb-\>getSysVar(L"CHAMFERB");
   double distance2 = pRb-\>getDouble();
   odPrintConsoleString(L"nSecond chamfer distance = %g", distance2);
   // or //
   odPrintConsoleString(L"nSecond chamfer distance = %g", pDb-\>getCHAMFERB());
  </code>
  For example, to set the value:
  <code>
   OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtDouble, 1.25);
   pDb-\>setSysVar(L"CHAMFERB", pRb);
   // or //
   pDb-\>setCHAMFERB(1.25);
  </code>
   
  <b>Note:</b> Use together with the CHAMFERA variable.  

<group TD_SysVars_C>
  */
//<dom_var_def>double CHAMFERB;
VAR_DEF(   ODTDIST,                CHAMFERB,     (0.0),                           (0.0),                           (),                         ValidateEqGr(0.))

/**
  <title CHAMFERC>
  <toctitle CHAMFERC>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   0.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        13+
  </table>
   
  The CHAMFERC variable specifies a default value of the chamfer length in drawing units.
   
  <table>
   <b>Access Methods</b>  
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getCHAMFERC()
   OdDbDatabase::setCHAMFERC()
  </table>
   
  For example, to get the value:
  <code>
   OdResBufPtr pRb = pDb-\>getSysVar(L"CHAMFERC");
   double length = pRb-\>getDouble();
   odPrintConsoleString(L"nChamfer length = %g units", length);
   // or //
   odPrintConsoleString(L"nChamfer length = %g units", pDb-\>getCHAMFERC());
   </code>
   For example, to set the value:
   <code>
   OdResBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtDouble, 2.25);
   pDb-\>setSysVar(L"CHAMFERC", pRb);
   // or //
   pDb-\>setCHAMFERC(2.25);
  </code>
   
  <b>Note:</b> Use together with the CHAMFERD variable.   

<group TD_SysVars_C>
  */
//<dom_var_def>double CHAMFERC;
VAR_DEF(   ODTDIST,                CHAMFERC,     (0.0),                           (0.0),                           (),                         ValidateEqGr(0.))

/**
  <title CHAMFERD>
  <toctitle CHAMFERD>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   0.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        13+
  </table>
   
  The CHAMFERD variable specifies a default value of the chamfer angle in radians.
   
  <table>
   <b>Access Methods</b>  
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getCHAMFERD()
   OdDbDatabase::setCHAMFERD()
  </table>
   
  For example, to get a value:
  <code>
   OdResBufPtr pRb = pDb-\>getSysVar(L"CHAMFERD");
   double angle = pRb-\>getDouble();
   odPrintConsoleString(L"nChamfer angle = %g radians", angle);
   // or //
   odPrintConsoleString(L"nChamfer angle = %g radians", pDb-\>getCHAMFERD());
  </code>
  For example, to set a value:
  <code>
   OdResBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtAngle, 0.375);
   pDb-\>setSysVar(L"CHAMFERD", pRb);
   // or //
   pDb-\>setCHAMFERD(0.375);
  </code>
   
  <b>Note:</b> Use together with the CHAMFERC variable. 

<group TD_SysVars_C>
*/
//<dom_var_def>double CHAMFERD;
VAR_DEF(   ODTORIENT,              CHAMFERD,     (0.0),                          (0.0),                            (),                         ValidateNone())

/**
  <title DISPSILH>
  <toctitle DISPSILH>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   false
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R13+
  </table>
   
  DISPSILH controls the display of silhouette curve edges and meshes.
   
  <table>
   <b>Value</b>   <b>Curve Edges</b>   <b>Meshes</b>
   false          Not displayed        Displayed
   true           Displayed            Not displayed
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getDISPSILH()
   OdDbDatabase::setDISPSILH()
  </table> 

  <group TD_SysVars_D>
  */
//<dom_var_def>bool DISPSILH;
VAR_DEF(   ODTBOOL,                DISPSILH,     (false),                        (false),                          (),                         ValidateBOOL())

/**
  <title DIMSTYLE>
  <toctitle DIMSTYLE>
   
  <table>
   <b>Type</b>             OdDbObjectId
   <b>Initial value</b>    "Standard" dimension style object ID for Imperial drawings or "ISO-25" dimension style object ID for Metric drawings.
   <b>Read-Only</b>        No
   <b>Saved In</b>         Database
   <b>Versions</b>         R12+
  </table>
   
  DIMSTYLE specifies the default dimension style for new entities.
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   &nbsp; 
   OdDbDatabase::getDIMSTYLE()
   OdDbDatabase::setDIMSTYLE()
   </table>  

<group TD_SysVars_D>
   */
//<dom_var_def>OdDbObjectId DIMSTYLE;
VAR_DEF(   ODTDIMSTYLEID,          DIMSTYLE,     (OdDbObjectId::kNull),          (OdDbObjectId::kNull),            (),                         ValidateDimStyle())

/**
  <title DIMASO>
  <toctitle DIMASO>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   true
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2.6+
  </table>
   
  DIMASO controls the creation of Associative dimension objects.
   
  <table>
   <b>Value</b>   <b>Description</b>
   false          Creates exploded dimensions.
   true           Creates associative dimension entities.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getDIMASO()
   OdDbDatabase::setDIMASO()
  </table>
   
  <b>Note:</b> DIMASO is obsolete, but is included for compatibility purposes.
   
  \sa
   <link DIMASSOC, DIMASSOC>

<group TD_SysVars_D>
 */
//<dom_var_def>bool DIMASO;
VAR_DEF(   ODTBOOL,                   DIMASO,       (true),                         (true),                           (),                         ValidateBOOL())

/**
  <title DIMSHO>
  <toctitle DIMSHO>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   true
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  DIMSHO controls the updating of associative dimension object while dragging.
   
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
   OdDbDatabase::getDIMSHO()
   OdDbDatabase::setDIMSHO()
  </table>
   
  <b>Note:</b> DIMSHO is obsolete, but is included for compatibility purposes.  

<group TD_SysVars_D>
  */
//<dom_var_def>bool DIMSHO;
VAR_DEF(   bool,                   DIMSHO,       (true),                         (true),                           (),                         ValidateNone())

/**
  <title LUNITS>
  <toctitle LUNITS>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   2
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  LUNITS controls the display of linear units.
   
  <table>
   <b>Value</b>   <b>Description</b>
   1              Scientific
   2              Decimal
   3              Engineering
   4              Architectural
   5              Fractional
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getLUNITS()
   OdDbDatabase::setLUNITS()
  </table>
   
  <b>Note:</b> LUNITS does not control the formatting of dimension text.  

<group TD_SysVars_L>
  */
//<dom_var_def>OdInt16 LUNITS;
VAR_DEF(   OdInt16,                LUNITS,       (2),                            (2),                              (),                         ValidateRange(1,5))

/**
  <title LUPREC>
  <toctitle LUPREC>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   4
   <b>Range</b>           0 ... 8
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  LUPREC specifies the number of decimal places in the display of linear units.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getLUPREC()
   OdDbDatabase::setLUPREC()
  </table>
   
  <b>Note:</b> LUPREC does not control the formatting of dimension text. 

<group TD_SysVars_L>
  */
//<dom_var_def>OdInt16 LUPREC;
VAR_DEF(   OdInt16,                LUPREC,       (4),                            (4),                              (),                         ValidateRange(0,8))

/**
  <title SKETCHINC>
  <toctitle SKETCHINC>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   0.1
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  SKETCHINC specifies the recording increment for the SKETCH command.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getSKETCHINC()
   OdDbDatabase::setSKETCHINC()
  </table> 

<group TD_SysVars_S>
  */
//<dom_var_def>double SKETCHINC;
VAR_DEF(   ODTDIST,                SKETCHINC,    (0.1),                          (1.0),                            (),                         ValidateNone())

/**
  <title FILLETRAD>
  <toctitle FILLETRAD>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   0.5 | 10.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  FILLETRAD specifies the default fillet radius.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getFILLETRAD()
   OdDbDatabase::setFILLETRAD()
  </table> 

<group TD_SysVars_F>
  */
//<dom_var_def>double FILLETRAD;
VAR_DEF(   ODTDIST,                FILLETRAD,    (0.5),                          (10.0),                           (),                         ValidateEqGr(0.))

/**
  <title AUNITS>
  <toctitle AUNITS>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   0
   <b>Range</b>           0 ... 4
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  AUNITS controls the display of angular units.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Degrees
   1              Degrees-Minutes-Seconds
   2              Gradians
   3              Radians
   4              Surveyor Units
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getAUNITS()
   OdDbDatabase::setAUNITS()
  </table>  

<group TD_SysVars_A>
  */
//<dom_var_def>OdInt16 AUNITS;
VAR_DEF(   OdInt16,                AUNITS,       (0),                            (0),                              (),                         ValidateRange(0,4))

/**
  <title AUPREC>
  <toctitle AUPREC>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   0
   <b>Range</b>           0 ... 8
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2.5+
  </table>
   
  AUPREC specifies the number of decimal places in the display of angular units.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getAUPREC()
   OdDbDatabase::setAUPREC()
  </table>
   
  <b>Note: </b>AUPREC does not control the formatting of dimension text.   

<group TD_SysVars_A>
  */
//<dom_var_def>OdInt16 AUPREC;
VAR_DEF(   OdInt16,                AUPREC,       (0),                            (0),                              (),                         ValidateRange(0,8))

/**
  <title MENUNAME>
  <toctitle MENUNAME>
   
  <table>
   <b>Type</b>            OdString
   <b>Initial value</b>   "."
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  MENUNAME returns the path and file name of the customization file.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getMENUNAME()
   OdDbDatabase::setMENUNAME()
  </table> 

<group TD_SysVars_M>
  */
//<dom_var_def>OdString MENUNAME;
VAR_DEF_H( OdString,               MENUNAME,     (OD_T(".")),                          (OD_T(".")),                            (),                         ValidateNone())

/**
  <title ELEVATION>
  <toctitle ELEVATION>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   0.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  ELEVATION specifies the default elevation for new entities in the current space or model space.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getELEVATION()
   OdDbDatabase::setELEVATION()
  </table> 

  <group TD_SysVars_E>
  */
//<dom_var_def>double ELEVATION;
VAR_DEF_H( ODTDIST,                ELEVATION,    (0.0),                          (0.0),                            (),                         ValidateNone())

/**
  <title PELEVATION>
  <toctitle PELEVATION>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   0.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        N/A
  </table>
   
  PELEVATION specifies the default elevation for new entities in the current paper space.
   
  <table>
   OdDbDatabase::getPELEVATION()
   OdDbDatabase::setPELEVATION()
  </table> 

<group TD_SysVars_P>
  */
//<dom_var_def>double PELEVATION;
VAR_DEF_H( ODTDIST,                PELEVATION,   (0.0),                          (0.0),                            (),                         ValidateNone())

/**
  <title THICKNESS>
  <toctitle THICKNESS>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   0.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  THICKNESS specifies the default 3D thickness for new entities.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getTHICKNESS()
   OdDbDatabase::setTHICKNESS()
  </table>  

<group TD_SysVars_T>
  */
//<dom_var_def>double THICKNESS;
VAR_DEF(   ODTDIST,                THICKNESS,    (0.0),                          (0.0),                            (),                         ValidateNone())

/**
  <title LIMCHECK>
  <toctitle LIMCHECK>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   false
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  The LIMCHECK system variable determines whether objects are allowed outside the grid limits for model 
  space or current space (current layout).
   
  <table>
   <b>Value</b>   <b>Description</b>
   false          Objects outside limits are allowed
   true           Objects outside limits are disallowed
  </table>
   
   <table>
   <b>Access Methods</b>  
   OdDbDatabase::getSysVar() &#8211; gets the outside limits status as an instance of tagged data (boolean type)
   OdDbDatabase::setSysVar() &#8211; sets the outside limits status as an instance of tagged data (boolean type)
   &nbsp; 
   OdDbDatabase::getLIMCHECK() &#8211; gets the outside limits status as a boolean value
   OdDbDatabase::setLIMCHECK() &#8211; sets the outside limits status as a boolean value
  </table>
   
  For example, to get the value:
  <code>
   OdRusBufPtr pRb = pDb-\>getSysVar(L"LIMCHECK");
   bool status = pRb-\>getBool();
   odPrintConsoleString(L"nOutside limits status = %s", ((status) ? L"On" : L"Off"));
   // or //
   odPrintConsoleString(L"nObjects outside limits are %s for current space",
                      (pDb-\>getLIMCHECK()) ? L"disallowed" : L"allowed");
  </code>
  For example, to set the value:
  <code>
   OdRusBufPtr pRb = OdResBuf::newRb(OdOdResBuf::kRtBool, true);
   pDb-\>setSysVar(L"LIMCHECK", pRb);
   // or //
   pDb-\>setLIMCHECK(false);
  </code>
  
  <group TD_SysVars_L>
  */
//<dom_var_def>bool LIMCHECK;
VAR_DEF_H( ODTBOOL,                   LIMCHECK,     (false),                        (false),                          (),                         ValidateBOOL())

/**
  <title SKPOLY>
  <toctitle SKPOLY>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   false
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  SKPOLY controls the generation of polylines during the SKETCH command.
   
  <table>
   <b>Value</b>   <b>Description</b>
   false          Generates Lines
   true           Generates Polylines
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getSKPOLY()
   OdDbDatabase::setSKPOLY()
  </table> 

<group TD_SysVars_S>
  */
//<dom_var_def>bool SKPOLY;
VAR_DEF(   ODTBOOL,                SKPOLY,       (false),                        (false),                          (),                         ValidateBOOL())

/**
  <title TDUCREATE>
  <toctitle TDUCREATE>
   
  <table>
   <b>Type</b>            OdDbDate
   <b>Initial value</b>   OdDbDate::kInitZero
   <b>Read-Only</b>       Yes
   <b>Saved In</b>        Database
   <b>Versions</b>        2002+
  </table>
   
  TDUCREATE represents the time and date, in Universal Time, the database was created.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   &nbsp; 
   OdDbDatabase::getTDUCREATE()
  </table>
  
  \sa
  <link odDbSetTDUCREATE(), odDbSetTDUCREATE()> 

<group TD_SysVars_T>
  */
//<dom_var_def>OdDbDate TDUCREATE;
RO_VAR_DEF(OdDbDate,               TDUCREATE,    (OdDbDate::kInitZero),          (OdDbDate::kInitZero),            (),                         ValidateNone())

/**
  <title TDUUPDATE>
  <toctitle TDUUPDATE>
   
  <table>
   <b>Type</b>            OdDbDate
   <b>Initial value</b>   OdDbDate::kInitZero
   <b>Read-Only</b>       Yes
   <b>Saved In</b>        Database
   <b>Versions</b>        2002+
  </table>
   
  TDUUPDATE represents the time and date, in Universal Time, the database was last saved.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   &nbsp; 
   OdDbDatabase::getTDUUPDATE()
  </table>
  
  \sa
   <link odDbSetTDUUPDATE(), odDbSetTDUUPDATE()> 

  <group TD_SysVars_T>
 */
//<dom_var_def>OdDbDate TDUUPDATE;
RO_VAR_DEF(OdDbDate,               TDUUPDATE,    (OdDbDate::kInitZero),          (OdDbDate::kInitZero),            (),                         ValidateNone())

/**
  <title TDINDWG>
  <toctitle TDINDWG>
   
  <table>
   <b>Type</b>            OdDbDate
   <b>Initial value</b>   OdDbDate::kInitZero
   <b>Read-Only</b>       Yes
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  TDINDWG represents the total editing time of the database.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   &nbsp; 
   OdDbDatabase::getTDINDWG()
  </table>
  
  \sa
  <link odDbSetTDINDWG(), odDbSetTDINDWG()> 

  <group TD_SysVars_T>
  */
//<dom_var_def>OdDbDate TDINDWG;
RO_VAR_DEF(OdDbDate,               TDINDWG,      (OdDbDate::kInitZero),          (OdDbDate::kInitZero),            (),                         ValidateNone())

/**
  <title TDUSRTIMER>
  <toctitle TDUSRTIMER>
   
  <table>
   <b>Type</b>            OdDbDate
   <b>Initial value</b>   OdDbDate::kInitZero
   <b>Read-Only</b>       Yes
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  TDUSRTIMER represents the user-elapsed timer.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   &nbsp; 
   OdDbDatabase::getTDUSRTIMER()
  </table>
  
  \sa
  <link odDbSetTDUSRTIMER(), odDbSetTDUSRTIMER()>

  <group TD_SysVars_T>
*/
//<dom_var_def>OdDbDate TDUSRTIMER;
RO_VAR_DEF(OdDbDate,               TDUSRTIMER,   (OdDbDate::kInitZero),          (OdDbDate::kInitZero),            (),                         ValidateNone())

/**
  <title USRTIMER>
  <toctitle USRTIMER>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   true
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        N/A
  </table>
   
  USRTIMER controls the user elapsed timer.
   
  <table>
   <b>Value</b>   <b>Description</b>
   false          Disabled
   true           Enabled
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getUSRTIMER()
   OdDbDatabase::setUSRTIMER()
  </table>  

<group TD_SysVars_U>
  */
//<dom_var_def>bool USRTIMER;
VAR_DEF_H( ODTBOOL,                USRTIMER,     (true),                         (true),                           (),                         ValidateBOOL())

/** 
  <title PDMODE>
  <toctitle PDMODE>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  PDMODE specifies the display of Point entities.
  
  <image PDMODE.gif>
  
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getPDMODE()
   OdDbDatabase::setPDMODE()
  </table>  

<group TD_SysVars_P>
  */
//<dom_var_def>OdInt16 PDMODE;
VAR_DEF(   OdInt16,                PDMODE,       (0),                            (0),                              (),                         ValidatePDMODE())

/**
  <title PDSIZE>
  <toctitle PDSIZE>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   0.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  PDSIZE specifies the display size of point entities.
   
  <table>
   <b>Value</b>     <b>Description</b>
   &lt;0            Fraction of viewport size in drawing units.
   0                5% of viewport size in drawing units.
   &gt;0            Size in drawing units.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getPDSIZE()
   OdDbDatabase::setPDSIZE()
  </table> 

<group TD_SysVars_P>
  */
//<dom_var_def>double PDSIZE;
VAR_DEF(   ODTDIST,                PDSIZE,       (0.0),                          (0.0),                            (),                         ValidateNone())

/**
  <title PLINEWID>
  <toctitle PLINEWID>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   0.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R13+
  </table>
   
  PLINEWID specifies the default width for new Polyline entities. Value should be greater than or equal to 0.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getPLINEWID()
   OdDbDatabase::setPLINEWID()
  </table>  

<group TD_SysVars_P>
  */
//<dom_var_def>double PLINEWID;
VAR_DEF(   ODTDIST,                PLINEWID,     (0.0),                          (0.0),                            (),                         ValidateNone())

/**
  <title SPLFRAME>
  <toctitle SPLFRAME>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   false
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  SPLFRAME controls the display of control polygons for Splines and spline-fit Polylines.
   
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
   OdDbDatabase::getSPLFRAME()
   OdDbDatabase::setSPLFRAME()
  </table> 

<group TD_SysVars_P>
*/
//<dom_var_def>bool SPLFRAME;
VAR_DEF(   ODTBOOL,                SPLFRAME,     (false),                        (false),                          (),                         ValidateBOOL())

/**
  <title SPLINETYPE>
  <toctitle SPLINETYPE>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   6
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  SPLINETYPE specifies the type of spline generated by the PEDIT/Spline command.
   
  <table>
   <b>Value</b>   <b>Description</b>
   5              Quadratic B-spline
   6              Cubic B-spline
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getSPLINETYPE()
   OdDbDatabase::setSPLINETYPE()
  </table>  

<group TD_SysVars_S>
  */
//<dom_var_def>OdInt16 SPLINETYPE;
VAR_DEF(   OdInt16,                SPLINETYPE,   (6),                            (6),                              (),                         ValidateRange(5,6))

/**
  <title SPLINESEGS>
  <toctitle SPLINESEGS>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   8
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
  
  The SPLINESEGS variable stores the number of segments for spline-fit polylines generated by the 
  PEDIT/Spline command. The value is a non-zero integer number from -32768 to 32767. A negative 
  value generates circular segments. A positive value generates linear segments. A zero value is not
  applicable. Fit-type polylines use arcs as approximating segments.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getSPLINESEGS()
   OdDbDatabase::setSPLINESEGS()
  </table>
   
   For example, to get the value:
   <code>
   OdRusBufPtr pRb = pDb-\>getSysVar(L"SPLINESEGS");
   OdInt16 number = pRb-\>getInt16();
   odPrintConsoleString(L"nNumber of segments = %d", number);
   // or //
   odPrintConsoleString(L"nNumber of segments = %d", pDb-\>getSPLINESEGS());
   </code>
   For example, to set the value:
   <code>
   OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtInt16, 60);
   pDb-\>setSysVar(L"SPLINESEGS", pRb);
   // or //
   pDb-\>setSPLINESEGS(120);
   </code>  

<group TD_SysVars_S>
   */
//<dom_var_def>OdInt16 SPLINESEGS;
VAR_DEF(   OdInt16,                SPLINESEGS,   (8),                            (8),                              (),                         ValidateNonZero())

/**
  <title SURFTAB1>
  <toctitle SURFTAB1>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   6
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  SURFTAB1 specifies the number of tabulations generated in the
  M direction by the EDGESURF, REVSURF, RULESURF, and TABSURF
  commands.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getSURFTAB1()
   OdDbDatabase::setSURFTAB1()
  </table> 

<group TD_SysVars_S>
  */
//<dom_var_def>OdInt16 SURFTAB1;
VAR_DEF(   OdInt16,                SURFTAB1,     (6),                            (6),                              (),                         ValidateNone())

/**
  <title SURFTAB2>
  <toctitle SURFTAB2>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   6
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  SURFTAB2 specifies the number of tabulations generated in the N direction by the EDGESURF and REVSURF commands.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getSURFTAB2()
   OdDbDatabase::setSURFTAB2()
  </table> 

<group TD_SysVars_S>
  */
//<dom_var_def>OdInt16 SURFTAB2;
VAR_DEF(   OdInt16,                SURFTAB2,     (6),                            (6),                              (),                         ValidateNone())

/**
  <title SURFTYPE>
  <toctitle SURFTYPE>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   6
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  SURFTYPE specifies the type of surface fitting for the PEDIT/Smooth command.
   
  <table>
   <b>Value</b>   <b>Description</b>
   5              Quadratic B-spline surface
   6              Cubic B-spline surface
   8              Bezier surface
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getSURFTYPE()
   OdDbDatabase::setSURFTYPE()
  </table>  

<group TD_SysVars_S>
  */
//<dom_var_def>OdInt16 SURFTYPE;
VAR_DEF(   OdInt16,                SURFTYPE,     (6),                            (6),                              (),                         ValidateRange(5,8))

/**
  <title SURFU>
  <toctitle SURFU>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   6
   <b>Range</b>           0 ... 200
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  SURFU specifies the surface density in the M direction for the PEDIT/Smooth command.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getSURFU()
   OdDbDatabase::setSURFU()
  </table> 

<group TD_SysVars_S>
  */
//<dom_var_def>OdInt16 SURFU;
VAR_DEF(   OdInt16,                SURFU,        (6),                            (6),                              (),                         ValidateRange(0,200))

/**
  <title SURFV>
  <toctitle SURFV>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   6
   <b>Range</b>           0 ... 200
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  SURFV specifies the surface density in the N direction for the PEDIT/Smooth command.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getSURFV()
   OdDbDatabase::setSURFV()
  </table>  

<group TD_SysVars_S>
  */
//<dom_var_def>OdInt16 SURFV;
VAR_DEF(   OdInt16,                SURFV,        (6),                            (6),                              (),                         ValidateRange(0,200))

/**
  <title USERI1>
  <toctitle USERI1>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  USERI1 is 'reserved for user applications'.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getUSERI1()
   OdDbDatabase::setUSERI1()
  </table> 

<group TD_SysVars_U>
  */
//<dom_var_def>OdInt16 USERI1;
VAR_DEF(   OdInt16,                USERI1,       (0),                            (0),                              (),                         ValidateNone())

/**
  <title USERI2>
  <toctitle USERI2>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  USERI2 is 'reserved for user applications'.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getUSERI2()
   OdDbDatabase::setUSERI2()
  </table>  

<group TD_SysVars_U>
  */
//<dom_var_def>OdInt16 USERI2;
VAR_DEF(   OdInt16,                USERI2,       (0),                            (0),                              (),                         ValidateNone())

/**
  <title USERI3>
  <toctitle USERI3>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  USERI3 is 'reserved for user applications'.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getUSERI3()
   OdDbDatabase::setUSERI3()
  </table>  

  <group TD_SysVars_U>
  */
//<dom_var_def>OdInt16 USERI3;
VAR_DEF(   OdInt16,                USERI3,       (0),                            (0),                              (),                         ValidateNone())

/**
  <title USERI4>
  <toctitle USERI4>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  USERI4 is 'reserved for user applications'.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getUSERI4()
   OdDbDatabase::setUSERI4()
  </table> 

<group TD_SysVars_U>
  */
//<dom_var_def>OdInt16 USERI4;
VAR_DEF(   OdInt16,                USERI4,       (0),                            (0),                              (),                         ValidateNone())

/**
  <title USERI5>
  <toctitle USERI5>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  USERI5 is 'reserved for user applications'.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getUSERI5()
   OdDbDatabase::setUSERI5()
  </table>  

<group TD_SysVars_U>
  */
//<dom_var_def>OdInt16 USERI5;
VAR_DEF(   OdInt16,                USERI5,       (0),                            (0),                              (),                         ValidateNone())

/**
  <title USERR1>
  <toctitle USERR1>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   0.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  USERR1 is 'reserved for user applications'.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getUSERR1()
   OdDbDatabase::setUSERR1()
  </table>  

<group TD_SysVars_U>
  */
//<dom_var_def>double USERR1;
VAR_DEF(   ODTDIST,                USERR1,       (0.0),                          (0.0),                            (),                         ValidateNone())

/**
  <title USERR2>
  <toctitle USERR2>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   0.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  USERR2 is 'reserved for user applications'.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getUSERR2()
   OdDbDatabase::setUSERR2()
  </table> 

<group TD_SysVars_U>
  */
//<dom_var_def>double USERR2;
VAR_DEF(   ODTDIST,                USERR2,       (0.0),                          (0.0),                            (),                         ValidateNone())

/**
  <title USERR3>
  <toctitle USERR3>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   0.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  USERR3 is 'reserved for user applications'.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getUSERR3()
   OdDbDatabase::setUSERR3()
  </table>  

<group TD_SysVars_U>
  */
//<dom_var_def>double USERR3;
VAR_DEF(   ODTDIST,                USERR3,       (0.0),                          (0.0),                            (),                         ValidateNone())

/**
  <title USERR4>
  <toctitle USERR4>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   0.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  USERR4 is 'reserved for user applications'.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getUSERR4()
   OdDbDatabase::setUSERR4()
  </table>  

<group TD_SysVars_U>
  */
//<dom_var_def>double USERR4;
VAR_DEF(   ODTDIST,                USERR4,       (0.0),                          (0.0),                            (),                         ValidateNone())

/**
  <title USERR5>
  <toctitle USERR5>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   0.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  USERR5 is 'reserved for user applications'.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getUSERR5()
   OdDbDatabase::setUSERR5()
  </table> 

<group TD_SysVars_U>
  */
//<dom_var_def>double USERR5;
VAR_DEF(   ODTDIST,                USERR5,       (0.0),                          (0.0),                            (),                         ValidateNone())

/**
  <title WORLDVIEW>
  <toctitle WORLDVIEW>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   true
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  WORLDVIEW controls the switching of the UCS to the WCS for the duration of the 3DORBIT, DVIEW and VPOINT commands.
   
  <table>
   <b>Value</b>   <b>Description</b>
   false          UCS is unchanged.
   true           UCS switches to WCS.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getWORLDVIEW()
   OdDbDatabase::setWORLDVIEW()
  </table> 

<group TD_SysVars_W>
  */
//<dom_var_def>bool WORLDVIEW;
VAR_DEF(   ODTBOOL,                WORLDVIEW,    (true),                         (true),                           (),                         ValidateBOOL())

/**
  <title SHADEDGE>
  <toctitle SHADEDGE>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   3
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  SHADEDGE specifies the rendering of faces and edges with the SHADE command.
   
  <table>
   <b>Value</b>   <b>Faces</b>   <b>Edges</b>
   0              Shaded         Not highlighted
   1              Shaded         Background color
   2              Not filled     Object color
   3              Object color   Background color
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getSHADEDGE()
   OdDbDatabase::setSHADEDGE()
  </table>  

<group TD_SysVars_S>
  */
//<dom_var_def>OdInt16 SHADEDGE;
VAR_DEF(   OdInt16,                SHADEDGE,     (3),                            (3),                              (),                         ValidateRange(0,3))

/**
  <title SHADEDIF>
  <toctitle SHADEDIF>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   70
   <b>Range</b>           0 ... 100
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  SHADEDIF specifies the percentage of diffuse reflective light (vs. ambient light) when (SHADEDGE == 0) || (SHADEDGE == 1).
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getSHADEDIF()
   OdDbDatabase::setSHADEDIF()
  </table>  

<group TD_SysVars_S>
*/
//<dom_var_def>OdInt16 SHADEDIF;
VAR_DEF(   OdInt16,                SHADEDIF,     (70),                           (70),                             (),                         ValidateRange(0,100))

/**
  <title TILEMODE>
  <toctitle TILEMODE>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   true
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  The TILEMODE system variable determines whether model or paper layout tab is active for database.
   
  <table>
   <b>Value</b>   <b>Description</b>
   false          Current paper layout tab is active.
   true           Model layout tab is active.
  </table>
   
  <b>Note: </b>The last active layout tab will be activated when TILEMODE is changed from true to false. 
  The database stores last layout tab status.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getTILEMODE()
   OdDbDatabase::setTILEMODE()
  </table>
   
  For example, to get the value:
  <code>
   OdRusBufPtr pRb = pDb-\>getSysVar(L"TILEMODE");
   bool status = pRb-\>getBool();
   odPrintConsoleString(L"nCurrent layout tab is %s", ((status) ? L"model" : L"paper"));
   // or //
   odPrintConsoleString(L"nThe %s layout is active", (pDb-\>getTILEMODE()) ? L"model" : L"paper");
  </code>
  For example, to set the value:
  <code>
   OdRusBufPtr pRb = OdResBuf::newRb(OdOdResBuf::kRtBool, true);
   pDb-\>setSysVar(L"TILEMODE", pRb);
   // or //
   pDb-\>setTILEMODE(false);
  </code>
  
<group TD_SysVars_T>
  */
//<dom_var_def>bool TILEMODE;
VAR_DEF_2( ODTBOOL,                TILEMODE,     (true),                         (true),                           (),                         ValidateBOOL())

/**
  <title MAXACTVP>
  <toctitle MAXACTVP>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   64
   <b>Range</b>           2 ... 64
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  MAXACTVP specifies the maximum of active viewports in a layout.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getMAXACTVP()
   OdDbDatabase::setMAXACTVP()
  </table> 

<group TD_SysVars_M>
  */
//<dom_var_def>OdInt16 MAXACTVP;
VAR_DEF(   OdInt16,                MAXACTVP,     (64),                           (64),                             (),                         ValidateRange(2,64))

/**
  <title PINSBASE>
  <toctitle PINSBASE>
   
  <table>
   <b>Type</b>            OdGePoint3d
   <b>Initial value</b>   (0.0, 0.0, 0.0)
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        N/A
  </table>
   
  PINSBASE specifies the WCS insertion base point of the current paper space.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getPINSBASE()
   OdDbDatabase::setPINSBASE()
  </table>  

  <group TD_SysVars_P>
  */
//<dom_var_def>OdGePoint3d PINSBASE;
VAR_DEF_H( OdGePoint3d,            PINSBASE,     (0.0, 0.0, 0.0),                (0.0, 0.0, 0.0),                  (),                         ValidateNone())

/** 
  <title PLIMCHECK>
  <toctitle PLIMCHECK>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   false
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        11+
  </table>
   
  The PLIMCHECK system variable determines whether objects are allowed outside the grid limits for 
  paper space (current paper layout).
   
  <table>
   <b>Value</b>   <b>Description</b>
   false          Objects outside paper limits are allowed.
   true           Objects outside paper limits are disallowed.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getPLIMCHECK()
   OdDbDatabase::setPLIMCHECK()
  </table>
   
  For example, to get the value:
  <code>
   OdRusBufPtr pRb = pDb-\>getSysVar(L"PLIMCHECK");
   bool status = pRb-\>getBool();
   odPrintConsoleString(L"nOutside paper limits status = %s", ((status) ? L"On" : L"Off"));
   // or //
   odPrintConsoleString(L"nObjects outside limits are %s for paper space", (pDb-\>getPLIMCHECK()) ? L"disallowed" : L"allowed");
  </code>
  For example, to set the value:
  <code>
   OdRusBufPtr pRb = OdResBuf::newRb(OdOdResBuf::kRtBool, true);
   pDb-\>setSysVar(L"PLIMCHECK", pRb);
   // or //
   pDb-\>setPLIMCHECK(false);
   </code>
 
<group TD_SysVars_P>
 */
//<dom_var_def>bool PLIMCHECK;
VAR_DEF_H( ODTBOOL,                PLIMCHECK,    (false),                        (false),                          (),                         ValidateBOOL())

/**
  <title PEXTMIN>
  <toctitle PEXTMIN>
   
  <table>
   <b>Type</b>            OdGePoint3d
   <b>Initial value</b>   (1.0E+20, 1.0E+20, 1.0E+20)
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        N/A
  </table>
   
  PEXTMIN specifies the lower-left corner of the extents of the current Paper Space.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getPEXTMIN()
   OdDbDatabase::setPEXTMIN()
  </table>
   
  <b>Note: </b>get/setSysVar() does not work for this variable. See <link EXTMIN, EXTMIN> description.

<group TD_SysVars_P>
  */
//<dom_var_def>OdGePoint3d PEXTMIN;
VAR_DEF_H( OdGePoint3d,            PEXTMIN,      (1.E+20, 1.E+20, 1.E+20),       (1.E+20, 1.E+20, 1.E+20),         (),                         ValidateNone())

/**
  <title PEXTMAX>
  <toctitle PEXTMAX>
   
  <table>
   <b>Type</b>            OdGePoint3d
   <b>Initial value</b>   (-1.E+20, -1.E+20, -1.E+20)
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        N/A
  </table>
   
   PEXTMAX specifies the upper-right corner of the extents of the current Paper Space.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getPEXTMAX()
   OdDbDatabase::setPEXTMAX()
  </table>  

<group TD_SysVars_P>
  */
//<dom_var_def>OdGePoint3d PEXTMAX;
VAR_DEF_H( OdGePoint3d,            PEXTMAX,      (-1.E+20, -1.E+20, -1.E+20),    (-1.E+20, -1.E+20, -1.E+20),      (),                         ValidateNone())

/**
  <title PLIMMIN>
  <toctitle PLIMMIN>
   
  <table>
   <b>Type</b>            OdGePoint2d
   <b>Initial value</b>   (0.0, 0.0)
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        N/A
  </table>
   
  The PLIMMIN system variable stores the lower-left corner of the grid limits for Paper Space (current paper layout).
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getPLIMMIN()
   OdDbDatabase::setPLIMMIN()
  </table>  

<group TD_SysVars_P>
  */
//<dom_var_def>OdGePoint2d PLIMMIN;
VAR_DEF_H( OdGePoint2d,            PLIMMIN,      (0.0, 0.0),                     (0.0, 0.0),                       (),                         ValidateNone())

/**
  <title PLIMMAX>
  <toctitle PLIMMAX>
   
  <table>
   <b>Type</b>            OdGePoint2d
   <b>Initial value</b>   (12.0, 9.0)|(420.0, 297.0)
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        N/A
   </table>
   
   The PLIMMAX system variable stores the upper-right corner of the grid limits for paper space (current paper layout).
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getPLIMMAX()
   OdDbDatabase::setPLIMMAX()
  </table> 

<group TD_SysVars_P>
  */
//<dom_var_def>OdGePoint2d PLIMMAX;
VAR_DEF_H( OdGePoint2d,            PLIMMAX,      (12.0, 9.0),                    (420.0, 297.0),                   (),                         ValidateNone())

/**
  <title UCSNAME>
  <toctitle UCSNAME>
   
  <table>
   <b>Type</b>            OdDbObjectId
   <b>Initial value</b>   OdDbObjectId::kNull
   <b>Read-Only</b>       Yes
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  UCSNAME specifies the current UCS for the current viewport of the current space or model space.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   &nbsp; 
   OdDbDatabase::getUCSNAME()
   OdDbDatabase::setUCSNAME()
  </table> 

<group TD_SysVars_U>
  */
//<dom_var_def>OdDbObjectId UCSNAME;
VAR_DEF_H( ODDBHARDPOINTERID,      UCSNAME,      (OdDbObjectId::kNull),          (OdDbObjectId::kNull),            (),                         ValidateUcsRec())

/**
  <title PUCSNAME>
  <toctitle PUCSNAME>
   
  <table>
   <b>Type</b>            OdDbObjectId
   <b>Initial value</b>   OdDbObjectId::kNull
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        N/A
  </table>
   
  PUCSNAME specifies the current UCS for the current paper space.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getPUCSNAME()
   OdDbDatabase::setPUCSNAME()
  </table>  

<group TD_SysVars_P>
  */
//<dom_var_def>OdDbObjectId PUCSNAME;
VAR_DEF_H( ODDBHARDPOINTERID,      PUCSNAME,     (OdDbObjectId::kNull),          (OdDbObjectId::kNull),            (),                         ValidateUcsRec())

/**
  <title UNITMODE>
  <toctitle UNITMODE>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  UNITMODE specifies the unit display format for Architectural Units and Surveyor's Units.
   
  <table>
   <b>Value</b>   <b>Architectural Units</b>   <b>Surveyor's Units</b>
   0              12'-5 1/4"                   N 48d0'0" E
   1              12'5-1/4"                    N 48d0'0" E
  </table>
   
  When UNITMODE = 1, the displayed units are in a format suitable for input.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getUNITMODE()
   OdDbDatabase::setUNITMODE()
  </table>  

<group TD_SysVars_U>
  */
//<dom_var_def>OdInt16 UNITMODE;
VAR_DEF(   OdInt16,                UNITMODE,     (0),                            (0),                              (),                         ValidateRange(0,1))

/**
  <title VISRETAIN>
  <toctitle VISRETAIN>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   true
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  VISRETAIN specifies the precedence of layer settings for externally referenced databases.
   
  <table>
   <b>Value</b>   <b>Description</b>
   false          Settings are saved in the external reference.
   true           Settings are saved in the current database.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getVISRETAIN()
   OdDbDatabase::setVISRETAIN()
  </table> 

<group TD_SysVars_V>
  */
//<dom_var_def>bool VISRETAIN;
VAR_DEF(   ODTBOOL,                VISRETAIN,    (true),                         (true),                           (),                         ValidateBOOL())

/**
  <title PLINEGEN>
  <toctitle PLINEGEN>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   false
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  PLINEGEN controls the generation of linetypes for Polylines.
   
  <table>
   <b>Value</b>   <b>Description</b>
   false          Each segment starts and ends in a dash.
   true           Linetype generation is continuous, ignoring the vertices.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getPLINEGEN()
   OdDbDatabase::setPLINEGEN()
  </table>  

<group TD_SysVars_P>
  */
//<dom_var_def>bool PLINEGEN;
VAR_DEF(   ODTBOOL,                PLINEGEN,     (false),                        (false),                          (),                         ValidateBOOL())

/**
  <title PSLTSCALE>
  <toctitle PSLTSCALE>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   true
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  PSLTSCALE controls the generation of linetypes for Polylines in paper space.
   
  <table>
   <b>Value</b>   <b>Description</b>
   false          Dash lengths in model space are based on model space drawing units.
   true           Dash lengths in model space are based on paper space drawing units. When PSLTSCALE is true, linetypes in viewports will appear the same size as those in paper space.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getPSLTSCALE()
   OdDbDatabase::setPSLTSCALE()
  </table>  

<group TD_SysVars_P>
  */
//<dom_var_def>bool PSLTSCALE;
VAR_DEF(   ODTBOOL,                PSLTSCALE,    (true),                         (true),                           (),                         ValidateBOOL())

/**
  <title TREEDEPTH>
  <toctitle TREEDEPTH>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   3020
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R13+
  </table>
   
  TREEDEPTH specifies depth of the spatial index. Spatial indexing, along with layer indexing, is used to maximize
  performance of demand loading and entity selection.
   
  <table>
   <b>Value</b>     <b>Description</b>
   &lt;0            2D indexing. An integer in the form -MMMPP specifies the depths for model space (MMM) and paper space (PP).
   0                Disables spatial indexing.
   &gt;0            3D indexing. An integer in the form MMMPP specifies the depths for model space (MMM) and paper space (PP).
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getTREEDEPTH()
   OdDbDatabase::setTREEDEPTH()
  </table>  

<group TD_SysVars_T>
  */
//<dom_var_def>OdInt16 TREEDEPTH;
VAR_DEF(   OdInt16,                TREEDEPTH,    (3020),                         (3020),                           (),                         ValidateNone())

/**
  <title CMLSTYLE>
  <toctitle CMLSTYLE>
   
  <table>
   <b>Type</b>            OdDbObjectId
   <b>Initial value</b>   "Standard" multi-line style object ID
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        13+
  </table>
   
  CMLSTYLE specifies the default style for new multi-line entities.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getCMLSTYLE()
   OdDbDatabase::setCMLSTYLE()
  </table> 

<group TD_SysVars_C>
  */
//<dom_var_def>OdDbObjectId CMLSTYLE;
VAR_DEF(   ODTMLSTYLEID,           CMLSTYLE,     (OdDbObjectId::kNull),          (OdDbObjectId::kNull),            (),                         ValidateMLStyle())

/**
  <title CMLJUST>
  <toctitle CMLJUST>
   
   <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        13+
   </table>
   
   CMLJUST specifies the default justification for new multiline entities.
   
   <table>
   <b>Value</b>   <b>Description</b>
   0              Top
   1              Middle
   2              Bottom
   </table>
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getCMLJUST()
   OdDbDatabase::setCMLJUST()
   </table>

<group TD_SysVars_C>
   */
//<dom_var_def>OdInt16 CMLJUST;
VAR_DEF(   OdInt16,                CMLJUST,      (0),                            (0),                              (),                         ValidateRange(0,2))

/**
  <title CMLSCALE>
  <toctitle CMLSCALE>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   1.0 | 20.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        13+
  </table>
   
  CMLSCALE specifies the default width scale for new multiline entities.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getCMLSCALE()
   OdDbDatabase::setCMLSCALE()
  </table>   

<group TD_SysVars_C>
  */
//<dom_var_def>double CMLSCALE;
VAR_DEF(   ODTUNITLESS4,           CMLSCALE,     (1.0),                          (20.0),                           (),                         ValidateNone())

/**
  <title PROXYGRAPHICS>
  <toctitle PROXYGRAPHICS>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   1
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R14+
  </table>
   
  PROXYGRAPHICS controls the saving of proxy images with the database.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              No proxy images.
   1              Proxy image saved with database.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getPROXYGRAPHICS()
   OdDbDatabase::setPROXYGRAPHICS()
  </table>
   
  <b>Note: </b> PROXYGRAPHICS is called SAVEIMAGES in R13. 

<group TD_SysVars_P>
  */
//<dom_var_def>OdInt16 PROXYGRAPHICS;
VAR_DEF(   OdInt16,                PROXYGRAPHICS,(1),                            (1),                              (),                         ValidateRange(0,1))

/**
  <title MEASUREMENT>
  <toctitle MEASUREMENT>
   
  <table>
   <b>Type</b>            OdDb::MeasurementValue
   <b>Initial value</b>   OdDb::kEnglish | OdDb::kMetric
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R14+
  </table>
   
  MEASUREMENT controls the use of metric vs. imperial linetype and hatch pattern files.
   
  <table>
   <b>Name</b>      <b>Value</b>   <b>Registry Settings Used</b>   <b>Default Settings</b>
   OdDb::kEnglish   0              ANSILinetype &amp; ANSIHatch     acad.lin &amp; acad.pat
   OdDb::kMetric    1              ISOLinetype &amp; ISOHatch       acadiso.lin &amp; acadiso.pat
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getMEASUREMENT()
   OdDbDatabase::setMEASUREMENT()
  </table>

<group TD_SysVars_M>
  */
//<dom_var_def>OdDb::MeasurementValue MEASUREMENT;
VAR_DEF(   ODTDB_MEASUREMENTVALUE, MEASUREMENT,  (OdDb::kEnglish),               (OdDb::kMetric),                  (),                         ValidateRange(OdDb::kEnglish,OdDb::kMetric))

/**
  <title CELWEIGHT>
  <toctitle CELWEIGHT>
   
  <table>
   <b>Type</b>            OdDb::LineWeight
   <b>Initial value</b>   OdDb::kLnWtByLayer
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2000+
  </table>
   
  CELWEIGHT specifies the default lineweight for new entities.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getCELWEIGHT()
   OdDbDatabase::setCELWEIGHT()
  </table>

  <group TD_SysVars_C>
  */
//<dom_var_def>OdDb::LineWeight CELWEIGHT;
VAR_DEF(   ODTDB_LINEWEIGHT,       CELWEIGHT,    (OdDb::kLnWtByLayer),           (OdDb::kLnWtByLayer),             (),                         ValidateLineWeight())

/**
  <title ENDCAPS>
  <toctitle ENDCAPS>
   
  <table>
   <b>Type</b>            OdDb::EndCaps
   <b>Initial value</b>   OdDb::kEndCapNone
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        N/A
  </table>
   
  ENDCAPS specifies the lineweight entcaps for new entities.
   
  <table>
   <b>Name</b>           <b>Value</b>   <b>Description</b>
   OdDb::kEndCapNone     0              None
   OdDb::kEndCapRound    1              Round
   OdDb::kEndCapAngle    2              Angle
   OdDb::kEndCapSquare   3              Square
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getENDCAPS()
   OdDbDatabase::setENDCAPS()
  </table>

<group TD_SysVars_E>
  */
//<dom_var_def>OdDb::EndCaps ENDCAPS;
VAR_DEF_H( ODTDB_ENDCAPS,          ENDCAPS,      (OdDb::kEndCapNone),            (OdDb::kEndCapNone),              (),                         ValidateRange(OdDb::kEndCapNone, OdDb::kEndCapSquare))

/**
  <title JOINSTYLE>
  <toctitle JOINSTYLE>
   
  <table>
   <b>Type</b>            OdDb::JoinStyle
   <b>Initial value</b>   OdDb::kJnStylNone
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        N/A
  </table>
   
  JOINSTYLE specifies the lineweight join style for new entities.
   
  <table>
   <b>Name</b>          <b>Value</b>   <b>Description</b>
   OdDb::kJnStylNone    0              None
   OdDb::kJnStylRound   1              Round
   OdDb::kJnStylAngle   2              Angle
   OdDb::kJnStylFlat    3              Flat
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getJOINSTYLE()
   OdDbDatabase::setJOINSTYLE()
  </table>
  
<group TD_SysVars_J>
  */
//<dom_var_def>OdDb::JoinStyle JOINSTYLE;
VAR_DEF_H( ODTDB_JOINSTYLE,        JOINSTYLE,    (OdDb::kJnStylNone),            (OdDb::kJnStylNone),              (),                         ValidateRange(OdDb::kJnStylNone,OdDb::kJnStylFlat))

/**
  <title LWDISPLAY>
  <toctitle LWDISPLAY>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   false
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2000+
  </table>
   
  LWDISPLAY controls the display of lineweights.
   
  <table>
   <b>Value</b>   <b>Description</b>
   false          Not displayed
   true           Displayed
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getLWDISPLAY()
   OdDbDatabase::setLWDISPLAY()
  </table>  

<group TD_SysVars_L>
  */
//<dom_var_def>bool LWDISPLAY;
VAR_DEF(   bool,                   LWDISPLAY,    (false),                        (false),                          (),                         ValidateBOOL())

/**
  <title INSUNITS>
  <toctitle INSUNITS>
   
  <table>
   <b>Type</b>            OdDb::UnitsValue
   <b>Initial value</b>   OdDb::kUnitsInches | OdDb::kUnitsMillimeters
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2000+
  </table>
   
  INSUNITS specifies the drawing units used in the automatic scaling of blocks, Xrefs, and images 
  inserted into or attached to this database.
   
  <table>
   <b>Name</b>                <b>Value</b>   <b>Description</b>
   OdDb::kUnitsUndefined      0              Undefined
   OdDb::kUnitsInches         1              Inches
   OdDb::kUnitsFeet           2              Feet
   OdDb::kUnitsMiles          3              Miles
   OdDb::kUnitsMillimeters    4              Millimeters
   OdDb::kUnitsCentimeters    5              Centimeters
   OdDb::kUnitsMeters         6              Meters
   OdDb::kUnitsKilometers     7              Kilometers
   OdDb::kUnitsMicroinches    8              Microinches
   OdDb::kUnitsMils           9              Mils
   OdDb::kUnitsYards          10             Yards
   OdDb::kUnitsAngstroms      11             Angstroms
   OdDb::kUnitsNanometers     12             Nanometers
   OdDb::kUnitsMicrons        13             Microns
   OdDb::kUnitsDecimeters     14             Decimeters
   OdDb::kUnitsDekameters     15             Dekameters
   OdDb::kUnitsHectometers    16             Hectometers
   OdDb::kUnitsGigameters     17             Gigameters
   OdDb::kUnitsAstronomical   18             Astronomical
   OdDb::kUnitsLightYears     19             LightYears
   OdDb::kUnitsParsecs        20             Parsecs
   OdDb::kUnitsUSSurveyFeet   21             US Survey Feet
   OdDb::kUnitsUSSurveyInch   22             US Survey Inch
   OdDb::kUnitsUSSurveyYard   23             US Survey Yard
   OdDb::kUnitsUSSurveyMile   24             US Survey Mile
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getINSUNITS()
   OdDbDatabase::setINSUNITS()
  </table>
  
  <group TD_SysVars_I>
  */
//<dom_var_def>OdDb::UnitsValue INSUNITS;
VAR_DEF(   ODTDB_UNITSVALUE,       INSUNITS,     (OdDb::kUnitsInches),           (OdDb::kUnitsMillimeters),        (),                         ValidateRange(OdDb::kUnitsUndefined,OdDb::kUnitsMax))

/**
  <title TSTACKALIGN>
  <toctitle TSTACKALIGN>
   
  <table>
   <b>Type</b>            OdUInt16
   <b>Initial value</b>   1
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2000+
  </table>
   
  TSTACKALIGN controls the alignment of stacked text.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Bottom
   1              Center
   2              Top
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getTSTACKALIGN()
   OdDbDatabase::setTSTACKALIGN()
  </table>  

<group TD_SysVars_T>
  */
//<dom_var_def>OdUInt16 TSTACKALIGN;
VAR_DEF(   OdUInt16,               TSTACKALIGN,  (1),                            (1),                              (),                         ValidateRange(0,2))

/**
  <title TSTACKSIZE>
  <toctitle TSTACKSIZE>
   
  <table>
   <b>Type</b>            OdUInt16
   <b>Initial value</b>   70
   <b>Range</b>           25 ... 125
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2000+
  </table>
   
  TSTACKSIZE specifies the size of stacked text as a percentage of the current text height.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getTSTACKSIZE()
   OdDbDatabase::setTSTACKSIZE()
  </table> 

<group TD_SysVars_T>
  */
//<dom_var_def>OdUInt16 TSTACKSIZE;
VAR_DEF(   OdUInt16,               TSTACKSIZE,   (70),                           (70),                             (),                         ValidateRange(25,125))

/**
  <title HYPERLINKBASE>
  <toctitle HYPERLINKBASE>
   
  <table>
   <b>Type</b>            OdString
   <b>Initial value</b>   ""
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2000+
  </table>
   
  HYPERLINKBASE specifies the base path for relative hyperlinks in this database. If HYPERLINKBASE 
  is an empty string, the database path is used.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getHYPERLINKBASE()
   OdDbDatabase::setHYPERLINKBASE()
  </table>  

<group TD_SysVars_H>
  */
//<dom_var_def>OdString HYPERLINKBASE;
VAR_DEF(   OdString,               HYPERLINKBASE,(OD_T("")),                           (OD_T("")),                             (),                         ValidateNone())

/**
  <title STYLESHEET>
  <toctitle STYLESHEET>
   
  <table>
   <b>Type</b>            OdString
   <b>Initial value</b>   ""
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        N/A
  </table>
   
  STYLESHEET specifies the name of the style sheet for the active layout.
   
  <table>
   OdDbDatabase::getSTYLESHEET()
   OdDbDatabase::setSTYLESHEET()
  </table>  

<group TD_SysVars_S>
  */
//<dom_var_def>OdString STYLESHEET;
VAR_DEF_H( OdString,               STYLESHEET,   (OD_T("")),                           (OD_T("")),                             (),                         ValidateNone())

/**
  <title XEDIT>
  <toctitle XEDIT>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   true
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2000+
  </table>
   
  XEDIT controls if this database, when an Xref, can be edited in-place.
   
  <table>
   <b>Value</b>   <b>Description</b>
   false          Not edited in-place
   true           Edited in-place
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getXEDIT()
   OdDbDatabase::setXEDIT()
  </table>      

<group TD_SysVars_X>
  */
//<dom_var_def>bool XEDIT;
VAR_DEF(   ODTBOOL,                XEDIT,        (true),                         (true),                           (),                         ValidateBOOL())

/**
  <title CEPSNTYPE>
  <toctitle CEPSNTYPE>
   
  <table>
   <b>Type</b>            OdDb::PlotStyleNameType
   <b>Initial value</b>   OdDb::kPlotStyleNameByLayer
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2000+
  </table>
   
  CEPSNTYPE specifies the plot style name type for new entities.
   
   <table>
   <b>Name</b>                         <b>Value</b>   <b>Description</b>
   OdDb::kPlotStyleNameByLayer         0              By Layer
   OdDb::kPlotStyleNameByBlock         1              By Block
   OdDb::kPlotStyleNameIsDictDefault   2              By Dictionary Default
   OdDb::kPlotStyleNameById            3              By Id
   </table>
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getCEPSNTYPE()
   OdDbDatabase::setCEPSNTYPE()
   </table> 

<group TD_SysVars_C>
   */
//<dom_var_def>OdDb::PlotStyleNameType CEPSNTYPE;
VAR_DEF_H( ODTDB_PLOTSTYLENAMETYPE, CEPSNTYPE,    (OdDb::kPlotStyleNameByLayer),  (OdDb::kPlotStyleNameByLayer),    (),                         ValidateNone())

/**
  <title CEPSNID>
  <toctitle CEPSNID>
   
  <table>
   <b>Type</b>            OdDbHardPointerId
   <b>Initial value</b>   OdDbObjectId::kNull
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2000+
  </table>
   
  CEPSNID specifies the plot style for new entities when CEPSNTYPE == OdDb::kPlotStyleNameById.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getCEPSNID()
   OdDbDatabase::setCEPSNID()
  </table>  

<group TD_SysVars_C>
  */
//<dom_var_def>OdDbHardPointerId CEPSNID;
VAR_DEF_H( ODDBHARDPOINTERID,      CEPSNID,      (OdDbObjectId::kNull),          (OdDbObjectId::kNull),            (),                         ValidatePSName())

/**
  <title PSTYLEMODE>
  <toctitle PSTYLEMODE>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   true
   <b>Read-Only</b>       Yes
   <b>Saved In</b>        Database
   <b>Versions</b>        2000+
  </table>
   
  PSTYLEMODE specifies if this database is in a color-dependent or named plot style mode.
   
  <table>
   <b>Value</b>   <b>Description</b>
   false          Named plot style mode
   true           Color-dependent plot style mode
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   &nbsp; 
   OdDbDatabase::getPSTYLEMODE()
  </table>  

<group TD_SysVars_P>
  */
//<dom_var_def>bool PSTYLEMODE;
RO_VAR_DEF(ODTBOOL,                PSTYLEMODE,   (true),                         (true),                           (),                         ValidateBOOL())

/**
  <title FINGERPRINTGUID>
  <toctitle FINGERPRINTGUID>
   
  <table>
   <b>Type</b>            OdString
   <b>Initial value</b>   odInitFINGERPRINTGUID()
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        N/A
  </table>
   
  FINGERPRINTGUID uniquely identifies a database. FINGERPRINTGUID set when the database is created.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getFINGERPRINTGUID()
   OdDbDatabase::setFINGERPRINTGUID()
  </table>   

<group TD_SysVars_F>
  */
//<dom_var_def>OdString FINGERPRINTGUID;
VAR_DEF_H( OdString,               FINGERPRINTGUID,(odInitFINGERPRINTGUID()),    (odInitFINGERPRINTGUID()),        (),                         ValidateNone())

/**
  <title VERSIONGUID>
  <toctitle VERSIONGUID>
   
  <table>
   <b>Type</b>            OdString
   <b>Initial value</b>   odInitVERSIONGUID()
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        N/A
  </table>
   
  VERSIONGUID uniquely identifies a specific version of a database. VERSIONGUID is updated each time 
  the database is saved.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getVERSIONGUID()
   OdDbDatabase::setVERSIONGUID()
  </table>     

<group TD_SysVars_V>
  */
//<dom_var_def>OdString VERSIONGUID;
VAR_DEF_H( OdString,               VERSIONGUID,  (odInitVERSIONGUID()),          (odInitVERSIONGUID()),            (),                         ValidateNone())

/**
  <title EXTNAMES>
  <toctitle EXTNAMES>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   true
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2000+
  </table>
   
  EXTNAMES specifies if symbol tables follow the R14- or R2000+ naming conventions.
   
   <table>
   <b>Value</b>   <b>Version</b>   <b>Max Len</b>   <b>Character Set</b>
   false          R14-             31               [A-Z] [0-9] $ _ -
   true           R2000+           255              All except \\ / : * ? " < > |
   </table>
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getEXTNAMES()
   OdDbDatabase::setEXTNAMES()
   </table>   

<group TD_SysVars_E>
   */
//<dom_var_def>bool EXTNAMES;
VAR_DEF(   ODTBOOL,                EXTNAMES,     (true),                         (true),                           (),                         ValidateBOOL())

/**
  <title PSVPSCALE>
  <toctitle PSVPSCALE>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   0.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2000+
  </table>
   
  PSVPSCALE specifies the viewport scale (PS/MS) for new viewports. A scale of 0.0 specifies Scaled to Fit.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getPSVPSCALE()
   OdDbDatabase::setPSVPSCALE()
  </table> 

<group TD_SysVars_P>
  */
//<dom_var_def>double PSVPSCALE;
VAR_DEF(   ODTUNITLESS8,           PSVPSCALE,    (0.0),                          (0.0),                            (),                         ValidateEqGr(0.))

/**
  <title OLESTARTUP>
  <toctitle OLESTARTUP>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   false
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2000+
  </table>
   
  OLESTARTUP specifies if the OLE source applications load when plotting. This may improve the plot quality.
   
  <table>
   <b>Value</b>   <b>Description</b>
   false          Not loaded
   true           Loaded
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getOLESTARTUP()
   OdDbDatabase::setOLESTARTUP()
  </table>  

<group TD_SysVars_O>
  */
//<dom_var_def>bool OLESTARTUP;
VAR_DEF(   ODTBOOL,                OLESTARTUP,   (false),                        (false),                          (),                         ValidateBOOL())

/**
  <title PELLIPSE>
  <toctitle PELLIPSE>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   false
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R13+
  </table>
   
  PELLIPSE specifies if polylines or ellipses are to be generated by the ELLIPSE command.
   
  <table>
   <b>Value</b>   <b>Description</b>
   false          Ellipses
   true           Polylines
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getPELLIPSE()
   OdDbDatabase::setPELLIPSE()
  </table>  

<group TD_SysVars_P>
  */
//<dom_var_def>bool PELLIPSE;
VAR_DEF(   ODTBOOL,                PELLIPSE,     (false),                        (false),                          (),                         ValidateBOOL())

/**
  <title ISOLINES>
  <toctitle ISOLINES>
   
  <table>
   <b>Type</b>            OdUInt16
   <b>Initial value</b>   4
   <b>Range</b>           0 ... 2047
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R13+
  </table>
   
  ISOLINES specifies the number of isolines displayed per surface on 3D solids.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getISOLINES()
   OdDbDatabase::setISOLINES()
  </table>    

<group TD_SysVars_I>
  */
//<dom_var_def>OdUInt16 ISOLINES;
VAR_DEF(   OdUInt16,               ISOLINES,     (4),                            (4),                              (),                         ValidateRange(0,2047))

/**
  <title TEXTQLTY>
  <toctitle TEXTQLTY>
   
  <table>
   <b>Type</b>            OdUInt16
   <b>Initial value</b>   50
   <b>Range</b>           0 ... 100
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R13+
  </table>
   
  TEXTQLTY specifies the rendering quality of TrueType fonts while plotting.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getTEXTQLTY()
   OdDbDatabase::setTEXTQLTY()
  </table> 

<group TD_SysVars_T>
  */
//<dom_var_def>OdUInt16 TEXTQLTY;
VAR_DEF(   OdUInt16,               TEXTQLTY,     (50),                           (50),                             (),                         ValidateRange(0,100))

/**
  <title FACETRES>
  <toctitle FACETRES>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   0.5
   <b>Range</b>           0.01 ... 10.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R13+
  </table>
   
  FACETRES specifies the quality of hidden, shaded, and rendered objects.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getFACETRES()
   OdDbDatabase::setFACETRES()
  </table>    

<group TD_SysVars_F>
  */
//<dom_var_def>double FACETRES;
VAR_DEF(   ODTUNITLESS4,           FACETRES,     (.5),                           (.5),                             (),                         ValidateRange(0.01,10.0))

/**
  <title UCSORG>
  <toctitle UCSORG>
   
  <table>
   <b>Type</b>            OdGePoint3d
   <b>Initial value</b>   (0.0, 0.0, 0.0)
   <b>Read-Only</b>       Yes
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  UCSORG specifies the WCS origin of the current UCS for the current viewport in the current space or model space.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   &nbsp; 
   OdDbDatabase::getUCSORG()
  </table>
  
  \sa
  <link odDbSetUCSORG(), odDbSetUCSORG()>
  
  <group TD_SysVars_U>
  */
//<dom_var_def>OdGePoint3d UCSORG;
RO_VAR_DEF_H(OdGePoint3d,          UCSORG,       (0.0, 0.0, 0.0),                (0.0, 0.0, 0.0),                  (),                         ValidateNone())

// READ ONLY IN R2000

/**
  <title UCSXDIR>
  <toctitle UCSXDIR>
   
  <table>
   <b>Type</b>            OdGeVector3d
   <b>Initial value</b>   (1.0, 0.0, 0.0)
   <b>Read-Only</b>       Yes
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  UCSXDIR specifies the WCS x-direction of the current UCS for the current viewport in the current space or model space.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   &nbsp; 
   OdDbDatabase::getUCSXDIR()
  </table>
  
  \sa
   <link odDbSetUCSXDIR(), odDbSetUCSXDIR()>

  <group TD_SysVars_U>
*/
//<dom_var_def>OdGeVector3d UCSXDIR;
RO_VAR_DEF_H(OdGeVector3d,         UCSXDIR,      (1.0, 0.0, 0.0),                (1.0, 0.0, 0.0),                  (),                         ValidateNone())

// READ ONLY IN R2000

/**
  <title UCSYDIR>
  <toctitle UCSYDIR>
   
  <table>
   <b>Type</b>            OdGeVector3d
   <b>Initial value</b>   (0.0, 1.0, 0.0)
   <b>Read-Only</b>       Yes
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  UCSYDIR specifies the WCS y-direction of the current UCS for the current viewport in the current 
  space or model space.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   &nbsp; 
   OdDbDatabase::getUCSYDIR()
  </table>
  
  \sa
  <link odDbSetUCSYDIR(), odDbSetUCSYDIR()>

  <group TD_SysVars_U>
*/
//<dom_var_def>OdGeVector3d UCSYDIR;
RO_VAR_DEF_H(OdGeVector3d,         UCSYDIR,      (0.0, 1.0, 0.0),                (0.0, 1.0, 0.0),                  (),                         ValidateNone())

// READ ONLY IN R2000

/**
  <title PUCSBASE>
  <toctitle PUCSBASE>
   
  <table>
   <b>Type</b>            OdDbObjectId
   <b>Initial value</b>   OdDbObjectId::kNull
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2000+
  </table>
   
  PUCSBASE specifies the UCS that defines the orthographic UCS orientation for the current paper space.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getPUCSBASE()
   OdDbDatabase::setPUCSBASE()
  </table>

   <group TD_SysVars_P> 
  */
//<dom_var_def>OdDbObjectId PUCSBASE;
VAR_DEF(   ODTUCSID,               PUCSBASE,     (OdDbObjectId::kNull),          (OdDbObjectId::kNull),            (),                         ValidateUcsRec())

/**
  <title PUCSORG>
  <toctitle PUCSORG>
   
  <table>
   <b>Type</b>            OdGePoint3d
   <b>Initial value</b>   (0.0, 0.0, 0.0)
   <b>Read-Only</b>       Yes
   <b>Saved In</b>        Database
   <b>Versions</b>        N/A
  </table>
   
  PUCSORG specifies the WCS origin of the current UCS of the current paper space.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getPUCSORG()
  </table>
   
  \sa
  <link odDbSetPUCSORG(), odDbSetPUCSORG()>

  <group TD_SysVars_P>
*/
//<dom_var_def>OdGePoint3d PUCSORG;
RO_VAR_DEF_H(OdGePoint3d,          PUCSORG,      (0.0, 0.0, 0.0),                (0.0, 0.0, 0.0),                  (),                         ValidateNone())

/**
  <title PUCSXDIR>
  <toctitle PUCSXDIR>
   
  <table>
   <b>Type</b>            OdGeVector3d
   <b>Initial value</b>   (1.0, 0.0, 0.0)
   <b>Read-Only</b>       Yes
   <b>Saved In</b>        Database
   <b>Versions</b>        N/A
  </table>
   
  PUCSXDIR specifies the WCS x-direction of the current UCS of the current paper space.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getPUCSXDIR()
  </table>
   
  \sa
  <link odDbSetPUCSXDIR(), odDbSetPUCSXDIR()>

  <group TD_SysVars_P>  
  */
//<dom_var_def>OdGeVector3d PUCSXDIR;
RO_VAR_DEF_H(OdGeVector3d,         PUCSXDIR,     (1.0, 0.0, 0.0),                (1.0, 0.0, 0.0),                  (),                         ValidateNone())

/**
  <title PUCSYDIR>
  <toctitle PUCSYDIR>
   
  <table>
   <b>Type</b>            OdGeVector3d
   <b>Initial value</b>   (0.0, 1.0, 0.0)
   <b>Read-Only</b>       Yes
   <b>Saved In</b>        Database
   <b>Versions</b>        N/A
  </table>
  
  PUCSYDIR specifies the WCS y-direction of the current UCS of the current paper space.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getPUCSYDIR()
  </table>
   
  \sa
  <link odDbSetPUCSYDIR(), odDbSetPUCSYDIR()>
  
  <group TD_SysVars_P>
  */
//<dom_var_def>OdGeVector3d PUCSYDIR;
RO_VAR_DEF_H(OdGeVector3d,         PUCSYDIR,     (0.0, 1.0, 0.0),                (0.0, 1.0, 0.0),                  (),                         ValidateNone())

/**
  <title PUCSORTHOVIEW>
  <toctitle PUCSORTHOVIEW>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        N/A
  </table>
   
  PUCSORTHOVIEW specifies the orthographic UCS with respect to the base UCS for the current paper space.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Non-orthographc
   1              Top
   2              Bottom
   3              Front
   4              Back
   5              Left
   6              Right
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getPUCSORTHOVIEW()
   OdDbDatabase::setPUCSORTHOVIEW()
  </table>   

  <group TD_SysVars_P>
  */
//<dom_var_def>OdInt16 PUCSORTHOVIEW;
VAR_DEF_H( OdInt16,                PUCSORTHOVIEW,(0),                            (0),                              (),                         ValidateRange(0,6))

/**
  <title PUCSORGTOP>
  <toctitle PUCSORGTOP>
   
  <table>
   <b>Type</b>            OdGePoint3d
   <b>Initial value</b>   (0.0, 0.0, 0.0)
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        N/A
  </table>
   
  PUCSORGTOP specifies the WCS origin of the top-orthographic UCS with respect to the base UCS for 
  the current paper space.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getPUCSORGTOP()
   OdDbDatabase::setPUCSORGTOP()
  </table>

  <group TD_SysVars_P>
  */
//<dom_var_def>OdGePoint3d PUCSORGTOP;
VAR_DEF_H( OdGePoint3d,            PUCSORGTOP,   (0.0, 0.0, 0.0),                (0.0, 0.0, 0.0),                  (),                         ValidateNone())

/**
  <title PUCSORGBOTTOM>
  <toctitle PUCSORGBOTTOM>
   
  <table>
   <b>Type</b>            OdGePoint3d
   <b>Initial value</b>   (0.0, 0.0, 0.0)
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        N/A
  </table>
   
  PUCSORGBOTTOM specifies the WCS origin of the bottom-orthographic UCS with respect to the base UCS for the
  current paper space.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getPUCSORGBOTTOM()
   OdDbDatabase::setPUCSORGBOTTOM()
  </table> 

  <group TD_SysVars_P>
  */
//<dom_var_def>OdGePoint3d PUCSORGBOTTOM;
VAR_DEF_H( OdGePoint3d,            PUCSORGBOTTOM,(0.0, 0.0, 0.0),                (0.0, 0.0, 0.0),                  (),                         ValidateNone())

/** 
  <title PUCSORGLEFT>
  <toctitle PUCSORGLEFT>
   
  <table>
   <b>Type</b>            OdGePoint3d
   <b>Initial value</b>   (0.0, 0.0, 0.0)
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        N/A
  </table>
   
  PUCSORGLEFT specifies the WCS origin of the left-orthographic UCS with respect to the base 
  UCS for the current paper space.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getPUCSORGLEFT()
   OdDbDatabase::setPUCSORGLEFT()
  </table>    

  <group TD_SysVars_P>
  */
//<dom_var_def>OdGePoint3d PUCSORGLEFT;
VAR_DEF_H( OdGePoint3d,            PUCSORGLEFT,  (0.0, 0.0, 0.0),                (0.0, 0.0, 0.0),                  (),                         ValidateNone())

/**
  <title PUCSORGRIGHT>
  <toctitle PUCSORGRIGHT>
   
  <table>
   <b>Type</b>            OdGePoint3d
   <b>Initial value</b>   (0.0, 0.0, 0.0)
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        N/A
  </table>
   
  PUCSORGRIGHT specifies the WCS origin of the right-orthographic UCS with respect to the base UCS for the
  current paper space.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getPUCSORGRIGHT()
   OdDbDatabase::setPUCSORGRIGHT()
  </table> 

  <group TD_SysVars_P>
  */
//<dom_var_def>OdGePoint3d PUCSORGRIGHT;
VAR_DEF_H( OdGePoint3d,            PUCSORGRIGHT, (0.0, 0.0, 0.0),                (0.0, 0.0, 0.0),                  (),                         ValidateNone())

/**
  <title PUCSORGFRONT>
  <toctitle PUCSORGFRONT>
   
  <table>
   <b>Type</b>            OdGePoint3d
   <b>Initial value</b>   (0.0, 0.0, 0.0)
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        N/A
  </table>
   
  PUCSORGFRONT specifies the WCS origin of the front-orthographic UCS with respect to the base UCS for the
  current paper space.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getPUCSORGFRONT()
   OdDbDatabase::setPUCSORGFRONT()
  </table> 

  <group TD_SysVars_P>
  */
//<dom_var_def>OdGePoint3d PUCSORGFRONT;
VAR_DEF_H( OdGePoint3d,            PUCSORGFRONT, (0.0, 0.0, 0.0),                (0.0, 0.0, 0.0),                  (),                         ValidateNone())

/**
  <title PUCSORGBACK>
  <toctitle PUCSORGBACK>
   
  <table>
   <b>Type</b>            OdGePoint3d
   <b>Initial value</b>   (0.0, 0.0, 0.0)
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        N/A
  </table>
   
  PUCSORGBACK specifies the WCS origin of the back-orthographic UCS with respect to the base UCS for the 
  current paper space.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getPUCSORGBACK()
   OdDbDatabase::setPUCSORGBACK()
  </table>    

  <group TD_SysVars_P>
  */
//<dom_var_def>OdGePoint3d PUCSORGBACK;
VAR_DEF_H( OdGePoint3d,            PUCSORGBACK,  (0.0, 0.0, 0.0),                (0.0, 0.0, 0.0),                  (),                         ValidateNone())

/**
  <title UCSBASE>
  <toctitle UCSBASE>
   
  <table>
   <b>Type</b>            OdDbHardPointerId
   <b>Initial value</b>   OdDbObjectId::kNull
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2000+
  </table>
   
  UCSBASE specifies the UCS that defines the orthographic UCS orientation for model space.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
    
   OdDbDatabase::getUCSBASE()
   OdDbDatabase::setUCSBASE()
  </table>

    <group TD_SysVars_U>
  */
//<dom_var_def>OdDbHardPointerId UCSBASE;
VAR_DEF(   ODTUCSID,               UCSBASE,      (OdDbObjectId::kNull),          (OdDbObjectId::kNull),            (),                         ValidateUcsRec())

/**
  <title UCSORTHOVIEW>
  <toctitle UCSORTHOVIEW>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        N/A
  </table>
   
  UCSORTHOVIEW specifies the model space orthographic UCS with respect to the base UCS.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Non-orthographc
   1              Top
   2              Bottom
   3              Front
   4              Back
   5              Left
   6              Right
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getUCSORTHOVIEW()
   OdDbDatabase::setUCSORTHOVIEW()
  </table>  

  <group TD_SysVars_U>
  */
//<dom_var_def>OdInt16 UCSORTHOVIEW;
VAR_DEF_H( OdInt16,                UCSORTHOVIEW, (0),                            (0),                              (),                         ValidateRange(0,6))

/**
  <title UCSORGTOP>
  <toctitle UCSORGTOP>
   
  <table>
   <b>Type</b>            OdGePoint3d
   <b>Initial value</b>   (0.0, 0.0, 0.0)
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        N/A
  </table>
   
  UCSORGTOP specifies the WCS origin of the model space top-orthographic UCS with respect to the base model space
  UCS.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getUCSORGTOP()
   OdDbDatabase::setUCSORGTOP()
  </table> 

  <group TD_SysVars_U>
  */
//<dom_var_def>OdGePoint3d UCSORGTOP;
VAR_DEF_H( OdGePoint3d,            UCSORGTOP,    (0.0, 0.0, 0.0),                (0.0, 0.0, 0.0),                  (),                         ValidateNone())

/**
  <title UCSORGBOTTOM>
  <toctitle UCSORGBOTTOM>
   
  <table>
   <b>Type</b>            OdGePoint3d
   <b>Initial value</b>   (0.0, 0.0, 0.0)
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        N/A
  </table>
   
  UCSORGBOTTOM specifies the WCS origin of the model space bottom-orthographic UCS with respect 
  to the base model space UCS.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getUCSORGBOTTOM()
   OdDbDatabase::setUCSORGBOTTOM()
  </table>  

  <group TD_SysVars_U>
  */
//<dom_var_def>OdGePoint3d UCSORGBOTTOM;
VAR_DEF_H( OdGePoint3d,            UCSORGBOTTOM, (0.0, 0.0, 0.0),                (0.0, 0.0, 0.0),                  (),                         ValidateNone())

/**
  <title UCSORGLEFT>
  <toctitle UCSORGLEFT>
   
  <table>
   <b>Type</b>            OdGePoint3d
   <b>Initial value</b>   (0.0, 0.0, 0.0)
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        N/A
  </table>
   
  UCSORGLEFT specifies the WCS origin of the model space left-orthographic UCS with respect to the base 
  model space UCS.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getUCSORGLEFT()
   OdDbDatabase::setUCSORGLEFT()
  </table>  

  <group TD_SysVars_U>
  */
//<dom_var_def>OdGePoint3d UCSORGLEFT;
VAR_DEF_H( OdGePoint3d,            UCSORGLEFT,   (0.0, 0.0, 0.0),                (0.0, 0.0, 0.0),                  (),                         ValidateNone())

/**
  <title UCSORGRIGHT>
  <toctitle UCSORGRIGHT>
   
  <table>
   <b>Type</b>            OdGePoint3d
   <b>Initial value</b>   (0.0, 0.0, 0.0)
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        N/A
  </table>
   
  UCSORGRIGHT specifies the WCS origin of the model space right-orthographic UCS with respect to the 
  base model space UCS.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getUCSORGRIGHT()
   OdDbDatabase::setUCSORGRIGHT()
  </table>     

  <group TD_SysVars_U>
  */
//<dom_var_def>OdGePoint3d UCSORGRIGHT;
VAR_DEF_H( OdGePoint3d,            UCSORGRIGHT,  (0.0, 0.0, 0.0),                (0.0, 0.0, 0.0),                  (),                         ValidateNone())

/**
  <title UCSORGFRONT>
  <toctitle UCSORGFRONT>
   
  <table>
   <b>Type</b>            OdGePoint3d
   <b>Initial value</b>   (0.0, 0.0, 0.0)
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        N/A
  </table>
   
  UCSORGFRONT specifies the WCS origin of the model space front-orthographic UCS with respect to the base 
  model space UCS.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getUCSORGFRONT()
   OdDbDatabase::setUCSORGFRONT()
  </table>  

  <group TD_SysVars_U>
  */
//<dom_var_def>OdGePoint3d UCSORGFRONT;
VAR_DEF_H( OdGePoint3d,            UCSORGFRONT,  (0.0, 0.0, 0.0),                (0.0, 0.0, 0.0),                  (),                         ValidateNone())

/**
  <title UCSORGBACK>
  <toctitle UCSORGBACK>
   
  <table>
   <b>Type</b>            OdGePoint3d
   <b>Initial value</b>   (0.0, 0.0, 0.0)
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        N/A
  </table>
   
  UCSORGBACK specifies the WCS origin of the model space back-orthographic UCS with respect to the base 
  model space UCS.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getUCSORGBACK()
   OdDbDatabase::setUCSORGBACK()
  </table>   

  <group TD_SysVars_U>
  */
//<dom_var_def>OdGePoint3d UCSORGBACK;
VAR_DEF_H( OdGePoint3d,            UCSORGBACK,   (0.0, 0.0, 0.0),                (0.0, 0.0, 0.0),                  (),                         ValidateNone())

/**
  <title DWGCODEPAGE>
  <toctitle DWGCODEPAGE>
   
  <table>
   <b>Type</b>            OdCodePageId
   <b>Initial value</b>   CP_ANSI_1252
   <b>Read-Only</b>       Yes
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  The DWGCODEPAGE variable specifies the database system's code page when the database was created. 
  The value can be:
   
  <table>
   <b>Name</b>    <b>Value</b>   <b>Description</b>
   CP_UNDEFINED   0              undefined
   CP_ASCII       1              ascii
   CP_8859_1      2              iso8859-1
   CP_8859_2      3              iso8859-2
   CP_8859_3      4              iso8859-3
   CP_8859_4      5              iso8859-4
   CP_8859_5      6              iso8859-5
   CP_8859_6      7              iso8859-6
   CP_8859_7      8              iso8859-7
   CP_8859_8      9              iso8859-8
   CP_8859_9      10             iso8859-9
   CP_DOS437      11             dos437
   CP_DOS850      12             dos850
   CP_DOS852      13             dos852
   CP_DOS855      14             dos855
   CP_DOS857      15             dos857
   CP_DOS860      16             dos860
   CP_DOS861      17             dos861
   CP_DOS863      18             dos863
   CP_DOS864      19             dos864
   CP_DOS865      20             dos865
   CP_DOS869      21             dos869
   CP_DOS932      22             dos932
   CP_MACINTOSH   23             mac-roman
   CP_BIG5        24             big5
   CP_KSC5601     25             ksc5601
   CP_JOHAB       26             johab
   CP_DOS866      27             dos866
   CP_ANSI_1250   28             ansi 1250
   CP_ANSI_1251   29             ansi 1251
   CP_ANSI_1252   30             ansi 1252
   CP_GB2312      31             gb2312
   CP_ANSI_1253   32             ansi 1253
   CP_ANSI_1254   33             ansi 1254
   CP_ANSI_1255   34             ansi 1255
   CP_ANSI_1256   35             ansi 1256
   CP_ANSI_1257   36             ansi 1257
   CP_ANSI_874    37             ansi 874
   CP_ANSI_932    38             ansi 932
   CP_ANSI_936    39             ansi 936
   CP_ANSI_949    40             ansi 949
   CP_ANSI_950    41             ansi 950
   CP_ANSI_1361   42             ansi 1361
   CP_ANSI_1200   43             ansi 1200
   CP_ANSI_1258   44             ansi 1258
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   &nbsp; 
   OdDbDatabase::getDWGCODEPAGE()
  </table>
   
   For example, to get the value:
   <code>
   OdRusBufPtr pRb = pDb-\>getSysVar(L"DWGCODEPAGE");
   OdInt16 page = pRb-\>getInt16();
   odPrintConsoleString(L"nDatabase system code page = %d", page);
   // or //
   odPrintConsoleString(L"nDatabase system code page = %d", (int)pDb-\>getDWGCODEPAGE();
   </code>
   
  \sa
  <link odDbSetDWGCODEPAGE(), The odDbSetDWGCODEPAGE() global function.>

  <group TD_SysVars_D>
  */
//<dom_var_def>OdCodePageId DWGCODEPAGE;
RO_VAR_DEF(OdCodePageId,           DWGCODEPAGE,  (CP_ANSI_1252),                 (CP_ANSI_1252),                   (),                         ValidateNone())

// New in Red Deer

/**
  <title CTABLESTYLE>
  <toctitle CTABLESTYLE>
   
  <table>
   <b>Type</b>            OdDbObjectId
   <b>Initial value</b>   "Standard" table style object ID
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2005+
  </table>
   
  CTABLESTYLE specifies the default style for new table entities.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::tablestyle()
   OdDbDatabase::setTablestyle()
  </table>  

  <group TD_SysVars_C>
  */
//<dom_var_def>OdDbObjectId CTABLESTYLE;
DICR21VAR( ODTTABLESTYLEID,        CTABLESTYLE,         (OdDbObjectId::kNull),    (OdDbObjectId::kNull),            (),                         ValidateTableStyle())

// New in AC2008
//DOM-IGNORE-BEGIN
/*** VARDOCS:
<properties>
<remarks>
"\n    CMLEADERSTYLE specifies the active multileader style, which will be assigned to the new multileaders in setDatabaseDefaults()"
</remarks>
</properties>
*/
//DOM-IGNORE-END

/**
  <title CMLEADERSTYLE>
   <toctitle CMLEADERSTYLE>
   
   <table>
   <b>Type</b>            OdDbObjectId
   <b>Initial value</b>   "Standard" multileader style object ID
   <b>Read-Only</b>       No
   <b>Saved In</b>        Drawing
   <b>Versions</b>        2008+
   </table>
   
   CMLEADERSTYLE specifies the current multileader style used by the drawing.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getCMLEADERSTYLE()
   OdDbDatabase::setCMLEADERSTYLE()
  </table> 

  <group TD_SysVars_C>
  */
//<dom_var_def>OdDbObjectId CMLEADERSTYLE;
MLEADERSTYLE_VAR_DEF( ODTMLEADERSTYLEID, CMLEADERSTYLE, (OdDbObjectId::kNull),    (OdDbObjectId::kNull),            (),                         ValidateMLeaderStyle())

//DOM-IGNORE-BEGIN
/*** VARDOCS:
<properties>
<remarks>
"\n    CANNOSCALE specifies the current annotation scale."
"\n    Unlike OdDbDatabase::cannoscale(), CANNOSCALE depends on the active viewport if the current layout is not model space."
</remarks>
</properties>
*/
//DOM-IGNORE-END

/**
  <title CANNOSCALE>
  <toctitle CANNOSCALE>
   
   <table>
   <b>Type</b>            OdDbAnnotationScalePtr
   <b>Initial value</b>   ID of the 1:1 annotation scale
   <b>Read-Only</b>       No
   <b>Saved In</b>        Drawing
   <b>Versions</b>        2008+
   </table>
   
   CANNOSCALE specifies the current annotation scale used by the current viewport.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getCANNOSCALE()
   OdDbDatabase::setCANNOSCALE()
  </table> 

  <group TD_SysVars_C>
  */
//<dom_var_def>OdDbAnnotationScalePtr CANNOSCALE;
ANNO_VAR_DEF( ODANNOSCALE,         CANNOSCALE,          (ODANNOSCALE()),          (ODANNOSCALE()),                  (),                         ValidateNone())

/**
  <title ANNOALLVISIBLE>
  <toctitle ANNOALLVISIBLE>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   true
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2008+
  </table>
   
  The ANNOALLVISIBLE variable is used to hide or display annotative objects that do not support the 
  current annotation scale. The value of ANNOALLVISIBLE variable is stored separately for model 
  space and paper space viewports.
   
  <table>
   <b>Value</b>   <b>Description</b>
   false          Displays only the annotative objects that support the current annotation scale.
   true           Displays all annotative objects.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getANNOALLVISIBLE()
   OdDbDatabase::setANNOALLVISIBLE()
  </table>
   
  <b>Note:</b> If the value of ANNOALLVISIBLE is set to true, annotative objects that support several 
  scales display only one scale view.

  <group TD_SysVars_A>
  */
//<dom_var_def>bool ANNOALLVISIBLE;
DICR21VAR( ODTBOOL,                ANNOALLVISIBLE,      (true),                   (true),                           (),                         ValidateBOOL())

/**
  <title ANNOTATIVEDWG>
  <toctitle ANNOTATIVEDWG>
   
  <table>
   <b>Type</b>            OdInt8
   <b>Initial value</b>   0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2008+
  </table>
   
  ANNOTATIVEDWG determines whether the current drawing will act like an annotative block if the 
  drawing is inserted into another drawing.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Non-annotative behavior.
   1              Annotative behavior.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getANNOTATIVEDWG()
   OdDbDatabase::setANNOTATIVEDWG()
  </table>
   
  <b>Note:</b> This variable becomes read only if the drawing contains annotative objects.

    <group TD_SysVars_A>
  */
//<dom_var_def>OdInt8 ANNOTATIVEDWG;
DICR21VAR(OdInt8,                  ANNOTATIVEDWG,       (0),                      (0),                              (),                         ValidateANNOTATIVEDWG())

DICR21VAR( ODTBOOL,                MSLTSCALE,           (true),                   (true),                           (),                         ValidateBOOL())

/**
  <title LAYEREVAL>
  <toctitle LAYEREVAL>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   1
   <b>Range</b>           0 ... 2
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2008+
  </table>
   
  LAYEREVAL controls when the list of unresolved layers is checked for analysis.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              No checking.
   1              Checks when new external reference layers are added.
   2              Checks when any new layers are added.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getLAYEREVAL()
   OdDbDatabase::setLAYEREVAL()
  </table> 

  <group TD_SysVars_L>
  */
//<dom_var_def>OdInt16 LAYEREVAL;
DICR21VAR( OdInt16,                LAYEREVAL,            (1),                     (1),                           (),                             ValidateRange(0,2))

/**
  <title LAYERNOTIFY>
  <toctitle LAYERNOTIFY>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   15
   <b>Range</b>           &#8211;63 ... 63
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2008+
  </table>
   
  LAYERNOTIFY controls when an alert is displayed for unresolved layers.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Do not display alerts.
   1              Display any alerts when printing.
   2              Display any alerts when opening drawings.
   4              Display any alerts when working with external references (attaching, loading, reloading).
   8              Display any alerts when restoring layer states.
   16             Display any alerts when saving drawings.
   32             Display any alerts when inserting blocks.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getLAYERNOTIFY()
   OdDbDatabase::setLAYERNOTIFY()
  </table>     

  <group TD_SysVars_L>
  */
//<dom_var_def>OdInt16 LAYERNOTIFY;
DICR21VAR( OdInt16,                LAYERNOTIFY,          (15),                    (15),                           (),                            ValidateRange(-63,63))

/**
  <title LIGHTINGUNITS>
  <toctitle LIGHTINGUNITS>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   2
   <b>Range</b>           0 ... 2
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2008+
  </table>
   
  The LIGHTINGUNITS controls the type of lighting that is used and its unit of measurement.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Standard lighting with no lighting units.
   1              Photometric lighting with US units.
   2              Photometric lighting with international units.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getLIGHTINGUNITS()
   OdDbDatabase::setLIGHTINGUNITS()
  </table>
   
  For example, to get the value:
  <code>
   OdRusBufPtr pRb = pDb-\>getSysVar(L"LIGHTINGUNITS");
   OdInt16 type = pRb-\>getInt16();
   odPrintConsoleString(L"nLight type = %d", type);
   // or //
   odPrintConsoleString(L"nLight type = %d", pDb-\>getLIGHTINGUNITS());
  </code>
  For example, to set the value:
  <code>
   OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtInt16, 1);
   pDb-\>setSysVar(L"LIGHTINGUNITS", pRb);
   // or //
   pDb-\>setLIGHTINGUNITS(1);
  </code> 

  <group TD_SysVars_L>
  */
//<dom_var_def>OdInt16 LIGHTINGUNITS;
DICR21VAR( OdInt16,                LIGHTINGUNITS,        (2),                     (2),                           (),                             ValidateRange(0,2)) // Default value since ACAD 2009 is 2 if drawing was created from scratch. But default acad template contains 0 value.

/**
  <title LIGHTSINBLOCKS>
  <toctitle LIGHTSINBLOCKS>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   1
   <b>Range</b>           0 ... 1
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2008+
  </table>
   
  LIGHTSINBLOCKS controls whether lights that are saved in blocks are used when rendering.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Lights in blocks are not used during rendering.
   1              Lights in blocks are used during rendering.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getLIGHTSINBLOCKS()
   OdDbDatabase::setLIGHTSINBLOCKS()
  </table>  

  <group TD_SysVars_L>
  */
//<dom_var_def>OdInt16 LIGHTSINBLOCKS;
DICR21VAR( OdInt16,                LIGHTSINBLOCKS,       (1),                     (1),                           (),                             ValidateRange(0,1))

/**
  <title DBCSTATE>
  <toctitle DBCSTATE>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2000+
  </table>
   
  DBCSTATE controls the display of the dbConnect Manager, whether or not the dbConnect Manager is active.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Not displayed.
   1              Displayed.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getDBCSTATE()
   OdDbDatabase::setDBCSTATE()
  </table> 

  <group TD_SysVars_D>
  */
//<dom_var_def>OdInt16 DBCSTATE;
DICR15VAR(  OdInt16,               DBCSTATE,            (0),                     (0),                              (),                         ValidateRange(0,1))

/**
  <title INTERSECTIONCOLOR>
  <toctitle INTERSECTIONCOLOR>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   257
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2004+
  </table>
   
  INTERSECTIONCOLOR specifies the ACI color index of intersection polylines. Intersection polylines are displayed
  at the intersections of 3D surfaces if and only if INTERSECTIONDISPLAY == true.
   
  <table>
   <b>Name</b>      <b>Value</b>   <b>Description</b>
   kACIbyBlock      0              ByBlock
   kACIRed          1              Red
   kACIYellow       2              Yellow
   kACIGreen        3              Green
   kACICyan         4              Cyan
   kACIBlue         5              Blue
   kACIMagenta      6              Magenta
   kACIforeground   7              Foregroundcolor
   ..               8-255          Defined by display device
   kACIbyLayer      256            ByLayer
   kACInone         257            No color
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getINTERSECTIONCOLOR()
   OdDbDatabase::setINTERSECTIONCOLOR()
  </table>    

  <group TD_SysVars_I>
  */
//<dom_var_def>OdInt16 INTERSECTIONCOLOR;
DICR15VAR(  OdInt16,               INTERSECTIONCOLOR,   (257),                   (257),                            (),                         ValidateRange(0,257))

/**
  <title INTERSECTIONDISPLAY>
  <toctitle INTERSECTIONDISPLAY>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   false
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2004+
  </table>
   
  INTERSECTIONDISPLAY controls the display of intersection polylines. Intersection polylines are displayed at the
  intersections of 3D surfaces.
   
  <table>
   <b>Value</b>   <b>Description</b>
   false          Not displayed
   true           Displayed
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getINTERSECTIONDISPLAY()
   OdDbDatabase::setINTERSECTIONDISPLAY()
  </table>   

  <group TD_SysVars_I>
  */
//<dom_var_def>bool INTERSECTIONDISPLAY;
DICR15VAR(  bool,                  INTERSECTIONDISPLAY, (false),                 (false),                          (),                         ValidateBOOL())

/** 
  <title HALOGAP>
  <toctitle HALOGAP>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2002+
  </table>
   
  HALOGAP specifies the gap to be displayed where one 3D object hides another. HALOGAP is zoom independent, 
  and is specified as a percentage of one drawing unit.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getHALOGAP()
   OdDbDatabase::setHALOGAP()
  </table>   

  <group TD_SysVars_H>
  */
//<dom_var_def>OdInt16 HALOGAP;
DICR15VAR(  OdInt16,               HALOGAP,             (0),                     (0),                              (),                         ValidateNone())

/**
  <title OBSCUREDCOLOR>
  <toctitle OBSCUREDCOLOR>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   257
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2002+
  </table>
   
  OBSCUREDCOLOR specifies the ACI color index of intersection polylines. Obscured lines are visible if and only if
  (OBSCUREDCOLOR != kACInone) &amp;&amp; (OBSCUREDLTYPE != 0)
   
  <table>
   <b>Name</b>      <b>Value</b>   <b>Description</b>
   kACIbyBlock      0              ByBlock
   kACIRed          1              Red
   kACIYellow       2              Yellow
   kACIGreen        3              Green
   kACICyan         4              Cyan
   kACIBlue         5              Blue
   kACIMagenta      6              Magenta
   kACIforeground   7              Foregroundcolor
   ..               8-255          Defined by display device
   kACIbyLayer      256            ByLayer
   kACInone         257            No color
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getOBSCUREDCOLOR()
   OdDbDatabase::setOBSCUREDCOLOR()
  </table>   

  <group TD_SysVars_O>
  */
//<dom_var_def>OdInt16 OBSCUREDCOLOR;
DICR15VAR(  OdInt16,               OBSCUREDCOLOR,       (257),                   (257),                            (),                          ValidateRange(0,257))

/**
  <title OBSCUREDLTYPE>
  <toctitle OBSCUREDLTYPE>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2002+
  </table>
   
  OBSCUREDLTYPE specifies the display and linetype of obscured lines.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Off
   1              Solid
   2              Dashed
   3              Dotted
   4              Short Dash
   5              Medium Dash
   6              Long Dash
   7              Double Short Dash
   8              Double Medium Dash
   9              Double Long Dash
   10             Medium Long Dash
   11             Sparse Dot
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getOBSCUREDLTYPE()
   OdDbDatabase::setOBSCUREDLTYPE()
  </table> 

  <group TD_SysVars_O>
  */
//<dom_var_def>OdInt16 OBSCUREDLTYPE;
DICR15VAR(  OdInt16,               OBSCUREDLTYPE,       (0),                     (0),                              (),                         ValidateRange(0,11))

// these was dictvardefs in old versions

/**
  <title INDEXCTL>
  <toctitle INDEXCTL>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   0
   <b>Range</b>           0 ... 3
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R14+
  </table>
  
  INDEXCTL specifies the creation of layer and spatial indices in the database.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              None created
   1              Layer index created
   2              Spatial index created
   3              Both created
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getINDEXCTL()
   OdDbDatabase::setINDEXCTL()
  </table>  

  <group TD_SysVars_I>
  */
//<dom_var_def>OdInt16 INDEXCTL;
DICR15VAR(  OdInt16,               INDEXCTL,            (0),                     (0),                              (),                         ValidateRange(0,3))

/**
  <title PROJECTNAME>
  <toctitle PROJECTNAME>
   
  <table>
   <b>Type</b>            OdString
   <b>Initial value</b>   ""
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2000+
  </table>
   
  PROJECTNAME specifies the project name associated with the database.
  
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getPROJECTNAME()
   OdDbDatabase::setPROJECTNAME()
  </table>    

  <group TD_SysVars_P>
  */
//<dom_var_def>OdString PROJECTNAME;
DICR15VAR( OdString,               PROJECTNAME,         (OdString::kEmpty),      (OdString::kEmpty),               (),                         ValidateNone())

/**
  <title SORTENTS>
  <toctitle SORTENTS>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   127
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R12+
  </table>
   
  SORTENTS controls the object sort order. SORTENTS will be a combination of the following:
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              No sorting
   1              Object selection sorting
   2              Object snap sorting
   4              Clears all checkboxes
   8              MSLIDE command sorting
   16             REGEN command sorting
   32             Plotting sorting
   64             Clears all checkboxes
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getSORTENTS()
   OdDbDatabase::setSORTENTS()
  </table>
   
  <b>Note: </b> The SORTENTS system variable has been depreciated.
   
  \sa
   <link DRAWORDERCTL, DRAWORDERCTL>
 
  <group TD_SysVars_S>
 */
//<dom_var_def>OdInt16 SORTENTS;
DICR15VAR( OdInt16,                SORTENTS,            (127),                   (127),                            (),                         ValidateRange(0,127))

/**
  <title DIMASSOC>
  <toctitle DIMASSOC>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   2
   <b>Range</b>           0 ... 2
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2002+
  </table>
   
  DIMASSOC controls the associativity of new dimension entities.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Creates unexploded dimensions.
   1              Creates un-associative dimension entities.
   2              Creates associative dimension entities.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getDIMASSOC()
   OdDbDatabase::setDIMASSOC()
  </table> 

<group TD_SysVars_D>
  */
//<dom_var_def>OdInt16 DIMASSOC;
DICR15VAR( OdInt16,                DIMASSOC,            (2),                     (2),                              (),                         ValidateRange(0,2))

/**
  <title HIDETEXT>
  <toctitle HIDETEXT>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   true
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2002+
  </table>
   
  HIDETEXT controls the participation of Text and MText entities in HIDE commands.
   
  <table>
   <b>Value</b>   <b>Description</b>
   false          Don't hide and are not hidden.
   true           Hide and are hidden.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getHIDETEXT()
   OdDbDatabase::setHIDETEXT()
  </table>  

<group TD_SysVars_H>
  */
//<dom_var_def>bool HIDETEXT;
DICR15VAR( bool,                   HIDETEXT,            (true),                  (true),                           (),                         ValidateBOOL())

/**
  <title DRAWORDERCTL>
  <toctitle DRAWORDERCTL>
   
  <table>
   <b>Type</b>            OdInt8
   <b>Initial value</b>   3
   <b>Range</b>           0 ... 3
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2005+
  </table>
   
  DRAWORDERCTL controls the display update and inheritance of draworder when using certain editing commands. 
  The following commands are affected by DRAWORDERCTL: BREAK, EXPLODE, FILLET, HATCH, HATCHEDIT, JOIN, 
  OFFSET, PEDIT, and TRIM.
     * With Draworder Inheritance on, entities created with
       these commands assume the draw order of the original objects.
     * With Draworder Inheritance off, they assume the default
       draworder (top).
     * With Display Update on, entities created with the
       aforementioned editing commands are displayed correctly
       according to their Draworder.
     * With Display Update off, they are displayed on top, until
       the next regeneration.
   
   <table>
   <b>Value</b>   <b>Draworder Inheritance</b>   <b>Display Update</b>
   0              Off                            Off
   1              Off                            On
   2              On                             Off
   3              On                             On
   </table>
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getDRAWORDERCTL()
   OdDbDatabase::setDRAWORDERCTL()
   </table> 

<group TD_SysVars_D>
   */
//<dom_var_def>OdInt8 DRAWORDERCTL;
DICR21VAR( OdInt8,                   DRAWORDERCTL,        (3),                     (3),                              (),                    ValidateRange(0,3))

// 2006 support

/**
  <title HPINHERIT>
  <toctitle HPINHERIT>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   false
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2006+
  </table>
   
  HPINHERIT specifies the hatch origin when hatch properties are inherited with the HATCH and HATCHEDIT commands.
   
  <table>
   <b>Value</b>   <b>Description</b>
   false          From HPORIGIN
   true           From Source
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getHPINHERIT()
   OdDbDatabase::setHPINHERIT()
  </table>    

<group TD_SysVars_H>
  */
//<dom_var_def>bool HPINHERIT;
DICR21VAR(   ODTBOOL,                HPINHERIT,           (false),                 (false),                          (),                         ValidateBOOL())

/**
  <title HPORIGIN>
  <toctitle HPORIGIN>
   
  <table>
   <b>Type</b>            OdGePoint2d
   <b>Initial value</b>   (0.0, 0.0)
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2006+
  </table>
   
  HPORIGIN specifies the hatch origin for new hatch entities relative to the current UCS.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getHPORIGIN()
   OdDbDatabase::setHPORIGIN()
  </table>    

<group TD_SysVars_H>
  */
//<dom_var_def>OdGePoint2d HPORIGIN;
DICR21VAR(   OdGePoint2d,            HPORIGIN,            (OdGePoint2d::kOrigin),  (OdGePoint2d::kOrigin),           (),                         ValidateNone())

/**
  <title FIELDEVAL>
  <toctitle FIELDEVAL>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   0x1F
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2005+
  </table>
   
  FIELDEVAL specifies when fields are updated. FIELDEVAL will be a combination of one or more of the 
  following values:
   
  <table>
   <b>Name</b>               <b>Value</b>   <b>Description</b>
   OdDbField::kDisable       0              Disable evaluation of field.
   OdDbField::kOnOpen        0x01           Evaluate on database open.
   OdDbField::kOnSave        0x02           Evaluate on database save.
   OdDbField::kOnPlot        0x04           Evaluate on database plot.
   OdDbField::kOnEtransmit   0x08           Evaluate on database Etransmit.
   OdDbField::kOnRegen       0x10           Evaluate on database regen.
   OdDbField::kOnDemand      0x20           Evaluate on demand.
   OdDbField::kAutomatic     0x3F           Evaluate automatically.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getFIELDEVAL()
   OdDbDatabase::setFIELDEVAL()
  </table>  

<group TD_SysVars_F>
  */
//<dom_var_def>OdInt16 FIELDEVAL;
DICR21VAR(   OdInt16,                FIELDEVAL,           (31),                     (31),                             (),                         ValidateRange(0,31))

DICR21VAR(   double,                 MSOLESCALE,          (1.0),                    (1.0),                            (),                         ValidateEqGr(0.))

/**
  <title UPDATETHUMBNAIL>
  <toctitle UPDATETHUMBNAIL>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   0x0F
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2005+
  </table>
   
  UPDATETHUMBNAIL controls the updating of preview images in the Sheet Set Manager. UPDATETHUMBNAIL will 
  be a combination of one or more of the following:
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Do not update previews.
   0x01           Update model view previews.
   0x02           Update sheet view previews.
   0x04           Update sheet previews.
   0x08           Update previews when sheets and views are modified, restored, or created.
   0x16           Update previews when database is saved.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getUPDATETHUMBNAIL()
   OdDbDatabase::setUPDATETHUMBNAIL()
  </table>  

<group TD_SysVars_U>
  */
//<dom_var_def>OdInt16 UPDATETHUMBNAIL;
DICR21VAR(   OdInt16,                UPDATETHUMBNAIL,     (15),                     (15),                             (),                         ValidateRange(0,31))

//-----------------------------------------
// Variables added in AC 2007
//-----------------------------------------

/**
  <title SOLIDHIST>
  <toctitle SOLIDHIST>
   
  <table>
   <b>Type</b>            OdInt8
   <b>Initial value</b>   0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2007+
  </table>
   
  SOLIDHIST controls the default history retention setting for all solids. When enabled, a history of 
  the solids comprising a composite solid is retained.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Disabled
   1              Enabled
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getSOLIDHIST()
   OdDbDatabase::setSOLIDHIST()
  </table>         

<group TD_SysVars_S>
  */
//<dom_var_def>OdInt8 SOLIDHIST;
VAR_DEF(   OdInt8,                  SOLIDHIST,            (0),                    (0),                             (),                         ValidateRange(0,1))

/** 
  <title SHOWHIST>
  <toctitle SHOWHIST>
   
  <table>
   <b>Type</b>            OdInt8
   <b>Initial value</b>   1
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2007+
  </table>
   
  SHOWHIST controls the History display for solids in a drawing.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Hides all history.
   1              Uses the individual solid Show History properties.
   2              Displays all history.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getSHOWHIST()
   OdDbDatabase::setSHOWHIST()
  </table>      

<group TD_SysVars_S>
  */
//<dom_var_def>OdInt8 SHOWHIST;
VAR_DEF(   OdInt8,                  SHOWHIST,            (1),                    (1),                             (),                         ValidateRange(0,2))

/**
  <title PSOLWIDTH>
  <toctitle PSOLWIDTH>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   0.25 | 5.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2007+
  </table>
   
  PSOLWIDTH specifies the default width for swept solids. PSOLWIDTH value should be greater than 0.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getPSOLWIDTH()
   OdDbDatabase::setPSOLWIDTH()
  </table>   

<group TD_SysVars_P>
  */
//<dom_var_def>double PSOLWIDTH;
DICR15VAR( ODTDIST,                 PSOLWIDTH,         (0.25),                  (5.0),                             (),                         ValidateRange(DBL_MIN,DBL_MAX))

/**
  <title LOFTPARAM>
  <toctitle LOFTPARAM>
   
  <table>
   <b>Type</b>            OdDb::LoftParamType
   <b>Initial value</b>   OdDb::kLoftDefault
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2007+
  </table>
   
  LOFTPARAM controls the shape of lofted solids and surfaces. LOFTPARAM will be a combination of 
  one or more of the following:
   
  <table>
   <b>Name</b>           <b>Value</b>   <b>Description</b>
   kLoftNoParam          0x00           No parameters.
   kLoftNoTwist          0x01           No twist between cross sections.
   kLoftAlignDirection   0x02           Align directions of cross sections.
   kLoftSimplify         0x04           Create simple solids and surfaces.
   kLoftClose            0x08           Close between the starting and ending cross sections.
   kLoftDefault          0x07           kLoftNoTwist | kLoftAlignDirection | kLoftSimplify
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getLOFTPARAM()
   OdDbDatabase::setLOFTPARAM()
  </table>   

<group TD_SysVars_L>
  */
//<dom_var_def>OdDb::LoftParamType LOFTPARAM;
VAR_DEF(  ODTDB_LOFTPARAMTYPE,                LOFTPARAM,            (OdDb::kLoftDefault),                    (OdDb::kLoftDefault),     (),   ValidateRange(OdDb::kLoftNoParam,ODTDB_LOFTPARAMTYPE(OdDb::kLoftNoTwist|OdDb::kLoftAlignDirection|OdDb::kLoftSimplify|OdDb::kLoftClose)))

/**
  <title LOFTNORMALS>
  <toctitle LOFTNORMALS>
   
  <table>
   <b>Type</b>            OdDb::LoftNormalsType
   <b>Initial value</b>   OdDb::kLoftSmooth
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2007+
  </table>
   
  LOFTNORMALS controls the normals of lofted objects as they pass through cross sections. 
  It is ignored when a path or guide curve is specified.
   
  <table>
   <b>Name</b>           <b>Value</b>   <b>Description</b>
   kLoftRuled            0              Ruled surface.
   kLoftSmooth           1              Smooth surface.
   kLoftFirstNormal      2              Surface is normal to the first cross section.
   kLoftLastNormal       3              Surface is normal to the last cross section.
   kLoftEndsNormal       4              Surface is normal to the first and last cross sections.
   kLoftAllNormal        5              Surface is normal to all cross sections.
   kLoftUseDraftAngles   6              Surface uses draft angle and magnitude.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getLOFTNORMALS()
   OdDbDatabase::setLOFTNORMALS()
  </table> 

<group TD_SysVars_L>
  */
//<dom_var_def>OdDb::LoftNormalsType LOFTNORMALS;
VAR_DEF( ODTDB_LOFTNORMALSTYPE,             LOFTNORMALS,            (OdDb::kLoftSmooth),                    (OdDb::kLoftSmooth),     (),         ValidateRange(OdDb::kLoftRuled,OdDb::kLoftUseDraftAngles))

/**
  <title LOFTANG1>
  <toctitle LOFTANG1>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   pi/2
   <b>Range</b>           0 ... 2pi
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2007+
  </table>
   
  LOFTANG1 specifies the draft angle through the first cross section in a loft operation. 0 is measured 
  in the plane of the curve, outward from the curve. Positive angles are measured toward the next cross section.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getLOFTANG1()
   OdDbDatabase::setLOFTANG1()
  </table>
   
  <b>Note: </b>All angles are expressed in radians.  

<group TD_SysVars_L>
  */
//<dom_var_def>double LOFTANG1;
VAR_DEF(  ODTORIENT,                   LOFTANG1,           (OdaPI2),                   (OdaPI2),                             (),                         ValidateRange(0, Oda2PI))

/**
  <title LOFTANG2>
  <toctitle LOFTANG2>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   pi/2
   <b>Range</b>           0 ... 2pi
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2007+
  </table>
   
  LOFTANG2 specifies the draft angle through the last cross section in a loft operation. 0 is measured 
  in the plane of the curve, outward from the curve. Positive angles are measured toward the previous 
  cross section.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getLOFTANG2()
   OdDbDatabase::setLOFTANG2()
  </table>
   
  <b>Note: </b>All angles are expressed in radians. 

<group TD_SysVars_L>
  */
//<dom_var_def>double LOFTANG2;
VAR_DEF(  ODTORIENT,                   LOFTANG2,           (OdaPI2),                   (OdaPI2),                             (),                         ValidateRange(0, Oda2PI))

/**
  <title LOFTMAG1>
  <toctitle LOFTMAG1>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   0.0
   <b>Range</b>           0.0 ... 1e100
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2007+
  </table>
   
  LOFTMAG1 specifies the draft angle mangitude through the first cross section of a lofted solid.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getLOFTMAG1()
   OdDbDatabase::setLOFTMAG1()
  </table>
  
  <group TD_SysVars_L>
 */
//<dom_var_def>double LOFTMAG1;
VAR_DEF(  ODTUNITLESS8,                LOFTMAG1,            (0.),                    (0.),                             (),                         ValidateRange(0.,1.e100))

/**
  <title LOFTMAG2>
  <toctitle LOFTMAG2>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   0.0
   <b>Range</b>           0.0 ... 1e100
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2007+
  </table>
   
  LOFTMAG2 specifies the draft angle mangitude through the last cross section of a lofted solid.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getLOFTMAG2()
   OdDbDatabase::setLOFTMAG2()
  </table>

  <group TD_SysVars_L>
  */
//<dom_var_def>double LOFTMAG2;
VAR_DEF(  ODTUNITLESS8,                LOFTMAG2,            (0.),                    (0.),                             (),                         ValidateRange(0., 1.e100))

/**
  <title LATITUDE>
  <toctitle LATITUDE>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   37.7950
   <b>Range</b>           ±90°
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2007+
  </table>
   
  LATITUDE specifies the latitude of the drawing model in decimal degrees. The default is determined by the country
  code in the operating system.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getLATITUDE()
   OdDbDatabase::setLATITUDE()
  </table>
   
  <b>Note: </b>All angles are expressed in decimal degrees. North latitudes are positive.      

<group TD_SysVars_L>
  */
//<dom_var_def>double LATITUDE;
VAR_DEF(  ODTORIENT,                    LATITUDE,            (37.7950),               (37.7950),                             (),                         ValidateRange(-90., 90.))

/**
  <title LONGITUDE>
  <toctitle LONGITUDE>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   \-122.3940
   <b>Range</b>           ±180&deg;
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2007+
  </table>
   
  LONGITUDE specifies the longitude of the drawing model in decimal degrees. The default is determined by 
  the country code in the operating system.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getLONGITUDE()
   OdDbDatabase::setLONGITUDE()
  </table>
   
  <b>Note: </b>All angles are expressed in decimal degrees. West longitudes are positive.  

<group TD_SysVars_L>
  */
//<dom_var_def>double LONGITUDE;
VAR_DEF(  ODTORIENT,                    LONGITUDE,           (-122.3940),             (-122.3940),                             (),                         ValidateRange(-180., 180.))

/**
  <title NORTHDIRECTION>
  <toctitle NORTHDIRECTION>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   0.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2007+
  </table>
   
  NORTHDIRECTION specifies the angle of the Sun from North. This angle is in the context of the WCS.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getNORTHDIRECTION()
   OdDbDatabase::setNORTHDIRECTION()
  </table>
   
  <b>Note: </b>All angles are expressed in radians.  

<group TD_SysVars_N>
*/
//<dom_var_def>double NORTHDIRECTION;
VAR_DEF(  ODTORIENT,                    NORTHDIRECTION,       (0.),                    (0.),                             (),                          ValidateNone())

/**
  <title TIMEZONE>
  <toctitle TIMEZONE>
   
  <table>
   <b>Type</b>            OdDb::TimeZone
   <b>Initial value</b>   OdDb::kPacific
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2007+
  </table>
   
  TIMEZONE sets the time zone for sun studies in the drawing. TIMEZONE represents hours and minutes 
  away from Greenwich Mean Time. Setting a geographic location sets TIMEZONE.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getTIMEZONE()
   OdDbDatabase::setTIMEZONE()
  </table>

 <group TD_SysVars_T> 
  */
//<dom_var_def>OdDb::TimeZone TIMEZONE;
VAR_DEF( ODTDB_TIMEZONE,             TIMEZONE, (OdDb::kPacific),      (OdDb::kPacific),                            (),                            ValidateRange(OdDb::kInternationalDateLine,OdDb::kTonga))

/**
  <title LIGHTGLYPHDISPLAY>
  <toctitle LIGHTGLYPHDISPLAY>
   
  <table>
   <b>Type</b>            OdInt8
   <b>Initial value</b>   1
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2007+
  </table>
   
  LIGHTGLYPHDISPLAY controls the display of light glyphs.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Not displayed
   1              Displayed
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getLIGHTGLYPHDISPLAY()
   OdDbDatabase::setLIGHTGLYPHDISPLAY()
  </table> 

<group TD_SysVars_L>
  */
//<dom_var_def>OdInt8 LIGHTGLYPHDISPLAY;
VAR_DEF(  OdInt8,          LIGHTGLYPHDISPLAY,            (1),                    (1),                             (),                              ValidateRange(0,1))

/**
  <title TILEMODELIGHTSYNCH>
  <toctitle TILEMODELIGHTSYNCH>
   
  <table>
   <b>Type</b>            OdInt8
   <b>Initial value</b>   1
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        N/A
  </table>
   
  TILEMODELIGHTSYNCH controls the sychronization of lighting in all model space viewports.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Not synchronized
   1              Synchronized
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getTILEMODELIGHTSYNCH()
   OdDbDatabase::setTILEMODELIGHTSYNCH()
  </table> 

<group TD_SysVars_T>
  */
//<dom_var_def>OdInt8 TILEMODELIGHTSYNCH;
VAR_DEF(  OdInt8,          TILEMODELIGHTSYNCH,            (1),                    (1),                             (),                              ValidateRange(0,1))

/**
  <title INTERFERECOLOR>
  <toctitle INTERFERECOLOR>
   
  <table>
   <b>Type</b>            OdCmColor
   <b>Initial value</b>   OdCmEntityColor::kByLayer
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2007+
  </table>
   
  INTERFERECOLOR specifies the color of interference objects.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getINTERFERECOLOR()
   OdDbDatabase::setINTERFERECOLOR()
  </table>  

<group TD_SysVars_I>
  */
//<dom_var_def>OdCmColor INTERFERECOLOR;
VAR_DEF(  OdCmColor,       INTERFERECOLOR, (OdCmEntityColor::kByLayer), (OdCmEntityColor::kByLayer),               (),                              ValidateNone())

/**
  <title INTERFEREOBJVS>
  <toctitle INTERFEREOBJVS>
   
  <table>
   <b>Type</b>            OdDbObjectId
   <b>Initial value</b>   OdDbObjectId::kNull
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2007+
  </table>
   
  INTERFEREOBJVS specifies the interference object visual style.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getINTERFEREOBJVS()
   OdDbDatabase::setINTERFEREOBJVS()
  </table> 

<group TD_SysVars_I>
  */
//<dom_var_def>OdDbObjectId INTERFEREOBJVS;
VAR_DEF( ODVISTYLEID,    INTERFEREOBJVS,       (OdDbObjectId::kNull),       (OdDbObjectId::kNull),              (),                               ValidateVisualStyle())

/**
  <title INTERFEREVPVS>
  <toctitle INTERFEREVPVS>
   
  <table>
   <b>Type</b>            OdDbObjectId
   <b>Initial value</b>   OdDbObjectId::kNull
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2007+
  </table>
   
  INTERFEREVPVS specifies the interference checking visual style for the viewport.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getINTERFEREVPVS()
   OdDbDatabase::setINTERFEREVPVS()
  </table>  

<group TD_SysVars_I>
  */
//<dom_var_def>OdDbObjectId INTERFEREVPVS;
VAR_DEF( ODVISTYLEID,       INTERFEREVPVS,  (OdDbObjectId::kNull),          (OdDbObjectId::kNull),              (),                                ValidateVisualStyle())

/**
  <title DRAGVS>
  <toctitle DRAGVS>
   
  <table>
   <b>Type</b>            OdDbObjectId
   <b>Initial value</b>   OdDbObjectId::kNull
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2007+
  </table>
   
  DRAGVS specifies the drag visual style.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getDRAGVS()
   OdDbDatabase::setDRAGVS()
  </table>  

<group TD_SysVars_D>
  */
//<dom_var_def>OdDbObjectId DRAGVS;
VAR_DEF( ODVISTYLEID,               DRAGVS,  (OdDbObjectId::kNull),          (OdDbObjectId::kNull),              (),                                ValidateVisualStyle())

/**
  <title CSHADOW>
  <toctitle CSHADOW>
   
  <table>
   <b>Type</b>            OdGiSubEntityTraits::ShadowFlags
   <b>Initial value</b>   OdGiSubEntityTraits::ShadowFlags::kShadowsCastAndReceive
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2007+
  </table>
   
  CSHADOW controls shadows for 3D objects.
   
  <table>
   <b>Name</b>              <b>Value</b>   <b>Description</b>
   kShadowsCastAndReceive   0x00           Shadows cast and received.
   kShadowsDoesNotReceive   0x01           Shadows cast, but not received.
   kShadowsDoesNotCast      0x02           Shadows received but not cast.
   kShadowsIgnore           0x03           Shadows neither cast nor received.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getCSHADOW()
   OdDbDatabase::setCSHADOW()
  </table>
   
  <b>Note: </b>Shadows must be turned on in the visual style that is applied to the viewport. 

<group TD_SysVars_C>
  */
//<dom_var_def>OdGiSubEntityTraits::ShadowFlags CSHADOW;
VAR_DEF(ODTDB_SHADOWFLAGS,         CSHADOW,    (OdGiSubEntityTraits::kShadowsCastAndReceive), (OdGiSubEntityTraits::kShadowsCastAndReceive),    (),                                ValidateRange(OdGiSubEntityTraits::kShadowsCastAndReceive,OdGiSubEntityTraits::kShadowsIgnore))

/**
  <title SHADOWPLANELOCATION>
  <toctitle SHADOWPLANELOCATION>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   0.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2007+
  </table>
   
  SHADOWPLANELOCATION specifies the location on the current Z-axis of an invisible ground plane 
  used to display shadows. The shadow plane casts and receives shadows.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getSHADOWPLANELOCATION()
   OdDbDatabase::setSHADOWPLANELOCATION()
  </table>

<group TD_SysVars_S>
  */
//<dom_var_def>double SHADOWPLANELOCATION;
VAR_DEF(  ODTDIST,         SHADOWPLANELOCATION,           (0.),                    (0.),                             (),                               ValidateNone())

/**
  <title CAMERADISPLAY>
  <toctitle CAMERADISPLAY>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   false
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2007+
  </table>
   
  CAMERADISPLAY controls the display of camera glyphs.
   
  <table>
   <b>Value</b>   <b>Description</b>
   false          Not displayed
   true           Displayed
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getCAMERADISPLAY()
   OdDbDatabase::setCAMERADISPLAY()
  </table> 

<group TD_SysVars_C>
  */
//<dom_var_def>bool CAMERADISPLAY;
VAR_DEF( ODTBOOL,                CAMERADISPLAY,           (false),                 (false),                          (),                         ValidateBOOL())

/**
  <title LENSLENGTH>
  <toctitle LENSLENGTH>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   50.0
   <b>Read-Only</b>       Yes
   <b>Saved In</b>        Database
   <b>Versions</b>        2007+
  </table>
   
  LENSLENGTH specifies the length, in millimeters, of the lens used in perspective viewing.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   &nbsp; 
   OdDbDatabase::getLENSLENGTH()
   OdDbDatabase::setLENSLENGTH()
  </table> 

<group TD_SysVars_L>
  */
//<dom_var_def>double LENSLENGTH;
VAR_DEF( ODTDIST,               LENSLENGTH,           (50.),                     (50.),                             (),                           ValidateNone())

/**
  <title CAMERAHEIGHT>
  <toctitle CAMERAHEIGHT>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   0.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2007+
  </table>
   
  CAMERAHEIGHT specifies the default height, in drawing units, for new camera objects.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getCAMERAHEIGHT()
   OdDbDatabase::setCAMERAHEIGHT()
  </table> 

<group TD_SysVars_C>
  */
//<dom_var_def>double CAMERAHEIGHT;
VAR_DEF( ODTDIST,               CAMERAHEIGHT,           (0.),                    (0.),                             (),                           ValidateNone())

/**
  <title STEPSPERSEC>
  <toctitle STEPSPERSEC>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   2.0
   <b>Range</b>           1.0 ... 30.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2007+
  </table>
   
  STEPSPERSEC specifies the number of steps taken per second in walk or fly mode.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getSTEPSPERSEC()
   OdDbDatabase::setSTEPSPERSEC()
  </table>   

<group TD_SysVars_S>
  */
//<dom_var_def>double STEPSPERSEC;
VAR_DEF( ODTDIST,               STEPSPERSEC,           (2.),                    (2.),                             (),                           ValidateRange(1,30))

/**
  <title STEPSIZE>
  <toctitle STEPSIZE>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   6.0
   <b>Range</b>           1e-6 ... 1e6
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2007+
  </table>
   
  STEPSIZE specifies the size of each step in drawing units, in walk or fly mode.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getSTEPSIZE()
   OdDbDatabase::setSTEPSIZE()
  </table>

<group TD_SysVars_S>
  */
//<dom_var_def>double STEPSIZE;
VAR_DEF( ODTDIST,               STEPSIZE,           (6.),                    (6.),                             (),                           ValidateRange(1e-6,1e+6))

/**
  <table>
    <b>Type</b>               double
    <b>Initial value</b>      2.0
    <b>Range</b>              1 ... 6   
    <b>Read-Only</b>          No
    <b>Saved In</b>           Database
    <b>Versions</b>           2007+
  </table>
  
  3DDWFPREC specifies the 3D DWF publishing precision.
  
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
    &nbsp; 
    OdDbDatabase::get3DDWFPREC()
    OdDbDatabase::set3DDWFPREC()
  </table>
  
  <group System_Variables_3>
*/
//<dom_var_deff>double 3DDWFPREC;
VAR_DEF( ODTDIST,               3DDWFPREC,           (2.),                    (2.),                             (),                           ValidateRange(1,6))

/**
  <title PSOLHEIGHT>
  <toctitle PSOLHEIGHT>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   4.0 | 80.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2007+
  </table>
   
  PSOLHEIGHT specifies the default height, in drawing units, used by the POLYSOLID command, for 
  swept solid objects.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getPSOLHEIGHT()
   OdDbDatabase::setPSOLHEIGHT()
  </table>  

<group TD_SysVars_P>
  */
//<dom_var_def>double PSOLHEIGHT;
DICR15VAR( ODTDIST,             PSOLHEIGHT,          (4.),                    (80.),                            (),                     ValidateNone())

/**
  <title CMATERIAL>
  <toctitle CMATERIAL>
   
  <table>
   <b>Type</b>            OdDbObjectId
   <b>Initial value</b>   "ByLayer" material object ID
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2007+
  </table>
   
  CMATERIAL specifies the material of new objects.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getCMATERIAL()
   OdDbDatabase::setCMATERIAL()
  </table> 

<group TD_SysVars_C>
  */
//<dom_var_def>OdDbObjectId CMATERIAL;
VAR_DEF( ODTMATERIALID,         CMATERIAL,      (OdDbObjectId::kNull),          (OdDbObjectId::kNull),            (),                     ValidateMaterial())

/**
  <title DWFFRAME>
  <toctitle DWFFRAME>
   
  <table>
   <b>Type</b>            OdInt8
   <b>Initial value</b>   2
   <b>Range</b>           0 ... 2
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2007+
  </table>
   
  DWFFRAME controls the visibility and plotting of the DWF underlay frame.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Not visible and not plotted.
   1              Visible and plotted.
   2              Visible but not plotted.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getDWFFRAME()
   OdDbDatabase::setDWFFRAME()
  </table> 

<group TD_SysVars_D>
  */
//<dom_var_def>OdInt8 DWFFRAME;
DICR21VAR_H(  OdInt8,          DWFFRAME,            (2),                    (2),                             (),                              ValidateRange(0,2))

/**
  <title DGNFRAME>
  <toctitle DGNFRAME>
   
  <table>
   <b>Type</b>            OdInt8
   <b>Initial value</b>   2
   <b>Range</b>           0 ... 2
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2008+
  </table>
   
  DGNFRAME controls the visibility and plotting of the DGN frame.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Not visible and not plotted.
   1              Visible and plotted.
   2              Visible but not plotted.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getDGNFRAME()
   OdDbDatabase::setDGNFRAME()
  </table>

<group TD_SysVars_D>
  */
//<dom_var_def>OdInt8 DGNFRAME;
VAR_DEF_H(  OdInt8,          DGNFRAME,            (2),                    (2),                             (),                              ValidateRange(0,2))

/**
  <title REALWORLDSCALE>
  <toctitle REALWORLDSCALE>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   true
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2007+
  </table>
   
  REALWORLDSCALE controls the rendering of materials with units set to real-world scale.
   
  <table>
   <b>Value</b>   <b>Description</b>
   false          Not rendered
   true           Rendered
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getREALWORLDSCALE()
   OdDbDatabase::setREALWORLDSCALE()
  </table> 

<group TD_SysVars_R>
  */
//<dom_var_def>bool REALWORLDSCALE;
VAR_DEF( bool,          REALWORLDSCALE,            (true),                    (true),                             (),                              ValidateBOOL())

//-----------------------------------------
// Variables added in AC 2008
//-----------------------------------------
DICR21VAR( OdInt16,     DXEVAL,     (12),                     (12),                          (),                         ValidateRange(0,511))

//-----------------------------------------
// Variables added in AC 2009
//-----------------------------------------
DICR21VAR( OdInt16,        GEOLATLONGFORMAT,        (1),                     (1),                          (),                         ValidateRange(0,1))

/**
  <title GEOMARKERVISIBILITY>
  <toctitle GEOMARKERVISIBILITY>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   1
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2009+
  </table>
   
  GEOMARKERVISIBILITY controls the visibility of the geolocation marker.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Geolocation marker is invisible.
   1              Geolocation marker is visible.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getGEOMARKERVISIBILITY()
   OdDbDatabase::setGEOMARKERVISIBILITY()
  </table>    

<group TD_SysVars_G>
  */
//<dom_var_def>OdInt16 GEOMARKERVISIBILITY;
DICR21VAR( OdInt16,        GEOMARKERVISIBILITY,     (1),                     (1),                          (),                         ValidateRange(0,1))

DICR21VAR( OdInt16,                PREVIEWTYPE,     (0),                     (0),                          (),                         ValidateRange(0,1))

//-----------------------------------------
// Variables added in AC 2010
//-----------------------------------------

// DIMTXTDIRECTION - defined in DimVarDefs.h

/*** VARDOCS:
  <properties>
    <remarks>
      "\n    EXPORTMODELSPACE specifies what part of the drawing to export to a DWF, DWFx, or PDF file from Model space"
      "\n"
      "\n    @table"
      "\n    Value   Description"
      "\n    0       Display"
      "\n    1       Extents"
      "\n    2       Window"
    </remarks>
  </properties>
*/
DICR24VAR( OdInt16,    EXPORTMODELSPACE,             (0),                 (0),                          (),                         ValidateRange(0,2))

/*** VARDOCS:
  <properties>
    <remarks>
      "\n    EXPORTPAPERSPACE specifies what part of the drawing to export to a DWF, DWFx, or PDF file from paper space"
      "\n"
      "\n    @table"
      "\n    Value   Description"
      "\n    0       Current Layout"
      "\n    1       All Layouts"
    </remarks>
  </properties>
*/
DICR24VAR( OdInt16,    EXPORTPAPERSPACE,             (0),                 (0),                          (),                         ValidateRange(0,1))

/*** VARDOCS:
  <properties>
    <remarks>
      "\n    EXPORTPAGESETUP specifies whether to export to a DWF, DWFx, or PDF file with the current page setup"
      "\n"
      "\n    @table"
      "\n    Value   Description"
      "\n    0       Default"
      "\n    1       Override"
    </remarks>
  </properties>
*/
DICR24VAR( OdInt16,     EXPORTPAGESETUP,             (0),                 (0),                          (),                         ValidateRange(0,1))

/*** VARDOCS:
  <properties>
    <remarks>
      "\n    FRAME controls the visibility of frames for external references, images and underlays. It overrides the individual IMAGEFRAME, DWFFRAME, PDFFRAME, DGNFRAME, and XCLIPFRAME settings"
      "\n"
      "\n    @table"
      "\n    Value   Description"
      "\n    0       Invisible"
      "\n    1       Is displayed and plotted"
      "\n    2       Is displayed but not plotted"
      "\n    3       The individual setting varies for different underlays, external references and images"
    </remarks>
  </properties>
*/
DICR24VAR_H( OdInt16,               FRAME,             (3),                 (3),                          (),                         ValidateRange(0,3))
/*** VARDOCS:
  <properties>
    <remarks>
      "\n    PDFFRAME controls the visibility of PDF underlay frames."
      "\n"
      "\n    @table"
      "\n    Value   Description"
      "\n    0       Invisible"
      "\n    1       Is displayed and plotted"
      "\n    2       Is displayed but not plotted"
    </remarks>
  </properties>
*/
/* This variable was bool in previous version */
DICR24VAR_H( OdInt16,               PDFFRAME,          (1),                 (1),                          (),                         ValidateRange(0,2))

/**
  <title XCLIPFRAME>
  <toctitle XCLIPFRAME>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   2
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R14+
  </table>
   
   XCLIPFRAME controls the visibility of Xref clip boundaries.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Xref clip boundaries are invisible and not plotted.
   1              Xref clip boundaries are visible and plotted.
   2              Xref clip boundaries are visible and not plotted.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getXCLIPFRAME()
   OdDbDatabase::setXCLIPFRAME()
  </table> 

<group TD_SysVars_X>
  */
//<dom_var_def>OdInt16 XCLIPFRAME;
/* This variable was bool (with initial value "false") in previous version */
DICR24VAR_H( OdInt16,               XCLIPFRAME,          (2),                 (2),                          (),                         ValidateRange(0,2))

/*** VARDOCS:
  <properties>
    <remarks>
      "\n    MESHTYPE Controls the type of mesh that is created by REVSURF, TABSURF, RULESURF and EDGESURF"
      "\n"
      "\n    @table"
      "\n    Value   Description"
      "\n    0       Creates legacy polygon or polyface mesh when you use REVSURF, TABSURF, RULESURF, or EDGESURF"
      "\n    1       Creates full-featured mesh objects when you use REVSURF, TABSURF, RULESURF, or EDGESURF (recommended)"
    </remarks>
  </properties>
*/
DICR24VAR( OdInt16,               MESHTYPE,          (1),                 (1),                          (),                         ValidateRange(0,1))

/*** VARDOCS:
  <properties>
    <remarks>
      "\n    SKYSTATUS Determines if the sky illumination is computed at render time"
      "\n"
      "\n    @table"
      "\n    Value   Description"
      "\n    0       Sky off"
      "\n    1       Sky background"
      "\n    2       Sky background and illumination"
    </remarks>
  </properties>
*/
DICR24VAR( OdInt16,               SKYSTATUS,         (0),                 (0),                          (),                         ValidateRange(0,2))

//-----------------------------------------
// Variables added in AC 2011
//-----------------------------------------

/**
  <title HPLAYER>
  <toctitle HPLAYER>
   
  <table>
   <b>Type</b>            OdString
   <b>Initial value</b>   "."
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2011+
  </table>
   
  HPLAYER stores the default layer of new hatches and fills.
   
  <b>Note: </b>If is equal to "." then CLAYER is used for new hatch entities.
  
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getHPLAYER()
   OdDbDatabase::setHPLAYER()
  </table> 

<group TD_SysVars_H>
  */
//<dom_var_def>OdDbObjectId HPLAYER;
DICR27VAR(OdString, HPLAYER,    (OdString(OD_T("."))),    (OdString(OD_T("."))),            (), ValidateNone())

DICR27VAR(OdInt16,                 MIRRHATCH,     (0),                            (0),                          (),                         ValidateRange(0,1))

/**
  <title HPTRANSPARENCY>
  <toctitle HPTRANSPARENCY>
   
  <table>
   <b>Type</b>            OdCmTransparency
   <b>Initial value</b>   OdCmTransparency::kErrorValue (Use CETRANSPARENCY for new Hatch entities)
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        R13+
  </table>
   
  HPTRANSPARENCY stores information about the transparency of new hatches and fills.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getHPTRANSPARENCY()
   OdDbDatabase::setHPTRANSPARENCY()
  </table> 

<group TD_SysVars_H>
  */
//<dom_var_def>OdCmTransparency HPTRANSPARENCY;
DICR27VAR(ODTTRANSPARENCYDEF, HPTRANSPARENCY, ((const OdCmTransparency&) OdCmTransparency(OdCmTransparency::kErrorValue)), ((const OdCmTransparency&) OdCmTransparency(OdCmTransparency::kErrorValue)), (), ValidateNone())

/**
  <title HPCOLOR>
  <toctitle HPCOLOR>
   
  <table>
   <b>Type</b>            OdCmColor
   <b>Initial value</b>   OdCmEntityColor::kNone
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2011+
  </table>
   
  HPTRANSPARENCY stores the default color of hatches.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getHPCOLOR()
   OdDbDatabase::setHPCOLOR()
  </table> 

<group TD_SysVars_H>
*/
DICR27VAR(ODTCOLORDEF, HPCOLOR, ((const OdCmColor&) OdCmColor(OdCmEntityColor::kNone)), ((const OdCmColor&) OdCmColor(OdCmEntityColor::kNone)), (), ValidateNone())

/**
  <title HPBACKGROUNDCOLOR>
  <toctitle HPBACKGROUNDCOLOR>
   
  <table>
   <b>Type</b>            OdCmColor
   <b>Initial value</b>   OdCmEntityColor::kNone
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2011+
  </table>
   
  HPTRANSPARENCY stores the default background color of hatch patterns.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getHPBACKGROUNDCOLOR()
   OdDbDatabase::setHPBACKGROUNDCOLOR()
  </table> 

<group TD_SysVars_H>
*/
DICR27VAR(ODTCOLORDEF, HPBACKGROUNDCOLOR, ((const OdCmColor&) OdCmColor(OdCmEntityColor::kNone)), ((const OdCmColor&) OdCmColor(OdCmEntityColor::kNone)), (), ValidateNone())

//-----------------------------------------
// Variables added in AC 2013
//-----------------------------------------
RO_VAR_DEF(   OdInt64,          REQUIREDVERSIONS,       (0),                 (0),                           (),                         ValidateNone())

/**
  <title CETRANSPARENCY>
  <toctitle CETRANSPARENCY>
   
  <table>
   <b>Type</b>            OdString | OdCmTransparency
   <b>Initial value</b>   "byLayer" | OdCmTransparency::kByLayer
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2011+
  </table>
   
  The CETRANSPARENCY variable specifies a default transparency for new entities (excluding hatches, 
  see HPTRANSPARENCY variable).
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getCETRANSPARENCY()
   OdDbDatabase::setCETRANSPARENCY()
  </table>
   
  For example, to get the value:
  <code>
   OdRusBufPtr pRb = pDb-\>getSysVar(L"CETRANSPARENCY");
   OdString sValue = pRb-\>getString();
   odPrintConsoleString(L"nTransparency value = %s", sValue.c_str());
   // or //
   OdCmTransparency clarity = pDb-\>getCETRANSPARENCY()
   odPrintConsoleString(L"nTransparency value = %x", clarity.serializeOut());
  </code>
  For example, to set the value:
  <code>
   OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtString, L"50");
   pDb-\>setSysVar(L"CETRANSPARENCY", pRb);
   // or //
   OdCmTransparency clarity(50);
   pDb-\>setCETRANSPARENCY(clarity);
  </code>

<group TD_SysVars_C>
  */
//<dom_var_def>OdCmTransparency CETRANSPARENCY;
DICR27VAR(OdCmTransparency, CETRANSPARENCY, ((const OdCmTransparency&) OdCmTransparency()), ((const OdCmTransparency&) OdCmTransparency()), (), ValidateTransparency())

/**
  <title CVIEWDETAILSTYLE>
  <toctitle CVIEWDETAILSTYLE>
   
  <table>
   <b>Type</b>            OdDbObjectId
   <b>Initial value</b>   OdDbObjectId::kNull
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2013+
  </table>
   
  The CVIEWDETAILSTYLE variable controls the current detail view style.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getCVIEWDETAILSTYLE()
   OdDbDatabase::setCVIEWDETAILSTYLE()
  </table> 

<group TD_SysVars_C>
  */
//<dom_var_def>OdDbObjectId CVIEWDETAILSTYLE;
DICR27VAR(ODTVIEWDETAILSTYLEID, CVIEWDETAILSTYLE, (OdDbObjectId::kNull), (OdDbObjectId::kNull), (), ValidateViewDetailStyle())

/**
  <title CVIEWSECTIONSTYLE>
  <toctitle CVIEWSECTIONSTYLE>
   
  <table>
   <b>Type</b>            OdDbObjectId
   <b>Initial value</b>   OdDbObjectId::kNull
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2013+
  </table>
   
  The CVIEWSECTIONSTYLE variable controls the current section view style.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getCVIEWSECTIONSTYLE()
   OdDbDatabase::setCVIEWSECTIONSTYLE()
  </table> 

<group TD_SysVars_C>
  */
//<dom_var_def>OdDbObjectId CVIEWSECTIONSTYLE;
DICR27VAR(ODTVIEWSECTIONSTYLEID, CVIEWSECTIONSTYLE, (OdDbObjectId::kNull), (OdDbObjectId::kNull), (), ValidateViewSectionStyle())

/**
  <title WIPEOUTFRAME>
  <toctitle WIPEOUTFRAME>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   1
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2013+
  </table>
   
  WIPEOUTFRAME controls the visibility of wipeout object frames.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Invisible
   1              Is displayed and plotted
   2              Is displayed but not plotted
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getWIPEOUTFRAME()
   OdDbDatabase::setWIPEOUTFRAME()
  </table>  

<group TD_SysVars_W>
  */
//<dom_var_def>OdInt16 WIPEOUTFRAME;
DICR27VAR( OdInt16,              WIPEOUTFRAME,          (1),                 (1),                          (),                         ValidateRange(0,2))

/**
  <title POINTCLOUDCLIPFRAME>
  <toctitle POINTCLOUDCLIPFRAME>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   2
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2013+
  </table>
   
  POINTCLOUDCLIPFRAME controls the visibility of point cloud object clipping boundary.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Invisible
   1              Is displayed and plotted
   2              Is displayed but not plotted
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getPOINTCLOUDCLIPFRAME()
   OdDbDatabase::setPOINTCLOUDCLIPFRAME()
  </table>  

<group TD_SysVars_P>
  */
//<dom_var_def>OdInt16 POINTCLOUDCLIPFRAME;
DICR27VAR( OdInt16,              POINTCLOUDCLIPFRAME,   (2),                 (2),                          (),                         ValidateRange(0,2))

/**
  <title MLEADERSCALE>
  <toctitle MLEADERSCALE>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   1.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2009+
  </table>
   
  The MLEADERSCALE variable specifies the scale factor applied to multileaders. The scale factor 
  affects the size of text, arrowheads, and other items that can be scaled, but does not
  affect measurements of lengths, angles, and coordinates.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getMLEADERSCALE()
   OdDbDatabase::setMLEADERSCALE()
  </table>
   
  For example, to get the value:
  <code>
   OdRusBufPtr pRb = pDb-\>getSysVar(L"MLEADERSCALE");
   double scale = pRb-\>getDouble();
   odPrintConsoleString(L"nOverall scale factor = %g", scale);
   // or //
   odPrintConsoleString(L"nOverall scale factor = %g ", pDb-\>getMLEADERSCALE());
  </code>
  For example, to set the value:
  <code>
   OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtDouble, 0.75);
   pDb-\>setSysVar(L"MLEADERSCALE", pRb);
   // or //
   pDb-\>setMLEADERSCALE(1.5);
  </code>

  <group TD_SysVars_M>
  */
//<dom_var_def>double MLEADERSCALE;
DICR27VAR( double,               MLEADERSCALE,          (1.0),               (1.0),                        (),                         ValidateMeaderScale())

/**
  <title DYNCONSTRAINTDISPLAY>
  <toctitle DYNCONSTRAINTDISPLAY>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   false
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2010+
  </table>
   
  The DYNCONSTRAINTDISPLAY variable specifies the visibility of dynamic constraints.
   
  <table>
   <b>Value</b>   <b>Description</b>
   false          Hides dynamic constraints.
   true           Shows dynamic constraints.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getDYNCONSTRAINTDISPLAY()
   OdDbDatabase::setDYNCONSTRAINTDISPLAY()
  </table>
   
  For example, to get the value:
  <code>
   OdRusBufPtr pRb = pDb-\>getSysVar(L"DYNCONSTRAINTDISPLAY");
   OdBool bView = pRb-\>getBool();
   odPrintConsoleString(L"nDynamic constraint visibility is %s", ((bView) ? "shown" : "hidden"));
   // or //
   odPrintConsoleString(L"nDynamic constraint visibility = %d", (int)pDb-\>getDYNCONSTRAINTDISPLAY());
  </code>
  For example, to set the value:
  <code>
   OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtBool, false);
   pDb-\>setSysVar(L"DYNCONSTRAINTDISPLAY", pRb);
   // or //
   pDb-\>setDYNCONSTRAINTDISPLAY(true);
  </code>

<group TD_SysVars_D>
  */
//<dom_var_def>bool DYNCONSTRAINTDISPLAY;
VAR_DEF(   ODTBOOL,                DYNCONSTRAINTDISPLAY,    (false),                        (false),                          (),                         ValidateBOOL())

/** 
  <title VIEWUPDATEAUTO>
  <toctitle VIEWUPDATEAUTO>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   1
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2013+
  </table>
   
  The VIEWUPDATEAUTO variable specifies whether model views are automatically updated when the model 
  is changed. The value can be:
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Views are not updated.
   1              Views are automatically updated.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getVIEWUPDATEAUTO()
   OdDbDatabase::setVIEWUPDATEAUTO()
  </table>
   
  For example, to get the value:
  <code>
   OdRusBufPtr pRb = pDb-\>getSysVar(L"VIEWUPDATEAUTO");
   OdInt16 viewupdate = pRb-\>getInt16();
   odPrintConsoleString(L"nView update mode is %s", ((viewupdate) ? "ON" : "OFF"));
   // or //
   odPrintConsoleString(L"nView update mode = %d", pDb-\>getVIEWUPDATEAUTO());
  </code>
  For example, to set the value:
  <code>
   OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtInt16, 1);
   pDb-\>setSysVar(L"VIEWUPDATEAUTO", pRb);
   // or //
   pDb-\>setVIEWUPDATEAUTO(1);
  </code>

  <group TD_SysVars_V>
*/
//<dom_var_def>OdInt16 VIEWUPDATEAUTO;
DICR27VAR(OdInt16,                VIEWUPDATEAUTO,            (1),                     (1),                              (),                         ValidateRange(0,1))

/**
  <title VSACURVATUREHIGH>
  <toctitle VSACURVATUREHIGH>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   1.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2013+
  </table>
   
  The VSACURVATUREHIGH variable stores a high value at which a surface displays as green tint when curvature 
  analysis is performed.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getVSACURVATUREHIGH()
   OdDbDatabase::setVSACURVATUREHIGH()
  </table>
   
  For example, to get the value:
  <code>
   OdRusBufPtr pRb = pDb-\>getSysVar(L"VSACURVATUREHIGH");
   double value = pRb-\>getDouble();
   odPrintConsoleString(L"nCurvatture high = %g", value);
   // or //
   odPrintConsoleString(L"nCurvatture high = %g", pDb-\>getVSACURVATUREHIGH());
  </code>
  For example, to set the value:
  <code>
   OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtDouble, 0.8);
   pDb-\>setSysVar(L"VSACURVATUREHIGH", pRb);
   // or //
   pDb-\>setVSACURVATUREHIGH(0.5);
  </code>

  <group TD_SysVars_V>
  */
//<dom_var_def>double VSACURVATUREHIGH;
DICR24VAR(ODTDIST,                VSACURVATUREHIGH,          (1.),                    (1.),                             (),                         ValidateNone())

/**
  <title VSACURVATURELOW>
  <toctitle VSACURVATURELOW>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   (–1.0)
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2013+
  </table>
  
  The VSACURVATURELOW variable stores a low value at which a surface displays as blue tint when 
  curvature analysis is performed.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getVSACURVATURELOW()
   OdDbDatabase::setVSACURVATURELOW()
  </table>
   
  For example, to get the value:
  <code>
   OdRusBufPtr pRb = pDb-\>getSysVar(L"VSACURVATURELOW");
   double value = pRb-\>getDouble();
   odPrintConsoleString(L"nCurvatture low = %g", value);
   // or //
   odPrintConsoleString(L"nCurvatture low = %g", pDb-\>getVSACURVATURELOW());
  </code>
  For example, to set the value:
  <code>
   OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtDouble, -0.8);
   pDb-\>setSysVar(L"VSACURVATURELOW", pRb);
   // or //
   pDb-\>setVSACURVATURELOW(-0.5);
  </code>

<group TD_SysVars_V>
  */
//<dom_var_def>double VSACURVATURELOW;
DICR24VAR(ODTDIST,                VSACURVATURELOW,           (-1.),                   (-1.),                            (),                         ValidateNone())

/**
  <title VSACURVATURETYPE>
  <toctitle VSACURVATURETYPE>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2013+
  </table>
  
  The VSACURVATURETYPE variable stores the type of curvature analysis which is applied to surfaces. 
  The value can be:
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Gaussian curvature type – calculates the area of high and low curvature values.
   1              Mean curvature – calculates the mean of the U and V surface curvature values.
   2              Maximum curvature – calculates the maximum of the U and V surface curvature values.
   3              Minimum curvature – calculates the minimum of the U and V surface curvature values.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getVSACURVATURETYPE()
   OdDbDatabase::setVSACURVATURETYPE()
  </table>
   
  For example, to get the value:
  <code>
   OdRusBufPtr pRb = pDb-\>getSysVar(L"VSACURVATURETYPE");
   OdInt16 type = pRb-\>getInt16();
   odPrintConsoleString(L"nCurvatture analysis type = %d", type);
   // or //
   odPrintConsoleString(L"nCurvatture analysis type = %d", pDb-\>getVSACURVATURETYPE());
  </code>
  For example, to set the value:
  <code>
   OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtInt16, 2);
   pDb-\>setSysVar(L"VSACURVATURETYPE", pRb);
   // or //
   pDb-\>setVSACURVATURETYPE(3);
  </code>

  <group TD_SysVars_V>
 */
//<dom_var_def>OdInt16 VSACURVATURETYPE;
DICR24VAR(OdInt16,                VSACURVATURETYPE,          (0),                     (0),                              (),                         ValidateRange(0,3))

/**
  <title VSADRAFTANGLEHIGH>
  <toctitle VSADRAFTANGLEHIGH>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   3.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2013+
  </table>
   
  The VSADRAFTANGLEHIGH variable stores a high angle value at which a surface displays as green tint 
  when draft analysis is performed. The draft angle defines an angle between the surface normal and 
  UCS plane and is measured in the range –90 to 90 degrees.
   
  <table>
   <b>Value</b>   <b>Description</b>
   –90            Opposite to the construction plane.
   0.0            Perpendicular to the construction plane.
   90.0           Parallel to the construction plane.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getVSADRAFTANGLEHIGH()
   OdDbDatabase::setVSADRAFTANGLEHIGH()
  </table>
   
  For example, to get the value:
  <code>
   OdRusBufPtr pRb = pDb-\>getSysVar(L"VSADRAFTANGLEHIGH");
   double angle = pRb-\>getDouble();
   odPrintConsoleString(L"nHigh draft angle = %g", angle);
   // or //
   odPrintConsoleString(L"nHigh draft angle = %g", pDb-\>getVSACURVATUREHIGH());
  </code>
  For example, to set the value:
  <code>
   OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtDouble, 6.5);
   pDb-\>setSysVar(L"VSADRAFTANGLEHIGH", pRb);
   // or //
   pDb-\>setVSADRAFTANGLEHIGH(8.0);
  </code>

<group TD_SysVars_V>
  */
//<dom_var_def>double VSADRAFTANGLEHIGH;
DICR24VAR(ODTDIST,                VSADRAFTANGLEHIGH,         (3.),                    (3.),                             (),                         ValidateDouble(-90.,90.))

/**
  <title VSADRAFTANGLELOW>
  <toctitle VSADRAFTANGLELOW>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   (–3.0)
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2013+
  </table>
   
  The VSADRAFTANGLELOW variable stores a low angle value at which a surface displays as blue 
  tint when draft analysis is performed. The draft angle defines an angle between the surface normal 
  and UCS plane and is measured in the range –90 to 90 degrees.
   
  <table>
   <b>Value</b>   <b>Description</b>
   –90            Opposite to the construction plane.
   0.0            Perpendicular to the construction plane.
   90.0           Parallel to the construction plane.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getVSADRAFTANGLELOW()
   OdDbDatabase::setVSADRAFTANGLELOW()
  </table>
   
  For example, to get the value:
  <code>
   OdRusBufPtr pRb = pDb-\>getSysVar(L"VSADRAFTANGLELOW");
   double angle = pRb-\>getDouble();
   odPrintConsoleString(L"nLow draft angle = %g", angle);
   // or //
   odPrintConsoleString(L"nLow draft angle = %g", pDb-\>getVSADRAFTANGLELOW());
  </code>
  For example, to set the value:
  <code>
   OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtDouble, â€“4.5);
   pDb-\>setSysVar(L"VSADRAFTANGLELOW", pRb);
   // or //
   pDb-\>setVSADRAFTANGLELOW(â€“5.0);
  </code>  

<group TD_SysVars_V>
  */
//<dom_var_def>double VSADRAFTANGLELOW;
DICR24VAR(ODTDIST,                VSADRAFTANGLELOW,          (-3.),                   (-3.),                            (),                         ValidateDouble(-90.,90.))

/**
  <title VSAZEBRACOLOR1>
  <toctitle VSAZEBRACOLOR1>
   
  <table>
   <b>Type</b>            OdString
   <b>Initial value</b>   "RGB:255,255,255"
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2013+
  </table>
   
  The VSAZEBRACOLOR1 variable stores the first tint of zebra streaks which are displayed when zebra 
  analysis is performed. TrueColor value is set as follows: "RGB:XXX,XXX,XXX", where XXX is the color 
  component value in the range 0 to 255. Value 0 defines a color ByBlock, value 256 defines a color ByLayer,
  values 1 to 255 define the ACI color.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getVSAZEBRACOLOR1()
   OdDbDatabase::setVSAZEBRACOLOR1()
  </table>
   
  For example, to get the value:
  <code>
   OdRusBufPtr pRb = pDb-\>getSysVar(L"VSAZEBRACOLOR1");
   OdString tint = pRb-\>getString();
   odPrintConsoleString(L"nFirst zebra streak tint = %s", tint);
   // or //
   odPrintConsoleString(L"nFirst zebra streak tint = %s", pDb-\>getVSAZEBRACOLOR1());
  </code>
  For example, to set the value:
  <code>
   OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtString, L"RGB:128,128,128");
   pDb-\>setSysVar(L"VSAZEBRACOLOR1", pRb);
   // or //
   pDb-\>setVSAZEBRACOLOR1("256");
  </code>

  <group TD_SysVars_V>
  */
//<dom_var_def>OdString VSAZEBRACOLOR1;
DICR24VAR(OdString,               VSAZEBRACOLOR1,            (OdString(OD_T("RGB:255,255,255"))),(OdString(OD_T("RGB:255,255,255"))),(),            ValidateNone())

/**
  <title VSAZEBRACOLOR2>
  <toctitle VSAZEBRACOLOR2>
   
  <table>
   <b>Type</b>            OdString
   <b>Initial value</b>   "RGB:0,0,0"
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2013+
  </table>
   
  The VSAZEBRACOLOR2 variable stores the second tint of zebra streaks which are displayed when zebra analysis 
  is performed. TrueColor value is set as follows: "RGB:XXX,XXX,XXX", where XXX is the color component value 
  in the range 0 to 255. Value 0 defines a color ByBlock, value 256 defines a color ByLayer, values 1 to 255 
  define the ACI color.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getVSAZEBRACOLOR2()
   OdDbDatabase::setVSAZEBRACOLOR2()
  </table>
   
  For example, to get the value:
  <code>
   OdRusBufPtr pRb = pDb-\>getSysVar(L"VSAZEBRACOLOR2");
   OdString tint = pRb-\>getString();
   odPrintConsoleString(L"nSecond zebra streak tint = %s", tint);
   // or //
   odPrintConsoleString(L"nSecond zebra streak tint = %s", pDb-\>getVSAZEBRACOLOR2());
  </code>
  For example, to set the value:
  <code>
   OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtString, L"RGB:128,128,128");
   pDb-\>setSysVar(L"VSAZEBRACOLOR2", pRb);
   // or //
   pDb-\>setVSAZEBRACOLOR2("256");
  </code>

  <group TD_SysVars_V>
  */
//<dom_var_def>OdString VSAZEBRACOLOR2;
DICR24VAR(OdString,               VSAZEBRACOLOR2,            (OdString(OD_T("RGB:0,0,0"))),(OdString(OD_T("RGB:0,0,0"))),(),                        ValidateNone())

/**
  <title VSAZEBRADIRECTION>
  <toctitle VSAZEBRADIRECTION>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   90
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2013+
  </table>
   
  The VSAZEBRADIRECTION variable stores the direction angle of zebra streaks which are displayed when zebra 
  analysis is performed. The value can be in the range 0 to 90 degrees. The value 0 defines horizontal 
  streaks. The value 90 defines vertical streaks.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getVSAZEBRADIRECTION()
   OdDbDatabase::setVSAZEBRADIRECTION()
  </table>
   
  For example, to get the value:
  <code>
   OdRusBufPtr pRb = pDb-\>getSysVar(L"VSAZEBRADIRECTION");
   OdInt16 angle = pRb-\>getInt16();
   odPrintConsoleString(L"nZebra streak angle = %d", angle);
   // or //
   odPrintConsoleString(L"nZebra streak angle = %d", pDb-\>getVSAZEBRADIRECTION());
  </code>
  For example, to set the value:
  <code>
   OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtInt16, 30);
   pDb-\>setSysVar(L"VSAZEBRADIRECTION", pRb);
   // or //
   pDb-\>setVSAZEBRADIRECTION(60);
  </code>

<group TD_SysVars_V>
  */
//<dom_var_def>OdInt16 VSAZEBRADIRECTION;
DICR24VAR(OdInt16,                VSAZEBRADIRECTION,         (90),                    (90),                             (),                         ValidateRange(0,90))

/**
  <title VSAZEBRASIZE>
  <toctitle VSAZEBRASIZE>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   45
   <b>Range</b>           –32768 ... 32767
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2013+
  </table>
   
  The VSAZEBRASIZE variable stores the width of the zebra streaks which are displayed when zebra analysis 
  is performed. The value can be in the range 1 to 100. 
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getVSAZEBRASIZE()
   OdDbDatabase::setVSAZEBRASIZE()
  </table>
   
  For example, to get the value:
  <code>
   OdRusBufPtr pRb = pDb-\>getSysVar(L"VSAZEBRASIZE");
   OdInt16 width = pRb-\>getInt16();
   odPrintConsoleString(L"nZebra streak width = %d", width);
   // or //
   odPrintConsoleString(L"nZebra streak width = %d", pDb-\>getVSAZEBRASIZE());
  </code>
  For example, to set the value:
  <code>
   OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtInt16, 30);
   pDb-\>setSysVar(L"VSAZEBRASIZE", pRb);
   // or //
   pDb-\>setVSAZEBRASIZE(60);
  </code>

<group TD_SysVars_V>
  */
//<dom_var_def>OdInt16 VSAZEBRASIZE;
DICR24VAR(OdInt16,                VSAZEBRASIZE,              (45),                    (45),                             (),                         ValidateRange(-32768,32767))

/**
  <title VSAZEBRATYPE>
  <toctitle VSAZEBRATYPE>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   1
   <b>Range</b>           0 ... 1
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2013+
  </table>
   
  The VSAZEBRATYPE variable stores the type of zebra analysis which is applied for surfaces. The value can be:
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Chrome Ball
   1              Cylinder
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getVSAZEBRATYPE()
   OdDbDatabase::setVSAZEBRATYPE()
  </table>
   
  For example, to get the value:
  <code>
   OdRusBufPtr pRb = pDb-\>getSysVar(L"VSAZEBRATYPE");
   OdInt16 type = pRb-\>getInt16();
   odPrintConsoleString(L"nZebra analysis type = %d", type);
   // or //
   odPrintConsoleString(L"nZebra analysis type = %d", pDb-\>getVSAZEBRATYPE());
  </code>
  For example, to set the value:
  <code>
   OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtInt16, 1);
   pDb-\>setSysVar(L"VSAZEBRATYPE", pRb);
   // or //
   pDb-\>setVSAZEBRATYPE(1);
  </code>

<group TD_SysVars_V>
  */
//<dom_var_def>OdInt16 VSAZEBRATYPE;
DICR24VAR(OdInt16,                VSAZEBRATYPE,              (1),                     (1),                              (),                         ValidateRange(0,1))


//-----------------------------------------
// Variables added in AC 2014
//-----------------------------------------
DICR27VAR(double,         GEOMARKPOSITIONSIZE,      (1.),         (1.),           (),             ValidateEqGr(0.))

//-----------------------------------------
// Variables added in AC 2015
//-----------------------------------------
DICR27VAR(OdInt16,        POINTCLOUDPOINTSIZE,      (2),          (2),            (),             ValidateRange(1,10))



//-----------------------------------------
// Variables added in AC 2016
//-----------------------------------------

/**
  <title DIMLAYER>
  <toctitle DIMLAYER>

  <table>
   <b>Type</b>            OdString
   <b>Initial value</b>   "."
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2016+
  </table>

  DIMLAYER stores default layer for new dimensions.

  <b>Note: </b>If is equal to "." then CLAYER is used for new dimension entities.

  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getDIMLAYER()
   OdDbDatabase::setDIMLAYER()
  </table>

<group TD_SysVars_D>
  */
//<dom_var_def>OdDbObjectId DIMLAYER;
DICR27VAR(OdString, DIMLAYER,    (OdString(OD_T("."))),    (OdString(OD_T("."))),            (), ValidateNone())/**

/**
  <title SECTIONOFFSETINC>
  <toctitle SECTIONOFFSETINC>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   6.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2016+
  </table>
   
  SECTIONOFFSETINC stores a default value of incrementation or reduction of the section offset.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getSECTIONOFFSETINC()
   OdDbDatabase::setSECTIONOFFSETINC()
  </table> 

<group TD_SysVars_S>
  */
//<dom_var_def>double SECTIONOFFSETINC;
DICR27VAR(ODTDIST,         SECTIONOFFSETINC,       (6.),          (6.),            (),    ValidateNone())

/**
  <title SECTIONTHICKNESSINC>
  <toctitle SECTIONTHICKNESSINC>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   1.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2016+
  </table>
   
  SECTIONTHICKNESSINC stores a default value of incrementation or reduction of slice thickness.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getSECTIONTHICKNESSINC()
   OdDbDatabase::setSECTIONTHICKNESSINC()
  </table>   

<group TD_SysVars_S>
  */
//<dom_var_def>double SECTIONTHICKNESSINC;
DICR27VAR(ODTDIST,         SECTIONTHICKNESSINC,    (1.),          (1.),            (),    ValidateNone())

/**
  <title XREFOVERRIDE>
  <toctitle XREFOVERRIDE>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Database
   <b>Versions</b>        2016+
  </table>
  
  XREFOVERRIDE controls the display of object properties in XREFs.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              XREF entities save settings of their properties.
   1              XREF entities are treated as if all their visual properties are set to ByLayer and depend on the properties of the XREF layers.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbDatabase::getXREFOVERRIDE()
   OdDbDatabase::setXREFOVERRIDE()
  </table>  

<group TD_SysVars_X>
  */
//<dom_var_def>OdInt16 XREFOVERRIDE;
DICR27VAR(OdInt16,         XREFOVERRIDE,           (0),           (0),             (),    ValidateRange(0, 1))


//-----------------------------------------
// Variables added in AC 2017
//-----------------------------------------

DICR27VAR(OdString, CENTERCROSSGAP, (OdString(OD_T("0.05x"))),(OdString(OD_T("0.05x"))),(), ValidateNone())
DICR27VAR(OdString, CENTERCROSSSIZE,(OdString(OD_T("0.1x"))), (OdString(OD_T("0.1x"))), (), ValidateNone())
DICR27VAR(double,   CENTEREXE,      (0.12),                   (3.5),(),                     ValidateNone())
DICR27VAR(OdString, CENTERLAYER,    (OdString(OD_T("."))),    (OdString(OD_T("."))),    (), ValidateNone())
DICR27VAR(double,   CENTERLTSCALE,  (1.0),                    (1.0),                    (), ValidatePositiveNonZero())
DICR27VAR(OdString, CENTERLTYPE,    (OdString(OD_T("CENTER2"))),  (OdString(OD_T("CENTER2"))),    (), ValidateNone())
DICR27VAR(OdString, CENTERLTYPEFILE,(OdString(OD_T("acad.lin"))), (OdString(OD_T("acadiso.lin"))),(), ValidateNone())
DICR27VAR(ODTBOOL,  CENTERMARKEXE,  (true),                   (true),                   (), ValidateBOOL())


#undef SVARS_FUNC_MODIFIER
#define SVARS_FUNC_MODIFIER

///////////////////////////////////////////////////////////////////////
// New Database variables section
// To not break binary compatibility for new variables
// non-virtual functions are generated

// Add new Database variables here



#ifdef DOUNDEF_VAR_DEF
#undef VAR_DEF
#undef DOUNDEF_VAR_DEF
#endif

#ifdef DOUNDEF_RO_VAR_DEF
#undef RO_VAR_DEF
#undef DOUNDEF_RO_VAR_DEF
#endif

#ifdef DOUNDEF_VAR_DEF_H
#undef VAR_DEF_H
#undef DOUNDEF_VAR_DEF_H
#endif

#ifdef DOUNDEF_RO_VAR_DEF_H
#undef RO_VAR_DEF_H
#undef DOUNDEF_RO_VAR_DEF_H
#endif

#ifdef DOUNDEF_DICR15VAR
#undef DICR15VAR
#undef DOUNDEF_DICR15VAR
#endif

#ifdef DOUNDEF_DICR21VAR_H
#undef DICR21VAR_H
#undef DOUNDEF_DICR21VAR_H
#endif

#ifdef DOUNDEF_DICR21VAR
#undef DICR21VAR
#undef DOUNDEF_DICR21VAR
#endif

#ifdef DOUNDEF_DICR24VAR_H
#undef DICR24VAR_H
#undef DOUNDEF_DICR24VAR_H
#endif

#ifdef DOUNDEF_DICR24VAR
#undef DICR24VAR
#undef DOUNDEF_DICR24VAR
#endif

#ifdef DOUNDEF_DICR27VAR_H
#undef DICR27VAR_H
#undef DOUNDEF_DICR27VAR_H
#endif

#ifdef DOUNDEF_DICR27VAR
#undef DICR27VAR
#undef DOUNDEF_DICR27VAR
#endif

#ifdef DOUNDEF_VAR_DEF_2
#undef VAR_DEF_2
#undef DOUNDEF_VAR_DEF_2
#endif

#ifdef DOUNDEF_ANNO_VAR_DEF
#undef ANNO_VAR_DEF
#undef DOUNDEF_ANNO_VAR_DEF
#endif

#ifdef DOUNDEF_MLEADERSTYLE_VAR_DEF
#undef MLEADERSTYLE_VAR_DEF
#undef DOUNDEF_MLEADERSTYLE_VAR_DEF
#endif

#undef SVARS_FUNC_MODIFIER


#ifdef REGVAR_DEF

#define SVARS_FUNC_MODIFIER virtual

#define ODTDB_PROXYIMAGE OdDb::ProxyImage /* {Secret} */

/**
  <title PLOTTRANSPARENCYOVERRIDE>
  <toctitle PLOTTRANSPARENCYOVERRIDE>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   1
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        2011+
  </table>
   
  PLOTTRANSPARENCYOVERRIDE controls plotting of object transparency.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Do not plot object transparency.
   1              Use settings from page.
   2              Plot object transparency.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbHostAppServices::getPLOTTRANSPARENCYOVERRIDE()
   OdDbHostAppServices::setPLOTTRANSPARENCYOVERRIDE() 
  </table> 

<group TD_SysVars_P>
  */
//<dom_var_def>OdUInt16 PLOTTRANSPARENCYOVERRIDE;
REGVAR_DEF(OdUInt16,          PLOTTRANSPARENCYOVERRIDE,   (1),   (),      ValidateRange(0, 2))

/**
  <title ATTREQ>
  <toctitle ATTREQ>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   true
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        9+
  </table>
   
  ATTREQ controls the requesting of attribute values during the INSERT command.
   
  <table>
   <b>Value</b>   <b>Description</b>
   false          Attributes values assume their defaults.
   true           Attributes values are requested.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbHostAppServices::getATTREQ()
   OdDbHostAppServices::setATTREQ()
  </table>
  
  \sa
  <link ATTDIA, ATTDIA>

<group TD_SysVars_A>
   */
//<dom_var_def>bool ATTREQ;
REGVAR_DEF(ODTBOOL,             ATTREQ,         (true),               (),       ValidateBOOL())

/**
  <title ATTDIA>
  <toctitle ATTDIA>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   false
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        9+
  </table>
   
  ATTDIA specifies if attribute values are requested via a dialog box during the INSERT command.
   
  <table>
   <b>Value</b>   <b>Description</b>
   false          Attributes are requested via the command prompt.
   true           Attributes are requested via a dialog box.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbHostAppServices::getATTDIA()
   OdDbHostAppServices::setATTDIA()
  </table>
   
  <b>Note: </b>Attributes are requested if and only if ATTREQ == true.
  
  \sa
  <link ATTREQ, ATTREQ>

<group TD_SysVars_A>
   */
//<dom_var_def>bool ATTDIA;
REGVAR_DEF(ODTBOOL,             ATTDIA,         (false),              (),       ValidateBOOL())

/**
  <title BLIPMODE>
  <toctitle BLIPMODE>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   false
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        2.5+
  </table>
   
  BLIPMODE controls the display of marker blips.
   
  <table>
   <b>Value</b>   <b>Description</b>
   false          Blips are not displayed.
   true           Blips are displayed.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbHostAppServices::getBLIPMODE()
   OdDbHostAppServices::setBLIPMODE()
  </table>  

<group TD_SysVars_B>
  */
//<dom_var_def>bool BLIPMODE;
REGVAR_DEF(ODTBOOL,             BLIPMODE,       (false),              (),       ValidateBOOL())

/**
  <title DELOBJ>
  <toctitle DELOBJ>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   true
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        13+
  </table>
   
  DELOBJ controls the deletion of objects used to create solid objects and regions. Commands affected are REGION, 
  EXTRUDE, and REVOLVE.
   
  <table>
   <b>Value</b>   <b>Description</b>
   false          Objects are not deleted.
   true           Objects are deleted.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbHostAppServices::getDELOBJ()
   OdDbHostAppServices::setDELOBJ()
  </table> 

<group TD_SysVars_D>
  */
//<dom_var_def>bool DELOBJ;
REGVAR_DEF(ODTBOOL,             DELOBJ,         (true),               (),       ValidateBOOL())

/**
  <title FILEDIA>
  <toctitle FILEDIA>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   true
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        R12+
  </table>
   
  FILEDIA controls the display of file navigation dialog boxes.
   
  <table>
   <b>Value</b>   <b>Description</b>
   false          Command prompt input.
   true           Dialog boxes displayed.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbHostAppServices::getFILEDIA()
   OdDbHostAppServices::setFILEDIA()
  </table> 

<group TD_SysVars_F>
  */
//<dom_var_def>bool FILEDIA;
REGVAR_DEF(ODTBOOL,             FILEDIA,        (true),               (),       ValidateBOOL())

/**
  <title COORDS>
  <toctitle COORDS>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   1
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        2.5+
  </table>
   
  COORDS controls the display of coordinate information in the status bar.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Absolute coordinates updated when points are picked.
   1              Absolute coordinates updated continuously.
   2              Absolute coordinates updated continuously, with polar coordinates displayed when appropriate.
   3              Geographic coordinates updated continuously.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbHostAppServices::getCOORDS()
   OdDbHostAppServices::setCOORDS()
  </table>   

<group TD_SysVars_C>
  */
//<dom_var_def>OdInt16 COORDS;
REGVAR_DEF(OdInt16,             COORDS,         (1),                  (),       ValidateRange(0,3))

/**
  <title DRAGMODE>
  <toctitle DRAGMODE>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   2
   <b>Range</b>           0 ... 2
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        R12+
  </table>
   
  DRAGMODE controls the display of dragged objects. This includes both objects being moved from place to place, as
  well as objects being created; e.g., Circles.
   
  <table>
   <b>Value</b>   <b>Mode</b>   <b>Description</b>
   0              Off           Never displays dragged objects.
   1              On            Displays dragged objects if and only if "drag" is entered at the command line while dragging.
   2              Auto          Always displays dragged objects.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbHostAppServices::getDRAGMODE()
   OdDbHostAppServices::setDRAGMODE()
  </table>  

<group TD_SysVars_D>
  */
//<dom_var_def>OdInt16 DRAGMODE;
REGVAR_DEF(OdInt16,             DRAGMODE,       (2),                  (),       ValidateRange(0,2))

/**
  <title OSMODE>
  <toctitle OSMODE>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   0x25
   <b>Range</b>           0 ... 32767
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        R12+
  </table>
   
  OSMODE specifies the running object snaps. OSMODE will be a combination of one or more of the following:
   
  <table>
   <b>Value</b>   <b>Description</b>
   0x00           NONe
   0x01           ENDpoint
   0x02           MIDpoint
   0x04           CENter
   0x08           NODe
   0x10           QUAdrant
   0x20           INTersection
   0x40           INSertion
   0x80           PERpendicular
   0x100          TANgent
   0x200          NEArest
   0x400          Clears all object snaps
   0x800          APParent intersection
   0x1000         EXTension
   0x2000         PARallel
   0x4000         Disable all
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbHostAppServices::getOSMODE()
   OdDbHostAppServices::setOSMODE()
  </table> 

<group TD_SysVars_O>
  */
//<dom_var_def>OdInt16 OSMODE;
REGVAR_DEF(OdInt16,             OSMODE,         (37),                 (),       ValidateRange(0,32767))

/**
  <title PICKFIRST>
  <toctitle PICKFIRST>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   true
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        R12+
  </table>
   
  PICKFIRST controls noun-verb object selection.
   
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
   OdDbHostAppServices::getPICKFIRST()
   OdDbHostAppServices::setPICKFIRST()
  </table>  

<group TD_SysVars_P>
*/
//<dom_var_def>bool PICKFIRST;
REGVAR_DEF(ODTBOOL,             PICKFIRST,      (1),                  (),       ValidateBOOL())

/**
  <title PICKBOX>
  <toctitle PICKBOX>
  
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   3
   <b>Range</b>           0 ... 50
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        R12+
  </table>
   
  PICKBOX specifies the object selection target height, in pixels.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbHostAppServices::getPICKBOX()
   OdDbHostAppServices::setPICKBOX()
  </table>   

<group TD_SysVars_P>
  */
//<dom_var_def>OdInt16 PICKBOX;
REGVAR_DEF(OdInt16,             PICKBOX,        (3),                  (),       ValidateRange(0,50))

/*** VARDOCS:
  <properties>
    <remarks>
      "\n    APERTURE specifies the object snapping target height, in pixels."
    </remarks>
  </properties>
*/
REGVAR_DEF(OdInt16,             APERTURE,       (10),                 (),       ValidateRange(1,50))

/**
  <title PICKADD>
  <toctitle PICKADD>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   2
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        R12+
  </table>
   
  The PICKADD variable controls whether subsequent selections replace the current selection set or add to it; 
  the variable takes the following values:
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              The objects and subobjects most recently selected become
                   the selection set. Previously selected objects and
                   subobjects are removed from the selection set. Add more
                   objects or subobjects to the selection set by pressing
                   SHIFT while selecting.
   1              Each object and subobject selected, either individually or
                   by windowing, is added to the current selection set. To
                   remove objects or subobjects from the selection set, press
                   the SHIFT key while selecting.
   2              Each object and subobject selected, either individually or
                   by windowing, is added to the current selection set and the
                   objects remains selected after the SELECT command. To
                   remove objects or subobjects from the selection set, press
                   the SHIFT key while selecting.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbHostAppServices::getPICKADD()
   OdDbHostAppServices::setPICKADD()
  </table>
   
  For example, to get the value:
  <code>
   OdRusBufPtr pRb = pDb-\>getSysVar(L"PICKADD");
   OdInt16 mode = pRb-\>getInt16();
   odPrintConsoleString(L"nSelection mode = %d", mode);
   // or //
   odPrintConsoleString(L"nSelection mode = %d", pDb-\>appServices()-\>getPICKADD());
  </code>
  For example, to set the value:
  <code>
   OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtInt16, 1);
   pDb-\>setSysVar(L"PICKADD", pRb);
   // or //
   pDb-\>appServices()-\>setPICKADD(1);
  </code>

<group TD_SysVars_P>
  */
//<dom_var_def>OdInt16 PICKADD;
REGVAR_DEF(OdInt16,             PICKADD,        (2),                  (),       ValidateRange(0,2))

/**
  <title PICKSTYLE>
  <toctitle PICKSTYLE>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   1
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        R13+
  </table>
   
  PICKSTYLE controls group and associative hatch selection.
   
  <table>
   <b>Value</b>   <b>Group</b>   <b>Associative Hatch</b>
   0              Disabled       Disabled
   1              Enabled        Disabled
   2              Disabled       Enabled
   3              Enabled        Enabled
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbHostAppServices::getPICKSTYLE()
   OdDbHostAppServices::setPICKSTYLE()
  </table>

<group TD_SysVars_P>
  */
//<dom_var_def>OdInt16 PICKSTYLE;
REGVAR_DEF(OdInt16,             PICKSTYLE,      (1),                  (),       ValidateRange(0,3))

// Range (0.1 - 1.0) lineweight setting slider

/**
  <title LWDISPSCALE>
  <toctitle LWDISPSCALE>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   0.55
   <b>Range</b>           0.1 ... 1.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        N/A
  </table>
   
  LWDISPSCALE emulates the lineweight setting slider.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbHostAppServices::getLWDISPSCALE()
   OdDbHostAppServices::setLWDISPSCALE()
  </table>   

<group TD_SysVars_L>
  */
//<dom_var_def>double LWDISPSCALE;
REGVAR_DEF(ODTUNITLESS4,        LWDISPSCALE,    (0.55),               (),       ValidateRange(0.1, 1.))

/**
  <title LWDEFAULT>
  <toctitle LWDEFAULT>
   
  <table>
   <b>Type</b>            OdDb::LineWeight
   <b>Initial value</b>   OdDb::kLnWt025
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        2000+
  </table>
   
  The LWDEFAULT variable stores the actual value used to display entities having "default" lineweight. You should
  specify all values in hundredths of millimeters subject to the OdDb::LineWeight enumeration.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbHostAppServices::getLWDEFAULT() &#8211; gets the default lineweight as an integer (enumeration) value
   OdDbHostAppServices::setLWDEFAULT() &#8211; sets the default lineweight as an integer (enumeration) value
  </table>
   
  For example, to get the value:
  <code>
   OdRusBufPtr pRb = pDb-\>getSysVar(L"LWDEFAULT");
   odPrintConsoleString(L"nThe default lineweight = %d", pRb-\>getInt16());
   // or //
   odPrintConsoleString(L"nThe default lineweight = %d", pDb-\>appServices()-\>getLWDEFAULT());
  </code>
  For example, to set the value:
  <code>
   OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtInt16, OdDb::kLnWt040);
   pDb-\>setSysVar(L"LWDEFAULT", pRb);
   // or //
   pDb-\>appServices()-\>setLWDEFAULT(OdDb::kLnWt080);
  </code>

  <group TD_SysVars_L>
  */
//<dom_var_def>OdDb::LineWeight LWDEFAULT;
REGVAR_DEF(ODTDB_LINEWEIGHT,    LWDEFAULT,      (OdDb::kLnWt025),     (),       ValidateLineWeight(true))

/**
  <title FONTALT>
  <toctitle FONTALT>
   
  <table>
   <b>Type</b>            OdString
   <b>Initial value</b>   "simplex.shx"
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        R13+
  </table>
   
  FONTALT specifies the filename of the font to be used when the font specified by a text style cannot be found.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;  
   OdDbHostAppServices::getFONTALT()
   OdDbHostAppServices::setFONTALT()
  </table>   

<group TD_SysVars_F>
  */
//<dom_var_def>OdString FONTALT;
REGVAR_DEF(OdString,            FONTALT,        (OD_T("simplex.shx")),      (),       ValidateNone())

/**
  <title PLINETYPE>
  <toctitle PLINETYPE>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   2
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        R14+
  </table>
   
  PLINETYPE controls polyline creation and conversion from OdDb2dPolyline to OdDbPolyline when 
  a database file is opened.
   
  <table>
   <b>Value</b>   <b>Conversions</b>   <b>New Polylines</b>
   0              Disabled             OdDb2dPolyline
   1              Disabled             OdDbPolyline
   2              Enabled              OdDbPolyline
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;  
   OdDbHostAppServices::getPLINETYPE()
   OdDbHostAppServices::setPLINETYPE()
  </table>

<group TD_SysVars_P>
  */
//<dom_var_def>OdInt16 PLINETYPE;
REGVAR_DEF(OdInt16,             PLINETYPE,      (2),                  (),       ValidateRange(0,2))

/**
  <title PROXYSHOW>
  <toctitle PROXYSHOW>
   
  <table>
   <b>Type</b>            OdDb::ProxyImage
   <b>Initial value</b>   OdDb::kProxyShow
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        R14+
  </table>
   
  PROXYSHOW controls the display of proxy graphics.
   
  <table>
   <b>Name</b>               <b>Value</b>   <b>Description</b>
   OdDb::kProxyNotShow       0              Proxy graphics are not displayed.
   OdDb::kProxyShow          1              Proxy graphics are displayed.
   OdDb::kProxyBoundingBox   2              Proxy bounding boxes are displayed.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;  
   OdDbHostAppServices::getPROXYSHOW()
   OdDbHostAppServices::setPROXYSHOW()
  </table>

<group TD_SysVars_P>
  */
//<dom_var_def>OdDb::ProxyImage PROXYSHOW;
REGVAR_DEF(ODTDB_PROXYIMAGE,    PROXYSHOW,      (OdDb::kProxyShow),   (),       ValidateRange(OdDb::kProxyNotShow, OdDb::kProxyBoundingBox))

/**
  <title TEXTFILL>
  <toctitle TEXTFILL>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   true
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        R13+
  </table>
   
  TEXTFILL controls the filling of TrueType fonts.
   
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
   OdDbHostAppServices::getTEXTFILL()
   OdDbHostAppServices::setTEXTFILL()
  </table>     

<group TD_SysVars_T>
  */
//<dom_var_def>bool TEXTFILL;
REGVAR_DEF(ODTBOOL,             TEXTFILL,       (true),               (),       ValidateBOOL())


// New in Red Deer (2004)

/**
  <title GRIPHOVER>
  <toctitle GRIPHOVER>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   3
   <b>Range</b>           1 ... 255
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        2004+
  </table>
   
  GRIPHOVER specifies the ACI color index of unselected grips when the cursor hovers them.
   
  <table>
   <b>Name</b>      <b>Value</b>   <b>Description</b>
   kACIbyBlock      0              ByBlock
   kACIRed          1              Red
   kACIYellow       2              Yellow
   kACIGreen        3              Green
   kACICyan         4              Cyan
   kACIBlue         5              Blue
   kACIMagenta      6              Magenta
   kACIforeground   7              Foregroundcolor
   ..               8-255          Defined by display device
   kACIbyLayer      256            ByLayer
   kACInone         257            No color
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;  
   OdDbHostAppServices::getGRIPHOVER()
   OdDbHostAppServices::setGRIPHOVER()
  </table>  

  <group TD_SysVars_G>
  */
//<dom_var_def>OdInt16 GRIPHOVER;
REGVAR_DEF(OdInt16,             GRIPHOVER,      (3),                  (),       ValidateRange(1,255))

/**
  <title GRIPOBJLIMIT>
  <toctitle GRIPOBJLIMIT>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   100
   <b>Range</b>           0 ... 32767
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        2004+
  </table>
   
  GRIPOBJLIMIT specifies the number of selected objects beyond which grips are suppressed for a selection set.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;  
   OdDbHostAppServices::getGRIPOBJLIMIT()
   OdDbHostAppServices::setGRIPOBJLIMIT()
  </table>  

<group TD_SysVars_G>
  */
//<dom_var_def>OdInt16 GRIPOBJLIMIT;
REGVAR_DEF(OdInt16,             GRIPOBJLIMIT,   (100),                (),       ValidateRange(0,32767))

/**
  <title GRIPTIPS>
  <toctitle GRIPTIPS>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   1
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        2004+
  </table>
   
  GRIPTIPS controls the display of grip tips for custom objects.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Disabled
   1              Enabled
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;  
   OdDbHostAppServices::getGRIPTIPS()
   OdDbHostAppServices::setGRIPTIPS()
  </table>  

<group TD_SysVars_G>
  */
//<dom_var_def>OdInt16 GRIPTIPS;
REGVAR_DEF(OdInt16,             GRIPTIPS,       (1),                  (),       ValidateRange(0,1))

/**
  <title HPASSOC>
  <toctitle HPASSOC>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   1
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        2004+
  </table>
   
  HPASSOC controls the associativity of new hatch patterns and gradient fills.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Not associative
   1              Associative
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;  
   OdDbHostAppServices::getHPASSOC()
   OdDbHostAppServices::setHPASSOC()
  </table> 

<group TD_SysVars_H>
  */
//<dom_var_def>OdInt16 HPASSOC;
REGVAR_DEF(OdInt16,             HPASSOC,        (1),                  (),       ValidateRange(0,1))

/**
  <title LOCALROOTPREFIX>
  <toctitle LOCALROOTPREFIX>
   
  <table>
   <b>Type</b>            OdString
   <b>Initial value</b>   ""
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        2004+
  </table>
   
  LOCALROOTPREFIX specifies the complete path to the local customization files.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;  
   OdDbHostAppServices::getLOCALROOTPREFIX()
   OdDbHostAppServices::setLOCALROOTPREFIX()
  </table>  

  <group TD_SysVars_L>
  */
//<dom_var_def>OdString LOCALROOTPREFIX;
REGVAR_DEF(OdString,            LOCALROOTPREFIX,(OD_T("")),                 (),       ValidateNone())

/**
  <title LOGFILEMODE>
  <toctitle LOGFILEMODE>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        R14+
  </table>
  
  LOGFILEMODE controls the output of the text window to a log file.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Not written
   1              Written
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;  
   OdDbHostAppServices::getLOGFILEMODE()
   OdDbHostAppServices::setLOGFILEMODE()
  </table>    

<group TD_SysVars_L>
  */
//<dom_var_def>OdInt16 LOGFILEMODE;
REGVAR_DEF(OdInt16,             LOGFILEMODE,    (0),                  (),       ValidateRange(0,1))

/**
  <title MAXHATCHDENSITY>
  <toctitle MAXHATCHDENSITY>
   
  <table>
   <b>Type</b>            OdUInt32
   <b>Initial value</b>   1000000
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        N/A
  </table>
   
  MAXHATCHDENSITY specifies the maximum number of lines in a Hatch entity.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbHostAppServices::getMAXHATCHDENSITY()
   OdDbHostAppServices::setMAXHATCHDENSITY()
  </table>  

<group TD_SysVars_M>
  */
//<dom_var_def>OdUInt32 MAXHATCHDENSITY;
REGVAR_DEF(OdUInt32,            MAXHATCHDENSITY,(1000000),            (),       ValidateNone())

/**
  <title FIELDDISPLAY>
  <toctitle FIELDDISPLAY>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   1
   <b>Range</b>           0 ... 1
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        2005+
  </table>
   
  FIELDDISPLAY controls the display of field backgrounds in text.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              No background
   1              Gray background
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;  
   OdDbHostAppServices::getFIELDDISPLAY()
   OdDbHostAppServices::setFIELDDISPLAY()
  </table>  

<group TD_SysVars_F>
  */
//<dom_var_def>OdInt16 FIELDDISPLAY;
REGVAR_DEF(OdInt16,             FIELDDISPLAY,   (1),                  (),       ValidateRange(0,1))

REGVAR_DEF(OdInt8,              AcisProxyMode,(1),              (),       ValidateRange(0,1))

/**
  <title UCSVIEW>
  <toctitle UCSVIEW>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   1
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        2000+
  </table>
   
  UCSVIEW controls the saving of the current UCS with a named view.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Not saved
   1              Saved
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;  
   OdDbHostAppServices::getUCSVIEW()
   OdDbHostAppServices::setUCSVIEW()
  </table>  

<group TD_SysVars_U>
  */
//<dom_var_def>OdInt16 UCSVIEW;
REGVAR_DEF(OdInt16,             UCSVIEW,        (1),                  (),       ValidateRange(0,1))

/**
  <title GRIPBLOCK>
  <toctitle GRIPBLOCK>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        R12+
  </table>
   
  GRIPBLOCK controls how grips display on a block after you select it. If this option is selected, 
  all grips display for each object in the block. If this option is cleared, one grip
  displays at the insertion point of the block.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Assigns a grip only to the insertion point of the block.
   1              Assigns grips to objects within the block.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;  
   OdDbHostAppServices::getGRIPBLOCK()
   OdDbHostAppServices::setGRIPBLOCK()
  </table>    

<group TD_SysVars_G>
  */
//<dom_var_def>OdInt16 GRIPBLOCK;
REGVAR_DEF(OdInt16,             GRIPBLOCK,      (0),                  (),       ValidateRange(0,1))

/**
  <title GRIPCOLOR>
  <toctitle GRIPCOLOR>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   160
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        R12+
  </table>
   
  GRIPCOLOR controls the color of grips that are not selected. The value can be 1-255.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;  
   OdDbHostAppServices::getGRIPCOLOR()
   OdDbHostAppServices::setGRIPCOLOR()
  </table>  

<group TD_SysVars_G>
  */
//<dom_var_def>OdInt16 GRIPCOLOR;
REGVAR_DEF(OdInt16,             GRIPCOLOR,      (160),                (),       ValidateRange(1,255))

/**
  <title GRIPDYNCOLOR>
  <toctitle GRIPDYNCOLOR>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   140
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        2006+
  </table>
   
  GRIPDYNCOLOR controls the color of custom grips for dynamic blocks. The value can be 1-255.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;  
   OdDbHostAppServices::getGRIPDYNCOLOR()
   OdDbHostAppServices::setGRIPDYNCOLOR()
  </table> 

<group TD_SysVars_G>
  */
//<dom_var_def>OdInt16 GRIPDYNCOLOR;
REGVAR_DEF(OdInt16,             GRIPDYNCOLOR,   (140),                (),       ValidateRange(1,255))

/**
  <title GRIPHOT>
  <toctitle GRIPHOT>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   1
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        R12+
  </table>
   
  GRIPHOT controls the color of grips that are selected. The value can be 1-255.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;  
   OdDbHostAppServices::getGRIPHOT()
   OdDbHostAppServices::setGRIPHOT()
  </table> 

<group TD_SysVars_G>
  */
//<dom_var_def>OdInt16 GRIPHOT;
REGVAR_DEF(OdInt16,             GRIPHOT,        (1),                  (),       ValidateRange(1,255))

/**
  <title GRIPS>
  <toctitle GRIPS>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   1
   <b>Range</b>           0 ... 1
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        R12+
  </table>
  
  GRIPS controls whether selection set grips are turned on or off.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Grips are turned off.
   1              Grips are turned on.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;  
   OdDbHostAppServices::getGRIPS()
   OdDbHostAppServices::setGRIPS()
  </table>  

<group TD_SysVars_G>
  */
//<dom_var_def>OdInt16 GRIPS;
REGVAR_DEF(OdInt16,             GRIPS,          (1),                  (),       ValidateRange(0,1))

/**
  <title GRIPSIZE>
  <toctitle GRIPSIZE>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   5
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        R12+
  </table>
   
  GRIPSIZE determines the size of the grip box. The value can be 1-255 pixels.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;  
   OdDbHostAppServices::getGRIPSIZE()
   OdDbHostAppServices::setGRIPSIZE()
  </table>  

<group TD_SysVars_G>
  */
//<dom_var_def>OdInt16 GRIPSIZE;
REGVAR_DEF(OdInt16,             GRIPSIZE,       (5),                  (),       ValidateRange(1,255))

/*** VARDOCS:
  <properties>
    <remarks>
    "\n   Controls whether the drawing is saved with visual fidelity."
    "\n"
    "\n    @table"
    "\n    Value    Description"
    "\n    0        Saved without visual fidelity"
    "\n    1        Saved with visual fidelity"
    </remarks>
  </properties>
*/
REGVAR_DEF(OdInt16,             SAVEFIDELITY,   (1),                  (),       ValidateRange(0,1))

/*** VARDOCS:
  <properties>
    <remarks>
      "\n    DWFOSNAP controls whether object snapping is active for geometry in DWF underlays
      "\n"
      "\n    @table"
      "\n    Value    Description"
      "\n    false    Disabled"
      "\n    true     Enabled"
    </remarks>
  </properties>
*/
REGVAR_DEF(ODTBOOL,             DWFOSNAP,      (1),                  (),       ValidateBOOL())

/*** VARDOCS:
  <properties>
    <remarks>
      "\n    DGNOSNAP controls whether object snapping is active for geometry in DGN underlays
      "\n"
      "\n    @table"
      "\n    Value    Description"
      "\n    false    Disabled"
      "\n    true     Enabled"
    </remarks>
  </properties>
*/
REGVAR_DEF(ODTBOOL,             DGNOSNAP,      (1),                  (),       ValidateBOOL())

/**
  <title OLEQUALITY>
  <toctitle OLEQUALITY>
   
  <table>
   <b>Type</b>            OdUInt8
   <b>Initial value</b>   3
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        2000+
  </table>
   
  The OLEQUALITY variable is used to set the default plot quality for OLE objects. If the value of 
  OLEQUALITY is set to 3, the quality level is assigned automatically based on the object type.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Monochrome
   1              Low graphics
   2              High graphics
   3              Automatically select
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;  
   OdDbHostAppServices::getOLEQUALITY()
   OdDbHostAppServices::setOLEQUALITY()
  </table>
   
  <b>Note: </b>OLEQUALITY specifies an output quality for newly created OdDbOle2Frame entities. 

<group TD_SysVars_O>
  */
//<dom_var_def>OdUInt8 OLEQUALITY;
REGVAR_DEF(OdUInt8,             OLEQUALITY,    (3),                  (),       ValidateRange(0, 3))

/**
  <title OLEHIDE>
  <toctitle OLEHIDE>
   
  <table>
   <b>Type</b>            OdInt8
   <b>Initial value</b>   0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        R14+
  </table>
   
  The OLEHIDE variable is used to control the display and plotting of OLE objects.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              All OLE objects are displayed and plotted.
   1              OLE objects are displayed and plotted in paper space only.
   2              OLE objects are displayed and plotted in model space only.
   3              No OLE objects are displayed and plotted.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;  
   OdDbHostAppServices::getOLEHIDE()
   OdDbHostAppServices::setOLEHIDE()
  </table>  

<group TD_SysVars_O>
  */
//<dom_var_def>OdInt8 OLEHIDE;
REGVAR_DEF(OdInt8,              OLEHIDE,       (0),                  (),       ValidateRange(0, 3))

//-------------------------
// Introduced in R13
//-------------------------


/**
  <title ISAVEPERCENT>
  <toctitle ISAVEPERCENT>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   50
   <b>Range</b>           0 ... 100
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        R13+
  </table>
   
  The ISAVEPERCENT variable stores percentage of total unused space tolerated in .dwg file 
  after incremental file saving. It effects behavior of OdDbDatabase::save() method.
  <table>
   <b>Value</b>         <b>Description</b>
   0                    OdDbDatabase::save() method always performs full save.
   100                  OdDbDatabase::save() method always performs incremental save.
   </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;  
   OdDbHostAppServices::getISAVEPERCENT()
   OdDbHostAppServices::setISAVEPERCENT()
  </table>  

<group TD_SysVars_I>
  */
//<dom_var_def>OdInt16 ISAVEPERCENT;
REGVAR_DEF(OdInt16,             ISAVEPERCENT,   (50),                  (),       ValidateRange(0, 100))


//-------------------------
// Introduced in R14
//-------------------------

/**
  <title DEMANDLOAD>
  <toctitle DEMANDLOAD>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   3
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        2014+
  </table>
   
  DEMANDLOAD controls demand-loading of an application. The following bit codes are available to 
  turn on demand-loading:
   
  <table>
   <b>Bit Value</b>     <b>Description</b>
   1                    If set, demand-loads the source application when you open a drawing with custom objects.
   2                    If set, demand-loads the source application when you call one of the application's commands. (Not supported by Teigha but can be implemented in client applications).
   </table>
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;  
   OdDbHostAppServices::getDEMANDLOAD()
   OdDbHostAppServices::setDEMANDLOAD()
   </table> 

<group TD_SysVars_D>
   */
//<dom_var_def>OdInt16 DEMANDLOAD;
REGVAR_DEF(OdInt16,             DEMANDLOAD,     (3),                    (),      ValidateRange(0, 3))

//-----------------------------------------
// Variables added in AC 2008
//-----------------------------------------

REGVAR_DEF(OdInt16,            LAYLOCKFADECTL,      (50),               (),       ValidateRange(-90, 90))

//-----------------------------------------
// Variables added in AC 2009
//-----------------------------------------

/**
  <title THUMBSIZE>
  <toctitle THUMBSIZE>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   3
   <b>Range</b>           0 ... 8
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        2009+
  </table>
   
  The THUMBSIZE specifies the size for thumbnail preview associated with a drawing in pixels.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              64x64
   1              128x128
   2              256x256
   3              512x512
   4              1024x1024
   5              1440x1440
   6              1600x1600
   7              1920x1920
   8              2560x2560
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;  
   OdDbHostAppServices::getTHUMBSIZE()
   OdDbHostAppServices::setTHUMBSIZE()
  </table>
   
  For example, to get the value:
  <code>
   OdRusBufPtr pRb = pDb-\>getSysVar(L"THUMBSIZE");
   OdInt16 size = pRb-\>getInt16();
   odPrintConsoleString(L"nThumbnail size = %d", size);
   // or //
   odPrintConsoleString(L"nThumbnail size = %d", pDb-\>appServices()-\>getTHUMBSIZE());
  </code>
  For example, to set the value:
  <code>
   OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtInt16, 1);
   pDb-\>setSysVar(L"THUMBSIZE", pRb);
   // or //
   pDb-\>appServices()-\>setTHUMBSIZE(1);
  </code>
  
<group TD_SysVars_T>
  */
//<dom_var_def>OdInt16 THUMBSIZE;
REGVAR_DEF(OdInt16,            THUMBSIZE,      (3),                  (),       ValidateRange(0,8))

REGVAR_DEF(OdInt16,         PUBLISHHATCH,      (1),                  (),       ValidateRange(0,1))

REGVAR_DEF(OdInt16,          OPENPARTIAL,      (1),                  (),       ValidateRange(0,1))

REGVAR_DEF(double,          DGNIMPORTMAX,    (0.0),                  (),       ValidateNone())

/**
  <title PLINECONVERTMODE>
  <toctitle PLINECONVERTMODE>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        2009+
  </table>
   
  PLINECONVERTMODE controls the fit method, which is used during conversion of a spline to polyline.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Linear segments will be used.
   1              Arc segments will be used.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;  
   OdDbHostAppServices::getPLINECONVERTMODE()
   OdDbHostAppServices::setPLINECONVERTMODE()
  </table> 

<group TD_SysVars_P>
  */
//<dom_var_def>OdInt16 PLINECONVERTMODE;
REGVAR_DEF(OdInt16,             PLINECONVERTMODE,      (0),                  (),       ValidateRange(0,1))



//-----------------------------------------
// Variables added in AC 2010
//-----------------------------------------

/*** VARDOCS:
<properties>
<remarks>
"\n    PDFOSNAP controls whether object snapping is active for geometry in PDF underlays
"\n"
"\n    @table"
"\n    Value    Description"
"\n    false    Disabled"
"\n    true     Enabled"
</remarks>
</properties>
*/
REGVAR_DEF(ODTBOOL,             PDFOSNAP,      (1),                  (),       ValidateBOOL())

REGVAR_DEF(OdInt16,             XFADECTL,     (50),                  (),       ValidateRange(0,90))

REGVAR_DEF(OdInt16,             XDWGFADECTL,    (50),                  (),       ValidateRange(-90, 90))

/**
  <title PARAMETERCOPYMODE>
  <toctitle PARAMETERCOPYMODE>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   1
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        2010+
  </table>
   
  PARAMETERCOPYMODE controls how constraints and related parameters are handled when constraint objects are copied
  between model space, paper space or a block.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Do not copy constraints.
   1              Replace all expressions with constants.
   2              Use existing parameters, and if a parameter is missing, replace it with a constant.
   3              Use existing parameters, and create a new parameter if it is missing.
   4              Use existing parameters, and rename a parameter if conflicts occur.
   </table>
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;  
   OdDbHostAppServices::getPARAMETERCOPYMODE()
   OdDbHostAppServices::setPARAMETERCOPYMODE()
   </table> 

<group TD_SysVars_P>
   */
//<dom_var_def>OdInt16 PARAMETERCOPYMODE;
REGVAR_DEF(OdInt16,             PARAMETERCOPYMODE,     (1),                  (),       ValidateRange(0,4))


//-----------------------------------------
// Variables added in AC 2011
//-----------------------------------------

/** 
  <title OBJECTISOLATIONMODE>
  <toctitle OBJECTISOLATIONMODE>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        2011+
  </table>
   
  OBJECTISOLATIONMODE controls whether objects that are hidden by HIDEOBJECTS and ISOLATEOBJECTS commands 
  will remain hidden after saving and reopening the drawing.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Isolation will not be saved.
   1              Isolation will be saved.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;  
   OdDbHostAppServices::getOBJECTISOLATIONMODE()
   OdDbHostAppServices::setOBJECTISOLATIONMODE()
  </table>  

<group TD_SysVars_O>
  */
//<dom_var_def>OdInt16 OBJECTISOLATIONMODE;
REGVAR_DEF(OdInt16,   OBJECTISOLATIONMODE,          (0),                  (),        ValidateRange(0,1))

//-----------------------------------------
// Variables added in AC 2012
//-----------------------------------------

/**
  <title ARRAYTYPE>
  <toctitle ARRAYTYPE>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        2012+
  </table>
   
  ARRAYTYPE specifies the default type of array.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Rectangular array
   1              Path array
   2              Polar array
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;  
   OdDbHostAppServices::getARRAYTYPE()
   OdDbHostAppServices::setARRAYTYPE()
  </table>  

<group TD_SysVars_A>
  */
//<dom_var_def>OdInt16 ARRAYTYPE;
REGVAR_DEF(OdInt16,   ARRAYTYPE          ,          (0),                  (),        ValidateRange(0,2))

/**
  <title POINTCLOUDAUTOUPDATE>
  <toctitle POINTCLOUDAUTOUPDATE>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   1
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        2011+
  </table>
   
  POINTCLOUDAUTOUPDATE controls whether a point cloud is regenerated automatically after manipulation, panning,
  zooming, or orbiting.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Point cloud entity is to be manually regenerated.
   1              Point cloud entity is regenerated automatically.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;  
   OdDbHostAppServices::getPOINTCLOUDAUTOUPDATE()
   OdDbHostAppServices::setPOINTCLOUDAUTOUPDATE()
  </table>  

<group TD_SysVars_P>
  */
//<dom_var_def>OdInt16 POINTCLOUDAUTOUPDATE;
REGVAR_DEF(OdInt16,   POINTCLOUDAUTOUPDATE,   (1),                  (),        ValidateRange(0,1))

/**
  <title POINTCLOUDDENSITY>
  <toctitle POINTCLOUDDENSITY>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   15
   <b>Range</b>           1 ... 100
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        2011+
  </table>
   
  POINTCLOUDDENSITY controls the number of points in percentage out of the maximum points per drawing 
  (as defined by POINTCLOUDPOINTMAX) displayed for all point clouds in the drawing view.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;  
   OdDbHostAppServices::getPOINTCLOUDDENSITY()
   OdDbHostAppServices::setPOINTCLOUDDENSITY()
  </table>  

<group TD_SysVars_P>
  */
//<dom_var_def>OdInt16 POINTCLOUDDENSITY;
REGVAR_DEF(OdInt16,   POINTCLOUDDENSITY,      (15),                 (),        ValidateRange(1,100))

/**
  <title POINTCLOUDLOCK>
  <toctitle POINTCLOUDLOCK>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        2011+
  </table>
   
  POINTCLOUDLOCK controls whether a newly created point cloud entity is locked (can't be manipulated, moved, 
  clipped, or rotated). The property doesn't affect existing point cloud entities.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Point cloud entity isn't locked and can be manipulated and edited.
   1              Point cloud entity is locked for manipulating and editing.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;  
   OdDbHostAppServices::getPOINTCLOUDLOCK()
   OdDbHostAppServices::setPOINTCLOUDLOCK()
  </table>  

<group TD_SysVars_P>
  */
//<dom_var_def>OdInt16 POINTCLOUDLOCK;
REGVAR_DEF(OdInt16,   POINTCLOUDLOCK,         (0),                  (),        ValidateRange(0,1))

/**
  <title POINTCLOUDRTDENSITY>
  <toctitle POINTCLOUDRTDENSITY>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   5
   <b>Range</b>           1 ... 100
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        2011+
  </table>
   
  POINTCLOUDRTDENSITY controls the number of points in percentage out of the maximum points per drawing 
  (as defined by POINTCLOUDPOINTMAX) displayed in the drawing view while zooming, panning, or orbiting in real time.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;  
   OdDbHostAppServices::getPOINTCLOUDRTDENSITY()
   OdDbHostAppServices::setPOINTCLOUDRTDENSITY()
  </table>

  <group TD_SysVars_P>
  */
//<dom_var_def>OdInt16 POINTCLOUDRTDENSITY;
REGVAR_DEF(OdInt16,   POINTCLOUDRTDENSITY,    (5),                  (),        ValidateRange(1,100))

//-----------------------------------------
// Variables added in AC 2013
//-----------------------------------------

/**
  <title ARRAYASSOCIATIVITY>
  <toctitle ARRAYASSOCIATIVITY>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   1
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        2013+
  </table>
   
  The ARRAYASSOCIATIVITY variable stores the default behavior of a new array which can be associative 
  or nonassociative.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Non-associative
   1              Associative
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;  
   OdDbHostAppServices::getARRAYASSOCIATIVITY()
   OdDbHostAppServices::setARRAYASSOCIATIVITY()
  </table>

  <group TD_SysVars_A>
  */
//<dom_var_def>OdInt16 ARRAYASSOCIATIVITY;
REGVAR_DEF(OdInt16,   ARRAYASSOCIATIVITY,           (1),                  (),        ValidateRange(0,1))

/**
  <title POINTCLOUDBOUNDARY>
  <toctitle POINTCLOUDBOUNDARY>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   1
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        2013+
  </table>
   
  POINTCLOUDBOUNDARY controls whether a point cloud boundary is displayed (but not printed).
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Point cloud boundary is hidden.
   1              Point cloud boundary is displayed including any clipped areas.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;  
   OdDbHostAppServices::getPOINTCLOUDBOUNDARY()
   OdDbHostAppServices::setPOINTCLOUDBOUNDARY()
  </table>

  <group TD_SysVars_P>
  */
//<dom_var_def>OdInt16 POINTCLOUDBOUNDARY;
REGVAR_DEF(OdInt16,   POINTCLOUDBOUNDARY,     (1),                  (),        ValidateRange(0,1))

/**
  <title POINTCLOUDPOINTMAX>
  <toctitle POINTCLOUDPOINTMAX>
   
  <table>
   <b>Type</b>            OdUInt32
   <b>Initial value</b>   1500000
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        2013+
  </table>
   
  POINTCLOUDPOINTMAX controls the maximum number of points that can be displayed for all point 
  clouds attached to the drawing.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;  
   OdDbHostAppServices::getPOINTCLOUDPOINTMAX()
   OdDbHostAppServices::setPOINTCLOUDPOINTMAX()
  </table>

<group TD_SysVars_P>
  */
//<dom_var_def>OdUInt32 POINTCLOUDPOINTMAX;
REGVAR_DEF(OdUInt32,  POINTCLOUDPOINTMAX,     (1500000),            (),        ValidateNone())

//-----------------------------------------
// Variables added in AC 2014
//-----------------------------------------

REGVAR_DEF(OdInt16,   TEXTALIGNMODEMODE,     (9),                  (),        ValidateRange(0,11))


//-----------------------------------------
// Variables added in AC 2015
//-----------------------------------------

/**
  <title LINESMOOTHING>
  <toctitle LINESMOOTHING>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        2015+
  </table>
   
  The LINESMOOTHING variable switches anti-aliasing on or off for lines.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Disabled
   1              Enabled
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;  
   OdDbHostAppServices::getLINESMOOTHING()
   OdDbHostAppServices::setLINESMOOTHING()
  </table>
   
  For example, to get the value:
  <code>
   OdRusBufPtr pRb = pDb-\>getSysVar(L"LINESMOOTHING");
   OdInt16 type = pRb-\>getInt16();
   odPrintConsoleString(L"nAnti-aliasing status = %d", type);
   // or //
   odPrintConsoleString(L"nAnti-aliasing status = %d", pDb-\>getLINESMOOTHING());
  </code>
  For example, to set the value:
   <code>
   OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtInt16, 1);
   pDb-\>setSysVar(L"LINESMOOTHING", pRb);
   // or //
   pDb-\>setLINESMOOTHING(1);
  </code>

  <group TD_SysVars_L>
  */
//<dom_var_def>OdInt16 LINESMOOTHING;
REGVAR_DEF(OdInt16,   LINESMOOTHING,          (0),                  (),        ValidateRange(0,1))

/**
  <title HPLINETYPE>
  <toctitle HPLINETYPE>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        2015+
  </table>
   
  HPLINETYPE controls the displaying of non-continuous linetypes in hatch patterns.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Continuous linetypes in predefined and custom hatch
                   patterns, regardless of the assigned linetype will be
                   displayed.
   1              Non-continuous linetypes in hatch patterns will be displayed.
  </table>
   
   <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;  
   OdDbHostAppServices::getHPLINETYPE()
   OdDbHostAppServices::setHPLINETYPE()
   </table>

   <group TD_SysVars_H>
   */
//<dom_var_def>OdInt16 HPLINETYPE;
REGVAR_DEF(OdInt16,   HPLINETYPE,             (0),                  (),        ValidateRange(0,1))

REGVAR_DEF(OdInt32,        POINTCLOUDPOINTMAXLEGACY,      (1500000),            (),             ValidateRange(1500000,10000000))
REGVAR_DEF(OdInt16,        POINTCLOUDLOD,                      (10),            (),             ValidateRange(1,10))

//-----------------------------------------
// Variables added in AC 2016
//-----------------------------------------

/**
  <title REVCLOUDCREATEMODE>
  <toctitle REVCLOUDCREATEMODE>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   1
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        2016+
  </table>
   
  The REVCLOUDCREATEMODE variable is used to specify the default mode for creation of revision clouds.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Freehand
   1              Rectangular
   2              Polygonal
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;  
   OdDbHostAppServices::getREVCLOUDCREATEMODE()
   OdDbHostAppServices::setREVCLOUDCREATEMODE()
  </table>
   
  <b>Note: </b>The last value of REVCLOUDCREATEMODE is stored in the system, and at the next command call 
  the revision clouds will have the same mode.  

<group TD_SysVars_R>
  */
//<dom_var_def>OdInt16 REVCLOUDCREATEMODE;
REGVAR_DEF(OdInt16,   REVCLOUDCREATEMODE,     (1),                  (),        ValidateRange(0,2))

/**
  <title REVCLOUDGRIPS>
  <toctitle REVCLOUDGRIPS>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   1
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        2016+
  </table>
   
  The REVCLOUDGRIPS variable is used to control the number of grips displayed on a revision cloud.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              A grip for each arc segment on a revision cloud is displayed.
   1              A limited number grips on a revision cloud are displayed.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;  
   OdDbHostAppServices::getREVCLOUDGRIPS()
   OdDbHostAppServices::setREVCLOUDGRIPS()
  </table>

<group TD_SysVars_R>
  */
//<dom_var_def>OdInt16 REVCLOUDGRIPS;
REGVAR_DEF(OdInt16,   REVCLOUDGRIPS,          (1),                  (),        ValidateRange(0,1))


//-----------------------------------------
// Variables added in AC 2017
//-----------------------------------------
REGVAR_DEF(ODTBOOL,   LTGAPSELECTION,     (0),                  (),        ValidateBOOL())



///////////////////////////////////////////
// Teigha-specific system variables
///////////////////////////////////////////

/**
  <title SAVEROUNDTRIP>
  <toctitle SAVEROUNDTRIP>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   true
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        N/A
  </table>
   
  SAVEROUNDTRIP controls the saving of information in a database file to allow round-tripping of object types not
  supported in the save file format.
   
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
   OdDbHostAppServices::getSAVEROUNDTRIP()
   OdDbHostAppServices::setSAVEROUNDTRIP()
  </table>
   
<group TD_SysVars_S>
 */
//<dom_var_def>bool SAVEROUNDTRIP;
REGVAR_DEF(bool,                SAVEROUNDTRIP,  (true),               (),       ValidateBOOL())

/**
  <title R12SaveDeviation>
  <toctitle R12SaveDeviation>
   
  <table>
   <b>Type</b>            double
   <b>Initial value</b>   0.0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        N/A
  </table>
   
  R12SaveDeviation controls the deviation for saving ellipses and splines to R12.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;  
   OdDbHostAppServices::getR12SaveDeviation()
   OdDbHostAppServices::setR12SaveDeviation()
  </table>

  <group TD_SysVars_R>
  */
//<dom_var_def>double R12SaveDeviation;
REGVAR_DEF(ODTDIST,             R12SaveDeviation,(0.),                (),       ValidateNone())

/**
  <title R12SaveAccuracy>
  <toctitle R12SaveAccuracy>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   8
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        N/A
  </table>
   
  R12SaveAccuracy controls the number of segments between spline control segments or on 90&#176; elliptical arcs, 
  for saving ellipses and splines to R12.
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbHostAppServices::getR12SaveAccuracy()
   OdDbHostAppServices::setR12SaveAccuracy()
  </table>

  <group TD_SysVars_R>
  */
//<dom_var_def>OdInt16 R12SaveAccuracy;
REGVAR_DEF(OdInt16,             R12SaveAccuracy,(8),                  (),       ValidateNone())

/**
  <title AcisSaveAsMode>
  <toctitle AcisSaveAsMode>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   0
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        N/A
  </table>
   
  AcisSaveAsMode controls the exploding of ACIS entities (3D solids, bodies, and regions) while saving to 
  R12 and earlier drawing files, saving to a proxy's graphics metafile, and exploding geometry.
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              3D solids, bodies, regions, and DbSurfaces are saved as polyface mesh entities.
   1              3D solids, bodies, regions, and DbSurfaces are saved with edges only (no isolines or silhouettes).
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbHostAppServices::getAcisSaveAsMode()
   OdDbHostAppServices::setAcisSaveAsMode()
  </table>

  <group TD_SysVars_A>
  */
//<dom_var_def>OdInt16 AcisSaveAsMode;
REGVAR_DEF(OdInt16,             AcisSaveAsMode,(0),                (),       ValidateRange(0,1))

/*** VARDOCS:
  <properties>
    <remarks>
      "\n    AcisProxyMode controls saving to proxy's graphics metafile mode of ACIS entities (3DSolids, Bodies, Regions)."
      "\n"
      "\n    @table"
      "\n    Value    Description"
      "\n    0        Save meshes"
      "\n    1        Save edges only"
    </remarks>
  </properties>
*/

/*** VARDOCS:
  <properties>
    <remarks>
      "\n    PLINECACHE controls creation the cache of vertexes for OdDb2dPolyline when a database file is opened."
      "\n"
      "\n    @table"
      "\n    Value    Cache"
      "\n    false    Disabled"
      "\n    true     Enabled"
    </remarks>
  </properties>
*/
REGVAR_DEF(ODTBOOL,             PLINECACHE,      (false),                  (),       ValidateBOOL())

/*** VARDOCS:
  <properties>
    <remarks>
      "\n    DxfTextAdjustAlignment controls if adjustAlignment() is called"
      " at the end of loading OdDbText from DXF file."
      "\n"
      "\n    @table"
      "\n    Value    Description"
      "\n    false    Disabled"
      "\n    true     Enabled"
    </remarks>
  </properties>
*/
REGVAR_DEF(bool,                DxfTextAdjustAlignment,  (false),               (),       ValidateBOOL())

/**
  <title RecomputeDimBlocksRequired>
  <toctitle RecomputeDimBlocksRequired>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   true
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
  </table>
   
  The RecomputeDimBlocksRequired system variable specifies how the absence of the RecomputeDimBlock.tx 
  module is handled. If it has a true value and if the RecomputeDimBlock module is absent, an exception 
  is thrown to prevent creating files where dimensions are not displayed. If client code does not 
  use RecomputeDimBlock functionality intentionally (not recommended), call 
  OdDbHostAppServices::setRecomputeDimBlocksRequired(false); before creating a database or loading a file.
   
  <table>
   <b>Value</b>   <b>Description</b>
   false          Disabled - RecomputeDimBlock.tx module is not required
   true           Enabled - RecomputeDimBlock.tx module is required
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbHostAppServices::getRecomputeDimBlocksRequired()
   OdDbHostAppServices::setRecomputeDimBlocksRequired()
  </table>

  <group TD_SysVars_R>
  */
//<dom_var_def>bool RecomputeDimBlocksRequired;
REGVAR_DEF(bool,                RecomputeDimBlocksRequired,  (true),               (),       ValidateBOOL())

/**
  <title AllowSavingEmptyAcisObjects>
  <toctitle AllowSavingEmptyAcisObjects>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   false
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        N/A
  </table>
   
  The AllowSavingEmptyAcisObjects variable determines how to resolve the situation of saving ACIS 
  objects when ACIS data cannot be converted to other versions. This may happen when a modeler module 
  is not loaded, so the result is that the conversion engine is unavailable. The value can be:
   
  <table>
   <b>Value</b>   <b>Description</b>
   false          Saving disabled; when the situation occurs, the system throws an exception to prevent the data loss.
   true           Saving enabled; when the situation occurs, the system does not throw exceptions and saves empty objects.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbHostAppServices::getAllowSavingEmptyAcisObjects()
   OdDbHostAppServices::setAllowSavingEmptyAcisObjects()
  </table>
   
  For example, to get the value:
  <code>
   OdResBufPtr pRb = pDb-\>getSysVar(L"AllowSavingEmptyAcisObjects");
   bool bAllow = pRb-\>getBool();
   odPrintConsoleString(L"nAllowSavingEmptyAcisObjects is %s", ((bAllow) ? "ON" : "OFF"));
   // or //
   bAllow = pDb-\>appServices()-\>getAllowSavingEmptyAcisObjects();
   odPrintConsoleString(L"nAllowSavingEmptyAcisObjects = %s", ((bAllow) ? "ON" : "OFF"));
  </code>
  For example, to set the value:
  <code>
   OdResBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtBool, true);
   pDb-\>setSysVar(L"AllowSavingEmptyAcisObjects", pRb);
   // or //
   pDb-\>appServices()-\>setAllowSavingEmptyAcisObjects(true);
  </code>

    <group TD_SysVars_A>
  */
//<dom_var_def>bool AllowSavingEmptyAcisObjects;
REGVAR_DEF(bool,                AllowSavingEmptyAcisObjects,  (false),               (),       ValidateBOOL())

/**
  <title RestoreHatchFromBlkRef>
  <toctitle RestoreHatchFromBlkRef>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   true
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        N/A
  </table>
   
  RestoreHatchFromBlkRef controls whether during file loading an attempt is made to restore a hatch entity 
  from round-trip XData of a Block Reference to an anonymous block representing a hatch in R12 files.
   
  <table>
   <b>Value</b>   <b>Description</b>
   false          An attempt to restore Hatch entity is not made during file loading.
   true           An attempt to restore Hatch entity is made during file loading.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbHostAppServices::getRestoreHatchFromBlkRef()
   OdDbHostAppServices::setRestoreHatchFromBlkRef()
  </table>

    <group TD_SysVars_R>
  */
//<dom_var_def>bool RestoreHatchFromBlkRef;
REGVAR_DEF(bool,                RestoreHatchFromBlkRef,  (true),               (),       ValidateBOOL())

/**
  <title MTMODE>
  <toctitle MTMODE>
   
  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   1
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        N/A
  </table>
   
  The MTMODE variable specifies whether multi-threading is used. The value is a bit mask:
   
  <table>
   <b>Value</b>   <b>Description</b>
   0              Multi-threaded file loading and regeneration are disabled.
   1              Multi-threaded file loading is enabled.
   2              Multi-threaded regeneration is enabled.
   3              Multi-threaded file loading and regeneration are enabled.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbHostAppServices::getMtMode()
   OdDbHostAppServices::setMtMode()
  </table>
   
  For example, to get the value:
  <code>
   OdRusBufPtr pRb = pDb-\>getSysVar(L"MTMODE");
   OdInt16 mtmode = pRb-\>getInt16();
   odPrintConsoleString(L"nMulti-thread mode is %s", ((mtmode &amp; 0x01) ? "ON" : "OFF"));
   // or //
   odPrintConsoleString(L"nMulti-thread mode = %d", pDb-\>appServices()-\>getMtMode());
  </code>
  For example, to set the value:
  <code>
   OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtInt16, 1);
   pDb-\>setSysVar(L"MTMODE", pRb);
   // or //
   pDb-\>appServices()-\>setMtMode(1);
  </code>

  <group TD_SysVars_M>
  */
//<dom_var_def>OdInt16 MTMODE;
REGVAR_DEF(OdInt16,              MtMode,  (1),               (),       ValidateNone())

/**
  <title NegativeHandlesReserved>
  <toctitle NegativeHandlesReserved>
   
  <table>
   <b>Type</b>            bool
   <b>Initial value</b>   false
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        N/A
  </table>
   
  The NegativeHandlesReserved variable specifies whether negative values of handles
  are reserved by application.
   
  <table>
   <b>Value</b>   <b>Description</b>
   false          Negative handles are used as ordinary handles.
   true           File controller decides itself how it will use negative handles.
  </table>
   
  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp; 
   OdDbHostAppServices::getNegativeHandlesReserved()
   OdDbHostAppServices::setNegativeHandlesReserved()
  </table>
   
  For example, to get the value:
  <code>
   OdRusBufPtr pRb = pDb-\>getSysVar(L"NegativeHandlesReserved");
   bool neghndlsres = pRb-\>getBool();
   odPrintConsoleString(L"nNegative handles reserved: %s", (neghndlsres ? "ON" : "OFF"));
   // or //
   odPrintConsoleString(L"nNegativeHandlesReserved = %d", pDb-\>appServices()-\>getNegativeHandlesReserved());
  </code>
  For example, to set the value:
  <code>
   OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtBool, true);
   pDb-\>setSysVar(L"NegativeHandlesReserved", pRb);
   // or //
   pDb-\>appServices()-\>setNegativeHandlesReserved(true);
  </code>

  <group TD_SysVars_M>
  */
//<dom_var_def>bool NegativeHandlesReserved;
REGVAR_DEF(bool,              NegativeHandlesReserved,  (false),               (),       ValidateBOOL())

#undef ODTDB_PROXYIMAGE


#undef SVARS_FUNC_MODIFIER
#define SVARS_FUNC_MODIFIER
///////////////////////////////////////////////////////////////////////
// New AppServices variables section
// To not break binary compatibility for new variables
// non-virtual functions are generated

// Add new REGVARs here

// Teigha-specific

/*** VARDOCS:
  <properties>
    <remarks>
      "\n    VerticalApplicationsMode controls behavior of vertical applications"
      "\n"   Bit-coded
      "\n    If bit set application creates its default objects in every drawing
      "\n    If not application does not create its default objects
      "\n    (enabler behavior)
      "\n    It also affects hatch rendering: empty stroke hatch is rendered as solid fill for Architecture"
      "\n    @table"
      "\n    Value       Description"
      "\n    bit 1     Teigha Architecture
      "\n    bit 2     Teigha Civil
      "\n    bit 4     Teigha Mechanical
    </remarks>
  </properties>
*/
REGVAR_DEF(OdUInt32,             VerticalApplicationsMode,     (0xFFFFFFFF),                    (),      ValidateNone())

/**
<title EnableAcisAudit>
<toctitle EnableAcisAudit>

<table>
<b>Type</b>            bool
<b>Initial value</b>   true
<b>Read-Only</b>       No
<b>Saved In</b>        Registry
<b>Versions</b>        N/A
</table>

The EnableAcisAudit variable allows you to turn on additional checks of ACIS
objects when ACIS data reads or writes. This is variable affects to Audit. So if EnableAcisAudit is set to
true additional checks of ACIS objects will be done and in case of some of them failed - ACIS object may be discarded

<table>
<b>Value</b>   <b>Description</b>
true           Use additional checks of ACIS data to prevent errors while a saved dwg/dxf loads by AutoCad2018.
false          Audit of ACIS data will not be performed, ACIS data will load and save without additional checks so AutiCad2018 may report error or show
               recovery dialog
</table>

<table>
<b>Access Methods</b>
OdDbDatabase::getSysVar()
OdDbDatabase::setSysVar()
&nbsp; 
OdDbHostAppServices::getEnableAcisAudit()
OdDbHostAppServices::setEnableAcisAudit()
</table>

For example, to get the value:
<code>
OdResBufPtr pRb = pDb-\>getSysVar(L"EnableAcisAudit");
bool bEnabled = pRb-\>getBool();
odPrintConsoleString(L"nEnableAcisAudit is %s", ((bDisabled) ? "ON" : "OFF"));
// or //
bool bEnabled = pDb-\>appServices()-\>getEnableAcisAudit();
odPrintConsoleString(L"nEnableAcisAudit = %s", ((bEnabled) ? "ON" : "OFF"));
</code>
For example, to set the value:
<code>
OdResBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtBool, true);
pDb-\>setSysVar(L"EnableAcisAudit", pRb);
// or //
pDb-\>appServices()-\>setEnableAcisAudit(true);
</code>

<group TD_SysVars_A>
*/
//<dom_var_def>bool DisableAcisAudit;
REGVAR_DEF(bool, EnableAcisAudit, (false), (), ValidateBOOL())


/*** VARDOCS:
  <properties>
    <remarks>
      "\n    If HonourLockedLayer is true, entities on locked layer cannot be opened for write: OdDbObjectId::openObject will return eOnLockedLayer "
    </remarks>
  </properties>
*/
REGVAR_DEF(bool,                 HonourLockedLayer,            (false),                         (),      ValidateBOOL())

/*** VARDOCS:
  <properties>
    <remarks>
      "\n    Controls the display of row numbers and column letters when the In-Place Text Editor is open for editing a table cell."
    </remarks>
   </properties>
*/
REGVAR_DEF(bool,                 TableIndicator,                (true),                          (),      ValidateBOOL())

/*** VARDOCS:
  <properties>
    <remarks>
      "\n    Controls the display link ranges in selected table."
    </remarks>
  </properties>
*/
REGVAR_DEF(bool,                 TableLinkIndicator,            (true),                          (),       ValidateBOOL())

/*** VARDOCS:
  <properties>
    <remarks>
      "\n    Controls the display selected cell range in selected table."
    </remarks>
  </properties>
*/
REGVAR_DEF(bool,                 TableSelectIndicator,           (true),                         (),        ValidateBOOL())

/*** VARDOCS:
<properties>
<remarks>
"\n    A directory for saving raster images when importing from a pdf files to a .dwg. If this variable isn't specified, the images are saved in the application temporary directory."
</remarks>
</properties>
*/
REGVAR_DEF(OdString,             PDFIMPORTIMAGEPATH,             (OD_T("")),                     (),         ValidateNone())

#undef SVARS_FUNC_MODIFIER


#endif //#ifdef REGVAR_DEF


#undef ODTBOOL
#undef ODTDIST
#undef ODTORIENT
#undef ODTUNITLESS4
#undef ODTUNITLESS8
#undef ODTDB_MEASUREMENTVALUE
#undef ODTDB_LINEWEIGHT
#undef ODTDB_JOINSTYLE
#undef ODTDB_UNITSVALUE
#undef ODTDB_PROXYIMAGE
#undef ODTDB_MEASUREMENTVALUE
#undef ODTTEXTSTYLEID
#undef ODTLAYERID
#undef ODTLAYERIDDEF
#undef ODTLINETYPEID
#undef ODTDIMSTYLEID
#undef ODTUCSID
#undef ODTMLSTYLEID
#undef ODTDB_ENDCAPS
#undef ODTDB_JOINSTYLE
#undef ODTDB_PLOTSTYLENAMETYPE
#undef ODTDB_TIMEZONE
#undef ODTMATERIALID
#undef ODVISTYLEID
#undef ODTTABLESTYLEID
#undef ODTMLEADERSTYLEID
#undef ODTVIEWDETAILSTYLEID
#undef ODTVIEWSECTIONSTYLEID
#undef ODANNOSCALE
#undef ODDBHARDPOINTERID
#undef ODDBOBJECTID
#undef ODTTRANSPARENCY
