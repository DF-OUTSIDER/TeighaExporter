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


#ifndef _DWFTK_PROPERTY_H
#define _DWFTK_PROPERTY_H


///
///\file        dwf/package/Property.h
///\brief       This file contains the DWFProperty class declaration.
///


#include "dwfcore/STL.h"
#include "dwfcore/Owner.h"
#include "dwfcore/Vector.h"
#include "dwfcore/SkipList.h"
#include "dwfcore/paging/Proxy.h"

using namespace DWFCore;

#include "dwf/Toolkit.h"
#include "dwf/package/XML.h"
#include "dwf/package/reader/PackageReader.h"
#include "dwf/package/writer/PackageWriter.h"



namespace DWFToolkit
{

///
///\ingroup     dwfpackage
///
///\class       DWFProperty   dwf/package/Property.h     "dwf/package/Property.h"
///\brief       This class represents a categorized name-value data pair.
///\since       7.0.1
///
///             Properties are available for use with the DWFPropertyContainer utility class
///             and those that specialize it.
///
class DWFProperty : public DWFXMLBuildable

#ifndef DWFTK_READ_ONLY
                  , public DWFXMLSerializable
#endif
                  , public IDWFOwnable
                  _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///\brief   This type defines a list of DWFProperty pointers.
    ///
    typedef _DWFTK_STD_VECTOR(DWFProperty*) tList;
    ///
    ///\brief   This type defines a DWFIterator on the list of DWFProperty pointers.
    ///
    typedef DWFVectorIterator<DWFProperty*, _DWFTK_STL_ALLOCATOR<DWFProperty*> >             tListIterator;
    ///
    ///\brief   This type defines an ordered list of DWFProperty pointers.
    ///
    typedef DWFOrderedVector<DWFProperty*>              tOrderedList;
    ///
    ///\brief   This type defines a mapped collection of DWFProperty pointers.
    ///
    typedef DWFStringKeyChainedSkipList<DWFProperty*>   tMap;
    ///
    ///\brief   This type defines a caching iterator on a collection of DWFProperty pointers,
    ///         that caches the pointers in its own container.
    ///
    typedef DWFCachingIterator<DWFProperty*>            tCachingIterator;
    ///
    ///\brief   This type defines a basic iterator on a collection of DWFProperty pointers.
    ///
    typedef DWFIterator<DWFProperty*>                   tIterator;

	///
	///\brief	This definitions are for return the attributes set of this DWFProperty
	///
	typedef pair<DWFString, DWFString>              tAttributePair;
	typedef _DWFTK_STD_VECTOR(tAttributePair)       tAttributeList;
	typedef std::map<DWFString, tAttributeList>     tAttributeMap;

public:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    DWFProperty()
        throw();

    ///
    ///         Constructor
    ///
    ///\param   zName       Identifies the property within its \a zCategory.
    ///\param   zValue      The data associated with the property.
    ///\param   zCategory   An optional grouping of properties.
    ///\param   zType       An optional type defining the data.
    ///\param   zUnits      An optional type defining the data.
    ///\throw   None
    ///
    _DWFTK_API
    DWFProperty( const DWFString& zName,
                 const DWFString& zValue,
                 const DWFString& zCategory = /*NOXLATE*/L"",
                 const DWFString& zType = /*NOXLATE*/L"",
                 const DWFString& zUnits = /*NOXLATE*/L"" )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFProperty()
        throw();

    ///
    ///         Copy Constructor
    ///
    ///\param   rProperty   The source property from which to copy.
    ///\throw   None
    ///
    _DWFTK_API
    DWFProperty( const DWFProperty& rProperty )
        throw();

    ///
    ///         Assignment Operator
    ///
    ///\param   rProperty   The source property from which to copy.
    ///\throw   None
    ///
    _DWFTK_API
    DWFProperty& operator=( const DWFProperty& rProperty )
        throw();

    ///
    ///         Returns the property name.
    ///
    ///\return  A string identifying the property with a \a category.
    ///\throw   None
    ///
    const DWFString& name() const
        throw()
    {
		return *oPropertyContent->_pszName;
    }

