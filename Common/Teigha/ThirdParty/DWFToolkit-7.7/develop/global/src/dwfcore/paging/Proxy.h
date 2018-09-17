/*
$COMMON_HEAD_COMMENTS_CONTEXT$
*/

/** @file 

*/

#ifndef _DWFCORE_AUTO_ALLOCATOR_H
#define _DWFCORE_AUTO_ALLOCATOR_H
#include "dwfcore/paging/PagingFwd.h"
#include "dwfcore/paging/MemoryManager.h"
#include "dwfcore/paging/Archive.h"
#include <cassert>

///
///\file        dwfcore/cache/DWFProxy.h
/// This header contains the declaration for archive concepts. An in/DWFArchive can serialize a object into a data tBlock.
/// Each specialization of the in/DWFArchive can duo to one single or group types
///

namespace DWFCore
{
	//internal use
	template<typename T, typename Alloc>
	struct _tUninitAllocatorPtr
	{
		explicit _tUninitAllocatorPtr(Alloc rAlloc)
			: _rAlloc(rAlloc)
#if !defined(__sun)
			, _pObj((T*)rAlloc.allocate(1))
#endif
		{
#if defined(__sun)
                  void *_Ptr = ::operator new(1 * sizeof (T));
                  _pObj = ((T*)_Ptr);
#endif
                }

		~_tUninitAllocatorPtr()
		{
			if (_pObj)
				_rAlloc.deallocate(_pObj, 1);
		}

		Alloc _rAlloc;
		T* _pObj;
	};
	//Helper smart pointer for implementing exception safety code. Work with allocator.
	//internal use
	template<typename T, typename Alloc>
	struct _tAllocatorPtr : private _tUninitAllocatorPtr<T, Alloc>
	{
		typedef _tUninitAllocatorPtr<T, Alloc> tBaseType;

        //
        // this is to make the template inheritance built successfully on GCC
        //
        using tBaseType::_rAlloc;
        using tBaseType::_pObj; 

		explicit _tAllocatorPtr(const Alloc& rAl)
			: tBaseType(rAl)
		{
#if defined(__sun)
      new (_pObj) T();
#else
      _rAlloc.construct(_pObj, T());
#endif
		}

		_tAllocatorPtr(const Alloc& rAl, const T& rObj)
			: tBaseType(rAl)
		{
#if defined(__sun)
        new (_pObj) T(rObj);
#else
      _rAlloc.construct(_pObj, T(rObj));
#endif
		}

		~_tAllocatorPtr()
		{
			if (_pObj)
				_rAlloc.deallocate(_pObj, 1);
		}

		T* release()
		{
			T* tmp = _pObj;
			_pObj = 0;
			return tmp;
		}

		T* get() const { return _pObj; }

	private:
		//disabled
		_tAllocatorPtr(const _tAllocatorPtr&);
		_tAllocatorPtr& operator=(const _tAllocatorPtr&);
	};

	///host an object of type T
	template
	<
		typename T, 
		typename Alloc = std::allocator<T>,
		typename Archive = DWFArchive<T>
	>
	class DWFProxy
	{
	public:
    typedef std::allocator<T> _tAlloc;
		typedef typename _tAlloc::value_type		tValueType;
		typedef typename _tAlloc::pointer			tPointer;
		typedef typename _tAlloc::const_pointer		tConstPointer;
		typedef typename _tAlloc::reference			tReference;
		typedef typename _tAlloc::const_reference	tConstReference;		
		typedef Archive tArchive;

		///default constructor.
		explicit DWFProxy(const _tAlloc& oAlloc = _tAlloc()) 
			: _oInfo(oAlloc)
		{
		}

		/// Constructor.
		explicit DWFProxy(tConstReference t, const _tAlloc& oAlloc = _tAlloc())
			: _oInfo(t, oAlloc)
		{			
		}

		/// Constructor.
		DWFProxy(const DWFProxy& rhs)
			: _oInfo(rhs._oInfo)
		{
		}		
	

		///assignment
		DWFProxy& operator=(const DWFProxy& rhs)
		{
			if (this != &rhs)
				DWFProxy(rhs).swap(*this);
			return *this;
		}

		///\return tPointer of hosted object
		///\post return != 0 
		tPointer operator->(){	return &**this;	}

		///\return const tPointer of hosted object
		///\post return != 0 
		tConstPointer operator->() const{ return &**this; }

		///\return tReference of hosted object		
		tReference operator*(){	return *_oInfo.get(); }

		///\return const tReference of hosted object
		tConstReference operator*() const{ return *_oInfo.get();}

		///if the object is managed, it's still managed after swapped with an unmanaged object.
		///the unmanaged object is still unmanaged even swapped with a managed object
		///\param rhs
		void swap(DWFProxy& rhs)
		{
			_oInfo.swap(rhs._oInfo);
		}

	private:
		typedef _tAllocatorPtr<tValueType, Alloc> _tAllocPtr;
		/// hold the information of managed object
		class tInfoHolderImp : public DWFMemoryManager::tInfoHolderBase
		{
		public:
			///\construct
			///\post state() = eHandle
			explicit tInfoHolderImp(const _tAlloc& rAl) 
				: _uStub()
				, _eState(eHandle)
				, _oAlloc(rAl)
			{
			}

