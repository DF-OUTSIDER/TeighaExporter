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

#ifndef _DWFCORE_FILE_ADAPTER_H
#define _DWFCORE_FILE_ADAPTER_H
#include "dwfcore/paging/Adapter.h"
#include "dwfcore/StreamFileDescriptor.h"

///
///\file        dwfcore/paging/FileAdapter.h
///\brief       This header contains the declaration for DWFFileAdapter.
///

namespace DWFCore
{
	/**
		This class intends to store the data into files.
	*/
	class DWFFileAdapter : public DWFPagingAdapter
	{
	public:
		/** Constructor
			\param zFileName file name
		*/
		explicit DWFFileAdapter(const DWFFile& rFile);

		/** Destructor
		*/
		~DWFFileAdapter();

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
		void init();
		void flush();

		DWFStreamFileDescriptor _oStream;
		bool _bTransacting;
		tHandleType _oNextHandle;
		tBufferType _oBuffer;

	private:	//disable copy constructor and assignment
		DWFFileAdapter(const DWFFileAdapter&);
		DWFFileAdapter& operator=(const DWFFileAdapter&);
	};
}
#endif //end _DWFCORE_FILE_ADAPTER_H
