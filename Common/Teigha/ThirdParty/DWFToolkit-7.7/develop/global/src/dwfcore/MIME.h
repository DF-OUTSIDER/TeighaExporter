//
//  Copyright (c) 2003-2006 by Autodesk, Inc.
//
//  By using this code, you are agreeing to the terms and conditions of
//  the License Agreement included in the documentation for this code.
//
//  AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED,
//  AS TO THE CORRECTNESS OF THIS CODE OR ANY DERIVATIVE
//  WORKS WHICH INCORPORATE IT.
//
//  AUTODESK PROVIDES THE CODE ON AN "AS-IS" BASIS
//  AND EXPLICITLY DISCLAIMS ANY LIABILITY, INCLUDING
//  CONSEQUENTIAL AND INCIDENTAL DAMAGES FOR ERRORS,
//  OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
//
//  Use, duplication, or disclosure by the U.S. Government is subject to
//  restrictions set forth in FAR 52.227-19 (Commercial Computer Software
//  Restricted Rights) and DFAR 252.227-7013(c)(1)(ii) (Rights in Technical
//  Data and Computer Software), as applicable.
//

#ifndef _DWFCORE_MIME_H
#define _DWFCORE_MIME_H


///
///\file        dwfcore/MIME.h
///\brief       This header contains a MIME type string to file extension map.
///


#include "dwfcore/Core.h"
#include "dwfcore/STL.h"
#include "dwfcore/Pointer.h"
#include "dwfcore/Iterator.h"
#include "dwfcore/SkipList.h"



namespace DWFCore
{

///
///\class       DWFMIME    dwfcore/MIME.h    "dwfcore/MIME.h"
///\brief       Singleton providing a map of known MIME type strings to their file extensions.
///\since       1.0.1
///
class DWFMIME   : virtual public DWFCoreMemory
{

public:

    ///
    ///\brief   L"model/vnd.dwf"
    ///
    static const wchar_t* const kzMIMEType_DWF1;
    ///
    ///\brief   L"application/x-dwf"
    ///
    static const wchar_t* const kzMIMEType_DWF2;
    ///
    ///\brief   L"drawing/x-dwf"
    ///
    static const wchar_t* const kzMIMEType_DWF3;
    ///
    ///\brief   L"application/x-w2d"
    ///
    static const wchar_t* const kzMIMEType_W2D;
    ///
    ///\brief   L"application/x-w3d"
    ///
    static const wchar_t* const kzMIMEType_W3D;
    ///
    ///\brief   L"application/x-dsd"
    ///
    static const wchar_t* const kzMIMEType_DSD;
    ///
    ///\brief   L"application/x-dwg-state"
    ///
    static const wchar_t* const kzMIMEType_PIA;
    ///
    ///\brief   L"image/bmp"
    ///
    static const wchar_t* const kzMIMEType_BMP;
    ///
    ///\brief   L"image/cals"
    ///
    static const wchar_t* const kzMIMEType_CALS;
    ///
    ///\brief   L"image/flic"
    ///
    static const wchar_t* const kzMIMEType_FLIC;
    ///
    ///\brief   L"image/gif"
    ///
    static const wchar_t* const kzMIMEType_GIF;
    ///
    ///\brief   L"image/ig4"
    ///
    static const wchar_t* const kzMIMEType_IG4;
    ///
    ///\brief   L"image/jpeg"
    ///
    static const wchar_t* const kzMIMEType_JPG;
    ///
    ///\brief   L"image/pcx"
    ///
    static const wchar_t* const kzMIMEType_PCX;
    ///
    ///\brief   L"image/pict"
    ///
    static const wchar_t* const kzMIMEType_PICT;
    ///
    ///\brief   L"image/png"
    ///
    static const wchar_t* const kzMIMEType_PNG;
    ///
    ///\brief   L"image/rlc"
    ///
    static const wchar_t* const kzMIMEType_RLC;
    ///
    ///\brief   L"image/targa"
    ///
    static const wchar_t* const kzMIMEType_TARGA;
    ///
    ///\brief   L"image/tiff"
    ///
    static const wchar_t* const kzMIMEType_TIFF;
    ///
    ///\brief   L"text/plain"
    ///
    static const wchar_t* const kzMIMEType_TXT;
    ///
    ///\brief   L"text/xml"
    ///
    static const wchar_t* const kzMIMEType_XML;
    ///
    ///\brief   L"application-xfont"
    ///
    static const wchar_t* const kzMIMEType_FONT;
    ///
    ///\brief   L"application/vnd.openxmlformats-package.relationships+xml"
    ///
    static const wchar_t* const kzMIMEType_RELS;
    ///
    ///\brief   L"application/vnd.adsk-package.dwfx-dwfdocumentsequence+xml"
    ///
    static const wchar_t* const kzMIMEType_DWFSEQ;
    ///
    ///\brief   L"application/vnd.ms-package.xps-fixeddocumentsequence+xml"
    ///
    static const wchar_t* const kzMIMEType_FDSEQ;
    ///
    ///\brief   L"application/vnd.ms-package.xps-fixeddocument+xml"
    ///
    static const wchar_t* const kzMIMEType_FDOC;
    ///
    ///\brief   L"application/vnd.ms-package.xps-fixedpage+xml"
    ///
    static const wchar_t* const kzMIMEType_FPAGE;
    ///
    ///\brief   L"application/vnd.ms-opentype"
    ///
    static const wchar_t* const kzMIMEType_TTF;
    ///
    ///\brief   L"application/vnd.ms-package.xps-resourcedictionary+xml"
    ///
    static const wchar_t* const kzMIMEType_REMOTERESOURCEDICTIONARY;
    ///
    ///\brief   L"application/vnd.ms-package.obfuscated-opentype"
    ///
    static const wchar_t* const kzMIMEType_OBFUSCATEDOPENTYPE;

