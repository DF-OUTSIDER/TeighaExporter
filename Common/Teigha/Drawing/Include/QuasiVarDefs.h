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



#ifndef QVAR_DEF_RO
#define DOUNDEF_QVAR_DEF_RO
#define QVAR_DEF_RO  QVAR_DEF /*!DOM*/
#endif

#ifndef QVAR_DEF_RO
#define DOUNDEF_QVAR_DEF_RO
#define QVAR_DEF_RO  QVAR_DEF /*!DOM*/
#endif


#ifndef ODTDIST
#define ODTDIST  double /*!DOM*/
#define DOUNDEF_ODTDIST
#endif

#ifndef ODTORIENT
#define ODTORIENT  double /*!DOM*/
#define DOUNDEF_ODTORIENT
#endif

#ifndef ODTANGLE
#define ODTANGLE double /*!DOM*/
#define DOUNDEF_ODTANGLE
#endif

#ifndef ODTLAYOUTID
#define ODTLAYOUTID  OdDbObjectId /*!DOM*/
#define DOUNDEF_ODTLAYOUTID
#endif

#ifndef ODTTEXTSTYLEID
#define ODTTEXTSTYLEID  OdDbObjectId /*!DOM*/
#define DOUNDEF_ODTTEXTSTYLEID
#endif

#ifndef ODTBOOL
#define ODTBOOL  bool /*!DOM*/
#define DOUNDEF_ODTBOOL
#endif

/*
#ifdef DATE
#define SAVED_DATE_DEFINITION
#undef DATE
#endif
*/

//         Type                   Name

/** 
  <title BACKZ>
  <toctitle BACKZ>
   
  <table>
    <b>Type</b>            double
    <b>Initial value</b>   0.0
    <b>Read-Only</b>       Yes
    <b>Saved In</b>        Database
    <b>Versions</b>        R12+
  </table>
   
  BACKZ specifies the back clipping distance for the current viewport.
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
  </table>
  
  \sa
  <link VIEWMODE, VIEWMODE>
  <group TD_SysVars_B>
*/
//<dom_var_def>double BACKZ;
QVAR_DEF_RO(ODTDIST,              BACKZ,            ValidateNone()    )

/**
  <title CTAB>
  <toctitle CTAB>
   
  <table>
    <b>Type</b>            OdDbObjectId
    <b>Initial value</b>   Model Layout Id
    <b>Read-Only</b>       No
    <b>Saved In</b>        Database
    <b>Versions</b>        2000+
  </table>
   
  CTAB specifies the current tab (model or layout) in the database.
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>
  <group TD_SysVars_C>
*/
//<dom_var_def>OdDbObjectId CTAB;
QVAR_DEF   (ODTLAYOUTID,          CTAB,             ValidateLayout()  )

/**
  <title CPLOTSTYLE>
  <toctitle CPLOTSTYLE>
   
  <table>
    <b>Type</b>            OdString
    <b>Initial value</b>   "ByColor"
    <b>Read-Only</b>       No
    <b>Saved In</b>        Database
    <b>Versions</b>        2000+
  </table>
   
  CPLOTSTYLE specifies the default plot style for new entities.
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>
  
  \sa
  <link CEPSNTYPE, CEPSNTYPE>
  <group TD_SysVars_C>
*/
//<dom_var_def>OdString CPLOTSTYLE;
QVAR_DEF   (OdString,             CPLOTSTYLE,       ValidateNone()    )

/**
  <title CVPORT>
  <toctitle CVPORT>
   
  <table>
    <b>Type</b>            OdInt16
    <b>Initial value</b>   2
    <b>Read-Only</b>       No
    <b>Saved In</b>        Database
    <b>Versions</b>        R12+
  </table>
   
  CVPORT specifies the ID of the current viewport. CVPORT == 1 specifies the current overall paper space viewport.
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
   </table>
  <group TD_SysVars_C>
*/
//<dom_var_def>OdInt16 CVPORT;
QVAR_DEF   (OdInt16,              CVPORT,           ValidateNone()    )

/**
  <title DWGNAME>
  <toctitle DWGNAME>
   
  <table>
    <b>Type</b>            OdString
    <b>Initial value</b>   ""
    <b>Read-Only</b>       Yes
    <b>Saved In</b>        Not saved
    <b>Versions</b>        R12+
  </table>
   
  DWGNAME specifies the name of the file associated with this database.
  * The path is not returned.
  * The extension is returned.
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
  </table>
  
  \sa
  <link DWGPREFIX, DWGPREFIX> 
  <group TD_SysVars_D>
*/
//<dom_var_def>OdString DWGNAME;
QVAR_DEF_RO(OdString,             DWGNAME,          ValidateNone()    )

/**
  <title DWGPREFIX>
  <toctitle DWGPREFIX>
   
  <table>
    <b>Type</b>            OdString
    <b>Initial value</b>   ""
    <b>Read-Only</b>       Yes
    <b>Saved In</b>        Not saved
    <b>Versions</b>        R12+
  </table>
   
  DWGPREFIX specifies the folder of the file associated with this database. The returned string, 
  if not empty, ends in a delimiter ('/' or '\\').
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
  </table>
  
  \sa
  <link DWGNAME, DWGNAME>
  <group TD_SysVars_D>
*/
//<dom_var_def>OdString DWGPREFIX;
QVAR_DEF_RO(OdString,             DWGPREFIX,        ValidateNone()    )

/**
  <title DWGTITLED>
  <toctitle DWGTITLED>
   
  <table>
    <b>Type</b>            bool
    <b>Initial value</b>   false
    <b>Read-Only</b>       Yes
    <b>Saved In</b>        Not saved
    <b>Versions</b>        R12+
  </table>
   
  DWGTITLED specifies whether a filename has been assigned to the database.
   
  <table>
    <b>Value</b>   <b>Description</b>
    false          Not assigned
    true           Assigned
  </table>
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
  </table>
  <group TD_SysVars_D>
*/
//<dom_var_def>bool DWGTITLED;
QVAR_DEF_RO(ODTBOOL,              DWGTITLED,        ValidateNone()    )

QVAR_DEF   (ODTDIST,              ELEVATION,        ValidateNone()    )
QVAR_DEF_RO(OdGePoint3d,          EXTMAX,           ValidateNone()    )
QVAR_DEF_RO(OdGePoint3d,          EXTMIN,           ValidateNone()    )

/**
  <title FRONTZ>
  <toctitle FRONTZ>
   
  <table>
    <b>Type</b>            double
    <b>Initial value</b>   0.0
    <b>Read-Only</b>       Yes
    <b>Saved In</b>        Database
    <b>Versions</b>        R12+
  </table>
   
  FRONTZ specifies the front clipping distance for the current viewport.
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
  </table>
  
  \sa
  <link VIEWMODE, VIEWMODE>
  <group TD_SysVars_F>
*/
//<dom_var_def>double FRONTZ;
QVAR_DEF_RO(ODTDIST,              FRONTZ,           ValidateNone()    )

/**
  <title GRIDMODE>
  <toctitle GRIDMODE>
   
  <table>
    <b>Type</b>            bool
    <b>Initial value</b>   false
    <b>Read-Only</b>       No
    <b>Saved In</b>        Database
    <b>Versions</b>        R12+
  </table>
   
  GRIDMODE controls the display of the grid in the current viewport.
   
  <table>
    <b>Value</b>   <b>Description</b>
    false          Not displayed
    true           Displayed
  </table>
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>
  <group TD_SysVars_G>
*/
//<dom_var_def>bool GRIDMODE;
QVAR_DEF   (ODTBOOL,              GRIDMODE,         ValidateBOOL()    )

/**
  <title GRIDUNIT>
  <toctitle GRIDUNIT>
   
  <table>
    <b>Type</b>            OdGePoint2d
    <b>Initial value</b>   (0.5, 0.5) | (10.0, 10.0)
    <b>Read-Only</b>       No
    <b>Saved In</b>        Database
    <b>Versions</b>        R12+
  </table>
   
  GRIDUNIT specifies the XY grid spacing in the current viewport.
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>
  <group TD_SysVars_G>
*/
//<dom_var_def>OdGePoint2d GRIDUNIT;
QVAR_DEF   (OdGePoint2d,          GRIDUNIT,         ValidateNone()    )

QVAR_DEF   (OdGePoint3d,          INSBASE,          ValidateNone()    )
QVAR_DEF_RO(ODTDIST,              LENSLENGTH,       ValidateNone()    )
QVAR_DEF   (ODTBOOL,              LIMCHECK,         ValidateBOOL()    )
QVAR_DEF   (OdGePoint2d,          LIMMAX,           ValidateNone()    )
QVAR_DEF   (OdGePoint2d,          LIMMIN,           ValidateNone()    )

/**
  <title SNAPANG>
  <toctitle SNAPANG>
   
  <table>
    <b>Type</b>            double
    <b>Initial value</b>   0.0
    <b>Read-Only</b>       No
    <b>Saved In</b>        Database
    <b>Versions</b>        R12+
  </table>
   
  SNAPANG controls the rotation in the current UCS of the snap and grid in the current viewport.
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>
   
  <b>Note: </b>All angles are expressed in radians.
  <group TD_SysVars_S>
*/
//<dom_var_def>double SNAPANG;
QVAR_DEF   (ODTANGLE,             SNAPANG,          ValidateNone()    )