			tInfoHolderImp(tConstReference t, const _tAlloc& rAl) 
				: _uStub()
				, _eState(eHandle)
				, _oAlloc(rAl)
			{
				_tAllocPtr ptr(_oAlloc, t); 
				attach(ptr.release());
			}

			tInfoHolderImp(const tInfoHolderImp& rhs) 
				: _uStub()
				, _eState(eHandle)
				, _oAlloc(rhs._oAlloc)
			{
                if (rhs._eState == eHandle && 
                    rhs._uStub._hHandle != DWFMemoryManager::kInvalidHandle)
                    rhs.get();

				if (rhs._eState == eInfo)
				{
					_tAllocPtr ptr(_oAlloc, *rhs.get());
					attach(ptr.release());
				}
			}
			~tInfoHolderImp()
			{
				dispose();
			}

			///\copydoc DWFMemoryManager::tInfoHolderBase::pageOut()
			virtual void pageOut() const
			{
				assert(_eState == eInfo && _uStub._pInfo && _uStub._pInfo->_pObj);

				DWFPagingAdapter::tHandleType hHandle = _uStub._pInfo->_hHandle;
				if (_uStub._pInfo->_isDirty)
				{
					DWFOutputAdapter* pAdapter = pagingAdapter();
					assert(pAdapter != 0);
					hHandle = tArchive::save(*pAdapter, (tConstPointer)_uStub._pInfo->_pObj, hHandle);					
				}
				dispose();
				assert(_eState == eHandle);

				_uStub._hHandle = hHandle;
			}

			///\copydoc DWFMemoryManager::tInfoHolderBase::pageIn()
			virtual void pageIn() const
			{
				assert(_eState == eHandle);
				
				_tAllocPtr ptr(_oAlloc);
				if (_uStub._hHandle == DWFMemoryManager::kInvalidHandle)
				{					
					attach(ptr.release());
				}
				else
				{
					DWFMemoryManager::tHandleType hHandle = _uStub._hHandle;

					DWFInputAdapter* pAdapter = pagingAdapter();
					assert(pAdapter != 0);

					tArchive::load(*pAdapter, hHandle, ptr.get());
					attach(ptr.get(), false, hHandle);
					ptr.release();
				}
			}

			/** query the state
				\return teState
			*/
			virtual teState state() const { return (teState)_eState;};

			tPointer get()
			{
				if (_eState == eHandle)
					pageIn();
				else
					_uStub._pInfo->_lastAccess = Now();

				_uStub._pInfo->_isDirty = true;			

				return (tPointer)_uStub._pInfo->_pObj;
			}

			tConstPointer get() const
			{
				if (_eState == eHandle)
					pageIn();
				else
					_uStub._pInfo->_lastAccess = Now();

				return (tConstPointer)_uStub._pInfo->_pObj;
			}

			void swap(tInfoHolderImp& rhs)
			{
				std::swap(_oAlloc, rhs._oAlloc);
				std::swap(_eState, rhs._eState);
				std::swap(_uStub, rhs._uStub);

				if (_eState == eInfo)
					_uStub._pInfo->_pOwner = this;

				if (rhs._eState == eInfo)
					rhs._uStub._pInfo->_pOwner = &rhs;
			}

		private:
			void dispose() const
			{
				if (_eState == eInfo)	//managed
				{
					assert(_uStub._pInfo && _uStub._pInfo->_pObj);

#if defined(__sun)
          ((tPointer)_uStub._pInfo->_pObj)->~tValueType();
#else
          _oAlloc.destroy((tPointer)_uStub._pInfo->_pObj);
#endif
					_oAlloc.deallocate((tPointer)_uStub._pInfo->_pObj, 1);

					DWFMemoryManager::tStubInfo::reset(*_uStub._pInfo);

					release(_uStub._pInfo);

					_eState = eHandle;
				}

				_uStub._hHandle = DWFMemoryManager::kInvalidHandle;
			}

			void attach(tPointer ptr, bool bDirty = true, 
				DWFMemoryManager::tHandleType hHandle = DWFMemoryManager::kInvalidHandle) const
			{
				assert(ptr != 0);
				assert(_eState == eHandle);

				_uStub._pInfo = acquire();
				_eState = eInfo;

				_uStub._pInfo->_hHandle = hHandle;
				_uStub._pInfo->_isDirty = bDirty;
				_uStub._pInfo->_lastAccess = Now();
				_uStub._pInfo->_pObj = ptr;
				_uStub._pInfo->_pOwner = this;
			}

			union tuInfoHandle
			{
				tuInfoHandle() : _hHandle(DWFMemoryManager::kInvalidHandle){}
				DWFMemoryManager::tStubInfo * _pInfo;
				DWFMemoryManager::tHandleType _hHandle;
			};

			mutable tuInfoHandle _uStub;
			mutable short _eState;	///< for saving memory
			mutable _tAlloc _oAlloc;
		};

		tInfoHolderImp _oInfo;
	};
}
#endif //end _DWFCORE_AUTO_ALLOCATOR_H
