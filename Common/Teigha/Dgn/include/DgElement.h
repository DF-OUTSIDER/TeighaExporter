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
#ifndef __DG_ELEMENT_H__
#define __DG_ELEMENT_H__


#include "TD_PackPush.h"

#include "Gi/GiDrawable.h"
#include "DgExport.h"
#include "DgMacros.h"
#include "DgElementId.h"
#include "DgElementIdArray.h"
#include "DgElementReactor.h"
#include "Ge/GeMatrix3d.h"
#include "IntArray.h"
#include "DgSubentId.h"
#include "DgElementIterator.h"
#include "OdBinaryData.h"

typedef OdSmartPtr<OdDgElement> OdDgElementPtr;

class OdDgElementImpl;
class OdDgDatabase;
class OdDgFiler;
class OdDgIdMapping;
class OdDgIdPair;
class OdDbGripData;

template <class T> class OdSharedPtr;
typedef OdSharedPtr<OdDbGripData> OdDgGripDataPtr;
typedef OdArray<OdDgGripDataPtr> OdDgGripDataPtrArray;
typedef OdArray<void*, OdMemoryAllocator<void*> > OdDgVoidPtrArray;

class OdDgLinkNode;
typedef OdSharedPtr<OdDgLinkNode> OdDgLinkNodePtr;


/** \details
  
    <group TG_Namespaces>
*/
namespace OdDg
{ 
  /** \details
    The file OdaDefs.h contains const definitions of DuplicateRecordCloning values
    that do not require the "OdDg::" scope resolution qualifier. They should be
    used only if there will be no naming conflicts.
   
    <group OdDg_Classes>
  */
  enum DuplicateRecordCloning
  {
    kDrcNotApplicable  = 0,   // Not applicable to the object.
    kDrcIgnore         = 1,   // If a duplicate record exists, use the existing record
                              // in the *database*, and ignore the clone.
    kDrcReplace        = 2,   // If a duplicate record exists, replace it with the cloned record.
    kDrcXrefMangleName = 3,   // Incoming record names are mangled with <Xref>$0$<name>
    kDrcMangleName     = 4,   // Incoming record names are mangled with $0$<name>
    kDrcUnmangleName   = 5,   // Unmangle the names mangled by kDrcMangleName, then default to kDrcIgnore.
                              // Typically used by RefEdit when checking records into the original *database*.  
    kDrcMax            = kDrcUnmangleName //  The maximum value of this enum.
  };

  /** \details
    The file OdaDefs.h contains const definitions of Visibility values
    that do not require the "OdDg::" scope resolution qualifier. They should be
    used only if there will be no naming conflicts.

    <group OdDg_Classes>
  */
  enum Visibility
  {
    kInvisible = 1,
    kVisible = 0
  };
}

