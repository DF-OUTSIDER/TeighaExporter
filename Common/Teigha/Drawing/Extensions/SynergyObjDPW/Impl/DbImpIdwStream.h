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

#ifndef DbImpIdwStream_INCLUDED_
#define DbImpIdwStream_INCLUDED_ /*!DOM*/

#include "DbIdwStream.h"
#include "OdBinaryData.h"

#include "TD_PackPush.h"

/** \details
  This class implements the IDW stream.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class SYNERGYOBJDPW_EXPORT OdDbImpIdwStream
{
public:     
  /** \details
    Constructor creates an instance of this class.
  */
  OdDbImpIdwStream();

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdDbImpIdwStream();

  /** \details
    Reads the .dwg file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId);

  /** \details
    Writes the .dwg file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const;

  /** \details
    Reads the .dxf file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId);

  /** \details
    Writes the .dxf file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const;

  //bool setData(long,unsigned char const *);
  //bool getData(long &,unsigned char * &) const;
protected:
  //static OdUInt32 kCurrentVersionNumber;
  //static class std::map<OdDbDatabase *, OdDbObjectId> mVersionCache;

  /*  {secret}  */
  friend class OdDbIdwStream;
  /*  {secret}  */
  OdBinaryData m_data;
};

#include "TD_PackPop.h"

#endif // DbImpIdwStream_INCLUDED_
