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


#ifndef _DWFTK_3D_CAMERA_H
#define _DWFTK_3D_CAMERA_H


#include "dwf/w3dtk/W3DCamera.h"

#include "dwf/package/XML.h"
#include "dwf/Toolkit.h"

namespace DWFToolkit
{

///
///\ingroup     DWF3DCamera
///
///\class       DWF3DCamera   dwf/presentation/DWF3DCamera.h     "dwf/presentation/DWF3DCamera.h"
///\brief       This class convert W3DCamera to a directly-serializable one.
///\since       7.5
///

class DWF3DCamera : public W3DCamera
#ifndef DWFTK_READ_ONLY
                  , public DWFXMLSerializable
#endif
				  _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///         Constructor
    ///
    _DWFTK_API
    DWF3DCamera()
	   throw()
        : W3DCamera()		
    {
        ;
    }

	///
	///         Constructor
	///			For DWF3DCamera doesn't have any members of its own, if a W3DCemera will be 
	///			convertd to DWF3DCamera, it's the best way to use this copy constructor
	///
	///\param   rCamera     A W3DCamera which be assigned.
	///\throw   None
	///
    _DWFTK_API
    DWF3DCamera(const W3DCamera& rCamera)
	   throw()
		: W3DCamera( rCamera )		        
    {
        ;
    }

    ///
    ///         Constructor
    ///
    ///\param   nPositionX
    ///\param   nPositionY
    ///\param   nPositionZ
    ///\param   nTargetX
    ///\param   nTargetY
    ///\param   nTargetZ
    ///\param   nUpVectorX
    ///\param   nUpVectorY
    ///\param   nUpVectorZ
    ///\param   nFieldWidth
    ///\param   nFieldHeight
    ///\param   eProjection
    ///
	_DWFTK_API
	DWF3DCamera( float nPositionX,    float nPositionY,   float nPositionZ,
				 float nTargetX,      float nTargetY,     float nTargetZ,
				 float nUpVectorX,    float nUpVectorY,   float nUpVectorZ,
				 float nFieldWidth,   float nFieldHeight, teProjection eProjection )
	   throw()
	 : W3DCamera( nPositionX,    nPositionY,   nPositionZ,
				  nTargetX,      nTargetY,     nTargetZ,
				  nUpVectorX,    nUpVectorY,   nUpVectorZ,
				  nFieldWidth,   nFieldHeight, eProjection )		
	{
		;
	}

    ///
    ///         Copy Constructor
	///
	///\param   rCamera     A DWF3DCamera which be assigned.
	///\throw   None
    ///
    _DWFTK_API
    DWF3DCamera( const DWF3DCamera& rCamera ) 
	    throw()
         : W3DCamera( rCamera )
    { 
         ; 
    }

    ///
    ///         Assignment Operator
	///
	///\param   rCamera     A DWF3DCamera which be assigned.
	///\throw   None
    ///
    _DWFTK_API
    DWF3DCamera& operator=( const DWF3DCamera& rCamera ) 
		throw()
    { 
        W3DCamera::operator=(rCamera); 
        return *this; 
    }

    ///
    ///         Destructor
    ///
    _DWFTK_API
    virtual ~DWF3DCamera()
        throw()
    {
        ;
    }

#ifndef DWFTK_READ_ONLY

    ///
    ///\copydoc DWFXMLSerializable::serializeXML()
    ///
    _DWFTK_API
    virtual void serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
        throw( DWFException );

#endif
};

}

#endif
