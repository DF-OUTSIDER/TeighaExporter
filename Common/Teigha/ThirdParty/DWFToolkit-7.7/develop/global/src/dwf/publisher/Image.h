//
//  Copyright (c) 1996-2006 by Autodesk, Inc.
//
//  By using this code, you are agreeing to the terms and conditions of
//  the License Agreement included in the documentation for this code.
//
//  AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE CORRECTNESS
//  OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE IT. AUTODESK
//  PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY DISCLAIMS ANY
//  LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL DAMAGES FOR ERRORS,
//  OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
//
//  Use, duplication, or disclosure by the U.S. Government is subject to
//  restrictions set forth in FAR 52.227-19 (Commercial Computer Software
//  Restricted Rights) and DFAR 252.227-7013(c)(1)(ii) (Rights in Technical
//  Data and Computer Software), as applicable.
//

#ifndef _DWFTK_IMAGE_H
#define _DWFTK_IMAGE_H

///
///\file        dwf/publisher/Image.h
///\brief       This file contains the DWFImage and DWFTexture class declarations.
///


#ifndef DWFTK_READ_ONLY



#include "dwfcore/String.h"
#include "dwfcore/BufferInputStream.h"
using namespace DWFCore;

#include "dwf/publisher/Publisher.h"


namespace DWFToolkit
{

///
///\ingroup         dwfpublish
///
///\interface   DWFImage   dwf/publisher/Image.h    "dwf/publisher/Image.h"
///\brief       This class represents a publishable image.
///\since       7.0.1
///
class DWFImage : public DWFPublishableResource
                _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    typedef enum
    {
        ePreview,
        eOverlayPreview,
        eMarkupPreview,
        eTexture,
        eThumbnail,
        eIcon,
        eRasterOverlay,
        eRasterMarkup
    } teResourceType;

public:

    ///
    ///         Constructor
    ///
    ///\param   zMIMEType       The MIME type of the image data.
    ///\param   eType           An enumeration describing the image role/type.
    ///\param   nBitsPerPixel   The image pixel depth.
    ///\param   nWidth          The horizontal extent of the image in pixels.
    ///\param   nHeight         The veritical extent of the image in pixels.
    ///\param   pClipRegion     An optional path against which the image should be clipped when shown.
    ///                         This pointer is not copied; the caller must delete this memory when no longer in use.
    ///\throw   None
    ///
    ///\todo    The clipping data should be copied.
    ///
    DWFImage( const DWFString& zMIMEType,
              teResourceType   eType,
              unsigned char    nBitsPerPixel,
              double           nWidth,
              double           nHeight,
              double*          pClipRegion = NULL )
        throw()
        : _bOwnStream( false )
        , _pImageStream( NULL )
        , _zMIMEType( zMIMEType )
        , _eType( eType )
        , _nBitsPerPixel( nBitsPerPixel )
        , _nWidth( nWidth )
        , _nHeight( nHeight )
        , _pClipRegion( pClipRegion )
    {;}

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///    
    ~DWFImage()
        throw()
    {
        if (_pImageStream && _bOwnStream)
        {
            DWFCORE_FREE_OBJECT( _pImageStream );
        }
    }

    ///
    ///         Binds a data source stream to the object.
    ///
    ///\param   pImageStream    The data stream.
    ///\param   bOwnStream      If \e true, this object will delete \a pImageStream
    ///                         on destruction; \a pImageStream must be allocated with
    ///                         \b DWFCORE_ALLOC_OBJECT.
    ///\throw   None
    ///
    void attach( DWFInputStream* pImageStream, bool bOwnStream )
        throw( DWFException )
    {
        if (_pImageStream && _bOwnStream)
        {
            DWFCORE_FREE_OBJECT( _pImageStream );
            _pImageStream = NULL;
        }

        _pImageStream = pImageStream;
        _bOwnStream = bOwnStream;
    }

    ///
    ///         Returns the MIME type of the image.
    ///
    ///\return  The MIME type string.
    ///\throw   None
    ///
    const DWFString& getMIMEType()
        throw()
    {
        return _zMIMEType;
    }

