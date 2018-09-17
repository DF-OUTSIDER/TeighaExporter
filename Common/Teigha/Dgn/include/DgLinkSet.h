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
#ifndef __DG_LINKSET_H__
#define __DG_LINKSET_H__

#include "DgApplicationData.h"
#include "DgElementIterator.h"

class OdDgLinkNode;
typedef OdSharedPtr<OdDgLinkNode> OdDgLinkNodePtr;

/** \details

  Corresponding C++ library: TG_Db
  <group OdDg_Classes>
*/
class TG_EXPORT OdDgLinkNode
{
  public:

    enum OdDgLinkNodeType
    {
      kUnknownLink     = 0,
      kFolderLink      = 1,
      kFileLink        = 2,
      kModelLink       = 3,
      kRegionLink      = 4,
      kPDFRegionLink   = 5,
      kExcelSheetLink  = 6,
      kWordHeadingLink = 7,
      kWordRegionLink  = 8,
      kURLLink         = 9
    };

  public:

    OdDgLinkNode()
    {}

    virtual ~OdDgLinkNode()
    {}

    virtual OdDgLinkNodeType getType() const = 0;
    virtual bool             getValidFlag() const = 0;
    virtual OdString         getName() const = 0;
    virtual void             setName( const OdString& strName ) = 0;

    virtual bool             clone( OdDgLinkNodePtr& pRet )const = 0;
    virtual bool             isEqual( const OdDgLinkNodePtr& obj ) const = 0;
};

//---------------------------------------------------------------------------------------------

/** \details

  Corresponding C++ library: TG_Db
  <group OdDg_Classes>
*/
class TG_EXPORT OdDgUnknownLinkNode : public OdDgLinkNode
{
  public:

    OdDgUnknownLinkNode();

    virtual OdDgLinkNodeType getType() const;
    virtual OdString         getName() const;
    virtual void             setName( const OdString& strName );

    OdString                 getHandlerName() const;
    void                     setHandlerName( const OdString& strHandlerName );
    OdString                 getHandler() const;
    void                     setHandler( const OdString& strHandler );
    bool                     getValidFlag() const;
    void                     setValidFlag( bool bSet );

    virtual bool             clone( OdDgLinkNodePtr& pRet ) const;
    virtual bool             isEqual( const OdDgLinkNodePtr& pObj ) const;

  private:

    OdString m_strName;
    OdString m_strHandlerName;
    OdString m_strHandler;
    bool     m_bValidFlag;
};

typedef OdSharedPtr<OdDgUnknownLinkNode> OdDgUnknownLinkNodePtr;

//---------------------------------------------------------------------------------------------

/** \details

  Corresponding C++ library: TG_Db
  <group OdDg_Classes>
*/
class TG_EXPORT OdDgFolderLinkNode : public OdDgLinkNode
{
public:

  OdDgFolderLinkNode();

  virtual OdDgLinkNodeType getType() const;
  virtual OdString         getName() const;
  virtual void             setName( const OdString& strName );
  bool                     getValidFlag() const;

  OdUInt32                 getChildCount() const;
  OdDgLinkNodePtr          getChild( OdUInt32 uIndex );
  void                     addChild( const OdDgLinkNodePtr pChild );
  void                     deleteChild( OdUInt32 uIndex );

  virtual bool             clone( OdDgLinkNodePtr& pRet ) const;
  virtual bool             isEqual( const OdDgLinkNodePtr& pObj ) const;

private:

  OdString                   m_strName;
  OdArray< OdDgLinkNodePtr > m_childPtrArr;
};

typedef OdSharedPtr<OdDgFolderLinkNode> OdDgFolderLinkNodePtr;

//---------------------------------------------------------------------------------------------

/** \details

  Corresponding C++ library: TG_Db
  <group OdDg_Classes>
*/
class TG_EXPORT OdDgFileLinkNode : public OdDgLinkNode
{
public:

  OdDgFileLinkNode();

  virtual OdDgLinkNodeType getType() const;
  virtual OdString         getName() const;
  virtual void             setName( const OdString& strName );