/**
  <title SNAPBASE>
  <toctitle SNAPBASE>
   
  <table>
    <b>Type</b>            OdGePoint2d
    <b>Initial value</b>   (0.0, 0.0)
    <b>Read-Only</b>       No
    <b>Saved In</b>        Database
    <b>Versions</b>        R12+
  </table>
   
  SNAPBASE controls the origin in the current UCS of the snap and grid in the current viewport.
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>
  <group TD_SysVars_S>
*/
//<dom_var_def>OdGePoint2d SNAPBASE;
QVAR_DEF   (OdGePoint2d,          SNAPBASE,         ValidateNone()    )

/**
  <title SNAPISOPAIR>
  <toctitle SNAPISOPAIR>
   
  <table>
    <b>Type</b>            OdInt16
    <b>Initial value</b>   0
    <b>Read-Only</b>       No
    <b>Saved In</b>        Database
    <b>Versions</b>        R12+
  </table>
   
  SNAPISOPAIR specifies the isometric plane for the current viewport.
   
  <table>
    <b>Value</b>   <b>Description</b>
    0              Left
    1              Top
    2              Bottom
  </table>
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>
  
  \sa
  <link SNAPSTYL, SNAPSTYL>
  <group TD_SysVars_S>
*/
//<dom_var_def>OdInt16 SNAPISOPAIR;
QVAR_DEF   (OdInt16,              SNAPISOPAIR,      ValidateNone()    )

/**
  <title SNAPMODE>
  <toctitle SNAPMODE>
   
  <table>
    <b>Type</b>            OdInt16
    <b>Initial value</b>   0
    <b>Read-Only</b>       No
    <b>Saved In</b>        Database
    <b>Versions</b>        R12+
  </table>
   
  SNAPMODE controls the snap mode in the current viewport.
   
  <table>
    <b>Value</b>   <b>Description</b>
    0              Disabled
    1              Enabled
  </table>
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>
  <group TD_SysVars_S>
*/
//<dom_var_def>OdInt16 SNAPMODE;
QVAR_DEF   (ODTBOOL,              SNAPMODE,         ValidateBOOL()    )

/**
  <title SNAPSTYL>
  <toctitle SNAPSTYL>
   
  <table>
    <b>Type</b>            OdInt16
    <b>Initial value</b>   0
    <b>Read-Only</b>       No
    <b>Saved In</b>        Database
    <b>Versions</b>        R12+
  </table>
   
  SNAPSTYL controls the snap style in the current viewport.
   
  <table>
    <b>Value</b>   <b>Description</b>
    0              Rectangular snap
    1              Isometric snap
  </table>
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>
  
  \sa
  <link SNAPISOPAIR, SNAPISOPAIR>
  <group TD_SysVars_S>
*/
//<dom_var_def>OdInt16 SNAPSTYL;
QVAR_DEF   (ODTBOOL,              SNAPSTYL,         ValidateBOOL()    )

/**
  <title SNAPUNIT>
  <toctitle SNAPUNIT>
   
  <table>
    <b>Type</b>            OdGePoint2d
    <b>Initial value</b>   (0.5, 0.5) | (10.0, 10.0)
    <b>Read-Only</b>       No
    <b>Saved In</b>        Database
    <b>Versions</b>        R12+
  </table>
   
  SNAPUNIT specifies the XY snap spacing in the current viewport.
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>
  <group TD_SysVars_S>
*/
//<dom_var_def>OdGePoint2d SNAPUNIT;
QVAR_DEF   (OdGePoint2d,          SNAPUNIT,         ValidateNone()    )

/**
  <title SYSCODEPAGE>
  <toctitle SYSCODEPAGE>
   
  <table>
    <b>Type</b>            OdCodePageId
    <b>Initial value</b>   Varies
    <b>Read-Only</b>       Yes
    <b>Saved In</b>        Nondatabase, Quasi variable
    <b>Versions</b>        R12+
  </table>
   
  The SYSCODEPAGE variable specifies the operating system's code page. The value can be:
   
  <table>
   <b>Name</b>    <b>Value</b>    <b>Description</b>
    CP_UNDEFINED  0	              undefined
    CP_ASCII	    1	              ascii
    CP_8859_1	    2	              iso8859-1
    CP_8859_2	    3	              iso8859-2
    CP_8859_3	    4	              iso8859-3
    CP_8859_4	    5	              iso8859-4
    CP_8859_5	    6	              iso8859-5
    CP_8859_6	    7	              iso8859-6
    CP_8859_7	    8	              iso8859-7
    CP_8859_8	    9	              iso8859-8
    CP_8859_9	    10	            iso8859-9
    CP_DOS437	    11	            dos437
    CP_DOS850	    12	            dos850
    CP_DOS852	    13	            dos852
    CP_DOS855	    14	            dos855
    CP_DOS857	    15	            dos857
    CP_DOS860	    16	            dos860
    CP_DOS861	    17	            dos861
    CP_DOS863	    18	            dos863
    CP_DOS864	    19	            dos864
    CP_DOS865	    20	            dos865
    CP_DOS869	    21	            dos869
    CP_DOS932	    22	            dos932
    CP_MACINTOSH	23	            mac-roman
    CP_BIG5	      24	            big5
    CP_KSC5601	  25	            ksc5601
    CP_JOHAB	    26	            johab
    CP_DOS866	    27	            dos866
    CP_ANSI_1250	28	            ansi 1250
    CP_ANSI_1251	29	            ansi 1251
    CP_ANSI_1252	30	            ansi 1252
    CP_GB2312	    31	            gb2312
    CP_ANSI_1253	32	            ansi 1253
    CP_ANSI_1254	33	            ansi 1254
    CP_ANSI_1255	34	            ansi 1255
    CP_ANSI_1256	35	            ansi 1256
    CP_ANSI_1257	36	            ansi 1257
    CP_ANSI_874	  37	            ansi 874
    CP_ANSI_932	  38	            ansi 932
    CP_ANSI_936	  39	            ansi 936
    CP_ANSI_949	  40	            ansi 949
    CP_ANSI_950	  41	            ansi 950
    CP_ANSI_1361	42	            ansi 1361
    CP_ANSI_1200	43	            ansi 1200
    CP_ANSI_1258	44	            ansi 1258
   </table>
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    &nbsp;
    OdDbHostAppServices::setSystemCodePage()    
  </table>
   
  \sa
  <link OdCodePageId, OdCodePageId> Enumeration.
  <group TD_SysVars_S>
*/
//<dom_var_def>OdCodePageId SYSCODEPAGE;
QVAR_DEF_RO(OdCodePageId,         SYSCODEPAGE,      ValidateNone()    )

/**
  <title TARGET>
  <toctitle TARGET>
   
  <table>
    <b>Type</b>            OdGePoint3d
    <b>Initial value</b>   (0.0, 0.0, 0.0)
    <b>Read-Only</b>       Yes
    <b>Saved In</b>        Database
    <b>Versions</b>        R12+
    </table>
   
  TARGET specifies the target point for the current viewport.
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
  </table>
  
  <group TD_SysVars_T>
*/
//<dom_var_def>OdGePoint3d TARGET;
QVAR_DEF_RO(OdGePoint3d,          TARGET,           ValidateNone()    )

/**
  <title UCSFOLLOW>
  <toctitle UCSFOLLOW>
   
  <table>
    <b>Type</b>            bool
    <b>Initial value</b>   false
    <b>Read-Only</b>       No
    <b>Saved In</b>        Database
    <b>Versions</b>        R12+
  </table>
   
  UCSFOLLOW controls the generation of plan views in the current viewport whenever the UCS changes.
   
  <table>
    <b>Value</b>   <b>Description</b>
    false          View unchanged
    true           View changes to plan view in the new UCS
  </table>
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>
  <group TD_SysVars_U>
  */
//<dom_var_def>bool UCSFOLLOW;
QVAR_DEF   (ODTBOOL,              UCSFOLLOW,        ValidateBOOL()    )

/**
  <title UCSICON>
  <toctitle UCSICON>
   
  <table>
    <b>Type</b>            OdInt16
    <b>Initial value</b>   3
    <b>Read-Only</b>       No
    <b>Saved In</b>        Database
    <b>Versions</b>        R12+
  </table>
   
  UCSICON controls the display of the UCS icon.
   
  <table>
    <b>Value</b>   <b>At Origin</b>   <b>On</b>
    0              No                 No
    1              No                 Yes
    2              Yes                No
    3              Yes                Yes
  </table>
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>
  <group TD_SysVars_U>
*/
//<dom_var_def>OdInt16 UCSICON;
QVAR_DEF   (OdInt16,              UCSICON,          ValidateNone()    )

/**
  <title UCSVP>
  <toctitle UCSVP>
   
  <table>
    <b>Type</b>            bool
    <b>Initial value</b>   false
    <b>Read-Only</b>       No
    <b>Saved In</b>        Database
    <b>Versions</b>        2000+
  </table>
   
  UCSVP controls the saving of the current UCS with the current viewport.
   
  <table>
    <b>Value</b>   <b>Description</b>
    false          UCS associated with the current viewport is unchanged.
    true           Saves the UCS with the current viewport.
  </table>
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>
  <group TD_SysVars_U>  
*/
//<dom_var_def>bool UCSVP;
QVAR_DEF   (ODTBOOL,              UCSVP,            ValidateBOOL()    )

QVAR_DEF_RO(ODTUCSNAME,           UCSNAME,          ValidateNone()    )
QVAR_DEF_RO(OdGePoint3d,          UCSORG,           ValidateNone()    )
QVAR_DEF_RO(OdGeVector3d,         UCSXDIR,          ValidateNone()    )
QVAR_DEF_RO(OdGeVector3d,         UCSYDIR,          ValidateNone()    )

