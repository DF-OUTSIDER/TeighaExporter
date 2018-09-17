//
//  Copyright (c) 2006 by Autodesk, Inc.
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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/xps/Constants.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//

#ifndef _DWFTK_XPSCONSTANTS_H
#define _DWFTK_XPSCONSTANTS_H


///
///\file        dwf/xps/Constants.h
///\brief       This file contains all XPS XML string declarations.
///


#include "dwfcore/Core.h"
#include "dwf/Toolkit.h"


///
///\ingroup     xps
///
///\class       XPSXML  dwf/xps/Constants.h     "dwf/xps/Constants.h"
///\brief       This class scopes the XPS XML strings known to the toolkit.
///\since       7.4.0
///
class XPSXML
{

public:

    ///
    ///\brief   "http://schemas.microsoft.com/xps/2005/06"
    ///
    static const char* const kzNamespaceURI_XPS;

    ///
    ///\brief   "FixedDocumentSequence"
    ///
    static const char* const kzElement_FixedDocumentSequence;
    ///
    ///\brief   "DocumentReference"
    ///
    static const char* const kzElement_DocumentReference;
    ///
    ///\brief   "FixedDocument"
    ///
    static const char* const kzElement_FixedDocument;
    ///
    ///\brief   "PageContent"
    ///
    static const char* const kzElement_PageContent;
    ///
    ///\brief   "PageContent.LinkTargets"
    ///
    static const char* const kzElement_PageContentLinkTargets;
    ///
    ///\brief   "LinkTarget"
    ///
    static const char* const kzElement_LinkTarget;
    ///
    ///\brief   "FixedPage"
    ///
    static const char* const kzElement_FixedPage;
    ///
    ///\brief   "Canvas"
    ///
    static const char* const kzElement_Canvas;
    ///
    ///\brief   "ImageBrush"
    ///
    static const char* const kzElement_ImageBrush;
    ///
    ///\brief   "Path"
    ///
    static const char* const kzElement_Path;
    ///
    ///\brief   "Path.Fill"
    ///
    static const char* const kzElement_Path_Fill;

    ///
    ///\brief   "Source"
    ///
    static const char* const kzAttribute_Source;
    ///
    ///\brief   "Width"
    ///
    static const char* const kzAttribute_Width;
    ///
    ///\brief   "Height"
    ///
    static const char* const kzAttribute_Height;
    ///
    ///\brief   "Name"
    ///
    static const char* const kzAttribute_Name;
    ///
    ///\brief "Data"
    ///
    static const char* const kzAttribute_Data;
    ///
    ///\brief "ImageSource"
    ///
    static const char* const kzAttribute_ImageSource;
    ///
    ///\brief "TileMode"
    ///
    static const char* const kzAttribute_TileMode;
    ///
    ///\brief "Viewbox"
    ///
    static const char* const kzAttribute_Viewbox;
    ///
    ///\brief "ViewboxUnits"
    ///
    static const char* const kzAttribute_ViewboxUnits;
    ///
    ///\brief "Viewport"
    ///
    static const char* const kzAttribute_Viewport;
    ///
    ///\brief "ViewportUnits"
    ///
    static const char* const kzAttribute_ViewportUnits;
    ///
    ///\brief "RenderTransform"
    ///
    static const char* const kzAttribute_RenderTransform;
    ///
    ///\brief "Fill"
    ///
    static const char* const kzAttribute_Fill;
    ///
    ///\brief "xml:lang"
    ///
    static const char* const kzAttribute_Language;

    ///
    ///\brief   "http://schemas.microsoft.com/xps/2005/06/required-resource"
    ///
    static const char* const kzRelationship_RequiredResource;
    ///
    ///\brief   "http://schemas.microsoft.com/xps/2005/06/fixedrepresentation"
    ///
    static const char* const kzRelationship_FixedRepresentation;
    ///
    ///\brief   "http://schemas.microsoft.com/xps/2005/06/restricted-font"
    ///
    static const char* const kzRelationship_RestrictedFont;
    ///
    ///\brief   "http://schemas.microsoft.com/xps/2005/06/signature-definitions"
    ///
    static const char* const kzRelationship_DigitalSignatureDefinitions;
    ///
    ///\brief   "http://schemas.microsoft.com/xps/2005/06/discard-control"
    ///
    static const char* const kzRelationship_DiscardControl;
    ///
    ///\brief   "http://schemas.microsoft.com/xps/2005/06/documentstructure"
    ///
    static const char* const kzRelationship_DocumentStructure;
    ///
    ///\brief   "http://schemas.microsoft.com/xps/2005/06/printticket"
    ///
    static const char* const kzRelationship_PrintTicket;
    ///
    ///\brief   "http://schemas.microsoft.com/xps/2005/06/storyfragments"
    ///
    static const char* const kzRelationship_StoryFragments;

};

    //
    // required for win32 dll external linkage
    //
