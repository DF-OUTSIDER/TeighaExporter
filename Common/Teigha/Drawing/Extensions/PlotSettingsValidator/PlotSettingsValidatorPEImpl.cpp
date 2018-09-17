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



#include "OdaCommon.h"
#include "PlotSettingsValidatorPEImpl.h"
#include "UInt8Array.h"
#include "OdString.h"

#ifdef OD_HAVE_WINSPOOL_FILE
#include <winspool.h>
#endif

#if defined(ODA_WINDOWS)
#include <commdlg.h>
#include <tchar.h>
#endif

#define STL_USING_MEMORY
#define STL_USING_ALGORITHM
#include "OdaSTL.h"

#if defined(OD_HAVE_GLOB_FUNC)
#include <glob.h>
#endif 
//ODRX_NO_CONS_DEFINE_MEMBERS(OdPlotSettingsValidatorPEImpl, OdRxObject)

extern OdArray<OdDbPlotSettingsValidatorPE::psvPaperInfo> *customMediaList;

namespace PE_PAPER
{
struct predefinedInfo
{
  OdDbPlotSettings::PlotPaperUnits _units;
  const OdChar* _CanonicalName;
};

predefinedInfo predefinedPaperInfo[] =
{
  OdDbPlotSettings::kInches,      /* 0 element */ OD_T(""),
  OdDbPlotSettings::kInches,      /* DMPAPER_LETTER               1  */ OD_T("Letter") ,
  OdDbPlotSettings::kInches,      /* DMPAPER_LETTERSMALL          2  */ OD_T("Letter Small") ,
  OdDbPlotSettings::kInches,      /* DMPAPER_TABLOID              3  */ OD_T("Tabloid") ,
  OdDbPlotSettings::kInches,      /* DMPAPER_LEDGER               4  */ OD_T("Ledger") ,
  OdDbPlotSettings::kInches,      /* DMPAPER_LEGAL                5  */ OD_T("Legal") ,
  OdDbPlotSettings::kInches,      /* DMPAPER_STATEMENT            6  */ OD_T("Statement") ,
  OdDbPlotSettings::kInches,      /* DMPAPER_EXECUTIVE            7  */ OD_T("Executive") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_A3                   8  */ OD_T("A3") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_A4                   9  */ OD_T("A4") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_A4SMALL             10  */ OD_T("A4 Small") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_A5                  11  */ OD_T("A5") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_B4                  12  */ OD_T("B4 (JIS)") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_B5                  13  */ OD_T("B5 (JIS)") ,
  OdDbPlotSettings::kInches,      /* DMPAPER_FOLIO               14  */ OD_T("Folio") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_QUARTO              15  */ OD_T("Quarto") ,
  OdDbPlotSettings::kInches,      /* DMPAPER_10X14               16  */ OD_T("10x14") ,
  OdDbPlotSettings::kInches,      /* DMPAPER_11X17               17  */ OD_T("11x17") ,
  OdDbPlotSettings::kInches,      /* DMPAPER_NOTE                18  */ OD_T("Note") ,
  OdDbPlotSettings::kInches,      /* DMPAPER_ENV_9               19  */ OD_T("Envelope #9") ,
  OdDbPlotSettings::kInches,      /* DMPAPER_ENV_10              20  */ OD_T("Envelope #10") ,
  OdDbPlotSettings::kInches,      /* DMPAPER_ENV_11              21  */ OD_T("Envelope #11") ,
  OdDbPlotSettings::kInches,      /* DMPAPER_ENV_12              22  */ OD_T("Envelope #12") ,
  OdDbPlotSettings::kInches,      /* DMPAPER_ENV_14              23  */ OD_T("Envelope #14") ,
  OdDbPlotSettings::kInches,      /* DMPAPER_CSHEET              24  */ OD_T("C size sheet") ,
  OdDbPlotSettings::kInches,      /* DMPAPER_DSHEET              25  */ OD_T("D size sheet") ,
  OdDbPlotSettings::kInches,      /* DMPAPER_ESHEET              26  */ OD_T("E size sheet") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_ENV_DL              27  */ OD_T("Envelope DL") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_ENV_C5              28  */ OD_T("Envelope C5") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_ENV_C3              29  */ OD_T("Envelope C3") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_ENV_C4              30  */ OD_T("Envelope C4") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_ENV_C6              31  */ OD_T("Envelope C6") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_ENV_C65             32  */ OD_T("Envelope C65") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_ENV_B4              33  */ OD_T("Envelope B4") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_ENV_B5              34  */ OD_T("Envelope B5") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_ENV_B6              35  */ OD_T("Envelope B6") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_ENV_ITALY           36  */ OD_T("Envelope") ,
  OdDbPlotSettings::kInches,      /* DMPAPER_ENV_MONARCH         37  */ OD_T("Envelope Monarch") ,
  OdDbPlotSettings::kInches,      /* DMPAPER_ENV_PERSONAL        38  */ OD_T("6 3/4 Envelope") ,
  OdDbPlotSettings::kInches,      /* DMPAPER_FANFOLD_US          39  */ OD_T("US Std Fanfold") ,
  OdDbPlotSettings::kInches,      /* DMPAPER_FANFOLD_STD_GERMAN  40  */ OD_T("German Std Fanfold") ,
  OdDbPlotSettings::kInches,      /* DMPAPER_FANFOLD_LGL_GERMAN  41  */ OD_T("German Legal Fanfold") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_ISO_B4              42  */ OD_T("B4 (ISO)") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_JAPANESE_POSTCARD   43  */ OD_T("Japanese Postcard") ,
  OdDbPlotSettings::kInches,      /* DMPAPER_9X11                44  */ OD_T("9x11") ,
  OdDbPlotSettings::kInches,      /* DMPAPER_10X11               45  */ OD_T("10x11") ,
  OdDbPlotSettings::kInches,      /* DMPAPER_15X11               46  */ OD_T("15x11") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_ENV_INVITE          47  */ OD_T("Envelope Invite") ,
  OdDbPlotSettings::kInches,      /* 48 element */ OD_T("Reserved48") ,
  OdDbPlotSettings::kInches,      /* 49 element */ OD_T("Reserved49") ,
  OdDbPlotSettings::kInches,      /* DMPAPER_LETTER_EXTRA        50  */ OD_T("Letter Extra") ,
  OdDbPlotSettings::kInches,      /* DMPAPER_LEGAL_EXTRA         51  */ OD_T("Legal Extra") ,
  OdDbPlotSettings::kInches,      /* DMPAPER_TABLOID_EXTRA       52  */ OD_T("Tabloid Extra") ,
  OdDbPlotSettings::kInches,      /* DMPAPER_A4_EXTRA            53  */ OD_T("A4 Extra") ,
  OdDbPlotSettings::kInches,      /* DMPAPER_LETTER_TRANSVERSE   54  */ OD_T("Letter Transverse") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_A4_TRANSVERSE       55  */ OD_T("A4 Transverse") ,
  OdDbPlotSettings::kInches,      /* DMPAPER_LETTER_EXTRA_TRANSVERSE 56 */ OD_T("Letter Extra Transverse") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_A_PLUS              57  */ OD_T("Super A") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_B_PLUS              58  */ OD_T("Super B") ,
  OdDbPlotSettings::kInches,      /* DMPAPER_LETTER_PLUS         59  */ OD_T("Letter Plus") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_A4_PLUS             60  */ OD_T("A4 Plus") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_A5_TRANSVERSE       61  */ OD_T("A5 Transverse") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_B5_TRANSVERSE       62  */ OD_T("B5 (JIS) Transverse") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_A3_EXTRA            63  */ OD_T("A3 Extra") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_A5_EXTRA            64  */ OD_T("A5 Extra") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_B5_EXTRA            65  */ OD_T("B5 (ISO) Extra") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_A2                  66  */ OD_T("A2") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_A3_TRANSVERSE       67  */ OD_T("A3 Transverse") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_A3_EXTRA_TRANSVERSE 68  */ OD_T("A3 Extra Transverse") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_DBL_JAPANESE_POSTCARD 69 */ OD_T("Japanese Double Postcard") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_A6                  70  */ OD_T("A6") ,
  OdDbPlotSettings::kInches,      /* DMPAPER_JENV_KAKU2          71  */ OD_T("Japanese Envelope Kaku #2") ,
  OdDbPlotSettings::kInches,      /* DMPAPER_JENV_KAKU3          72  */ OD_T("Japanese Envelope Kaku #3") ,
  OdDbPlotSettings::kInches,      /* DMPAPER_JENV_CHOU3          73  */ OD_T("Japanese Envelope Chou #3") ,
  OdDbPlotSettings::kInches,      /* DMPAPER_JENV_CHOU4          74  */ OD_T("Japanese Envelope Chou #4") ,
  OdDbPlotSettings::kInches,      /* DMPAPER_LETTER_ROTATED      75  */ OD_T("Letter Rotated") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_A3_ROTATED          76  */ OD_T("A3 Rotated") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_A4_ROTATED          77  */ OD_T("A4 Rotated") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_A5_ROTATED          78  */ OD_T("A5 Rotated") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_B4_JIS_ROTATED      79  */ OD_T("B4 (JIS) Rotated") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_B5_JIS_ROTATED      80  */ OD_T("B5 (JIS) Rotated") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_JAPANESE_POSTCARD_ROTATED 81 */ OD_T("Japanese Postcard Rotated") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_DBL_JAPANESE_POSTCARD_ROTATED 82 */ OD_T("Double Japan Postcard Rotated") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_A6_ROTATED          83  */ OD_T("A6 Rotated") ,
  OdDbPlotSettings::kInches,      /* DMPAPER_JENV_KAKU2_ROTATED  84  */ OD_T("Japan Envelope Kaku #2 Rotated") , // ????
  OdDbPlotSettings::kInches,      /* DMPAPER_JENV_KAKU3_ROTATED  85  */ OD_T("Japan Envelope Kaku #3 Rotated") , // ????
  OdDbPlotSettings::kInches,      /* DMPAPER_JENV_CHOU3_ROTATED  86  */ OD_T("Japan Envelope Chou #3 Rotated") , // ????
  OdDbPlotSettings::kInches,      /* DMPAPER_JENV_CHOU4_ROTATED  87  */ OD_T("Japan Envelope Chou #4 Rotated") , // ????
  OdDbPlotSettings::kMillimeters, /* DMPAPER_B6_JIS              88  */ OD_T("B6 (JIS)") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_B6_JIS_ROTATED      89  */ OD_T("B6 (JIS) Rotated") ,
  OdDbPlotSettings::kInches,      /* DMPAPER_12X11               90  */ OD_T("12x11") ,
  OdDbPlotSettings::kInches,      /* DMPAPER_JENV_YOU4           91  */ OD_T("Japan Envelope You #4") , // ????
  OdDbPlotSettings::kInches,      /* DMPAPER_JENV_YOU4_ROTATED   92  */ OD_T("Japan Envelope You #4 Rotated") , // ????
  OdDbPlotSettings::kMillimeters, /* DMPAPER_P16K                93  */ OD_T("PRC 16K") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_P32K                94  */ OD_T("PRC 32K") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_P32KBIG             95  */ OD_T("PRC 32K(Big)") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_PENV_1              96  */ OD_T("PRC Envelope #1") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_PENV_2              97  */ OD_T("PRC Envelope #2") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_PENV_3              98  */ OD_T("PRC Envelope #3") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_PENV_4              99  */ OD_T("PRC Envelope #4") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_PENV_5              100 */ OD_T("PRC Envelope #5") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_PENV_6              101 */ OD_T("PRC Envelope #6") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_PENV_7              102 */ OD_T("PRC Envelope #7") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_PENV_8              103 */ OD_T("PRC Envelope #8") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_PENV_9              104 */ OD_T("PRC Envelope #9") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_PENV_10             105 */ OD_T("PRC Envelope #10") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_P16K_ROTATED        106 */ OD_T("PRC 16K Rotated") , // ????
  OdDbPlotSettings::kMillimeters, /* DMPAPER_P32K_ROTATED        107 */ OD_T("PRC 32K Rotated") , // ????
  OdDbPlotSettings::kMillimeters, /* DMPAPER_P32KBIG_ROTATED     108 */ OD_T("PRC 32K(Big) Rotated") , // ????
  OdDbPlotSettings::kMillimeters, /* DMPAPER_PENV_1_ROTATED      109 */ OD_T("PRC Envelope #1 Rotated") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_PENV_2_ROTATED      110 */ OD_T("PRC Envelope #2 Rotated") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_PENV_3_ROTATED      111 */ OD_T("PRC Envelope #3 Rotated") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_PENV_4_ROTATED      112 */ OD_T("PRC Envelope #4 Rotated") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_PENV_5_ROTATED      113 */ OD_T("PRC Envelope #5 Rotated") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_PENV_6_ROTATED      114 */ OD_T("PRC Envelope #6 Rotated") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_PENV_7_ROTATED      115 */ OD_T("PRC Envelope #7 Rotated") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_PENV_8_ROTATED      116 */ OD_T("PRC Envelope #8 Rotated") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_PENV_9_ROTATED      117 */ OD_T("PRC Envelope #9 Rotated") ,
  OdDbPlotSettings::kMillimeters, /* DMPAPER_PENV_10_ROTATED     118 */ OD_T("PRC Envelope #10 Rotated")
};

enum kPaperType
{
  eDeviceDefined = 0,
  eUserDefined = 1
};

const OdUInt32 predefinedPaperInfoMax = sizeof(predefinedPaperInfo) / sizeof(predefinedPaperInfo[0]);

OdString getCanonicalNameForDevice(OdUInt16 paperId, PE_PAPER::kPaperType mediaType, OdString localeName)
{
  if (mediaType == PE_PAPER::eDeviceDefined && paperId < PE_PAPER::predefinedPaperInfoMax)
  {
    //ODA_ASSERT(!localeName.compare( PE_PAPER::predefinedPaperInfo[paperId]._CanonicalName ));
    return PE_PAPER::predefinedPaperInfo[paperId]._CanonicalName;
  }
  
  return (mediaType == PE_PAPER::eUserDefined) ? OdString().format(OD_T("USER%d"), paperId) : localeName;
}

OdDbPlotSettings::PlotPaperUnits getUnitsForDevice(OdUInt16 paperId)
{
  return (paperId >= PE_PAPER::predefinedPaperInfoMax) ? OdDbPlotSettings::kMillimeters : PE_PAPER::predefinedPaperInfo[paperId]._units;
}

};


