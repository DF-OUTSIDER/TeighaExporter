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
#ifndef __DG_MLINESTYLETABLERECORD_H__
#define __DG_MLINESTYLETABLERECORD_H__


#include "TD_PackPush.h"

#include "DgTableRecord.h"

class OdDgFiler;

//symbology of lines & caps

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgMultilineSymbology
{
public:

  OdDgMultilineSymbology();

  OdUInt32 getLineStyleEntryId() const;
  void setLineStyleEntryId( OdUInt32 style );

  OdUInt32 getLineWeight() const;
  void setLineWeight( OdUInt32 weight );

  OdUInt32 getColorIndex() const;
  void setColorIndex( OdUInt32 color );

  OdUInt32 getLevelEntryId() const;
  void setLevelEntryId( OdUInt32 level );


  bool getUseStyleFlag() const;
  void setUseStyleFlag( bool value );

  bool getUseWeightFlag() const;
  void setUseWeightFlag( bool value );

  bool getUseColorFlag() const;
  void setUseColorFlag( bool value );

  bool getUseClassFlag() const;
  void setUseClassFlag( bool value );

  bool getCapInArcFlag() const;
  void setCapInArcFlag( bool value );

  bool getCapOutArcFlag() const;
  void setCapOutArcFlag( bool value );

  bool getCapLineFlag() const;
  void setCapLineFlag( bool value );

  bool getCustomStyleFlag() const;
  void setCustomStyleFlag( bool value );

  bool getCapColorFromSegmentFlag() const;
  void setCapColorFromSegmentFlag( bool value );

  bool getConstructionClassFlag() const;
  void setConstructionClassFlag( bool value );

  void dgnInFields( OdDgFiler* pFiler );
  void dgnOutFields( OdDgFiler* pFiler ) const;

private:

  // the flags determine an appearance of the symbology
  enum Flags
  {
    kUseStyle             = 0x00010000,
    kUseWeight            = 0x00020000,
    kUseColor             = 0x00040000,
    kCapInArc             = 0x00080000,
    kCapOutArc            = 0x00100000,
    kCapLine              = 0x00200000,
    kUseClass             = 0x00400000,
    kCustomStyle          = 0x00800000,
    kCapColorFromSegment  = 0x01000000,
    kConstructionClass    = 0x80000000
  };
  OdUInt32 m_flags;

  OdUInt32 m_style;
  OdUInt32 m_weight;
  OdUInt32 m_color;
  OdUInt32 m_level;
};


//profile describing a line inside of the multiline

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgMultilineProfile
{
public:

  OdDgMultilineProfile();

  //distance from the work line
  double getDistance() const;
  void setDistance( double distance );

  //the symbology, that the profile can override common style with
  OdDgMultilineSymbology& symbology();
  const OdDgMultilineSymbology& getSymbology() const;
  void getSymbology( OdDgMultilineSymbology& symbology ) const;
  void setSymbology( const OdDgMultilineSymbology& symbology );


  void dgnInFields( OdDgFiler* pFiler );
  void dgnOutFields( OdDgFiler* pFiler ) const;

private:

  double                  m_distance;     //an offset from the work line
  OdDgMultilineSymbology  m_symbology;    //an appearance

};


/** \details
    This class represents MultilineStyle records in the OdDgMultilineStyleTable in an OdDgDatabase instance.

    Corresponding C++ library: TG_Db
    
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgMultilineStyleTableRecord : public OdDgTableRecord
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgMultilineStyleTableRecord);
public:
  bool getUseFillColorFlag() const;
  void setUseFillColorFlag( bool );

  OdUInt32 getFillColorIndex() const;
  void setFillColorIndex( OdUInt32 );

  // profiles, which determine all parallel lines inside multiline
  OdUInt32 getProfilesCount() const;
  void getProfile( OdUInt32 index, OdDgMultilineProfile& profile ) const;
  void addProfile( const OdDgMultilineProfile& profile );
  void removeAllProfiles();

  // angles of the caps
  double getOriginCapAngle() const;
  void setOriginCapAngle( double angle );

  double getEndCapAngle() const;
  void setEndCapAngle( double angle );

  // origin/end/middle caps properties
  void getOriginCap( OdDgMultilineSymbology& symbology ) const;
  void setOriginCap( const OdDgMultilineSymbology& symbology );

  void getEndCap( OdDgMultilineSymbology& symbology ) const;
  void setEndCap( const OdDgMultilineSymbology& symbology );

  void getMiddleCap( OdDgMultilineSymbology& symbology ) const;
  void setMiddleCap( const OdDgMultilineSymbology& symbology );

  OdUInt32 getEntryId() const;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgMultilineStyleTableRecord object pointers.
*/
typedef OdSmartPtr<OdDgMultilineStyleTableRecord> OdDgMultilineStyleTableRecordPtr;


#include "TD_PackPop.h"

#endif // __DG_MLINESTYLETABLERECORD_H__
