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

#ifndef _DWFCORE_SQLITE_ADAPTER_H
#define _DWFCORE_SQLITE_ADAPTER_H

#include "dwfcore/paging/Adapter.h"
#include "dwfcore/sqlite/sqlite3.h"
///
///\file        dwfcore/paging/SqlAdapter.h
///\brief       This header contains the declaration for DWFSqliteAdapter.
///

namespace DWFCore
{
	/**	This class intends to store the data into SQLite.
	*/
	class DWFSqliteAdapter : public DWFPagingAdapter
	{
	public:
		/** Constructor
		*/
		DWFSqliteAdapter();
    void _open();

		/// Destructor
		virtual ~DWFSqliteAdapter();
    void _close();

		///\copydoc DWFInputAdapter::load()
		virtual void load(tHandleType, tBufferType& );

		///\copydoc DWFOutputAdapter::save()
		virtual tHandleType save(const tBufferType& src, tHandleType old);

		///\copydoc DWFOutputAdapter::begin()
		virtual void begin();

		///\copydoc DWFOutputAdapter::begin()
		virtual void commit();

		///\copydoc DWFOutputAdapter::rollback()
		virtual void rollback();

	private:
		Btree* _pBTree;
		BtCursor* _pCur;
	private: //disable copy constructor and assignment
		DWFSqliteAdapter(const DWFSqliteAdapter&);
		DWFSqliteAdapter& operator=(const DWFSqliteAdapter&);
	};
}
#endif // _DWFCORE_SQLITE_ADAPTER_H
