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
#if !defined XAML_XAML_PARSER
#define XAML_XAML_PARSER

#include "XAML/pch.h"
#include "dwfcore/XMLParser.h"
#include <stack>

class WT_XAML_Xaml_Parser : public DWFXMLCallback
{
    enum eViewportState
    {
        None = 0,
        Inside
    };

public:
    typedef std::stack < XamlDrawable* >  tDrawableStack;
    typedef std::stack < XamlXML::tAttributeMap* > tAttributeMapStack;

public:

    WT_XAML_Xaml_Parser(WT_XAML_File& file) throw(WT_Result);
    virtual ~WT_XAML_Xaml_Parser() throw();

    WT_XAML_File&            xamlFile() { return _rXamlFile; }
    DWFXMLParser*            xamlParser() { return _pXamlParser; }

    WT_Result                parseXaml();

    bool                     isParsingFinished() { return (xamlParser() ?  xamlParser()->isParsingFinished() : true); }

    const XamlCanvas*        viewportCanvas() const { return _pViewportCanvas; }

private:  
    // DWFXMLCallback handlers here
    void notifyStartElement( const char* zName, const char** ppAttributeList ) throw();
    void notifyEndElement( const char* zName ) throw();
    void notifyCharacterData( const char* zCData, int nLength ) throw();
    void notifyStartNamespace( const char* zPrefix, const char* zURI ) throw();
    void notifyEndNamespace( const char* zPrefix ) throw();

    void _processStartElement(void) throw(DWFException);
    void _processEndElement(void) throw();
    const wchar_t* _getName( XamlDrawable* pDrawable );

private:
    DWFXMLParser*               _pXamlParser;
    WT_XAML_File&               _rXamlFile;
    XamlXML::tAttributeMap      _oMap;
    XamlCanvas*                 _pViewportCanvas;
    tDrawableStack              _oDrawables;
    int                         _nLastIndex;
    WT_String                   _zCurrentElementName;
    bool                        _bPendingElementEnded;
    bool                        _bSuspended;
};

#endif //XAML_XAML_PARSER