/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgElement : public OdGiDrawable
{
  ODDG_DECLARE_MEMBERS(OdDgElement);
public:

  // All types of elements
  // 1. The most types below have appropriate classes that are derived from OdDgElement
  //    and have similar names (OdDgMultiline for kMultiline, etc)
  // 2. Some types determine the classes that are derived from OdDgElement but are not available to use
  // 3. Some types are special values for internal use only
  enum ElementTypes
  {
    kTypeUnapplicable = -1,
    kTypeUndefined = 0,
    kTypeLibraryCellHeader = 1,
    kTypeCellHeader = 2,
    kTypeLine = 3,
    kTypeLineString = 4,
    kTypeGroupData = 5,
    kTypeShape = 6,
    kTypeTextNode = 7,
    kTypeDigitizerDataSet = 8,
    kTypeDesignFileHeader = 9,
    kTypeLevelSymbology = 10,
    kTypeCurve = 11,
    kTypeComplexString = 12,
    kTypeConicSection = 13,
    kTypeComplexShape = 14,
    kTypeEllipse = 15,
    kTypeArc = 16,
    kTypeText = 17,
    kTypeSurface = 18,
    kTypeSolid = 19,
    kTypeBSplinePoles = 21,
    kTypePointString = 22,
    kTypeCone = 23,
    kTypeBSplineSurface = 24,
    kTypeBSplineSurfaceBoundary = 25,
    kTypeBSplineKnots = 26,
    kTypeBSplineCurve = 27,
    kTypeBSplineWeights = 28,
    kTypeDimension = 33,
    kTypeSharedCellDefinition = 34,
    kTypeSharedCellReference = 35,
    kTypeMultiline = 36,
    kTypeTagElement = 37,
    kTypeDgnStoreComponent = 38,
    kTypeDgnStoreHeader = 39,
    kTypeApplicationData = 66,
    kTypeRasterHeader = 87,
    kTypeRasterComponent = 88,
    kTypeRasterAttachmentHeader = 90,
    kTypeRasterAttachmentComponent = 91,
    kTypeRasterFrame = 94,
    kTypeTableEntry = 95,
    kTypeTableHeader = 96,
    kTypeViewGroup = 97,
    kTypeView = 98,
    kTypeLevelMask = 99,
    kTypeReferenceAttachmentHeader = 100,
    kTypeMatrixHeader = 101,
    kTypeMatrixIntegerData = 102,
    kTypeMatrixDoubleData = 103,
    kTypeMesh = 105,
    kTypeExtraGraphicsElement = 106,
    kTypeExtraNonGraphicsElement = 107,
    kTypeReferenceOverride = 108,
    kTypeNamedGroupHeader = 110,
    kTypeNamedGroupComponent = 111,
    kTypeEOF = 127
  };

  // Grip points support

  enum OsnapMode
  {
    kOsModeEnd      = 1,  // Endpoint
    kOsModeMid      = 2,  // Midpoint
    kOsModeCen      = 3,  // Center
    kOsModeNode     = 4,  // Node
    kOsModeQuad     = 5,  // Quadrant
    kOsModeIntersec = 6,  // Intersection
    kOsModeIns      = 7,  // Insertion point
    kOsModePerp     = 8,  // Perpendicular
    kOsModeTan      = 9,  // Tangent
    kOsModeNear     = 10, // Nearest
    kOsModeApint    = 11, // Apparent intersection
    kOsModePar      = 12, // Parallel
    kOsModeStart    = 13  // Startpoint
  };

  enum DragStat
  {
    kDragStart = 0,
    kDragEnd   = 1,
    kDragAbort = 2
  };

  enum GripStat
  {
    kGripsDone          = 0,
    kGripsToBeDeleted   = 1,
    kDimDataToBeDeleted = 2
  };

  // Constructor/destructor
  OdDgElement(OdDgElementImpl* pImpl);
  OdDgElement();
  virtual ~OdDgElement();

  //
  virtual OdResult getGeomExtents(const OdDgElementId& idView, OdGeExtents3d& extents) const;
  // OdGiDrawable overridden
  OdResult getGeomExtents(OdGeExtents3d& extents) const; //block overloading
  OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;
  bool subWorldDraw(OdGiWorldDraw* pWd) const;
  void subViewportDraw(OdGiViewportDraw* pVd) const;
  virtual bool isPersistent() const;
  virtual OdDbStub* id() const;
  virtual void setGsNode(OdGsCache* pGsNode);
  virtual OdGsCache* gsNode() const;

  // OdRxObject overridden
  virtual void addRef();
  virtual void release();
  virtual long numRefs() const;

  /** \details
  Applies the specified 3D transformation matrix to this entity.

  \param xfm [in]  3D transformation matrix.

  \returns
  Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult transformBy( const OdGeMatrix3d& xfm );

    /** \details
    Creates a copy of this element, and applies the supplied transformation
    to the newly created copy.

    \param xfm [in]  3D transformation matrix.
    \param pCopy [out]  Receives a SmartPointer to the newly created copy.

    \returns
    Returns eOk if successful, or an appropriate error code if not.
    */
  virtual OdResult getTransformedCopy(
    const OdGeMatrix3d& xfm,
    OdDgElementPtr& pCopy) const;

  /** \details
    Returns the element type.
  */
  virtual ElementTypes getElementType() const;

  /** \details
    Returns the element subtype.
  */
  virtual OdUInt8 getElementSubType() const;

  /** \details
    Returns the element ID of this element's owner.
    
    \remarks
    Returns a null ID if this element has not been added to a database,
    or if OdDgElement::setOwnerId has not been called.
  */
  OdDgElementId ownerId() const;

  /** \details
    Sets this element's ownerId data member. 
    
    \param ownerId [in]  Owner's elementId.
    
    \remarks
    This function lets this element know its owner; it does not
    notify the owner. 
  */
  virtual void setOwnerId(
    OdDgElementId ownerId);

  /** \details
    Returns the Object ID of this object.
    
    \remarks
    Returns a null ID if this object has not been added to a database.
  */
  OdDgElementId elementId() const;

  /** \details
  Returns the time of the last modification (milliseconds since midnight, January 1, 1970)
  */
  double getLastModifiedTime() const;

  /** \details
    Returns the OdDgDatabase that contains this object.
  */
  OdDgDatabase* database() const;

  /** \details
    Returns true if and only if this object is marked as erased.
  */
  bool isErased() const;

  /** \details
  Returns true if and only if this object has not been closed since it was created.
  */
  bool isNewObject() const;

  /** \details
    Returns true if and only if this object is open OdDg::kForRead.
  */
  bool isReadEnabled() const;

  /** \details
    Returns true if and only if this object is open OdDg::kForWrite.
  */
  bool isWriteEnabled() const;

  /** \details
    Returns true if and only if this object is open OdDg::kForNotify.
  */
  bool isNotifyEnabled() const;

  /** \details
    Returns true if and only this object is a database -resident object.
  */
  bool isDBRO() const;

  /** \details
    Returns true if and only if an object derived from OdDbEntity has been modified.
    
    \remarks
    
    This function returns true if and only if an object derived from OdDgElement
    either
    
        1) Calls assertWriteEnabled()
        
        2) Calls recordGraphicsModified(true).
  */
  bool isModifiedGraphics() const;

  /** \details
    Throws an exception if this object is not open OdDg::kForRead.
    
    \remarks
    This function should be used only inside member functions that do not modify this object. 
    It should be the first function called by these functions.
    
    Throws:
    <table>
    Exception             Cause
    eNotOpenForRead       !isReadEnabled()
    </table>
  */
  void assertReadEnabled() const;

  /** \details
    Throws an exception if this object is not open OdDg::kForWrite,
    and controls automatic undo and notification of modifications.
    
    \param autoUndo [in]  Specifies if automatic undo should be done.
    \param recordModified [in]  Specifies if graphics are to be updated, and
                        "openedForModify", "modified" and "modifiedGraphics"
                        notifications are to be sent. 
    
    \remarks
    This function should be used only inside member functions that modify this object. 
    It should be the first function called by these functions.
    
    Throws:
    <table>
    Exception             Cause
    eNotOpenForWrite     !isWriteEnabled()
    </table>
  */
  void assertWriteEnabled(
    bool autoUndo = true, 
    bool recordModified = true);

  /** \details
    Throws an exception if this object is not open OdDg::kForNotify.
    
    \remarks
    This function should be used only inside member functions that are used only when this object
    is open OdDg::kForNotify. It should be the first function called by these functions.
    
    Throws:
    <table>
    Exception            Cause
    eInvalidOpenState    !isNotifyEnabled()
    </table>
  */
  void assertNotifyEnabled() const;

  /** \details
    Controls the undo recording of this object in OdDgDatabase.

    \param disable [in]  Boolean to control undo recording.
    
    \remarks
    Disabling undo recording does not erase the undo recording; it merely suspends it.
    Undo recording is initially off for newly created OdDbDatabase objects.
  */
  void disableUndoRecording(bool disable);

  /** \details
    Returns the undo filer associated with this object.
    
    \remarks
    This function is typically used by custom classes using partial Undo
    to add Undo information to the filer that would be used by this
    object applyPartialUndo().
    
    Throws:
    <table>
    Exception           Cause
    eNotOpenForWrite    !isWriteEnabled()
    </table>
  */
  OdDgFiler* undoFiler();

  /** \details
    Notification function called each time an Undo operation is performed 
    this object is using partial Undo.
    
    \param pUndoFiler [in]  A pointer to the undo filer with the partial undo information.
    \param pClassObj     [in] A pointer to the OdRxClass object for the class that will handle the Undo.
    
    \remarks
    An object indicates it's using the partial undo mechanism, if and only if
    it has set autoUndo false in all calls to assertWriteEnabled().
    
    This member function must know which types of fields to scan, 
    and must stop after reading what it it needs.

    If the class type specified by pClassObj does not matches the class of this object,
    this member function must call the parent class's applyPartialUndo() 
    and return whatever it returns.

    If it does match the class of this object, this member function must use pUndoFiler to read the undo data, 
    then typically use this object's set() method.
    
    Throws:
    <table>
    Exception                   Cause
    eNotThatKindOfClass         pClassObj != OdDgElement::desc()
    </table>
  */

  virtual void applyPartialUndo( OdDgFiler* pUndoFiler, OdRxClass* pClassObj);

  /** \details
    Upgrades this object from OdDg::kForRead to OdDg::kForWrite if there is only one reader, and
    returns isWriteEnabled();
  */
  void upgradeOpen();
  
  /** \details
    Downgrades this object from OdDg::kForWrite to OdDg::kForRead, and
    returns isReadEnabled();
     
    \remarks
    Any pending changes to this object are committed to the database. 
  */
  void downgradeOpen();

  /** \details
    Notification function called by the Teigha™ for .dgn files framework immediately before an object is opened. 

    \param mode [in]  Mode in which the object is being opened.
    
    \remarks
    This function is notified just before an object is to be opened; giving this function
    the ability to cancel the object being opened.
         
    Returns eOk if and only if open() is to continue.

    Overriding this function in a child class allows a child instance to be notified ach time an
    object is opened.

    When overriding this function:
    
    1)  If the OdDgElement's state is incorrect, return 
        something other than eOk.
    
    2)  If the parent class's subOpen() returns anything
        other than eOk, immediately return it. 
    
    3)  If other actions are required before open, do them.
    
    4)  Return eOk.
    
    The default implementation of this function does nothing but return eOk.  This function can be
    overridden in custom classes.
  */
  virtual OdResult subOpen(
    OdDg::OpenMode mode);

  /** \details
    Called as the first operation as this object is being closed, for
    database -resident objects only. 
    
    When overriding this function:
    
    1)  If the OdDgElement's state is incorrect, return 
        something other than eOk.
    
    2)  If the parent class's subClose() returns anything
        other than eOk, immediately return it. 
    
    3)  If other actions are required before close, do them.
    
    4)  Return eOk.

    If you must make changes to this object's state, either make them after
    step 2, or roll them back if step 2 returns false. 
    
    The default implementation of this function does nothing but return eOk.  This function can be
    overridden in custom classes.
  */
  virtual OdResult subClose();
  
  /** \details
    Sets the erased mode of this object. 

    \param eraseIt [in]  Boolean to specify if object is to be erased or unerased.

    \remarks
    Erased objects are not deleted from the database or from memory.

    Erased objects are not filed when the database is saved or sent to a DGN file.
  */
  OdResult erase(
    bool eraseIt = true);

  virtual OdResult subTransformBy( const OdGeMatrix3d& xfm );

  virtual OdResult subGetTransformedCopy( const OdGeMatrix3d& xfm, 
                                            OdDgElementPtr& pCopy) const;
  
  /** \details
    Called as the first operation as this object is being erased or unerased. 

    \param erasing [in]  A copy of the erasing argument passed to erase().

    \remarks
    This function is notified just before the current object is to be erased, giving this function
    the ability to cancel the erase.
         
    Returns eOk if and only if erase() is to continue.

    When overriding this function:
    
    1)  If the OdDgElement's state is incorrect, return 
        something other than eOk.
    
    2)  If the parent class's subErase() returns anything
        other than eOk, immediately return it. 
    
    3)  If other actions are required before erase, do them.
    
    4)  Return eOk.

    If you must make changes to this object's state, either make them after
    step 2, or roll them back if step 2 returns other than eOk. 
    
    The default implementation of this function does nothing but return eOk.  This function can be
    overridden in custom classes.
    
  */
  virtual OdResult subErase(
    bool erasing);

  /** \details
    Copies the contents of the specified object into this object when possible.

    \param pSource [in]  Pointer to the source object.

    \remarks
    The source object and this object need not be of the same type.  
    This function is meaningful only when implemented by custom classes.
    
    Throws:
    <table>
    Exception           Cause
    eNullObjectPointer  pSource->isNull()
    </table>
  */
  virtual void copyFrom(
    const OdRxObject* pSource);

  /** \details
    Reads the DGN format data of this object from the specified file.
       
    \param pFiler [in]  Pointer to the filer from which the data are to be read.
    
    \remarks
    This function calls dgnInFields(pFiler),
    then loads any data associated with this object.
  */
  virtual void dgnIn(
    OdDgFiler* pFiler);
  
  /** \details
    Writes the DGN format data of this object to the specified filer. 
    
    \param pFiler [in]  Pointer to the filer to which the data are to be written.
    
    \remarks
    This function calls this object's dgnOutFields() function,
    writes loads any data associated with this object.
  */
  virtual void dgnOut(
    OdDgFiler* pFiler) const;

  /** \details
    Reads the DGN data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \returns
    Returns the filer status.
    \remarks
    This function is called by dgnIn() to allow the object to read its data.

    When overriding this function:
     
    
    1)  Call assertWriteEnabled(). 
    2)  Call the parent class's dgnInFields(pFiler). 
    3)  If it returns eOK, continue; otherwise return whatever the parent's dgnInFields(pFiler) returned. 
    4)  Call the OdDgFiler(pFiler) methods to read each of the object's data items in the order they were written.
    5)  Return pFiler->filerStatus().
  */
  virtual OdResult dgnInFields(
    OdDgFiler* pFiler);
  
  /** \details
    Writes the DGN data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
    
    \remarks
   
    This function is called by dgnIn() to allow the object to write its data.

    When overriding this function:
     
    
    1)  Call assertReadEnabled(). 
    2)  Call the parent class's dgnOutFields(pFiler). 
    3)  Call the OdDgFiler(pFiler) methods to write each of the object's data items in the order they were written.
  */
  virtual void dgnOutFields(
    OdDgFiler* pFiler) const;

  /** \details
    Writes object references of this object during wblock clone. 

    \param pFiler [in]  Pointer to the wblock clone filer to which data are written.
    
    \remarks
   
    To save reference for wblock clone use OdDgFiler::wrHardPointerId(...)
  */
  virtual void copyFieldReferencesForWBlockClone( 
    OdDgFiler* pFiler ) const;

  /** \details
    Update object references of this object after wblock clone. 

    \param idMap [in]  Owner's ID map.
  */

  virtual void updateFieldReferencesForWBlockClone( 
    const OdDgIdMapping& idMap );

    /** \details
    Writes linkage references of this object during wblock clone. 

    \param pFiler [in]  Pointer to the wblock clone filer to which data are written.
    
    \remarks
   
    To save reference for wblock clone use OdDgFiler::wrHardPointerId(...)
    */

  virtual void copyLinkageReferencesForWBlockClone( 
    OdDgFiler* pFiler ) const;

  /** \details
    Update linkage references of this object after wblock clone. 

    \param idMap [in]  Owner's ID map.
  */

  virtual void updateLinkageReferencesForWBlockClone( 
    const OdDgIdMapping& idMap );

  /** \details
    Writes x attribute references of this object during wblock clone. 

    \param pFiler [in]  Pointer to the wblock clone filer to which data are written.
    
    \remarks
   
    To save reference for wblock clone use OdDgFiler::wrHardPointerId(...)
  */

  virtual void copyXAttributeReferencesForWBlockClone( 
    OdDgFiler* pFiler ) const;

  /** \details
    Update x attribute references of this object after wblock clone. 

    \param idMap [in]  Owner's ID map.
  */

  virtual void updateXAttributeReferencesForWBlockClone(
    const OdDgIdMapping& idMap );

  /** \details
    Returns all linkages.
  */
  void getLinkages(
    OdRxObjectPtrArray& linkages) const;

  /** \details
    Returns the linkages with specified application ID.
    \remarks
    For all methods working with linkages, appId == 0 for DMRS linkages. It is acceptable,
          because DMRS linkages do not have appId (aka primaryId) and appId == 0 is not available for non-DMRS linkages.
  */
  void getLinkages(
    OdUInt16 appId,
    OdRxObjectPtrArray& linkages) const;

  /** \details
    Sets the linkages with specified application ID.
  */
  void setLinkages(
    OdUInt16 appId,
    const OdRxObjectPtrArray& linkages);

  /** \details
    Removes the linkages with specified application ID.
  */
  void removeLinkages(
    OdUInt16 appId);

  /** \details
    Adds the linkage with specified application ID.
  */
  void addLinkage(
    OdUInt16 linkageId,
    OdRxObjectPtr pLinkage);

  /** \details
    Adds the linkage data with specified application ID.
  */
  void addLinkage(
    OdUInt16 linkageId,
    OdUInt8* pData,
    OdUInt32 dataSize);

  /** \details
    Adds the linkage data with specified application ID.
  */
  void addLinkage(
    OdUInt16 linkageId,
    OdBinaryData const& data);

  /** \details
    Returns the String Linkage value.
  */
  OdString getStringLinkage(
    OdUInt32 stringId) const;

  /** \details
    Sets the String Linkage value.
  */
  void setStringLinkage(
    OdUInt32 stringId,
    const OdString& str);

  /** \details
    Returns the XData Linkage value.
  */
  OdRxObjectPtr getXDataLinkage(
    const OdString& appName = OdString::kEmpty) const;

  /** \details
    Sets the XData Linkage value.
  */
  void setXDataLinkage(
    OdRxObjectPtr pResBuf);

  /** \details
    Returns the XAttributes.
  */
  void getXAttributes(
    OdRxObjectPtrArray& xattributes) const;

  /** \details
    Returns the XAttributes with specified handler ID.
  */
  void getXAttributes(
    OdUInt32 handlerId,
    OdRxObjectPtrArray& xattributes) const;

  /** \details
    Sets the XAttributes with specified handler ID.
  */
  void setXAttributes(
    OdUInt32 handlerId,
    OdRxObjectPtrArray& xattributes);

  /** \details
    Removes the XAttributes with specified handler ID.
  */
  void removeXAttributes(
    OdUInt32 handlerId);

  /** \details
    Adds the XAttribute with specified handler ID.
  */
  void addXAttribute(
    OdUInt32 handlerId,
    OdRxObjectPtr XAttribute);

  /** \details
    Adds the specified transient reactor to this element's reactor list.

    \param pReactor [in]  Pointer to the transient reactor object.

    \remarks
    An element must be open either OdDg::kForRead or OdDg::kForWrite in order to add a transient reactor.
  */
  void addReactor(
    OdDgElementReactor* pReactor) const;
  
  /** \details
    Removes the specified transient reactor from this element's reactor list.

    \param pReactor [in]  Pointer to the transient reactor object.

    \remarks
    An element must be open either OdDg::kForRead or OdDg::kForWrite in order to remove a transient reactor.
  */
  void removeReactor(
    OdDgElementReactor* pReactor) const;
  
  /** \details
    Adds the specified persistent reactor to this element's reactor list.

    \param elmId [in]  Element ID of the persistent reactor.

    \remarks
    An element must be open OdDg::kForWrite in order to add a persistent reactor.

    If the persistent reactor does not have an owner, 
    it isn't saved with the drawing.
  */
  virtual void addPersistentReactor(
    const OdDgElementId& elmId);
  
  /** \details
    Removes the specified persistent reactor from this element's reactor list.

    \param elmId [in]  Element ID of the persistent reactor.

    \remarks
    An element must be open OdDg::kForWrite in order to remove one of its persistent reactors.
  */
  virtual void removePersistentReactor(
    const OdDgElementId& elmId);
  
  /** \details
    This method returns true if elmId is the Element ID of a reactor attached to this element. Otherwise, it returns false.

    \param elmId [in]  Element ID of the persistent reactor.
  */
  bool hasPersistentReactor(
    const OdDgElementId& elmId) const;

  /** \details
    Returns this element's persistent reactors.

    \param elmIds [out]  Receives Element IDs of this element's persistent reactors.
  */
  void getPersistentReactors(
    OdDgElementIdArray& elmIds);

  /** \details
    Returns this element's transient reactors.

    \param reactors [out]  Receives this element's transient reactors.
  */
  void getTransientReactors(
    OdDgElementReactorArray& reactors);
  
  /** \details
    Allows a subelement of a complex element to notify its root element that it has been changed. 
    
    \param pSubElm [in]  Pointer to the modified subelement.

    \remarks
    Here's how it's supposed to work:
    
    1)  The subelement class's close() calls its triggers a "modified" notification which
        calls its xmitPropagateModify().
    2)  The subelement class calls the its parent's recvPropagateModify() its element's pointer.
    3)  The owner's class's recvPropagateModify() sends a "modified" notification to the top of its class.
    
    The default implementation of this function does nothing. This function can be overridden in custom classes.
  */
  virtual void recvPropagateModify(
    const OdDgElement* pSubElm);
  
  /** \details
    Allows a subelement of a complex element to notify its root element that it has been changed. 

    \remarks
    The default implementation of this function inform the database 
    in which the owner resides has been modified; triggering reactors 
    attached to it. This function can be overridden in custom classes.
  */
  virtual void xmitPropagateModify() const;

    /** \details
    Performs a deep clone of this object.
    
    \param ownerIdMap [in]  Owner's ID map.

    \remarks
    Returns a SmartPointer to the newly created clone,
    and adds a record to the specified ID map. 

    If the cloning operation fails, a null SmartPointer is returned.
    
    A deep clone is a clone of this object and everything it owns.
    
    This function should not be called by client code; use OdDgDatabase::deepCloneObjects() instead.

    This function can be overridden in custom classes.

    The default implementation of this function appends the cloned object to the specified owner object.   
    */
  virtual OdDgElementPtr deepClone(
    OdDgIdMapping& ownerIdMap) const;
  
  /** \details
    Performs a shallow clone of this object. 
    
    \param ownerIdMap [in/out] Owner's ID map.

    \remarks
    Returns a SmartPointer to the newly created clone,
    and adds a record to the specified ID map. 

    If the cloning operation fails, a null SmartPointer is returned.
    
    A shallow clone is a clone of only this object.
    
    This function should not be called by client code; use OdDgDatabase::wblockCloneObjects() instead.

    The default implementation of this function calls subWblockClone() for all objects hard-referenced by this object.
    This function can be overridden in custom classes.
    
    \sa
    subDeepClone()

  */
        virtual OdDgElementPtr wblockClone(
    OdDgIdMapping& ownerIdMap) const;

  /** \details
    This function appends this object to the specified owner object.
    
    \param idPair [in]  ID pair to append.
    \param pOwnerObject [in]  Pointer to the owner object.
    \param ownerIdMap [in/out] Owner's ID map.
    
    \remarks
    Adds a record to the specified ID map.     
    
    This function is used internally to subDeepClone() and subWblockClone().
    
    Throws:
    <table>
    Exception              Cause
    eInvalidOwnerObject    !pOwnerObject->get()
    </table>
    
  */
  virtual void appendToOwner(
    OdDgIdPair& idPair, 
    OdDgElement* pOwnerObject, 
    OdDgIdMapping& ownerIdMap);

  /** \details
    Called on a new created cloned object to indicate that its Object ID is not valid. 
    
    \remarks
    The flags are cleared when the OdDgElement::subDeepClone or OdDgElement::subWblockClone() operation has been completed.
  */
  void setOdDgElementIdsInFlux();

  /** \details
      Returns true if and only if this object's Object ID is not valid because
      the OdDgElement::subDeepClone or OdDgElement::subWblockClone() have yet to be completed.
  */
  bool isOdDgElementIdsInFlux() const;

  /** \details
    Notification function called whenever the notifying element has had its clone() member function called. 
    
    \param pElement [in]  Pointer to the notifying element.
    \param pNewElement [in]  Pointer to the element resulting from the copy.

    \remarks
    The default implementation of this function does nothing. This function can be
    overridden in custom classes.
  */
  virtual void copied(
    const OdDgElement* pElement, 
    const OdDgElement* pNewElement);

  /** \details
    Notification function called whenever an element has been erased or unerased.

    \param pElement [in]  Pointer to the element that was erased/unerased.
    \param erasing [in]  True if and only if this element is being erased.
    
    \remarks
    The default implementation of this function does nothing. This function can be
    overridden in custom classes.
  */
  virtual void erased(
    const OdDgElement* pElement, 
    bool erasing = true);

  /** \details
    Notification function called just before an element is deleted from memory. 
    
    \param pElement [in]  Pointer to the element that is being deleted.

    \remarks
    The default implementation of this function does nothing. This function can be
    overridden in custom classes.
  */
  virtual void goodbye(
    const OdDgElement* pElement);

  /** \details
    Notification function called whenever an element is opened for modify OdDg::kForWrite.

    \param pElement [in]  Pointer to the element that is being opened.

    \remarks
    The default implementation of this function does nothing. This function can be
    overridden in custom classes.
  */
  virtual void openedForModify(
    const OdDgElement* pElement);

  /** \details
      Notification function called whenever an element is opened OdDg::kForWrite, a function
      has been called that could modify the contents of this element, and this element is now being
      closed.

      \param pElement [in]  Pointer to the element that is being closed after being modified.

      \remarks
      The default implementation of this function does nothing. This function can be
      overridden in custom classes.
  */
  virtual void modified(
    const OdDgElement* pElement);

  /** \details
    Notification function called whenever an element derived from OdDgGraphicsElement is modified.
    
    \param pElement [in]  Pointer to the modified element.
    
    \remarks
    The default implementation of this function does nothing. This function can be
    overridden in custom classes.
    
    This function is called whenever the following has occurred.
    
    1)  The calling element is opened OdDg::kForWrite.
    
    2)  One of its member functions either
    
        a Calls its assertWriteEnabled with recordModified == true.
        
        b Calls its recordGraphicsModified(true).
    
    3) The calling element is being closed.
    
    This function is called whenever the element as been modified. It therefore
    indicates only that the graphics for it may have changed.
  */
  virtual void modifiedGraphics(
    const OdDgElement* pElement);

  /** \details
    Notification function called whenever the attribute linkages has been written
    to the notifying element.
    
    \param pElement [in]  Pointer to the notifying element.
    
    \remarks
    Notification is made upon pElement->close().

    The notifying element is open OdDg::kForRead.
    
    The default implementation of this function does nothing. This function can be
    overridden in custom classes.
  */
  virtual void modifiedLinkages(
    const OdDgElement* pElement);

  /** \details
    Notification function called whenever a method of the
    specified subelement has called assertWriteEnabled()
    
    \param pElement [in]  Pointer to the notifying element.
    \param pSubElm [in]  A pointer to the subelement owned by pElement.
    
    \remarks
    Notification is made upon pElement->close().
  */
  virtual void subElementModified(
    const OdDgElement* pElement, 
    const OdDgElement* pSubElm);

  /** \details
    Notification function called whenever the notifying
    element is in the midst an Undo operation that
    is undoing modifications.
    
    \param pElement [in]  Pointer to the notifying element.
    
    \remarks
    The notifying element is open OdDg::kForRead.
    
    The default implementation of this function does nothing. This function can be
    overridden in custom classes.
  */
  virtual void modifyUndone(
    const OdDgElement* pElement);

  /** \details
    Notification function called whenever the Undo process
    processes the appending of the notifying element to the database.
    
    \param pElement [in]  Pointer to the notifying element.
    
    \remarks
    The notifying element is marked as erased. It is not removed the database, and can be unerased,
    even 'before' it was created. 
    
    The default implementation of this function does nothing. This function can be
    overridden in custom classes.
  */
  virtual void unappended(
    const OdDgElement* pElement);

  /** \details
    Notification function called whenever a Redo process
    processes the reappending of the notifying element to the database.
    
    \param pElement [in]  Pointer to the notifying element.
    
    \remarks
    
    The default implementation of this function does nothing. This function can be
    overridden in custom classes.
  */
  virtual void reappended(
    const OdDgElement* pElement);

  /** \details
    Notification function called immediately before an element is closed.

    \param elementId [in]  Element ID of the element that is being closed.

    \remarks
    The default implementation of this function does nothing. This function can be
    overridden in custom classes.
  */
  virtual void closed(
    const OdDgElementId& elementId);

  /** \details
    Returns true if and only if this element is marked as locked.    
  */
  virtual bool getLockedFlag();
  virtual void setLockedFlag(bool flag);

  /** \details
    Sets the bit flag indicating the entity's geometry is changed.

    \param graphicsModified [in]  New value.

    \remarks
    If true, assures that modifiedGraphics() will be called as the entity is being closed, even
    if the object was not opened for write.
  */
  void recordGraphicsModified(bool graphicsModified = true);

  /** \details
    Replaces this database -resident (DBRO) object with the specified non- database -resident (NDBRO) object,
    while retaining this object's elementId, handle, owner, and reactor list.
   
    \param pNewElement [in]  Pointer to the object with which to replace this object in the database.
                    
    \remarks
    This object must be open OdDg::kForWrite. 
    
    The replacement object will opened OdDg::kForWrite, and must be closed.
    
    It is up to the caller to delete the replaced (this) object.
    
    Throws:
    <table>
    Exception               Cause
    eIllegalReplacement     This object is NDBRO or NewObject is DBRO.
    </table>
  */
  void handOverTo( OdDgElement* pNewElement );
  
  /** \details
    Called as the first operation of the handOverTo function.  
    
    \param pNewElement [in]  Pointer to the object with which to replace this object in the database.

    \remarks
    This function allows custom classes to populate the new object.

    \remarks
    Overriding this function in a child class allows a child instance to be notified each time an
    object is handed over.
    
    This function is notified just before an object is to be handed over; giving this function
    the ability to cancel the handover.
         
    When overriding this function:
    
    1)  If the OdDgElements's state is incorrect, throw exception.
    
    2)  Call parent class's subHandover().
    
    3)  If other actions are required before handover, do them.
    
    The default implementation of this function does nothing.  This function can be
    overridden in custom classes.

  */
  virtual void subHandOverTo(
    OdDgElement* pNewElement);
  
  /** \details
    Swaps the elementIDs, and handles between
    this object and another object.

    \param otherId [in]  Element ID of object with which to swap.
    
    Throws:
    <table>
    Exception               Cause
    eIllegalReplacement     Either object is NDBRO.
    </table>
  */
  void swapIdWith(
    OdDgElementId otherId );
  
  /** \details
    Called as the first operation of swapIdWith

    \param otherId [in]  Element ID to be swapped with this object's Element ID.

    \remarks
    This function is notified just before an object is to be opened; giving this function
    the ability to cancel the object being swapped or perform necessary actions.
         
    When overriding this function:
    
    1)  If the OdDgElement's state is incorrect, throw exception.
    
    2)  Call parent class's subSwapIdWith().
    
    3)  If other actions are required before swapping IDs, do them.
    
    The default implementation of this function does nothing.  This function can be
    overridden in custom classes.
 
  */
  virtual void subSwapIdWith(
    const OdDgElementId& otherId );

  /** \details
  Returns all appropriate object snap points of this entity.

  \param osnapMode [in]  The object snap mode being queried.
  \param gsSelectionMark [in]  The GS marker of the subentity being queried.
  \param pickPoint [in]  The WCS point being queried.
  \param lastPoint [in]  The WCS point picked before pickPoint.
  \param xWorldToEye [in]  The WCS->DCS transformation matrix.
  \param snapPoints [in/out] Receives an array of UCS object snap points.

  \remarks
  Object snap points are appended to the specified array.

  osnapMode must be one of the following:

  <table>
  Name                      Value   Description
  OdDb::kOsModeEnd          1       Endpoint
  OdDb::kOsModeMid          2       Midpoint
  OdDb::kOsModeCen          3       Center
  OdDb::kOsModeNode         4       Node
  OdDb::kOsModeQuad         5       Quadrant
  OdDb::kOsModeIntersec     6       Intersection
  OdDb::kOsModeIns          7       Insertion point
  OdDb::kOsModePerp         8       Perpendicular
  OdDb::kOsModeTan          9       Tangent
  OdDb::kOsModeNear         10      Nearest
  OdDb::kOsModeApint        11      Apparent intersection
  OdDb::kOsModePar          12      Parallel
  OdDb::kOsModeStart        13      Unknown
  </table>

  */

  virtual OdResult getOsnapPoints( OdDgElement::OsnapMode osnapMode,
                                     OdGsMarker gsSelectionMark,
                                       const OdGePoint3d& pickPoint,
                                         const OdGePoint3d& lastPoint,
                                           const OdGeMatrix3d& xWorldToEye,
                                             OdGePoint3dArray& snapPoints ) const;

  virtual OdResult getOsnapPoints( OdDgElement::OsnapMode osnapMode,
                                     OdGsMarker gsSelectionMark,
                                       const OdGePoint3d& pickPoint,
                                         const OdGePoint3d& lastPoint,
                                           const OdGeMatrix3d& xWorldToEye,
                                             OdGePoint3dArray& snapPoints,
                                               const OdGeMatrix3d& insertionMat) const;

  virtual bool     isContentSnappable() const;

  /** \details
  Returns all grip points of this entity.


  \param gripPoints [in/out] Receives an array of WCS grip points.

  \remarks
  Grip points are appended to the specified array.
  */
  virtual OdResult getGripPoints( OdGePoint3dArray& gripPoints ) const;

  /** \details
  Moves the specified grip points of this entity.

  \param indices [in]  Array of indices.
  \param offset [in] The direction and magnitude of the grip points offset (WCS).

  \remarks
  Each element in gripPoints has a corresponding entry in indices, which specifies the index of
  the grip point as returned by getGripPoints.
  */
  virtual OdResult moveGripPointsAt( const OdIntArray& indices, const OdGeVector3d& offset );

  /** \details
  Returns OdDgGripData objects for grip points of this entity.

  \param grips [in/out] Receives an array of OdDbGripData objects.

  \remarks
  Grip points are appended to the specified array.
  */
  virtual OdResult getGripPoints( OdDgGripDataPtrArray& grips,
                                    const double curViewUnitSize,
                                      const int gripSize,
                                        const OdGeVector3d& curViewDir,
                                          const int bitFlags ) const;

  /** \details
  Moves the specified grip points of this entity.

  \param gripAppData [in]  Array of appData pointers from the OdDbGripData objects (one for each grip point to be moved).
  \param offset [in] The direction and magnitude of the grip points offset (WCS).
  */
  virtual OdResult moveGripPointsAt( const OdDgVoidPtrArray& grips, 
                                       const OdGeVector3d& offset,
                                         int bitFlags );

  /** \details
  Returns all stretch points of this entity.

  \param stretchPoints [in/out] Receives an array of WCS stretch points.

  \remarks
  Stretch points are appended to the specified array.
  */

  virtual OdResult getStretchPoints( OdGePoint3dArray& stretchPoints ) const;

  /** \details
  Moves the specified stretch points of this entity.

  \param stretchPoints [in]  Array of moved grip points.
  \param indices [in]  Array of indices.
  \param offset [in] The direction and magnitude of the stretch points offset (WCS).

  \remarks
  Each element in stretchPoints has a corresponding entry in indices, which specifies the index of
  the stretch point as returned by getStretchPoints.
  */
  virtual OdResult moveStretchPointsAt( const OdIntArray& indices, const OdGeVector3d& offset );

  /** \details
  Notification function called when this entity is involved in a drag operation.

  \param status [in]  Status of the drag operation.
  */
  virtual void dragStatus( const OdDgElement::DragStat status );

  /** \details
  Notification function called when this entity is involved in a grip operation.

  \param status [in]  Status of the grip operation.
  */
  virtual void gripStatus( const OdDgElement::GripStat status );

  /** \details
  Returns the cloneMeForDragging flag for this entity.
  */
  virtual bool cloneMeForDragging();

  /** \details
  Returns the hideMeForDragging flag for this entity.
  */
  virtual bool hideMeForDragging() const;

  virtual void highlight(bool bDoIt = true, const OdDgFullSubentPath* pSubId = 0, bool highlightAll = false) const;

  /** \details
  Explodes this entity into a set of simpler entities.

  \param entitySet [in/out] Receives an array of pointers to the new entities.

  \returns
  Returns eOk if successful, or an appropriate error code if not.

  \remarks
  Entities resulting from the explosion are appended to the specified array.

  The newly created entities are not database resident.

  The default implementation of this function returns eNotApplicable. This function can be
  overridden in custom classes.
  */

  virtual OdResult explode( OdRxObjectPtrArray& entitySet) const;

  /** \details
  Explodes this entity into a set of simpler entities, and adds them to model or shared cell definition.

  \param pParentElement [in]  Pointer to OdDgModel, OdDgSheetModel, OdDgCellHeader, or OdDgSharedCellDefinition element.
  \param ids [in/out] Receives an array of Element IDs of the new entities.

  \remarks
  Entities resulting from the explosion are appended to the specified array.

  The newly created entities are database resident.

  The default implementation of this function returns eNotApplicable. This function can be
  overridden in custom classes.
  */

  virtual OdResult explodeToBlock( OdDgElement* pParentElement, OdDgElementIdArray *ids = 0);

  /** \details
  Explodes this entity into a set of simpler entities.

  \param entitySet [in/out] Receives an array of pointers to the new entities.

  \remarks
  The newly created entities will be not database resident.

  \returns 
  Returns eOk if successful, or an appropriate error code if not.

  \remarks
  Entities resulting from the explosion are appended to the specified array.

  The default implementation of this function calls subWorldDraw() and makes
  entities from geometry generated by subWorldDraw(). This function can be
  overridden in custom classes.
  */

  virtual OdResult explodeGeometry( OdRxObjectPtrArray& entitySet ) const;

  /** \details
  Explodes this entity into a set of simpler entities, and adds them to model or shared cell definition.

  \param pParentElement [in]  Pointer to OdDgModel, OdDgSheetModel, OdDgCellHeader, or OdDgSharedCellDefinition element.
  \param ids [in/out] Receives array of Element IDs of the new entities.

  \remarks
  The newly created entities are database resident.

  Entities resulting from the explosion are appended to the specified array.

  The default implementation of this function calls subWorldDraw() and makes
  entities from geometry generated by subWorldDraw(). This function can be
  overridden in custom classes.
  */

  virtual OdResult explodeGeometryToBlock( OdDgElement* pParentElement, OdDgElementIdArray *ids = 0);

 /** \details
    Copies the properties from the specified entity to this entity.
    \param pSource [in]  Pointer to the source entity.
    \param doSubents [in]  If and only if true, applies the change to all sub-entities owned by
                this entity.
 */
  void setPropertiesFrom(const OdDgElement* pEntFrom, bool doSubents = true );


protected:
  virtual OdResult subGetGeomExtents(const OdDgElementId& idView, OdGeExtents3d& extents) const;
  virtual OdResult subGetGeomExtents(OdGeExtents3d& extents) const;

  virtual OdResult subExplode( OdRxObjectPtrArray& entitySet ) const;

  virtual void subGripStatus( const OdDgElement::GripStat status );

  virtual OdResult subGetOsnapPoints( OdDgElement::OsnapMode osnapMode,
                                        OdGsMarker gsSelectionMark,
                                          const OdGePoint3d& pickPoint,
                                            const OdGePoint3d& lastPoint,
                                              const OdGeMatrix3d& xWorldToEye,
                                                OdGePoint3dArray& snapPoints ) const;

  virtual OdResult subGetOsnapPoints( OdDgElement::OsnapMode osnapMode,
                                        OdGsMarker gsSelectionMark,
                                          const OdGePoint3d& pickPoint,
                                            const OdGePoint3d& lastPoint,
                                              const OdGeMatrix3d& xWorldToEye,
                                                OdGePoint3dArray& snapPoints,
                                                  const OdGeMatrix3d& insertionMat) const;

  virtual bool subIsContentSnappable() const;

  virtual OdResult subGetGripPoints( OdGePoint3dArray& gripPoints ) const;
  virtual OdResult subMoveGripPointsAt(const OdIntArray& indices,const OdGeVector3d& offset );
  virtual OdResult subGetGripPoints( OdDgGripDataPtrArray& grips,
                                       const double curViewUnitSize,
                                         const int gripSize,
                                           const OdGeVector3d& curViewDir,
                                             const int bitFlags ) const;

  virtual OdResult subMoveGripPointsAt( const OdDgVoidPtrArray& grips,
                                          const OdGeVector3d& offset,
                                            int bitFlags );

  virtual OdResult subGetStretchPoints( OdGePoint3dArray& stretchPoints ) const;

  virtual OdResult subMoveStretchPointsAt( const OdIntArray& indices,
                                             const OdGeVector3d& offset );

  virtual bool subCloneMeForDragging();

  virtual bool subHideMeForDragging() const;

  virtual void subHighlight(bool bDoIt = true, const OdDgFullSubentPath* pSubId = 0, bool highlightAll = false) const;

  virtual OdDgElementPtr subDeepClone(OdDgIdMapping& ownerIdMap) const;
  virtual OdDgElementPtr subWblockClone(OdDgIdMapping& ownerIdMap) const;

  OdDgElementImpl* m_pImpl;
  friend class OdDgElementImpl;
  friend class OdDgNamedGroupHeader;

  friend class OdDgTransformOverrule;
  friend class OdDgOsnapOverrule;
  friend class OdDgGripOverrule;
  friend class OdDgHighlightOverrule;
  friend class OdDgGeometryOverrule;
  friend class OdDgElementOverrule;
};

