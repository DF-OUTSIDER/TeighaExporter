//
//  Copyright (c) 2007 by Autodesk, Inc.
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


#ifndef _DWFTK_CONTENT_CAMERA_CONTAINER_H
#define _DWFTK_CONTENT_CAMERA_CONTAINER_H


///
///\file        dwf/presentation/utility/DWF3DCameraContainer.h
///\brief       This file contains the DWF3DCameraContainer class declaration.
///

#include "dwfcore/STL.h"
#include "dwfcore/Vector.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"

namespace DWFToolkit
{

//
// fwd decl
//
class DWF3DCamera;

///
///\ingroup     dwfpackage
///
///\class       DWF3DCameraContainer   dwf/package/utility/DWF3DCameraContainer.h     "dwf/package/utility/DWF3DCameraContainer.h"
///\brief       An collection utility for DWF3DCamera objects.
///\since       7.5
///
///             This class is a dwf3dcamera owner.
///
class DWF3DCameraContainer _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE
{

public:

public:

    ///
    ///         Constructor
    ///
    ///
    ///\throw   None
    ///
    _DWFTK_API
    DWF3DCameraContainer()
        throw();

    ///
    ///         Destructor
    ///
    ///         This method will delete all of the container cameras
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWF3DCameraContainer()
        throw();

    ///
    ///         Inserts a camera to the container.
    ///
    ///\param   camera  The camera to add to the container .
    ///                 A new pointer will be created and owned by the container
    ///                 and will be released with the \b DWFCORE_FREE_OBJECT macro.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void addCamera( DWF3DCamera& camera )
        throw( DWFException );

    ///
    ///         Insert a camera at the given index. If the given index is greater
    ///         than the size of the vector, an exception is thrown. 
    ///
    ///\param   p3DCamera   The camera to add to the container.
    ///                     A new pointer will be created and owned by the container
    ///                     and will be released with the \b DWFCORE_FREE_OBJECT macro.
    ///\param   index       The index of the location to insert it.
    ///\throw   A DWFUnexpectedException is thrown if the index is greater than the size.
    ///
    _DWFTK_API
    virtual void insertAt( DWF3DCamera* p3DCamera, size_t index )
	    throw( DWFException );

    ///
    ///         Locates all cameras in the container
    ///
    ///\return  A pointer to an iterator for enumerating the cameras (may be NULL).  
    ///         This pointer must be deleted by the caller with the \b DWFCORE_FREE_OBJECT
    ///         macro.
    ///         Nodes returned from the iterator are owned by the container 
    ///         and must not be deleted by the caller.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFOrderedVector<DWF3DCamera*>::Iterator* getCameras( )
        throw();

    ///
    ///         Clears the camera from the Container.
    ///
    ///\param   index   the index of the location to remove the camera
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void removeCameraAt( size_t index )
        throw( DWFException );

protected:

    ///
    ///\brief       The main collection of nodes
    ///
    DWFOrderedVector<DWF3DCamera*>      _oCameras;

private:

    DWF3DCameraContainer( const DWF3DCameraContainer& );
    DWF3DCameraContainer& operator=( const DWF3DCameraContainer& );
};

}

#endif
