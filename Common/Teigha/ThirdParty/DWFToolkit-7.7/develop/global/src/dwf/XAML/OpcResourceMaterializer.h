//
//  Copyright (c) 2006 by Autodesk, Inc.
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

///
/// \file       dwf/XAML/OpcResourceMaterializer.h

#ifndef OpcResourceMaterializer_H
#define OpcResourceMaterializer_H

#include "XAML/XamlCore.h"

#include "whiptk/whip_toolkit.h"
#include "dwfcore/String.h"
#include "dwfcore/InputStream.h"

/// \class WT_OpcResourceMaterializer
/// Provides a mechanism whereby resources can be made available, used by  
/// WT_XAML_* objects to pull binary data out of a DWFx package resource 
/// (the data is then contained in the WT_XAML_* object.)
class XAMLTK_API WT_OpcResourceMaterializer
{
protected:
	WT_OpcResourceMaterializer() {}

public:
	virtual ~WT_OpcResourceMaterializer() {}

	/** Callback interface used to retrieve binary data stored in an OPC (DWFx) package.  Client code 
      * needs to call this, providing the path attribute which references the content to be provided
      * in the input stream returned.
	  * \param ppStream (Output param) Pointer to the stream from which the binary data can be read
	  * \param rzPath (Output param) The OPC internal package path which contains the data
	  * \note The caller assumes ownership of the stream and should destroy it when done.
	  * \return WT_Result::Success if all is well
	  */
	virtual WT_Result getPartInputStream(
		const DWFCore::DWFString& rzPath, 
		DWFCore::DWFInputStream** ppStream) = 0;

};
#endif // OpcResourceMaterializer_H
