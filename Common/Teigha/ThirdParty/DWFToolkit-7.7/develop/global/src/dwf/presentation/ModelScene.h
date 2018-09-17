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


#ifndef _DWFTK_MODEL_SCENE_H
#define _DWFTK_MODEL_SCENE_H


///
///\file        dwf/presentation/ModelScene.h
///\brief       This file contains the DWFModelScene class declaration.
///

#include "dwfcore/STL.h"
#include "dwfcore/Vector.h"
#include "dwfcore/String.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"
#include "dwf/package/XML.h"
#include "dwf/w3dtk/BOpcodeHandler.h"
#include "dwf/w3dtk/W3DCamera.h"
#include "dwf/publisher/model/AttributeHandlerBuilder.h"
#include "dwf/publisher/model/GeometryHandlerBuilder.h"
#include "dwf/publisher/model/FeatureHandlerBuilder.h"

//
// fwd decl
//
class TK_Color;
class TK_Color_RGB;
class TK_Cutting_Plane;
class TK_Matrix;
class TK_Visibility;
class W3DCamera;

namespace DWFToolkit
{

//
// fwd decl
//
class DWFInstance;

///
///\ingroup     dwfpresentation
///
///\class       DWFModelSceneChangeHandler   dwf/presentation/ModelScene.h     "dwf/presentation/ModelScene.h"
///\brief       
///\since       7.2
///
class DWFModelSceneChangeHandler
#ifndef DWFTK_READ_ONLY
                    : public DWFXMLSerializable
#endif
{
public:
    ///
    ///\ingroup     dwfpresentation
    ///
    ///\class       UserAttribute   dwf/presentation/ModelScene.h     "dwf/presentation/ModelScene.h"
    ///\brief       This class holds user attributes that can be specified with any model scene change handler.
    ///\since       7.2
    ///
    class UserAttribute
    {
    public:
        ///
        ///\brief   This type defines a list of UserAttribute pointers.
        ///
        typedef DWFOrderedVector<UserAttribute*>            tList;

    public:
        UserAttribute( const DWFString& zName,
                        const DWFString& zValue,
                        const DWFString& zNamespace )
                        : _zName( zName )
                        , _zValue( zValue )
                        , _zNamespace( zNamespace )
        {
            ;
        }

    public:
        ///
        ///         Gets the name of this user attribute.
        ///
        ///\return  The name of this user attribute.
        ///\throw   none
        ///
        const DWFString& name() const
            throw()
        {
            return _zName;
        }

        ///
        ///         Gets the value of this user attribute.
        ///
        ///\return  The value of this user attribute.
        ///\throw   none
        ///
        const DWFString& value() const
            throw()
        {
            return _zValue;
        }

        ///
        ///         Gets the namespace of this user attribute.
        ///
        ///\return  The namespace of this user attribute.
        ///\throw   none
        ///
        const DWFString& nameSpace() const
            throw()
        {
            return _zNamespace;
        }

    private:
        DWFString   _zName;
        DWFString   _zValue;
        DWFString   _zNamespace;
    };

    UserAttribute::tList  _oUserAttributes;

    ///
    ///\enum    teChangeType
    ///\brief   Enumeration values that define the various attributes that can
    ///         be changed on a model scene.
    ///         
    ///
    typedef enum
    {
        ///
        ///     Change that applies to the entire scene
        ///
        eSceneLevel,

        ///
        ///     Change that applies to all instances
        ///
        eInstanceLevel
    } teChangeType;

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
	_DWFTK_API
    virtual ~DWFModelSceneChangeHandler()
        throw();
    
    ///
    ///         clone
    ///
    ///\return  A type specific clone
    ///\throw   none
    ///
    virtual DWFModelSceneChangeHandler* clone () const
        throw() = 0;

    ///
    ///         The change type.
    ///
    ///\return  The type indicating whether this is a scene level of instance level change.
    ///\throw   none
    ///
    virtual teChangeType changeType () const
        throw() = 0;
    
    ///
    ///         Adds a user attribute for this change
    ///
    ///         User attributes can be added to any change handler. All user attributes must
    ///         be contained in a user specified namespace.
    ///         
    ///
    ///\param   zName           The name of the attribute.
    ///\param   zValue          The value of the attribute.
    ///\param   zNamespace      The namespace to add to the attribute.
    ///\throw   DWFException
    ///
	_DWFTK_API
    virtual void addUserAttribute( const DWFString& zName,
                                   const DWFString& zValue,
                                   const DWFString& zNamespace )
    throw( DWFException );
    
    ///
    ///         Gets all user attributes associates with this change handler.
    ///
    ///\return  A pointer to an iterator for enumerating the user attributes (may be NULL).  
    ///         This pointer must be deleted by the caller with the \b DWFCORE_FREE_OBJECT
    ///         macro.
    ///         User attributes returned from the iterator are owned by the change handler 
    ///         and must not be deleted by the caller.
    ///\throw   none
    ///
    virtual UserAttribute::tList::Iterator* getUserAttributes()
        throw()
    {
        return _oUserAttributes.iterator();
    }

#ifndef DWFTK_READ_ONLY
    ///
    ///\copydoc DWFXMLSerializable::serializeXML()
    ///
	_DWFTK_API
    void serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
        throw( DWFException );
#endif

private:
};


///
///\ingroup     dwfpresentation
///
///\class       DWFModelScene   dwf/package/ModelScene.h     "dwf/package/ModelScene.h"
///\brief       
///\since       7.2
///
class DWFModelScene : public DWFAttributeHandlerBuilder
                    , public DWFGeometryHandlerBuilder
                    , public DWFFeatureHandlerBuilder
                    , public BaseOpcodeHandlerObserver
#ifndef DWFTK_READ_ONLY
                    , public DWFXMLSerializable
#endif
                    _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///\enum    teAttributeType
    ///\brief   Enumeration values that define the various attributes that can
    ///         be changed on a model scene.
    ///         
    ///
    typedef enum
    {
        ///
        ///     Camera
        ///
        eCamera                 = 0x0001,

        ///
        ///     Color
        ///
        eColor                  = 0x0002,

        ///
        ///     Cutting Plane
        ///
        eCuttingPlane           = 0x0004,

        ///
        ///      Modelling Matrix
        ///
        eModellingMatrix        = 0x0008,

        ///
        ///     Texture Matrix
        ///
        eTextureMatrix          = 0x0010,

        ///
        ///     Visibility
        ///
        eVisibility             = 0x0020,

        ///
        ///     InstanceVisibility
        ///
        eInstanceVisibility     = 0x0040,

        ///
        ///     InstanceTransparency
        ///
        eInstanceTransparency   = 0x0080,

        ///
        ///     Geometric Variation
        ///
        eGeometricVariation     = 0x0100,

        ///
        ///     Display Mode
        ///
        eDisplayMode            = 0x0200
    } teAttributeType;