  virtual OdString         getFileName() const;
  virtual void             setFileName( const OdString& strFileName );
  virtual OdString         getFullPath() const;
  virtual void             setFullPath( const OdString& strPath );
  bool                     getValidFlag() const;
  void                     setValidFlag( bool bSet );

  virtual bool             clone( OdDgLinkNodePtr& pRet ) const;
  virtual bool             isEqual( const OdDgLinkNodePtr& pObj ) const;

protected:

  OdString                 m_strName;
  OdString                 m_strFileName;
  OdString                 m_strFullPath;
  bool                     m_bValidFlag;

};

typedef OdSharedPtr<OdDgFileLinkNode> OdDgFileLinkNodePtr;

//---------------------------------------------------------------------------------------------

/** \details

  Corresponding C++ library: TG_Db
  <group OdDg_Classes>
*/
class TG_EXPORT OdDgModelLinkNode : public OdDgFileLinkNode
{
public:

  enum OdDgModelLinkNodeType
  {
    kUnknownModel = 0,
    kDesignModel  = 1,
    kSheetModel   = 2
  };
public:

  OdDgModelLinkNode();

  virtual OdDgLinkNodeType getType() const;

  OdDgModelLinkNodeType getModelType() const;
  void                  setModelType( OdDgModelLinkNodeType uType );
  OdString              getModelTypeName() const;
  void                  setModelTypeName( const OdString& strModelTypeName );
  OdString              getModelName() const;
  void                  setModelName( const OdString& strModelName );

  virtual bool          clone( OdDgLinkNodePtr& pRet ) const;
  virtual bool          isEqual( const OdDgLinkNodePtr& pObj ) const;

private:

  OdString                 m_strModelName;
  OdString                 m_strModelTypeName;
  OdDgModelLinkNodeType    m_uModelType;

};

typedef OdSharedPtr<OdDgModelLinkNode> OdDgModelLinkNodePtr;

//---------------------------------------------------------------------------------------------

/** \details

  Corresponding C++ library: TG_Db
  <group OdDg_Classes>
*/
class TG_EXPORT OdDgRegionLinkNode : public OdDgFileLinkNode
{
public:

  enum OdDgRegionLinkNodeType
  {
    kUnknownRegion = 0,
    kReference     = 1,
    kSavedView     = 2
  };
public:

  OdDgRegionLinkNode();

  virtual OdDgLinkNodeType getType() const;

  OdDgRegionLinkNodeType getRegionType() const;
  void                   setRegionType( OdDgRegionLinkNodeType uType );
  OdString               getRegionTypeName() const;
  void                   setRegionTypeName( const OdString& strRegionTypeName );
  OdString               getModelName() const;
  void                   setModelName( const OdString& strModelName );
  OdString               getRegionName() const;
  void                   setRegionName( const OdString& strRegionName );

  virtual bool           clone( OdDgLinkNodePtr& pRet ) const;
  virtual bool           isEqual( const OdDgLinkNodePtr& pObj ) const;

private:

  OdString                 m_strModelName;
  OdString                 m_strRegionName;
  OdString                 m_strRegionTypeName;
  OdDgRegionLinkNodeType   m_uRegionType;

};

typedef OdSharedPtr<OdDgRegionLinkNode> OdDgRegionLinkNodePtr;

//---------------------------------------------------------------------------------------------

/** \details

  Corresponding C++ library: TG_Db
  <group OdDg_Classes>
*/
class TG_EXPORT OdDgPdfRegionInfoItem
{
  public:

    enum OdDgPdfRegionType
    {
      kUnknownRegion = 0,
      kBookmark      = 1,
      kHeading       = 2
    };

  public:

    OdDgPdfRegionInfoItem();

    OdDgPdfRegionType getRegionType() const;
    void              setRegionType( OdDgPdfRegionType uType );
    OdString          getRegionTypeName() const;
    void              setRegionTypeName( const OdString& strRegionTypeName );
    OdString          getRegionName() const;
    void              setRegionName( const OdString& strRegionName );
    OdUInt32          getSequenceNumber() const;
    void              setSequenceNumber( OdUInt32 uNumber );

