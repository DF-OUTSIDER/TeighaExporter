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
///
///\file        XAML/XamlObjectFactory.h
///

#ifndef _XAML_OBJECT_FACTORY_H
#define _XAML_OBJECT_FACTORY_H


class XamlObjectFactory
{
    friend class WT_XAML_Text;

private:
    XamlObjectFactory() {}

public:
    static WT_Result processAttributes( WT_XAML_File& rFile, XamlDrawable* pDrawable );
    static WT_Result processObjects( WT_XAML_File& rFile, XamlDrawable* pDrawable );

protected:
    static WT_Result insertObject( WT_XAML_File& rFile, WT_Object *pObject, WT_Class_Factory& rClassFactory );
    static WT_Object* findPendingObject( WT_XAML_File& rFile, WT_Object::WT_Type eType, WT_Object::WT_ID eID );

private:
    static WT_Result _processPathAttributes( WT_XAML_File& rFile, XamlPath* pPath );
    static WT_Result _processPathObjects( WT_XAML_File& rFile, XamlPath* pPath );

    static WT_Result _processCanvasAttributes( WT_XAML_File& rFile, XamlCanvas* pPath );
    static WT_Result _processCanvasObjects( WT_XAML_File& rFile, XamlCanvas* pPath );

    static WT_Result _processGlyphsAttributes( WT_XAML_File& rFile, XamlGlyphs* pPath );
    static WT_Result _processGlyphsObjects( WT_XAML_File& rFile, XamlGlyphs* pPath );

    static WT_Color* _createOptimizedColor( WT_XAML_File& rFile, WT_RGBA32 &rgba );
};


#endif //_XAML_OBJECT_FACTORY_H