    ///
    ///\enum    teDisplayMode
    ///\brief   Enumeration values that define the display modes that can
    ///         set on the model scene. The display modes can be combined
    ///         to create new display modes.
    ///         
    ///
    typedef enum
    {
        ///
        /// Shaded
        ///
        eShaded = 0x01,
        ///
        /// Edges
        eEdges =  0x02

    } teDisplayMode;

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    DWFModelScene()
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFModelScene()
        throw();

    ///
    ///         Sets the instance on which subsequent attribute changes are to be applied.
    ///
    ///\param   rInstance       The instance on which subsequent attribute changes are to be applied.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void setCurrentInstance( const DWFInstance& rInstance )
        throw( DWFException );

    ///
    ///         Sets the id of the instance on which subsequent attribute changes are to be applied.
    ///
    ///\param   zID       The id of the instance on which subsequent attribute changes are to be applied.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void setCurrentInstanceID( const DWFString& zID )
        throw( DWFException );

    ///
    ///         Sets the current view
    ///
    ///\param   rCamera             The camera parameters for the view
    ///\param   bSmoothTransition   Specifies whether the camera transition must be done smoothly or not
    ///\throw   DWFException
    ///
    ///
    _DWFTK_API
    virtual void setCamera( const W3DCamera& rCamera, bool bSmoothTransition )
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getColorHandler()
    ///
    _DWFTK_API
    virtual TK_Color& getColorHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFGeometryHandlerBuilder::getCuttingPlaneHandler()
    ///
    _DWFTK_API
    virtual TK_Cutting_Plane& getCuttingPlaneHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getModellingMatrixHandler()
    ///
    _DWFTK_API
    virtual TK_Matrix& getModellingMatrixHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFFeatureHandlerBuilder::getTextureMatrixHandler()
    ///
    _DWFTK_API
    virtual TK_Matrix& getTextureMatrixHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getVisibilityHandler()
    ///
    _DWFTK_API
    virtual TK_Visibility& getVisibilityHandler()
        throw( DWFException );

