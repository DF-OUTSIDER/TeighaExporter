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




#ifndef _SmSubset_h_Included_
#define _SmSubset_h_Included_

#include "Sm/SmComponent.h"
#include <OdString.h>
#include "Sm/SmEvents.h"
#include "Sm/SmFileReference.h"
#include "Sm/SmNamedDbObjectReference.h"
#include "Sm/SmSheet.h"

/** \details
  
    <group TD_Namespaces>
*/
namespace dst
{
  struct SMDLL_EXPORT OdSmSubset;
  typedef OdSmartPtr<OdSmSubset> OdSmSubsetPtr;

  /** \details

       
  */
  struct SMDLL_EXPORT OdSmSubset : OdSmComponent
  {
    ODRX_DECLARE_MEMBERS(OdSmSubset);
    // Returns the OdSmFileReference that points to the location in the file system where new sheets are created. 
    virtual const OdSmFileReference* getNewSheetLocation() const = 0;
    // sets the OdSmFileReference that points to the location in the file system where new sheets are created
    virtual void setNewSheetLocation ( OdSmFileReference* pFileRef ) = 0;
    // Returns the OdSmNamedAcDbObjectReference that points to the layout used as a template for new sheets. 
    virtual const OdSmDbLayoutReference* getDefDwtLayout() const = 0;
    // sets the OdSmDbLayoutReference that points to the layout used as a template for new sheets.
    virtual void setDefDwtLayout ( OdSmDbLayoutReference* pLayoutRef ) = 0;
    // Returns a Boolean flag indicating if the application should prompt for a DWT file.
    virtual bool getPromptForDwt() const = 0;
    // sets a Boolean flag indicating if the application should prompt for a DWT file.
    virtual void setPromptForDwt( bool askForDwt ) = 0;
    // Returns a Boolean flag indicating an override for the subset's sheets DoNotPlot option.
    virtual bool getOverrideSheetPublish() const = 0;
    // sets a Boolean flag indicating an override for the subset's sheets DoNotPlot option.
    virtual void setOverrideSheetPublish( bool override ) = 0;
    // Returns an enumerator of all components in this subset
    virtual OdSmEnumComponentPtr getSheetEnumerator() const = 0;
    // Adds a new sheet component without adding it to the subset.
    // To add the new sheet component to the subset, use the InsertComponent method
    virtual OdSmSheetPtr addNewSheet( const OdString& name, const OdString& desc ) = 0;
    // Inserts a sheet at the position specified.
    virtual void insertComponent ( OdSmComponent* newSheet, OdSmComponent* beforeComp = 0) = 0;
    // Inserts a sheet at the position specified
    virtual void insertComponentAfter( OdSmComponent* newSheet, OdSmComponent* afterComp = 0 ) = 0;
    // Adds a new sheet component that references the layout of OdSmDbLayoutReference. 
    // The new component is not added to the subset.
    // To add the new sheet component to the subset, use the InsertComponent method
    virtual OdSmSheetPtr importSheet( OdSmDbLayoutReference* pLayoutRef ) = 0;
    // Removes the specified sheet component from the subset.
    virtual void removeSheet( OdSmSheet * sheet ) = 0;
    // Adds a new subset component without adding it to the current subset.
    virtual OdSmSubsetPtr createSubset( const OdString& name, const OdString& desc ) = 0;
    // Removes the specified subset component from the collection
    virtual void removeSubset( OdSmSubset * subset ) = 0;
    // Forces a notification to all registered notification handlers
    virtual void notifyRegisteredEventHandlers( OdSmEvent event, OdSmPersist * comp ) = 0;
    // Updates the hints in the open drawings referenced in the subset
    virtual void updateInMemoryDwgHints ( ) = 0;
  };
}

#endif //_SmSubset_h_Included_
