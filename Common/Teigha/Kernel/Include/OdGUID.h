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

#ifndef ODGUID_H_INCLUDED
#define ODGUID_H_INCLUDED

class OdString;

/** \details
    <group Other_Classes>
*/
class FIRSTDLL_EXPORT OdGUID {
public:
  typedef OdUInt8 DataType[0x10];
  typedef OdUInt8 Data4Type[0x8];
  struct StringFormat {
    enum Enum {
      Digits,
      Hyphenses,
      Braces,
      Parentheses,
      Extended
    };
  };

  FIRSTDLL_EXPORT_STATIC static const OdGUID kNull;

  OdGUID();
  OdGUID(const OdUInt32, const OdUInt16, const OdUInt16, const OdUInt8,
         const OdUInt8, const OdUInt8, const OdUInt8, const OdUInt8,
         const OdUInt8, const OdUInt8, const OdUInt8);
  OdGUID(const OdInt32, const OdInt16, const OdInt16, const Data4Type&);
  explicit OdGUID(const DataType&);
  explicit OdGUID(const OdString&);

  OdGUID& operator=(const OdGUID&);

  bool operator<(const OdGUID&) const;
  bool operator>(const OdGUID&) const;
  bool operator==(const OdGUID&) const;
  bool operator!=(const OdGUID&) const;
  bool operator<=(const OdGUID&) const;
  bool operator>=(const OdGUID&) const;

  /** \details
    Returns string representation for this OdGUID.

    \param format [in]  A single format specifier that indicates how to format the value of this OdGUID.
                        Default value is OdGUID::StringFormat::Hyphenses.

    \remarks
    The following table shows the accepted format specifiers for the format parameter.
    "0" represents a digit; hyphens ("-"), braces ("{", "}"), and parentheses ("(", ")") appear as shown.

    OdGUID::StringFormat::Digits:      00000000000000000000000000000000
    OdGUID::StringFormat::Hyphenses:   00000000-0000-0000-0000-000000000000
    OdGUID::StringFormat::Braces:      {00000000-0000-0000-0000-000000000000}
    OdGUID::StringFormat::Parentheses: (00000000-0000-0000-0000-000000000000)
    OdGUID::StringFormat::Extended:    {0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}}
  */
  OdString toString(
      const StringFormat::Enum format = StringFormat::Hyphenses) const;

  const DataType& data() const;
private:
  void init(const DataType&);
  void init(
      const OdUInt32 p0, const OdUInt16 p1, const OdUInt16 p2, const OdUInt8 p3,
      const OdUInt8 p4, const OdUInt8 p5, const OdUInt8 p6, const OdUInt8 p7,
      const OdUInt8 p8, const OdUInt8 p9, const OdUInt8 p10);

  OdUInt8 m_data[0x10];
};

#endif // ODGUID_H_INCLUDED
