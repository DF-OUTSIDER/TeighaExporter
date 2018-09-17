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

#ifndef OD_MODELDOCVIEWSTYLE_H
#define OD_MODELDOCVIEWSTYLE_H

#include "TD_PackPush.h"
#include "DbObject.h"

/** \details

    Corresponding C++ library: TD_Db
    <group OdDb_Classes> 
*/
class ODRX_ABSTRACT DBENT_EXPORT OdDbModelDocViewStyle : public OdDbObject
{
protected:
  // Default constructor is protected since OdDbModelDocViewStyle is an abstract class.
  OdDbModelDocViewStyle();

public:
  ODDB_DECLARE_MEMBERS(OdDbModelDocViewStyle);

  /** \details
    Defines the attachment point for the model documentation view label text. 
  */
  enum AttachmentPoint {
    kAboveView    = 0,  // Label text is attached above the view.   
    kBelowView    = 1,  // Label text is attached below the view.
  };

  /** \details
    Defines the alignment for the model documentation view label text. 
  */
  enum TextAlignment {
    kTextAlignmentLeft   = 0, // Left alignment is used for the view label text.
    kTextAlignmentCenter = 1, // Center alignment is used for the view label text.
    kTextAlignmentRight  = 2, // Right alignment is used for the view label text.
  };

  // Default destructor.
  virtual ~OdDbModelDocViewStyle() = 0;

  /** \details
    Returns the name of this OdDbModelDocViewStyle object.
  */
  virtual OdString getName() const;

  /** \details
    Sets the name of this OdDbModelDocViewStyle object.

    \param name [in]  Name.
  */
  virtual void setName(const OdString& name);

  /** \details
    Returns the description of this view style.
  */
  virtual OdString description() const;

  /** \details
    Sets the description of this view style.
    
    \param description [in]  Description.
  */
  virtual void setDescription(const OdString& description);

  /** \details
    Sets defaults values for the properties of this view style.
  */
  virtual void setDatabaseDefaults(OdDbDatabase* pDb);

  /** \details
    Includes this view style object in a OdDbDatabase object.
    
    \param pDb [in]  Pointer of the database to receive the OdDbModelDocViewStyle object.
    \param styleName [in]  Name of the view style.
    
    \returns
    Returns the OdDbObjectID of the newly added OdDbModelDocViewStyle object.
  */
  virtual OdDbObjectId postViewStyleToDb(OdDbDatabase* pDb, 
                                         const OdString& styleName) = 0;

  /** \details
    Specifies whether properties were changed. 
	
	\returns
    True if properties were changed; returns false otherwise.
  */
  bool isModifiedForRecompute() const;

  /** \details
    Returns the default view name based on the specified index.

    \param index [in]  Index of default name.
  */
  OdString defaultViewName(int index) const;

  /** \details
    Checks whether a label name is valid.

    \param name [in]  Label name to check.

    \returns
    True if the specified name is a valid string.
  */
  bool isViewNameValid(const OdString& name) const;

  ///** \details
  //  Calculates a view label's position.
  //
  //  \param offsetVector [out] Offset (vector) of the view label.
  //  \param textAlignment [out] Text alignment of the view label.
  //  \param attachment [in] Use with the offset argument to specify label position.
  //  \param offset [in] Use with the attachment argument to specify label position. 
  //  \param minPt [in] Point located in the lower left corner of the view's bounding box. 
  //  \param maxPt [in] Point located in the upper right corner of the view's bounding box. 
  //
  //  \returns
  //  Position of the view label.
  //*/
  //static OdGePoint3d calculateViewLabelPosition(OdGeVector2d& offsetVector,
  //                                              OdDbMText::AttachmentPoint& textAlignment,
  //                                              OdDbModelDocViewStyle::AttachmentPoint attachment,
  //                                              double offset,
  //                                              const OdGePoint3d& minPt,
  //                                              const OdGePoint3d& maxPt);

  ///** \details
  //  Sets the position of a view label.
  //  
  //  \param pMText [in] Pointer to MText object to be positioned as a label.
  //  \param attachment [in] Use with the offset argument to specify label position. 
  //  \param offset [in] Use with the attachment argument to specify label position.
  //  \param minPt [in] Point located in the lower left corner of the view's bounding box.
  //  \param maxPt [in] Point located in the upper right corner of the view's bounding box. 
  //  
  //  \returns
  //  Position of the view label.
  //*/
  //static OdGeVector2d setViewLabelPosition(OdDbMText* pMText,
  //                                         OdDbModelDocViewStyle::AttachmentPoint attachment,
  //                                         double offset,
  //                                         const OdGePoint3d& minPt,
  //                                         const OdGePoint3d& maxPt);

  ///** \details
  //  Updates the position of a view label.
  //
  //  \param pMText [in] Pointer to MText object to be positioned as a label.
  //  \param attachment [in] Specifies whether the label is attached. 
  //  \param minPt [in] Point located in the lower left corner of the view's bounding box.
  //  \param maxPt [in] Point located in the upper right corner of the view's bounding box. 
  //
  //  \returns
  //  Position of the view label.
  //*/
  //static OdGeVector2d updateViewLabelPosition(OdDbMText* pMText,
  //                                            OdDbModelDocViewStyle::AttachmentPoint attachment,
  //                                            const OdGePoint3d& minPt,
  //                                            const OdGePoint3d& maxPt);

  OdString getDisplayName() const;
  OdResult SetDispayName( const OdString& name );
  bool canRename() const;
  OdResult setCanRename(bool canRename);

protected:
  // OdDbObject overridden functions
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

  enum
  {
    kCanRename = 1
  };

//protected:
//  // For internal use only
//  virtual OdDbObjectPtr subDeepClone(OdDbObject* pOwner,
//                                     OdDbIdMapping& idMap, bool isPrimary = true) const;
//  virtual OdDbObjectPtr subWblockClone(OdDbObject* pOwner,
//                                       OdDbIdMapping& idMap, bool isPrimary = true) const;
//private:
//    friend class OdDbModelDocViewStyleImp;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbModelDocViewStyle object pointers.
*/
typedef OdSmartPtr<OdDbModelDocViewStyle> OdDbModelDocViewStylePtr;

#include "TD_PackPop.h"

#endif // OD_MODELDOCVIEWSTYLE_H