/**
  <title VIEWCTR>
  <toctitle VIEWCTR>
   
  <table>
    <b>Type</b>            OdGePoint3d
    <b>Initial value</b>   Varies
    <b>Read-Only</b>       Yes
    <b>Saved In</b>        Database
    <b>Versions</b>        R12+
  </table>
   
  VIEWCTR specifies the UCS center of the view for the current viewport.
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
  </table> 
  <group TD_SysVars_V>
*/
//<dom_var_def>OdGePoint3d VIEWCTR;
QVAR_DEF_RO(OdGePoint3d,          VIEWCTR,          ValidateNone()    )

/**
  <title VIEWDIR>
  <toctitle VIEWDIR>
   
  <table>
    <b>Type</b>            OdGePoint3d
    <b>Initial value</b>   (0.0, 0.0, 1.0)
    <b>Read-Only</b>       Yes
    <b>Saved In</b>        Database
    <b>Versions</b>        R12+
  </table>
   
  VIEWDIR specifies the vector from the target point for the current viewport to the camera point.
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
  </table> 
  <group TD_SysVars_V>
*/
//<dom_var_def>OdGeVector3d VIEWDIR;
QVAR_DEF_RO(OdGeVector3d,         VIEWDIR,          ValidateNone()    )

/**
  <title VIEWMODE>
  <toctitle VIEWMODE>
   
  <table>
    <b>Type</b>            OdInt16
    <b>Initial value</b>   0
    <b>Read-Only</b>       Yes
    <b>Saved In</b>        Database
    <b>Versions</b>        R12+
  </table>
   
  VIEWMODE specifies the view mode for the current viewport. VIEWMODE will be the sum of 
  zero or more of the following:
   
  <table>
    <b>Value</b>   <b>Description</b>
    0x00           Disabled
    0x01           Perspective enabled
    0x02           Front clip enabled
    0x04           Back clip enabled
    0x08           UCS follow enabled
    0x10           Front clipping plane not at eye
  </table>
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
  </table>
  <group TD_SysVars_V>
*/
//<dom_var_def>OdInt16 VIEWMODE;
QVAR_DEF_RO(OdInt16,              VIEWMODE,         ValidateNone()    )

/**
  <title VIEWSIZE>
  <toctitle VIEWSIZE>
   
  <table>
    <b>Type</b>            double
    <b>Initial value</b>   Varies
    <b>Read-Only</b>       Yes
    <b>Saved In</b>        Database
    <b>Versions</b>        R12+
  </table>
   
  VIEWSIZE specifies the UCS height of the view for the current viewport.
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
  </table> 
  <group TD_SysVars_V>
*/
//<dom_var_def>double VIEWSIZE;
QVAR_DEF_RO(ODTDIST,              VIEWSIZE,         ValidateNone()    )

/**
  <title VIEWTWIST>
  <toctitle VIEWTWIST>
   
  <table>
    <b>Type</b>            double
    <b>Initial value</b>   0.0
    <b>Read-Only</b>       Yes
    <b>Saved In</b>        Database
    <b>Versions</b>        R12+
  </table>
   
  VIEWTWIST specifies the view twist for the current viewport.
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
  </table>
  
  <b>Note: </b>All angles are expressed in radians.
  <group TD_SysVars_V>
*/
//<dom_var_def>double VIEWTWIST;
QVAR_DEF_RO(ODTORIENT,            VIEWTWIST,        ValidateNone()    )

/**
  <title WORLDUCS>
  <toctitle WORLDUCS>
   
  <table>
    <b>Type</b>            OdInt16
    <b>Initial value</b>   1
    <b>Read-Only</b>       Yes
    <b>Saved In</b>        Not saved
    <b>Versions</b>        R12+
  </table>
   
  WORLDUCS indicates if the current UCS is the same as the WCS.
   
  <table>
    <b>Value</b>   <b>Description</b>
    0              Different
    1              Same
  </table>
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
  </table> 
  <group TD_SysVars_W>
*/
//<dom_var_def>OdInt16 WORLDUCS;
QVAR_DEF_RO(OdInt16,              WORLDUCS,         ValidateNone()    )

QVAR_DEF_RO(OdString,             LOGINNAME,        ValidateNone()    )
QVAR_DEF_RO(OdDbDate,             DATE,             ValidateNone()    )
QVAR_DEF_RO(double,               CDATE,            ValidateNone()    )

//-----------------------------------------
// Variables added in AC 2008
//-----------------------------------------

/**
  <title CANNOSCALEVALUE>
  <toctitle CANNOSCALEVALUE>
   
  <table>
    <b>Type</b>            double
    <b>Initial value</b>   1.0
    <b>Read-Only</b>       Yes
    <b>Saved In</b>        Drawing
    <b>Versions</b>        2008+
  </table>
   
  The CANNOSCALEVALUE variable stores a default value of the annotation scale to be used in the current drawing.
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
  </table>
  <group TD_SysVars_C>
*/
//<dom_var_def>double CANNOSCALEVALUE;
QVAR_DEF_RO(double,               CANNOSCALEVALUE,  ValidateNone()    )
QVAR_DEF   (ODTBOOL,              ANNOALLVISIBLE,   ValidateBOOL()    )

/**
  <title DIMANNO>
  <toctitle DIMANNO>
   
  <table>
    <b>Type</b>            bool
    <b>Initial value</b>   false
    <b>Read-Only</b>       Yes
    <b>Saved In</b>        Database
    <b>Versions</b>        2008+
  </table>
   
  DIMANNO indicates whether or not the current dimension style is annotative.
   
  <table>
    <b>Value</b>   <b>Description</b>
    false          Current dimension style is not annotative.
    true           Current dimension style is annotative.
  </table>
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    &nbsp;
    OdDbDatabase::getDIMANNO()
  </table>
  
  <group TD_SysVars_D>
*/
//<dom_var_def>bool DIMANNO;
QVAR_DEF_RO(ODTBOOL,              DIMANNO,          ValidateNone()    )

/* TODO:
 GRIDMAJOR
*/

QVAR_DEF   (ODTBOOL,              PERSPECTIVE,      ValidateNone()    )
QVAR_DEF   (OdInt16,              DIMUNIT,          ValidateNone()    )

//-----------------------------------------
// Variables added in AC 2010 
//-----------------------------------------

/** * VARDOCS:
  <properties>
    <remarks>
      "\n    IMAGEFRAME Controls whether image frames are displayed and plotted"
      "\n"
      "\n    @table"
      "\n    Value   Description"
      "\n    0       Invisible"
      "\n    1       Is displayed and plotted"
      "\n    2       Is displayed but not plotted"
    </remarks>
  </properties>
*/
QVAR_DEF   (OdInt16,              IMAGEFRAME,          ValidateNone()    )
QVAR_DEF   (OdInt16,              FRAME,               ValidateNone()    )
QVAR_DEF   (OdInt16,              PDFFRAME,            ValidateNone()    )
QVAR_DEF   (OdInt16,              XCLIPFRAME,          ValidateNone()    )
QVAR_DEF   (OdInt8,               DWFFRAME,            ValidateNone()    )
QVAR_DEF   (OdInt8,               DGNFRAME,            ValidateNone()    )
QVAR_DEF   (OdInt8,               OLEFRAME,            ValidateNone()    )
QVAR_DEF   (OdInt16,              WIPEOUTFRAME,        ValidateNone()    )
QVAR_DEF   (OdInt16,              POINTCLOUDCLIPFRAME, ValidateNone()    )

QVAR_DEF   (ODTTEXTSTYLEID,       TEXTSTYLE,           ValidateTextStyle())
QVAR_DEF   (double,               TEXTSIZE,            ValidateNone()    )

/**
  <title GRIDDISPLAY>
  <toctitle GRIDDISPLAY>
   
  <table>
    <b>Type</b>            OdInt16
    <b>Initial value</b>   2
    <b>Range</b>           0 ... 15
    <b>Read-Only</b>       No
    <b>Saved In</b>        Nondatabase, Quasi variable
    <b>Versions</b>        2007+
  </table>
   
  The GRIDDISPLAY variable controls the display behavior and display limits of the grid. 
  The setting is stored as a bitcode using the sum of the following values:
   
  <table>
    <b>Value</b>   <b>Description</b>
    0              Restricts the grid to the area specified by the LIMITS command.
    1              Does not restrict the grid to the area specified by the LIMITS command.
    2              Turns on adaptive grid display, which limits the density of the grid when zoomed out.
    4              If the grid is set to adaptive display and when zoomed in, generates additional, more closely spaced grid lines in the same proportion as the intervals of the major grid lines.
    8              Changes the grid plane to follow the XY plane of the dynamic UCS (if value is 2, this setting is ignored).
  </table>
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>
  
  <group TD_SysVars_G>
*/
//<dom_var_def>OdInt16 GRIDDISPLAY;
QVAR_DEF   (OdInt16,              GRIDDISPLAY,         ValidateNone()    )

/**
  <title GRIDMAJOR>
  <toctitle GRIDMAJOR>
   
  <table>
    <b>Type</b>            OdInt16
    <b>Initial value</b>   5
    <b>Read-Only</b>       No
    <b>Saved In</b>        Nondatabase, Quasi variable
    <b>Versions</b>        2007+
  </table>
   
  The GRIDMAJOR variable specifies the frequency of major grid lines compared to minor grid lines 
  in the range 1 to 100.
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>
   
  For example, to get the value:
  <code>
OdRusBufPtr pRb = pDb-\>getSysVar(L"GRIDMAJOR");
OdInt16 major = pRb-\>getInt16();
odPrintConsoleString(L"nGrid major = %d", major);
  </code>
  For example, to set the value:
  <code>
OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtInt16, 10);
pDb-\>setSysVar(L"GRIDMAJOR", pRb);
  </code>                                                      
  
  <group TD_SysVars_G>
*/
//<dom_var_def>OdInt16 GRIDMAJOR;
QVAR_DEF   (OdInt16,              GRIDMAJOR,           ValidateNone()    )

