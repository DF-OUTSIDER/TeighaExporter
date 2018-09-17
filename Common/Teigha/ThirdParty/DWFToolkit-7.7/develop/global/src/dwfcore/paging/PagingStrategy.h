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

#ifndef _DWFCORE_PAGING_STRATEGY_H
#define _DWFCORE_PAGING_STRATEGY_H
#include "dwfcore/paging/PagingFwd.h"
#include "dwfcore/paging/Adapter.h"
#include "dwfcore/paging/MemoryManager.h"

///
///\file        dwfcore/paging/PagingStrategy.h
/// This header contains the declaration for DWFPagingStrategyBase and DWFNruStrategy. 
///

namespace DWFCore
{	
	/// This class intends to be called when the tStubInfo node is acquired or released.
	/// It intends to be used by DWFMemoryManager only
	class DWFPagingStrategyBase
	{
	public:
		/// Constructor
		_DWFCORE_API explicit DWFPagingStrategyBase(DWFPagingAdapter& rPagingAdapter);

		_DWFCORE_API virtual ~DWFPagingStrategyBase();

		/// Called when a tStubInfo is acquired.
		/// \param rTableInfo information of DWFMemoryManager
		/// \pre isInstalled()
		_DWFCORE_API virtual void onAcquire(DWFMemoryManager::tTableInfo& rTableInfo) = 0;	

		/// Called when a tStubInfo is release.
		/// \param rTableInfo information of DWFMemoryManager
		/// \pre isInstalled()
		_DWFCORE_API virtual void onRelease(DWFMemoryManager::tTableInfo& rTableInfo ) = 0; 

		/// Get the limit of high water mark
		_DWFCORE_API size_t highWater() const;

		/// Get the limit of low water mark
		_DWFCORE_API size_t lowWater() const;

		/// Set the limit of high water mark
		_DWFCORE_API void highWater(size_t nNewMarker);

		/// Set the limit of low water mark
		_DWFCORE_API void lowWater(size_t nNewMarker);

		/// return true if has been _install into DWFMemoryManager
		_DWFCORE_API bool isInstalled() const;
		
		_DWFCORE_API DWFPagingAdapter& pagingAdapter() const;

	protected:
		DWFMemoryManager* _pMemoryManager;
		size_t _nHighWater;
		size_t _nLowWater;
		DWFPagingAdapter* _pPagingAdapter;

	private:
		/// It is called by DWFMemoryManager, customer programmer never call this function directly
		/// \param rMMgr point to DWFMemoryManager object. if rMMgr is a null pointer, it means _uninstall.
		void _install(DWFMemoryManager& rMMgr);

		/// It is called by DWFMemoryManager, customer programmer never call this function directly		
		void _uninstall();		

		friend class DWFMemoryManager;

	private:	///disabled
		DWFPagingStrategyBase(const DWFPagingStrategyBase&);
		DWFPagingStrategyBase& operator=(const DWFPagingStrategyBase&);
	};

	/// NRU(Not Recently Used) Strategy
	class DWFNruStrategy : public DWFPagingStrategyBase
	{
	public:
		/// Constructor
		_DWFCORE_API explicit DWFNruStrategy(DWFPagingAdapter& rAdapter);
		
		///\copydoc DWFPagingStrategyBase::onAcquire(DWFMemoryManager::tTableInfo& rTableInfo)
		_DWFCORE_API virtual void onAcquire(DWFMemoryManager::tTableInfo& rTableInfo);

		///\copydoc DWFPagingStrategyBase::onRelease(DWFMemoryManager::tTableInfo& rTableInfo)
		_DWFCORE_API virtual void onRelease(DWFMemoryManager::tTableInfo& /*rTableInfo*/);
	};
}

#endif //end _DWFCORE_PAGING_STRATEGY_H
