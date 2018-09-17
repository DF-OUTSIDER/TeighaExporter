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




#ifndef _ODDBGROUP_INCLUDED_
#define _ODDBGROUP_INCLUDED_

#include "TD_PackPush.h"

#include "DbObject.h"
#include "DwgDeclareMembers.h"

class OdDbGroupIterator;
class OdCmColor;
class OdDbGroupImpl;
class OdDbGroup;

/** \details
    This class defines Iterator objects that traverse entries in OdDbGroup 
    objects in an OdDbDatabase instance.
    
    \sa
    TD_Db

    <group OdDb_Classes>
*/
class DBENT_EXPORT OdDbGroupIterator : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbGroupIterator);

  /** \details
    Returns the entity currently referenced by this Iterator object.
  
    \param openMode [in]  Mode in which to open the current entity.

    \remarks
    Returns a null SmartPointer if not successful.
  */
  virtual OdDbObjectPtr getObject(
    OdDb::OpenMode openMode) = 0;

  /** \details
    Returns the Object ID of the entity currently referenced by this Iterator object.
  */
  virtual OdDbObjectId objectId() const = 0;

  /** \details
    Returns true if and only if the traversal by this Iterator object is complete.
  */
    virtual bool done() const = 0;

  /** \details
    Sets this Iterator object to reference the entity following the current entity.
    
    \returns
    Returns true if and only if not at the end of the list.
  */
    virtual bool next() = 0;

  /*
     virtual OdResult subGetClassID(void* pClsid) const;
  */
protected:
  OdDbGroupIterator() {}
};
/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbGroupIterator object pointers.
*/
typedef OdSmartPtr<OdDbGroupIterator> OdDbGroupIteratorPtr;

/** \details
    This class represents Group objects in an OdDbDatabase instance.

    \remarks
    Only top level entities in ModelSpace or PaperSpace can be placed in a Group.

    <group OdDb_Classes>
*/
class DBENT_EXPORT OdDbGroup: public OdDbObject
{
public:
  DWGMAP_DECLARE_MEMBERS(OdDbGroup);

  OdDbGroup();

  /** \details
    Creates an Iterator object that provides access to the entities in this Group object.
  */
  OdDbGroupIteratorPtr newIterator();

  /** \details
    Returns the description of this Group object (DXF 300).
  */
  OdString description() const;

  /** \details
    Sets the description of this Group object (DXF 300).
    \param description [in]  Description.
  */
  void setDescription(
    const OdString& description);

  /** \details
    Returns true if and only if this Group object is selectable (DXF 71).
  */
  bool isSelectable() const;

  /** \details
    Sets this Group object as selectable (DXF 71).

    \param selectable [in]  Selectable flag.
  */
  void setSelectable(
    bool selectable);

  /** \details
    Returns the name of this Group object.
  */
  OdString name() const;

  /** \details
    Sets the name for this Group object.
    \param name [in]  Name.
  */
  void setName(const OdString& name);

  /** \details
    Returns true if and only if this Group object is inaccessible.
      
    \remarks
    Currently, only groups in Xrefs are flagged as inaccessable.  
  */
  bool isNotAccessible() const;

  /** \details
    Returns true if and only if this Group object is anonymous (DXF 70).
  */
  bool isAnonymous() const;

  /** \details
    Sets this Group object as anonymous (DXF 70).
    
    \remarks
    The name of this Group object will be set to an automatically generated anonymous group name.
  */
  void setAnonymous();

  /** \details
    Appends the specified entity to this Group object.  

    \param objectId [in]  Object ID of the entity to be appended.
  */
  void append(
    OdDbObjectId objectId);

  /** \details
    Appends the specified entities to this Group object. 
    \param objectIds [in]  Object IDs of the entities to be appended.
  */
  void append(
    const OdDbObjectIdArray& objectIds);

  /** \details
    Prepends the specified entity to this Group object.  

    \param objectId [in]  Object ID of the entity to be prepended.
  */
  void prepend(
    OdDbObjectId objectId);

  /** \details
    Prepends the specified entities to this Group object. 
    \param objectIds [in]  Object IDs of the entities to be prepended.
  */
  void prepend(
    const OdDbObjectIdArray& objectIds);

  /** \details
    Inserts the specified entity to this Group object at the specified index.  

    \param objectId [in]  Object ID of the entity to be inserted.
    \param insertionIndex [in]  Insertion index.
  */
  void insertAt(
    OdUInt32 insertionIndex, 
    OdDbObjectId objectId);

  /** \details
    Inserts the specified entities to this Group object at the specified index.
    \param objectIds [in]  Object IDs of the entities to be inserted.
  */
  void insertAt(
    OdUInt32 insertionIndex, 
    const OdDbObjectIdArray& objectIds);

  /** \details
    Removes the specified entity from this Group object.
    
    \param objectId [in]  Object ID of the entity to be removed.
  */
  void remove(
    OdDbObjectId objectId);

  /** \details
    Removes the entity at the specified index, 
    or the specified entities at or above the specified index,
    from this Group object.

    \param entityIndex [in]  Entity index.
  */
  void removeAt(
    OdUInt32 entityIndex);

  /** \details
    Removes the specified entities from this Group object.
    \param objectIds [in]  Object IDs of the entities to be removed.
  */
  void remove(
    const OdDbObjectIdArray& objectIds);