  private:

    OdDgPdfRegionType m_uType;
    OdString          m_strRegionName;
    OdString          m_strRegionTypeName;
    OdUInt32          m_uSequenceNumber;
};

/** \details

  Corresponding C++ library: TG_Db
  <group OdDg_Classes>
*/
class TG_EXPORT OdDgPdfRegionLinkNode : public OdDgFileLinkNode
{

public:

  OdDgPdfRegionLinkNode();

  virtual OdDgLinkNodeType getType() const;

  OdUInt32              getPdfInfoItemCount() const;
  OdDgPdfRegionInfoItem getPdfInfoItem( OdUInt32 uIndex ) const;
  void                  setPdfInfoItem( OdUInt32 uIndex, const OdDgPdfRegionInfoItem& item );
  void                  addPdfInfoItem( const OdDgPdfRegionInfoItem& item );
  void                  deletePdfInfoItem( OdUInt32 uIndex );

  virtual bool          clone( OdDgLinkNodePtr& pRet ) const;
  virtual bool          isEqual( const OdDgLinkNodePtr& pObj ) const;

private:

  OdArray<OdDgPdfRegionInfoItem> m_itemArr;

};

typedef OdSharedPtr<OdDgPdfRegionLinkNode> OdDgPdfRegionLinkNodePtr;
//---------------------------------------------------------------------------------------------

/** \details

  Corresponding C++ library: TG_Db
  <group OdDg_Classes>
*/
class TG_EXPORT OdDgExcelSheetLinkNode : public OdDgFileLinkNode
{
public:

  OdDgExcelSheetLinkNode();

  virtual OdDgLinkNodeType getType() const;

  OdString               getSheetName() const;
  void                   setSheetName( const OdString& strSheetName );

  virtual bool          clone( OdDgLinkNodePtr& pRet ) const;
  virtual bool          isEqual( const OdDgLinkNodePtr& pObj ) const;

private:

  OdString                 m_strSheetName;

};

typedef OdSharedPtr<OdDgExcelSheetLinkNode> OdDgExcelSheetLinkNodePtr;

//---------------------------------------------------------------------------------------------

/** \details

  Corresponding C++ library: TG_Db
  <group OdDg_Classes>
*/
class TG_EXPORT OdDgWordHeadingInfoItem
{
public:

  enum OdDgWordHeadingItemType
  {
    kUnknown       = 0,
    kBookmark      = 1,
    kHeading       = 2
  };

public:

  OdDgWordHeadingInfoItem();

  OdDgWordHeadingItemType getType() const;
  void              setType( OdDgWordHeadingItemType uType );
  OdString          getTypeName() const;
  void              setTypeName( const OdString& strTypeName );
  OdString          getName() const;
  void              setName( const OdString& strName );
  OdUInt32          getSequenceNumber() const;
  void              setSequenceNumber( OdUInt32 uNumber );
  OdUInt32          getWordHeadingStyle() const;
  void              setWordHeadingStyle( OdUInt32 uStyle );

private:

  OdDgWordHeadingItemType m_uType;
  OdString                m_strName;
  OdString                m_strTypeName;
  OdUInt32                m_uSequenceNumber;
  OdUInt32                m_uStyle;
};

/** \details

  Corresponding C++ library: TG_Db
  <group OdDg_Classes>
*/
class TG_EXPORT OdDgWordHeadingLinkNode : public OdDgFileLinkNode
{

public:

  OdDgWordHeadingLinkNode();

  virtual OdDgLinkNodeType getType() const;

  OdUInt32                getInfoItemCount() const;
  OdDgWordHeadingInfoItem getInfoItem( OdUInt32 uIndex ) const;
  void                    setInfoItem( OdUInt32 uIndex, const OdDgWordHeadingInfoItem& item );
  void                    addInfoItem( const OdDgWordHeadingInfoItem& item );
  void                    deleteInfoItem( OdUInt32 uIndex );