static OdResult getSystemDeviceList(OdArray<OdString> &pDeviceList)
{
#if defined(OD_WINDOWS_DESKTOP)
  // add system installed printer devices
  DWORD pcbNeeded(0), pcReturned(0);
  DWORD dwFlags( PRINTER_ENUM_CONNECTIONS|PRINTER_ENUM_LOCAL);
  BOOL bRes = EnumPrinters( dwFlags, NULL, 4, NULL, 0, &pcbNeeded, &pcReturned );
  //if (!bRes) // This can be simulated by disabling "Print Spooler" service.
  //  return eExtendedError;

  // member reported that new BYTE[pcbNeeded] with pcbNeeded=0 may throw an exception
  if (pcbNeeded == 0)
    return eOk;

  OdUInt8Array pPrinterEnum;
  pPrinterEnum.resize(pcbNeeded, 0);

  bRes = EnumPrinters( dwFlags, NULL, 4, pPrinterEnum.asArrayPtr(), pcbNeeded, &pcbNeeded, &pcReturned );
  if (!bRes)
    return eExtendedError;

  PRINTER_INFO_4 *pPrinterInfo = (PRINTER_INFO_4 *)pPrinterEnum.asArrayPtr();

  DWORD n = pDeviceList.size();
  pDeviceList.resize(n + pcReturned);
  for(DWORD f=0; f<pcReturned; ++f)
  {
    pDeviceList[n+f] = pPrinterInfo[f].pPrinterName;
  }

  return eOk;
#else
  return eOk;//return eOk, to provide the possibility to add some custom devices later
#endif
}

#if defined(OD_WINDOWS_DESKTOP)
class AutoClosePrinter
{
public:
  HANDLE m_hPrinter;

  AutoClosePrinter( ) : m_hPrinter( 0 ) { }
  AutoClosePrinter(HANDLE hPrinter) : m_hPrinter(hPrinter) { }
  operator HANDLE&() { return m_hPrinter; }
  operator LPHANDLE() { return &m_hPrinter; }

  virtual ~AutoClosePrinter()
  {
    ClosePrinter(m_hPrinter);
  }
};

struct TCHAR64
{
  TCHAR tch[64];

  OdString toOdString() const
  {
    return OdString(tch).left(64);
  }

  static OdString toOdString(LPCTSTR str)
  {
    return OdString(str).left(64);
  }
};
#endif

class CDoubleRect
{
public:
  double left;
  double right;
  double top;
  double bottom;

  CDoubleRect(const CDoubleRect &cdr)
  {
    left = cdr.left;
    right = cdr.right;
    top = cdr.top;
    bottom = cdr.bottom;
  }

  CDoubleRect()
    : left(0.), right(0.), top(0.), bottom(0.)
  {}
};

#if defined(OD_WINDOWS_DESKTOP)
class CPrinterInfo : public AutoClosePrinter
{
  OdString m_DriverName;
  OdString m_DeviceName;
  OdString m_PortName;
public:
  CPrinterInfo(OdString deviceName)
    : m_DeviceName(deviceName)
  {
  }

  OdResult openPrinter()
  {
    if (!OpenPrinter((LPTSTR)(LPCTSTR)m_DeviceName, &m_hPrinter, 0))
    {
      //DWORD err = GetLastError();
      return eInvalidInput;
    }

    m_DriverName = OD_T("WINSPOOL");

    return getPrinterPortName(m_PortName);
  }
protected:
  OdResult getPrinterPortName(OdString &portName)
  {
    DWORD neeeded;

    GetPrinter(m_hPrinter, 2, 0, 0, &neeeded);

    //DWORD err = GetLastError();

    OdUInt8Array pi2;
    pi2.resize(neeeded, 0);

    if (!GetPrinter(m_hPrinter, 2, pi2.asArrayPtr(), neeeded, &neeeded))
      return eInvalidInput;

    portName = ((PRINTER_INFO_2 *)pi2.asArrayPtr())->pPortName;

    return eOk;
  }

private:
  static bool twice (const FORM_INFO_1 &elem1, const TCHAR64 &elem2 )
  {
    OdString s1 = TCHAR64::toOdString(elem1.pName);
    OdString s2 = elem2.toOdString();
    return !s1.iCompare(s2);
  }

protected:
  OdResult getMediaTypes(const OdArray<TCHAR64> &paperNames, const OdArray <WORD> &paperIDs, OdArray<PE_PAPER::kPaperType> &mediaTypes)
  {
    DWORD pcReturned, pcbNeeded;
    EnumForms(
      m_hPrinter,    // handle to printer object
      1,        // data level
      0,       // form information buffer
      0,        // size of form information buffer
      &pcbNeeded,  // bytes received or required
      &pcReturned  // number of forms received
      );

    OdArray<BYTE> buf; buf.resize(pcbNeeded, 0);

    EnumForms(
      m_hPrinter,    // handle to printer object
      1,        // data level
      buf.asArrayPtr(), // form information buffer
      pcbNeeded,        // size of form information buffer
      &pcbNeeded,  // bytes received or required
      &pcReturned  // number of forms received
      );

    PFORM_INFO_1 pForm1 = (PFORM_INFO_1)buf.asArrayPtr();
    mediaTypes.resize(paperNames.size(), PE_PAPER::eDeviceDefined);

    for(OdUInt32 f=0; f<paperNames.size(); ++f)
    {
      if (paperIDs[f] >= PE_PAPER::predefinedPaperInfoMax)
      {
        mediaTypes[f] = PE_PAPER::eUserDefined;
      }
      else
      {
        PFORM_INFO_1 pFounded = 
          std::search(pForm1, pForm1 + pcReturned, paperNames.begin() + f, paperNames.begin() + f + 1, CPrinterInfo::twice);
        if (pFounded == pForm1 + pcReturned)
          mediaTypes[f] = PE_PAPER::eUserDefined;
      }
    }

    return eOk;
  }

  OdResult getPaperNames(OdArray<TCHAR64> &paperNames)
  {
    DWORD dw, numPapers;

    // got paper sizes name
    numPapers = DeviceCapabilities(
      m_DeviceName, m_PortName,
      DC_PAPERNAMES, 0, NULL );

    //dw = GetLastError();

    if (numPapers == -1)
      return eInvalidInput;

    paperNames.resize(numPapers);

    dw = DeviceCapabilities(
      m_DeviceName, m_PortName,
      DC_PAPERNAMES, (LPTSTR)paperNames.asArrayPtr(), NULL );
    if (dw == -1)
      return eInvalidInput;

    return eOk;
  }

  OdResult getPaperSizeId(OdArray<WORD> &paperIDs)
  {
    DWORD dw, numStandardPaperSizes;

    // got paper sizes id
    numStandardPaperSizes = DeviceCapabilities(
      m_DeviceName, m_PortName,
      DC_PAPERS, 0, NULL );

    paperIDs.resize(numStandardPaperSizes);

    dw = DeviceCapabilities(
      m_DeviceName, m_PortName,
      DC_PAPERS, (LPTSTR)paperIDs.asArrayPtr(), NULL );
    if (dw == -1)
      return eInvalidInput;

    return eOk;
  }

  OdResult getPaperSizes(OdArray<POINT> &paperSizes)
  {
    DWORD dw, numMMPapers;

    // got paper sizes in tenths of a millimeter
    numMMPapers = DeviceCapabilities(
      m_DeviceName, m_PortName,
      DC_PAPERSIZE, 0, NULL );

    paperSizes.resize(numMMPapers);

    dw = DeviceCapabilities(
      m_DeviceName, m_PortName,
      DC_PAPERSIZE, (LPTSTR)paperSizes.asArrayPtr(), NULL );
    if (dw == -1)
      return eInvalidInput;

    return eOk;
  }

