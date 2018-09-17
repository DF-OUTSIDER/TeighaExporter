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

#ifndef _DWFCORE_PAGING_FORWARD_H
#define _DWFCORE_PAGING_FORWARD_H
#include "dwfcore/Core.h"
#include "dwfcore/STL.h"
#include <memory>
namespace DWFCore
{
	/// memory management class, and this class is designed as a singleton
	class DWFMemoryManager;

	//
	// fwd decl
	//
	class DWFThreadMutex;

	/// This class intends to be called when the tStubInfo node is acquired or released.
	/// It intends to be used by DWFMemoryManager only
	class DWFPagingStrategyBase;

	//
	// fwd decl
	//
	class DWFFile;
}
#endif
