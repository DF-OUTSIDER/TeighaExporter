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
#ifndef __DG_TAGSETDEFINITION_H__
#define __DG_TAGSETDEFINITION_H__


#include "TD_PackPush.h"

#include "DgElement.h"
#include "DgElementIterator.h"
#include "OdBinaryData.h"
#include "DgItemTypeLibrary.h"


/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgTagDefinition : public OdRxObject
{
  ODRX_DECLARE_MEMBERS(OdDgTagDefinition);

public:
  enum Type
  {
    kChar    = 1,
    kInt16   = 2,
    kInt32   = 3,
    kDouble  = 4,
    kBinary  = 5
  };

  virtual Type getType() const = 0;
  virtual OdUInt16 getEntryId() const = 0;
  virtual OdString getName() const = 0;
  virtual void setName( const OdString& name ) = 0;

  virtual OdString getPrompt() const = 0;
  virtual void setPrompt( const OdString& prompt ) = 0;

  virtual OdString getString() const = 0;
  virtual void setString( const OdString& string ) = 0;

  virtual OdInt16 getInt16() const = 0;
  virtual void setInt16( OdInt16 value ) = 0;

  virtual OdInt32 getInt32() const = 0;
  virtual void setInt32( OdInt32 value ) = 0;

  virtual double getDouble() const = 0;
  virtual void setDouble( double value ) = 0;

  virtual OdUInt16 getBinarySize() const = 0;
  virtual void getBinaryData( OdBinaryData& data ) const = 0;
  virtual void setBinaryData( const OdBinaryData& data ) = 0;

  virtual bool isVariable() const = 0;
};

typedef OdSmartPtr<OdDgTagDefinition> OdDgTagDefinitionPtr;

/** \details

Corresponding C++ library: TG_Db 

<group OdDg_Classes>
*/
class TG_EXPORT OdDgTagDefinitionDgnStore : public OdRxObject
{
  ODRX_DECLARE_MEMBERS(OdDgTagDefinitionDgnStore);

public:
  virtual OdUInt16 getBinarySize() const = 0;
  virtual void getBinaryData( OdBinaryData& data ) const = 0;
  virtual void setBinaryData( const OdBinaryData& data ) = 0;
};

typedef OdSmartPtr<OdDgTagDefinitionDgnStore> OdDgTagDefinitionDgnStorePtr;

/** \details

Corresponding C++ library: TG_Db 

<group OdDg_Classes>
*/
class TG_EXPORT OdDgTagDefinitionXml : public OdRxObject
{
  ODRX_DECLARE_MEMBERS(OdDgTagDefinitionXml);

public:
  virtual OdString getXmlString() const = 0;
  virtual void     setXmlString( const OdString& strXml ) = 0;

  virtual OdBinaryData getExtendedData() const = 0;
  virtual void     setExtendedData( const OdBinaryData& binData ) = 0;
};

typedef OdSmartPtr<OdDgTagDefinitionXml> OdDgTagDefinitionXmlPtr;


/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgTagDefinitionSet : public OdDgElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgTagDefinitionSet);

public:
  OdString getName() const;
  void setName(const OdString& name);

  OdDgTagDefinitionPtr addDefinition(
    OdDgTagDefinition::Type type,
    const OdString& sName );
  OdDgTagDefinitionDgnStorePtr addDgnStoreDefinition(
    const OdBinaryData &data );
  bool removeDefinition( OdRxObjectPtr def );

  OdUInt32 getCount() const;
  OdRxObjectPtr getByIndex( OdUInt32 index ) const;
  OdDgTagDefinitionPtr getByType( OdDgTagDefinition::Type type ) const;
  OdDgTagDefinitionPtr getByEntryId( OdUInt16 entryId ) const;
  OdDgTagDefinitionPtr getByName( const OdString& name ) const;

  // If we can't identify tag definition set data, we read them as proxy binary data
  // and set proxy tag definition set flag.

  virtual bool getProxyTagDefinitionSetFlag() const;
  virtual void setProxyTagDefinitionSetFlag( bool bSet );

  //
  // The listed below methods are applied only if a proxy tag definition set flag is turned on.
  //
  virtual OdUInt16 getBinarySize() const;
  virtual void getBinaryData( OdBinaryData& data ) const;
  virtual void setBinaryData( const OdBinaryData& data );
  virtual OdDgElementIteratorPtr createIterator( bool atBeginning = true, bool skipDeleted = true ) const;
  virtual void add( OdDgElementPtr pChildElement );
};

typedef OdSmartPtr<OdDgTagDefinitionSet> OdDgTagDefinitionSetPtr;

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgTagDefinitionSetTable : public OdDgElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgTagDefinitionSetTable);
public:
  /** \details
  Adds the specified tag set definition to this collection.

  \param pModel [in]  Pointer to the object to add.
  */
  void add(OdDgElementPtr pTagDefinition);

  /** \details
  Returns an Iterator object that can be used to traverse this collection object.

  \param atBeginning [in]  True to start at the beginning, false to start at the end. 
  \param skipDeleted [in]  If and only if true, deleted objects are skipped.
  */
  virtual OdDgElementIteratorPtr createIterator( bool atBeginning = true, bool skipDeleted = true ) const;

  virtual OdDgElementId getAt( const OdString& name, bool getErasedRecord = false) const;

  virtual bool has( const OdDgElementId& elementId) const;
};

typedef OdSmartPtr<OdDgTagDefinitionSetTable> OdDgTagDefinitionSetTablePtr;

/** \details

Corresponding C++ library: TG_Db 

<group OdDg_Classes>
*/
class TG_EXPORT OdDgDgnStoreHeader : public OdDgElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgDgnStoreHeader);

public:
  virtual OdUInt16 getBinarySize() const;
  virtual void getBinaryData( OdBinaryData& data ) const;
  virtual void setBinaryData( const OdBinaryData& data );
};

typedef OdSmartPtr<OdDgDgnStoreHeader> OdDgDgnStoreHeaderPtr;


#include "TD_PackPop.h"

#endif // __DG_TAGSETDEFINITION_H__
