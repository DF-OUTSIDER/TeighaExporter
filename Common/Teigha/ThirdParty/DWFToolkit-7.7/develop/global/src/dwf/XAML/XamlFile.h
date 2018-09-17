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


#if !defined WT_XAML_FILE_HEADER
#define WT_XAML_FILE_HEADER

#include <set>

//using namespace std;
#include "dwf/Toolkit.h"
#include "dwf/package/XML.h"
#include "dwfcore/Core.h"
#include "dwfcore/UUID.h"
#include "dwfcore/StreamFileDescriptor.h"
#include "dwfcore/FileInputStream.h"
#include "dwfcore/FileOutputStream.h"
#include "dwfcore/StreamFileDescriptor.h"
#include "dwfcore/XML.h"
#include "dwfcore/DWFXMLSerializer.h"
#include "dwfcore/Exception.h"

// DWFXml stuff and expat might be needed to be moved to the DWFCore
#include "whiptk/whip_toolkit.h"
#include "XAML/XamlPath.h"
#include "XAML/XamlCanvas.h"
#include "XAML/XamlRendition.h"
#include "XAML/XamlResource.h"
#include "XAML/OpcResourceSerializer.h"
#include "XAML/OpcResourceMaterializer.h"
#include "XAML/XamlW2XParser.h"
#include "XAML/XamlParser.h"
#include "XAML/xversion.h"
#include "XAML/MemoryBuffer.h"

using namespace DWFCore;

#define LONG_MAX_FLOAT_MANTISSA             16777215.0f                           // 2^24 - 1 
#define LONG_TO_FLOAT_SCALE                 LONG_MAX_FLOAT_MANTISSA / (float)LONG_MAX       //
#define XAML_DPI_MULTIPLIER                 96.0f
#define DEFAULT_DPI                         400.0f

class WT_Units;
class WT_XAML_Units;
class WT_XAML_Macro_Definition;

