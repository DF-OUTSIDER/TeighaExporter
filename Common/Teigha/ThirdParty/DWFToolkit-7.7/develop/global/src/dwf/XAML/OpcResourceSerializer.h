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
/// \author     Jeffrey R. Klug
///  created    11-Oct-2006

#ifndef OpcResourceSerializer_H
#define OpcResourceSerializer_H

#include "XAML/XamlCore.h"


#include "whiptk/whip_toolkit.h"
#include "dwfcore/String.h"
#include "dwfcore/OutputStream.h"

/// \class WT_OpcResourceSerializer
/// Provides a mechanism whereby resources can be stored, used by 
/// WT_XAML_* objects to store binary data into a DWFx package 
/// resource (the external resource is then referenced by the XML
/// representation of the WT_XAML_* object.)
class XAMLTK_API WT_OpcResourceSerializer
{
protected:
	WT_OpcResourceSerializer() {}

public:
	virtual ~WT_OpcResourceSerializer() {}

	/** Callback interface used to store binary data in an OPC (DWFx) package.  Client code needs to call 
      * this prior to writing data, as the stream returned is an output stream ostensibly suitable for 
      * inclusion in an OPC package.
	  * \param rzMimeType (Input param) MIME type describing the type of data contained in the stream
	  * \param rzPath (Output param) The OPC internal package path to be used as the XAML reference 
	  * \param ppStream (Input param) The stream to which the binary data will be written
	  * \note The caller assumes ownership of the stream and should destroy it when done.
	  * \return WT_Result::Success if all is well
	  */ 
	virtual WT_Result getPartOutputStream(
		const DWFCore::DWFString& rzMimeType,
        DWFCore::DWFString& rzPath,
        DWFCore::DWFOutputStream** ppStream) = 0;

};
#endif // OpcResourceSerializer_H
