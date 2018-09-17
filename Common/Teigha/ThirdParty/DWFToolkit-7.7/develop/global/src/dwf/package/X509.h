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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/package/X509.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//

#ifndef _DWFTK_X509_H
#define _DWFTK_X509_H

///
///\file        dwf/package/X509.h
///\brief       This file contains the class declarations for X.509 signature data elements.
///

#include "dwfcore/STL.h"
#include "dwfcore/Vector.h"
#include "dwfcore/SkipList.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"



namespace DWFToolkit
{




///
///\class   X509DataItem     dwf/package/X509.h     "dwf/package/X509.h"
///\brief   a X509DataItem ...
///\since       7.3.0
///
///         A X509DataItem ...
///
class X509DataItem
{
public:
    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    X509DataItem()
        throw()
    {
    }

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    virtual ~X509DataItem()
        throw()
    {
    }

#ifndef DWFTK_READ_ONLY

    //
    // Serializes just the data item
    //
    virtual void serialize( DWFXMLSerializer& /*rSerializer*/, const DWFString& /*zNamespace*/ )
        throw( DWFException )
    {;}

#endif

private:
};

///
///\class   X509IssuerSerial     dwf/package/X509.h     "dwf/package/X509.h"
///\brief   a X509IssuerSerial ...
///\since       7.3.0
///
///         A X509IssuerSerial ...
///
class X509IssuerSerial : public X509DataItem
{
public:
    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    X509IssuerSerial()
        throw()
        : _zX509IssuerName("")
        ,_nX509SerialNumber(-1)
    {
    }

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    virtual ~X509IssuerSerial()
        throw()
    {
    }

    ///
    ///         Returns the \b DWFString representation of this Issuer Name.
    ///
    ///\return  A reference to the DWFString representation of this Issuer Name.
    ///\throw   None
    ///
    virtual const DWFString& issuerName() const
        throw()
    {
        return _zX509IssuerName;
    }

    ///
    ///         Sets the string to be used for representing this Issuer Name.
    ///
    ///\param   zName  The string to be used for representing this Issuer Name.
    ///\throw   None
    ///
    virtual void setIssuerName(const DWFString& zName)
        throw()
    {
        _zX509IssuerName = zName;
    }

    ///
    ///         Returns the Issuer Serial Number.
    ///
    ///\return  The Issuer Serial Number.
    ///\throw   None
    ///
    virtual const long serialNumber() const
        throw()
    {
        return _nX509SerialNumber;
    }

    ///
    ///         Sets the Issuer Serial Number.
    ///
    ///\param   nNumber  The Issuer Serial Number.
    ///\throw   None
    ///
    virtual void setSerialNumber(const long nNumber)
        throw()
    {
        _nX509SerialNumber = nNumber;
    }

#ifndef DWFTK_READ_ONLY

    //
    // Serializes just the data item
    //
    _DWFTK_API
    virtual void serialize( DWFXMLSerializer& rSerializer, const DWFString& zNamespace )
        throw( DWFException );

#endif

private:
    DWFString   _zX509IssuerName;
    long        _nX509SerialNumber;

};

///
///\class   X509SKI     dwf/package/X509.h     "dwf/package/X509.h"
///\brief   a X509SKI ...
///\since       7.3.0
///
///         A X509SKI transmits an X509 Subject Key Identifier string.
///
class X509SKI : public X509DataItem
{
public:
    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    X509SKI()
        throw()
        : _zSubjectKeyIdentifier()
    {
    }

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    virtual ~X509SKI()
        throw()
    {
    }

    ///
    ///         Returns the \b DWFString representation of the Subject Key Identifier.
    ///
    ///\return  A reference to the DWFString representation of the Subject Key Identifier.
    ///\throw   None
    ///
    virtual const DWFString& subjectKeyIdentifier () const
        throw()
    {
        return _zSubjectKeyIdentifier;
    }

    ///
    ///         Sets the string to be used for representing the Subject Key Identifier.
    ///
    ///\param   zSubjectKeyIdentifier  The string to be used for representing the Subject Key Identifier.
    ///\throw   None
    ///
    virtual void setSubjectKeyIdentifier(const DWFString& zSubjectKeyIdentifier )
        throw()
    {
        _zSubjectKeyIdentifier  = zSubjectKeyIdentifier ;
    }

#ifndef DWFTK_READ_ONLY

    //
    // Serializes just the data item
    //
    _DWFTK_API
    virtual void serialize( DWFXMLSerializer& rSerializer, const DWFString& zNamespace )
        throw( DWFException );

#endif

private:
    DWFString   _zSubjectKeyIdentifier;
};

///
///\class   X509SubjectName     dwf/package/X509.h     "dwf/package/X509.h"
///\brief   a X509SubjectName ...
///\since       7.3.0
///
///         A X509SubjectName ...
///
class X509SubjectName : public X509DataItem
{
public:
    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    X509SubjectName()
        throw()
        : _zName()
    {
    }

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    virtual ~X509SubjectName()
        throw()
    {
    }

