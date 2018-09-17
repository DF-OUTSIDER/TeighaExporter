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


#ifndef _DWFCORE_ARCHIVE_H
#define _DWFCORE_ARCHIVE_H
#include "dwfcore/paging/PagingFwd.h"
#include "dwfcore/paging/Adapter.h"

#include <cassert>

///
///\file        dwfcore/paging/Archive.h
/// This header contains the declaration for archive concepts. An in/out archive can serialize a object into a data tBlock.
/// Each specialization of the in/out archive can duo to one single or group types
///

namespace DWFCore
{
	/** This namespace is internal
	*/
	namespace detail
	{
		/** helper for implement IsPodType ;
		*/
		template<typename T>
		struct IsPodTypeHelper
		{
			union
			{
				T t_;
				char i_;
			};		
		};

		/** type trait, implemented for check POD( plain old data) type.
		*/
		template<typename T>
		struct IsPodType
		{
			enum { size = sizeof(IsPodTypeHelper<T>)};
		};
	}

	/** This class intends to load the data tBlock via DWFInputAdapter interface first, and then create the object and return to caller
		\note the default implementation can't duo to any user defined types except POD (plain old data) types.
		DWFArchive defined a CONCEPT and concept is a term of GP(generic programming). 
		A concept contains a set of requirements that describe a family of abstractions.
		More information:http://www.generic-programming.org/
	*/
	//POD implementation;
	///\param T the type to be loaded
	template<typename T>
	class DWFArchive
	{
		///if T is not a POD type, it will cause a compilation error.
	  static const int kCheck = detail::IsPodType<T>::size;
	public:
		typedef T tValueType;
		typedef T* tPointer;
		typedef const T* tConstPointer;
		typedef typename DWFPagingAdapter::tHandleType tHandleType;

		/** load the data tBlock of the object indicated by hHandle, and create the object with the data.
			\pre hHandle != DWFInputAdapter::kInvalidHandle
			\pre pObject != 0
			\param rInputAdapter InputAdapter for load from
			\param hHandle indicator of the object
			\param pObject point to uninitialized object.
			\post the object pointed by pObject was constructed.
			\throw any
		*/
		static void load(DWFInputAdapter& rInputAdapter, tHandleType hHandle, tPointer pObject)
		{
			assert(hHandle != DWFInputAdapter::kInvalidHandle);
			assert(pObject != 0);

			DWFInputAdapter::tBufferType buf;
			rInputAdapter.load(hHandle, buf);

			if (buf.size() != sizeof(tValueType))
			{				
				_DWFCORE_THROW( DWFUnexpectedException, /*NOXLATE*/L"The Adapter returned invalid data, data source may be invalid" );
			}

			memcpy(pObject, &buf[0], buf.size());	//because T is a pod type, so here can use memcpy safely
		}
		
		/** it intends to serialize the object and save the data tBlock via DWFOutputAdapter interface.
			\pre t != 0
			\param t tPointer of the object will to be saved
			\param old hHandle of the object before saved
			\return new hHandle of the object after saved.
			\post return != invalid_handle
			\throw any
		*/
		static tHandleType save(DWFOutputAdapter& rOutputAdapter, tConstPointer t, tHandleType old)
		{
			DWFInputAdapter::tBufferType buf(sizeof(tValueType));
			memcpy(&buf[0], t, sizeof(tValueType));
			return rOutputAdapter.save(buf, old);
		}
	};

	/**	The specialization of DWFArchive for std::basic_string
	    the descriptions of template parameters can see std::basic_string
	*/
	template<typename T, typename Traits, typename Alloc>
	class DWFArchive<std::basic_string<T, Traits, Alloc> >
	{
	public:
		typedef std::basic_string<T, Traits, Alloc> tValueType;
		typedef tValueType* tPointer;
		typedef const tValueType* tConstPointer;
		typedef typename DWFPagingAdapter::tHandleType tHandleType;

		///\copydoc DWFArchive<T>::load(DWFInputAdapter& rInputAdapter, tHandleType hHandle, tPointer pObject)
		static void load(DWFInputAdapter& rInputAdapter, tHandleType hHandle, tPointer pObject)
		{
			assert(hHandle != DWFInputAdapter::kInvalidHandle);
			assert(pObject != 0);

			DWFInputAdapter::tBufferType buf;
			rInputAdapter.load(hHandle, buf);//only load the raw data of basic_string

			if (buf.size() % sizeof(T) != 0)
			{				
				_DWFCORE_THROW( DWFUnexpectedException, /*NOXLATE*/L"The Adapter returned invalid data, data source may be damaged" );
			}

			tPointer first = (tPointer)&*buf.begin();
			//restore the object. we can restore any object if the object's public method support these operation.
			::new (pObject) tValueType(first, first + buf.size() / sizeof(tValueType));
		}

		///\copydoc DWFArchive<T>::save(DWFInputAdapter& rOutputAdapter, tConstPointer p, tHandleType old)
		static tHandleType save(DWFOutputAdapter& rOutputAdapter, tConstPointer p, tHandleType old)
		{
			DWFInputAdapter::tBufferType buf(p->size() * sizeof(tValueType));
			std::copy(p->begin(), p->end(), (tPointer)&buf[0] );	//we have no need to store everything of the object, we just store enough information for restoration.

			return rOutputAdapter.save(buf, old);
		}
	};
}

#endif	//end _DWFCORE_ARCHIVE_H