  OdResult getPaperMargins(const OdArray<WORD> &paperIDs, OdArray< CDoubleRect > &paperMargins)
  {
    paperMargins.resize(paperIDs.size());

    // To prepare DEVMODE structure that specifies information 
    //  about the device initialization and environment of a printer.

    LONG
      numberOfBytes = DocumentProperties(
      NULL, m_hPrinter,	              // handle of printer object
      (LPTSTR)(LPCTSTR)m_DeviceName,	// address of device name
      0, 0, 0); 	  // mode flag
    if (numberOfBytes<0)
      return eInvalidInput;

    OdUInt8Array pNewDefModeBuf;
    pNewDefModeBuf.resize(numberOfBytes, 0);
    PDEVMODE pNewDefMode = (PDEVMODE)pNewDefModeBuf.asArrayPtr();

    LONG
    nResult = DocumentProperties(
      NULL, m_hPrinter,	              // handle of printer object
      (LPTSTR)(LPCTSTR)m_DeviceName,	// address of device name
      pNewDefMode, 0, DM_OUT_BUFFER); 	  // mode flag
    
    if (nResult < 0)
      return eInvalidInput;

    pNewDefMode->dmFields |= DM_ORIENTATION | DM_PAPERSIZE;
    pNewDefMode->dmOrientation = (short)DMORIENT_PORTRAIT;

    HDC m_hPrinterDC = CreateDC( m_DriverName, m_DeviceName, NULL, pNewDefMode );
    if (!m_hPrinterDC)
      return eInvalidInput;

    OdUInt32 f, k = paperIDs.size();
    for(f = 0; f < k && nResult >= 0; ++f)
    {
      // To change Printer settings

      if (paperIDs[f] == DMPAPER_USER)
      {           
        paperMargins[f].left   = 0.;
        paperMargins[f].right  = 0.;
        paperMargins[f].top    = 0.;
        paperMargins[f].bottom = 0.;
      }
      else
      {
        pNewDefMode->dmPaperSize = paperIDs[f];


        LONG nResult = DocumentProperties(
          NULL, m_hPrinter,	              // handle of printer object
          (LPTSTR)(LPCTSTR)m_DeviceName,	// address of device name
          pNewDefMode, pNewDefMode, DM_OUT_BUFFER|DM_IN_BUFFER); 	  // mode flag

        if (nResult < 0)
          return eInvalidInput;

        m_hPrinterDC = ResetDC(m_hPrinterDC, pNewDefMode);

        double dLogPixelX           = GetDeviceCaps(m_hPrinterDC, LOGPIXELSX);
        double dLogPixelY           = GetDeviceCaps(m_hPrinterDC, LOGPIXELSY);
        double koeffX               = dLogPixelX / kMmPerInch;
        double koeffY               = dLogPixelY / kMmPerInch;

        double dPrinterWidth        = GetDeviceCaps(m_hPrinterDC, PHYSICALWIDTH) / koeffX;
        double dPrinterHeight       = GetDeviceCaps(m_hPrinterDC, PHYSICALHEIGHT) / koeffY;

        ODA_ASSERT(!OdZero(dPrinterWidth) & !OdZero(dPrinterHeight));

        double dPrinterLeftMargin   = GetDeviceCaps(m_hPrinterDC, PHYSICALOFFSETX) / koeffX;
        double dPrinterTopMargin    = GetDeviceCaps(m_hPrinterDC, PHYSICALOFFSETY) / koeffY;
        double dPrinterMarginWidth  = GetDeviceCaps(m_hPrinterDC, HORZRES) / koeffX;
        double dPrinterMarginHeight = GetDeviceCaps(m_hPrinterDC, VERTRES) / koeffY;
        double dPrinterRightMargin  = dPrinterWidth  - dPrinterMarginWidth  - dPrinterLeftMargin;
        double dPrinterBottomMargin = dPrinterHeight - dPrinterMarginHeight - dPrinterTopMargin;

        paperMargins[f].left   = dPrinterLeftMargin;
        paperMargins[f].right  = dPrinterRightMargin;
        paperMargins[f].top    = dPrinterTopMargin;
        paperMargins[f].bottom = dPrinterBottomMargin;
      }
    }
    DeleteDC(m_hPrinterDC);

    return eOk;
  }

public:  
  
  OdResult getDefaultPaper(OdString &defaultPaper)
  {
    defaultPaper = "";

    DWORD neeeded;

    GetPrinter(m_hPrinter, 2, 0, 0, &neeeded);

    OdUInt8Array pi2;
    pi2.resize(neeeded, 0);

    if (!GetPrinter(m_hPrinter, 2, pi2.asArrayPtr(), neeeded, &neeeded))
      return eInvalidInput;

    defaultPaper = ((PRINTER_INFO_2 *)pi2.asArrayPtr())->pDevMode->dmFormName;

    return eOk;
  }
  
  OdResult getPrinterPaperInfo(OdArray<TCHAR64> &paperNames, OdArray<WORD> &paperIDs, OdArray<POINT> &paperSizes, OdArray< CDoubleRect > &paperMargins, OdArray<PE_PAPER::kPaperType> &mediaTypes, bool bUpdateMediaMargins, OdDbPlotSettingsValidatorPE::psvPaperInfo *pRequestedMediaInfo)
  {
    OdResult res;

    if (pRequestedMediaInfo)
    {
      res = getPaperNames(paperNames);
      if (res == eOk)
      {
        res = getPaperSizeId(paperIDs);
        if (res == eOk)
        {
          for(OdUInt32 f=0; f<paperNames.size(); ++f)
          {
            if (paperNames[f].toOdString() == pRequestedMediaInfo->localeName)
            {
              OdArray<WORD> singleIDs;
              singleIDs.push_back( paperIDs[f] );
              res = getPaperMargins(singleIDs, paperMargins);
              return res;
            }
          }
        }
      }
      return eInvalidInput;
    }
    else
    {
      res = getPaperNames(paperNames);
      if (res == eOk)
      {
        res = getPaperSizeId(paperIDs);
        if (res == eOk)
        {
          res = getPaperSizes(paperSizes);
          if (res == eOk)
          {
            if (bUpdateMediaMargins)
            {
              res = getPaperMargins(paperIDs, paperMargins);
            }

            if (res == eOk)
            {
              res = getMediaTypes(paperNames, paperIDs,  mediaTypes);
            }
          }
        }
      }
    }

    return res;
  }
};
#endif

static OdResult getSystemDeviceDefaultMedia(const OdString &deviceName, OdString &defaultMedia)
{
  OdResult res = eExtendedError;

#if defined(OD_WINDOWS_DESKTOP)
  CPrinterInfo hPrinter(deviceName);
  res = hPrinter.openPrinter();
  if (res != eOk)
    return res;

  // This is not canonical media name! Should be converted later.
  res = hPrinter.getDefaultPaper(defaultMedia);
#endif

  return res;
}

static OdResult getSystemDeviceMediaList(const OdString &deviceName, OdArray<OdDbPlotSettingsValidatorPE::psvPaperInfo> &pMediaList, bool bUpdateMediaMargins, OdDbPlotSettingsValidatorPE::psvPaperInfo *pRequestedMediaInfo)
{
  OdResult res = eExtendedError;

#if defined(OD_WINDOWS_DESKTOP)
  CPrinterInfo hPrinter(deviceName);
  res = hPrinter.openPrinter();
  if (res != eOk)
    return res;

  OdArray<TCHAR64> paperNames;
  OdArray<WORD> paperIDs;
  OdArray<POINT> paperSizes;
  OdArray<CDoubleRect> paperMargins;
  OdArray<PE_PAPER::kPaperType> mediaTypes;

  if (pRequestedMediaInfo)
  {
    res = hPrinter.getPrinterPaperInfo(paperNames, paperIDs, paperSizes, paperMargins, mediaTypes, false, pRequestedMediaInfo);
    if (res == eOk)
    {
      pRequestedMediaInfo->left = paperMargins.first().left;
      pRequestedMediaInfo->right = paperMargins.first().right;
      pRequestedMediaInfo->top = paperMargins.first().top;
      pRequestedMediaInfo->bottom = paperMargins.first().bottom;
      if (pRequestedMediaInfo->units == OdDbPlotSettings::kInches)
      {
        pRequestedMediaInfo->left /= kMmPerInch;;
        pRequestedMediaInfo->right /= kMmPerInch;;
        pRequestedMediaInfo->top /= kMmPerInch;;
        pRequestedMediaInfo->bottom /= kMmPerInch;;
      }
    }
    else
    {
      pRequestedMediaInfo->left = -HUGE_VAL;
      pRequestedMediaInfo->right = -HUGE_VAL;
      pRequestedMediaInfo->top = -HUGE_VAL;
      pRequestedMediaInfo->bottom = -HUGE_VAL;
    }
    return res;
  }

  res = hPrinter.getPrinterPaperInfo(paperNames, paperIDs, paperSizes, paperMargins, mediaTypes, bUpdateMediaMargins, 0);
  if (res != eOk)
    return res;

  // fill media list
  pMediaList.resize(paperNames.size());
  for(OdUInt32 f=0; f<pMediaList.size(); ++f)
  {
    pMediaList[f].localeName    = paperNames[f].toOdString();
    pMediaList[f].canonicalName = PE_PAPER::getCanonicalNameForDevice(paperIDs[f], mediaTypes[f], pMediaList[f].localeName);
    pMediaList[f].units         = PE_PAPER::getUnitsForDevice(paperIDs[f]);

    pMediaList[f].w = paperSizes[f].x / 10.;
    pMediaList[f].h = paperSizes[f].y / 10.;
    if (pMediaList[f].units == OdDbPlotSettings::kInches)
    {
      pMediaList[f].w /= kMmPerInch;
      pMediaList[f].h /= kMmPerInch;
    }

    if (bUpdateMediaMargins)
    {
      pMediaList[f].left   = paperMargins[f].left;
      pMediaList[f].right  = paperMargins[f].right;
      pMediaList[f].top    = paperMargins[f].top;
      pMediaList[f].bottom = paperMargins[f].bottom;
      if (bUpdateMediaMargins && pMediaList[f].units == OdDbPlotSettings::kInches)
      {
        pMediaList[f].left /= kMmPerInch;
        pMediaList[f].right /= kMmPerInch;
        pMediaList[f].top /= kMmPerInch;
        pMediaList[f].bottom /= kMmPerInch;
      }
    }
    else
    {
      pMediaList[f].left   = -HUGE_VAL;
      pMediaList[f].right  = -HUGE_VAL;
      pMediaList[f].top    = -HUGE_VAL;
      pMediaList[f].bottom = -HUGE_VAL;
    }
  }
#endif
  return res;
}

static void findFilesByPath(OdString nextPath, OdArray<OdString> &pPlotStyleSheetList)
{
#if defined(OD_WINDOWS_DESKTOP)
  WIN32_FIND_DATA findFileData;
  memset(&findFileData, 0, sizeof(WIN32_FIND_DATA));

  HANDLE hFind = FindFirstFile(nextPath, &findFileData);
  BOOL bFind = (hFind != INVALID_HANDLE_VALUE);
  while (bFind)
  {
    if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
    {
      pPlotStyleSheetList.push_back( OdString(findFileData.cFileName) );
    }

    bFind = ::FindNextFile(hFind, &findFileData);
  }

  FindClose(hFind);
#elif defined(OD_HAVE_GLOB_FUNC)
// Linux implementation of file search by pattern
glob_t glob_result;
glob(std::string(nextPath).c_str(), GLOB_TILDE, NULL, &glob_result);
for(OdUInt32 i = 0; i < glob_result.gl_pathc; ++i)
{
  OdString fName(glob_result.gl_pathv[i]);
  OdInt32 pos = fName.reverseFind(L'\\'),
          pos1 = fName.reverseFind(L'/');
  pos = odmax(pos, pos1);
  if ( pos != -1 )
    fName = fName.right(fName.getLength()-pos-1);
  pPlotStyleSheetList.push_back(fName);
}
globfree(&glob_result);
#endif
}

// User defined device sample. DWG To PDF.pc3 device.
#define PDF_DEVICE_NAME L"DWG To PDF.pc3"
#define DWF_DEVICE_NAME L"DWF6 ePlot.pc3"

OdResult OdPlotSettingsValidatorPEImpl::getMediaMargins(const OdString &deviceName, psvPaperInfo &pMediaInfo)
{
  OdResult res = eExtendedError;
  OdArray<OdDbPlotSettingsValidatorPE::psvPaperInfo> pMediaList; // dummy
#if defined(OD_WINDOWS_DESKTOP)
  res = getSystemDeviceMediaList(deviceName, pMediaList, false, &pMediaInfo);
#else
  if (!deviceName.compare(CUSTOM_DEVICE_NAME) || !deviceName.compare(PDF_DEVICE_NAME) || !deviceName.compare(DWF_DEVICE_NAME))
  {
    getMediaList(deviceName, pMediaList, false);
    OdArray < OdDbPlotSettingsValidatorPE::psvPaperInfo >::const_iterator iter = customMediaList->begin();
    int i = 0;
    while (iter != customMediaList->end())
    {
      if (pMediaInfo.localeName == (*iter).localeName)
      {
        pMediaInfo = (*iter);
        res =  eOk;
      }
      ++iter;
      ++i;
    }
    res = eNoMatchingMedia;
  }
  else
    res = eInvalidInput;
#endif

  return res;
}

