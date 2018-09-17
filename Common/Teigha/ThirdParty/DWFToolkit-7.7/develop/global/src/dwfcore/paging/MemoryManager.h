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

#ifndef _DWFCORE_MEMORY_MANAGER_H
#define _DWFCORE_MEMORY_MANAGER_H
#include "dwfcore/paging/PagingFwd.h"
#include "dwfcore/paging/Adapter.h"

///
///\file        dwfcore/paging/MemoryManager.h
/// This header contains the declaration for DWFMemoryManager. 
///

namespace DWFCore
{	
	/// memory management class, and this class is designed as a singleton
	class DWFMemoryManager
	{
	public: //type definition
		typedef DWFPagingAdapter::tHandleType tHandleType;		///< handle type
		typedef long tTimestampType;	///< time stamp type

		static const tHandleType kInvalidHandle = DWFPagingAdapter::kInvalidHandle;
		static const tTimestampType kInvalidTimestamp = 0;

		/// store the information of managed object
		struct tStubInfo;

		/// hold the basic information of managed object
		struct tInfoHolderBase
		{
			///holder's states
			enum teState
			{
				eHandle = 0,	///< hold a hHandle
				eInfo = 1		///< hold a tStubInfo
			};

			/** query the state
				\return teState
			*/
			virtual teState state() const = 0;

			/** Page out the object
				\pre state() == eInfo
			*/
			virtual void pageOut() const= 0;

			/** load an object indicated by the hHandle
				\pre state() == eHandle
			*/
			virtual void pageIn() const= 0;

			_DWFCORE_API DWFPagingAdapter* pagingAdapter() const;

		protected:
			_DWFCORE_API tStubInfo* acquire() const;

			_DWFCORE_API void release(tStubInfo* pInfo) const;

			_DWFCORE_API static tTimestampType Now();

			/// Destructor
			virtual ~tInfoHolderBase(){}
		};		

		struct tStubInfo
		{
			/// helper function used to initialize a stud_info structure and avoid to destroy the data members _next and _prev 
			static void reset(tStubInfo& rInfo)
			{
				rInfo._hHandle = kInvalidHandle;
				rInfo._lastAccess = kInvalidTimestamp;
				rInfo._pObj = 0;
				rInfo._pOwner = 0;
				rInfo._isDirty = false;
			}

			tStubInfo* _prev;		///<used by memory management
			tStubInfo* _next;		///<used by memory management

			tHandleType _hHandle;	///< indicator of a managed object, 
			tTimestampType _lastAccess;	///< the last access time
			void* _pObj;				///< point to a managed object
			const tInfoHolderBase* _pOwner;		///< indicate which holder own the object
			bool _isDirty; ///< true if the object was modified
		};

		/// memory block
		struct tBlock
		{	
			size_t _size;
			tBlock* _next;
			tStubInfo _Nodes[1];
		};

		/// record the information of memory management
		struct tTableInfo
		{
			tBlock* _pBlockHead;			///< point to the head of the memory tBlock
			tStubInfo* _pFreeHead;			///< point to the _next available tStubInfo _Nodes
			tStubInfo* _pUsedHead;			///< point to the head of the linked list of used _Nodes

			size_t _nUsedCount;			///< record the number of used tStubInfo _Nodes
			size_t _nFreeCount;			///< record the number of freed tStubInfo _Nodes

			size_t _nHighWater;			///< cached value from DWFPagingStrategyBase
			size_t _nLowWater;				///< cached value from DWFPagingStrategyBase

			size_t _nBlockSize; //= 10236;
		};

	public: // methods
		/** \return tPointer of a tStubInfo node
		\post return != 0
		\throw any				
		*/
		_DWFCORE_API tStubInfo* acquire();

		/** \pre p != 0
		\param p tStubInfo node
		release the node and add into free linked list
		*/
		_DWFCORE_API void release(tStubInfo* pStubInfo);

		/// _install the cache strategy
		///\param cs if cs is null, disable cache strategy
		_DWFCORE_API void cacheStrategy(DWFPagingStrategyBase* pPs);

		/// get the tPointer of cache strategy
		_DWFCORE_API DWFPagingStrategyBase* cacheStrategy() const;

		/// Called by the implementations of DWFPagingStrategyBase interface. If the properties of \n
		/// implementation object has been changed, it should notify DWFMemoryManager object. 
		_DWFCORE_API void notify();

		_DWFCORE_API DWFPagingAdapter* pagingAdapter() const;

		/// return the timestamp
		_DWFCORE_API static tTimestampType Now();

		/// return the instance of the class
		_DWFCORE_API static DWFMemoryManager& Instance();
		
    _DWFCORE_API static void Purge();

	private:	// private methods

		DWFMemoryManager();
#if defined(__BCPLUSPLUS__)
public:
#endif
  ~DWFMemoryManager();
  private:
    void _init();
		void _clear();

	private:	//data members
		tTableInfo _oInfo;
		DWFPagingStrategyBase* _pStrategy;
		DWFThreadMutex* _pMutex;

		static DWFMemoryManager* _instance;
	
	private:	//disabled
		DWFMemoryManager (const DWFMemoryManager&);
		DWFMemoryManager& operator=(const DWFMemoryManager&);
	};
}	

#endif //end _DWFCORE_MEMORY_MANAGER_H
