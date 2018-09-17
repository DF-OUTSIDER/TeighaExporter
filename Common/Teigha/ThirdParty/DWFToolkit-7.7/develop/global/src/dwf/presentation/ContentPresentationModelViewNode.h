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

//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/presentation/ContentPresentationModelViewNode.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $

#ifndef _DWFTK_CONTENT_PRESENTATION_VIEW_NODE_H
#define _DWFTK_CONTENT_PRESENTATION_VIEW_NODE_H


///
///\file        dwf/presentation/ContentPresentationModelViewNode.h
///\brief       This file contains the DWFPresentationViewNode class declaration.
///

#include "dwfcore/String.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"
#include "dwf/w3dtk/BOpcodeHandler.h"
#include "dwf/presentation/ContentPresentationNode.h"
#include "dwf/presentation/ContentPresentationReferenceNode.h"
#include "dwf/presentation/ModelScene.h"


namespace DWFToolkit
{

///
///\ingroup     dwfpresentation
///
///\class       DWFContentPresentationModelViewNode     dwf/package/ContentPresentationModelViewNode.h      "dwf/package/ContentPresentationModelViewNode.h"
///\brief       The DWFContentPresentationModelViewNode class supports scene changes to a 3D model.
///\since       7.2
///
///             The DWFContentPresentationModelViewNode class supports scene changes to a 3D model.
///

class DWFContentPresentationModelViewNode : public  DWFContentPresentationReferenceNode
                                          , public  DWFModelScene 
                                          _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///         Constructor
    ///
    ///\param   zLabel  An optional user-friendly label for this node.
    ///\param   zID     A unique string identifying this node. If not provided, one will be assigned.
    ///\throw   None
    ///
    _DWFTK_API
    DWFContentPresentationModelViewNode( const DWFString& zLabel = /*NOXLATE*/L"",
                                         const DWFString& zID = /*NOXLATE*/L"" )
        throw();

    ///
    ///         Destructor
    ///
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFContentPresentationModelViewNode()
        throw();

    ///
    ///         This call cause the scene changes captured by this node to be
    ///         "played back" on the interface implementation that's passed in.
    ///         The final change will be followed by \a close().
    ///
    ///\param   rScene          An implementation of DWFModelScene interface
    ///                         that will receive the scene changes.
    ///\throw   None
    ///
    _DWFTK_API
    virtual void getModelSceneChanges( DWFModelScene& rScene )
        throw( DWFException );

    ///
    ///         Returns the bitwise-or of the change types that must be
    ///         reset before applying this node.
    ///
    ///\return  The bitwise-or of scene change types that must be reset
    ///\throw   None
    ///
    _DWFTK_API
    virtual unsigned int getResetFlags() const
        throw( DWFException );

    ///
    ///\copydoc BaseOpcodeHandlerObserver::notify
    ///
    _DWFTK_API
    virtual void notify( BBaseOpcodeHandler* pHandler,
                         const void*         pTag = NULL )
        throw( DWFException );

    ///
    ///         This method will copy scene changes recorded at the node
    ///         that's passed in.
    ///
    ///\param   rNode       The node whose changes are to be copied.
    ///
    _DWFTK_API
    virtual void insertSceneChanges( DWFContentPresentationModelViewNode& rNode )
        throw( DWFException );

    ///
    ///         Sets a bitwise-or of the scene change types that must be
    ///         reset before applying this node.
    ///
    ///\param   nFlags      The bitwise-or of changes types to reset
    ///\throw   None
    ///
    _DWFTK_API
    virtual void setResetFlags( unsigned int nFlags )
        throw( DWFException );

    ///
    ///\copydoc DWFCore::DWFXMLBuildable::parseAttributeList()
    ///
    _DWFTK_API
    virtual void parseAttributeList( const char** ppAttributeList )
        throw( DWFException );

#ifndef DWFTK_READ_ONLY

    ///
    ///\copydoc DWFXMLSerializable::addNamespace()
    ///
    _DWFTK_API
    const DWFXMLNamespace& addNamespace( const DWFString& zNamespace, 
                                         const DWFString& zXMLNS )
        throw( DWFException );

    ///
    ///\copydoc DWFXMLSerializable::serializeXML()
    ///
    _DWFTK_API
    virtual void serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
        throw( DWFException );

private:

    void serializeAttributes( DWFXMLSerializer& rSerializer, unsigned int nFlags )
        throw( DWFException );

private:

    typedef std::map<DWFString, DWFXMLNamespace>    _tNamespaceMap;
    _tNamespaceMap                                  _oAddedNamespaces;

#endif

private:

    void _preProcessHandler (DWFModelSceneChangeHandler* pHandler)
        throw( DWFException );

private:

    unsigned int            _nResetFlags;
    bool                    _bInstancePreviouslyOpen;
    DWFString               _zPreviousInstanceID;
};

}

#endif