OdResult OdPlotSettingsValidatorPEImpl::getPlotStyleSheetList(OdArray<OdString> &pPlotStyleSheetList, OdDbBaseHostAppServices *pHostApp)
{
  OdString nextPath;

  if (pHostApp)
  {
    pHostApp->getEnv(OD_T("DDPLOTSTYLEPATHS"), nextPath);
  }
  else
  {
    ::odrxSystemServices()->getEnvVar(OD_T("DDPLOTSTYLEPATHS"), nextPath);
  }

  while (!nextPath.isEmpty())
  {
    OdString path = nextPath.spanExcluding(";");
    if (nextPath.find(';') >= 0 && nextPath.find(';') < nextPath.getLength() - 1)
    {
      nextPath = nextPath.mid(nextPath.find(';') + 1);
    }
    else
    {
      nextPath.empty();
    }
#if defined(OD_WINDOWS_DESKTOP)
    findFilesByPath(path + OD_T("\\*.ctb"), pPlotStyleSheetList);
    findFilesByPath(path + OD_T("\\*.stb"), pPlotStyleSheetList);
#else
    findFilesByPath(path + OD_T("/*.ctb"), pPlotStyleSheetList);
    findFilesByPath(path + OD_T("/*.stb"), pPlotStyleSheetList);
#endif
  }
  return eOk;
}