    ///
    ///             Sets the index of the geometric variation to activate.
    ///
    ///\param       nIndex          Sets the index of the geometric variation to activate.
    ///\throw       DWFException
    ///
    _DWFTK_API
    virtual void setGeometricVariationIndex( unsigned int nIndex )
        throw( DWFException );

    ///
    ///             Sets the visibility of the current instance
    ///
    ///\param       bVisibility    If \e true turns the visibility of the
    ///                             instance ON, else turns the visibility off.
    ///\throw       DWFException
    ///
    _DWFTK_API
    virtual void setVisibility( bool bVisibility )
        throw( DWFException );

    ///
    ///             Sets the transparency of the current instance
    ///
    ///\param       bTransparency   If \e true turns the transparency of the
    ///                             instance ON, else turns the transparency off.
    ///\throw       DWFException
    ///
    _DWFTK_API
    virtual void setTransparency( bool bTransparency )
        throw( DWFException );

    ///
    ///             This method will cause attributes on this
    ///             segment to override local attributes on child segments.
    ///
    ///\param       eType   The attribute to be locked.
    ///\throw       DWFException
    ///
    _DWFTK_API
    virtual void lockAttribute( teAttributeType eType )
        throw( DWFException );

    ///
    ///                 This method will cause attribute locking to be removed.
    ///
    ///\param           eType   The attribute to be unlocked.
    ///\throw           DWFException
    ///
    _DWFTK_API
    virtual void unlockAttribute( teAttributeType eType )
        throw( DWFException );

    ///
    ///                 Sets the current display mode
    ///
    ///\param           nDisplayMode    A combination of one or more enumeration values that specifies
    ///                                 the display mode.
    ///\throw           DWFException
    ///
    _DWFTK_API
    virtual void setDisplayMode( unsigned int nDisplayMode )
        throw( DWFException );

    ///
    ///         This method initiates model changes. This method must called
    ///         before any scene changes have been recorded.
    ///
    ///\throw   DWFException
    ///
    virtual void open()
        throw( DWFException )
    {
        ;
    }

    ///
    ///         This method completes model changes. This method must called
    ///         after all scene changes have been recorded.
    ///
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void close()
        throw( DWFException );

#ifndef DWFTK_READ_ONLY
    ///
    ///\copydoc DWFXMLSerializable::serializeXML()
    ///
    _DWFTK_API
    virtual void serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
        throw( DWFException );
#endif


protected:

    class _W3DXMLPseudoHandler : public BBaseOpcodeHandler
                               , public DWFModelSceneChangeHandler
    {
    public:
        _W3DXMLPseudoHandler() : BBaseOpcodeHandler( (unsigned char)(-1) ) {;}
        _W3DXMLPseudoHandler( const _W3DXMLPseudoHandler& ) : BBaseOpcodeHandler( (unsigned char)(-1) ) {;}
        virtual ~_W3DXMLPseudoHandler()
            throw()
        {;}

        virtual TK_Status Read (BStreamFileToolkit&) { _DWFCORE_THROW( DWFUnexpectedException, /*NOXLATE*/L"This handler cannot be materialized into the scene graph" ); }
        virtual TK_Status Write (BStreamFileToolkit&) { _DWFCORE_THROW( DWFUnexpectedException, /*NOXLATE*/L"This handler cannot be serialized from the scene graph" ); }
    private:
        _W3DXMLPseudoHandler& operator=( const _W3DXMLPseudoHandler& rHandler );
    };

    class _W3DInstance : public _W3DXMLPseudoHandler
    {
    public:
        _W3DInstance() {;}
        _W3DInstance( const DWFString& zID )
            : _zID( zID )
        {;}
        _W3DInstance( const _W3DInstance& rInstance )
            : _W3DXMLPseudoHandler( rInstance )
            , _zID( rInstance._zID )
        {;}
        virtual ~_W3DInstance()
            throw()
        {;}
        virtual DWFModelSceneChangeHandler* clone () const 
            throw()
        {
            return DWFCORE_ALLOC_OBJECT( _W3DInstance(*this) );
        }

        virtual teChangeType changeType () const
            throw()
        {
            return eSceneLevel;
        }