QVAR_DEF   (OdInt16,              VSBACKGROUNDS,       ValidateNone()    )

/**
  <title VSEDGECOLOR>
  <toctitle VSEDGECOLOR>
   
  <table>
    <b>Type</b>            OdString
    <b>Initial value</b>   "ByEntity"
    <b>Read-Only</b>       No
    <b>Saved In</b>        Database
    <b>Versions</b>        2007+
  </table>
   
  The VSEDGECOLOR variable stores the tint of edges in the visual style for the current viewport. 
  TrueColor value is set as follows: "RGB:XXX,XXX,XXX", where XXX is the color component value in 
  the range 0 to 255. Value 0 defines a color ByBlock, value 256 defines a color ByLayer, value 257
  defines a color ByEntity, values 1 to 255 define the ACI color.
   
  <b>Note: </b>An existing visual style is not changed when the system variable gets a new value. 
  Temporary visual style will be created as a clone of visual style assigned for current viewport.
  
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>
   
  For example, to get the value:
  <code>
OdRusBufPtr pRb = pDb-\>getSysVar(L"VSEDGECOLOR");
OdString tint = pRb-\>getString();
odPrintConsoleString(L"nColor of edges = %s", tint);
  </code>
  For example, to set the value:
  <code>
OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtString, L"RGB:128,128,128");
pDb-\>setSysVar(L"VSEDGECOLOR", pRb);
 </code>

  <group TD_SysVars_G>
*/
//<dom_var_def>OdString VSEDGECOLOR;
QVAR_DEF   (OdString,             VSEDGECOLOR,         ValidateNone()    )

/**
  <title VSEDGEJITTER>
  <toctitle VSEDGEJITTER>
   
  <table>
    <b>Type</b>            OdInt16
    <b>Initial value</b>   (–2)
    <b>Read-Only</b>       No
    <b>Saved In</b>        Database
    <b>Versions</b>        2010+
  </table>
   
  The VSEDGEJITTER variable switches the display of edges to be viewed with several linear streaks, 
  as if the edges were sketched using a pencil. The value defines the number of jitters which can be:
   
  <table>
    <b>Value</b>   <b>Description</b>
    1              Low jitters
    2              Medium jitters
    3              High jitters
  </table>
  
  A negative value turns off the sketch effect, but the variable continues to store the number of jitters.
   
  <b>Note: </b>An existing visual style is not changed when the system variable gets a new value. 
  Temporary visual style will be created as a clone of visual style assigned for current
  viewport.
  
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>
   
  For example, to get the value:
  <code>
OdRusBufPtr pRb = pDb-\>getSysVar(L"VSEDGEJITTER");
OdInt16 jitters = pRb-\>getInt16();
odPrintConsoleString(L"nJitters = %d", jitters);
  </code>
  For example, to set the value:
  <code>
OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtInt16, 1);
pDb-\>setSysVar(L"VSEDGEJITTER", pRb)
  </code>                                                      

  <group TD_SysVars_V>
*/
//<dom_var_def>OdInt16 VSEDGEJITTER;
QVAR_DEF   (OdInt16,              VSEDGEJITTER,        ValidateNone()    )

/**
  <title VSEDGELEX>
  <toctitle VSEDGELEX>
   
  <table>
    <b>Type</b>            OdInt16
    <b>Initial value</b>   (–6)
    <b>Read-Only</b>       No
    <b>Saved In</b>        Nondatabase, Quasi variable
    <b>Versions</b>        2011+
  </table>
   
  The VSEDGELEX variable allows edges to extend beyond their intersection for 3D entities. 
  The admissible range is 1 to 100 pixels. A negative value turns off the line extensions
  effect, but the variable continues to store the previous setting.
   
  <b>Note: </b>An existing visual style is not changed when the system variable gets a new value. 
  Temporary visual style will be created as a clone of visual style assigned for current viewport.
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>
   
  For example, to get the value:
  <code>
OdRusBufPtr pRb = pDb-\>getSysVar(L"VSEDGELEX");
OdInt16 effect = pRb-\>getInt16();
odPrintConsoleString(L"nLine extensions effect = %d", effect);
  </code>
  For example, to set the value:
  <code>
OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtInt16, 10);
pDb-\>setSysVar(L"VSEDGELEX", pRb);
  </code>

  <group TD_SysVars_V>
*/
//<dom_var_def>OdInt16 VSEDGELEX;
QVAR_DEF   (OdInt16,              VSEDGELEX,           ValidateNone()    )

/**
  <title VSEDGEOVERHANG>
  <toctitle VSEDGEOVERHANG>
   
  <table>
    <b>Type</b>            OdInt16
    <b>Initial value</b>   (–6)
    <b>Read-Only</b>       No
    <b>Saved In</b>        Nondatabase, Quasi variable
    <b>Versions</b>        2007+
  </table>
   
  The VSEDGEOVERHANG variable overhangs edges that extend beyond their intersections for 3D entities. 
  The value can be in the range 1 to 100 pixels. A negative value turns off the overhang effect, 
  but the variable continues to store the setting.
   
  <b>Note: </b>An existing visual style is not changed when the system variable gets a new value. 
  Temporary visual style will be created as a clone of visual style assigned for current viewport.
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>
   
  For example, to get the value:
  <code>
OdRusBufPtr pRb = pDb-\>getSysVar(L"VSEDGEOVERHANG");
OdInt16 effect = pRb-\>getInt16();
odPrintConsoleString(L"nOverhang effect = %d", effect);
  </code>
  For example, to set the value:
  <code>
OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtInt16, 10);
pDb-\>setSysVar(L"VSEDGEOVERHANG", pRb);
  </code> 
  
  <group TD_SysVars_V>
*/
//<dom_var_def>OdInt16 VSEDGEOVERHANG;
QVAR_DEF   (OdInt16,              VSEDGEOVERHANG,      ValidateNone()    )

/**
  <title VSEDGES>
  <toctitle VSEDGES>
   
  <table>
    <b>Type</b>            OdInt16
    <b>Initial value</b>   1
    <b>Read-Only</b>       No
    <b>Saved In</b>        Nondatabase, Quasi variable
    <b>Versions</b>        2007+
  </table>
   
  The VSEDGES variable stores the type of edges which are used for displaying in the current viewport. 
  The value can be:
   
  <table>
    <b>Value</b>   <b>Description</b>
    0              Edges are not displayed
    1              Isolines of edges are displayed
    2              Facet edges are displayed
  </table>
  
  <b>Note: </b>An existing visual style is not changed when the system variable gets a new value. 
  Temporary visual style will be created as a clone of visual style assigned for current viewport.
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>
   
  For example, to get the value:
  <code>
OdRusBufPtr pRb = pDb-\>getSysVar(L"VSEDGES");
OdInt16 type = pRb-\>getInt16();
odPrintConsoleString(L"nEdge type = %d", type);
  </code>
  For example, to set the value:
  <code>
OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtInt16, 1);
pDb-\>setSysVar(L"VSEDGES", pRb);
  </code>

  <group TD_SysVars_V>
*/
//<dom_var_def>OdInt16 VSEDGES;
QVAR_DEF   (OdInt16,              VSEDGES,             ValidateNone()    )

/**
  <title VSEDGESMOOTH>
  <toctitle VSEDGESMOOTH>
   
  <table>
    <b>Type</b>            OdInt16
    <b>Initial value</b>   1
    <b>Read-Only</b>       No
    <b>Saved In</b>        Nondatabase, Quasi variable
    <b>Versions</b>        2007+
  </table>
   
  The VSEDGESMOOTH variable stores the angle at which edges are displayed in the current viewport. 
  The value can be in the range 0 to 180 degrees.
   
  <b>Note: </b>An existing visual style is not changed when the system variable gets a new value. 
  Temporary visual style will be created as a clone of visual style assigned for current viewport.
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>
   
  For example, to get the value:
  <code>
OdRusBufPtr pRb = pDb-\>getSysVar(L"VSEDGESMOOTH");
OdInt16 angle = pRb-\>getInt16();
odPrintConsoleString(L"nAngle = %d", angle);
  </code>
  For example, to set the value:
  <code>
OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtInt16, 90);
pDb-\>setSysVar(L"VSEDGESMOOTH", pRb);
  </code> 

  <group TD_SysVars_V>
*/
//<dom_var_def>OdInt16 VSEDGESMOOTH;
QVAR_DEF   (OdInt16,              VSEDGESMOOTH,        ValidateNone()    )

/**
  <title VSFACECOLORMODE>
  <toctitle VSFACECOLORMODE>
   
  <table>
    <b>Type</b>            OdInt16
    <b>Initial value</b>   0
    <b>Read-Only</b>       No
    <b>Saved In</b>        Nondatabase, Quasi variable
    <b>Versions</b>        2007+
  </table>
   
  The VSFACECOLORMODE variable stores the mode that defines how to calculate the face color. 
  The value can be:
   
  <table>
    <b>Value</b>   <b>Description</b>
    0              Normal mode – face color is not used.
    1              Monochrome mode – uses the color from VSMONOCOLOR system variable to display all faces.
    2              Tint mode – uses the color from VSMONOCOLOR system variable to shade all faces by changing the tint and saturation of colors.
    3              Desaturate mode – softens the color of all faces by reducing its saturation (30 percent).
  </table>
  
  <b>Note: </b> An existing visual style is not changed when the system variable gets a new value. 
  Temporary visual style will be created as a clone of visual style assigned for current viewport.
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>
   
  For example, to get the value:
  <code>
OdRusBufPtr pRb = pDb-\>getSysVar(L"VSFACECOLORMODE");
OdInt16 mode = pRb-\>getInt16();
odPrintConsoleString(L"nFace color mode = %d", mode);
  </code>
  For example, to set the value:
  <code>
OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtInt16, 1);
pDb-\>setSysVar(L"VSFACECOLORMODE", pRb);
  </code> 

  <group TD_SysVars_V>
*/
//<dom_var_def>OdInt16 VSFACECOLORMODE;
QVAR_DEF   (OdInt16,              VSFACECOLORMODE,     ValidateNone()    )