static OdResult getPDFMediaList(OdArray<OdDbPlotSettingsValidatorPE::psvPaperInfo> &pMediaList)
{
  struct PdfPaper
  {
    const OdChar* _localeName;
    const OdChar* _canonicalName;
    double _w;
    double _h;
    OdDbPlotSettings::PlotPaperUnits _units;

    double _xminMargin;
    double _yminMargin;
    double _xmaxMargin;
    double _ymaxMargin;
  };

  static PdfPaper pdfPapers [] = 
  {
    { OD_T("ISO full bleed B5 (250.00 x 176.00 MM)")    , OD_T("ISO_full_bleed_B5_(250.00_x_176.00_MM)")    , 250     , 176     , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793746948242188 , 0.793746948242188 } ,
    { OD_T("ISO full bleed B5 (176.00 x 250.00 MM)")    , OD_T("ISO_full_bleed_B5_(176.00_x_250.00_MM)")    , 176     , 250     , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793746948242188 , 0.793746948242188 } ,
    { OD_T("ISO full bleed B4 (353.00 x 250.00 MM)")    , OD_T("ISO_full_bleed_B4_(353.00_x_250.00_MM)")    , 353     , 250     , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793762207031250 , 0.793746948242188 } ,
    { OD_T("ISO full bleed B4 (250.00 x 353.00 MM)")    , OD_T("ISO_full_bleed_B4_(250.00_x_353.00_MM)")    , 250     , 353     , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793746948242188 , 0.793762207031250 } ,
    { OD_T("ISO full bleed B3 (500.00 x 353.00 MM)")    , OD_T("ISO_full_bleed_B3_(500.00_x_353.00_MM)")    , 500     , 353     , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793762207031250 , 0.793762207031250 } ,
    { OD_T("ISO full bleed B3 (353.00 x 500.00 MM)")    , OD_T("ISO_full_bleed_B3_(353.00_x_500.00_MM)")    , 353     , 500     , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793762207031250 , 0.793762207031250 } ,
    { OD_T("ISO full bleed B2 (707.00 x 500.00 MM)")    , OD_T("ISO_full_bleed_B2_(707.00_x_500.00_MM)")    , 707     , 500     , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793762207031250 , 0.793762207031250 } ,
    { OD_T("ISO full bleed B2 (500.00 x 707.00 MM)")    , OD_T("ISO_full_bleed_B2_(500.00_x_707.00_MM)")    , 500     , 707     , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793762207031250 , 0.793762207031250 } ,
    { OD_T("ISO full bleed B1 (1000.00 x 707.00 MM)")   , OD_T("ISO_full_bleed_B1_(1000.00_x_707.00_MM)")   , 1000    , 707     , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793762207031250 , 0.793762207031250 } ,
    { OD_T("ISO full bleed B1 (707.00 x 1000.00 MM)")   , OD_T("ISO_full_bleed_B1_(707.00_x_1000.00_MM)")   , 707     , 1000    , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793762207031250 , 0.793762207031250 } ,
    { OD_T("ISO full bleed B0 (1414.00 x 1000.00 MM)")  , OD_T("ISO_full_bleed_B0_(1414.00_x_1000.00_MM)")  , 1414    , 1000    , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793701171875000 , 0.793762207031250 } ,
    { OD_T("ISO full bleed B0 (1000.00 x 1414.00 MM)")  , OD_T("ISO_full_bleed_B0_(1000.00_x_1414.00_MM)")  , 1000    , 1414    , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793762207031250 , 0.793701171875000 } ,
    { OD_T("ISO full bleed A5 (210.00 x 148.00 MM)")    , OD_T("ISO_full_bleed_A5_(210.00_x_148.00_MM)")    , 210     , 148     , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793746948242188 , 0.793746948242188 } ,
    { OD_T("ISO full bleed A5 (148.00 x 210.00 MM)")    , OD_T("ISO_full_bleed_A5_(148.00_x_210.00_MM)")    , 148     , 210     , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793746948242188 , 0.793746948242188 } ,
    { OD_T("ISO full bleed 2A0 (1189.00 x 1682.00 MM)") , OD_T("ISO_full_bleed_2A0_(1189.00_x_1682.00_MM)") , 1189    , 1682    , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793701171875000 , 0.793701171875000 } ,
    { OD_T("ISO full bleed 4A0 (1682.00 x 2378.00 MM)") , OD_T("ISO_full_bleed_4A0_(1682.00_x_2378.00_MM)") , 1682    , 2378    , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793701171875000 , 0.793701171875000 } ,
    { OD_T("ISO full bleed A4 (297.00 x 210.00 MM)")    , OD_T("ISO_full_bleed_A4_(297.00_x_210.00_MM)")    , 297     , 210     , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793762207031250 , 0.793746948242188 } ,
    { OD_T("ISO full bleed A4 (210.00 x 297.00 MM)")    , OD_T("ISO_full_bleed_A4_(210.00_x_297.00_MM)")    , 210     , 297     , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793746948242188 , 0.793762207031250 } ,
    { OD_T("ISO full bleed A3 (420.00 x 297.00 MM)")    , OD_T("ISO_full_bleed_A3_(420.00_x_297.00_MM)")    , 420     , 297     , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793762207031250 , 0.793762207031250 } ,
    { OD_T("ISO full bleed A3 (297.00 x 420.00 MM)")    , OD_T("ISO_full_bleed_A3_(297.00_x_420.00_MM)")    , 297     , 420     , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793762207031250 , 0.793762207031250 } ,
    { OD_T("ISO full bleed A2 (594.00 x 420.00 MM)")    , OD_T("ISO_full_bleed_A2_(594.00_x_420.00_MM)")    , 594     , 420     , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793762207031250 , 0.793762207031250 } ,
    { OD_T("ISO full bleed A2 (420.00 x 594.00 MM)")    , OD_T("ISO_full_bleed_A2_(420.00_x_594.00_MM)")    , 420     , 594     , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793762207031250 , 0.793762207031250 } ,
    { OD_T("ISO full bleed A1 (841.00 x 594.00 MM)")    , OD_T("ISO_full_bleed_A1_(841.00_x_594.00_MM)")    , 841     , 594     , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793762207031250 , 0.793762207031250 } ,
    { OD_T("ISO full bleed A1 (594.00 x 841.00 MM)")    , OD_T("ISO_full_bleed_A1_(594.00_x_841.00_MM)")    , 594     , 841     , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793762207031250 , 0.793762207031250 } ,
    { OD_T("ISO full bleed A0 (841.00 x 1189.00 MM)")   , OD_T("ISO_full_bleed_A0_(841.00_x_1189.00_MM)")   , 841     , 1189    , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793762207031250 , 0.793701171875000 } ,
    { OD_T("ARCH full bleed E1 (30.00 x 42.00 Inches)") , OD_T("ARCH_full_bleed_E1_(30.00_x_42.00_Inches)") , 30      , 42      , OdDbPlotSettings::kInches      , 0.031249999530672 , 0.031249999530672 , 0.031250480591781 , 0.031248077632874 } ,
    { OD_T("ARCH full bleed E (36.00 x 48.00 Inches)")  , OD_T("ARCH_full_bleed_E_(36.00_x_48.00_Inches)")  , 36      , 48      , OdDbPlotSettings::kInches      , 0.031249999530672 , 0.031249999530672 , 0.031250480591781 , 0.031248077632874 } ,
    { OD_T("ARCH full bleed D (36.00 x 24.00 Inches)")  , OD_T("ARCH_full_bleed_D_(36.00_x_24.00_Inches)")  , 36      , 24      , OdDbPlotSettings::kInches      , 0.031249999530672 , 0.031249999530672 , 0.031250480591781 , 0.031250480591781 } ,
    { OD_T("ARCH full bleed D (24.00 x 36.00 Inches)")  , OD_T("ARCH_full_bleed_D_(24.00_x_36.00_Inches)")  , 24      , 36      , OdDbPlotSettings::kInches      , 0.031249999530672 , 0.031249999530672 , 0.031250480591781 , 0.031250480591781 } ,
    { OD_T("ARCH full bleed C (24.00 x 18.00 Inches)")  , OD_T("ARCH_full_bleed_C_(24.00_x_18.00_Inches)")  , 24      , 18      , OdDbPlotSettings::kInches      , 0.031249999530672 , 0.031249999530672 , 0.031250480591781 , 0.031250480591781 } ,
    { OD_T("ARCH full bleed C (18.00 x 24.00 Inches)")  , OD_T("ARCH_full_bleed_C_(18.00_x_24.00_Inches)")  , 18      , 24      , OdDbPlotSettings::kInches      , 0.031249999530672 , 0.031249999530672 , 0.031250480591781 , 0.031250480591781 } ,
    { OD_T("ARCH full bleed B (18.00 x 12.00 Inches)")  , OD_T("ARCH_full_bleed_B_(18.00_x_12.00_Inches)")  , 18      , 12      , OdDbPlotSettings::kInches      , 0.031249999530672 , 0.031249999530672 , 0.031250480591781 , 0.031250480591781 } ,
    { OD_T("ARCH full bleed B (12.00 x 18.00 Inches)")  , OD_T("ARCH_full_bleed_B_(12.00_x_18.00_Inches)")  , 12      , 18      , OdDbPlotSettings::kInches      , 0.031249999530672 , 0.031249999530672 , 0.031250480591781 , 0.031250480591781 } ,
    { OD_T("ARCH full bleed A (12.00 x 9.00 Inches)")   , OD_T("ARCH_full_bleed_A_(12.00_x_9.00_Inches)")   , 12      , 9       , OdDbPlotSettings::kInches      , 0.031249999530672 , 0.031249999530672 , 0.031250480591781 , 0.031249879852055 } ,
    { OD_T("ARCH full bleed A (9.00 x 12.00 Inches)")   , OD_T("ARCH_full_bleed_A_(9.00_x_12.00_Inches)")   , 9       , 12      , OdDbPlotSettings::kInches      , 0.031249999530672 , 0.031249999530672 , 0.031249879852055 , 0.031250480591781 } ,
    { OD_T("ANSI full bleed F (28.00 x 40.00 Inches)")  , OD_T("ANSI_full_bleed_F_(28.00_x_40.00_Inches)")  , 28      , 40      , OdDbPlotSettings::kInches      , 0.031249999530672 , 0.031249999530672 , 0.031250480591781 , 0.031250480591781 } ,
    { OD_T("ANSI full bleed E (34.00 x 44.00 Inches)")  , OD_T("ANSI_full_bleed_E_(34.00_x_44.00_Inches)")  , 34      , 44      , OdDbPlotSettings::kInches      , 0.031249999530672 , 0.031249999530672 , 0.031250480591781 , 0.031248077632874 } ,
    { OD_T("ANSI full bleed D (34.00 x 22.00 Inches)")  , OD_T("ANSI_full_bleed_D_(34.00_x_22.00_Inches)")  , 34      , 22      , OdDbPlotSettings::kInches      , 0.031249999530672 , 0.031249999530672 , 0.031250480591781 , 0.031250480591781 } ,
    { OD_T("ANSI full bleed D (22.00 x 34.00 Inches)")  , OD_T("ANSI_full_bleed_D_(22.00_x_34.00_Inches)")  , 22      , 34      , OdDbPlotSettings::kInches      , 0.031249999530672 , 0.031249999530672 , 0.031250480591781 , 0.031250480591781 } ,
    { OD_T("ANSI full bleed C (22.00 x 17.00 Inches)")  , OD_T("ANSI_full_bleed_C_(22.00_x_17.00_Inches)")  , 22      , 17      , OdDbPlotSettings::kInches      , 0.031249999530672 , 0.031249999530672 , 0.031250480591781 , 0.031250480591781 } ,
    { OD_T("ANSI full bleed C (17.00 x 22.00 Inches)")  , OD_T("ANSI_full_bleed_C_(17.00_x_22.00_Inches)")  , 17      , 22      , OdDbPlotSettings::kInches      , 0.031249999530672 , 0.031249999530672 , 0.031250480591781 , 0.031250480591781 } ,
    { OD_T("ANSI full bleed B (17.00 x 11.00 Inches)")  , OD_T("ANSI_full_bleed_B_(17.00_x_11.00_Inches)")  , 17      , 11      , OdDbPlotSettings::kInches      , 0.031249999530672 , 0.031249999530672 , 0.031250480591781 , 0.031250480591781 } ,
    { OD_T("ANSI full bleed B (11.00 x 17.00 Inches)")  , OD_T("ANSI_full_bleed_B_(11.00_x_17.00_Inches)")  , 11      , 17      , OdDbPlotSettings::kInches      , 0.031249999530672 , 0.031249999530672 , 0.031250480591781 , 0.031250480591781 } ,
    { OD_T("ANSI full bleed A (11.00 x 8.50 Inches)")   , OD_T("ANSI_full_bleed_A_(11.00_x_8.50_Inches)")   , 11      , 8.5     , OdDbPlotSettings::kInches      , 0.031249999530672 , 0.031249999530672 , 0.031250480591781 , 0.031249879852055 } ,
    { OD_T("ANSI full bleed A (8.50 x 11.00 Inches)")   , OD_T("ANSI_full_bleed_A_(8.50_x_11.00_Inches)")   , 8.5     , 11      , OdDbPlotSettings::kInches      , 0.031249999530672 , 0.031249999530672 , 0.031249879852055 , 0.031250480591781 } ,
    { OD_T("ISO expand A0 (841.00 x 1189.00 MM)")       , OD_T("ISO_expand_A0_(841.00_x_1189.00_MM)")       , 841.0   , 1189.0  , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 10.793749809265137, 5.793762207031250 , 10.793701171875000 } ,
    { OD_T("ISO A0 (841.00 x 1189.00 MM)")              , OD_T("ISO_A0_(841.00_x_1189.00_MM)")              , 841.0   , 1189.0  , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 17.793750762939453, 5.793762207031250 , 17.793701171875000 } ,
    { OD_T("ISO expand A1 (841.00 x 594.00 MM)")        , OD_T("ISO_expand_A1_(841.00_x_594.00_MM)")        , 841.0   , 594.0   , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 10.793749809265137, 5.793762207031250 , 10.793762207031250 } ,
    { OD_T("ISO expand A1 (594.00 x 841.00 MM)")        , OD_T("ISO_expand_A1_(594.00_x_841.00_MM)")        , 594.0   , 841.0   , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 10.793749809265137, 5.793762207031250 , 10.793762207031250 } ,
    { OD_T("ISO A1 (841.00 x 594.00 MM)")               , OD_T("ISO_A1_(841.00_x_594.00_MM)")               , 841.0   , 594.0   , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 17.793750762939453, 5.793762207031250 , 17.793762207031250 } ,
    { OD_T("ISO A1 (594.00 x 841.00 MM)")               , OD_T("ISO_A1_(594.00_x_841.00_MM)")               , 594.0   , 841.0   , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 17.793750762939453, 5.793762207031250 , 17.793762207031250 } ,
    { OD_T("ISO expand A2 (594.00 x 420.00 MM)")        , OD_T("ISO_expand_A2_(594.00_x_420.00_MM)")        , 594.0   , 420.0   , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 10.793749809265137, 5.793762207031250 , 10.793762207031250 } ,
    { OD_T("ISO expand A2 (420.00 x 594.00 MM)")        , OD_T("ISO_expand_A2_(420.00_x_594.00_MM)")        , 420.0   , 594.0   , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 10.793749809265137, 5.793762207031250 , 10.793762207031250 } ,
    { OD_T("ISO A2 (594.00 x 420.00 MM)")               , OD_T("ISO_A2_(594.00_x_420.00_MM)")               , 594.0   , 420.0   , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 17.793750762939453, 5.793762207031250 , 17.793762207031250 } ,
    { OD_T("ISO A2 (420.00 x 594.00 MM)")               , OD_T("ISO_A2_(420.00_x_594.00_MM)")               , 420.0   , 594.0   , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 17.793750762939453, 5.793762207031250 , 17.793762207031250 } ,
    { OD_T("ISO expand A3 (420.00 x 297.00 MM)")        , OD_T("ISO_expand_A3_(420.00_x_297.00_MM)")        , 420.0   , 297.0   , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 10.793749809265137, 5.793762207031250 , 10.793762207031250 } ,
    { OD_T("ISO expand A3 (297.00 x 420.00 MM)")        , OD_T("ISO_expand_A3_(297.00_x_420.00_MM)")        , 297.0   , 420.0   , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 10.793749809265137, 5.793762207031250 , 10.793762207031250 } ,
    { OD_T("ISO A3 (420.00 x 297.00 MM)")               , OD_T("ISO_A3_(420.00_x_297.00_MM)")               , 420.0   , 297.0   , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 17.793750762939453, 5.793762207031250 , 17.793762207031250 } ,
    { OD_T("ISO A3 (297.00 x 420.00 MM)")               , OD_T("ISO_A3_(297.00_x_420.00_MM)")               , 297.0   , 420.0   , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 17.793750762939453, 5.793762207031250 , 17.793762207031250 } ,
    { OD_T("ISO expand A4 (297.00 x 210.00 MM)")        , OD_T("ISO_expand_A4_(297.00_x_210.00_MM)")        , 297.0   , 210.0   , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 10.793749809265137, 5.793762207031250 , 10.793746948242188 } ,
    { OD_T("ISO expand A4 (210.00 x 297.00 MM)")        , OD_T("ISO_expand_A4_(210.00_x_297.00_MM)")        , 210.0   , 297.0   , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 10.793749809265137, 5.793746948242188 , 10.793762207031250 } ,
    { OD_T("ISO A4 (297.00 x 210.00 MM)")               , OD_T("ISO_A4_(297.00_x_210.00_MM)")               , 297.0   , 210.0   , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 17.793750762939453, 5.793762207031250 , 17.793746948242188 } ,
    { OD_T("ISO A4 (210.00 x 297.00 MM)")               , OD_T("ISO_A4_(210.00_x_297.00_MM)")               , 210.0   , 297.0   , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 17.793750762939453, 5.793746948242188 , 17.793762207031250 } ,
    { OD_T("ARCH expand E1 (30.00 x 42.00 Inches)")     , OD_T("ARCH_expand_E1_(30.00_x_42.00_Inches)")     , 30.0    , 42.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100874292569 , 0.424948865034449 } ,
    { OD_T("ARCH E1 (30.00 x 42.00 Inches)")            , OD_T("ARCH_E1_(30.00_x_42.00_Inches)")            , 30.0    , 42.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100874292569 , 0.700539416215551 } ,
    { OD_T("ARCH expand E (36.00 x 48.00 Inches)")      , OD_T("ARCH_expand_E_(36.00_x_48.00_Inches)")      , 36.0    , 48.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100874292569 , 0.424948865034449 } ,
    { OD_T("ARCH E (36.00 x 48.00 Inches)")             , OD_T("ARCH_E_(36.00_x_48.00_Inches)")             , 36.0    , 48.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100874292569 , 0.700539416215551 } ,
    { OD_T("ARCH expand D (36.00 x 24.00 Inches)")      , OD_T("ARCH_expand_D_(36.00_x_24.00_Inches)")      , 36.0    , 24.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100874292569 , 0.424951267993356 } ,
    { OD_T("ARCH expand D (24.00 x 36.00 Inches)")      , OD_T("ARCH_expand_D_(24.00_x_36.00_Inches)")      , 24.0    , 36.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100874292569 , 0.424951267993356 } ,
    { OD_T("ARCH D (36.00 x 24.00 Inches)")             , OD_T("ARCH_D_(36.00_x_24.00_Inches)")             , 36.0    , 24.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100874292569 , 0.700541819174459 } ,
    { OD_T("ARCH D (24.00 x 36.00 Inches)")             , OD_T("ARCH_D_(24.00_x_36.00_Inches)")             , 24.0    , 36.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100874292569 , 0.700541819174459 } ,
    { OD_T("ARCH expand C (24.00 x 18.00 Inches)")      , OD_T("ARCH_expand_C_(24.00_x_18.00_Inches)")      , 24.0    , 18.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100874292569 , 0.424951267993356 } ,
    { OD_T("ARCH expand C (18.00 x 24.00 Inches)")      , OD_T("ARCH_expand_C_(18.00_x_24.00_Inches)")      , 18.0    , 24.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100874292569 , 0.424951267993356 } ,
    { OD_T("ARCH C (24.00 x 18.00 Inches)")             , OD_T("ARCH_C_(24.00_x_18.00_Inches)")             , 24.0    , 18.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100874292569 , 0.700541819174459 } ,
    { OD_T("ARCH C (18.00 x 24.00 Inches)")             , OD_T("ARCH_C_(18.00_x_24.00_Inches)")             , 18.0    , 24.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100874292569 , 0.700541819174459 } ,
    { OD_T("ANSI expand E (34.00 x 44.00 Inches)")      , OD_T("ANSI_expand_E_(34.00_x_44.00_Inches)")      , 34.0    , 44.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100874292569 , 0.424948865034449 } ,
    { OD_T("ANSI E (34.00 x 44.00 Inches)")             , OD_T("ANSI_E_(34.00_x_44.00_Inches)")             , 34.0    , 44.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100874292569 , 0.700539416215551 } ,
    { OD_T("ANSI expand D (34.00 x 22.00 Inches)")      , OD_T("ANSI_expand_D_(34.00_x_22.00_Inches)")      , 34.0    , 22.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100874292569 , 0.424951267993356 } ,
    { OD_T("ANSI expand D (22.00 x 34.00 Inches)")      , OD_T("ANSI_expand_D_(22.00_x_34.00_Inches)")      , 22.0    , 34.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100874292569 , 0.424951267993356 } ,
    { OD_T("ANSI D (34.00 x 22.00 Inches)")             , OD_T("ANSI_D_(34.00_x_22.00_Inches)")             , 34.0    , 22.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100874292569 , 0.700541819174459 } ,
    { OD_T("ANSI D (22.00 x 34.00 Inches)")             , OD_T("ANSI_D_(22.00_x_34.00_Inches)")             , 22.0    , 34.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100874292569 , 0.700541819174459 } ,
    { OD_T("ANSI expand C (22.00 x 17.00 Inches)")      , OD_T("ANSI_expand_C_(22.00_x_17.00_Inches)")      , 22.0    , 17.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100874292569 , 0.424951267993356 } ,
    { OD_T("ANSI expand C (17.00 x 22.00 Inches)")      , OD_T("ANSI_expand_C_(17.00_x_22.00_Inches)")      , 17.0    , 22.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100874292569 , 0.424951267993356 } ,
    { OD_T("ANSI C (22.00 x 17.00 Inches)")             , OD_T("ANSI_C_(22.00_x_17.00_Inches)")             , 22.0    , 17.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100874292569 , 0.700541819174459 } ,
    { OD_T("ANSI C (17.00 x 22.00 Inches)")             , OD_T("ANSI_C_(17.00_x_22.00_Inches)")             , 17.0    , 22.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100874292569 , 0.700541819174459 } ,
    { OD_T("ANSI expand B (17.00 x 11.00 Inches)")      , OD_T("ANSI_expand_B_(17.00_x_11.00_Inches)")      , 17.0    , 11.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100874292569 , 0.424951267993356 } ,
    { OD_T("ANSI expand B (11.00 x 17.00 Inches)")      , OD_T("ANSI_expand_B_(11.00_x_17.00_Inches)")      , 11.0    , 17.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100874292569 , 0.424951267993356 } ,
    { OD_T("ANSI B (17.00 x 11.00 Inches)")             , OD_T("ANSI_B_(17.00_x_11.00_Inches)")             , 17.0    , 11.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100874292569 , 0.700541819174459 } ,
    { OD_T("ANSI B (11.00 x 17.00 Inches)")             , OD_T("ANSI_B_(11.00_x_17.00_Inches)")             , 11.0    , 17.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100874292569 , 0.700541819174459 } ,
    { OD_T("ANSI expand A (11.00 x 8.50 Inches)")       , OD_T("ANSI_expand_A_(11.00_x_8.50_Inches)")       , 11.0    , 8.5     , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100874292569 , 0.424950667253629 } ,
    { OD_T("ANSI expand A (8.50 x 11.00 Inches)")       , OD_T("ANSI_expand_A_(8.50_x_11.00_Inches)")       , 8.5     , 11.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100273552842 , 0.424951267993356 } ,
    { OD_T("ANSI A (11.00 x 8.50 Inches)")              , OD_T("ANSI_A_(11.00_x_8.50_Inches)")              , 11.0    , 8.5     , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100874292569 , 0.700541218434732 } ,
    { OD_T("ANSI A (8.50 x 11.00 Inches)")              , OD_T("ANSI_A_(8.50_x_11.00_Inches)")              , 8.5     , 11.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100273552842 , 0.700541819174459 } ,
  };

  OdUInt32 numPapers = sizeof(pdfPapers)/sizeof(pdfPapers[0]);
  OdUInt32 f = pMediaList.size();
  pMediaList.resize(f + numPapers);
  for(OdUInt32 i=0; i<numPapers; ++i,++f)
  {
    pMediaList[f].canonicalName = pdfPapers[i]._canonicalName;
    pMediaList[f].localeName    = pdfPapers[i]._localeName;
    pMediaList[f].w             = pdfPapers[i]._w;
    pMediaList[f].h             = pdfPapers[i]._h;
    pMediaList[f].left          = pdfPapers[i]._xminMargin;
    pMediaList[f].top           = pdfPapers[i]._ymaxMargin;
    pMediaList[f].right         = pdfPapers[i]._xmaxMargin;
    pMediaList[f].bottom        = pdfPapers[i]._yminMargin;
    pMediaList[f].units         = pdfPapers[i]._units;
  }

  return eOk;
}

