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

#ifndef _ODGILINETYPESEG_INCLUDED_
#define _ODGILINETYPESEG_INCLUDED_


#include "TD_PackPush.h"

#include "OdString.h"
#include "OdArray.h"
#include <math.h>


class OdDbStub;


/** \details

    <group OdGi_Classes> 
*/
class OdGiLinetypeDash
{
public:
  ODRX_HEAP_OPERATORS();

  struct offset
  {
  double            x;
	double            y;
  };
  double            length;
	double            shapeScale;
	double            shapeRotation;
  OdUInt16          flags;
  OdUInt16          shapeNumber;
	offset            shapeOffset;
	OdString          textString;
	OdDbStub*         styleId;

  OdGiLinetypeDash()
  {
    shapeScale = 1.0;
    shapeRotation = shapeScale = length = 0.0;
    flags = shapeNumber = 0;
    shapeOffset.x = shapeOffset.y = 0.0;                                    // MKU 10.03.2004   CR 1690
    styleId = NULL;
  }

  bool isRotationAbsolute() const { return GETBIT(flags, 1); }
	void setRotationAbsolute(bool value = true) { SETBIT(flags, 1, value); }

  bool isRotationUpright() const { return GETBIT(flags, 8); }
	void setRotationUpright(bool value = true) { SETBIT(flags, 8, value); }

  bool isEmbeddedTextString() const { return GETBIT(flags, 2); }
	void setEmbeddedTextString(bool value = true) { SETBIT(flags, 2, value); }

  bool isEmbeddedShape() const { return GETBIT(flags, 4); }
	void setEmbeddedShape(bool value = true) { SETBIT(flags, 4, value); }
};

typedef OdArray<OdGiLinetypeDash> OdGiLinetypeDashArray;
/** \details

    <group OdGi_Classes> 
*/
class OdGiLinetype
{
	enum
	{
		kByBlock            = 1,
		kByLayer            = 2,
		kPatternLengthValid = 4
	};

  mutable OdUInt32          m_flags;
  mutable double            m_patternLength;
  OdGiLinetypeDashArray     m_dashes;
  friend class OdDbLinetypeTableRecord;
  friend class OdDbLinetypeTableRecordImpl;
public:
  OdGiLinetype();

  bool isContinuous() const;

  bool isByBlock() const;
	void setByBlock(bool value);

  bool isByLayer() const;
	void setByLayer(bool value);

  /** \details
      Returns the overall length of this linetype pattern.
  */
  double patternLength() const;

  void setPatternLength(double patternLength);

  /** \details
      Returns the number of dashes of this linetype.
  */
  int numDashes() const;

  /** \details
      Sets the number of dashes for this linetype.
  */
  void setNumDashes(int count);

  void dashAt(int index, OdGiLinetypeDash& dash) const;
  const OdGiLinetypeDash& dashAt(int index) const;

  void setDashAt(int index, double value);
  void setDashAt(int index, const OdGiLinetypeDash& dash);

  void dashes(OdGiLinetypeDashArray& dashes) const;
  void setDashes(const OdGiLinetypeDashArray& dashes);
};


inline bool OdGiLinetype::isByBlock() const     { return GETBIT(m_flags, kByBlock); }
inline void OdGiLinetype::setByBlock(bool value){ SETBIT(m_flags, kByBlock, value); }

inline bool OdGiLinetype::isByLayer() const     { return GETBIT(m_flags, kByLayer); }
inline void OdGiLinetype::setByLayer(bool value){ SETBIT(m_flags, kByLayer, value); }

inline bool OdGiLinetype::isContinuous() const  { return m_dashes.isEmpty(); }

inline void OdGiLinetype::dashes(OdGiLinetypeDashArray& dashes) const { dashes = m_dashes; }
inline void OdGiLinetype::setDashes(const OdGiLinetypeDashArray& dashes)
{
  m_dashes = dashes;
  SETBIT(m_flags, kPatternLengthValid, false);
}

inline OdGiLinetype::OdGiLinetype()
  : m_flags(kPatternLengthValid)
  , m_patternLength(0.)
{
}

inline double OdGiLinetype::patternLength() const
{
  if(!GETBIT(m_flags, kPatternLengthValid))
  {
    int n = m_dashes.size();
    m_patternLength = 0.0;
    while(n--)
    {
      m_patternLength += fabs(m_dashes[n].length);
    }
    SETBIT(m_flags, kPatternLengthValid, true);
  }
  return m_patternLength;
}

inline void OdGiLinetype::setPatternLength(double patternLength)
{
  m_patternLength = patternLength;
  if ( OdZero(patternLength) )
    SETBIT(m_flags, kPatternLengthValid, false);
  else
    SETBIT(m_flags, kPatternLengthValid, true);
}

inline int OdGiLinetype::numDashes() const
{
  return m_dashes.size();
}

inline void OdGiLinetype::setNumDashes(int count)
{
  SETBIT(m_flags, kPatternLengthValid, false);
  m_dashes.setLogicalLength(count);
}

inline void OdGiLinetype::dashAt(int index, OdGiLinetypeDash& dash) const
{
  dash = m_dashes[index];
}

inline const OdGiLinetypeDash& OdGiLinetype::dashAt(int index) const
{
  return m_dashes[index];
}

inline void OdGiLinetype::setDashAt(int index, const OdGiLinetypeDash& dash)
{
  SETBIT(m_flags, kPatternLengthValid, false);
  m_dashes[index] = dash;
}

inline void OdGiLinetype::setDashAt(int index, double dashLength)
{
  SETBIT(m_flags, kPatternLengthValid, false);
  m_dashes[index] = OdGiLinetypeDash();
  m_dashes[index].length = dashLength;
}

#include "TD_PackPop.h"

#endif //_ODGILINETYPESEG_INCLUDED_
