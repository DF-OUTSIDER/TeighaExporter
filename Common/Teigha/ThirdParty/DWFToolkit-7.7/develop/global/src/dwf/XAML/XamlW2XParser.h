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
#if !defined XAML_W2X_PARSER
#define XAML_W2X_PARSER

#include "XAML/pch.h"
#include "XAML/XamlClassFactory.h"
#include "XAML/XamlXML.h"
#include "dwfcore/XMLParser.h"

class WT_XAML_W2X_Parser : public DWFXMLCallback
{
public:
    WT_XAML_W2X_Parser(WT_XAML_File& file) throw(WT_Result);
    virtual ~WT_XAML_W2X_Parser() throw();

    WT_XAML_File&            xamlFile() { return _rXamlFile; }
    WT_XAML_Class_Factory*   xamlClassFactory() { return _pClassFactory; }
    DWFXMLParser*            w2xParser() { return _pW2XParser; }

    WT_Result                parseW2X();

    bool                     isParsingFinished() { return w2xParser()->isParsingFinished(); }
    int                      insideMacroDraw() { return _nInsideMacroDraw; }

protected:
    WT_Result                Create_Object_Shell(const char* pcName);
    WT_Result                Create_Named_View_List_Shell();
    WT_Result                Create_Named_View_In_List();
    WT_Result                Create_URL_List_Shell();
    WT_Result                Create_Attribute_URL_List_Shell();
    WT_Result                Create_URL_Item_In_List();
    WT_Result                Create_Attribute_URL_Item_In_List();
    WT_Result                Create_Image_Color_Map();
    WT_Result                Create_Image_Shell();
    WT_Result                Create_PNG_Group4_Image_Shell();
    WT_Result                Create_Polymarker_Shell();
    WT_Result                Create_User_Hatch_Pattern_Shell();
    WT_Result                Create_Hatch_Pattern_In_User_Hatch_Pattern();
    WT_Result                Create_DWF_Header();
    WT_Result                Create_Polygon_Shell();    

private:  
    // DWFXMLCallback handlers here
    void notifyStartElement( const char* zName, const char** ppAttributeList ) throw();
    void notifyEndElement( const char* zName ) throw();
    void notifyCharacterData( const char* zCData, int nLength ) throw();
    void notifyStartNamespace( const char* zPrefix, const char* zURI ) throw();
    void notifyEndNamespace( const char* zPrefix ) throw();

    void _processStartElement(void) throw();
    void _processEndElement(void) throw();

private:
    //list kind of object
    WT_Object*                   _pListKindOfObject;
    DWFXMLParser*                _pW2XParser;
    WT_XAML_File&                _rXamlFile;
    XamlXML::tAttributeMap       _oMap;
    WT_XAML_Class_Factory*       _pClassFactory;
    int                          _nLastIndex;
    WT_String                    _zCurrentElementName;
    bool                         _bPendingElementEnded;
    bool                         _bSuspended;
    bool                         _bAttributeURLStarted;
    int                          _nInsideMacroDraw;
};

#endif //XAML_W2X_PARSER