/**
  <title VSFACEHIGHLIGHT>
  <toctitle VSFACEHIGHLIGHT>
   
  <table>
    <b>Type</b>            OdInt16
    <b>Initial value</b>   (–30)
    <b>Read-Only</b>       No
    <b>Saved In</b>        Nondatabase, Quasi variable
    <b>Versions</b>        2007+
  </table>
   
  The VSFACEHIGHLIGHT variable stores the reflex highlights of faces without materials to be displayed 
  in the current viewport. The value can be in the range &#8211;100 to 100. The higher the value, the 
  larger the highlight. The smaller the value, the smaller the highlight. If the VSMATERIALMODE
  system variable is on, attached materials ignore the highlight.
   
  <b>Note: </b> An existing visual style is not changed when the system variable gets a new value. 
  Temporary visual style will be created as a clone of visual style assigned for current viewport.
  
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>
   
  For example, to get the value:
  <code>
OdRusBufPtr pRb = pDb-\>getSysVar(L"VSFACEHIGHLIGHT");
OdInt16 highlight = pRb-\>getInt16();
odPrintConsoleString(L"nFace highlight = %d", highlight);
  </code>
  For example, to set the value:
  <code>
OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtInt16, 60);
pDb-\>setSysVar(L"VSFACEHIGHLIGHT", pRb);
  </code>  

  <group TD_SysVars_V>
*/
//<dom_var_def>OdInt16 VSFACEHIGHLIGHT;
QVAR_DEF   (OdInt16,              VSFACEHIGHLIGHT,     ValidateNone()    )

/**
  <title VSFACEOPACITY>
  <toctitle VSFACEOPACITY>
   
  <table>
    <b>Type</b>            OdInt16
    <b>Initial value</b>   (–60)
    <b>Read-Only</b>       No
    <b>Saved In</b>        Nondatabase, Quasi variable
    <b>Versions</b>        2007+
  </table>
   
  The VSFACEOPACITY variable switches the preset level of transparency for 3D entities. 
  The value can be in the range 0 to 100 percent. The value 100 is limpid, the value 0 is
  opaque. A negative value stores the transparency but turns off the effect.
   
  <b>Note: </b>An existing visual style is not changed when the system variable gets a new value. 
  Temporary visual style will be created as a clone of visual style assigned for current viewport.
  
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>
   
  For example, to get the value:
  <code>
OdRusBufPtr pRb = pDb-\>getSysVar(L"VSFACEOPACITY");
OdInt16 transparency = pRb-\>getInt16();
odPrintConsoleString(L"nTransparency level = %d", transparency);
  </code>
  For example, to set the value:
  <code>
OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtInt16, 50);
pDb-\>setSysVar(L"VSFACEOPACITY", pRb);
  </code> 

  <group TD_SysVars_V>
*/
//<dom_var_def>OdInt16 VSFACEOPACITY;
QVAR_DEF   (OdInt16,              VSFACEOPACITY,       ValidateNone()    )

/**
  <title VSFACESTYLE>
  <toctitle VSFACESTYLE>
   
  <table>
    <b>Type</b>            OdInt16
    <b>Initial value</b>   0
    <b>Read-Only</b>       No
    <b>Saved In</b>        Nondatabase, Quasi variable
    <b>Versions</b>        2007+
  </table>
   
  The VSFACESTYLE variable stores the face mode that defines how faces display in the current viewport. 
  The value can be:
   
  <table>
    <b>Value</b>   <b>Description</b>
    0              Off – style is not applied.
    1              Real – defines how the face would appear in the real world mode.
    2              Gooch – uses cool and warm colors instead of dark and light to display the faces which might be shadowed and difficult to review in the realistic mode.
  </table>
   
  <b>Note: </b>An existing visual style is not changed when the system variable gets a new value. 
  Temporary visual style will be created as a clone of visual style assigned for current viewport.
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>

  For example, to get the value:
  <code>
OdRusBufPtr pRb = pDb-\>getSysVar(L"VSFACESTYLE");
OdInt16 mode = pRb-\>getInt16();
odPrintConsoleString(L"nFace mode = %d", mode);
  </code>
  For example, to set the value:
  <code>
OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtInt16, 1);
pDb-\>setSysVar(L"VSFACESTYLE", pRb);
  </code>  

  <group TD_SysVars_V>
*/
//<dom_var_def>OdInt16 VSFACESTYLE;
QVAR_DEF   (OdInt16,              VSFACESTYLE,         ValidateNone()    )

/**
  <title VSHALOGAP>
  <toctitle VSHALOGAP>
   
  <table>
    <b>Type</b>            OdInt16
    <b>Initial value</b>   0
    <b>Read-Only</b>       No
    <b>Saved In</b>        Nondatabase, Quasi variable
    <b>Versions</b>        2007+
  </table>
   
  The VSHALOGAP variable stores the aureole gap in the visual style for the current viewport. 
  The value can be in the range 0 to 100.
   
  <b>Note: </b>An existing visual style is not changed when the system variable gets a new value. 
  Temporary visual style will be created as a clone of visual style assigned for current viewport.
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>
   
  For example, to get the value:
  <code>
OdRusBufPtr pRb = pDb-\>getSysVar(L"VSHALOGAP");
OdInt16 gap = pRb-\>getInt16();
odPrintConsoleString(L"nAureole gap = %d", gap);
  </code>
  For example, to set the value:
  <code>
OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtInt16, 60);
pDb-\>setSysVar(L"VSHALOGAP", pRb);
 </code>

  <group TD_SysVars_V>
*/
//<dom_var_def>OdInt16 VSHALOGAP;
QVAR_DEF   (OdInt16,              VSHALOGAP,           ValidateNone()    )

/**
  <title VSINTERSECTIONCOLOR>
  <toctitle VSINTERSECTIONCOLOR>
   
  <table>
    <b>Type</b>            OdString
    <b>Initial value</b>   "7"
    <b>Read-Only</b>       No
    <b>Saved In</b>        Nondatabase, Quasi variable
    <b>Versions</b>        2007+
  </table>
   
  The VSINTERSECTIONCOLOR variable stores the tint of intersection polylines in the visual style 
  for the current viewport when it is set to 2D Wireframe. Value 0 defines a color ByBlock, 
  value 256 defines a color ByLayer, value 257 defines a color ByEntity, values 1 to 255 
  define the ACI color.
   
  <b>Note: </b>An existing visual style is not changed when the system variable gets a new value. 
  Temporary visual style will be created as a clone of visual style assigned for current viewport.
  
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>
   
  For example, to get the value:
  <code>
OdRusBufPtr pRb = pDb-\>getSysVar(L"VSINTERSECTIONCOLOR");
OdString tint = pRb-\>getString();
odPrintConsoleString(L"nColor of intersection = %s", tint);
  </code>
  For example, to set the value:
  <code>
OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtString, L"128");
pDb-\>setSysVar(L"VSINTERSECTIONCOLOR", pRb);
  </code> 

  <group TD_SysVars_V>
 */
//<dom_var_def>OdString VSINTERSECTIONCOLOR;
QVAR_DEF   (OdString,             VSINTERSECTIONCOLOR, ValidateNone()    )

/**
  <title VSINTERSECTIONEDGES>
  <toctitle VSINTERSECTIONEDGES>
   
  <table>
    <b>Type</b>            OdInt16
    <b>Initial value</b>   0
    <b>Read-Only</b>       No
    <b>Saved In</b>        Nondatabase, Quasi variable
    <b>Versions</b>        2007+
  </table>
   
  The VSINTERSECTIONEDGES variable determines whether intersection edges are displayed in the visual style for the
  current viewport. The value can be:
   
  <table>
    <b>Value</b>   <b>Description</b>
    0              Display off
    1              Display on
  </table>
   
  <b>Note: </b>An existing visual style is not changed when the system variable gets a new value. 
  Temporary visual style will be created as a clone of visual style assigned for current viewport.
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>
   
  For example, to get the value:
  <code>
OdRusBufPtr pRb = pDb-\>getSysVar(L"VSINTERSECTIONEDGES");
OdInt16 status = pRb-\>getInt16();
odPrintConsoleString(L"nIntersection display status = %d", status);
  </code>
  For example, to set the value:
  <code>
OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtInt16, 1);
pDb-\>setSysVar(L"VSINTERSECTIONEDGES", pRb);
  </code> 

  <group TD_SysVars_V>
*/
//<dom_var_def>OdInt16 VSINTERSECTIONEDGES;
QVAR_DEF   (OdInt16,              VSINTERSECTIONEDGES, ValidateNone()    )

