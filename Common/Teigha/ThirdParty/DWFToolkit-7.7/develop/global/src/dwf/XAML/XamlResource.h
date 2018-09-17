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
#if !defined XAML_RESOURCE_H
#define XAML_RESOURCE_H

#include "dwfcore/DWFXMLSerializer.h"
#include "whiptk/whip_toolkit.h"
#include "whiptk/typedefs_defines.h"


class WT_XAML_File;

//
// XAML resource, whose definition is streamed out in a separate
// dictionary stream for lightweight referencing
//
class XamlResource
{

public:

    //
    // resource type, should map to a given class
    //
    typedef enum {
        eUserHatchBrushResource,
        eFixedPatternBrushResource,
        eUserPatternBrushResource,
        eImageBrushResource,
        eDecoratedLineStyleResource,
        eMacroVisualBrushResource
    } eResourceType;

    //
    // dtor
    //
    virtual ~XamlResource() {}

    //
    // comparison operator on resources
    //
    virtual bool operator==(
        const XamlResource& ) const = 0;     // comparee

    //
    // deep copy operation
    //
    virtual XamlResource *copy() const = 0;

    //
    // returns the resource type
    //
    virtual eResourceType resourceType() const = 0;

    //
    // outputs the resource as XAML for the given key and
    // serializer
    //
    // note : the implementing code must output the x:Key attribute
    //
    virtual WT_Result serializeResource(
        const wchar_t *,                // key, != 0
        WT_XAML_File &,                 // file
        DWFXMLSerializer &) const = 0;  // serializer
};

//
// lookup table keying resource instances by strings
//
// the resource instances are duplicated and kept alive until the dtor
// is called
//
// the lookup operation is always guaranteed to succeed and return a string
//
class XamlDictionary
{
public :

    //
    // ctor, wrapping an xml serializer
    //
    XamlDictionary(
        WT_XAML_File &);            // file

    //
    // dtor, releases the internal allocated resources
    //
    ~XamlDictionary();

    //
    // resource instance to key lookup (linear)
    //
    // the returned key string is guaranteed unique within this
    // dictionary and non-null
    //
    // if the instance is not yet keyed, it is deep-copied and
    // placed in the internal map under a newly allocated key string
    // it is then streamed out to the dictionary stream
    //
    const wchar_t * lookup(
        const XamlResource &what);  // resource to lookup  

private :

    //
    // private assignment operator, avoiding warning C4512
    //
    XamlDictionary &operator = (
        XamlDictionary & );         // assignee

protected :

    WT_Integer32                    _nKeyId;                // unique id sequence
    WT_XAML_File &                  _rFile;                 // file
    map<const wchar_t *, XamlResource *>  _oKeyToResource;  // string->ptr map
};

#endif //XAML_RESOURCE_H

