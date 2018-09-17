//
//  Copyright (c) 1996-2006 by Autodesk, Inc.
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
#if !defined XAML_ATTRIBUTE
#define XAML_ATTRIBUTE


#include "whiptk/whip_toolkit.h"
#include "whiptk/typedefs_defines.h"
#include "dwfcore/DWFXMLSerializer.h"
#include "dwfcore/Owner.h"

class WT_XAML_File;

template<class TC>
class XamlAttribute
{
public:
    XamlAttribute<TC>(void)
    : _bAbbreviatedMode(true)
    {
    }
    
    virtual ~XamlAttribute<TC>(void) {}

    // copy constructor
	XamlAttribute<TC>( const TC& r ) {}
    
    // assignment operator
    virtual TC& operator=( const TC& ) = 0;
    
    // serialize as an XML attribute
    virtual WT_Result serializeAttribute( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const = 0;

    // serialize as an XML element (verbose)
    virtual WT_Result serializeElement( WT_XAML_File &, DWFCore::DWFXMLSerializer * ) const = 0;
    
    // returnes whether it's okay to serialize as an XML attribute ( assume it's always valid verbose )
    virtual bool validAsAttribute( void ) const = 0;    
    
private:
    bool _bAbbreviatedMode;
};

#endif //XAML_ATTRIBUTE

