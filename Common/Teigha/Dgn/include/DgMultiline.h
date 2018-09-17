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
#ifndef __DG_MULTI_LINE_H__
#define __DG_MULTI_LINE_H__

#include "TD_PackPush.h"

#include "DgGraphicsElement.h"
#include "DgMultilineStyleTableRecord.h"
#include "Ge/GePoint3d.h"
#include "OdArray.h"


//breaks in multiline
//they are connected to points, but not to the hosting multiline

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgMultilineBreak
{
public:

  enum Flags
  {
    kStandardByDistance  = 0x0000,
    kFromJoint           = 0x8000,
    kToJoint             = 0x4000
  };

  OdDgMultilineBreak();

  //the mask determines interrupted lines in the multiline
  OdUInt32 getLinesMask() const;
  void setLinesMask( OdUInt32 mask );

  //an offset from the base point (the base point is determined by flags)
  double getOffset() const;
  void setOffset( double offset );

  //a length of this break
  double getLength() const;
  void setLength( double length );

  //base point
  Flags getFlags() const;
  void setFlags( Flags value );

private:
  Flags    m_flags;             //flags
  OdUInt32 m_nLinesMask;        //a mask of the lines broken by this break
  double   m_dOffset;           //offset from the base point (the base point is determined by flags)
  double   m_dLength;           //a length of this break
};


//these points determine a curve built by multiline

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgMultilinePoint
{
public:

  OdDgMultilinePoint();

  //the point
  void getPoint( OdGePoint3d& point ) const;
  void setPoint( const OdGePoint3d& point );

  //breaks over the segment from this point to the next one
  OdUInt32 getBreaksCount() const;
  void getBreak( OdUInt32 index, OdDgMultilineBreak& break_ ) const;
  void addBreak( const OdDgMultilineBreak& break_ );
  void removeAllBreaks();

private:

  OdGePoint3d                   m_point;    //a point
  OdArray< OdDgMultilineBreak > m_breaks;    //breaks over the segment from this point to the next one

};


/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgMultiline : public OdDgGraphicsElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL( OdDgMultiline );
public:

  //
  // angles of the caps
  //

  double getOriginCapAngle() const;
  void setOriginCapAngle( double angle );

  double getEndCapAngle() const;
  void setEndCapAngle( double angle );

  //
  // the caps
  //

  void getOriginCap( OdDgMultilineSymbology& symbology ) const;
  void setOriginCap( const OdDgMultilineSymbology& symbology );

  void getMiddleCap( OdDgMultilineSymbology& symbology ) const;
  void setMiddleCap( const OdDgMultilineSymbology& symbology );

  void getEndCap( OdDgMultilineSymbology& symbology ) const;
  void setEndCap( const OdDgMultilineSymbology& symbology );

  //Z vector(determines the plane where the multiline lies)
  OdGeVector3d getZVector() const;
  void setZVector( const OdGeVector3d& zVector );

  //the only used flag
  bool getClosedFlag() const;
  void setClosedFlag( bool value );

  //profiles, which determine all parallel lines inside multiline
  OdUInt32 getProfilesCount() const;
  void getProfile( int index, OdDgMultilineProfile& profile ) const;
  void addProfile( const OdDgMultilineProfile& profile );
  void removeAllProfiles();

  //points, which determine this multiline as a curve
  OdUInt32 getPointsCount() const;
  void getPoint( int index, OdDgMultilinePoint& point ) const;
  void addPoint( const OdDgMultilinePoint& point );
  void removeAllPoints();

  //
  // the offset: mode, flags
  //

  enum OffsetMode
  {
    kByWorkLine = 0,
    kByCenter,
    kByMaximum,
    kByMinimum
  };

  bool getOffsetIsValidFlag() const;
  void setOffsetIsValidVlag( bool value );

  OffsetMode getOffsetMode() const;
  void setOffsetMode( OffsetMode value );

  // Multiline style

  OdUInt64 getMultilineStyleId() const;
  void     setMultilineStyleId( OdUInt64 uId );
  void     applyMultilineStyle( const OdDgElementId& idMultilineStyle );

  // OdGiDrawable overridden
  bool subWorldDraw( OdGiWorldDraw* pWd ) const;
  virtual OdResult subExplode( OdRxObjectPtrArray& entitySet ) const;
};

typedef OdSmartPtr< OdDgMultiline > OdDgMultilinePtr;


#include "TD_PackPop.h"

#endif // __DG_MULTI_LINE_H__