  virtual bool            clone( OdDgLinkNodePtr& pRet ) const;
  virtual bool            isEqual( const OdDgLinkNodePtr& pObj ) const;

private:

  OdArray<OdDgWordHeadingInfoItem> m_itemArr;

};

typedef OdSharedPtr<OdDgWordHeadingLinkNode> OdDgWordHeadingLinkNodePtr;

//---------------------------------------------------------------------------------------------

/** \details

  Corresponding C++ library: TG_Db
  <group OdDg_Classes>
*/
class TG_EXPORT OdDgWordRegionLinkNode : public OdDgFileLinkNode
{

public:

  OdDgWordRegionLinkNode();

  virtual OdDgLinkNodeType getType() const;

  OdDgWordHeadingInfoItem::OdDgWordHeadingItemType getRegionType() const;
  void                   setRegionType( OdDgWordHeadingInfoItem::OdDgWordHeadingItemType uType );
  OdString               getRegionTypeName() const;
  void                   setRegionTypeName( const OdString& strRegionTypeName );
  OdString               getRegionName() const;
  void                   setRegionName( const OdString& strRegionName );

  virtual bool           clone( OdDgLinkNodePtr& pRet ) const;
  virtual bool           isEqual( const OdDgLinkNodePtr& pObj ) const;

private:

  OdString                 m_strRegionName;
  OdString                 m_strRegionTypeName;
  OdDgWordHeadingInfoItem::OdDgWordHeadingItemType   m_uRegionType;

};

typedef OdSharedPtr<OdDgWordRegionLinkNode> OdDgWordRegionLinkNodePtr;

//---------------------------------------------------------------------------------------------

/** \details

  Corresponding C++ library: TG_Db
  <group OdDg_Classes>
*/
class TG_EXPORT OdDgURLLinkNode : public OdDgLinkNode
{
public:

  OdDgURLLinkNode();

  virtual OdDgLinkNodeType getType() const;
  virtual OdString         getName() const;
  virtual void             setName( const OdString& strName );

  virtual OdString         getURL() const;
  virtual void             setURL( const OdString& strURL );
  bool                     getValidFlag() const;
  void                     setValidFlag( bool bSet );

  virtual bool             clone( OdDgLinkNodePtr& pRet ) const;
  virtual bool             isEqual( const OdDgLinkNodePtr& pObj ) const;

protected:

  OdString                 m_strName;
  OdString                 m_strURL;
  bool                     m_bValidFlag;

};

typedef OdSharedPtr<OdDgURLLinkNode> OdDgURLLinkNodePtr;

//--------------------------------------------------------------

class OdDgLinkSet;
typedef OdSmartPtr<OdDgLinkSet> OdDgLinkSetPtr;

//--------------------------------------------------------------

/** \details

  Corresponding C++ library: TG_Db
  <group OdDg_Classes>
*/
class TG_EXPORT OdDgLinkSet : public OdDgApplicationData
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgLinkSet);

public:

  enum
  {
    kSignature = 0x5842
  };

  OdString getName() const;
  void setName( OdString& strName );

  OdUInt32 getLinkNodeCount() const;
  OdDgLinkNodePtr getLinkNode( OdUInt32 uIndex );
  void     addLinkNode( const OdDgLinkNodePtr& pLinkNode );
  void     deleteLinkNode( OdUInt32 uIndex );
};

/** \details

  Corresponding C++ library: TG_Db
  <group OdDg_Classes>
*/
class TG_EXPORT OdDgLinkSetTable : public OdDgElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgLinkSetTable);
public:

  void add( OdDgElementPtr pLinkSet);

  OdDgElementIteratorPtr createIterator( bool atBeginning = true, bool skipDeleted = true ) const;

  OdDgElementId getAt( const OdString& strLinkSetName );
};

typedef OdSmartPtr<OdDgLinkSetTable> OdDgLinkSetTablePtr;

#endif // __DG_LINKSET_H__