static OdResult getDWFMediaList(OdArray<OdDbPlotSettingsValidatorPE::psvPaperInfo> &pMediaList)
{
  struct Paper
  {
    const OdChar* _localeName;
    const OdChar* _canonicalName;
    double _w;
    double _h;
    OdDbPlotSettings::PlotPaperUnits _units;

    double _xminMargin;
    double _yminMargin;
    double _xmaxMargin;
    double _ymaxMargin;
  };

  static const Paper dwfPapers [] = 
  {
    { OD_T("ISO full bleed B5 (250.00 x 176.00 MM)")    , OD_T("ISO_full_bleed_B5_(250.00_x_176.00_MM)")    , 250  , 176  , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793746948242188 , 0.793746948242188 } ,
    { OD_T("ISO full bleed B5 (176.00 x 250.00 MM)")    , OD_T("ISO_full_bleed_B5_(176.00_x_250.00_MM)")    , 176  , 250  , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793746948242188 , 0.793746948242188 } ,
    { OD_T("ISO full bleed B4 (353.00 x 250.00 MM)")    , OD_T("ISO_full_bleed_B4_(353.00_x_250.00_MM)")    , 353  , 250  , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793762207031250 , 0.793746948242188 } ,
    { OD_T("ISO full bleed B4 (250.00 x 353.00 MM)")    , OD_T("ISO_full_bleed_B4_(250.00_x_353.00_MM)")    , 250  , 353  , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793746948242188 , 0.793762207031250 } ,
    { OD_T("ISO full bleed B3 (500.00 x 353.00 MM)")    , OD_T("ISO_full_bleed_B3_(500.00_x_353.00_MM)")    , 500  , 353  , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793762207031250 , 0.793762207031250 } ,
    { OD_T("ISO full bleed B3 (353.00 x 500.00 MM)")    , OD_T("ISO_full_bleed_B3_(353.00_x_500.00_MM)")    , 353  , 500  , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793762207031250 , 0.793762207031250 } ,
    { OD_T("ISO full bleed B2 (707.00 x 500.00 MM)")    , OD_T("ISO_full_bleed_B2_(707.00_x_500.00_MM)")    , 707  , 500  , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793762207031250 , 0.793762207031250 } ,
    { OD_T("ISO full bleed B2 (500.00 x 707.00 MM)")    , OD_T("ISO_full_bleed_B2_(500.00_x_707.00_MM)")    , 500  , 707  , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793762207031250 , 0.793762207031250 } ,
    { OD_T("ISO full bleed B1 (1000.00 x 707.00 MM)")   , OD_T("ISO_full_bleed_B1_(1000.00_x_707.00_MM)")   , 1000 , 707  , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793762207031250 , 0.793762207031250 } ,
    { OD_T("ISO full bleed B1 (707.00 x 1000.00 MM)")   , OD_T("ISO_full_bleed_B1_(707.00_x_1000.00_MM)")   , 707  , 1000 , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793762207031250 , 0.793762207031250 } ,
    { OD_T("ISO full bleed B0 (1414.00 x 1000.00 MM)")  , OD_T("ISO_full_bleed_B0_(1414.00_x_1000.00_MM)")  , 1414 , 1000 , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793701171875000 , 0.793762207031250 } ,
    { OD_T("ISO full bleed B0 (1000.00 x 1414.00 MM)")  , OD_T("ISO_full_bleed_B0_(1000.00_x_1414.00_MM)")  , 1000 , 1414 , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793762207031250 , 0.793701171875000 } ,
    { OD_T("ISO full bleed A5 (210.00 x 148.00 MM)")    , OD_T("ISO_full_bleed_A5_(210.00_x_148.00_MM)")    , 210  , 148  , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793746948242188 , 0.793746948242188 } ,
    { OD_T("ISO full bleed A5 (148.00 x 210.00 MM)")    , OD_T("ISO_full_bleed_A5_(148.00_x_210.00_MM)")    , 148  , 210  , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793746948242188 , 0.793746948242188 } ,
    { OD_T("ISO full bleed 2A0 (1189.00 x 1682.00 MM)") , OD_T("ISO_full_bleed_2A0_(1189.00_x_1682.00_MM)") , 1189 , 1682 , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793701171875000 , 0.793701171875000 } ,
    { OD_T("ISO full bleed 4A0 (1682.00 x 2378.00 MM)") , OD_T("ISO_full_bleed_4A0_(1682.00_x_2378.00_MM)") , 1682 , 2378 , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793701171875000 , 0.793701171875000 } ,
    { OD_T("ISO full bleed A4 (297.00 x 210.00 MM)")    , OD_T("ISO_full_bleed_A4_(297.00_x_210.00_MM)")    , 297  , 210  , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793762207031250 , 0.793746948242188 } ,
    { OD_T("ISO full bleed A4 (210.00 x 297.00 MM)")    , OD_T("ISO_full_bleed_A4_(210.00_x_297.00_MM)")    , 210  , 297  , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793746948242188 , 0.793762207031250 } ,
    { OD_T("ISO full bleed A3 (420.00 x 297.00 MM)")    , OD_T("ISO_full_bleed_A3_(420.00_x_297.00_MM)")    , 420  , 297  , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793762207031250 , 0.793762207031250 } ,
    { OD_T("ISO full bleed A3 (297.00 x 420.00 MM)")    , OD_T("ISO_full_bleed_A3_(297.00_x_420.00_MM)")    , 297  , 420  , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793762207031250 , 0.793762207031250 } ,
    { OD_T("ISO full bleed A2 (594.00 x 420.00 MM)")    , OD_T("ISO_full_bleed_A2_(594.00_x_420.00_MM)")    , 594  , 420  , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793762207031250 , 0.793762207031250 } ,
    { OD_T("ISO full bleed A2 (420.00 x 594.00 MM)")    , OD_T("ISO_full_bleed_A2_(420.00_x_594.00_MM)")    , 420  , 594  , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793762207031250 , 0.793762207031250 } ,
    { OD_T("ISO full bleed A1 (841.00 x 594.00 MM)")    , OD_T("ISO_full_bleed_A1_(841.00_x_594.00_MM)")    , 841  , 594  , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793762207031250 , 0.793762207031250 } ,
    { OD_T("ISO full bleed A1 (594.00 x 841.00 MM)")    , OD_T("ISO_full_bleed_A1_(594.00_x_841.00_MM)")    , 594  , 841  , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793762207031250 , 0.793762207031250 } ,
    { OD_T("ISO full bleed A0 (841.00 x 1189.00 MM)")   , OD_T("ISO_full_bleed_A0_(841.00_x_1189.00_MM)")   , 841  , 1189 , OdDbPlotSettings::kMillimeters , 0.793749988079071 , 0.793749988079071 , 0.793762207031250 , 0.793701171875000 } ,
    { OD_T("ARCH full bleed E1 (30.00 x 42.00 Inches)") , OD_T("ARCH_full_bleed_E1_(30.00_x_42.00_Inches)") , 30   , 42   , OdDbPlotSettings::kInches      , 0.031249999530672 , 0.031249999530672 , 0.031250480591781 , 0.031248077632874 } ,
    { OD_T("ARCH full bleed E (36.00 x 48.00 Inches)")  , OD_T("ARCH_full_bleed_E_(36.00_x_48.00_Inches)")  , 36   , 48   , OdDbPlotSettings::kInches      , 0.031249999530672 , 0.031249999530672 , 0.031250480591781 , 0.031248077632874 } ,
    { OD_T("ARCH full bleed D (36.00 x 24.00 Inches)")  , OD_T("ARCH_full_bleed_D_(36.00_x_24.00_Inches)")  , 36   , 24   , OdDbPlotSettings::kInches      , 0.031249999530672 , 0.031249999530672 , 0.031250480591781 , 0.031250480591781 } ,
    { OD_T("ARCH full bleed D (24.00 x 36.00 Inches)")  , OD_T("ARCH_full_bleed_D_(24.00_x_36.00_Inches)")  , 24   , 36   , OdDbPlotSettings::kInches      , 0.031249999530672 , 0.031249999530672 , 0.031250480591781 , 0.031250480591781 } ,
    { OD_T("ARCH full bleed C (24.00 x 18.00 Inches)")  , OD_T("ARCH_full_bleed_C_(24.00_x_18.00_Inches)")  , 24   , 18   , OdDbPlotSettings::kInches      , 0.031249999530672 , 0.031249999530672 , 0.031250480591781 , 0.031250480591781 } ,
    { OD_T("ARCH full bleed C (18.00 x 24.00 Inches)")  , OD_T("ARCH_full_bleed_C_(18.00_x_24.00_Inches)")  , 18   , 24   , OdDbPlotSettings::kInches      , 0.031249999530672 , 0.031249999530672 , 0.031250480591781 , 0.031250480591781 } ,
    { OD_T("ARCH full bleed B (18.00 x 12.00 Inches)")  , OD_T("ARCH_full_bleed_B_(18.00_x_12.00_Inches)")  , 18   , 12   , OdDbPlotSettings::kInches      , 0.031249999530672 , 0.031249999530672 , 0.031250480591781 , 0.031250480591781 } ,
    { OD_T("ARCH full bleed B (12.00 x 18.00 Inches)")  , OD_T("ARCH_full_bleed_B_(12.00_x_18.00_Inches)")  , 12   , 18   , OdDbPlotSettings::kInches      , 0.031249999530672 , 0.031249999530672 , 0.031250480591781 , 0.031250480591781 } ,
    { OD_T("ARCH full bleed A (12.00 x 9.00 Inches)")   , OD_T("ARCH_full_bleed_A_(12.00_x_9.00_Inches)")   , 12   , 9    , OdDbPlotSettings::kInches      , 0.031249999530672 , 0.031249999530672 , 0.031250480591781 , 0.031249879852055 } ,
    { OD_T("ARCH full bleed A (9.00 x 12.00 Inches)")   , OD_T("ARCH_full_bleed_A_(9.00_x_12.00_Inches)")   , 9    , 12   , OdDbPlotSettings::kInches      , 0.031249999530672 , 0.031249999530672 , 0.031249879852055 , 0.031250480591781 } ,
    { OD_T("ANSI full bleed F (28.00 x 40.00 Inches)")  , OD_T("ANSI_full_bleed_F_(28.00_x_40.00_Inches)")  , 28   , 40   , OdDbPlotSettings::kInches      , 0.031249999530672 , 0.031249999530672 , 0.031250480591781 , 0.031250480591781 } ,
    { OD_T("ANSI full bleed E (34.00 x 44.00 Inches)")  , OD_T("ANSI_full_bleed_E_(34.00_x_44.00_Inches)")  , 34   , 44   , OdDbPlotSettings::kInches      , 0.031249999530672 , 0.031249999530672 , 0.031250480591781 , 0.031248077632874 } ,
    { OD_T("ANSI full bleed D (34.00 x 22.00 Inches)")  , OD_T("ANSI_full_bleed_D_(34.00_x_22.00_Inches)")  , 34   , 22   , OdDbPlotSettings::kInches      , 0.031249999530672 , 0.031249999530672 , 0.031250480591781 , 0.031250480591781 } ,
    { OD_T("ANSI full bleed D (22.00 x 34.00 Inches)")  , OD_T("ANSI_full_bleed_D_(22.00_x_34.00_Inches)")  , 22   , 34   , OdDbPlotSettings::kInches      , 0.031249999530672 , 0.031249999530672 , 0.031250480591781 , 0.031250480591781 } ,
    { OD_T("ANSI full bleed C (22.00 x 17.00 Inches)")  , OD_T("ANSI_full_bleed_C_(22.00_x_17.00_Inches)")  , 22   , 17   , OdDbPlotSettings::kInches      , 0.031249999530672 , 0.031249999530672 , 0.031250480591781 , 0.031250480591781 } ,
    { OD_T("ANSI full bleed C (17.00 x 22.00 Inches)")  , OD_T("ANSI_full_bleed_C_(17.00_x_22.00_Inches)")  , 17   , 22   , OdDbPlotSettings::kInches      , 0.031249999530672 , 0.031249999530672 , 0.031250480591781 , 0.031250480591781 } ,
    { OD_T("ANSI full bleed B (17.00 x 11.00 Inches)")  , OD_T("ANSI_full_bleed_B_(17.00_x_11.00_Inches)")  , 17   , 11   , OdDbPlotSettings::kInches      , 0.031249999530672 , 0.031249999530672 , 0.031250480591781 , 0.031250480591781 } ,
    { OD_T("ANSI full bleed B (11.00 x 17.00 Inches)")  , OD_T("ANSI_full_bleed_B_(11.00_x_17.00_Inches)")  , 11   , 17   , OdDbPlotSettings::kInches      , 0.031249999530672 , 0.031249999530672 , 0.031250480591781 , 0.031250480591781 } ,
    { OD_T("ANSI full bleed A (11.00 x 8.50 Inches)")   , OD_T("ANSI_full_bleed_A_(11.00_x_8.50_Inches)")   , 11   , 8.5  , OdDbPlotSettings::kInches      , 0.031249999530672 , 0.031249999530672 , 0.031250480591781 , 0.031249879852055 } ,
    { OD_T("ANSI full bleed A (8.50 x 11.00 Inches)")   , OD_T("ANSI_full_bleed_A_(8.50_x_11.00_Inches)")   , 8.5  , 11   , OdDbPlotSettings::kInches      , 0.031249999530672 , 0.031249999530672 , 0.031249879852055 , 0.031250480591781 } ,
    { OD_T("ISO expand A0 (841.00 x 1189.00 MM)")       , OD_T("ISO_expand_A0_(841.00_x_1189.00_MM)")       , 841  , 1189 , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 10.793749809265137, 5.793762207031250 , 10.793701171875000 } ,
    { OD_T("ISO A0 (841.00 x 1189.00 MM)")              , OD_T("ISO_A0_(841.00_x_1189.00_MM)")              , 841  , 1189 , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 17.793750762939453, 5.793762207031250 , 17.793701171875000 } ,
    { OD_T("ISO expand A1 (841.00 x 594.00 MM)")        , OD_T("ISO_expand_A1_(841.00_x_594.00_MM)")        , 841  , 594  , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 10.793749809265137, 5.793762207031250 , 10.793762207031250 } ,
    { OD_T("ISO expand A1 (594.00 x 841.00 MM)")        , OD_T("ISO_expand_A1_(594.00_x_841.00_MM)")        , 594  , 841  , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 10.793749809265137, 5.793762207031250 , 10.793762207031250 } ,
    { OD_T("ISO A1 (841.00 x 594.00 MM)")               , OD_T("ISO_A1_(841.00_x_594.00_MM)")               , 841  , 594  , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 17.793750762939453, 5.793762207031250 , 17.793762207031250 } ,
    { OD_T("ISO A1 (594.00 x 841.00 MM)")               , OD_T("ISO_A1_(594.00_x_841.00_MM)")               , 594  , 841  , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 17.793750762939453, 5.793762207031250 , 17.793762207031250 } ,
    { OD_T("ISO expand A2 (594.00 x 420.00 MM)")        , OD_T("ISO_expand_A2_(594.00_x_420.00_MM)")        , 594  , 420  , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 10.793749809265137, 5.793762207031250 , 10.793762207031250 } ,
    { OD_T("ISO expand A2 (420.00 x 594.00 MM)")        , OD_T("ISO_expand_A2_(420.00_x_594.00_MM)")        , 420  , 594  , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 10.793749809265137, 5.793762207031250 , 10.793762207031250 } ,
    { OD_T("ISO A2 (594.00 x 420.00 MM)")               , OD_T("ISO_A2_(594.00_x_420.00_MM)")               , 594  , 420  , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 17.793750762939453, 5.793762207031250 , 17.793762207031250 } ,
    { OD_T("ISO A2 (420.00 x 594.00 MM)")               , OD_T("ISO_A2_(420.00_x_594.00_MM)")               , 420  , 594  , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 17.793750762939453, 5.793762207031250 , 17.793762207031250 } ,
    { OD_T("ISO expand A3 (420.00 x 297.00 MM)")        , OD_T("ISO_expand_A3_(420.00_x_297.00_MM)")        , 420  , 297  , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 10.793749809265137, 5.793762207031250 , 10.793762207031250 } ,
    { OD_T("ISO expand A3 (297.00 x 420.00 MM)")        , OD_T("ISO_expand_A3_(297.00_x_420.00_MM)")        , 297  , 420  , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 10.793749809265137, 5.793762207031250 , 10.793762207031250 } ,
    { OD_T("ISO A3 (420.00 x 297.00 MM)")               , OD_T("ISO_A3_(420.00_x_297.00_MM)")               , 420  , 297  , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 17.793750762939453, 5.793762207031250 , 17.793762207031250 } ,
    { OD_T("ISO A3 (297.00 x 420.00 MM)")               , OD_T("ISO_A3_(297.00_x_420.00_MM)")               , 297  , 420  , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 17.793750762939453, 5.793762207031250 , 17.793762207031250 } ,
    { OD_T("ISO expand A4 (297.00 x 210.00 MM)")        , OD_T("ISO_expand_A4_(297.00_x_210.00_MM)")        , 297  , 210  , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 10.793749809265137, 5.793762207031250 , 10.793746948242188 } ,
    { OD_T("ISO expand A4 (210.00 x 297.00 MM)")        , OD_T("ISO_expand_A4_(210.00_x_297.00_MM)")        , 210  , 297  , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 10.793749809265137, 5.793746948242188 , 10.793762207031250 } ,
    { OD_T("ISO A4 (297.00 x 210.00 MM)")               , OD_T("ISO_A4_(297.00_x_210.00_MM)")               , 297  , 210  , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 17.793750762939453, 5.793762207031250 , 17.793746948242188 } ,
    { OD_T("ISO A4 (210.00 x 297.00 MM)")               , OD_T("ISO_A4_(210.00_x_297.00_MM)")               , 210  , 297  , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 17.793750762939453, 5.793746948242188 , 17.793762207031250 } ,
    { OD_T("ARCH expand E1 (30.00 x 42.00 Inches)")     , OD_T("ARCH_expand_E1_(30.00_x_42.00_Inches)")     , 30   , 42   , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100874292569 , 0.424948865034449 } ,
    { OD_T("ARCH E1 (30.00 x 42.00 Inches)")            , OD_T("ARCH_E1_(30.00_x_42.00_Inches)")            , 30   , 42   , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100874292569 , 0.700539416215551 } ,
    { OD_T("ARCH expand E (36.00 x 48.00 Inches)")      , OD_T("ARCH_expand_E_(36.00_x_48.00_Inches)")      , 36   , 48   , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100874292569 , 0.424948865034449 } ,
    { OD_T("ARCH E (36.00 x 48.00 Inches)")             , OD_T("ARCH_E_(36.00_x_48.00_Inches)")             , 36   , 48   , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100874292569 , 0.700539416215551 } ,
    { OD_T("ARCH expand D (36.00 x 24.00 Inches)")      , OD_T("ARCH_expand_D_(36.00_x_24.00_Inches)")      , 36   , 24   , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100874292569 , 0.424951267993356 } ,
    { OD_T("ARCH expand D (24.00 x 36.00 Inches)")      , OD_T("ARCH_expand_D_(24.00_x_36.00_Inches)")      , 24   , 36   , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100874292569 , 0.424951267993356 } ,
    { OD_T("ARCH D (36.00 x 24.00 Inches)")             , OD_T("ARCH_D_(36.00_x_24.00_Inches)")             , 36   , 24   , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100874292569 , 0.700541819174459 } ,
    { OD_T("ARCH D (24.00 x 36.00 Inches)")             , OD_T("ARCH_D_(24.00_x_36.00_Inches)")             , 24   , 36   , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100874292569 , 0.700541819174459 } ,
    { OD_T("ARCH expand C (24.00 x 18.00 Inches)")      , OD_T("ARCH_expand_C_(24.00_x_18.00_Inches)")      , 24   , 18   , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100874292569 , 0.424951267993356 } ,
    { OD_T("ARCH expand C (18.00 x 24.00 Inches)")      , OD_T("ARCH_expand_C_(18.00_x_24.00_Inches)")      , 18   , 24   , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100874292569 , 0.424951267993356 } ,
    { OD_T("ARCH C (24.00 x 18.00 Inches)")             , OD_T("ARCH_C_(24.00_x_18.00_Inches)")             , 24   , 18   , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100874292569 , 0.700541819174459 } ,
    { OD_T("ARCH C (18.00 x 24.00 Inches)")             , OD_T("ARCH_C_(18.00_x_24.00_Inches)")             , 18   , 24   , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100874292569 , 0.700541819174459 } ,
    { OD_T("ANSI expand E (34.00 x 44.00 Inches)")      , OD_T("ANSI_expand_E_(34.00_x_44.00_Inches)")      , 34   , 44   , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100874292569 , 0.424948865034449 } ,
    { OD_T("ANSI E (34.00 x 44.00 Inches)")             , OD_T("ANSI_E_(34.00_x_44.00_Inches)")             , 34   , 44   , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100874292569 , 0.700539416215551 } ,
    { OD_T("ANSI expand D (34.00 x 22.00 Inches)")      , OD_T("ANSI_expand_D_(34.00_x_22.00_Inches)")      , 34   , 22   , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100874292569 , 0.424951267993356 } ,
    { OD_T("ANSI expand D (22.00 x 34.00 Inches)")      , OD_T("ANSI_expand_D_(22.00_x_34.00_Inches)")      , 22   , 34   , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100874292569 , 0.424951267993356 } ,
    { OD_T("ANSI D (34.00 x 22.00 Inches)")             , OD_T("ANSI_D_(34.00_x_22.00_Inches)")             , 34   , 22   , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100874292569 , 0.700541819174459 } ,
    { OD_T("ANSI D (22.00 x 34.00 Inches)")             , OD_T("ANSI_D_(22.00_x_34.00_Inches)")             , 22   , 34   , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100874292569 , 0.700541819174459 } ,
    { OD_T("ANSI expand C (22.00 x 17.00 Inches)")      , OD_T("ANSI_expand_C_(22.00_x_17.00_Inches)")      , 22   , 17   , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100874292569 , 0.424951267993356 } ,
    { OD_T("ANSI expand C (17.00 x 22.00 Inches)")      , OD_T("ANSI_expand_C_(17.00_x_22.00_Inches)")      , 17   , 22   , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100874292569 , 0.424951267993356 } ,
    { OD_T("ANSI C (22.00 x 17.00 Inches)")             , OD_T("ANSI_C_(22.00_x_17.00_Inches)")             , 22   , 17   , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100874292569 , 0.700541819174459 } ,
    { OD_T("ANSI C (17.00 x 22.00 Inches)")             , OD_T("ANSI_C_(17.00_x_22.00_Inches)")             , 17   , 22   , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100874292569 , 0.700541819174459 } ,
    { OD_T("ANSI expand B (17.00 x 11.00 Inches)")      , OD_T("ANSI_expand_B_(17.00_x_11.00_Inches)")      , 17   , 11   , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100874292569 , 0.424951267993356 } ,
    { OD_T("ANSI expand B (11.00 x 17.00 Inches)")      , OD_T("ANSI_expand_B_(11.00_x_17.00_Inches)")      , 11   , 17   , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100874292569 , 0.424951267993356 } ,
    { OD_T("ANSI B (17.00 x 11.00 Inches)")             , OD_T("ANSI_B_(17.00_x_11.00_Inches)")             , 17   , 11   , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100874292569 , 0.700541819174459 } ,
    { OD_T("ANSI B (11.00 x 17.00 Inches)")             , OD_T("ANSI_B_(11.00_x_17.00_Inches)")             , 11   , 17   , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100874292569 , 0.700541819174459 } ,
    { OD_T("ANSI expand A (11.00 x 8.50 Inches)")       , OD_T("ANSI_expand_A_(11.00_x_8.50_Inches)")       , 11   , 8.5  , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100874292569 , 0.424950667253629 } ,
    { OD_T("ANSI expand A (8.50 x 11.00 Inches)")       , OD_T("ANSI_expand_A_(8.50_x_11.00_Inches)")       , 8.5  , 11   , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100273552842 , 0.424951267993356 } ,
    { OD_T("ANSI A (11.00 x 8.50 Inches)")              , OD_T("ANSI_A_(11.00_x_8.50_Inches)")              , 11   , 8.5  , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100874292569 , 0.700541218434732 } ,
    { OD_T("ANSI A (8.50 x 11.00 Inches)")              , OD_T("ANSI_A_(8.50_x_11.00_Inches)")              , 8.5  , 11   , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100273552842 , 0.700541819174459 } ,
  };

  OdUInt32 numPapers = sizeof(dwfPapers)/sizeof(dwfPapers[0]);
  OdUInt32 f = pMediaList.size();
  pMediaList.resize(f + numPapers);
  for(OdUInt32 i=0; i<numPapers; ++i,++f)
  {
    pMediaList[f].canonicalName = dwfPapers[i]._canonicalName;
    pMediaList[f].localeName    = dwfPapers[i]._localeName;
    pMediaList[f].w             = dwfPapers[i]._w;
    pMediaList[f].h             = dwfPapers[i]._h;
    pMediaList[f].left          = dwfPapers[i]._xminMargin;
    pMediaList[f].top           = dwfPapers[i]._ymaxMargin;
    pMediaList[f].right         = dwfPapers[i]._xmaxMargin;
    pMediaList[f].bottom        = dwfPapers[i]._yminMargin;
    pMediaList[f].units         = dwfPapers[i]._units;
  }

  return eOk;
}