    //
    //  MIMETypes with no corresponding extensions
    //
    ///
    ///\brief   L"application/vnd.openxmlformats-package.core-properties+xml"
    ///
    static const wchar_t* const kzMIMEType_COREPROPERTIES;
    ///
    ///\brief   L"application/vnd.adsk-package.dwfx-fixedpage+xml"
    ///
    static const wchar_t* const kzMIMEType_FIXEDPAGEXML;

    ///
    ///\brief   "application/x-w2d-s"
    ///
    static const wchar_t* const kzMIMEType_W2D_S;
    ///
    ///\brief   "application/x-w3d-s"
    ///
    static const wchar_t* const kzMIMEType_W3D_S;
    ///
    ///\brief   "application/x-dsd-s"
    ///
    static const wchar_t* const kzMIMEType_DSD_S;
    ///
    ///\brief   "application/x-dwg-state-s"
    ///
    static const wchar_t* const kzMIMEType_PIA_S;
    ///
    ///\brief   "image/bmp-s"
    ///
    static const wchar_t* const kzMIMEType_BMP_S;
    ///
    ///\brief   "image/cals-s"
    ///
    static const wchar_t* const kzMIMEType_CALS_S;
    ///
    ///\brief   "image/gif-s"
    ///
    static const wchar_t* const kzMIMEType_GIF_S;
    ///
    ///\brief   "image/flic-s"
    ///
    static const wchar_t* const kzMIMEType_FLIC_S;
    ///
    ///\brief   "image/ig4-s"
    ///
    static const wchar_t* const kzMIMEType_IG4_S;
    ///
    ///\brief   "image/jpeg-s"
    ///
    static const wchar_t* const kzMIMEType_JPG_S;
    ///
    ///\brief   "image/pcx-s"
    ///
    static const wchar_t* const kzMIMEType_PCX_S;
    ///
    ///\brief   "image/pict-s"
    ///
    static const wchar_t* const kzMIMEType_PICT_S;
    ///
    ///\brief   "image/png-s"
    ///
    static const wchar_t* const kzMIMEType_PNG_S;
    ///
    ///\brief   "image/rlc-s"
    ///
    static const wchar_t* const kzMIMEType_RLC_S;
    ///
    ///\brief   "image/targa-s"
    ///
    static const wchar_t* const kzMIMEType_TARGA_S;
    ///
    ///\brief   "image/tiff-s"
    ///
    static const wchar_t* const kzMIMEType_TIFF_S;
    ///
    ///\brief   "text/plain-s"
    ///
    static const wchar_t* const kzMIMEType_TXT_S;
    ///
    ///\brief   "text/xml-s"
    ///
    static const wchar_t* const kzMIMEType_XML_S;
    ///
    ///\brief   "application/x-font-s"
    ///
    static const wchar_t* const kzMIMEType_FONT_S;
    ///
    ///\brief   L"application/vnd.ms-package.xps-fixedpage+xml-s"
    ///
    static const wchar_t* const kzMIMEType_FPAGE_S;
    ///
    ///\brief   L"application/vnd.ms-opentype-s"
    ///
    static const wchar_t* const kzMIMEType_TTF_S;
    ///
    ///\brief   L"application/vnd.ms-package.xps-resourcedictionary+xml-s"
    ///
    static const wchar_t* const kzMIMEType_REMOTERESOURCEDICTIONARY_S;
    ///
    ///\brief   L"application/vnd.ms-package.obfuscated-opentype-s"
    ///
    static const wchar_t* const kzMIMEType_OBFUSCATEDOPENTYPE_S;
    ///
    ///\brief   L"application/vnd.adsk-package.dwfx-fixedpage+xml-s"
    ///
    static const wchar_t* const kzMIMEType_FIXEDPAGEXML_S;