  /** \details
    Removes the entity at the specified index, 
    or the specified entities at or above the specified index,
    from this Group object.

    \param objectIds [in]  Object IDs of the entities to be inserted.

    \remarks
    If any of the entities in objectIds are not in this Group object,
    or are at an index > index, none of the entities will be removed.
  */
  void removeAt(
    OdUInt32 index, 
    const OdDbObjectIdArray& objectIds);

  /** \details
    Replaces the specified entity in this Group object with 
    the specified entity currently not in this Group object.

    \param oldId [in]  Object ID of the entity to be 
    \param newId [in]  Object ID of the entity to replace oldId.
  */
  void replace(
    OdDbObjectId oldId, 
    OdDbObjectId newId);

  /** \details
    Rearranges the entities within this Group object.

    \param fromIndex [in]  Index of the first entity to be moved.
    \param toIndex [in]  Destination index of the first entity moved.
    \param numItems [in]  Number of objects to move.
      
    \remarks
    This function transfers numItems starting at the fromIndex to the toIndex. 
  */
  void transfer(
    OdUInt32 fromIndex, 
    OdUInt32 toIndex, 
    OdUInt32 numItems);

  /** \details
    Clears the contents of this Group object.
    
    \remarks
    After clearing, this Group object contains no entities.
  */
  void clear();

  /** \details
    Returns the number of entities in this Group object.
  */
  OdUInt32 numEntities() const;

  /** \details
    Returns true if and only if this Group object contains the specified entity.
    
    \param pEntity [in]  Pointer to the entity being tested.
  */
  bool has(
    const OdDbEntity* pEntity) const;

  /** \details
    Returns the number of entities in this Group object, and their Object IDs.

    \param objectIds [out]  Receives the Object IDs.
  */
  OdUInt32 allEntityIds(
    OdDbObjectIdArray& objectIds) const;

  /** \details
    Returns the index of the specified entity within this Group object.

    \param objectId [in]  Object ID of the entity.
    \param index [out]  Index of objectId within this Group.

    Throws:
    <table>
    Exception            Cause
    eInvalidInput        objectId is not in this Group object.
    </table>
  */
  void getIndex(
    OdDbObjectId objectId, 
    OdUInt32& index) const;

  /** \details
    Reverses the order of the entities in this Group object.
  */
  void reverse();

  /** \details
      Sets the color of all entities in this Group object to the specified color.
  void setColor(const OdCmColor &color);
  */

  /** \details
    Sets the color index of all entities in this Group object to the specified value.
    
    \param colorIndex [in]  Color index.
  */
  void setColorIndex(
    OdUInt16 colorIndex);

  /** \details
    Sets the color of all entities in this Group object to the specified value.

    \param color [in]  Color.
  */
  void setColor(
    const OdCmColor& color);

  /** \details
    Sets the layer of all entities in this Group object to the specified value.
    
    \param layer [in]  Layer name.
  */
  void setLayer(
    const OdString& layer);

  /** \details
    Sets the layer of all entities in this Group object to the specified value.

    \param layerId [in]  Object ID of the LayerTableRecord.  
  */
  void setLayer(
    OdDbObjectId layerId);

  /** \details
    Sets the linetype of all entities in this Group object to the specified value.
    
    \param linetype [in]  Linetype name.
  */
  void setLinetype(
    const OdString& linetype);

  /** \details
    Sets the linetype of all entities in this Group object to the specified value.

    \param linetypeId [in]  Object ID of linetype.
  */
  void setLinetype(
    OdDbObjectId linetypeID);

  /** \details
    Sets the linetype scale of all entities in this Group object to the specified value.
    
    \param linetypeScale [in]  Linetype scale.
  */
  void setLinetypeScale(
    double linetypeScale);

  /** \details
    Sets the visibility of all entities in this Group object to the specified value.
    
    \param visibility [in]  Visibility.
  */
  void setVisibility(
    OdDb::Visibility visibility);

  /** \details
    Sets the PlotStyleName of all entities in this Group object to the specified value.

    \param plotStyleName [in]  PlotStyleName.
  */
  void setPlotStyleName(
    const OdString& plotStyleName);

  /** \details
    Sets the lineweight of all entities in this Group object to the specified value.
    
    \param lineWeight [in]  Lineweight.
  */
  void setLineweight(
    OdDb::LineWeight lineWeight);

  OdResult setHighlight(
    bool newVal);
  OdResult setMaterial( 
    const OdString& materialName);
  OdResult setMaterial(
    OdDbObjectId materialID);

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  virtual void copied (
    const OdDbObject* pObject, 
    const OdDbObject* pNewObject);

  virtual void appendToOwner(
    OdDbIdPair& idPair, 
    OdDbObject* pOwnerObject, 
    OdDbIdMapping& ownerIdMap);

  virtual OdResult subGetClassID(
    void* pClsid) const;

  virtual void subClose() ODRX_OVERRIDE;
  virtual OdDbObjectPtr subWblockClone(OdDbIdMapping& ownerIdMap, OdDbObject*, bool bPrimary) const ODRX_OVERRIDE;
  virtual OdDbObjectPtr subDeepClone(OdDbIdMapping& ownerIdMap, OdDbObject*, bool bPrimary) const ODRX_OVERRIDE;

  /*
      void setHighlight(bool newVal);
      virtual void applyPartialUndo(OdDbDwgFiler* undoFiler, OdRxClass* classObj);
      virtual OdResult subErase(bool erasing = true);
      virtual void goodbye(const OdDbObject* dbObj);
  */

};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbGroup object pointers.
*/
typedef OdSmartPtr<OdDbGroup> OdDbGroupPtr;

#include "TD_PackPop.h"

#endif //_ODDBGROUP_INCLUDED_