/** \details

    <group OdDg_Classes>
*/

class TG_EXPORT OdDgLinkNodeElementPE : public OdRxObject
{
  ODRX_DECLARE_MEMBERS(OdDgLinkNodeElementPE);
  long        m_references;     //used by each object separately
public:

  OdDgLinkNodeElementPE();
  ~OdDgLinkNodeElementPE();

  //OdRxObject overridden
  virtual void addRef();
  virtual void release();
  virtual long numRefs() const;

  /** \details
    Returns number of link nodes, that assigned to the element.
  */

  virtual OdUInt32 getLinkNodeCount( const OdDgElement* obj );

  /** \details
    Returns link node pointer by index from array of link nodes, that assigned to the element.

    \param obj [in] Pointer to dgn element.
           uLinkNodeIndex [in] Index of link node into array.
           pLinkNode [out] Pointer to link node.

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult getLinkNode( const OdDgElement* obj, OdUInt32 uLinkNodeIndex, OdDgLinkNodePtr& pLinkNode);

  /** \details
    Set link node pointer by index to array of link nodes, that assigned to the element.

    \param obj [in] Pointer to dgn element.
           uLinkNodeIndex [in] Index of link node into array.
           pLinkNode [in] Pointer to link node.

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult setLinkNode( OdDgElement* obj, OdUInt32 uLinkNodeIndex, const OdDgLinkNodePtr& pLinkNode);

  /** \details
    Add link node pointer to the end of array of link nodes, that assigned to the element.

    \param obj [in] Pointer to dgn element.
           pLinkNode [in] Pointer to link node.

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult addLinkNode( OdDgElement* obj, OdDgLinkNodePtr& pLinkNode);

  /** \details
    Delete link node pointer by index from array of link nodes, that assigned to the element.

    \param obj [in] Pointer to dgn element.
           uLinkNodeIndex [in] Index of link node into array.

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult deleteLinkNode( OdDgElement* obj, OdUInt32 uLinkNodeIndex );
};

typedef OdSmartPtr<OdDgLinkNodeElementPE> OdDgLinkNodeElementPEPtr;

//====================================================================================================================================
//  Return element iterator for all complex elements except OdDgModel.

TG_EXPORT OdDgElementIteratorPtr createComplexElementIterator(const OdDgElement* obj, bool atBeginning = true, bool skipDeleted = true );

#include "TD_PackPop.h"

#endif // __DG_ELEMENT_H__