    ///
    ///\brief   L"dwf"
    ///
    static const wchar_t* const kzExtension_DWF;
    ///
    ///\brief   L"w2d"
    ///
    static const wchar_t* const kzExtension_W2D;
    ///
    ///\brief   L"w3d"
    ///
    static const wchar_t* const kzExtension_W3D;
    ///
    ///\brief   L"dsd"
    ///
    static const wchar_t* const kzExtension_DSD;
    ///
    ///\brief   L"pia"
    ///
    static const wchar_t* const kzExtension_PIA;
    ///
    ///\brief   L"bmp"
    ///
    static const wchar_t* const kzExtension_BMP;
    ///
    ///\brief   L"cal"
    ///
    static const wchar_t* const kzExtension_CALS;
    ///
    ///\brief   L"flc"
    ///
    static const wchar_t* const kzExtension_FLIC;
    ///
    ///\brief   L"gif"
    ///
    static const wchar_t* const kzExtension_GIF;
    ///
    ///\brief   L"ig4"
    ///
    static const wchar_t* const kzExtension_IG4;
    ///
    ///\brief   L"jpg"
    ///
    static const wchar_t* const kzExtension_JPG;
    ///
    ///\brief   L"pcx"
    ///
    static const wchar_t* const kzExtension_PCX;
    ///
    ///\brief   L"pct"
    ///
    static const wchar_t* const kzExtension_PICT;
    ///
    ///\brief   L"png"
    ///
    static const wchar_t* const kzExtension_PNG;
    ///
    ///\brief   L"rlc"
    ///
    static const wchar_t* const kzExtension_RLC;
    ///
    ///\brief   L"tga"
    ///
    static const wchar_t* const kzExtension_TARGA;
    ///
    ///\brief   L"tif"
    ///
    static const wchar_t* const kzExtension_TIFF;
    ///
    ///\brief   L"txt"
    ///
    static const wchar_t* const kzExtension_TXT;
    ///
    ///\brief   L"xml"
    ///
    static const wchar_t* const kzExtension_XML;
    ///
    ///\brief   L"ef_"
    ///
    static const wchar_t* const kzExtension_FONT;
    ///
    ///\brief   L"rels"
    ///
    static const wchar_t* const kzExtension_RELS;
    ///
    ///\brief   L"dwfseq"
    ///
    static const wchar_t* const kzExtension_DWFSEQ;
    ///
    ///\brief   L"fdseq"
    ///
    static const wchar_t* const kzExtension_FDSEQ;
    ///
    ///\brief   L"fdoc"
    ///
    static const wchar_t* const kzExtension_FDOC;
    ///
    ///\brief   L"fpage"
    ///
    static const wchar_t* const kzExtension_FPAGE;
    ///
    ///\brief   L"ttf"
    ///
    static const wchar_t* const kzExtension_TTF;
    ///
    ///\brief   L"odttf"
    ///
    static const wchar_t* const kzExtension_ODTTF;


