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




#ifndef _HOOPSEXPORTPARAMS_INCLUDED_
#define _HOOPSEXPORTPARAMS_INCLUDED_ /*!DOM*/

#include "HoopsExportDef.h"
#include "DbBaseDatabase.h"
#include "OdStreamBuf.h"

#define TTF_TEXT_AS_GEOMETRY 1
#define SHX_TEXT_AS_GEOMETRY 2
#define ZOOM_TO_EXTENTS      4

#define DWG_FILE_TYPE        1
#define DGN_FILE_TYPE        2

/** \details
  This namespace represents structures, classes and functions, that implement functionality for exporting to a hoops file.
  
  <group OdExport_Classes> 
*/
namespace TD_HOOPS_COMMON_EXPORT {

/** \details
  This class implements the hoops export parameters.
  
  <group OdExport_Classes> 
*/
class HoopsExportParams
{
public:
  /** \details 
    Creates an instance of a hoops export parameter set.
  */
  HoopsExportParams(): pDb(0)
    , flags(0)
    , strFileName("")
    , background(ODRGB(255, 255, 255))
    , iStartKey(0)
    , bSetCamera(false)
    , bExportCurrentLayout(true)
    , bResetSystemExport(false)
    , bSetExportRenderModeFrpmLayout(false)
    , iExportFileType(DWG_FILE_TYPE)                                                                           
  {
    m_pClrPalette = NULL;
  }

  /** \details 
    Creates an instance of a hoops export parameter set as a copy of an already existing one (copy constructor).
    
    \param Obj [in] A parameter set instance to be copied.
  */
  HoopsExportParams( const HoopsExportParams& Obj )
  {
    pDb                  = Obj.pDb;
    flags                = Obj.flags;
    strFileName          = Obj.strFileName;
    background           = Obj.background;
    iStartKey            = Obj.iStartKey;
    bSetCamera           = Obj.bSetCamera;
    bExportCurrentLayout = Obj.bExportCurrentLayout;
    bResetSystemExport   = Obj.bResetSystemExport;
    iExportFileType      = Obj.iExportFileType;
    bSetExportRenderModeFrpmLayout = Obj.bSetExportRenderModeFrpmLayout;

    HoopsExportParams* pObj = const_cast<HoopsExportParams*>(&Obj);

    const ODCOLORREF* pPalette = pObj->palette();

    if( pPalette )
    {
      m_pClrPalette = new ODCOLORREF[256];
      memcpy( m_pClrPalette, pPalette, 256 * sizeof(ODCOLORREF) );
    }
    else
    {
      m_pClrPalette = NULL;
    }
  }

  /** \details 
    Destroys an instance of a hoops export parameter set.
  */
  ~HoopsExportParams()
  {
    if( m_pClrPalette )
    {
      delete[] m_pClrPalette;
    }
  }

  /** \details 
    Assignment operator overload for a hoops export parameters class.
    
    \param Obj [in] An instance of a HoopsExportParams class as an assignment right operand.
  */
  HoopsExportParams& operator=( const HoopsExportParams& Obj )
  {
    if( &Obj != this )
    {
      pDb                  = Obj.pDb;
      flags                = Obj.flags;
      strFileName          = Obj.strFileName;
      background           = Obj.background;
      iStartKey            = Obj.iStartKey;
      bSetCamera           = Obj.bSetCamera;
      bExportCurrentLayout = Obj.bExportCurrentLayout;
      bResetSystemExport   = Obj.bResetSystemExport;
      iExportFileType      = Obj.iExportFileType;
      bSetExportRenderModeFrpmLayout = Obj.bSetExportRenderModeFrpmLayout;

      HoopsExportParams* pObj = const_cast<HoopsExportParams*>(&Obj);

      const ODCOLORREF* pPalette = pObj->palette();

      if( m_pClrPalette )
      {
        delete[] m_pClrPalette;
        m_pClrPalette = NULL;
      }

      if( pPalette )
      {
        m_pClrPalette = new ODCOLORREF[256];
        memcpy( m_pClrPalette, pPalette, 256 * sizeof(ODCOLORREF) );
      }
      else
      {
        m_pClrPalette = NULL;
      }
    }

    return *this;
  }

  /** \details 
    Returns the current color palette that is used for exporting to hoops.
  */
  const ODCOLORREF* palette()
  {
    return m_pClrPalette;
  }

  /** \details 
    Sets a new color palette to be used for exporting to hoops.
    
    \param colors [in] A palette to be set.
  */
  void setPalette(const ODCOLORREF* colors)
  {
    if( !m_pClrPalette )
    {
      m_pClrPalette = new ODCOLORREF[256];
    }

    memcpy( m_pClrPalette, colors, 256 * sizeof(ODCOLORREF) );
  }

public:

  OdDbBaseDatabase     *pDb;                              //!< A pointer to a drawing database, which will be filled after export operation.
  OdString              strFileName;                      //!< A hoops file, which content will be exported to a drawing database.

  int                   flags;                            //!< Flags for exporting to hoops.
  ODCOLORREF            background;                       //!< Background color used for export.
  bool                  bSetCamera;                       //!< A flag value that is used to determine whether a camera is set for export.
  bool                  bResetSystemExport;               //!< This flag is used for internal Teigha purposes (for utility tests).
  bool                  bExportCurrentLayout;             //!< A flag value that determines whether the export only the current layout (if true) or all existing layouts (if false).
  bool                  bSetExportRenderModeFrpmLayout;   //!< A flag value that determines whether the rendering mode is exported from layout settings. If rendering mode should be exported from the layout, the value is equal to true. Otherwise the value is equal to false.
  OdInt64               iStartKey;                        //!< A hoops object key for the starting segment.
  OdUInt16              iExportFileType;                  //!< A drawing file type to be created as an export result.

//DOM-IGNORE-BEGIN
private:

  ODCOLORREF* m_pClrPalette;
//DOM-IGNORE-END
};
}
#endif //_HOOPSEXPORTPARAMS_INCLUDED_