    ///
    ///         Modifies the property name.
    ///
    ///\param   zName       The new name.
    ///\throw   None
    ///
    _DWFTK_API
    void setName( const DWFString& zName )
        throw();

    ///
    ///         Returns the property value.
    ///
    ///\return  A string identifying the property data.
    ///\throw   None
    ///
    const DWFString& value() const
        throw()
    {
		return *oPropertyContent->_pszValue;
    }

    ///
    ///         Modifies the property value.
    ///
    ///\param   zValue  The new value.
    ///\throw   None
    ///
    _DWFTK_API
    void setValue( const DWFString& zValue )
        throw();

    ///
    ///         Returns the property category.
    ///
    ///\return  A string identifying an optional grouping.
    ///\throw   None
    ///
    const DWFString& category() const
        throw()
    {
		return *oPropertyContent->_pszCategory;
    }

    ///
    ///         Modifies the property category.
    ///
    ///\param   zCategory   The new category.
    ///\throw   None
    ///
    _DWFTK_API
    void setCategory( const DWFString& zCategory )
        throw();

    ///
    ///         Returns the property value type.
    ///
    ///\return  A string identifying the value type.
    ///\throw   None
    ///
    const DWFString& type() const
        throw()
    {
		return *oPropertyContent->_pszType;
    }

    ///
    ///         Modifies the property value type.
    ///
    ///\param   zType       The new type.
    ///\throw   None
    ///
    _DWFTK_API
    void setType( const DWFString& zType )
        throw();

    ///
    ///         Returns the property value units.
    ///
    ///\return  A string identifying the data units.
    ///\throw   None
    ///
    const DWFString& units() const
        throw()
    {
		return *oPropertyContent->_pszUnits;
    }

    ///
    ///         Modifies the property value units.
    ///
    ///\param   zUnits      The new units.
    ///\throw   None
    ///
    _DWFTK_API
    void setUnits( const DWFString& zUnits )
        throw();

    ///
    ///\copydoc DWFCore::DWFXMLBuildable::parseAttributeList(const char** ppAttributeList)
    ///
    _DWFTK_API
    virtual void parseAttributeList( const char** ppAttributeList )
        throw( DWFException );

    ///
    ///			Ignore the customize attributes.
    ///
    _DWFTK_API
    virtual void parseAttributeList( const char** ppAttributeList, bool bIgnoreCustomizeAttribute )
        throw( DWFException );

	///
	///\return the customer defined attributes
	///
	_DWFTK_API
	const tAttributeMap& getCustomizedAttributeList()
		throw();

	///
    ///\copydoc DWFCore::IDWFOwnable::own(DWFOwner& rOwner)
    ///
    _DWFTK_API
    virtual void own( DWFOwner& rOwner )
        throw( DWFException );

	///
    ///\copydoc DWFCore::IDWFOwnable::disown(DWFOwner& rOwner, bool bForget)
    ///
    _DWFTK_API
    virtual bool disown( DWFOwner& rOwner, bool bForget )
        throw( DWFException );

	///
    ///\copydoc DWFCore::IDWFOwnable::owner()
    ///
    _DWFTK_API
    virtual DWFOwner* owner()
        throw( DWFException );

	///
    ///\copydoc DWFCore::IDWFOwnable::observe(DWFOwner& rObserver)
    ///
    _DWFTK_API
    virtual void observe( DWFOwner& rObserver )
        throw( DWFException );

	///
    ///\copydoc DWFCore::IDWFOwnable::unobserve(DWFOwner& rObserver)
    ///
    _DWFTK_API
    virtual bool unobserve( DWFOwner& rObserver )
        throw( DWFException );

	///
    ///\copydoc DWFCore::DWFOwnable::_notifyDelete()
    ///
	_DWFTK_API
    void _notifyDelete()
        throw();