    ///
    ///\brief   "w2d_s"
    ///
    static const wchar_t* const kzExtension_W2D_S;
    ///
    ///\brief   "w3d_s"
    ///
    static const wchar_t* const kzExtension_W3D_S;
    ///
    ///\brief   "dsd_s"
    ///
    static const wchar_t* const kzExtension_DSD_S;
    ///
    ///\brief   "pia_s"
    ///
    static const wchar_t* const kzExtension_PIA_S;
    ///
    ///\brief   "bmp_s"
    ///
    static const wchar_t* const kzExtension_BMP_S;
    ///
    ///\brief   "cals_s"
    ///
    static const wchar_t* const kzExtension_CALS_S;
    ///
    ///\brief   "gif_s"
    ///
    static const wchar_t* const kzExtension_GIF_S;
    ///
    ///\brief   "flc_s"
    ///
    static const wchar_t* const kzExtension_FLIC_S;
    ///
    ///\brief   "ig4_s"
    ///
    static const wchar_t* const kzExtension_IG4_S;
    ///
    ///\brief   "jpg_s"
    ///
    static const wchar_t* const kzExtension_JPG_S;
    ///
    ///\brief   "pcx_s"
    ///
    static const wchar_t* const kzExtension_PCX_S;
    ///
    ///\brief   "pct_s"
    ///
    static const wchar_t* const kzExtension_PICT_S;
    ///
    ///\brief   "png_s"
    ///
    static const wchar_t* const kzExtension_PNG_S;
    ///
    ///\brief   "rlc_s"
    ///
    static const wchar_t* const kzExtension_RLC_S;
    ///
    ///\brief   "trg_s"
    ///
    static const wchar_t* const kzExtension_TARGA_S;
    ///
    ///\brief   "tif_s"
    ///
    static const wchar_t* const kzExtension_TIFF_S;
    ///
    ///\brief   "txt_s"
    ///
    static const wchar_t* const kzExtension_TXT_S;
    ///
    ///\brief   "xml_s"
    ///
    static const wchar_t* const kzExtension_XML_S;
    ///
    ///\brief   "ef__s"
    ///
    static const wchar_t* const kzExtension_FONT_S;
    ///
    ///\brief   "xaml_s"
    ///
    static const wchar_t* const kzExtension_XAML_S;
    ///
    ///\brief   L"fpage_s"
    ///
    static const wchar_t* const kzExtension_FPAGE_S;
    ///
    ///\brief   L"ttf_s"
    ///
    static const wchar_t* const kzExtension_TTF_S;
    ///
    ///\brief   L"odttf_s"
    ///
    static const wchar_t* const kzExtension_ODTTF_S;


public:

    ///
    ///         Returns a list of zero or more MIME type strings
    ///         mapped to the file extension specified.
    ///
    ///\param   zExtension  The file extension (without a leading ".")
    ///\return  A pointer to an interator of MIME type string constants.
    ///         This pointer must be deleted by the caller using the \b DWFCORE_FREE_OBJECT macro.
    ///\throw   DWFException
    ///
    _DWFCORE_API 
    static DWFIterator<const wchar_t* const>* GetMIMEType( const wchar_t* const zExtension )
        throw( DWFException );

    ///
    ///         Returns the file extension string mapped to the MIME type specified.
    ///
    ///\param   zMIMEType   The MIME type string.
    ///\return  The file extension (without a leading ".") or NULL if the MIME type is unmapped.
    ///\throw   DWFException
    ///
    _DWFCORE_API
    static const wchar_t* const GetExtension( const wchar_t* const zMIMEType )
        throw( DWFException );

    ///
    ///         Given the MIME type, this returns whether a resource of the corresponding type
    ///         should be compressed or not in a package. If a MIME type is unknown, this will
    ///         return true.
    ///
    ///\param   zMIMEType   The MIMI type string.
    ///\return  Returns true if a resource of the given type should be compressed.
    ///\throw   None
    ///
    _DWFCORE_API
    static bool CompressByDefault( const wchar_t* const zMIMEType )
        throw( DWFException );

private:

    //
    // Constructor
    //
    DWFMIME()
        throw();

    //
    //
    //
    static void _Build()
        throw( DWFException );

private:

