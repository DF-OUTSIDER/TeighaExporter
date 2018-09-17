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

#ifndef _DbDynBlock_h_Included_
#define _DbDynBlock_h_Included_


#include "DbEvalGraph.h"
#include "TD_PackPush.h"

// Forward class declarations

class OdDbBlockReference;
class OdDbDynBlockReference;
class OdDbDynBlockReferenceProperty;
class OdDbDynBlockReferencePropertyImpl;
class OdDbDynBlockReferenceImpl;
struct OdDbBlkParamPropertyDescriptor;
class OdDbBlockParameter;
class OdDbDynBlockTableRecordImpl;
class OdDbBlockRepresentationContext;
typedef OdSmartPtr<OdDbBlockParameter> OdDbBlockParameterPtr;

/** \details
    This class is a utility class referencing properties of dynamic blocks.
    
    \remarks
    
    Corresponding C++ library: TD_DynBlocks
    <group OdDb_Classes>
*/
class DYNBLOCKS_EXPORT OdDbDynBlockReferenceProperty : public OdRxObject
{
public:

  enum UnitsType {
    kNoUnits  = 0,  // No units defined.
    kAngular  = 1,  // Angle.
    kDistance = 2,  // Distance.
    kArea     = 3   // 2D area.
  };

  OdDbDynBlockReferenceProperty ();
  virtual ~OdDbDynBlockReferenceProperty ();

  /** 
    \param other [in]  Constant reference to the dynamic block reference property.
  */
  OdDbDynBlockReferenceProperty (const OdDbDynBlockReferenceProperty& other);

  OdDbDynBlockReferenceProperty& operator=(
    const OdDbDynBlockReferenceProperty& other);

  /** \details
    Returns the Object ID of the dynamic block reference that owns this property.
  */
  OdDbObjectId blockId() const;

  /** \details
    Returns the name of this property.
    \remarks
    All property names in a given block reference are guaranteed unique.
  */
  OdString propertyName()  const;

  /** \details
    Returns the type of the property value.
  */
  OdResBuf::ValueType propertyType() const;

  /** \details
    Returns true if and only if this property is Read-Only.
  */
  bool readOnly() const;

  
  /** \details
    Returns true if and only if this property is displayed on the Property Palette.
  */
  bool show() const;

  /** \details
    Returns true if and only if this property is visible in the current visibility state.
  */
  bool visibleInCurrentVisibilityState() const;

  /** \details
    Returns the description of this property.
  */
  OdString description () const;

  /** \details
    Returns the units of this property.
    
    \remarks
    This function may be used to format property values.
    
    unitsType() returns one of the following:
    
    <table>
    Name        Value   Description
    kNoUnits    0       No units defined.
    kAngular    1       Angle.
    kDistance   2       Distance.
    kArea       3       2D area.
    </table>
  */
  UnitsType             unitsType () const;


  /** \details
    Returns an array of allowed values for this property.
    \param allowedValues [out]  Receives an an array of allowed values.
  */
  void getAllowedValues(OdResBufArray& allowedValues);

  /** \details
    Returns the current value of the property on the dynamic block reference.
  */
  OdResBufPtr value () const;

  /** \details
    Sets the current value of the property on the dynamic block reference.
    \param value [in]  Value.
    \returns
    Returns true if and only if successful.
  */
  bool setValue (const OdResBufPtr& value);

  /** \details
    Returns a parameter smart pointer.
  */
  OdDbBlockParameterPtr getParameter();
  
