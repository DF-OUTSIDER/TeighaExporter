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

#ifndef _DWFCORE_ADAPTER_H
#define _DWFCORE_ADAPTER_H
#include "dwfcore/paging/PagingFwd.h"

///
///\file        dwfcore/paging/Adapter.h
///\brief       This header contains the declaration for adatpter interface.
///


namespace DWFCore
{
	/**
		This class intends to load the data tBlock of an object's serialization.
		Implementors of this interface can load from different device.
	*/
	struct DWFInputAdapter
	{	
		typedef long tHandleType;	///< hHandle type
		static const tHandleType kInvalidHandle = 0;	///< invalid hHandle
		typedef std::vector<unsigned char> tBufferType;	///< _buffer type

		virtual ~DWFInputAdapter(){}
		
		/** \param hHandle the object hHandle, can't be kInvalidHandle
			\param buf data tBlock of the object indicated by hHandle
			\throw any
			load the data tBlock of the object indicated by handlle
		*/
		virtual void load(tHandleType hHandle, tBufferType& buf) = 0;
	};

	/**	This class intends to save the serialization of an object.
		Implementors of this interface can save to different device.
	*/
	struct DWFOutputAdapter
	{
		typedef long tHandleType; ///< hHandle type
		static const tHandleType kInvalidHandle = 0;///< invalid hHandle
		typedef std::vector<unsigned char> tBufferType;	///< _buffer type

		/** This class intends to track the transactions states. It is used to write exception safety code.
		*/
		class tScopedTransaction
		{
		public:
			/** RAII idiom. if a tScopedTransaction object was constructed sucessful means a transaction was started.
				\param rOutputAdapter Reference to a DWFOutputAdapter object.
			*/
			explicit tScopedTransaction(DWFOutputAdapter& rOutputAdapter)
				: _rOutputAdapter(rOutputAdapter)
				, _bSuccess(false)
			{
				rOutputAdapter.begin();
			}

			/** commit or rollback the transaction started in constructior
			*/
			~tScopedTransaction()
			{
				if (_bSuccess)
					_rOutputAdapter.commit();
				else
					_rOutputAdapter.rollback();
			}

			/** Mark the started transaction succeeded or failed.  
				\param bSuccess true means success, false means failed.
			*/
			void setSuccess(bool bSuccess = true)
			{
				_bSuccess = bSuccess;
			}
		private:
			DWFOutputAdapter& _rOutputAdapter;
			bool _bSuccess;

		private: //disable copy constructor and assignment
			tScopedTransaction(const tScopedTransaction&);
			tScopedTransaction& operator=(const tScopedTransaction&);
		};

		virtual ~DWFOutputAdapter(){}

		/** \param src the data tBlock of serialization
			\pre !src.empty()
			\param old the old hHandle of the serialized object
			\return new hHandle of the serialized object
			\post return != kInvalidHandle
			\throw any
		*/
		virtual tHandleType save(const tBufferType& src, tHandleType old) = 0;

		/**	start a transaction
		\throw any
		*/
		virtual void begin() {};
		/**
		finish the transaction
		\throw none
		*/
		virtual void commit() {};

		/**
		rollback the transaction
		\throw none
		*/
		virtual void rollback() {};
	};

	/**
		This class intends to combine the functions of both DWFInputAdapter and DWFOutputAdapter
	*/
	struct DWFPagingAdapter : DWFInputAdapter, DWFOutputAdapter
	{
		typedef long tHandleType; ///< hHandle type
		static const tHandleType kInvalidHandle = 0;///< invalid hHandle
		typedef std::vector<unsigned char> tBufferType;	///< _buffer type
		typedef DWFOutputAdapter::tScopedTransaction tScopedTransaction;

		virtual ~DWFPagingAdapter(){}

		///\copydoc DWFInputAdapter::load(tHandleType hHandle, tBufferType& buf)
		virtual void load(tHandleType, tBufferType& ) = 0;

		///\copydoc DWFOutputAdapter::save(const tBufferType& src, tHandleType old)
		virtual tHandleType save(const tBufferType& src, tHandleType old) = 0;

		///\copydoc DWFOutputAdapter::begin()
		virtual void begin() {};

		///\copydoc DWFOutputAdapter::commit()
		virtual void commit() {};

		///\copydoc DWFOutputAdapter::rollback()
		virtual void rollback() {};
	};	
}
#endif //end _DWFCORE_ADAPTER_H
