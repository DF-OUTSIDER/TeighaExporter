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

#ifndef __TDWF_DATABASE_H__
#define __TDWF_DATABASE_H__

#include "DwfObject.h" //#include "Gi/GiDrawable.h"
#include "RxSystemServices.h"
//#include "DbBaseDatabase.h"
#include "DwfdbExport.h"

class OdDwfHostAppServices;
class DwfDatabaseImpl;

class DWFDBEXPORT OdDwfDatabase : public OdDwfObject
{
  ODRX_DECLARE_MEMBERS(OdDwfDatabase);
  OdDwfDatabase();
public:
  ~OdDwfDatabase();

  /** \details
    Returns the OdDwfHostAppServices object associated with this database object.
  */
  virtual OdDwfHostAppServices* appServices() const;

  /** \details
    Adds the default set of objects and settings to this database object.
  */
  //void initialize(
  //  bool defaultModelIs3d = true);

  /** \details
    Reads the contents of the specified StreamBuf object or file into this database object.

    \param pStreamBuf [in]  Pointer to the StreamBuf object from which the data are to be read.
    \param partialLoad [in]  Controls the partial loading of .dwg files.
    \param password [in]  Password for file.


    Throws:
    OdError if the read is unsuccessful.
  */
  virtual void readFile(OdStreamBuf* pStreamBuf,
                        bool partialLoad = true,
                        const OdString& password = OdString::kEmpty);

  /** \details
    Reads the contents of the specified file into this database object.

    \param fileName [in]  Name of the file from which the data are to be read.
    \param partialLoad [in]  Controls the partial loading of DGN files.
    \param password [in]  Password for file.

    Throws:
    OdError if the write is unsuccessful.
  */
  virtual void readFile(const OdString& fileName,
                        bool partialLoad =  true,
                        const OdString& password = OdString::kEmpty);

  /** \details
    Writes the contents of this database object to the specified file.

    \param filename [in]  Name of the file to which the data are to be written.

    Throws:
    OdError if the write is unsuccessful.
  */
  //void writeFile(
  //  OdString fileName);


  /** \details
      Returns the name of the file associated with this database object.
  */
  virtual OdString getFilename() const;

  //OdUInt32 getMajorVersion() const;
  //OdUInt32 getMinorVersion() const;
  //double getLastSaveTime() const;

  // OdDwfObject methods:
  virtual OdDwfObjectId objectId() const;
  virtual OdDbStub* ownerId() const;
  virtual void addReactor(OdDwfObjectReactor* pReactor);
  virtual void removeReactor(OdDwfObjectReactor* pReactor);
  // OdGiDrawable methods:
  virtual OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;
  virtual bool subWorldDraw(OdGiWorldDraw* pWd) const;
  virtual void subViewportDraw(OdGiViewportDraw* pVd) const;
  virtual bool isPersistent() const;
  virtual OdDbStub* id() const;
  virtual void setGsNode(OdGsCache* pGsNode);
  virtual OdGsCache* gsNode() const;

  // OdRxObject overridden
  //virtual void addRef();
  //virtual void release();
  //virtual long numRefs() const;

  virtual class OdUnitsFormatter& formatter();
  OdDwfObjectIdArray layoutIds();
  OdDwfObjectId currentLayoutId() const;

  OdDwfObjectId addObject(OdDwfObjectPtr pObj, 
                          const OdDbHandle& handle = OdDbHandle());
  OdDwfObjectId getObjectId(const OdDbHandle& handle, 
                            bool createIfNotFound = false);
private:
  friend class DwfDatabaseImpl;
  OdSmartPtr<DwfDatabaseImpl> m_pImpl;
};

typedef OdSmartPtr<OdDwfDatabase> OdDwfDatabasePtr;

#endif //__TDWF_DATABASE_H__