class XAMLTK_API WT_XAML_File : public WT_File 
    , public XamlDrawableAttributes::Name::Provider
{
    friend class WT_XAML_Class_Factory;

protected:
    WT_XAML_File() throw(WT_Result);


public:

	class WT_XAML_ObjectList
	{
    private:

        //This private class is a node item in a doubly-linked list.  
        //We have two linked lists in the WT_XAML_ObjectList class;
        //One for attribures, and one for all other objects.  With
        //one exception (DWF_Header), Attributes always get emitted
        //first via the "get_next_object" method.
        class Node
        {
        private:
            Node **_head, **_tail;
        public:
            Node *prev, *next;
            WT_Object *object;

            Node();
            void init( WT_Object *pObject, Node **pHead, Node **pTail);
            void set_tail();
            void add_before( Node* );
            void remove();
        };

    public:
		WT_XAML_ObjectList();
		~WT_XAML_ObjectList();

		// insert the object in one of the internal linked lists,
        // based on a few rules (see the implementation for a 
        // walkthrough of the logic.)
		void insert( WT_Object* ) throw (DWFException);

		// performs an update of the "first incomplete" pointer,
        // starting a linear search from the given object's next
        // sibling - looking for the next incomplete object 
		void update_next_incomplete( WT_Object* );

		// peek front of virtually aggregated list
		WT_Object*& front();

		// removes front entity from the virtually aggregated list
		void pop_front();

		// number of objects in the virtually aggregated list
		size_t size() const;

		// is the virtually aggregated list empty
		bool empty() const;

		// find by id
		bool find_by_id( WT_Object::WT_ID,
						 std::multimap<WT_Object::WT_ID, WT_Object*>::iterator&, 
						 std::multimap<WT_Object::WT_ID, WT_Object*>::iterator& );

		// find by type
		bool find_by_type( WT_Object::WT_Type,
						   std::multimap<WT_Object::WT_Type, WT_Object*>::iterator&, 
						   std::multimap<WT_Object::WT_Type, WT_Object*>::iterator& );

		// find by id, first incomplete
		bool find_first_incomplete( WT_Object::WT_Type, WT_Object::WT_ID, WT_Object*& );
      

		// find by type, first incomplete
		bool find_first_incomplete( WT_Object::WT_Type, WT_Object*& );

	private:
        // removes objects from the search maps
		void _cull( WT_Object* pObj );

	private:
        // the first item is a pointer to the object list head node, 
        // the second is the tail node
        pair<Node*, Node*> _oObjects;
        // likewise, for the attribute list
        pair<Node*, Node*> _oAttributes;

        // A stack (pool) which tracks unused nodes.  The object list 
        // gets pretty busy, so it behooves us to keep the allocations 
        // to a minimum by recycling memory
        stack<Node*> _oNodePool;

        // Points to the first incomplete object in the object linked-list
        // list.  Incomplete here means that the referenced WT_Object is
        // marked as !materialized().  There are only a few such entities
        // that are partially defined in both the W2X and XAML, so it is
        // important that we manage the materialization flag properly, as
        // it is a factor in directing the insertion of Nodes referencing
        // WT_Objects in the linked list(s).
        Node *_pIncompleteObjects;
        Node *_pIncompleteAttributes;

        // The number of nodes/objects currently in the list.
        size_t _nObjects;

        // Special-case flag.  This is the only non-attribute which gets
        // sent out first.  It's the first thing the application must see
        // from the object list, so it gets special treatment.
        bool _bIsDwfHeaderSent;

        // multimap for searching for object ids.
		std::multimap<WT_Object::WT_ID, WT_Object*> _idmap;

        // multimap for searching for object types.
		std::multimap<WT_Object::WT_Type, WT_Object*> _typemap;

        // map to find a linked-list node given a WT_Object.
        std::map<WT_Object*, Node* > _nodemap;
	};

    class WT_XAML_Memory_File : public WT_FileExt
    {
    public:
        WT_XAML_Memory_File( DWFInputStream *pInputStream = NULL );
        ~WT_XAML_Memory_File();

        WT_Result process_stream_close (void);
        WT_Result process_stream_end_seek (void);
        WT_Result process_stream_open (void);
        WT_Result process_stream_read (int desired_bytes, int& bytes_read, void* buffer);
        WT_Result process_stream_seek (int distance, int& amount_seeked);
        WT_Result process_stream_tell (unsigned long *current_file_pointer_position);
        WT_Result process_stream_write(int size, void const* buffer);

        DWFInputStream *pSourceInputStream;
        DWFBufferInputStream *pInputStream;
        DWFBufferOutputStream *pOutputStream;
    };

public:
    typedef  map< const wchar_t*, const wchar_t*, tDWFWCharCompareLess >  tStringMap;
    typedef  map< WT_Integer32, WT_XAML_Macro_Definition* >               tMacroMap;
    typedef  map< const wchar_t*, WT_XAML_Font::CachedTTFFontData*, tDWFWCharCompareLess >      tCachedTTFFontMap;
    typedef  stack< WT_Object *>                                          tObjectStack; 

public:
    virtual ~WT_XAML_File() throw();

    // Serializers
    DWFXMLSerializer*&          xamlSerializer();
    DWFXMLSerializer*&          w2xSerializer();
    DWFXMLSerializer*&          xamlDictionarySerializer();

    // I/O (these pointers can be both set and retrieved through these methods)
    DWFOutputStream*&                       xamlStreamOut();
    DWFOutputStream*&                       w2xStreamOut();
    DWFInputStream*&                        xamlStreamIn();
    DWFInputStream*&                        w2xStreamIn();
    WT_OpcResourceSerializer*&              opcResourceSerializer();
    WT_OpcResourceMaterializer*&            opcResourceMaterializer();
    WT_XAML_W2X_Parser*&                    w2xParser();
    WT_XAML_Xaml_Parser*&                   xamlParser();

    //ObjectList access
	const WT_XAML_ObjectList&   object_list() const; 
	WT_XAML_ObjectList&		    object_list(); 
    void                        set_materialized( WT_Object* );

    const tObjectStack&         nested_object_stack() const;
    tObjectStack&               nested_object_stack();

    //Font methods
    const bool&                 obfuscate_embedded_fonts() const;
    bool&                       obfuscate_embedded_fonts();
    tCachedTTFFontMap&          cached_ttf_fonts();

    DWFOutputStream *&          xamlDictionaryStreamOut();
    DWFInputStream *&           xamlDictionaryStreamIn();

    // WT_File overrides
    virtual WT_Result           open();
    virtual WT_Result           close();
    virtual WT_Rendition &      desired_rendition();
    virtual WT_Rendition &      rendition();
    virtual WT_Result           get_next_object();
    virtual WT_Result           compress_write(int count, void const * data);


    //
    // page layout methods
    //
    // - definePageLayout() : must be called before the first drawable is serialized
    //   - may be called several times before a <Path>, <Glyph> or <Canvas> gets
    //     serialized
    //   - after that, an error code is returned
    //
    // - isLayoutValid() : returns true iif definePageLayout() was called with proper parameters
    //
    // - isTopLevelCanvasWritten() : returns true if the top-level canvas has been
    // committed to xml
    //   - in that case, definePageLayout() will always return an error code
    //
    // - innerPageTransform() : returns the 3x3 paper transform (used by the top-level
    // <Canvas> element)
    //
    // - flipPoint() : modifies the point so that it's paper projection will be
    // mirrored in y - should be called for all points by all drawables (otherwise the
    // result will be reversed in XPS)
    //
    // - convertToPaperSpace() : converts a lenght expressed in logical units into
    // page space units
    //
    WT_Result                   definePageLayout(const WT_Matrix2D &rTransform, double fWidth, double fHeight, double fScale);
    WT_Result                   writeTopLevelCanvas();
    bool                        isLayoutValid() const;
    bool                        isTopLevelCanvasWritten() const;
    const WT_Matrix2D &         innerPageTransform() const;  
    WT_Result                   flipPoint(WT_Point2D &);
    WT_Result                   unflipPoint(WT_Point2D &);
    float                       convertToPaperSpace(float length) const;

    //
    // overposting methods
    //
    void                        addToOverpostClip(const WT_Logical_Box &);
    bool                        isFittingInOverpostClip(const WT_Logical_Box &);

    // relative point utils     
    virtual WT_Point2D          de_update_current_point(WT_Point2D const & delta);
    virtual WT_Point2D          update_current_point(WT_Point2D const & new_point);
    virtual WT_Logical_Point    de_update_current_point(WT_Logical_Point const & delta);
    virtual WT_Logical_Point    update_current_point(WT_Logical_Point const & new_point);

    // utility functions
    static float                scaleLongToFloat(long n);
    static float                scaleToXamlResolution(float fDots);
    const DWFString &           getDictionaryResourcePath() const;
    XamlDictionary *            dictionary() const;
    WT_Result                   calculateDesiredFontStringSize( const WT_String& rString, double &rfWidth, double &rfHeight, double &rfBase, double &rfAscent ) const;
    WT_Result                   registerFontUri( const wchar_t* zFontName,  const wchar_t* zFontUri );
    const wchar_t*              resolveFontUri( const wchar_t* zFontName ) const;
    WT_Result                   registerMacro( const WT_XAML_Macro_Definition * );
    bool                        findMacro( WT_Integer32 nIndex, WT_XAML_Macro_Definition *& ) const;
    bool&                       serializingAsW2DContent();
    const bool &                serializingAsW2DContent() const;
    WT_File* &                  w2dContentFile();
    WT_File* const &            w2dContentFile() const;
    tMemoryBuffer*              getBuffer( size_t nMinLengthInBytes ) throw(DWFException);
    void                        releaseBuffer( tMemoryBuffer *pBuffer ) throw(DWFException);
    tMemoryBuffer*              exchangeBuffer( tMemoryBuffer*, size_t nMinLengthInBytes ) throw(DWFException);

    // file state
    const int&                  nameIndex() const;
    int&                        nameIndex();

    // version overrides            
    int                         toolkit_major_revision() const;
    int                         toolkit_minor_revision() const;

    // path delay logic
    virtual WT_Result           merge_or_delay_path(XamlPath const & current);
    void                        set_delayed_path(XamlPath *path);
    XamlPath *                  delayed_path() const;
    WT_Result                   dump_delayed_drawable();

    //RenditionSync tag started flag - used in all attributes serialization method
    bool                        isRenditionSyncStarted();
    void                        setRenditionSyncStarted(bool bRenditionSyncStarted);
    WT_Result                   serializeRenditionSyncStartElement();
    WT_Result                   serializeRenditionSyncEndElement();
    //Viewport serialization needs to start new Canvas element
    WT_Result                   serializeCanvasStartElement( bool bNesting=true );
    WT_Result                   serializeCanvasEndElement( bool bNesting=true );
    // path utilities
    void                        setPathData(const char* pData);
    const char*                 getPathData() const;
    bool                        isPathDataSet() const;
    WT_Result                   materializePathData();

    // refName string 
    const wchar_t*              nameIndexString();
    const DWFString&            nameIndexPrefix() const;
    DWFString&                  nameIndexPrefix();
    int                         parseNameIndex( const char* pName );

    // XamlDrawableAttributes::Name::Provider
    WT_Result                   provideName( XamlDrawableAttributes::Name *& );

private:
    DWFString                   generateUniquePrefix() const;
    bool                        isWriteMode() const;
    void                        setCurrentCmd(const char* pStartAddress);
    char                        getNextCommand();//const char* &pPathString);
    static WT_Result            DWFXamlStreamOpen(WT_File & file);
    static WT_Result            DWFXamlStreamClose (WT_File & file);

public:
    // String defines for each WhipTk class, overkill but we'll need most of them
#define CALLBACK_MACRO(class_name, class_lower) \
    static const char* const pcW2X##class_name##_Element;
    CALLBACK_LIST
#undef CALLBACK_MACRO

        //Name prefix
        static const wchar_t* const kpzName_Prefix;

private:

    // I/O
    DWFOutputStream*                    _pXamlStreamOut;
    DWFOutputStream*                    _pXamlDictionaryStreamOut;
    DWFOutputStream*                    _pW2xStreamOut;
    DWFInputStream*                     _pXamlStreamIn;
    DWFInputStream*                     _pXamlDictionaryStreamIn;
    DWFInputStream*                     _pW2xStreamIn;
    DWFXMLSerializer*                   _pXamlSerializer;
    DWFXMLSerializer*                   _pXamlDictionarySerializer;
    DWFXMLSerializer*                   _pW2xSerializer;
    WT_OpcResourceSerializer*           _pOpcResourceSerializer;
    WT_OpcResourceMaterializer*         _pOpcResourceMaterializer;
    WT_XAML_W2X_Parser*                 _pW2XParser;
    WT_XAML_Xaml_Parser*                _pXamlParser;

    // WT_File override data
    WT_XAML_Rendition           _oDesiredRendition;
    WT_XAML_Rendition*          _pRendition;
	WT_XAML_ObjectList			_oObjectList;
    tObjectStack                _oNestedObjectStack;

    // Page layout

    //
    // the page/reverse page transforms are the actual logical-space <->
    // final paper projections
    //
    // the inner page transform is just a translation matrix used to
    // inhibit the translation from the 2D graphics resource page transform
    // (added by the toolkit to the top-level xml element). This is
    // mandatory to preserve accuracy (the idea is to have a concatenated
    // page transform * inner transform with a 0 translation)
    //
    WT_Matrix2D                 _oPageTransform;
    WT_Matrix2D                 _oReversePageTransform;
    WT_Matrix2D                 _oInnerPageTransform;

    WT_Point2D                  _oPageSize;
    WT_Point2D                  _oTranslation;
    double                      _fYMirror;
    bool                        _bValidLayout;
    bool                        _bTopLevelCanvasWritten;


    // Overposting
    vector<WT_Logical_Box>      _oOverpostClip;

    // Path state
    XamlPath*                   _pDelayedPath;
    WT_Point2D                  _oCurrentPoint;

    // Flags
    bool                        _bIsCurrentlyDumpingDelayedPath;
    bool                        _bIsUnitsTransformSet;
    bool                        _bIsRenditionSyncStarted;
    bool                        _bIsNonNestedCanvasElementStarted;
    bool                        _bIsObfuscateEmbeddedFonts;
    bool                        _bIsEndOfDwfSent;
    bool                        _bSerializingMacroDefinition;

    // State
    int                         _nNameIndex;
    wchar_t*                    _pwcNameString;
    DWFString                   _zNamePrefix;

    // Utility objects
    DWFString                   _szDictionaryResourcePath;
    XamlDictionary *            _pDictionary;
    DWFUUID                     _oUUIDProvider;
    tStringMap                  _oFontRefNameMap;
    tMacroMap                   _oMacroMap;
    tCachedTTFFontMap           _oCachedTTFFontMap;

    WT_File*                    _pMacroDefinitionFile;
    tMemoryBufferMap            _oAvailableBuffers;
};