/**
  <title VSINTERSECTIONLTYPE>
  <toctitle VSINTERSECTIONLTYPE>
   
  <table>
    <b>Type</b>            OdInt16
    <b>Initial value</b>   1
    <b>Read-Only</b>       No
    <b>Saved In</b>        Nondatabase, Quasi variable
    <b>Versions</b>        2007+
  </table>
   
  The VSINTERSECTIONLTYPE variable stores the linetype number defining the shape of intersection 
  lines in the visual style for the current viewport. The value can be:
   
  <table>
    <b>Value</b>   <b>Description</b>
    1              Solid line
    2              Dashed line
    3              Dotted line
    4              Short dashed line
    5              Medium dashed line
    6              Long dashed line
    7              Double short dashed line
    8              Double medium dashed line
    9              Double long dashed line
    10             Medium long dashed line
    11             Sparse dotted line
  </table>
   
  <b>Note: </b>An existing visual style is not changed when the system variable gets a new value. 
  Temporary visual style will be created as a clone of visual style assigned for current viewport.
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>
   
  For example, to get the value:
  <code>
OdRusBufPtr pRb = pDb-\>getSysVar(L"VSINTERSECTIONLTYPE");
OdInt16 linetype = pRb-\>getInt16();
odPrintConsoleString(L"nIntersection linetype = %d", linetype);
 </code>
  For example, to set the value:
  <code>
OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtInt16, 5);
pDb-\>setSysVar(L"VSINTERSECTIONLTYPE", pRb);
  </code>
  
  <group TD_SysVars_V>
*/
//<dom_var_def>OdInt16 VSINTERSECTIONLTYPE;
QVAR_DEF   (OdInt16,              VSINTERSECTIONLTYPE, ValidateNone()    )

/**
  <title VSISOONTOP>
  <toctitle VSISOONTOP>
   
  <table>
    <b>Type</b>            OdInt16
    <b>Initial value</b>   0
    <b>Read-Only</b>       No
    <b>Saved In</b>        Nondatabase, Quasi variable
    <b>Versions</b>        2007+
  </table>
   
  The VSISOONTOP variable determines whether isolines are on top of shaded entities in the visual 
  style for the current viewport. The value can be:
   
  <table>
    <b>Value</b>   <b>Description</b>
    0              Isolines off
    1              Isolines on
  </table>
   
  <b>Note: </b>An existing visual style is not changed when the system variable gets a new value. 
  Temporary visual style will be created as a clone of visual style assigned for current viewport.
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>
   
  For example, to get the value:
  <code>
OdRusBufPtr pRb = pDb-\>getSysVar(L"VSISOONTOP");
OdInt16 status = pRb-\>getInt16();
odPrintConsoleString(L"nIsoline status = %d", status);
  </code>
  For example, to set the value:
  <code>
OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtInt16, 1);
pDb-\>setSysVar(L"VSISOONTOP", pRb);
  </code>  

  <group TD_SysVars_V>
*/
//<dom_var_def>OdInt16 VSISOONTOP;
QVAR_DEF   (OdInt16,              VSISOONTOP,          ValidateNone()    )

/**
  <title VSLIGHTINGQUALITY>
  <toctitle VSLIGHTINGQUALITY>
   
  <table>
    <b>Type</b>            OdInt16
    <b>Initial value</b>   1
    <b>Read-Only</b>       No
    <b>Saved In</b>        Nondatabase, Quasi variable
    <b>Versions</b>        2007+
  </table>
   
  The VSLIGHTINGQUALITY variable stores the light quality mode in the visual style for the current 
  viewport. The value can be:
   
  <table>
    <b>Value</b>   <b>Description</b>
    0              Faceted – uses a single color for each face.
    1              Smooth – computes colors of faces as a gradient between the vertices.
    2              Smoothest – computes colors of faces for individual pixels using the per-pixel lighting setting.
  </table>
   
  <b>Note: </b>An existing visual style is not changed when the system variable gets a new value. 
  Temporary visual style will be created as a clone of visual style assigned for current viewport.
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>
   
  For example, to get the value:
  <code>
OdRusBufPtr pRb = pDb-\>getSysVar(L"VSLIGHTINGQUALITY");
OdInt16 mode = pRb-\>getInt16();
odPrintConsoleString(L"nLight quality mode = %d", mode);
  </code>
  For example, to set the value:
  <code>
OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtInt16, 2);
pDb-\>setSysVar(L"VSLIGHTINGQUALITY", pRb);
  </code>

  <group TD_SysVars_V>
  */
//<dom_var_def>OdInt16 VSLIGHTINGQUALITY;
QVAR_DEF   (OdInt16,              VSLIGHTINGQUALITY,   ValidateNone()    )

/**
  <title VSMATERIALMODE>
  <toctitle VSMATERIALMODE>
   
  <table>
    <b>Type</b>            OdInt16
    <b>Initial value</b>   0
    <b>Read-Only</b>       No
    <b>Saved In</b>        Nondatabase, Quasi variable
    <b>Versions</b>        2007+
  </table>
   
  The VSMATERIALMODE variable stores the material mode that defines how materials are to be 
  displayed in the current viewport. The value can be:
   
  <table>
    <b>Value</b>   <b>Description</b>
    0              Materials are not displayed.
    1              Textures are not displayed and materials are displayed.
    2              Textures and materials are displayed.
  </table>
   
  <b>Note: </b>An existing visual style is not changed when the system variable gets a new value. 
  Temporary visual style will be created as a clone of visual style assigned for current viewport.
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>
   
  For example, to get the value:
  <code>
OdRusBufPtr pRb = pDb-\>getSysVar(L"VSMATERIALMODE");
OdInt16 mode = pRb-\>getInt16();
odPrintConsoleString(L"nMaterial mode = %d", mode);
  </code>
  For example, to set the value:
  <code>
OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtInt16, 1);
pDb-\>setSysVar(L"VSMATERIALMODE", pRb);
  </code> 

  <group TD_SysVars_V>
*/
//<dom_var_def>OdInt16 VSMATERIALMODE;
QVAR_DEF   (OdInt16,              VSMATERIALMODE,      ValidateNone()    )

/** 
  <title VSMONOCOLOR>
  <toctitle VSMONOCOLOR>
   
  <table>
    <b>Type</b>            OdString
    <b>Initial value</b>   "RGB:255,255,255"
    <b>Read-Only</b>       No
    <b>Saved In</b>        Nondatabase, Quasi variable
    <b>Versions</b>        2007+
  </table>
   
  The VSMONOCOLOR variable stores the monochrome tint of faces in the visual style for the current 
  viewport. TrueColor value is set as follows: "RGB:XXX,XXX,XXX", where XXX is the color component
  value in the range 0 to 255.
   
  <b>Note: </b>An existing visual style is not changed when the system variable gets a new value. 
  Temporary visual style will be created as a clone of visual style assigned for current viewport.
  
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>
   
  For example, to get the value:
  <code>
OdRusBufPtr pRb = pDb-\>getSysVar(L"VSMONOCOLOR");
OdString tint = pRb-\>getString();
odPrintConsoleString(L"nMonochrome color = %s", tint);
  </code>
  For example, to set the value:
  <code>
OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtString, L"RGB:128,128,128");
pDb-\>setSysVar(L"VSMONOCOLOR", pRb);
  </code>   

  <group TD_SysVars_V>
  */
//<dom_var_def>OdString VSMONOCOLOR;
QVAR_DEF   (OdString,             VSMONOCOLOR,         ValidateNone()    )

/**
  <title VSOBSCUREDCOLOR>
  <toctitle VSOBSCUREDCOLOR>
   
  <table>
    <b>Type</b>            OdString
    <b>Initial value</b>   "ByEntity"
    <b>Read-Only</b>       No
    <b>Saved In</b>        Nondatabase, Quasi variable
    <b>Versions</b>        2007+
  </table>
   
  The VSOBSCUREDCOLOR variable stores the tint of obscured (hidden) lines in the visual style for the 
  current viewport. TrueColor value is set as follows: "RGB:XXX,XXX,XXX", where XXX is the color 
  component value in the range 0 to 255. Value 0 defines a color ByBlock, value 256 defines a 
  color ByLayer, value 257 defines a color ByEntity, values 1 to 255 define the ACI color.
   
  <b>Note: </b>An existing visual style is not changed when the system variable gets a new value. 
  Temporary visual style will be created as a clone of visual style assigned for current viewport.
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>
   
  For example, to get the value:
  <code>
OdRusBufPtr pRb = pDb-\>getSysVar(L"VSOBSCUREDCOLOR");
OdString tint = pRb-\>getString();
odPrintConsoleString(L"nColor of obscured lines = %s", tint);
  </code>
  For example, to set the value:
  <code>
OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtString, L"RGB:128,128,128");
pDb-\>setSysVar(L"VSOBSCUREDCOLOR", pRb);
  </code>    

  <group TD_SysVars_V>
*/
//<dom_var_def>OdString VSOBSCUREDCOLOR;
QVAR_DEF   (OdString,             VSOBSCUREDCOLOR,     ValidateNone()    )

/**
  <title VSOBSCUREDEDGES>
  <toctitle VSOBSCUREDEDGES>
   
  <table>
    <b>Type</b>            OdInt16
    <b>Initial value</b>   1
    <b>Read-Only</b>       No
    <b>Saved In</b>        Nondatabase, Quasi variable
    <b>Versions</b>        2007+
  </table>
   
  The VSOBSCUREDEDGES variable determines whether obscured (hidden) edges are displayed in the 
  visual style for the current viewport. The value can be:
   
  <table>
    <b>Value</b>   <b>Description</b>
    0              Display off
    1              Display on
  </table>
   
  <b>Note: </b>An existing visual style is not changed when the system variable gets a new value. 
  Temporary visual style will be created as a clone of visual style assigned for current viewport.
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>
   
  For example, to get the value:
  <code>
OdRusBufPtr pRb = pDb-\>getSysVar(L"VSOBSCUREDEDGES");
OdInt16 status = pRb-\>getInt16();
odPrintConsoleString(L"nObscured edges display status = %d", status);
  </code>
  For example, to set the value:
  <code>
OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtInt16, 0);
pDb-\>setSysVar(L"VSOBSCUREDEDGES", pRb);
  </code>     

  <group TD_SysVars_V>
*/
//<dom_var_def>OdInt16 VSOBSCUREDEDGES;
QVAR_DEF   (OdInt16,              VSOBSCUREDEDGES,     ValidateNone()    )