    typedef DWFWCharKeySkipList<const wchar_t*>                                 _tList;
public:  // allows access to _Iterator below on some UNIX platforms
    typedef std::multimap<const wchar_t*, const wchar_t*, tDWFWCharCompareLess> _tMultiMap;

private:
    static DWFPointer<_tList>       _kapTypeMap;         // MIME -> extension
    static DWFPointer<_tMultiMap>   _kapExtensionMap;    // extension -> MIME

    //
    //
    //
    class _Iterator : public DWFIterator<const wchar_t* const>
                    , virtual public DWFCoreMemory
    {

    public:

        //
        // Constructor
        //
      _Iterator( std::multimap<const wchar_t*, const wchar_t*, tDWFWCharCompareLess>::iterator iBegin,
                   std::multimap<const wchar_t*, const wchar_t*, tDWFWCharCompareLess>::iterator iEnd )
            throw()
            : _iBegin( iBegin )
            , _iEnd( iEnd )
            , _iCurrent( iBegin )
        {;}

        //
        // Copy Constructor
        //
        _Iterator( const _Iterator& i )
            throw()
            : _iBegin( i._iBegin )
            , _iEnd( i._iEnd )
            , _iCurrent( i._iCurrent )
        {;}

        //
        // Assignment
        //
        _Iterator& operator=( const _Iterator& i )
            throw()
        {
            _iBegin = i._iBegin;
            _iEnd = i._iEnd;
            _iCurrent = i._iCurrent;

            return *this;
        }

        //
        // Destructor
        //
        virtual ~_Iterator()
            throw()
        {;}

        //
        // Reset the iterator back to its initial position
        //
        void reset()
            throw()
        {
            _iCurrent = _iBegin;
        }

        //
        // Returns true if the iterator points to a valid element
        //
        bool valid()
            throw()
        {
            return (_iCurrent != _iEnd);
        }

        //
        // Move the iterator forward one element
        // Returns true if the iterator points to a valid element
        //      or false if the end of the iterator has been reached
        //
        bool next()
            throw()
        {
            if (valid())
            {
                _iCurrent++;
                return valid();
            }
            else
            {
                return false;
            }
        }

        //
        // Returns the current element or throws an exception
        //  if the end of the iterator has been reached
        //
        const wchar_t* const& get()
            throw( DWFException )
        {
            if (valid())
            {
                return _iCurrent->second;
            }
            else
            {
                _DWFCORE_THROW( DWFDoesNotExistException, /*NOXLATE*/L"No more elements" );
            }
        }

    private:

        _tMultiMap::iterator _iBegin;
        _tMultiMap::iterator _iEnd;
        _tMultiMap::iterator _iCurrent;
    };

private:

    //
    // Unimplemented
    //
    DWFMIME( const DWFMIME& );
    DWFMIME& operator=( const DWFMIME& );
};


#ifndef _DWFCORE_DEFINE_FOR_DOXYGEN_ONLY