//Inlines
inline DWFXMLSerializer*&
WT_XAML_File::xamlSerializer()   
{ 
    return _pXamlSerializer; 
}

inline DWFXMLSerializer*&
WT_XAML_File::xamlDictionarySerializer()   
{ 
    return _pXamlDictionarySerializer; 
}

inline DWFXMLSerializer*&
WT_XAML_File::w2xSerializer()
{ 
    return _pW2xSerializer; 
}

inline DWFOutputStream*&
WT_XAML_File::xamlStreamOut()
{
    return _pXamlStreamOut;
}

inline DWFOutputStream*&
WT_XAML_File::xamlDictionaryStreamOut()
{
    return _pXamlDictionaryStreamOut;
}

inline DWFOutputStream*&
WT_XAML_File::w2xStreamOut()
{
    return _pW2xStreamOut;
}

inline DWFInputStream*&
WT_XAML_File::xamlStreamIn()
{
    return _pXamlStreamIn;
}

inline DWFInputStream*&
WT_XAML_File::xamlDictionaryStreamIn()
{
    return _pXamlDictionaryStreamIn;
}

inline DWFInputStream*&
WT_XAML_File::w2xStreamIn()
{
    return _pW2xStreamIn;
}

inline WT_XAML_W2X_Parser*&
WT_XAML_File::w2xParser()
{
    return _pW2XParser;
}