  /** \details
    Returns a parameter descriptor.
    
    \param parameterId [out]  Parameter ID.
    \param pDescriptor [out]  Parameter descriptor.     
  */
  void getDescriptor(OdDbEvalNodeId& parameterId, OdDbBlkParamPropertyDescriptor& pDescriptor);

private:
  friend class OdDbDynBlockReferencePropertyImpl;
  OdDbDynBlockReferencePropertyImpl* m_pImpl;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbDynBlockReferenceProperty object pointers.
*/
typedef OdSmartPtr<OdDbDynBlockReferenceProperty> OdDbDynBlockReferencePropertyPtr;
/** \details
    This template class is a specialization of the OdArray class for OdDbDynBlockReferencePropertyPtr objects.
*/
typedef OdArray<OdDbDynBlockReferencePropertyPtr> OdDbDynBlockReferencePropertyArray;

/** \details
    This class is a utility class providing access to information on dynamic block references. 
    Corresponding C++ library: TD_DynBlocks
    <group OdDb_Classes>
*/
class DYNBLOCKS_EXPORT OdDbDynBlockReference
{
  ODRX_HEAP_OPERATORS();
public:
  
  /** 
    \param blockRefId [in]  Object ID of the dynamic block reference.
  */
  OdDbDynBlockReference(const OdDbObjectId& blockRefId);

  /** 
    \param pRef [in]  Pointer to the dynamic block reference.
  */
  OdDbDynBlockReference(OdDbBlockReference* pRef);

  virtual ~OdDbDynBlockReference ();

  /** \details
    Returns true if and only if the specified Block Table Record 
    contains dynamic block information.
    
    \param blockTableRecordId [in]  Block Table Record Object ID.
    
    \remarks
    If blockTableRecordId is not specified, the Block Reference specified
    in the constructor will be used.
  */
  static bool isDynamicBlock (const OdDbObjectId& blockTableRecordId);
  
  /** \details
    Returns true if and only if the specified Block Table Record 
    contains dynamic block information.
    
    \remarks
    The Block Reference specified in the constructor will be used.
  */
  bool isDynamicBlock () const;

  /** \details
    Returns the Object ID of the block reference.
  */
  OdDbObjectId blockId() const;


  /** \details
    Returns an array of the dynamic block properties of the block reference.
    
    \param properties [out]  Receives an array of properties.
    
    \remarks
    An empty array will be returned if the block reference has no dynamic block properties.
  */
  void getBlockProperties(OdDbDynBlockReferencePropertyArray& properties) const;

  /** \details
    Resets all properties in the block reference to those of the block definition.
  */
  void resetBlock();

  /** \details
    Converts the dynamic block reference to a static block.
    
    \param newBlockName [in]  New block name.
    
    \remarks
    If newBlockName is not specified, an anonymous block is created.
  */
  bool convertToStaticBlock();
  bool convertToStaticBlock (const OdString& newBlockName);

  /** \details
    Returns the Object ID of the dynamic block BlockTableRecord.
    
    \returns
    Returns NULL if the block reference is not a dynamic block. 
  */
  OdDbObjectId dynamicBlockTableRecord () const;

  /** \details
    Returns the ObjectID of the anonymous representation block BlockTableRecord used to draw the dynamic block.
  */
  OdDbObjectId anonymousBlockTableRecord () const; 

  OdDbBlockRepresentationContext* getRepresentationContext() const;
private:
  OdDbDynBlockReference();
  OdDbDynBlockReferenceImpl* m_pImpl;
  friend class OdDbDynBlockTableRecord;
};

/** \details
  <group OdDb_Classes>
  Corresponding C++ library: TD_DynBlocks
*/
class DYNBLOCKS_EXPORT OdDbDynBlockTableRecord
{
  ODRX_HEAP_OPERATORS();
public:
  OdDbDynBlockTableRecord(OdDbObjectId blockId);
  ~OdDbDynBlockTableRecord();
  bool isDynamicBlock() const;
  OdDbObjectId blockTableRecordId() const;
  
  /** \details
    Gets an ObjectId array of all anonymous representation blocks created from the dynamic block definition.
    
    \param anonymousIds [out]  An output OdDbObjectIdArray.
  */
  void getAnonymousBlockIds(OdDbObjectIdArray& anonymousIds) const;
  void updateAnonymousBlocks() const;
private:
  OdDbDynBlockTableRecord();
  OdDbDynBlockTableRecordImpl* m_pImpl;
};

#include "TD_PackPop.h"

#endif