    //
    // required for win32 dll external linkage
    //
#ifdef  _DWFCORE_WIN32_SYSTEM
#ifdef _TOOLKIT_IN_DLL_

//DNT_START
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_DWF1 =                        L"model/vnd.dwf";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_DWF2 =                        L"application/x-dwf";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_DWF3 =                        L"drawing/x-dwf";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_W2D =                         L"application/x-w2d";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_W3D =                         L"application/x-w3d";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_DSD =                         L"application/x-dsd";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_PIA =                         L"application/x-dwg-state";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_BMP =                         L"image/bmp";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_CALS =                        L"image/cals";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_FLIC =                        L"image/flic";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_GIF =                         L"image/gif";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_IG4 =                         L"image/ig4";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_JPG =                         L"image/jpeg";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_PCX =                         L"image/pcx";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_PICT =                        L"image/pict";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_PNG =                         L"image/png";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_RLC =                         L"image/rlc";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_TARGA =                       L"image/targa";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_TIFF =                        L"image/tiff";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_TXT =                         L"text/plain";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_XML =                         L"text/xml";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_FONT =                        L"application/x-font";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_RELS =                        L"application/vnd.openxmlformats-package.relationships+xml";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_DWFSEQ =                      L"application/vnd.adsk-package.dwfx-dwfdocumentsequence+xml";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_FDSEQ =                       L"application/vnd.ms-package.xps-fixeddocumentsequence+xml";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_FDOC =                        L"application/vnd.ms-package.xps-fixeddocument+xml";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_FPAGE =                       L"application/vnd.ms-package.xps-fixedpage+xml";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_TTF =                         L"application/vnd.ms-opentype";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_REMOTERESOURCEDICTIONARY =    L"application/vnd.ms-package.xps-resourcedictionary+xml";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_OBFUSCATEDOPENTYPE =          L"application/vnd.ms-package.obfuscated-opentype";

//
//  No corresponding extension for these MIMETypes
//
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_COREPROPERTIES = L"application/vnd.openxmlformats-package.core-properties+xml";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_FIXEDPAGEXML = L"application/vnd.adsk-package.dwfx-fixedpage+xml";


_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_W2D_S             = L"application/x-w2d-s";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_W3D_S             = L"application/x-w3d-s";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_DSD_S             = L"application/x-dsd-s";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_PIA_S             = L"application/x-dwg-state-s";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_BMP_S             = L"image/bmp-s";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_CALS_S            = L"image/cals-s";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_GIF_S             = L"image/gif-s";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_FLIC_S            = L"image/flic-s";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_IG4_S             = L"image/ig4-s";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_JPG_S             = L"image/jpeg-s";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_PCX_S             = L"image/pcx-s";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_PICT_S            = L"image/pict-s";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_PNG_S             = L"image/png-s";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_RLC_S             = L"image/rlc-s";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_TARGA_S           = L"image/targa-s";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_TIFF_S            = L"image/tiff-s";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_TXT_S             = L"text/plain-s";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_XML_S             = L"text/xml-s";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_FONT_S            = L"application/x-font-s";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_FPAGE_S                    = L"application/vnd.ms-package.xps-fixedpage+xml-s";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_TTF_S                      = L"application/vnd.ms-opentype-s";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_REMOTERESOURCEDICTIONARY_S = L"application/vnd.ms-package.xps-resourcedictionary+xml-s";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_OBFUSCATEDOPENTYPE_S       = L"application/vnd.ms-package.obfuscated-opentype-s";
_declspec(selectany) const wchar_t* const DWFMIME::kzMIMEType_FIXEDPAGEXML_S             = L"application/vnd.adsk-package.dwfx-fixedpage+xml-s";


_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_DWF =     L"dwf";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_W2D =     L"w2d";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_W3D =     L"w3d";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_DSD =     L"dsd";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_PIA =     L"pia";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_BMP =     L"bmp";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_CALS =    L"cal";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_FLIC =    L"flc";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_GIF =     L"gif";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_IG4 =     L"ig4";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_JPG =     L"jpg";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_PCX =     L"pcx";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_PICT =    L"pct";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_PNG =     L"png";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_RLC =     L"rlc";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_TARGA=    L"tga";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_TIFF =    L"tif";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_TXT =     L"txt";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_XML =     L"xml";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_FONT =    L"ef_";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_RELS =    L"rels";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_DWFSEQ =  L"dwfseq";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_FDSEQ =   L"fdseq";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_FDOC =    L"fdoc";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_FPAGE =   L"fpage";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_TTF =     L"ttf";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_ODTTF =   L"odttf";

_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_W2D_S =   L"w2d_s";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_W3D_S =   L"w3d_s";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_DSD_S =   L"dsd_s";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_PIA_S =   L"pia_s";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_BMP_S =   L"bmp_s";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_CALS_S =  L"cal_s";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_FLIC_S =  L"flc_s";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_GIF_S =   L"gif_s";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_IG4_S =   L"ig4_s";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_JPG_S =   L"jpg_s";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_PCX_S =   L"pcx_s";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_PICT_S =  L"pct_s";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_PNG_S =   L"png_s";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_RLC_S =   L"rlc_s";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_TARGA_S = L"tga_s";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_TIFF_S =  L"tif_s";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_TXT_S =   L"txt_s";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_XML_S =   L"xml_s";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_FONT_S =  L"ef__s";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_FPAGE_S =   L"fpage_s";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_TTF_S =     L"ttf_s";
_declspec(selectany) const wchar_t* const DWFMIME::kzExtension_ODTTF_S =   L"odttf_s";
//DNT_END

#endif
#endif
#endif


}

#endif
