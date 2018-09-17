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

#ifndef __ODGSUPDATEMANAGER_H__
#define __ODGSUPDATEMANAGER_H__

class OdDbStub;
class OdGsEntityNode;
class OdRxObject;
class OdGsViewImpl;

#include "Gs/GsExport.h"
#include "Ge/GeExtents3d.h"
#define  STL_USING_MAP
#include <map>
#include "TD_PackPush.h"

/** \details
    This class represents interface of the Update Manager for Gs and Cloud purposes

    Corresponding C++ library: TD_Gs
    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsUpdateManager : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdGsUpdateManager);

  /** \details
      Filtration type
  */
  enum FiltrationType
  {
    kKeepDrawOrder = 0, //Keep draw order
    kFilterByLimit = (1 << 0) //Filtration by memory limit (CLOUD)
  };

  /** \details
      Action type for process callback
  */
  enum Action
  {
    kAdd,
    kRemove,
    kNoChanges
  };

  class GS_TOOLKIT_EXPORT OdGsUpdateManagerElement
  {
  protected:
    std::map< OdUInt32, OdUInt8 > m_stateFlags;
    //OdUInt8 m_stateFlags;
  public:
    OdGsUpdateManagerElement(){ };
    OdUInt8 state( OdUInt32 vpId ) const 
    { 
      std::map< OdUInt32, OdUInt8 >::const_iterator it = m_stateFlags.find( vpId );
      if( it != m_stateFlags.end() )
      {
        return it->second;
      }
      return 0;
    }
    void setState( OdUInt8 state, OdUInt32 vpId ) 
    { 
      m_stateFlags[ vpId ] = state;
    }
  };

  /** \details
      Process callback for processing procedure

      \param action [in] Current action
      \param viewportId [in] Viewport ID of the viewport required data
      \param drawableId [in] Drawable ID of the drawable object
      \param pNode [in] Pointer to the OdGsEntityNode
      \param pElement [in] Pointer to the OdGsUpdateManagerElement
  */
  typedef void (*UpdateManagerProcessCallback)( Action action, OdUInt32 viewportId, OdDbStub* drawableId, OdGsEntityNode* pNode, OdGsUpdateManagerElement* pElement );

  /** \details
      Update Manager settings
  */
  struct UpdateManagerSettings
  {
    FiltrationType type;
    OdUInt64 nMemoryLimit;
    OdUInt64 nMinMemoryLimit;
    OdUInt64 nClientCurrentHeapUsage;
  };

public:
  /** \details
      Retrives current manager settings
  */
  virtual UpdateManagerSettings settings() const = 0;

  /** \details
      Sets settings
  */
  virtual void setSettings( const UpdateManagerSettings& ) = 0;

  /** \details
      Adds element to the Update Manager in no limit mode

      \param viewportId [in] Viewport ID
      \param pNode [in] Pointer to the OdGsEntityNode
      \param pMetafile [in] Pointer to the metafile
  */
  virtual void addElement( OdUInt32 viewportId, OdGsEntityNode* pNode, OdGsUpdateManagerElement* pElement ) = 0;

  /** \details
      Adds element to the Update Manager in memory limit

      \param viewportId [in] Viewport ID
      \param pNode [in] Pointer to the OdGsEntityNode
      \param pElement [in] Pointer to the OdGsUpdateManagerElement
      \param nLength [in] Length of the metafile
      \param pView [in] Pointer to the view
      \param extents [in] Extents of cache
      \param bSetZeroWeight [in] If true visual weight will be zero
  */
  virtual void addElement( OdUInt32 viewportId, OdGsEntityNode* pNode, OdGsUpdateManagerElement* pElement, OdUInt64 nLength, OdGsViewImpl* pView, const OdGeExtents3d& extents, bool bSetZeroWeight = false ) = 0;

  /** \details
      Add priority element to the Update Manager in memory limit mode
  */
  virtual void addPriorityElement( OdUInt32 viewportId, OdGsEntityNode* pNode, OdGsUpdateManagerElement* pElement, OdUInt64 nLength ) = 0;

  /** \details
      Sets processing callback
  */
  virtual void setProcessCallback( UpdateManagerProcessCallback ) = 0;

  /** \details
      Process added elements
  */
  virtual void process() = 0;

  /** \details
      Resets saved data (map, lists) but do not reset settings
  */
  virtual void reset() = 0;

  /** \details
      Removes element added by addElement

      \remarks
      Right now only pMetafile is used
  */
  virtual void removeElement( OdUInt32 viewportId, OdGsEntityNode* pNode, OdGsUpdateManagerElement* pElement ) = 0;
};

#include "TD_PackPop.h"

#endif // __ODGSUPDATEMANAGER_H__
