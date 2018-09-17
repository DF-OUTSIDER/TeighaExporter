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

#ifndef _OdTtfDescriptor_h_Included_
#define _OdTtfDescriptor_h_Included_

#include "TD_PackPush.h"

#include "Gi/Gi.h"
#include "OdString.h"
#include "OdFont.h"

enum TextRenderingMode
{
  kTextRenderingBasedA = 0,
  kTextRenderingBasedM = 1
};

/** \details
    This class implements TrueType Descriptor objects which provide an interface to
    the characteristics of a TrueType font.
    
    <group Other_Classes>
*/
class FIRSTDLL_EXPORT OdTtfDescriptor
{
  // Members

	OdUInt32                    m_nFlags;
  OdString                    m_sFontFile;
	OdString                    m_Typeface;
  TextRenderingMode           m_renderingMode;

public:
  // Constructor

  OdTtfDescriptor() : m_nFlags(0), m_renderingMode(kTextRenderingBasedA)
  {
  }

  /** \param typeface [in]  Typeface.
    \param bold [in]  True if and only if a bold font.
    \param italic [in]  True if and only if an italic font.
    \param charset [in]  Character set.
    \param pitchAndFamily [in]  Pitch and Family.
    
    \remarks
    The two low-order bits of pitchAndFamily specify the pitch of the font.
    Bits 4 through 7 of pitchAndFamily specify the font family.

  */
  OdTtfDescriptor(const OdString& typeface, bool bold, bool italic, int charset, int pitchAndFamily)
    : m_Typeface(typeface), m_nFlags(0), m_renderingMode(kTextRenderingBasedA)
  {
    setTtfFlags(bold, italic, charset, pitchAndFamily);
  }

  // Accessors /transformers

  /** \details
    Returns the filename of this TtfDescriptor object.
  */
  const OdString &fileName() const     { return m_sFontFile; }

  /** \details
    Returns the typeface of this TtfDescriptor object.
  */
  const OdString &typeface() const     { return m_Typeface;  }

  /** \details
    Returns the flags of this TtfDescriptor object. 
  */
  OdUInt32 getTtfFlags() const  { return m_nFlags;    }

  /** \details
    Clears the filename of this TtfDescriptor object.
  */
  void clearFileName()
  { 
    m_sFontFile.empty();  
  }
  /** \details
    Clears the typeface of this TtfDescriptor object.
  */
  void clearTypeface()
  { 
    m_Typeface.empty();  
  }
  /** \details
    Adds the specified typeface to this TtfDescriptor object.
  */
  void addTypeface(OdChar typeface)
  { 
    m_Typeface += typeface;
  }

  /** \details
    Returns the flags of this TtfDescriptor object. 
    \param bold [out]  Receives true if and only if this is a bold font.
    \param italic [out]  Receives true if and only if this is an italic font.
    \param charset [out]  Receives the character set.
    \param pitchAndFamily [out]  Receives the Pitch and Family.
    
    \remarks
    The two low-order bits of pitchAndFamily specify the pitch of the font.
    Bits 4 through 7 of pitchAndFamily specify the font family.
  */
	void getTtfFlags(bool& bold, bool& italic, int& charset, int& pitchAndFamily) const
  {
	  bold = isBold();
	  italic = isItalic();
	  charset = charSet();
	  pitchAndFamily = this->pitchAndFamily();
  }

  /** \details
    Sets the filename of this TtfDescriptor object.
    \param filename [in]  Filename.
  */
  void setFileName(const OdString& filename)  { m_sFontFile = filename; }
  /** \details
    Sets the typeface of this TtfDescriptor object.
    \param typeface [in]  Typeface.
  */
  void setTypeFace(const OdString& typeface)  { m_Typeface = typeface;  }
  /** \details
    Sets the flags of this TtfDescriptor object.
    \param flags [in]  Flags.
  */
  void setTtfFlags(OdUInt32 flags)           { m_nFlags = flags;      }
  /** \details
    Sets the flags of this TtfDescriptor object.
	
    \param bold [in]  True if and only if a bold font.
    \param italic [in]  True if and only if an italic font.
    \param charset [in]  Character set.
    \param pitchAndFamily [in]  Pitch and Family.
    
    \remarks
    The two low-order bits of pitchAndFamily specify the pitch of the font.
    Bits 4 through 7 of pitchAndFamily specify the font family.
  */
  void setTtfFlags(bool bold, bool italic, int charset, int pitchAndFamily)
  {
    setBold(bold);
    setItalic(italic);
    setCharSet(charset);
    setPitchAndFamily(pitchAndFamily);
  }

  /** \details
    Sets the bold flag of this TtfDescriptor object.
    \param bold [in]  True if and only if a bold font.
  */
   void setBold(bool bold)          { SETBIT(m_nFlags, 0x02000000, bold);  }
  /** \details
    Sets the italic flag of this TtfDescriptor object.
    \param italic [in]  True if and only if an italic font.
  */
   void setItalic(bool italic)        { SETBIT(m_nFlags, 0x01000000, italic);  }
  /** \details
    Sets the character set of this TtfDescriptor object.
    \param charset [in]  Character set.
  */
   void setCharSet(int charset)        { m_nFlags = ((m_nFlags & 0xFFFF00FF) | ((charset << 8) & 0x0000FF00)); }
  /** \details
    Sets the pitch and family of this TtfDescriptor object.
    \param pitchAndFamily [in]  Pitch and Family.
    
    \remarks
    The two low-order bits of pitchAndFamily specify the pitch of the font.
    Bits 4 through 7 of pitchAndFamily specify the font family.
  */
  void setPitchAndFamily(int pitchAndFamily) { m_nFlags = ((m_nFlags & 0xFFFFFF00) | (pitchAndFamily & 0x000000FF));  }

  /** \details
    Returns true if and only if this TtfDescriptor is a bold font.
  */
  bool isBold() const             { return GETBIT(m_nFlags, 0x02000000);  }
  /** \details
    Returns true if and only if this TtfDescriptor is an italic font.
  */
  bool isItalic() const           { return GETBIT(m_nFlags, 0x01000000);  }
  /** \details
    Returns the character set for this TtfDescriptor object.
  */
  OdUInt16 charSet() const        { return OdUInt16((m_nFlags & 0x0000ff00) >> 8); }
  /** \details
    Returns the pitch and family of this TtfDescriptor object.
    
    \remarks
    The two low-order bits of pitchAndFamily specify the pitch of the font.
    Bits 4 through 7 of pitchAndFamily specify the font family.
  */
  int pitchAndFamily() const      { return (m_nFlags & 0x000000ff); }

  /** \details
    Returns the pitch of this TtfDescriptor object.
  */
  int getPitch() const            { return (pitchAndFamily() & 0x00000003); }
  /** \details
    Returns the family of this TtfDescriptor object.
  */
  int getFamily() const           { return (pitchAndFamily() & 0x000000f0); }

  /** \details
    Returns text rendering mode (BasedA/BasedM) of this TtfDescriptor object.
  */
  TextRenderingMode textRenderingMode() const { return m_renderingMode; }

  /** \details
    Sets text rendering mode (BasedA/BasedM) of this TtfDescriptor object.
  */
  void setTextRenderingMode(TextRenderingMode renderingMode) { m_renderingMode = renderingMode; }

};

#include "TD_PackPop.h"

#endif // _OdTtfDescriptor_h_Included_
