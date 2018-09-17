//
//  Copyright (c) 1996-2006 by Autodesk, Inc.
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

#ifndef _DWFW3DTK_CAMERA_H
#define _DWFW3DTK_CAMERA_H


///
///\file        dwf/w3dtk/W3DCamera.h
///\brief       This file contains the W3DCamera class declaration.
///


#include "dwf/w3dtk/BStream.h"

///
///\interface   W3DCamera   dwf/w3dtk/W3DCamera.h     "dwf/w3dtk/W3DCamera.h"
///\brief       HSF independent camera class
///\since       1.0.1000
///
class BBINFILETK_API W3DCamera
{

public:

    typedef enum
    {
        eOrthographic,
        ePerspective
    } teProjection;

public:

    ///
    ///         Constructor
    ///
    W3DCamera()
        throw();

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
    W3DCamera( float nPositionX,    float nPositionY,   float nPositionZ,
               float nTargetX,      float nTargetY,     float nTargetZ,
               float nUpVectorX,    float nUpVectorY,   float nUpVectorZ,
               float nFieldWidth,   float nFieldHeight,
               teProjection eProjection )
       throw();

    ///
    ///         Destructor
    ///
    virtual ~W3DCamera()
        throw();

    ///
    ///         Copy Constructor
    ///
    W3DCamera( const W3DCamera& rCamera )
        throw();

    ///
    ///         Assignment Operator
    ///
    W3DCamera& operator=( const W3DCamera& rCamera )
        throw();

    ///
    ///         Change camera's position
    ///
    void setPosition( float nPositionX,
                      float nPositionY,
                      float nPositionZ )
        throw();

    ///
    ///         Get camera's position
    ///
    const float* getPosition( float anPosition[3] ) const
        throw();

    ///
    ///         Change camera's target
    ///
    void setTarget( float nTargetX,
                    float nTargetY,
                    float nTargetZ )
        throw();

    ///
    ///         Get camera's target
    ///
    const float* getTarget( float anTarget[3] ) const
        throw();

    ///
    ///         Change camera's up-vector
    ///
    void setUpVector( float nUpVectorX,
                      float nUpVectorY,
                      float nUpVectorZ )
        throw();

    ///
    ///         Get camera's up-vector
    ///
    const float* getUpVector( float anUpVector[3] ) const
        throw();

    ///
    ///         Change camera's field
    ///
    void setField( float nFieldWidth,
                   float nFieldHeight )
        throw();

    ///
    ///         Get camera's field
    ///
    const float* getField( float anField[2] ) const
        throw();

    ///
    ///         Change camera's projection
    ///
    void setProjection( teProjection eProjection )
        throw();

    ///
    ///         Get camera's projection
    ///
    teProjection getProjection() const
        throw();

private:

    float           _nPositionX;
    float           _nPositionY;
    float           _nPositionZ;
    float           _nTargetX;
    float           _nTargetY;
    float           _nTargetZ;
    float           _nUpVectorX;
    float           _nUpVectorY;
    float           _nUpVectorZ;
    float           _nFieldWidth;
    float           _nFieldHeight;
    teProjection    _eProjection;
};



#endif
