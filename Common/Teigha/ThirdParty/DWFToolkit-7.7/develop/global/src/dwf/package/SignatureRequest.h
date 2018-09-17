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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/package/SignatureRequest.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//

#ifndef _DWFTK_SIGNATURE_REQUEST_H
#define _DWFTK_SIGNATURE_REQUEST_H

///
///\file        dwf/package/SignatureRequest.h
///\brief       This file contains the DWFSignatureRequest class declaration.
///

#include "dwfcore/STL.h"
#include "dwfcore/DWFXMLSerializer.h"
#include "dwfcore/Vector.h"
#include "dwfcore/SkipList.h"
#include "dwfcore/DigestOutputStream.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"
#include "dwf/package/Signature.h"


namespace DWFToolkit
{

// forward decl
class DWFResource;
class DWFSection;


///
///\ingroup     dwfpackage
///
///\class       DWFSignatureRequest     dwf/package/SignatureRequest.h      "dwf/package/SignatureRequest.h"
///\brief       A DWFSignatureRequest is used to ...
///\since       7.3.0
///
///             The DWFSignatureRequest is used to ...
///
class DWFSignatureRequest
            : public DWFSignature
#ifndef DWFTK_READ_ONLY
            , public DWFXMLSerializable
#endif
            _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{
    //
    // During writing, PackageWriter needs access to this class.
    //
    friend class DWFPackageWriter;
    friend class DWF6PackageWriter;

public:
    ///
    ///\brief   This type defines a list of DWFSignatureRequest pointers.
    ///
    typedef DWFOrderedVector<DWFSignatureRequest*>             tList;
    ///
    ///\brief   This type defines a mapped collection of DWFSignatureRequest pointers.
    ///
    typedef DWFStringKeySkipList<DWFSignatureRequest*>         tMap;
    ///
    ///\brief   This type defines a caching iterator on a collection of DWFSignatureRequest pointers,
    ///         that caches the pointers in its own container.
    ///
    typedef DWFCachingIterator<DWFSignatureRequest*>           tCachingIterator;
    ///
    ///\brief   This type defines a basic iterator on a collection of DWFSignatureRequest pointers.
    ///
    typedef DWFIterator<DWFSignatureRequest*>                  tIterator;

public:

    ///
    ///\class   Reference     dwf/package/SignatureRequest.h     "dwf/package/SignatureRequest.h"
    ///\brief   a Reference ...
    ///\since       7.3.0
    ///
    ///         A Reference ...
    ///
    class Reference : public DWFSignature::Reference
    {
        friend class DWFSignatureRequest;

    public:
        ///
        ///         Constructor
        ///
        ///\throw   None
        ///
        Reference()
            throw()
            : _pDigestOutputStream(NULL)
        {
            ;
        }

        ///
        ///         Destructor
        ///
        ///\throw   None
        ///
        virtual ~Reference()
            throw()
        {
            ;
        }

        ///
        ///         Returns the \b DWFDigestOutputStream that will be used by this Reference
        //          for calculation of its digest.
        ///
        ///\return  A pointer to the DWFDigestOutputStream used by this Reference.
        ///\throw   None
        ///
        DWFDigestOutputStream* digestOutputStream()
            throw()
        {
            return _pDigestOutputStream;
        }

        ///
        ///         Sets the \b DWFDigestOutputStream that will be used by this Reference
        //          for calculation of its digest.
        ///
        ///\param   pDigestOutputStream  A pointer to the DWFDigestOutputStream to be used by this Reference.
        ///\throw   None
        ///
        void setDigestOutputStream(DWFDigestOutputStream* pDigestOutputStream)
            throw( DWFException )
        {
            if(_pDigestOutputStream != NULL )
            {
                _DWFCORE_THROW( DWFInvalidArgumentException, /*NOXLATE*/ L"Can't reassign digest streams." );
            }

            _pDigestOutputStream = pDigestOutputStream;
        }


        ///
        ///         Returns the DWFSignatureRequest that this Reference belongs to.
        ///
        ///\throw   None
        ///
        DWFSignatureRequest* signatureRequest()
        {
            return dynamic_cast<DWFSignatureRequest*>(DWFSignature::Reference::signature());
        }


    protected:
        ///
        ///         Sets the DWFSignatureRequest that this Reference belongs to.
        ///
        ///\param   pSignatureRequest   A pointer to the DWFSignatureRequest that that this Reference belongs to.
        ///\throw   None
        ///
        void setSignatureRequest(DWFSignatureRequest* pSignatureRequest)
            throw( DWFException )
        {
            if(signatureRequest() != NULL )
            {
                _DWFCORE_THROW( DWFInvalidArgumentException, /*NOXLATE*/ L"Can't re-use References." );
            }

            DWFSignature::Reference::setSignature(pSignatureRequest);
        }

    private:

        // The digest stream which will be used to compute our digest
        DWFDigestOutputStream*  _pDigestOutputStream;
    };

    ///
    ///\class   ConcreteReference     dwf/package/SignatureRequest.h     "dwf/package/SignatureRequest.h"
    ///\brief   a ConcreteReference ...
    ///\since       7.3.0
    ///
    ///         A ConcreteReference ...
    ///
    class ConcreteReference : public Reference
    {
    public:
        ///
        ///         Constructor
        ///
        ///\throw   None
        ///
        _DWFTK_API
        ConcreteReference()
            throw();

        ///
        ///         Destructor
        ///
        ///\throw   None
        ///
        _DWFTK_API
        virtual ~ConcreteReference()
            throw();

        ///
        ///         Returns the \b DWFResource that this Reference references.
        ///
        ///\return  A pointer to the DWFResource referred to by this Reference.
        ///\throw   None
        ///
        virtual const DWFResource* resource() const
            throw();

        ///
        ///         Sets the \b DWFResource that this Reference references.
        ///
        ///\return  A pointer to the DWFResource to be referred to by this Reference.
        ///\throw   None
        ///
        virtual void setResource(const DWFResource* pResource)
            throw();

        ///
        ///\copydoc DWFSignature::Reference::URI()
        ///
        virtual const DWFString& URI() const
            throw( DWFException );

    private:
        const DWFResource* _pResource;
    };

    ///
    ///\class   ForwardReference     dwf/package/SignatureRequest.h     "dwf/package/SignatureRequest.h"
    ///\brief   a ForwardReference ...
    ///\since       7.3.0
    ///
    ///         A ForwardReference ...
    ///
    class ForwardReference : public Reference
    {
    };

    ///
    ///\class   ManifestReference     dwf/package/SignatureRequest.h     "dwf/package/SignatureRequest.h"
    ///\brief   a ManifestReference ...
    ///\since       7.3.0
    ///
    ///         A ManifestReference ...
    ///
    class ManifestReference : public ForwardReference
    {
    public:
        ///
        ///         Constructor
        ///
        ///\throw   None
        ///
        ManifestReference()
            throw()
        {
            setURI(L"manifest.xml");
        }

        ///
        ///         Destructor
        ///
        ///\throw   None
        ///
        virtual ~ManifestReference()
            throw()
        {
            ;
        }

    };

    ///
    ///\class   SectionDescriptorReference     dwf/package/SignatureRequest.h     "dwf/package/SignatureRequest.h"
    ///\brief   a SectionDescriptorReference ...
    ///\since       7.3.0
    ///
    ///         A SectionDescriptorReference ...
    ///
    class SectionDescriptorReference : public ForwardReference
    {
    public:
        ///
        ///         Constructor
        ///
        ///\throw   None
        ///
        SectionDescriptorReference()
            throw()
            : _pSection(NULL)
        {
        }

        ///
        ///         Destructor
        ///
        ///\throw   None
        ///
        virtual ~SectionDescriptorReference()
            throw()
        {
        }

        ///
        ///         Returns the \b DWFSection that this SectionDescriptorReference references.
        ///
        ///\return  A pointer to the DWFSection referred to by this SectionDescriptorReference.
        ///\throw   None
        ///
        virtual const DWFSection* section() const
            throw()
        {
            return _pSection;
        }

        ///
        ///         Sets the \b DWFSection that this SectionDescriptorReference references.
        ///
        ///\return  A pointer to the DWFSection to be referred to by this SectionDescriptorReference.
        ///\throw   None
        ///
        virtual void setSection(const DWFSection* pSection)
            throw()
        {
            _pSection = pSection;
        }

    private:
        const DWFSection*    _pSection;
    };

    ///
    ///\class   SignatureRequestReference     dwf/package/SignatureRequest.h     "dwf/package/SignatureRequest.h"
    ///\brief   a SignatureRequestReference ...
    ///\since       7.3.0
    ///
    ///         A SignatureRequestReference ...
    ///
    class SignatureRequestReference : public ForwardReference
    {
    public:
        ///
        ///         Constructor
        ///
        ///\throw   None
        ///
        SignatureRequestReference(const DWFSignatureRequest* pSignatureRequest=NULL)
            throw()
            : _pSignatureRequest(pSignatureRequest)
        {
        }

        ///
        ///         Destructor
        ///
        ///\throw   None
        ///
        virtual ~SignatureRequestReference()
            throw()
        {
        }

        ///
        ///         Returns the \b DWFSignatureRequest that this SignatureRequestReference references.
        ///
        ///\return  A pointer to the DWFSignatureRequest referred to by this SignatureRequestReference.
        ///\throw   None
        ///
        virtual const DWFSignatureRequest* signatureRequest() const
            throw()
        {
            return _pSignatureRequest;
        }

        ///
        ///         Sets the \b DWFSignatureRequest that this SignatureRequestReference references.
        ///
        ///\return  A pointer to the DWFSignatureRequest to be referred to by this SignatureRequestReference.
        ///\throw   None
        ///
        virtual void setSignatureRequest(const DWFSignatureRequest* pSignatureRequest)
            throw()
        {
            _pSignatureRequest = pSignatureRequest;
        }

    private:
        const DWFSignatureRequest*    _pSignatureRequest;
    };

    ///
    ///\class   ContentReference     dwf/package/SignatureRequest.h     "dwf/package/SignatureRequest.h"
    ///\brief   a ContentReference ...
    ///\since       7.3.0
    ///
    ///         A ContentReference ...
    ///
    class ContentReference : public ForwardReference
    {
    public:
        ///
        ///         Constructor
        ///
        ///\throw   None
        ///
        ContentReference(const DWFContent* pContent=NULL)
            throw()
            : _pContent(pContent)
        {
        }

        ///
        ///         Destructor
        ///
        ///\throw   None
        ///
        virtual ~ContentReference()
            throw()
        {
        }

        ///
        ///         Returns the \b DWFContent that this ContentReference references.
        ///
        ///\return  A pointer to the DWFContent referred to by this ContentReference.
        ///\throw   None
        ///
        virtual const DWFContent* content() const
            throw()
        {
            return _pContent;
        }

        ///
        ///         Sets the \b DWFContent that this ContentReference references.
        ///
        ///\return  A pointer to the DWFContent to be referred to by this ContentReference.
        ///\throw   None
        ///
        virtual void setContent(const DWFContent* pContent)
            throw()
        {
            _pContent = pContent;
        }

    private:
        const DWFContent*    _pContent;
    };



public:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    DWFSignatureRequest()
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFSignatureRequest()
        throw();

    ///
    ///         Sets the private key to be used for signing.
    ///
    ///\param   pPrivateKey  A pointer to the PrivateKey.
    ///\throw   None
    ///
    _DWFTK_API
    void setPrivateKey(DWFCryptoKey* pPrivateKey)
        throw();


#ifndef DWFTK_READ_ONLY

    ///
    ///\copydoc DWFXMLSerializable::serializeXML()
    ///
    _DWFTK_API
    virtual void serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
        throw( DWFException );

#endif


private:

    ///
    ///         Returns the private key to be used.
    ///
    ///\return  A pointer to the specified PrivateKey.
    ///\throw   None
    ///
    DWFCryptoKey* privateKey() const
        throw()
    {
        return _pPrivateKey;
    }


    //
    // Returns the resource that will be associated with this request.
    //
    // The resource will be created the first time this method is called.
    //
    DWFResource* resource()
        throw();


private:
    float                   _nVersion;
    DWFResource*            _pResource;
    DWFCryptoKey*           _pPrivateKey;


private:

    DWFSignatureRequest( const DWFSignatureRequest& );
    DWFSignatureRequest& operator=( const DWFSignatureRequest& );
};

}

#endif