/**
  <title VSOBSCUREDLTYPE>
  <toctitle VSOBSCUREDLTYPE>
   
  <table>
    <b>Type</b>            OdInt16
    <b>Initial value</b>   Depends on the visual style
    <b>Read-Only</b>       No
    <b>Saved In</b>        Nondatabase, Quasi variable
    <b>Versions</b>        2007+
  </table>
   
  The VSOBSCUREDLTYPE variable stores the linetype number defining the shape of obscured (hidden) 
  lines in the visual style for the current viewport. The value can be:
   
  <table>
    <b>Value</b>   <b>Description</b>
    1              Solid line
    2              Dashed line
    3              Dotted line
    4              \Short dashed line
    5              Medium dashed line
    6              \Long dashed line
    7              Double short dashed line
    8              Double medium dashed line
    9              Double long dashed line
    10             Medium long dashed line
    11             Sparse dotted line
  </table>
   
  The current visual style defines the initial value:
   
  <table>
    <b>Value</b>   <b>Description</b>
    1              2D Wireframe
    1              Conceptual
    2              Hidden
    1              Shaded
    2              Shaded with Edges
    1              Shades of Gray
    1              Sketchy
    1              Wireframe
    1              X-ray
    1              Realistic
  </table>
   
   <b>Note: </b>An existing visual style is not changed when the system variable gets a new value. 
   Temporary visual style will be created as a clone of visual style assigned for current viewport.
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>
   
  For example, to get the value:
  <code>
OdRusBufPtr pRb = pDb-\>getSysVar(L"VSOBSCUREDLTYPE");
OdInt16 linetype = pRb-\>getInt16();
odPrintConsoleString(L"nObscured linetype = %d", linetype);
  </code>
  For example, to set the value:
  <code>
OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtInt16, 5);
pDb-\>setSysVar(L"VSOBSCUREDLTYPE", pRb);
  </code>     

  <group TD_SysVars_V>
*/
//<dom_var_def>OdInt16 VSOBSCUREDLTYPE;
QVAR_DEF   (OdInt16,              VSOBSCUREDLTYPE,     ValidateNone()    )

/**
  <title VSOCCLUDEDCOLOR>
  <toctitle VSOCCLUDEDCOLOR>
   
  <table>
    <b>Type</b>            OdString
    <b>Initial value</b>   "ByEntity"
    <b>Read-Only</b>       No
    <b>Saved In</b>        Nondatabase, Quasi variable
    <b>Versions</b>        2011+
  </table>
   
  The VSOCCLUDEDCOLOR variable stores the tint of occluded (hidden) lines in the visual style for 
  the current viewport. TrueColor value is set as follows: "RGB:XXX,XXX,XXX", where XXX is the color
  component value in the range 0 to 255. Value 0 defines a color ByBlock, value 256 defines 
  a color ByLayer, value 257 defines a color ByEntity, values 1 to 255 define the ACI color.
   
  <b>Note: </b>An existing visual style is not changed when the system variable gets a new value. 
  Temporary visual style will be created as a clone of visual style assigned for current viewport.
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>
   
  For example, to get the value:
  <code>
OdRusBufPtr pRb = pDb-\>getSysVar(L"VSOCCLUDEDCOLOR");
OdString tint = pRb-\>getString();
odPrintConsoleString(L"nColor of occluded lines = %s", tint);
  </code>
  For example, to set the value:
  <code>
OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtString, L"RGB:128,128,128");
pDb-\>setSysVar(L"VSOCCLUDEDCOLOR", pRb);
  </code>  

  <group TD_SysVars_V>
*/
//<dom_var_def>OdString VSOCCLUDEDCOLOR;
QVAR_DEF   (OdString,             VSOCCLUDEDCOLOR,     ValidateNone()    )

/**
  <title VSOCCLUDEDEDGES>
  <toctitle VSOCCLUDEDEDGES>
   
  <table>
    <b>Type</b>            OdInt16
    <b>Initial value</b>   1
    <b>Read-Only</b>       No
    <b>Saved In</b>        Nondatabase, Quasi variable
    <b>Versions</b>        2011+
  </table>
   
  The VSOCCLUDEDEDGES variable determines whether occluded (hidden) edges are displayed in the visual 
  style for the current viewport. The value can be:
   
  <table>
    <b>Value</b>   <b>Description</b>
    0              Display off
    1              Display on
  </table>
   
  <b>Note: </b>An existing visual style is not changed when the system variable gets a new value. 
  Temporary visual style will be created as a clone of visual style assigned for current viewport.
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>
   
  For example, to get the value:
  <code>
OdRusBufPtr pRb = pDb-\>getSysVar(L"VSOCCLUDEDEDGES");
OdInt16 status = pRb-\>getInt16();
odPrintConsoleString(L"nOccluded edges display status = %d", status);
  </code>
  For example, to set the value:
  <code>
OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtInt16, 0);
pDb-\>setSysVar(L"VSOCCLUDEDEDGES", pRb);
  </code>  

  <group TD_SysVars_V>
*/
//<dom_var_def>OdInt16 VSOCCLUDEDEDGES;
QVAR_DEF   (OdInt16,              VSOCCLUDEDEDGES,     ValidateNone()    )

/**
  <title VSOCCLUDEDLTYPE>
  <toctitle VSOCCLUDEDLTYPE>
   
  <table>
    <b>Type</b>            OdInt16
    <b>Initial value</b>   Depends on the visual style
    <b>Read-Only</b>       No
    <b>Saved In</b>        Nondatabase, Quasi variable
    <b>Versions</b>        2011+
  </table>
   
  The VSOCCLUDEDLTYPE variable stores the linetype number
  defining the shape of occluded (hidden) lines in the visual
  style for the current viewport. The value can be:
   
  <table>
    <b>Value</b>   <b>Description</b>
    1              Solid line
    2              Dashed line
    3              Dotted line
    4              Short dashed line
    5              Medium dashed line
    6              Long dashed line
    7              Double short dashed line
    8              Double medium dashed line
    9              Double long dashed line
    10             Medium long dashed line
    11             Sparse dotted line
  </table>
   
  The current visual style defines the initial value:
   
  <table>
    <b>Value</b>   <b>Description</b>
    1              2D Wireframe
    1              Conceptual
    2              Hidden
    1              Shaded
    2              Shaded with Edges
    1              Shades of Gray
    1              Sketchy
    1              Wireframe
    1              X-ray
    1              Realistic
  </table>
   
  <b>Note: </b>An existing visual style is not changed when the
  system variable gets a new value. Temporary visual style will
  be created as a clone of visual style assigned for current
  viewport.
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>
   
  For example, to get the value:
  <code>
OdRusBufPtr pRb = pDb-\>getSysVar(L"VSOCCLUDEDLTYPE");
OdInt16 linetype = pRb-\>getInt16();
odPrintConsoleString(L"nOccluded linetype = %d", linetype);
  </code>
  For example, to set the value:
  <code>
OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtInt16, 5);
pDb-\>setSysVar(L"VSOCCLUDEDLTYPE", pRb);
  </code> 

  <group TD_SysVars_V>
*/
//<dom_var_def>OdInt16 VSOCCLUDEDLTYPE;
QVAR_DEF   (OdInt16,              VSOCCLUDEDLTYPE,     ValidateNone()    )

/**
  <title VSSHADOWS>
  <toctitle VSSHADOWS>
   
  <table>
    <b>Type</b>            OdInt16
    <b>Initial value</b>   0
    <b>Read-Only</b>       No
    <b>Saved In</b>        Nondatabase, Quasi variable
    <b>Versions</b>        2007+
  </table>
   
  The VSSHADOWS variable stores the shadow mode that defines
  whether a visual style displays shadows in the current
  viewport. The value can be:
   
  <table>
    <b>Value</b>   <b>Description</b>
    0              Shadows are not displayed
    1              Ground shadows are only displayed
    2              Full shadows are displayed
  </table>
   
  <b>Note: </b>An existing visual style is not changed when the
  system variable gets a new value. Temporary visual style will
  be created as a clone of visual style assigned for current
  viewport.
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>
   
  For example, to get the value:
  <code>
OdRusBufPtr pRb = pDb-\>getSysVar(L"VSSHADOWS");
OdInt16 mode = pRb-\>getInt16();
odPrintConsoleString(L"nShadow mode = %d", mode);
  </code>
  For example, to set the value:
  <code>
OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtInt16, 1);
pDb-\>setSysVar(L"VSSHADOWS", pRb);
  </code>  

  <group TD_SysVars_V>
*/
//<dom_var_def>OdInt16 VSSHADOWS;
QVAR_DEF   (OdInt16,              VSSHADOWS,           ValidateNone()    )