OdResult OdPlotSettingsValidatorPEImpl::getDeviceList(OdArray<OdString> &pDeviceList)
{
  // add system installed printers
  OdResult res = getSystemDeviceList(pDeviceList);
  if (res != eOk)
    return res;

  // User defined device sample. add PDF device.
  pDeviceList.append(PDF_DEVICE_NAME);
  pDeviceList.append(DWF_DEVICE_NAME);

  OdDbPlotSettingsValidatorCustomMediaPEPtr pPE = OdDbPlotSettingsValidatorPE::desc()->getX(OdDbPlotSettingsValidatorCustomMediaPE::desc());
  if (!pPE.isNull())
  {
    pDeviceList.append(CUSTOM_DEVICE_NAME);
  }

  return eOk;
}

OdResult OdPlotSettingsValidatorPEImpl::getDefaultMedia(const OdString &deviceName, OdString &defaultMedia)
{
  OdResult res = eOk;

  OdDbPlotSettingsValidatorCustomMediaPEPtr pPE = OdDbPlotSettingsValidatorPE::desc()->getX(OdDbPlotSettingsValidatorCustomMediaPE::desc());
  if (!pPE.isNull() && !deviceName.iCompare(CUSTOM_DEVICE_NAME))
  {
    if (customMediaList)
      defaultMedia = customMediaList->last().localeName;
  }
  else if (!deviceName.iCompare(PDF_DEVICE_NAME))
  {
    defaultMedia = "ANSI A (11.00 x 8.50 Inches)";
  }
  else if (!deviceName.iCompare(DWF_DEVICE_NAME))
  {
    defaultMedia = "ANSI A (8.50 x 11.00 Inches)";
  }
  else
  {
    // Add media list for system installed devices.
    res = getSystemDeviceDefaultMedia(deviceName, defaultMedia);
  }
  return res;
}