        const DWFString& id() const { return _zID; }

		_DWFTK_API
        void serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
            throw( DWFException );
    private:
        DWFString       _zID;
    private:
        _W3DInstance& operator=( const _W3DInstance& );
    };

    class _W3DInstanceOptions : public _W3DXMLPseudoHandler
    {
    public:
        _W3DInstanceOptions() 
            : _nVisible( -1 )
            , _nTransparent( -1 )
            , _nVariation( -1 )
        {;}
        _W3DInstanceOptions( const _W3DInstanceOptions& rOptions )
            : _nVisible( rOptions._nVisible )
            , _nTransparent( rOptions._nTransparent )
            , _nVariation( rOptions._nVariation )
        {;}
        virtual ~_W3DInstanceOptions()
            throw()
        {;}
        virtual DWFModelSceneChangeHandler* clone () const
            throw()
        {
            return DWFCORE_ALLOC_OBJECT( _W3DInstanceOptions(*this) );
        }
        virtual teChangeType changeType () const
            throw()
        {
            return eInstanceLevel;
        }

        void show()         { _nVisible = 1; }
        void hide()         { _nVisible = 0; }
        void ghost()        { _nTransparent = 1; }
        void unghost()      { _nTransparent = 0; }
        void vary( int i )  { _nVariation = i; }

        int visible() const { return _nVisible; }
        int transparent() const { return _nTransparent; }
        int variation() const { return _nVariation; }

		_DWFTK_API
        void serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
            throw( DWFException );
    private:
        _W3DInstanceOptions& operator=( const _W3DInstanceOptions& );

    private:
        short   _nVisible;
        short   _nTransparent;
        int     _nVariation;
    };

    class _W3DAttributeLock : public _W3DXMLPseudoHandler
    {
    public:
        _W3DAttributeLock( DWFModelScene::teAttributeType eAttribute ) 
            : _bLock( true )
            , _eAttribute( eAttribute )
        {;}
        _W3DAttributeLock( const _W3DAttributeLock& rLock )
            : _bLock( rLock._bLock )
            , _eAttribute( rLock._eAttribute )
        {;}
        virtual ~_W3DAttributeLock()
            throw()
        {;}
        virtual DWFModelSceneChangeHandler* clone () const
            throw()
        {
            return DWFCORE_ALLOC_OBJECT( _W3DAttributeLock(*this) );
        }
        virtual teChangeType changeType () const
            throw()
        {
            return eInstanceLevel;
        }

        void lock() { _bLock = true; }
        void unlock() { _bLock = false; }

        bool locked() const { return (_bLock == true); }
        DWFModelScene::teAttributeType attribute() const { return _eAttribute; }

		_DWFTK_API
        void serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
            throw( DWFException );
    private:
        _W3DAttributeLock& operator=( const _W3DAttributeLock& );

    private:
        bool                            _bLock;
        DWFModelScene::teAttributeType  _eAttribute;
    };

    class _W3DCamera : public W3DCamera
                     , public _W3DXMLPseudoHandler
    {
    public:
        _W3DCamera()
            : W3DCamera()
            , _bSmoothTransition( false )
        {;}
        _W3DCamera( const W3DCamera& rCamera, bool bSmoothTransition )
            : W3DCamera( rCamera )
            , _bSmoothTransition( bSmoothTransition )
        {;}
        _W3DCamera( const _W3DCamera& rCamera ) 
            : W3DCamera( rCamera )
            , _bSmoothTransition( rCamera._bSmoothTransition) 
        {;}
        _W3DCamera& operator=( const _W3DCamera& rCamera ) { W3DCamera::operator=(rCamera); return *this; }
        virtual ~_W3DCamera()
            throw()
        {;}
        virtual DWFModelSceneChangeHandler* clone () const
            throw()
        {
            return DWFCORE_ALLOC_OBJECT( _W3DCamera(*this) );
        }
        virtual teChangeType changeType () const
            throw()
        {
            return eSceneLevel;
        }
    
		_DWFTK_API
        void serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
            throw( DWFException );

        bool smoothTransition() const { return _bSmoothTransition; }
    private:
        bool        _bSmoothTransition;
    };