/**
  <title VSSILHEDGES>
  <toctitle VSSILHEDGES>
   
  <table>
    <b>Type</b>            OdInt16
    <b>Initial value</b>   Depends on the visual style
    <b>Read-Only</b>       No
    <b>Saved In</b>        Nondatabase, Quasi variable
    <b>Versions</b>        2007+
  </table>
   
  The VSSILHEDGES variable determines whether silhouette edges
  of 3D entities are displayed in the visual style for the
  current viewport. The value can be:
   
  <table>
    <b>Value</b>   <b>Description</b>
    0              Display off
    1              Display on
  </table>
   
  The current visual style defines the initial value:
   
  <table>
    <b>Value</b>   <b>Description</b>
    0              2D Wireframe
    1              Conceptual
    1              Hidden
    0              Shaded
    1              Shaded with Edges
    1              Shades of Gray
    1              Sketchy
    0              Wireframe
    0              X-ray
    0              Realistic
  </table>
   
  <b>Note: </b>An existing visual style is not changed when the
  system variable gets a new value. Temporary visual style will
  be created as a clone of visual style assigned for current
  viewport.
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>
   
  For example, to get the value:
  <code>
OdRusBufPtr pRb = pDb-\>getSysVar(L"VSSILHEDGES");
OdInt16 status = pRb-\>getInt16();
odPrintConsoleString(L"nSilhouette edge display status = %d", status);
  </code>
  For example, to set the value:
  <code>
OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtInt16, 1);
pDb-\>setSysVar(L"VSSILHEDGES", pRb);
  </code>

  <group TD_SysVars_V>
*/
//<dom_var_def>OdInt16 VSSILHEDGES;
QVAR_DEF   (OdInt16,              VSSILHEDGES,         ValidateNone()    )

/**
  <title VSSILHWIDTH>
  <toctitle VSSILHWIDTH>
   
  <table>
    <b>Type</b>            OdInt16
    <b>Initial value</b>   5
    <b>Read-Only</b>       No
    <b>Saved In</b>        Nondatabase, Quasi variable
    <b>Versions</b>        2007+
  </table>
   
  The VSSILHWIDTH variable stores the width in pixels of
  silhouette edges in the visual style for the current
  viewport. The value can be in the range 0 to 25.
   
  <b>Note: </b>An existing visual style is not changed when the
  system variable gets a new value. Temporary visual style will
  be created as a clone of visual style assigned for current
  viewport.
  
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>
   
  For example, to get the value:
  <code>
OdRusBufPtr pRb = pDb-\>getSysVar(L"VSSILHWIDTH");
OdInt16 width = pRb-\>getInt16();
odPrintConsoleString(L"nSilhouette width = %d", width);
  </code>
  For example, to set the value:
  <code>
OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtInt16, 12);
pDb-\>setSysVar(L"VSSILHWIDTH", pRb);
  </code> 

  <group TD_SysVars_V>
*/
//<dom_var_def>OdInt16 VSSILHWIDTH;
QVAR_DEF   (OdInt16,              VSSILHWIDTH,         ValidateNone()    )

/**
  <title NAVVCUBEDISPLAY>
  <toctitle NAVVCUBEDISPLAY>
   
  <table>
    <b>Type</b>            OdInt16
    <b>Initial value</b>   3
    <b>Read-Only</b>       No
    <b>Saved In</b>        Nondatabase, Quasi variable
    <b>Versions</b>        2010+
  </table>
   
   The NAVVCUBEDISPLAY variable specifies whether the viewcube
   tool should be displayed in the current paper or display
   viewport (OdDbViewport or OdDbViewportTableRecord) depending
   on the current visual style. The TILEMODE variable defines
   whether the current viewport is paper or display. The
   viewcube is a navigation tool which is displayed in 2D and 3D
   visual styles and also allows switching between standard and
   isometric views. The value can be:
   
  <table>
    <b>Value</b>   <b>Description</b>
    0              2D and 3D visual styles do not display the viewcube
    1              3D visual styles display the viewcube, 2D visual styles do not
    2              2D visual styles display the viewcube, 3D visual styles do not
    3              2D and 3D visual styles display the viewcube
  </table>
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>
   
   Access methods provide access to the value for the current
   viewport (OdDbViewport if a paper layout is active or
   OdViewportTableRecord if model space is active). See also
   OdDbAbstractViewportData::navvcubeddisplay() and
   OdDbAbstractViewportData::setNavvcubeddisplay() methods.
   
  For example, to get the value:
  <code>
OdRusBufPtr pRb = pDb-\>getSysVar(L"NAVVCUBEDISPLAY");
OdInt16 view = pRb-\>getInt16();
odPrintConsoleString(L"nThe viewcube status = %d", view);
  </code>
  For example, to set the value:
  <code>
OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtInt16, 1);
pDb-\>setSysVar(L"NAVVCUBEDISPLAY", pRb);
  </code>  

  <group TD_SysVars_N>
  */
//<dom_var_def>OdInt16 NAVVCUBEDISPLAY;
QVAR_DEF   (OdInt16,              NAVVCUBEDISPLAY,     ValidateRange(0, 3))

/**
  <title DEFAULTLIGHTING>
  <toctitle DEFAULTLIGHTING>
   
  <table>
    <b>Type</b>            OdInt16
    <b>Initial value</b>   1
    <b>Read-Only</b>       No
    <b>Saved In</b>        Nondatabase, Quasi variable
    <b>Versions</b>        2007+
  </table>
   
   The DEFAULTLIGHTING variable is used for turning on and off default lighting 
   instead of other lighting types. 
   The value can be:
   
  <table>
    <b>Value</b>   <b>Description</b>
    0              Default lighting is turned off automatically when the other lights such as distant lights, point lights, spotlights, or the sun are turned on.
    1              Only default lighting is turned on.
  </table>
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>
   
  Access methods provide access to the value for the current viewport.
  Alternatively users can invoke direct access methods in their code depending 
  on the type of database object:
  <table>
    OdAbstractViewPE::isDefaultLightingOn()
    OdAbstractViewPE::setDefaultLightingOn()
    OdDbAbstractViewTableRecord::isDefaultLightingOn()
    OdDbAbstractViewTableRecord::setDefaultLightingOn()
    OdDbViewport::isDefaultLightingOn()
    OdDbViewport::setDefaultLightingOn()
  </table>   
   
  <group TD_SysVars_D>
  */
//<dom_var_def>OdInt16 DEFAULTLIGHTING;
QVAR_DEF   (OdInt16,              DEFAULTLIGHTING,     ValidateRange(0, 1))

/**
  <title DEFAULTLIGHTINGTYPE>
  <toctitle DEFAULTLIGHTINGTYPE>
   
  <table>
    <b>Type</b>            OdInt16
    <b>Initial value</b>   1
    <b>Read-Only</b>       No
    <b>Saved In</b>        Nondatabase, Quasi variable
    <b>Versions</b>        2007+
  </table>
   
  The DEFAULTLIGHTINGTYPE variable is used for specifying the old or new default lighting 
  type in a viewport. When the default lighting type is old, only one distant light is used. 
  The new default lighting type allows for two distant lights and ambient light adjustments.
  The value can be:
   
  <table>
    <b>Value</b>   <b>Description</b>
    0              Old type of default lighting
    1              New type of default lighting
  </table>
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::setSysVar()
  </table>
   
  Access methods provide access to the value for the current viewport. 
  Alternatively users can invoke direct access methods in their code depending 
  on the type of database object:
  <table>
    OdAbstractViewPE::defaultLightingType()
    OdAbstractViewPE::setDefaultLightingType()
    OdDbAbstractViewTableRecord::defaultLightingType()
    OdDbAbstractViewTableRecord::setDefaultLightingType()
    OdDbViewport::defaultLightingType()
    OdDbViewport::setDefaultLightingType()
  </table>
   
  <group TD_SysVars_D>
  */
//<dom_var_def>OdInt16 DEFAULTLIGHTINGTYPE;
QVAR_DEF   (OdInt16,              DEFAULTLIGHTINGTYPE, ValidateRange(0, 1))


// Variables added in 2019
/**
  <title STUDENTDRAWING>
  <toctitle STUDENTDRAWING>
   
  <table>
    <b>Type</b>            bool
    <b>Initial value</b>   depends on drawing
    <b>Read-Only</b>       Yes
    <b>Saved In</b>        Drawing
    <b>Versions</b>        2019+
  </table>
   
  STUDENTDRAWING specifies whether the drawing was created by educational version of software
   
  <table>
    <b>Value</b>   <b>Description</b>
    false          Created by non-educational version
    true           Created by educational version
  </table>
   
  <table>
    <b>Access Methods</b>
    OdDbDatabase::getSysVar()
    OdDbDatabase::isEMR()
  </table>
  <group TD_SysVars_S>
*/
//<dom_var_def>bool STUDENTDRAWING;
QVAR_DEF_RO(ODTBOOL,              STUDENTDRAWING,        ValidateNone()    )


#ifdef DOUNDEF_ODTDIST
#undef ODTDIST
#undef DOUNDEF_ODTDIST
#endif

#ifdef DOUNDEF_ODTORIENT
#undef ODTORIENT
#undef DOUNDEF_ODTORIENT
#endif

#ifdef DOUNDEF_ODTANGLE
#undef ODTANGLE
#undef DOUNDEF_ODTANGLE
#endif

#ifdef DOUNDEF_ODTLAYOUTID
#undef ODTLAYOUTID
#undef DOUNDEF_ODTLAYOUTID
#endif

#ifdef DOUNDEF_ODTTEXTSTYLEID
#undef ODTTEXTSTYLEID
#undef DOUNDEF_ODTTEXTSTYLEID
#endif

#ifdef DOUNDEF_QVAR_DEF_RO
#undef QVAR_DEF_RO
#undef DOUNDEF_QVAR_DEF_RO
#endif

#ifdef DOUNDEF_ODTBOOL
#undef ODTBOOL
#undef DOUNDEF_ODTBOOL
#endif