OdResult OdPlotSettingsValidatorPEImpl::getMediaList(const OdString &deviceName, OdArray<psvPaperInfo> &pMediaList, bool bUpdateMediaMargins)
{
  OdResult res;

  OdDbPlotSettingsValidatorCustomMediaPEPtr pPE = OdDbPlotSettingsValidatorPE::desc()->getX(OdDbPlotSettingsValidatorCustomMediaPE::desc());
  if (!pPE.isNull() && !deviceName.iCompare(CUSTOM_DEVICE_NAME))
  {
    if (customMediaList)
      pMediaList = *customMediaList;
    else
      pMediaList.clear();
    res = eOk;
  }
  else  if (!deviceName.iCompare(PDF_DEVICE_NAME))
  {
    // User defined device sample. Add papers for PDF device.
    res = getPDFMediaList(pMediaList);
  }
  else if (!deviceName.iCompare(DWF_DEVICE_NAME))
  {
    res = getDWFMediaList(pMediaList);
  }
  else
  {
    // Add media list for system installed devices.
    res = getSystemDeviceMediaList(deviceName, pMediaList, bUpdateMediaMargins, 0);
  }

  return res;
}

OdResult OdPlotSettingsValidatorCustomMediaPEImpl::addMedia(const OdDbPlotSettingsValidatorPE::psvPaperInfo& mediaList)
{
  if (mediaList.w < .001 || mediaList.h < .001)
    return eInvalidInput;
  if (((mediaList.left + mediaList.right) > mediaList.w) ||
     ((mediaList.top + mediaList.bottom) > mediaList.h))
    return eInvalidInput;
  if (mediaList.localeName.isEmpty() || mediaList.canonicalName.isEmpty())
    return eInvalidInput;

  if (!customMediaList)
    customMediaList = new OdArray < OdDbPlotSettingsValidatorPE::psvPaperInfo > ;

  OdArray < OdDbPlotSettingsValidatorPE::psvPaperInfo >::iterator iter = customMediaList->begin();
  int i = 0;
  while (iter != customMediaList->end())
  {
    if (mediaList.canonicalName == (*iter).canonicalName)
    {
      customMediaList->removeAt(i);
      customMediaList->insertAt(i, mediaList);
      return eOk;
    }
    ++iter;
    ++i;
  }

  customMediaList->push_back(mediaList);
  return eOk;
}

OdResult OdPlotSettingsValidatorCustomMediaPEImpl::editMedia(const OdDbPlotSettingsValidatorPE::psvPaperInfo& mediaList)
{
  if (!customMediaList)
    return eNoMatchingMedia;

  if (mediaList.w < .001 || mediaList.h < .001)
    return eInvalidInput;
  if (((mediaList.left + mediaList.right) > mediaList.w) ||
    ((mediaList.top + mediaList.bottom) > mediaList.h))
    return eInvalidInput;
  if (mediaList.localeName.isEmpty() || mediaList.canonicalName.isEmpty())
    return eInvalidInput;

  OdArray < OdDbPlotSettingsValidatorPE::psvPaperInfo >::iterator iter = customMediaList->begin();
  int i = 0;
  while (iter != customMediaList->end())
  {
    if (mediaList.canonicalName == (*iter).canonicalName)
    {
      customMediaList->removeAt(i);
      customMediaList->insertAt(i, mediaList);
      return eOk;
    }
    ++iter;
    ++i;
  }
  return eNoMatchingMedia;
}

OdResult OdPlotSettingsValidatorCustomMediaPEImpl::removeMedia(const OdString& canonicalName)
{
  if (!customMediaList)
    return eNoMatchingMedia;
  if (canonicalName.isEmpty())
    return eInvalidInput;

  OdArray < OdDbPlotSettingsValidatorPE::psvPaperInfo >::iterator iter = customMediaList->begin();
  int i = 0;
  while (iter != customMediaList->end())
  {
    if (canonicalName == (*iter).canonicalName)
    {
      customMediaList->removeAt(i);
      return eOk;
    }
    ++iter;
    ++i;
  }
  return eNoMatchingMedia;
}

OdResult OdPlotSettingsValidatorCustomMediaPEImpl::getMedia(const OdString& canonicalName, OdDbPlotSettingsValidatorPE::psvPaperInfo& media)
{
  if (!customMediaList)
    return eNoMatchingMedia;
  if (canonicalName.isEmpty())
    return eInvalidInput;

  OdArray < OdDbPlotSettingsValidatorPE::psvPaperInfo >::const_iterator iter = customMediaList->begin();
  int i = 0;
  while (iter != customMediaList->end())
  {
    if (canonicalName == (*iter).canonicalName)
    {
      media = (*iter);
      return eOk;
    }
    ++iter;
    ++i;
  }
  return eNoMatchingMedia;
}

OdResult OdPlotSettingsValidatorCustomMediaPEImpl::getMedia(OdUInt16 index, OdDbPlotSettingsValidatorPE::psvPaperInfo& media)
{
  if (!customMediaList)
    return eOutOfRange;
  if (index >= customMediaList->size())
    return eOutOfRange;

  media = customMediaList->at(index);
  return eOk;
}

OdResult OdPlotSettingsValidatorCustomMediaPEImpl::clear()
{
  if (customMediaList)
    customMediaList->clear();
  return eOk;
}

OdUInt16 OdPlotSettingsValidatorCustomMediaPEImpl::size()
{
  if (customMediaList)
    return customMediaList->size();
  return 0;
}
