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





#if !defined(_ODTHUMBNAILIMAGE_INCLUDED_)
#define _ODTHUMBNAILIMAGE_INCLUDED_

#include "TD_PackPush.h"

#include "OdBinaryData.h"

class OdGiRasterImage;
typedef OdSmartPtr<OdGiRasterImage> OdGiRasterImagePtr;

/** \details
    This class is a data container class for .dwg file Preview Bitmaps.
    Corresponding C++ library: TD_DbRoot
    <group Other_Classes>
*/
class DBROOT_EXPORT OdThumbnailImage
{
public:  
  OdBinaryData header,
               bmp, // DIB (device independent bitmap)
               wmf,
               png;
  /** \details
    Returns true if and only if the header data is populated
    for this ThumbnailImage object.
  */
	bool hasHeader() const { return !header.empty(); }
  /** \details
    Returns true if and only if the bmp data is populated
    for this ThumbnailImage object.
  */
	bool hasBmp() const { return !bmp.empty(); }
  /** \details
    Returns true if and only if the wmf data is populated
    for this ThumbnailImage object.
  */
	bool hasWmf() const { return !wmf.empty(); }
  /** \details
    Returns true if and only if the png data is populated
    for this ThumbnailImage object.
  */
	bool hasPng() const { return !png.empty(); }

  /** \details
    Returns the number of entries populated for this ThumbnailImage object.
    
    \remarks
    Returns ((hasHeader()?1:0)+(hasBmp()?1:0)+(hasWmf()?1:0)+(hasPng()?1:0))
  */
	int getNumEntries() const
  {
    return (  (hasHeader() ? 1 : 0) 
            + (hasBmp() ? 1 : 0)
            + (hasWmf() ? 1 : 0)
            + (hasPng() ? 1 : 0));
  }

  /** \details
    Converts the png data to bmp data for this ThumbnailImage object.
  */
  bool convPngToBmp();

  /** \details
    Converts the bmp data to png data for this ThumbnailImage object.
  */
  bool convBmpToPng();

  /** \details
    Sets PNG, BMP or DIB data for this ThumbnailImage object.
  */
  void setImageData( const void* pData, OdUInt32 dataLengt);

  /** \details
    Gets PNG, BMP or DIB data for this ThumbnailImage object.
  */
  const void* getImageData(OdUInt32& dataLengt);

  /** \details
    Sets data from image for this ThumbnailImage object.
  */
  bool setRasterImage (
    const OdGiRasterImage* image);

  /** \details
    Gets raster image from data for this ThumbnailImage object.
  */
  OdGiRasterImagePtr getRasterImage ();

};

#include "TD_PackPop.h"

#endif // !defined(_ODTHUMBNAILIMAGE_INCLUDED_)

