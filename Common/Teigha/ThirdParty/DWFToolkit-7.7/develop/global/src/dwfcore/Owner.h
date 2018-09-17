//
//  Copyright (c) 2003-2006 by Autodesk, Inc.
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


#ifndef _DWFCORE_OWNER_H
#define _DWFCORE_OWNER_H

///
///\file        dwfcore/Owner.h
///\brief       This header contains the declaration for the DWFMonitoredInputStream class.
///


#include "dwfcore/SkipList.h"
#include "dwfcore/Exception.h"



namespace DWFCore
{


//
// fwd decl
//
class DWFOwnable;


///
///\class       DWFOwner     dwfcore/Owner.h  "dwfcore/Owner.h"
///\brief       Generalization for classes that maintain ownership of others.
///\since       1.0.1
///
///             This class base and callback interface is available as part of
///             a simple object ownership mechanism in the library.  Objects
///             specializing this class may be assigned as owners of objects
///             that specialize the DWFOwnable class.
///
class DWFOwner
{

public:

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFCORE_API
    virtual ~DWFOwner()
        throw();

    ///
    ///         This callback notification is made when the ownership
    ///         of an owned object changes.  This owner should take whatever
    ///         action is necessary to respond to the status change.
    ///
    ///\param   rOwnable    The object now owned by a different owner.
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual void notifyOwnerChanged( DWFOwnable& rOwnable )
        throw( DWFException );

    ///
    ///         This callback notification is made when a previously
    ///         owned object is about to be destroyed.  This method
    ///         will not be invoked if the ownership change was made
    ///         with \a DWFOwnable::disown() with \a bForget set to \e true.
    ///\note    The current object owner is not notified through this method.
    ///         Obviously, the owner ought be the only one deleting the ownable.
    ///         
    ///\param   rOwnable    The object being deleted.
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual void notifyOwnableDeletion( DWFOwnable& rOwnable )
        throw( DWFException );

protected:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    _DWFCORE_API DWFOwner()
        throw();
};

///
///\class       IDWFOwnable    dwfcore/Pointer.h    "dwfcore/Pointer.h"
///\brief       Generalization for classes that can be owned by others.
///\since       1.5
///
///             This is the interface of DWFOwnable 
///
struct IDWFOwnable
{
	///
    ///         Claims ownership of this object.
    ///
    ///\param   rOwner  The new owner.
    ///\throw   DWFException
    ///
	_DWFCORE_API
    virtual void own( DWFOwner& rOwner )
        throw( DWFException ) = 0;

	///
    ///         Orphans this object.
    ///
    ///\param   rOwner  The previous owner.
    ///\param   bForget if \e true, the previous owner will be removed from
    ///                 the internal observer list and not receive ownership change
    ///                 and object deletion notification callbacks;
    ///                 if \e false, the previous owner will continue to receive
    ///                 these notifications.
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual bool disown( DWFOwner& rOwner, bool bForget )
        throw( DWFException )= 0;

	///
    ///         Returns the current owner of this object.
    ///
    ///\return  A pointer to the current owner or NULL if this object is an orphan.
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual DWFOwner* owner()
        throw( DWFException )= 0;

	///
    ///         Informs that this object is being held onto by an "owner", but without claims of ownership.
    ///
    ///\param   rObserver  The observing "owner".
    ///\throw   DWFException
    ///\since   7.2
    ///
    _DWFCORE_API
    virtual void observe( DWFOwner& rObserver )
        throw( DWFException )= 0;

	///
    ///         Informs that this object is no longer being observed in a non-owning manner.
    ///
    ///\param   rObserver  The object that was observing.
    ///\throw   DWFException
    ///\since   7.2
    ///
    _DWFCORE_API
    virtual bool unobserve( DWFOwner& rObserver )
        throw( DWFException )= 0;

	///
    ///         Destructor
    ///
    ///\throw   None
    ///
	
	virtual ~IDWFOwnable() throw(){}
};

///
///\class       DWFOwnable  dwfcore/Owner.h  "dwfcore/Owner.h"
///\brief       Generalization for classes that can be owned by others.
///\since       1.0.1
///
///             This class base is available as part of
///             a simple object ownership mechanism in the library.  Objects
///             specializing this class may be owned by other objects
///             that specialize the DWFOwner class.
///
///\note        Specializations must call \a _notifyDelete() from their destructor.
///
class DWFOwnable : public IDWFOwnable
{

public:

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFCORE_API
    virtual ~DWFOwnable()
        throw();

    ///
    ///         Claims ownership of this object.
    ///
    ///\param   rOwner  The new owner.
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual void own( DWFOwner& rOwner )
        throw( DWFException );

    ///
    ///         Orphans this object.
    ///
    ///\param   rOwner  The previous owner.
    ///\param   bForget if \e true, the previous owner will be removed from
    ///                 the internal observer list and not receive ownership change
    ///                 and object deletion notification callbacks;
    ///                 if \e false, the previous owner will continue to receive
    ///                 these notifications.
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual bool disown( DWFOwner& rOwner, bool bForget )
        throw( DWFException );

    ///
    ///         Returns the current owner of this object.
    ///
    ///\return  A pointer to the current owner or NULL if this object is an orphan.
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual DWFOwner* owner()
        throw( DWFException );

    ///
    ///         Informs that this object is being held onto by an "owner", but without claims of ownership.
    ///
    ///\param   rObserver  The observing "owner".
    ///\throw   DWFException
    ///\since   7.2
    ///
    _DWFCORE_API
    virtual void observe( DWFOwner& rObserver )
        throw( DWFException );

    ///
    ///         Informs that this object is no longer being observed in a non-owning manner.
    ///
    ///\param   rObserver  The object that was observing.
    ///\throw   DWFException
    ///\since   7.2
    ///
    _DWFCORE_API
    virtual bool unobserve( DWFOwner& rObserver )
        throw( DWFException );


protected:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    _DWFCORE_API DWFOwnable()
        throw();

    ///
    ///         This method notifies previous owners of the
    ///         imminent destruction of this object.
    ///\note    Specialized classes \b must call this method in their
    ///         destructor to ensure the proper type is available to
    ///         \a DWFOwner::notifyOwnableDeletion() via RTTI
    ///         (therefore, this cannot occur in the destructor for this class).
    ///\throw   None
    ///
    _DWFCORE_API
    void _notifyDelete()
        throw();

private:

    DWFOwner*                _pOwner;
    std::set<DWFOwner*>      _oOwnerObservers;
};

}


#endif