    ///
    ///         Returns a stream from which the image data can be read.
    ///
    ///\return  The data stream. The caller must delete the pointer with \b DWFCORE_FREE_OBJECT.
    ///\throw   DWFException
    ///
    ///\todo    The original stream is wrapped in a buffer stream which prevents its deletion
    ///         and allows another publisher to acquire its data.  However, the original stream
    ///         is never rewound or verified that such an operation is possible.
    ///         Update this implementation to perform the expected behavior.
    ///
    DWFInputStream* getInputStream()
        throw( DWFException )
    {
        if (_pImageStream == NULL)
        {
            _DWFCORE_THROW( DWFNullPointerException, /*NOXLATE*/L"No stream bound to object" );
        }

        //
        // Wrap this up in a buffer stream
        //
        return DWFCORE_ALLOC_OBJECT( DWFBufferInputStream(_pImageStream, false) );
    }

    ///
    ///         Returns an enumeration describing the image role/type.
    ///
    ///\return  The type enumeration.
    ///\throw   None
    ///
    teResourceType type() const
        throw()
    {
        return _eType;
    }

    ///
    ///         Returns the image pixel depth.
    ///
    ///\return  The bits per image pixel.
    ///\throw   None
    ///
    unsigned char depth() const
        throw()
    {
        return _nBitsPerPixel;
    }

    ///
    ///         Returns the horizontal extent of the image in pixels.
    ///
    ///\return  The image width.
    ///\throw   None
    ///
    double width() const
        throw()
    {
        return _nWidth;
    }

    ///
    ///         Returns the vertical extent of the image in pixels.
    ///
    ///\return  The image height.
    ///\throw   None
    ///
    double height() const
        throw()
    {
        return _nHeight;
    }

    ///
    ///         Returns the clipping path to apply to the image when shown.
    ///
    ///\return  The clipping path (may be NULL.) 
    ///\throw   None
    ///
    double* clip() const
        throw()
    {
        return _pClipRegion;
    }

private:

    bool                _bOwnStream;
    DWFInputStream*     _pImageStream;
    DWFString           _zMIMEType;
    teResourceType      _eType;
    unsigned char       _nBitsPerPixel;
    double              _nWidth;
    double              _nHeight;
    double*             _pClipRegion;

private:

    //
    // Not Implemented
    //

    DWFImage();
    DWFImage( const DWFImage& );
    DWFImage& operator=( const DWFImage& );
};

///
///\ingroup         dwfpublish
///
///\interface   DWFTexture   dwf/publisher/Image.h    "dwf/publisher/Image.h"
///\brief       This class represents a publishable texture image.
///\since       7.0.1
///
class DWFTexture : public DWFImage
                   _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///         Constructor
    ///
    ///\param   zName           Unique name for the texture.
    ///\param   zMIMEType       The MIME type of the image data.
    ///\param   nBitsPerPixel   The image pixel depth.
    ///\param   nWidth          The horizontal extent of the image in pixels.
    ///\param   nHeight         The veritical extent of the image in pixels.
    ///\param   pClipRegion     An optional path against which the image should be clipped when shown.
    ///                         This pointer is not copied; the caller must delete this memory when no longer in use.
    ///\throw   None
    ///
    ///\todo    The clipping data should be copied.
    ///
    DWFTexture( const DWFString& zName,
                const DWFString& zMIMEType,
                unsigned char    nBitsPerPixel,
                double           nWidth,
                double           nHeight,
                double*          pClipRegion = NULL )
        throw()
        : DWFImage( zMIMEType,
                    eTexture,
                    nBitsPerPixel,
                    nWidth,
                    nHeight,
                    pClipRegion )
        , _zName( zName )
    {;}

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///   
    ~DWFTexture()
        throw()
    {;}

    ///
    ///         Returns the unique name of the texture.
    ///
    ///\return  The texture name.
    ///\throw   None
    ///
    const DWFString& name() const
        throw()
    {
        return _zName;
    }

private:

    DWFString _zName;

private:

    //
    // Not Implemented
    //

    DWFTexture();
    DWFTexture( const DWFTexture& );
    DWFTexture& operator=( const DWFTexture& );
};

}


#endif
#endif