inline WT_XAML_Xaml_Parser*&
WT_XAML_File::xamlParser()
{
    return _pXamlParser;
}

inline const WT_XAML_File::WT_XAML_ObjectList&
WT_XAML_File::object_list() const
{
    return _oObjectList;
}

inline WT_XAML_File::WT_XAML_ObjectList&
WT_XAML_File::object_list()
{
    return _oObjectList;
}

inline const WT_XAML_File::tObjectStack&
WT_XAML_File::nested_object_stack() const
{
    return _oNestedObjectStack;
}

inline WT_XAML_File::tObjectStack&
WT_XAML_File::nested_object_stack()
{
    return _oNestedObjectStack;
}


inline bool
WT_XAML_File::isLayoutValid() const
{
    return _bValidLayout;
}

inline bool
WT_XAML_File::isTopLevelCanvasWritten() const
{
    return _bTopLevelCanvasWritten;
}

//
// note : to switch back to the initial layout scheme, return
// _oPageTransform, instead of _oInnerPageTransform
//
inline const WT_Matrix2D &
WT_XAML_File::innerPageTransform() const
{
    return _oInnerPageTransform;
}

inline const DWFString &
WT_XAML_File::getDictionaryResourcePath() const
{
    return _szDictionaryResourcePath;
}

inline XamlDictionary *
WT_XAML_File::dictionary() const
{
    return _pDictionary;
}