    ///
    ///         This method can be used by publishers to add extra data
    ///         to property elements.  These data are not part of the 
    ///         DWF format schema and must therefore be scoped by a unique
    ///         namespace.  
    ///
    ///\param   rNamespace      The publisher-specific (non DWF) namespace for the attribute.
    ///                         This object is obtained from only those classes that support
    ///                         and implement the DWFXMLSerializable::addNamespace() method.
    ///                         This parameter is required; this method will thrown an exception
    ///                         if this parameter is not provided.
    ///\param   zName           The name of the attribute.
    ///                         This parameter is required; this method will thrown an exception
    ///                         if this parameter is not provided.
    ///\param   zValue          The attribute value.
    ///                         This parameter is required; this method will thrown an exception
    ///                         if this parameter is not provided.
    ///
    _DWFTK_API
    void addXMLAttribute( const DWFXMLNamespace&    rNamespace,
                          const DWFString&          zName,
                          const DWFString&          zValue )
       throw( DWFException );

#ifndef DWFTK_READ_ONLY
    ///
    ///\copydoc DWFXMLSerializable::serializeXML()
    ///
    _DWFTK_API
    void serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
        throw( DWFException );

#endif

private:

  friend struct tPropertyContent;
	/** This structure intend to store all data of property, and it will be managed by DWFProxy*/
	
	struct _DWFTK_API tPropertyContent
	{
		tPropertyContent();

		tPropertyContent(const tPropertyContent& rhs);

		tPropertyContent& operator=(const tPropertyContent& rhs);

		const DWFString* _pszName;    
		const DWFString* _pszCategory;
		const DWFString* _pszType;
		const DWFString* _pszUnits;
		const DWFString* _pszValue;

		DWFOwner*                _pOwner;
		DWFSortedList<DWFOwner*> _oOwnerObservers;

		typedef pair<DWFString, DWFString>              _tAttribute;
		typedef _DWFTK_STD_VECTOR(_tAttribute)          _tAttributeList;
		typedef std::map<DWFString, _tAttributeList>    _tAttributeMap;
		_tAttributeMap	_oAttributes;

	};

	friend class tPropertyArchive;

	/**  This class intends to:
		1. serialize a tPropertyContent type object into a physical adapter and return a handle
		2. load data from physical adapter and restore the object
		tPropertyArchive can be treat as a specialization of DWFArchive. Because DWFProxy support specifying a user defined Archive, 
		so we can implement a standalone Archive instead of specialization. 
		When we intend to implement an Archive, we should give out following typedefs,load and save functions. 
		the prototypes of load and save should look LIKE in DWFArchive.
		\see DWFArchive
	*/
	class _DWFTK_API tPropertyArchive
	{
	public:

		typedef DWFToolkit::DWFProperty::tPropertyContent tValueType;
		typedef tValueType* tPointer;
		typedef const tValueType* tConstPointer;
		typedef DWFPagingAdapter::tHandleType tHandleType;

		///\copydoc DWFArchive<T>::load
		static void load(DWFInputAdapter& rInputAdapter, 
			tHandleType hHandle, 
			tPointer pObject);

		///\copydoc DWFArchive<T>::save
		static tHandleType save(DWFOutputAdapter& rOutputAdapter, 
			tConstPointer p, 
			tHandleType old);

	private:

		typedef std::back_insert_iterator<DWFInputAdapter::tBufferType> tOutputIterator;

		static void _copyByte(const void* src, 
			size_t size, 
			tOutputIterator itr);

		static void * _loadStr(DWFString& str, void *pos);
	};

public:
	///
    ///         Convert to DWFOwnable& type
    ///
    ///\return  The reference of DWFOwnable.
    ///\throw   None
    ///
	DWFOwnable& toOwnable()
	{
		return _toOwnable();
	}

private:
	DWFOwnable& _toOwnable();	///here is a hack, but it's safe

	//specify the Archive type with tPropertyArchive instead of DWFArchive
	typedef DWFProxy<tPropertyContent, std::allocator<tPropertyContent>, tPropertyArchive> tImpType;
	tImpType oPropertyContent;
#if defined(__BCPLUSPLUS__)
    public:
#endif
	static const DWFString _koEmpty;
	static DWFStringTable* _kpDataTable;
};

}

#endif