    ///
    ///         Returns the \b DWFString representation of this Subject Name.
    ///
    ///\return  A reference to the DWFString representation of this Subject Name.
    ///\throw   None
    ///
    virtual const DWFString& subjectName() const
        throw()
    {
        return _zName;
    }

    ///
    ///         Sets the string to be used for representing this Subject Name.
    ///
    ///\param   zName  The string to be used for representing this Subject Name.
    ///\throw   None
    ///
    virtual void setSubjectName(const DWFString& zName)
        throw()
    {
        _zName = zName;
    }

#ifndef DWFTK_READ_ONLY

    //
    // Serializes just the data item
    //
    _DWFTK_API
    virtual void serialize( DWFXMLSerializer& rSerializer, const DWFString& zNamespace )
        throw( DWFException );

#endif

private:
    DWFString _zName;
};

///
///\class   X509Certificate     dwf/package/X509.h     "dwf/package/X509.h"
///\brief   a X509Certificate ...
///\since       7.3.0
///
///         A X509Certificate ...
///
class X509Certificate : public X509DataItem
{
public:
    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    X509Certificate()
        throw()
        : _zCert()
    {
    }

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    virtual ~X509Certificate()
        throw()
    {
    }

    ///
    ///         Returns the \b DWFString representation of this certificate.
    ///
    ///\return  A reference to the DWFString representation of this certificate.
    ///\throw   None
    ///
    virtual const DWFString& certificate() const
        throw()
    {
        return _zCert;
    }

    ///
    ///         Sets the string to be used for representing this certificate.
    ///
    ///\param   zCert  The string to be used for representing this certificate.
    ///\throw   None
    ///
    virtual void setCertificate(const DWFString& zCert)
        throw()
    {
        _zCert = zCert;
    }

#ifndef DWFTK_READ_ONLY

    //
    // Serializes just the data item
    //
    _DWFTK_API
    virtual void serialize( DWFXMLSerializer& rSerializer, const DWFString& zNamespace )
        throw( DWFException );

#endif

private:
    DWFString _zCert;
};


///
///\class   X509CRL     dwf/package/X509.h     "dwf/package/X509.h"
///\brief   a X509CRL ...
///\since       7.3.0
///
///         A X509CRL ...
///
class X509CRL : public X509DataItem
{
public:
    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    X509CRL()
        throw()
    {
    }

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    virtual ~X509CRL()
        throw()
    {
    }

    ///
    ///         Returns the \b DWFString representation of this CRL.
    ///
    ///\return  A reference to the DWFString representation of this CRL.
    ///\throw   None
    ///
    virtual const DWFString& CRL() const
        throw()
    {
        return _zCRL;
    }

    ///
    ///         Sets the string to be used for representing this CRL.
    ///
    ///\param   zCRL  The string to be used for representing this CRL.
    ///\throw   None
    ///
    virtual void setCRL(const DWFString& zCRL)
        throw()
    {
        _zCRL = zCRL;
    }

#ifndef DWFTK_READ_ONLY

    //
    // Serializes just the data item
    //
    _DWFTK_API
    virtual void serialize( DWFXMLSerializer& rSerializer, const DWFString& zNamespace )
        throw( DWFException );

#endif

private:
    DWFString   _zCRL;
};

///
///\class   X509Data     dwf/package/X509.h     "dwf/package/X509.h"
///\brief   a X509Data ...
///\since       7.3.0
///
///         A X509Data ...
///
class X509Data
{
public:
    ///
    ///\brief   This type defines a list of X509DataItem pointers.
    ///
    typedef DWFOrderedVector<X509DataItem*>   tX509DataItemVector;

public:
    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    X509Data()
        throw()
        : _oDataItemVector()
    {
    }

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~X509Data()
        throw();

    ///
    ///         Adds a data item to an X509 Data collection.
    ///
    ///\param pDataItem A pointer to the data item to add.
    ///\throw   None
    ///
    virtual void addDataItem(X509DataItem* pDataItem)
        throw()
    {
        _oDataItemVector.push_back(pDataItem);
    }

    
    ///
    ///         Returns an iterator providing access to the X509 Data Item collection.
    ///
    ///\return  An iterator providing access to the X509 Data Item collection
    ///\throw   None
    ///
    virtual tX509DataItemVector::Iterator* dataItems()
        throw()
    {
        return _oDataItemVector.iterator();
    }

#ifndef DWFTK_READ_ONLY

    //
    // Serializes all the data items
    //
    _DWFTK_API
    virtual void serialize( DWFXMLSerializer& rSerializer, const DWFString& zNamespace )
        throw( DWFException );

#endif

private:
    tX509DataItemVector _oDataItemVector;
};

}

#endif

