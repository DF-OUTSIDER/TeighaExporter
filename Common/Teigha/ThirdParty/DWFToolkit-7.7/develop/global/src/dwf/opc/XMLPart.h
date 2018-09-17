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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/opc/XMLPart.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//
//

#ifndef _DWFTK_OPCXMLPART_H
#define _DWFTK_OPCXMLPART_H

#include "dwf/opc/Part.h"
#include "dwfcore/TempFile.h"

namespace DWFToolkit
{

class OPCXMLPart : public OPCPart
{

public:

    _DWFTK_API
    OPCXMLPart()
        throw();

    _DWFTK_API
    virtual ~OPCXMLPart()
        throw();

    _DWFTK_API
    virtual DWFInputStream* getInputStream()
        throw( DWFException );

	//
	// These two interfaces are for extension in the furture.
	// Currently, use file as default.
	//
	_DWFTK_API
	virtual void setUseFileStream(bool useFile)
	{
		_useFileStream = useFile;
	}

	_DWFTK_API
	virtual	bool getUseFileStream( )
	{
		return _useFileStream;
	}

#ifndef DWFTK_READ_ONLY

    ///
    ///         Write an XML representation of the object into a stream.
    ///
    ///\param   rSerializer     The object that accepts the serialization of the object.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void serializeXML( DWFXMLSerializer& rSerializer )
        throw( DWFException ) = 0;

#endif

protected:

    //
    // Buffer to which the XML is written for serialization of this content.
    //
    char*           _pBuffer;

	//
	//	To collect all temp files.
	//
	std::list<DWFTempFile*>    _lTempFiles; 
	//
	// A flag to determine if to use TempFile as output and input file stream
	// true: use temp file
	// false: use buffer stream
	//
	bool			_useFileStream;

};

}

#endif