inline void 
WT_XAML_File::set_delayed_path(XamlPath *pPath) 
{   
    WD_Assert(!_pDelayedPath); 
    _pDelayedPath = pPath; 
}

inline XamlPath *
WT_XAML_File::delayed_path() const
{
    return _pDelayedPath; 
}

inline float
WT_XAML_File::scaleLongToFloat(long n)
{
    return (float)n * LONG_TO_FLOAT_SCALE; 
}

inline float
WT_XAML_File::scaleToXamlResolution(float fDots)
{
    return fDots / DEFAULT_DPI * XAML_DPI_MULTIPLIER; 
}

inline int const &
WT_XAML_File::nameIndex() const
{
    return _nNameIndex; 
}

inline int&
WT_XAML_File::nameIndex()
{
    return _nNameIndex; 
}

inline WT_Rendition &
WT_XAML_File::desired_rendition()
{
    return _oDesiredRendition; 
}

inline WT_Rendition &
WT_XAML_File::rendition()
{
    return *_pRendition; 
}

inline bool
WT_XAML_File::isWriteMode() const  
{ 
    return file_mode() != WT_File::File_Read && file_mode() != WT_File::Block_Read; 
}

inline WT_OpcResourceSerializer*&   
WT_XAML_File::opcResourceSerializer()
{
    return _pOpcResourceSerializer;
}

inline WT_OpcResourceMaterializer*& 
WT_XAML_File::opcResourceMaterializer()
{
    return _pOpcResourceMaterializer;
}

inline bool
WT_XAML_File::isRenditionSyncStarted()
{
    return _bIsRenditionSyncStarted;
}

inline void
WT_XAML_File::setRenditionSyncStarted(bool bRenditionSyncStarted)
{
    _bIsRenditionSyncStarted = bRenditionSyncStarted;
}

inline const bool&
WT_XAML_File::obfuscate_embedded_fonts() const
{
    return _bIsObfuscateEmbeddedFonts;
}

inline bool&
WT_XAML_File::obfuscate_embedded_fonts()
{
    return _bIsObfuscateEmbeddedFonts;
}

inline WT_XAML_File::tCachedTTFFontMap&
WT_XAML_File::cached_ttf_fonts()
{
    return _oCachedTTFFontMap;
}

inline bool&
WT_XAML_File::serializingAsW2DContent()
{
    return _bSerializingMacroDefinition;
}

inline const bool&
WT_XAML_File::serializingAsW2DContent() const
{
    return _bSerializingMacroDefinition;
}

inline WT_File* &
WT_XAML_File::w2dContentFile()
{
    return _pMacroDefinitionFile;
}

inline  WT_File* const &
WT_XAML_File::w2dContentFile() const
{
    return _pMacroDefinitionFile;
}

inline int
WT_XAML_File::toolkit_major_revision() const    
{    
    return WD_XAML_Toolkit_Major_Revision;    
}

inline int
WT_XAML_File::toolkit_minor_revision() const
{    
    return WD_XAML_Toolkit_Minor_Revision;    
}

#endif // WT_XAML_FILE_HEADER
