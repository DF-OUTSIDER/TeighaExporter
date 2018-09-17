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
#if !defined XAML_RESOURCE_DICTIONARY_PARSER_HEADER
#define XAML_RESOURCE_DICTIONARY_PARSER_HEADER

#include "XAML/pch.h"
#include "XAML/XamlXML.h"
#include "dwfcore/XMLParser.h"

class WT_XAML_Resource_Dictionary_Parser : public DWFXMLCallback
{

public:
    WT_XAML_Resource_Dictionary_Parser(WT_XAML_File& file) throw(WT_Result);
    virtual ~WT_XAML_Resource_Dictionary_Parser() throw();

    WT_XAML_File*&            xamlFile() { return _pXamlFile; }
    DWFXMLParser*            dictionaryParser() { return _pDictionaryParser; }

    WT_Result                parseResource();

    bool                     isParsingFinished() { return dictionaryParser()->isParsingFinished(); }


private:  
    // DWFXMLCallback handlers here
    _DWFTK_API void notifyStartElement( const char* zName, const char** ppAttributeList ) throw();
    _DWFTK_API void notifyEndElement( const char* zName ) throw();
    _DWFTK_API void notifyCharacterData( const char* zCData, int nLength ) throw();
    _DWFTK_API void notifyStartNamespace( const char* zPrefix, const char* zURI ) throw();
    _DWFTK_API void notifyEndNamespace( const char* zPrefix ) throw();

   /* _DWFTK_API void _processStartElement(void) throw();
    _DWFTK_API void _processEndElement(void) throw();*/

private:
    DWFXMLParser*               _pDictionaryParser;
    WT_XAML_File*               _pXamlFile;


};
#endif //XAML_RESOURCE_DICTIONARY_PARSER_HEADER
