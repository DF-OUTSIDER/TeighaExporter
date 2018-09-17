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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/opc/CoreProperties.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//
//

#ifndef _DWFTK_OPCCOREPROPERTIES_H
#define _DWFTK_OPCCOREPROPERTIES_H

#include "dwf/opc/XMLPart.h"
#include "dwf/opc/reader/CorePropertiesReader.h"

namespace DWFToolkit
{

class OPCCoreProperties : public OPCXMLPart
                        , public OPCCorePropertiesReader
{

public:

    ///
    ///\brief   This type defines a name-value string pair for properties.
    ///
    typedef     std::pair<DWFString, DWFString>         tNameValuePair;
    ///
    ///\brief   This type defines an iterator over a collection of OPC property name-value pairs.
    ///
    typedef     DWFCachingIterator<tNameValuePair>      tPropertyIterator;

public:

    ///
    ///\brief   "CoreProperties.xml"
    ///
    static const char* const kzName;

public:

    ///
    ///             Constructor
    ///
    _DWFTK_API
    OPCCoreProperties()
        throw();

    _DWFTK_API
    virtual ~OPCCoreProperties()
        throw();

    ///
    ///\internal    Return the Autodesk defined schema ID that corresponds to the OPC core properties.
    ///
    ///\return      The Core Properties schema ID as defined by Autodesk.
    ///\throw       None
    ///
    _DWFTK_API
    const DWFString& getSchemaID()
        throw();

    _DWFTK_API
    void setPropertyKeywords( const DWFString& zValue )
        throw();

    _DWFTK_API
    DWFString keywords() const
        throw();

    _DWFTK_API
    void setPropertyContentType( const DWFString& zValue )
        throw();

    _DWFTK_API
    DWFString contentType() const
        throw();

    _DWFTK_API
    void setPropertyCategory( const DWFString& zValue )
        throw();

    _DWFTK_API
    DWFString category() const
        throw();

    _DWFTK_API
    void setPropertyVersion( const DWFString& zValue )
        throw();

    _DWFTK_API
    DWFString version() const
        throw();

    _DWFTK_API
    void setPropertyRevision( const DWFString& zValue )
        throw();

    _DWFTK_API
    DWFString revision() const
        throw();

    _DWFTK_API
    void setPropertyLastModifiedBy( const DWFString& zValue )
        throw();

    _DWFTK_API
    DWFString lastModifiedBy() const
        throw();

    _DWFTK_API
    void setPropertyLastPrinted( const DWFString& zValue )
        throw();

    _DWFTK_API
    DWFString lastPrinted() const
        throw();

    _DWFTK_API
    void setPropertyContentStatus( const DWFString& zValue )
        throw();

    _DWFTK_API
    DWFString contentStatus() const
        throw();

    _DWFTK_API
    void setPropertyCreator( const DWFString& zValue )
        throw();

    _DWFTK_API
    DWFString creator() const
        throw();

    _DWFTK_API
    void setPropertyIdentifier( const DWFString& zValue )
        throw();

    _DWFTK_API
    DWFString identifier() const
        throw();

    _DWFTK_API
    void setPropertyTitle( const DWFString& zValue )
        throw();

    _DWFTK_API
    DWFString title() const
        throw();

    _DWFTK_API
    void setPropertySubject( const DWFString& zValue )
        throw();

    _DWFTK_API
    DWFString subject() const
        throw();

    _DWFTK_API
    void setPropertyDescription( const DWFString& zValue )
        throw();

    _DWFTK_API
    DWFString description() const
        throw();

    _DWFTK_API
    void setPropertyLanguage( const DWFString& zValue )
        throw();

    _DWFTK_API
    DWFString language() const
        throw();

    _DWFTK_API
    void setPropertyCreated( int nYear, int nMonth, int nDay )
        throw( DWFException );

    _DWFTK_API
    void setPropertyCreated( const DWFString& zValue )
        throw();

    _DWFTK_API
    DWFString created() const
        throw();

    _DWFTK_API
    void setPropertyModified( int nYear, int nMonth, int nDay )
        throw( DWFException );

    _DWFTK_API
    void setPropertyModified( const DWFString& zValue )
        throw();

    _DWFTK_API
    DWFString modified() const
        throw();

    ///
    ///         Get an iterator over all the properties set.
    ///
    ///\return  Pointer to an iterator over name-value pairs. The iterator
    ///         must be deleted by the caller using DWFCORE_FREE_OBJECT.
    ///         If no properties are set, the return value may be NULL.
    ///
    _DWFTK_API
    OPCCoreProperties::tPropertyIterator* properties() const
        throw();

    ///
    ///         Get an iterator over all the properties in the OPC namespace
    ///         that have been set.
    ///
    ///\return  Pointer to an iterator over name-value pairs. The iterator
    ///         must be deleted by the caller using DWFCORE_FREE_OBJECT.
    ///         If no such properties are set, the return value may be NULL.
    ///
    _DWFTK_API
    OPCCoreProperties::tPropertyIterator* propertiesInOPCNamespace() const
        throw();

    ///
    ///         Get an iterator over all the properties in the Dublin Core 
    ///         namespace that have been set.
    ///
    ///\return  Pointer to an iterator over name-value pairs. The iterator
    ///         must be deleted by the caller using DWFCORE_FREE_OBJECT.
    ///         If no such properties are set, the return value may be NULL.
    ///
    _DWFTK_API
    OPCCoreProperties::tPropertyIterator* propertiesInDCNamespace() const
        throw();

    ///
    ///         Get an iterator over all the properties in the Dublin Core
    ///         Terms namespace that have been set.
    ///
    ///\return  Pointer to an iterator over name-value pairs. The iterator
    ///         must be deleted by the caller using DWFCORE_FREE_OBJECT.
    ///         If no such properties are set, the return value may be NULL.
    ///
    _DWFTK_API
    OPCCoreProperties::tPropertyIterator* propertiesInDCTermsNamespace() const
        throw();

public:

    ///
    ///\copydoc OPCCorePropertiesReader::provideKeywords()
    ///
    _DWFTK_API
    virtual DWFString provideKeywords( const DWFString& zValue )
        throw();

    ///
    ///\copydoc OPCCorePropertiesReader::provideContentType()
    ///
    _DWFTK_API
    virtual DWFString provideContentType( const DWFString& zValue )
        throw();

    ///
    ///\copydoc OPCCorePropertiesReader::provideCategory()
    ///
    _DWFTK_API
    virtual DWFString provideCategory( const DWFString& zValue )
        throw();

    ///
    ///\copydoc OPCCorePropertiesReader::provideVersion()
    ///
    _DWFTK_API
    virtual DWFString provideVersion( const DWFString& zValue )
        throw();

    ///
    ///\copydoc OPCCorePropertiesReader::provideRevision()
    ///
    _DWFTK_API
    virtual DWFString provideRevision( const DWFString& zValue )
        throw();

    ///
    ///\copydoc OPCCorePropertiesReader::provideLastModifiedBy()
    ///
    _DWFTK_API
    virtual DWFString provideLastModifiedBy( const DWFString& zValue )
        throw();

    ///
    ///\copydoc OPCCorePropertiesReader::provideLastPrinted()
    ///
    _DWFTK_API
    virtual DWFString provideLastPrinted( const DWFString& zValue )
        throw();

    ///
    ///\copydoc OPCCorePropertiesReader::provideContentStatus()
    ///
    _DWFTK_API
    virtual DWFString provideContentStatus( const DWFString& zValue )
        throw();

    ///
    ///\copydoc OPCCorePropertiesReader::provideCreator()
    ///
    _DWFTK_API
    virtual DWFString provideCreator( const DWFString& zValue )
        throw();

    ///
    ///\copydoc OPCCorePropertiesReader::provideIdentifier()
    ///
    _DWFTK_API
    virtual DWFString provideIdentifier( const DWFString& zValue )
        throw();

    ///
    ///\copydoc OPCCorePropertiesReader::provideTitle()
    ///
    _DWFTK_API
    virtual DWFString provideTitle( const DWFString& zValue )
        throw();

    ///
    ///\copydoc OPCCorePropertiesReader::provideSubject()
    ///
    _DWFTK_API
    virtual DWFString provideSubject( const DWFString& zValue )
        throw();

    ///
    ///\copydoc OPCCorePropertiesReader::provideDescription()
    ///
    _DWFTK_API
    virtual DWFString provideDescription( const DWFString& zValue )
        throw();

    ///
    ///\copydoc OPCCorePropertiesReader::provideLanguage()
    ///
    _DWFTK_API
    virtual DWFString provideLanguage( const DWFString& zValue )
        throw();

    ///
    ///\copydoc OPCCorePropertiesReader::provideCreated()
    ///
    _DWFTK_API
    virtual DWFString provideCreated( const DWFString& zValue )
        throw();

    ///
    ///\copydoc OPCCorePropertiesReader::provideModified()
    ///
    _DWFTK_API
    virtual DWFString provideModified( const DWFString& zValue )
        throw();

public:

#ifndef DWFTK_READ_ONLY

    ///
    ///\copydoc OPCXMLPart::serializeXML()
    ///
    _DWFTK_API
    void serializeXML( DWFXMLSerializer& rSerializer )
        throw( DWFException );

#endif

private:

    typedef std::map<DWFString, DWFString>      _tPropertyMap;

    _tPropertyMap           _oOPCProperties;
    _tPropertyMap           _oDCProperties;
    _tPropertyMap           _oDCTermsProperties;

private:

    DWFString _getProperty( const _tPropertyMap& rMap,
                            const DWFString& zKey ) const
        throw();

    DWFString _getDateString( int nYear, int nMonth, int nDay ) const
        throw( DWFException );

private:

    //
    // Not implemented
    //
    OPCCoreProperties( const OPCCoreProperties& );
    OPCCoreProperties& operator=( const OPCCoreProperties& );

};

//
// required for win32 dll external linkage
//
#ifdef  _DWFCORE_WIN32_SYSTEM
#ifndef DWFTK_STATIC

//DNT_START

_declspec(selectany) const char* const OPCCoreProperties::kzName = "CoreProperties.xml";

//DNT_END

#endif
#endif

}

#endif