    class _W3DColor : public TK_Color
                    , public DWFModelSceneChangeHandler
    {
    public:
        _W3DColor() : TK_Color() {;}
        _W3DColor(const _W3DColor& rColor) : TK_Color(rColor) {;}
        virtual ~_W3DColor()
            throw()
        {;}
            virtual DWFModelSceneChangeHandler* clone () const
            throw()
        {
            return DWFCORE_ALLOC_OBJECT( _W3DColor(*this) );
        }

        virtual teChangeType changeType () const
            throw()
        {
            return eInstanceLevel;
        }

		_DWFTK_API
        void serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
            throw( DWFException );

        inline void _serializeChannels( DWFXMLSerializer& rSerializer, const float* anChannels );
    };

    class _W3DCuttingPlanes : public TK_Cutting_Plane
                            , public DWFModelSceneChangeHandler
    {
    public:
        _W3DCuttingPlanes() : TK_Cutting_Plane() {;}
        _W3DCuttingPlanes(const _W3DCuttingPlanes& rPlanes) : TK_Cutting_Plane(rPlanes) {;}
        virtual ~_W3DCuttingPlanes()
            throw()
        {;}
        virtual DWFModelSceneChangeHandler* clone () const
            throw()
        {
            return DWFCORE_ALLOC_OBJECT( _W3DCuttingPlanes(*this) );
        }

        virtual teChangeType changeType () const
            throw()
        {
            return eSceneLevel;
        }

		_DWFTK_API
        void serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
            throw( DWFException );
    };

    class _W3DTransform : public TK_Matrix
                        , public DWFModelSceneChangeHandler
    {
    public:
        _W3DTransform( unsigned char eWhich ) : TK_Matrix( eWhich ) {;}
        _W3DTransform( const _W3DTransform& rTransform ) : TK_Matrix( rTransform ) {;}
        virtual ~_W3DTransform()
            throw()
        {;}
        virtual DWFModelSceneChangeHandler* clone () const
            throw()
        {
            return DWFCORE_ALLOC_OBJECT( _W3DTransform(*this) );
        }
    
        virtual teChangeType changeType () const
            throw()
        {
            return eInstanceLevel;
        }

		_DWFTK_API
        void serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
            throw( DWFException );
    };

    class _W3DVisibility : public TK_Visibility
                         , public DWFModelSceneChangeHandler
    {
    public:
        _W3DVisibility() : TK_Visibility() {;}
        _W3DVisibility( const _W3DVisibility& rVis ) : TK_Visibility( rVis ) {;}
        virtual ~_W3DVisibility()
            throw()
        {;}
        virtual DWFModelSceneChangeHandler* clone () const
            throw()
        {
            return DWFCORE_ALLOC_OBJECT( _W3DVisibility(*this) );
        }
    
        virtual teChangeType changeType () const
            throw()
        {
            return eInstanceLevel;
        }

		_DWFTK_API
        void serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
            throw( DWFException );
    };

    class _W3DDisplayMode : public _W3DXMLPseudoHandler
    {
    public:
        _W3DDisplayMode( unsigned int nDisplayMode ) 
            : _nDisplayMode( nDisplayMode )
        {;}
        _W3DDisplayMode( const _W3DDisplayMode& rDisplayMode )
            : _nDisplayMode( rDisplayMode._nDisplayMode ) 
        {;}
        virtual ~_W3DDisplayMode()
            throw()
        {;}
        virtual DWFModelSceneChangeHandler* clone () const
            throw()
        {
            return DWFCORE_ALLOC_OBJECT( _W3DDisplayMode(*this) );
        }

        virtual teChangeType changeType () const
            throw()
        {
            return eSceneLevel;
        }

        void setMode( unsigned int nDisplayMode) { _nDisplayMode = nDisplayMode; }
        unsigned int mode() const { return _nDisplayMode; }

		_DWFTK_API
        void serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
            throw( DWFException );
    private:
        _W3DDisplayMode& operator=( const _W3DDisplayMode& );

    private:
        unsigned int                    _nDisplayMode;
    };

protected:

    std::deque<DWFModelSceneChangeHandler*>  _oSerializableSceneAPI;
    bool                        _bOpenInstance;
    DWFString                   _zOpenInstanceID;
    bool                        _bInstancePreviouslyOpen;
    DWFString                   _zPreviousInstanceID;

private:

    void _checkForOpenInstance()
    throw( DWFException );

    DWFModelScene( const DWFModelScene& );
    DWFModelScene& operator=( const DWFModelScene& );

};

}

#endif