#ifdef  _DWFCORE_WIN32_SYSTEM
#ifndef DWFTK_STATIC

//DNT_Start
_declspec(selectany) const char* const XPSXML::kzNamespaceURI_XPS = "http://schemas.microsoft.com/xps/2005/06";

_declspec(selectany) const char* const XPSXML::kzElement_FixedDocumentSequence  = "FixedDocumentSequence";
_declspec(selectany) const char* const XPSXML::kzElement_DocumentReference      = "DocumentReference";
_declspec(selectany) const char* const XPSXML::kzElement_FixedDocument          = "FixedDocument";
_declspec(selectany) const char* const XPSXML::kzElement_PageContent            = "PageContent";
_declspec(selectany) const char* const XPSXML::kzElement_PageContentLinkTargets = "PageContent.LinkTargets";
_declspec(selectany) const char* const XPSXML::kzElement_LinkTarget             = "LinkTarget";
_declspec(selectany) const char* const XPSXML::kzElement_FixedPage              = "FixedPage";
_declspec(selectany) const char* const XPSXML::kzElement_Canvas                 = "Canvas";
_declspec(selectany) const char* const XPSXML::kzElement_ImageBrush             = "ImageBrush";
_declspec(selectany) const char* const XPSXML::kzElement_Path                   = "Path";
_declspec(selectany) const char* const XPSXML::kzElement_Path_Fill              = "Path.Fill";

_declspec(selectany) const char* const XPSXML::kzAttribute_Source               = "Source";
_declspec(selectany) const char* const XPSXML::kzAttribute_Width                = "Width";
_declspec(selectany) const char* const XPSXML::kzAttribute_Height               = "Height";
_declspec(selectany) const char* const XPSXML::kzAttribute_Name                 = "Name";
_declspec(selectany) const char* const XPSXML::kzAttribute_Data                 = "Data";
_declspec(selectany) const char* const XPSXML::kzAttribute_ImageSource          = "ImageSource";
_declspec(selectany) const char* const XPSXML::kzAttribute_TileMode             = "TileMode";
_declspec(selectany) const char* const XPSXML::kzAttribute_Viewbox              = "Viewbox";
_declspec(selectany) const char* const XPSXML::kzAttribute_ViewboxUnits         = "ViewboxUnits";
_declspec(selectany) const char* const XPSXML::kzAttribute_Viewport             = "Viewport";
_declspec(selectany) const char* const XPSXML::kzAttribute_ViewportUnits        = "ViewportUnits";
_declspec(selectany) const char* const XPSXML::kzAttribute_RenderTransform      = "RenderTransform";
_declspec(selectany) const char* const XPSXML::kzAttribute_Fill                 = "Fill";

_declspec(selectany) const char* const XPSXML::kzAttribute_Language             = "xml:lang";

_declspec(selectany) const char* const XPSXML::kzRelationship_RequiredResource          = "http://schemas.microsoft.com/xps/2005/06/required-resource";
_declspec(selectany) const char* const XPSXML::kzRelationship_FixedRepresentation       = "http://schemas.microsoft.com/xps/2005/06/fixedrepresentation";
_declspec(selectany) const char* const XPSXML::kzRelationship_RestrictedFont            = "http://schemas.microsoft.com/xps/2005/06/restricted-font";
_declspec(selectany) const char* const XPSXML::kzRelationship_DigitalSignatureDefinitions = "http://schemas.microsoft.com/xps/2005/06/signature-definitions";
_declspec(selectany) const char* const XPSXML::kzRelationship_DiscardControl            = "http://schemas.microsoft.com/xps/2005/06/discard-control";
_declspec(selectany) const char* const XPSXML::kzRelationship_DocumentStructure         = "http://schemas.microsoft.com/xps/2005/06/documentstructure";
_declspec(selectany) const char* const XPSXML::kzRelationship_PrintTicket               = "http://schemas.microsoft.com/xps/2005/06/printticket";
_declspec(selectany) const char* const XPSXML::kzRelationship_StoryFragments            = "http://schemas.microsoft.com/xps/2005/06/storyfragments";
//DNT_End


#endif
#endif

#endif
