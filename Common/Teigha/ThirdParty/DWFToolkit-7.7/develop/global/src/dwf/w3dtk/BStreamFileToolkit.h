//
// Copyright (c) 2000 by Tech Soft 3D, LLC.
// The information contained herein is confidential and proprietary to
// Tech Soft 3D, LLC., and considered a trade secret as defined under
// civil and criminal statutes.  Tech Soft 3D shall pursue its civil
// and criminal remedies in the event of unauthorized use or misappropriation
// of its trade secrets.  Use of this information by anyone other than
// authorized employees of Tech Soft 3D, LLC. is granted only under a
// written non-disclosure agreement, expressly prescribing the scope and
// manner of such use.
//
//
//
//  Copyright (c) 1996-2006 by Autodesk, Inc.
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
//
// $Header: //DWF/Working_Area/Willie.Zhu/w3dtk/BStreamFileToolkit.h#1 $
//

#ifndef BBINFILETK_TOOLKIT
#define BBINFILETK_TOOLKIT

///
///\file        dwf/w3dtk/BStreamFileToolkit.h
///

#include "dwfcore/InputStream.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"


/*!
    Base class for any class whose objects may have ownership passed across DLL boundaries.
    Allocation & deallocation need to take place in the sane DLL scope (at least on Windows),
    this forces it to happen within the stream DLL.
*/
class BBINFILETK_API2 BControlledMemoryObject {
    public:
        void * operator new (size_t size);  //!< force allocation to happen in the stream toolkit's scope
        void   operator delete (void * p);  //!< force deallocation to happen in the stream toolkit's scope
};

class BBaseOpcodeHandler;


#ifndef DOXYGEN_SHOULD_SKIP_THIS

/*
   Buffers data so basic handlers can deal with simple Get/Put calls of basic data types (and arrays of those types).
   Handles both reading and writing, ZLIB compressed or uncompressed.
*/
class BBINFILETK_API2 Internal_Data_Accumulator {
    private:
        char *          m_pending_buffer;           /*< The whole burrito. */
        int             m_pending_buffer_allocated; /*< How big the buffer is (so we know when to reallocate) */
        char *          m_pending_position;         /*< Start of actual data to be processed.*/
        int             m_pending_size;             /*< Size of actual data. */

        char *          m_buffer_data;              /*< The buffer shown to the user (i.e. passed to GenerateBuffer or ParseBuffer) */
        int             m_buffer_size;              /*< The size of the user's buffer */

        int             m_failed_size;              /*< When a read fails because of insufficient data, how much space would have been required for success? */

        int             m_generated;                /*< How much has been written into the user's buffer (after considering compression).  */

        struct z_stream_s * m_z_stream;             /*< The control structure for ZLIB */
        bool            m_compressed;               /*< are we in compressed mode? */
        bool            m_writing;                  /*< are we compressing or decompressing? */
        int             m_original_size;            /*< size of buffer originally passed in */

    public:
        Internal_Data_Accumulator () : m_pending_buffer (0), m_pending_buffer_allocated (0),
                                       m_pending_position (0), m_pending_size (0),
                                       m_failed_size (0), m_generated (0),
                                       m_z_stream (0), m_compressed (false), m_writing (false) {}
        ~Internal_Data_Accumulator ();

        void        set_data (char * b, int s) alter { m_buffer_data = b; m_original_size = m_buffer_size = s; }
        void        save (void) alter;
        TK_Status   consume (void) alter;

        TK_Status   read (char alter * b, int s) alter;
        TK_Status   write (char const * b, int s) alter;
        TK_Status   lookat (char alter & b) alter;

        int         get_original_buffer_size(){return m_original_size; }

        void        restart (void) alter;
        void        clean (void) alter;
        int         unused (void) const     { return m_buffer_size; }
        int         generated (void) const  { return m_generated; }

        TK_Status   start_compression (void) alter;
        TK_Status   stop_compression (bool flush) alter;
        TK_Status   start_decompression (void) alter;
        TK_Status   stop_decompression (bool force) alter;
        bool        compressed (void) const { return m_compressed; }

        TK_Status   error (char const * msg = 0) const;
};


/*
    Provides index <-> key translation, plus storage of additional item-specific data, such as
    file offset and size and bounding volume.
*/
struct IT_Index_Key_Extra {
    int     m_variants[8][2];
    int     m_options;
    float   m_bounds[6];
};

class BBINFILETK_API2 Internal_Translator {
    friend class TK_Dictionary; // note, dictionary writer tied closely to this implementation
    private:
        // array -- index to key is trivial
        int                     m_size;
        int                     m_used;

        struct Index_Key_Pair {
            int                 m_index;
            ID_Key              m_key;
            IT_Index_Key_Extra *m_extra;
        } *                     m_pairs;

        enum Options {
            Bounds_Valid    = 0x0001,

            Extended        = 0x0080    // reserved
        };

        // hash of key to locate potential indices for key to index lookup
        struct Hash_Block {
            Hash_Block *    m_next;
            int             m_used;
            int             m_indices[32];
        } *                     m_blocks[1024];


    public:
        Internal_Translator () : m_size (0), m_used (0), m_pairs (0) { memset (m_blocks, 0, 1024*sizeof(void *)); }
        ~Internal_Translator ();

        TK_Status   add_pair (int index, ID_Key key) alter;
        TK_Status   add_variant (ID_Key key, int variant, int value1, int value2 = 0) alter;
        TK_Status   add_bounds (ID_Key key, float const * bounds) alter;
        TK_Status   index_to_key (int index, ID_Key alter & key) const;
        TK_Status   key_to_index (ID_Key key, int alter & index) const;
        TK_Status   key_variant_offset (ID_Key key, int variant,
                                        int alter & offset, int alter & length, int alter & index) const;
        TK_Status   key_bounds (ID_Key key, float alter * bounds) const;
        int         used (void) const { return m_used; }

        void        clean (void) alter;

        // older forms:
        TK_Status   key_variant_offset (ID_Key key, int variant, int alter & offset) const {
                        int                 length, index;
                        return key_variant_offset (key, variant, offset, length, index);
                    }
        TK_Status   key_variant_offset (ID_Key key, int variant, int alter & offset, int alter & length) const {
                        int                 index;
                        return key_variant_offset (key, variant, offset, length, index);
                    }

};


class BBINFILETK_API2 Internal_Key_Record {
    private:
        // hash of key to list of recorded segments
        struct Hash_Block {
            Hash_Block *    m_next;
            int             m_used;
            ID_Key          m_keys[32];
        } *                     m_blocks[1024];


    public:
        Internal_Key_Record () { memset (m_blocks, 0, 1024*sizeof(void *)); }
        ~Internal_Key_Record ();

        TK_Status   add_key (ID_Key key) alter;
        TK_Status   find_key (ID_Key key) const;

        void        clean (void) alter;
};



// control memory on these objects, not sure who might be creating/destroying them

class Internal_Segment_List : public BControlledMemoryObject {
    public:
        Internal_Segment_List *     m_next;
        ID_Key                      m_key;

    public:
        Internal_Segment_List (ID_Key k) : m_next (0), m_key (k) {}

        ID_Key  key (void) const { return m_key; }
};

class Internal_Revisit_Item : public BControlledMemoryObject {
    public:
        Internal_Revisit_Item * m_next;
        ID_Key                  m_key;
        ID_Key                  m_owner;
        int                     m_lod;
        float                   m_priority;
        char                    m_opcode;
};


class Internal_ExRef_List : public BControlledMemoryObject {
    public:
        Internal_ExRef_List *   m_next;
        char *                  m_ref;
        ID_Key                  m_context;

    public:
        Internal_ExRef_List (char const * ref, ID_Key context);
        ~Internal_ExRef_List ();

        char const *    Reference (void) const { return m_ref; }
        ID_Key          Context (void) const { return m_context; }
};


class BBINFILETK_API Recorded_Instance : public BControlledMemoryObject {
    public:
        Recorded_Instance *     m_next;

        ID_Key                  m_key;
        int                     m_variant;
        int                     m_values[3];

        float                   m_local_basis[16];      // matrix from our basis to identity
        int                     m_basis_indices[4];
        float                   m_arbitrary_point[3];
        int                     m_arbitrary_index;
        bool                    m_basis_valid;

        unsigned char           m_opcode;

#ifdef _DEBUG
        int                     m_times_used;
#endif

        Recorded_Instance (ID_Key key, int variant, unsigned char op, int val1, int val2, int val3)
            : m_next (0), m_key (key), m_variant (variant), m_basis_valid (false), m_opcode (op) {
            m_values[0] = val1;     m_values[1] = val2;     m_values[2] = val3;
#ifdef _DEBUG
            m_times_used = 0;
#endif
        }

        bool basis_valid (void) const { return m_basis_valid; }
        bool generate_basis (int count, float const * points) alter;
};

class Internal_Segment_Processor;

/*
    Callback which can be used for reporting progress during writing/reading.
    During writing:
        so_far is number of "objects" written, expected is the number the toolkit will likely write.
        Note: the toolkit won't know about objects in pre/post-walk handlers or extra objects due to
            overloading the default classes, so "so_far" may exceed "expected"
    During reading:
        so_far is the number of bytes processed, expected is the file size.
    user_data allows the application to pass any necessary values through.
    returns a flag which indicates whether the processing should continue.
*/
typedef bool (*TK_Progress_Callback) (unsigned long so_far, unsigned long expected, void * user_data);

#endif /* DOXYGEN_SHOULD_SKIP_THIS */



//! The BStreamFileToolkit class provides support for importing/exporting HOOPS Stream File information.
/*! 
    BStreamFileToolkit is the primary support class which manages streaming of HOOPS Stream File data into the HOOPS database.
    The class also provides support for the user to manage export of information from the HOOPS database to a HOOPS Stream 
    File.  It is HOOPS/3dGS-independent; therefore the data does not have to reside in a HOOPS/3dGS scene-graph prior to export
    and it will not be mapped to a HOOPS/3dGS scene-graph during import. (See the opcode handler notes below)

    The HSF file reading process involves reading the opcode at the start of each piece of binary information,
    and calling the Read method of the associated opcode handler. After the opcode handler reports 
    that reading is complete, the Execute method should be called. 
    The ParseBuffer method of the BStreamFileToolkit object encapsulates this process.

    The HSF file writing process involves determining the proper opcode for each piece of binary 
    information, and calling the Interpret method of the associated opcode handler.  After interpretation is complete,  
    the Write method of the opcode handler should be called until writing is complete.  

    BStreamFileToolkit uses a concept of opcode handlers to manage reading and writing of logical pieces of binary information 
    stored in the file.  A set of default opcode handlers support standard HOOPS file objects such as segments, attributes and 
    geometry, and the toolkit can be extended by deriving from the existing opcode handlers or adding new ones in order to handle
    user-specific data.  All of the default opcode handlers registered with this class are of the naming convention TK_XXX, where
    XXX is the type of HSF object.  The TK_XXX opcode handlers only provide implementations of the Read and Write methods; therefore
    the following two methods need to be implemented:
     - Interpret :  this method should query the graphics data (meaning, extract it from data structures); the data will then be written out to an HSF via the Write method
     - Execute :    this mthod should map the HSF data to custom data structures after it has been read in from the HSF file via the Read method

    Streaming refers to a method of import where the user can incrementally supply pieces of data to the file toolkit object. 
    This is useful in order to incrementally update the scene while data is still being read in, rather than wait until 
    the entire file has been read before any visual result is available. 

    To begin the streaming process, read a buffer of HOOPS Stream File data from a local file, a remote location, or memory.  
    This reading process could happen asynchronously on a thread that is separate from the main application thread.  The toolkit
    class provides some wrapper functions to open/close and read/write a buffer of data to and from the HOOPS Stream File.

*/
class BBINFILETK_API2 BStreamFileToolkit : public BControlledMemoryObject {
    friend class BBaseOpcodeHandler;
    friend class TK_Default;
    friend class TK_Comment;
    friend class TK_Header;
    friend class TK_Compression;
    friend class TK_Dictionary;
    friend class Internal_Segment_Processor;
    friend class TK_Shell;

    friend class TK_Tag;
    friend class TK_Instance;
    protected:

#ifndef DOXYGEN_SHOULD_SKIP_THIS

        Internal_Data_Accumulator       m_accumulator;          /*!< A smart data buffer. */
        Internal_Translator             m_translator;           /*!< The KeyToIndex lookup. */

        BBaseOpcodeHandler *            m_objects[256];         /*!< Array of all opcode handlers. Place in array determines which opcodes a handler will handle */
        BBaseOpcodeHandler *            m_default_object;       /*!< Handler to read one opcode and dispatch. */
        int                             m_prewalk_count;
        int                             m_postwalk_count;
        BBaseOpcodeHandler **           m_prewalk;              /*!< Optionally assigned handler for before the HOOPS tree is walked.  null (and unused) by default. */
        BBaseOpcodeHandler **           m_postwalk;             /*!< Optionally assigned handler for after the HOOPS tree is walked.  null (and unused) by default.*/
        BBaseOpcodeHandler *            m_current_object;       /*!< The currently active opcode. */
        Internal_Segment_List *         m_active_segments;      /*!< The ancestry of the currently open segment. (determines HOOPS context) */
        Internal_Key_Record             m_visited_items;		/*!< All segments seen so far. (prevents multiple transmission of includes) */
        ID_Key                          m_context_key;          /*!< Key of the currently open segment (parent of opened geometry) */
        ID_Key *                        m_last_keys;            /*!< Key of most recently opened objects (could be segment, include or geometry)*/
        int                             m_last_keys_used;
        int                             m_last_keys_allocated;
        Internal_Revisit_Item *         m_revisit;              /*!< Things to be processed in the next pass. */
        Internal_Revisit_Item *         m_revisit_working;      /*!< Things getting processed in this pass. */
        int                             m_stage;                /*!< Tracks the progress of the toolkit. */
        int                             m_substage;             /*!< Tracks progress within a stage */
        int                             m_pass;                 /*!< How many times we've traversed a revisit list. */
        int                             m_tag_count;            /*!< Number of tag requests so far (and implicitly the index of the next key) */
        int                             m_position;             /*!< Bookmarks a particular file location (e.g. for tagging later) */
        unsigned int                    m_offset;               /*!< User-specifiable starting file position (e.g. hsf postpended onto another format) */
        int                             m_unused;               /*!< Unprocessed portion of the user's buffer. */
        int                             m_write_flags;          /*!< File writing options. */
        int                             m_read_flags;           /*!< File reading options. */
        int                             m_num_normal_bits;      /*!< Number of bits to use per normal -- sum of xyz combined (if compression is turned on). */
        int                             m_num_vertex_bits;      /*!< Number of bits to use per vertex location -- sum of xyz combined (if compression is turned on). */
        int                             m_num_parameter_bits;   /*!< Number of bits to use per vertex parameter -- sum of uvw combined(a.k.a. texture coordinate) */
        int                             m_num_color_bits;       /*!< Number of bits to use per vertex colors -- sum of rgb combined. */
        int                             m_num_index_bits;       /*!< Number of bits to use per vertex colors-by-index */
        int                             m_file_version;         /*!< File version read. */
        int                             m_target_version;       /*!< File version format to be written. */
        bool                            m_header_comment_seen;  /*!< Have we processed the header yet? (first comment, a.k.a. the header, assumed to contain file version, other comments ignored) */
        char *                          m_log_file;             /*!< Name of file for logging object transactions */
        FILE *                          m_log_fp;               /*!< Internal file pointer to access log file */
        bool                            m_logging;              /*!< Indicates whether we are logging */
        unsigned int                    m_logging_options;      /*!< Controls additional logging information */
        unsigned int                    m_log_line_length;      /*!< Currentl line length in log output */
        unsigned int                    m_opcode_sequence;      /*!< Sequence number for logging & debugging */
        unsigned int                    m_objects_written;      /*!< Number of objects written to file */
        TK_Progress_Callback            m_progress_callback;    /*!< Callback used to report progress */
        void *                          m_progress_value;       /*!< Passthrough value used with progress callback */
        int                             m_buffer_limit;         /*!< Limit on number of bytes to process at a time */
        int                             m_nesting_level;        /*!< Current segment level when parsing */
        int                             m_dictionary_format;    /*!< Desired dictionary type */
        int                             m_dictionary_options;   /*!< Desired dictionary options */
        int                             m_dictionary_size;      /*!< Recorded dictionary size */
        int                             m_dictionary_offset;    /*!< Recorded dictionary offset */
        Recorded_Instance *             m_instance_hash[256];   /*!< Hash table for finding potential instances */
        int                             m_jpeg_quality;         /*!< Quality setting for JPEG compression of images */
        int *                           m_pause_table;          /*!< Table of Pause offsets */
        int                             m_pause_table_size;     /*!< Size of pause table */
        unsigned short                  m_num_pauses;           /*!< Number of pauses recorded */
        float *                         m_world_bounding;       /*!< Bounding cuboid around all data of the entire file */
        Internal_ExRef_List *           m_external_references;  /*!< List of external references */
        Internal_ExRef_List *           m_external_ref_tail;    /*!< Tail of external reference list */

        char **                         m_file_names;           /*!< array of associated filenames processed by the toolkit in this run */
        int *                           m_file_indices;         /*!< starting translator indices associated with the filenames */
        int                             m_file_count;           /*!< number of filenames/indices set in the arrays */
        int                             m_files_allocated;      /*!< size of the arrays */
        char *                          m_current_filename;     /*!< current (last) filenames entry */
        int                             m_index_base;           /*!< current (last) index entry */
        float                           m_quantization_error;   /*!< an upper bound on the largest quantization error seen so far (as reported by opcode handlers -- see ReportQuantizationError) */

        int                             m_save_write_flags;     /*!< Saved copy of file writing options (for code that need to override them temporarily). */
        char *                          m_filename;             /*!< Name of file to process */
        unsigned short *                m_wfilename;            /*!< Unicode name of file to process */
		bool							m_geometry_open;		/*!< flag to indicate if geometry is open for local attibutes */

		bool							m_is_ascii;				/*!< flag to indicate that toolkit is in ASCII mode */
		int								m_num_tabs;		/*!< number of tabs to be set in  a opcode */

		/* Internal use.
         * A quick utility to verify that an array is in fact sorted.  
         * FOR DEBUGGING ONLY
         */
        bool issorted_revisit(Internal_Revisit_Item **array, int count);

        /*! Internal use.
         * We use QuickSort, but try to be intelligent about our choice of pivots.
         * We use the median of the start-, mid-, and end-point as our pivot.
         */
        void qsort_revisit(Internal_Revisit_Item **, Internal_Revisit_Item **);
        TK_Status sort_revisit();

        virtual void read_completed () alter;

#endif /* DOXYGEN_SHOULD_SKIP_THIS */


    protected:

        /*!  Pointer to the file associated with the BStreamFileToolkit object  */
        void *                          m_file;
        DWFInputStream*                 _pInputStream;

    public:

        /*! 
           Default constructor for an BStreamFileToolkit object 
        */
        BStreamFileToolkit (void);
        virtual ~BStreamFileToolkit ();

        /*!
			Sets toolkit into ASCII Mode
		*/
		static bool SupportsAsciiMode(void) {
#ifndef BSTREAM_DISABLE_ASCII
			return true;
#else
			return false;
#endif
		}

        /*!
			Sets toolkit into ASCII Mode
		*/
		TK_Status	SetAsciiMode(bool whether);

		/*!
			Gets current ASCII Mode of toolkit
		*/
		bool		GetAsciiMode();

		void           SetTabs(int);
		int            GetTabs();  

        /*!
			ParseVersion attempts to read a given block as a header to extract the file version
			\param block A pointer to a character buffer.
			\return the version number if the block starts with ";; HSF V", otherwise 0 
		 */
        int   ParseVersion (char const * block) const;

        /*!
            ParseBuffer reads the opcode at the start of the binary data buffer and continually calls the Read method of the associated 
            opcode handler.  The Read methods of the default object handlers read binary information from the buffer that was passed in
            and convert it into HOOPS/3dGS formatted segment, geometry and attribute information. After the opcode handler reports that 
            reading is complete, ParseBuffer calls the Execute method of the opcode handler.  

            ----------
            If the HOOPS/3dGS-specific opcode handlers are being used (the HoopsStream classes, of the form HTK_XXX) then the Execute methods will call the appropriate HOOPS/3dGS
            functions to either create segemnts, insert geometry or set attributes underneath the currently open HOOPS/3dGS segment. When using HoopsStream, 
            a HOOPS/3dGS scene-graph segment must currently be open before calling ParseBuffer. 

            If the HOOPS/3dGS-independent default object handlers are used (the BaseStream classes, of the form TK_XXX), then the Execute method does nothing 
            and the developer would need to overload it to map the HSF data to custom data structures.
            ----------
            
            If the buffer being parsed ends in the middle of an object, it will be saved along with any HOOPS database state information 
            and the next buffer will be appended to it.

            The mode parameter allows for control of how the buffer is parsed.  The default of TK_Normal indicates read until the
            Termination code.  

            TK_Pause indicates read to the next Pause or Termination.  If the HOOPS Stream File has been written out with default
            streaming properties, a pause will occur at the end of the HOOPS segment tree.  This tree contains simple bounding
            box representations for the HOOPS geometric primitives that are used to represent facetted surfaces.

            TK_Single indicates read only the first object in the buffer.  If the buffer ends in the middle of an object, it will
            be saved and the next buffer will be appended to it.  Any extra data in the buffer than is associated with more than one 
            object is thrown away.  This is referred to as 'a la carte mode', and is useful when the application wants specific 
            entities to be read in from the file, typically from random locations.  This is necessary for image sweetening.

            \param b A pointer to a character buffer.
            \param s An integer denoting the length of the buffer.
            \param mode A flag controlling how the buffer is to be parsed.
            \return The result of the function call.
        */
        TK_Status   ParseBuffer (char const * b, int s, TK_Status mode = TK_Normal) alter;
 

        /*!
            Tells the toolkit where to begin writing data.
            \param b A character pointer denoting the buffer to write into.
            \param s Size of the buffer
        */
        TK_Status   PrepareBuffer(char * b, int s) alter;

        /*! Returns how much of the internal data buffer is currently filled */
        int         CurrentBufferLength() {return m_accumulator.get_original_buffer_size() -
                                                  m_accumulator.unused();}

        /*! 
            While parsing a buffer it may be necessary to keep a context.  This method will be called by ParseBuffer
            to activate a context, and should be overloaded to provide custom context-activation.
            \param key A context identifier 
        */
        virtual void    ActivateContext (ID_Key key) { (void)key; }

        /*! 
            While parsing a buffer it may be necessary to keep a context.  This method will be called by ParseBuffer
            to deactivate a context, and should be overloaded to provide custom context-deactivation.
            \param key A context identifier 
        */
        virtual void    DeactivateContext (ID_Key key) { (void)key; }

        /*! 
            While processing External_References a previously visited context will be revisited.  This method
            will be called by the default Read_Stream_File to indicate that context should be prepared to receive
            content, and should be overloaded to provide custom preparation.  This might include operations such
            as clearing out any placeholder geometry.
            \param key A context identifier 
        */
        virtual void    NewFileContext (ID_Key key) { (void)key; }

        /*! 
           Returns the number of bytes that have been written since the beginning of the writing process.  This is currently 
           used internally during writing to build up a dictionary of file offets for each logical piece of binary data in the 
           file, so that random lookups may be performed during file loading.
        */
        int             GeneratedSoFar (void) const { return m_accumulator.generated(); }

        /*! 
           Returns the number of objects that have been written since the beginning of the writing process.  This is
           available to applications for estimation of writing progress if there is a means of estimating the total.
        */
        unsigned int    ObjectsSoFar (void) const { return m_objects_written; }

        /*!
            Registers an opcode handler for the specified type, allowing for variations on normal processing of HOOPS Stream Metafile
            objects, or handling of user-defined binary objects.
            \param which The type of binary object to associated the opcode handler with.
            \param handler A pointer to an BBaseOpcodeHandler object.
        */
        void        SetOpcodeHandler (int which, BBaseOpcodeHandler * handler) alter;

        /*!
            Registers an opcode handler which will be called by the toolkit before it processes (walks and imports/exports) the HOOPS segment tree.
            This allows the user to import/export user-specified data from/to the HOOPS Stream File, at a location
            prior to the segment tree information.
            objects, or handling of user-defined binary objects.
            \param handler A pointer to an BBaseOpcodeHandler object.
        */
        void        SetPrewalkHandler (BBaseOpcodeHandler * handler) alter;


        /*!
            Registers an opcode handler which will be called by the toolkit after it processes (walks and imports/exports) the HOOPS segment tree.
            This allows the user to import/export user-specified data from/to the HOOPS Stream File, at a location
            following the segment tree information.
            objects, or handling of user-defined binary objects.
            \param handler A pointer to an BBaseOpcodeHandler object.
        */
        void        SetPostwalkHandler (BBaseOpcodeHandler * handler) alter;

        /*! \return A pointer to the requested opcode handler */
        BBaseOpcodeHandler *    GetOpcodeHandler (int which) const { return m_objects[which]; }

        /*! 
            Initializes the file toolkit.  This is useful if a single toolkit object is to be used for dealing with different files, 
            rather than deleting the current toolkit object and creating a new instance.  Call this function after processing of the 
            current file is complete.
        */
        virtual void    Restart (void) alter;

        // access to index <-> key tranlation
        /*! 
            Calculates the HOOPS key associated with the specified index.
            \param index An integer denoting a file index.
            \param key The HOOPS key associated with the specified index. Returned to user. Passed by reference.
            \return The result of the function call.
        */
        TK_Status   IndexToKey (int index, ID_Key alter & key) const;

        /*! 
            Calculates the index associated with the specified HOOPS key.
            \param key A HOOPS key. 
            \param index An integer denoting the file index associated with the HOOPS key. Returned to user. Passed by reference.
            \return The result of the function call.
        */        
        TK_Status   KeyToIndex (ID_Key key, int alter & index) const;

        /*! 
            Associates a HOOPS key with a file index, and adds the an entry to the toolit's index/key association table.
            \param index An integer denoting a file index.
            \param key The HOOPS key that to associate with the specified index.
            \return The result of the function call.
        */
        TK_Status   AddIndexKeyPair (int index, ID_Key key) alter
                                        { return m_translator.add_pair (index, key); }

        /*! 
            Records the file offset, and optionally the size, of a variant (LOD) associated with a previously recorded key.
            \param key The HOOPS key of the object
            \param variant The variant (LOD number) being recorded
            \param value1 The file offset of the start of the variant
            \param value2 The size of the variant
            \return The result of the function call.
        */
        TK_Status   AddVariant (ID_Key key, int variant, int value1, int value2 = -1) alter
                                        { return m_translator.add_variant (key, variant, value1, value2);   }

        /*! 
            Records the bounding volume associated with a previously recorded object.
            \param key The HOOPS key of the object
            \param bounds The bounding volume as 6 floats (min & max points of a box)
            \return The result of the function call.
        */
        TK_Status   AddBounds (ID_Key key, float const * bounds) alter
                                        { return m_translator.add_bounds (key, bounds);                     }

        /*! 
            Returns the file offset of a variant (LOD) associated with a previously recorded key.
            \param key The HOOPS key of the object
            \param variant The variant (LOD number) being recorded
            \param offset The file offset of the start of the variant
            \return The result of the function call.
        */
        TK_Status   GetOffset (ID_Key key, int variant, int alter & offset) const
                                        { return m_translator.key_variant_offset (key, variant, offset);            }

        /*! 
            Returns the file offset and length of a variant (LOD) associated with a previously recorded key.
            \param key The HOOPS key of the object
            \param variant The variant (LOD number) being recorded
            \param offset The file offset of the start of the variant
            \param length The size of the variant (returned as 0 if unknown)
            \return The result of the function call.
        */
        TK_Status   GetOffset (ID_Key key, int variant, int alter & offset, int alter & length) const
                                        { return m_translator.key_variant_offset (key, variant, offset, length);    }

        /*! 
            Returns the file offset and length of a variant (LOD) associated with a previously recorded key.
            \param key The HOOPS key of the object
            \param variant The variant (LOD number) being recorded
            \param offset The file offset of the start of the variant
            \param length The size of the variant (returned as 0 if unknown)
            \param filename pointer to the stored filename associated with the key (may be null)
            \return The result of the function call.
        */
        TK_Status   GetOffset (ID_Key key, int variant,
                               int alter & offset, int alter & length, char const * alter & filename) const;

        /*! 
            Returns the bounding volume associated with a previously recorded key.
            \param key The HOOPS key of the object
            \param bounds The bounding volume as 6 floats (min & max points of a box)
            \return The result of the function call.
        */
        TK_Status   GetBounds (ID_Key key, float alter * bounds) const
                                        { return m_translator.key_bounds (key, bounds);                             }

        /*! 
            Reserves & returns the next available tag index
            \return The tag index.
        */
        int         NextTagIndex (void) alter   { return m_tag_count++; }
        /*! 
            Returns the next available tag index without reserving it for use
            \return The tag index.
        */
        int         PeekTagIndex (void) const   { return m_tag_count; }

        /*!
            Sets the starting filename for the toolkit (normally used internally).

            \param name The name of the file.
        */
        void        SetFilename (char const * name) alter;
        /*!
            Sets the starting filename for the toolkit (normally used internally).

            \param name The name of the file.
        */
        void        SetFilename (unsigned short const * name) alter;

        /*!
            Reads the file associated with the toolkit (internal utility).
            \return The result of the function call.
        */
		TK_Status	Read_Stream_File (void);

        /*!
            Associates a new filename with the toolkit.

            \param name The name of the file.
        */
        void            SetNewFile (char const * name) alter;
        /*!
            Returns the name of the last file, if any, that was associated with the toolkit.
            \return The name of the file associated with the toolkit.
        */
        char const *    GetCurrentFile (void) const    { return m_current_filename; }
        /*!
            [Re]Selects a previously accessed file for further processing

            \param name The name of the file.
            \return TK_Normal if the file was found and selected, TK_NotFound otherwise.
        */
        TK_Status       SelectFile (char const * name) alter;

        /*!
            Opens a file.  This acts as a simple wrapper for the 'fopen' command.  

            \param name The name of the file to open.
            \param write True if the file is open for writing, otherwise, file is open for reading.
            \return The result of function call.
        */
        virtual TK_Status   OpenFile (char const * name, bool write = false) alter;
        /*!
            Opens a file.  This acts as a simple wrapper for the 'fopen' command.  

            \param name The name of the file to open.
            \param write True if the file is open for writing, otherwise, file is open for reading.
            \return The result of function call.
        */
        virtual TK_Status   OpenFile (unsigned short const * name, bool write = false) alter;

        //
        // Lets the toolkit read directly from a stream rather than assuming
        // a disk file will be the source of the data feed.
        //
        virtual TK_Status   OpenStream( DWFInputStream& rReadStream ) alter;
        virtual TK_Status   CloseStream() alter;

        /*!
            Closes a file.  This acts as a simple wrapper for the 'fclose' command.  
            \return The result of function call.
        */
        virtual TK_Status   CloseFile (void) alter;

        /*!
            Reads a buffer of data to the file associated with the BStreamFileToolkit object.  This acts as a simple wrapper 
            for the 'fread' command.  

            \param buffer  Character buffer used to store the data being read.  
            \param size Integer denoting amount of data to read.
            \param amount_read The amount of data actually read into the buffer.
            \return The result of function call.
        */
        virtual TK_Status   ReadBuffer (char alter * buffer, int size, int alter & amount_read) alter;

        /*!
            Writes a buffer of data to the file associated with the BStreamFileToolkit object.  This acts as a simple wrapper 
            for the 'fwrite' command.  

            \param buffer  Character buffer to be written out.  
            \param size Integer denoting amount of data to write.
            \return The result of function call.
        */
        virtual TK_Status   WriteBuffer (char alter * buffer, int size) alter;

        /*!
            Utility function which repositions the current file pointer.  This acts a wrapper for fseek, and is used by 
            LocateDictionary and LocateEntity, but might be reimplemented by classes derived from BStreamFileToolkit
            to perform custom file repositioning.

            \param offset If positive, repositions the file pointer to offset bytes from the beginning of the file, otherwise
            repositions the file pointer to offset bytes from the end of the file. 
            \return The result of function call.
        */
        virtual TK_Status   PositionFile (int offset) alter;

        /*!
            Utility function which returns the size of the currently open file.  This acts a wrapper for fstat, and is used
            by the progress callback (if any), but might be reimplemented by classes derived from BStreamFileToolkit
            to perform custom file repositioning.

            \param size reference to the variable to receive the file size.
            \return The result of function call.
        */
        virtual TK_Status   GetFileSize (unsigned long & size) alter;


        /*!
            Instructs the BStreamFileToolkit object to sets its internal file pointer to the location of the 
            file dictionary.  The dictionary contains entries for the file locations of various representations of 
            entities.

            Objects which are derived from BStreamFileToolkit may reimplement to support locating a dictionary for a file that 
            resides on a remote location such as a web server.  This might involve passing a message that instructs the 
            server process to locate the dictionary.

            \return The result of function call.
        */
        virtual TK_Status   LocateDictionary (void) alter;

        /*!
            Instructs the BStreamFileToolkit object to sets its internal file pointer to the location of a  
            variant of an entity, which could be the full representation or a LOD representation. This allows
            reading a single entity from the file, even though it resides at an offset in the file.

            \param key The key of a HOOPS entity.
            \param variant  The variant of the HOOPS entity to locate.  0 represents the full representation, 1 represents
            LOD level 1, etc....
            \return The result of function call.
        */
        virtual TK_Status   LocateEntity (ID_Key key, int variant) alter;
        
        int         GetFlags (void) const       { return GetWriteFlags(); }     /*!< \b Deprecated; Use GetWriteFlags; Returns the writing flags set on the toolkit.   */
        void        SetFlags (int flags) alter  { SetWriteFlags(flags); }       /*!< \b Deprecated; Use SetWriteFlags; Sets the toolkit writing flags. */
        

        /*! Sets the toolkit writing flags.  These flags control a variety of export properties, and are defined in #TK_File_Write_Options   */
        void        SetWriteFlags (int flags) alter     { m_write_flags = flags; } 
        /*! Returns the writing flags set on the toolkit.  These flags control a variety of export properties, and are defined in #TK_File_Write_Options

            \param mask of flags to be returned (default is all).
            \return The result of function call.
        */                      
        int         GetWriteFlags (int mask = ~0) const { return m_write_flags & mask; }
                
        /*! Sets the toolkit reading flags.  These flags control various import properties, and are defined in #TK_File_Read_Options   */
        void        SetReadFlags (int flags) alter      { m_read_flags = flags; }  
        /*! Returns the reading flags set on the toolkit.  These flags control various import properties, and are defined in #TK_File_Read_Options

            \param mask of flags to be returned (default is all).
            \return The result of function call.
        */
        int         GetReadFlags (int mask = ~0) const  { return m_read_flags & mask; }


        int         GetNumNormalBits (void) const { return m_num_normal_bits; } /*!< Returns the maximum number of bits that will be used per normal */
        /*! 
           Sets the maximum number of bits that will be used per normal (x,y, and z combined).
           The default is 10 (for the entire normal).
           Opcode handlers may, at their option, decide to use less
        */
        void        SetNumNormalBits (int numbits) alter  { m_num_normal_bits = (numbits<=72) ? numbits : 72; } 

        int         GetNumVertexBits (void) const { return m_num_vertex_bits; } /*!< Returns the maximum number of bits that will be used per vertex location (x,y,z combined).*/
        /*! Sets the maximum number of bits to use for vertex locations (x,y, and z combined). Default is 24. */
        void        SetNumVertexBits (int numbits) alter  { m_num_vertex_bits = (numbits<=72) ? numbits : 72; }      
        int         GetNumParameterBits (void) const { return m_num_parameter_bits; } /*!< Returns 3 times the number of bits that will be used per sample in vertex parameters (texture cordinate) */
	/*! Sets the number of bits per sample for vertex parameters (texture coordinates).  For backwards compatibility it is specified
	    as 3 times the number (previous versions of the stream toolkit specified the number of bits for the entire parameter, but always
	    assumed 3 parameters (u,v,w) per vertex).  Thus, for example, if you specify '24' bits, a 1d texture will have 8 bits total, a 2d texture
	    will have 16, and a 3d texture will have 24. */
        void        SetNumParameterBits (int numbits) alter  { m_num_parameter_bits = (numbits<=72) ? numbits : 72; }   
        int         GetNumColorBits (void) const { return m_num_color_bits; } /*!< Returns the maximum number of bits that will be used per vertex color  */
        void        SetNumColorBits (int numbits) alter  { m_num_color_bits = (numbits<=72) ? numbits : 72; }        /*!< Sets the maximum number of bits to use for vertex colors */
        int         GetNumIndexBits (void) const { return m_num_index_bits; } /*!< Returns the maximum number of bits that will be used per vertex color-by-index */
        void        SetNumIndexBits (int numbits) alter  { m_num_index_bits = (numbits<=24) ? numbits : 24; }        /*!< Sets the maximum number of bits to use for vertex colors-by-index */

        /*! Sets the quality level for JPEG compression, range is 0-100, 0 disables, 75 by default */
        void        SetJpegQuality (int quality = 75) alter     { m_jpeg_quality = quality; }
        /*! Returns the current JPEG quality setting */
        int         GetJpegQuality (void) const                 { return m_jpeg_quality;    }

        int         GetVersion (void) const     { return m_file_version; }  /*!< Returns the file format version during reading */

        void        SetTargetVersion (int version) alter     { m_target_version = version; }  /*!< Sets the file format version desired for writing */
        int         GetTargetVersion (void) const     { return m_target_version; }  /*!< Returns the file format version desired for writing */

        //! Gets the file offset, a displacement to be added to positions used in the dictionary (for example, in case the data is appended to another file)
        unsigned int    GetFileOffset (void) const                  { return m_offset;      }
        //! Sets the file offset, a displacement to be added to positions used in the dictionary (for example, in case the data is appended to another file)
        void            SetFileOffset (unsigned int offset) alter   { m_offset = offset;    }
        /*! \return the amount of data in the user's buffer left unused */
        int             Unused (void) const                     { return m_unused; }
        /*! Prints out a debug message informing the user that an error has occurred.
            Also provides a handy choke point for break points during debugging.
            \return TK_Error always */
        virtual TK_Status   Error(char const * msg = 0) const;

        /*! Returns the name of the log file */
        char const *GetLogFile (void) const                     { return m_log_file;    }
        /*! Sets the name of the log file */
        void        SetLogFile (char const * filename = 0) alter;

        /*! Returns the logging state */
        bool        GetLogging (void) const                     { return m_logging;     }
        /*! Sets the logging state.  If true, the toolkit will output files called hsf_export_log.txt and
            hsf_import_log.txt which contain a byte representing each opcode that was exported or imported. 
            during the write and/or read phase, respectively. */
        void        SetLogging (bool setting) alter             { m_logging = setting;  }

        /*! Returns the logging options */
        unsigned int    GetLoggingOptions (unsigned int mask = ~0) const
                                                                { return m_logging_options & mask; }
        /*! Sets the logging options.  controls the types of logging information collected. */
        void        SetLoggingOptions (unsigned int options = ~0) alter
                                                                { m_logging_options = options;  }

        /*! Opens the log file for writing. 
            \param filename the name of the file to be opened as a log
            \param mode passed through to fopen
            \return TK_Error on failure */
        TK_Status   OpenLogFile (char const * filename, char const * mode) alter;
        /*! Writes a message into the log */
        void        LogEntry (char const * string) alter;
        /*! Writes a message into the log with wide characters */
        void        LogEntry (unsigned short const * string) alter;
        /*! Closes the log file */
        void        CloseLogFile (void) alter;

                /*! internal use */
        unsigned int    NextOpcodeSequence (void) alter                 { return ++m_opcode_sequence;   }
                /*! internal use */
        void            SetOpcodeSequence (unsigned int seq=0) alter    { m_opcode_sequence = seq;      }

        /*! Returns true if the header comment has been processed */
        bool        HeaderCommentSeen(void) const                       { return m_header_comment_seen; }

        /*! Returns the progress callback */
        TK_Progress_Callback    GetProgressCallback (void) const        { return m_progress_callback;   }
        /*! Sets the progress callback */
        void    SetProgressCallback (TK_Progress_Callback cb = 0) alter { m_progress_callback = cb;     }

        /*! Returns the progress passthrough value */
        void *      GetProgressValue (void) const                       { return m_progress_value;      }
        /*! Sets  the progress passthrough value */
        void        SetProgressValue (void * value) alter               { m_progress_value = value;     }

        /*! Returns the progress passthrough value */
        int         GetBufferLimit (void) const                         { return m_buffer_limit;      }
        /*! Sets  the progress passthrough value */
        void        SetBufferLimit (int limit) alter    {
                            m_buffer_limit = (0 < limit && limit < TK_DEFAULT_BUFFER_SIZE) ?
                                             limit : TK_DEFAULT_BUFFER_SIZE;
                        }

        /*! Reports to the toolkit the identifier of the last object processed */
        void        SetLastKey (ID_Key key) alter;
                /*! Appends the identifier of the last object processed to the current identifier list */
        TK_Status   AppendLastKey (ID_Key key) alter;
                /*! Clears identifier of the last object processed */
        void        ClearLastKey () alter;
                /*! Returns identifier of the last object processed */
        TK_Status   GetLastKey (ID_Key &key) const;

        /*!
            Sets the desired dictionary format level and options
            \param format The desried dictionary format
            \param options Bitmask of any options that many modify a specific format
        */
        void        SetDictionaryFormat (int format = 3, int options = TK_Dictionary_Bounding_Volumes) alter
                                        { m_dictionary_format = format; m_dictionary_options = options; }
        /*!
            Returns the currently requested dictionary format level
            \return Dictionary format
        */
        int         GetDictionaryFormat (void) const    { return m_dictionary_format;     }
        /*!
            Returns the options for the dictionary format.
            \return Dictionary options bitmask
        */
        int         GetDictionaryOptions (void) const   { return m_dictionary_options;    }
        /*!
            Sets (records) the dictionary offset
            \param offset The dictionary offset
        */
        void        SetDictionaryOffset (int offset) alter  { m_dictionary_offset = offset; }
        /*!
            Returns the recorded dictionary offset
            \return Dictionary offset
        */
        int         GetDictionaryOffset (void) const	    { return m_dictionary_offset;   }
        /*!
            Sets (records) the dictionary size
            \param size The dictionary size
        */
        void        SetDictionarySize (int size) alter	    { m_dictionary_size = size;	    }
        /*!
            Returns the recorded dictionary size
            \return Dictionary size
        */
        int         GetDictionarySize (void) const	    { return m_dictionary_size;     }

        /*!
            Record a pause at the given file offset
            \param offset File offset of this pause
        */
        void        RecordPause (int offset) alter;
        /*!
            Clears the pause table
        */
        void        ClearPauses (void) alter            { m_num_pauses = 0;     }
        /*!
            Returns the number of pauses recorded
            \return The number of pauses
        */
        int         GetPauseCount (void) const          { return m_num_pauses;  }
        /*!
            Returns the pause offsets
            \return address of an array of file offsets corresponding the the pauses
        */
        int const * GetPauseTable (void) const          { return m_pause_table; }

        /*!  supported for older code */
        void        SetFirstPause (int offset) alter    { if (GetPauseCount() == 0) RecordPause (offset);
                                                          else m_pause_table[0] = offset;                   }
                /*!  supported for older code */
        int         GetFirstPause (void) const          { return (m_num_pauses > 0) ? m_pause_table[0] : 0; }

        /*!
            Returns the current file position
        */
        int         GetPosition (void) const            { return m_position;        }

                /*! Sets the global bounding box to be used for bounding box aggegation.  This is utilized when 
                        the TK_Global_Quantization bit of TK_File_Write_Options is set 
                        \param bbox pointer to the bounding box coordinates 
                */
        void        SetWorldBounding( float const *bbox );

                /*! Sets the global bounding box (by sphere) be used for bounding box aggegation.  This is utilized when 
                        the TK_Global_Quantization bit of TK_File_Write_Options is set 
                        \param pt center point of sphere
                        \param radius radius of sphere
                */
        void        SetWorldBoundingBySphere( float const *pt, float radius );

                /*! \returns the world bounding box */
        float const * GetWorldBounding() const          { return m_world_bounding; }


        /*! Saves an external reference (string) and associated context (current open segment) to be processed.
            \param ref external reference string
            \param context external reference context
        */
        void        AddExternalReference (char const * ref, ID_Key context) alter;
        /*! Removes the current external reference and readies the next one (if any).
            \return true if another external reference is present
        */
        bool        NextExternalReference (void) alter;
        /*! Returns the string representing the external reference at the beginning of the list, null if none.
            \return external reference string
        */
        char const *GetExternalReference (void) const {
                            return  (m_external_references != 0) ? m_external_references->m_ref : 0;
                        }
        /*! Returns the context associated with the external reference at the beginning of the list, -1 if none.
            \return external reference context
        */
        ID_Key      GetExternalReferenceContext (void) const {
                            return  (m_external_references != 0) ? m_external_references->m_context : 0;
                        }

        /*! Adds the specified key to the from of a list to keep track of "open" segments.
            \param key of segment being openned
        */
        void        AddSegment (ID_Key key) alter;
        /*! Removes the most currently "openned" segment
            \return the key of the segment which was removed
        */
        ID_Key      RemoveSegment (void) alter;
        /*! Returns the key of the currently "open" segment
            \return key of the current segment
        */
        ID_Key      CurrentSegment (void) alter { return (m_active_segments != 0) ? m_active_segments->key() : -1; }
        /*! resets the quantization error back to zero */
        void ResetQuantizationError() { m_quantization_error = 0; }
        /*! Intended primarily for use by opcode handlers, this function sets m_quantization_error to 
            error if it is the largest error seen so far.
        */
        void ReportQuantizationError( float error ) { if (error > m_quantization_error) m_quantization_error = error; };
        /*! Intended primarily for use by opcode handlers, this function calculates the longest dimension cell
            size and uses that as an upper bound on quantization error for a call to ReportQuantizationError
        */
        void ReportQuantizationError( int bits_per_sample, float const *bounding, int num_dimensions = 3 );
        /*! \return the largest quantization error reported by any of the opcodes during writing.
            Error reported is object space, with no attempt to account for modelling matrices.
        */
        float GetQuantizationError() const { return m_quantization_error; }

        /*! Prepares the toolkit for dealing with geometry-level attributes
		    \return status of the request, error if a geometry is already open
        */
		TK_Status	OpenGeometry (void) alter	{
						if (m_geometry_open)
							return Error ("recursive geometry open");
						m_geometry_open = true;
						return TK_Normal;
					}
        /*! Completes dealing with geometry-level attributes
		    \return status of the request, error if no geometry is open
        */
		TK_Status	CloseGeometry (void) alter	{
						if (!m_geometry_open)
							return Error ("no geometry open");
						m_geometry_open = false;
						return TK_Normal;
					}

		/*! Queries the state of geometry-level attribute handling
			\return whether geometry-level handling is active
		*/
		bool		GeometryIsOpen (void) const		{ return m_geometry_open;	}


    protected:

#ifndef DOXYGEN_SHOULD_SKIP_THIS

        // normal data access for objects
        TK_Status   read (char alter * b, int n) alter      { return m_accumulator.read (b, n); }
        TK_Status   write (char const * b, int n) alter     { return m_accumulator.write (b, n); }
        TK_Status   lookat (char alter & b) alter           { return m_accumulator.lookat (b); }

        // used by segment handlers to make sure we know which segment should be "open"
        void        add_segment (ID_Key key) alter { AddSegment (key); }
        ID_Key      remove_segment (void) alter { return RemoveSegment(); }

        // used by renumber key, maybe by lookup functions (to be implemented...)
        //! obsolete
        void        set_last_key (ID_Key key) alter { SetLastKey( key ); } 
        ID_Key      context_key (void) const { return m_context_key; }
        void        set_context_key (ID_Key key) alter;
        //! obsolete
        ID_Key      last_key (void) const { 
            if( m_last_keys_used == 1 ) 
                return m_last_keys[0];
            else 
                return -1; 
        } 

        // keep track of visited segments (for things like include)
        void        remember_item (ID_Key key) alter;
        bool        find_item (ID_Key key) const;

        /*! Returns a pointer to the BBaseOpcodeHandler object located at the specified index */
        BBaseOpcodeHandler alter *  opcode_handler (int index) const { return m_objects[index]; }

        void        adjust_written (int count) alter    { m_objects_written += count; }

        int         pass (void) const { return m_pass; }

        //! Internal use.  Record the current object for later reprocessing
        TK_Status   revisit (unsigned char opcode, float priority, int lod=0) alter;

        //! Interal use. Record the current object for possible instancing
        void        record_instance (ID_Key key, int variant, BBaseOpcodeHandler const * object,
                                     int val1, int val2 = 0, int val3 = 0) alter;

        //! Internal use.  Try to locate an instance matching the specified object
        bool        find_instance (BBaseOpcodeHandler * object, int val1, int val2 = 0, int val3 = 0) const;

        //! Internal use. Position is the offset to the start of the current object
        int         position (void) const           { return m_position; }
        void        set_position (int pos) alter    { m_position = pos; }
        void        mark_position (void) alter      { set_position (GeneratedSoFar()); }

        /*! Internal use.  Deal with a request to tag an object
             generate a new index if we can't find the key in the table
             if variant != -1, the variant & offset() are associated with the item
        */
        virtual TK_Status   tag (int variant) alter;

        void        increase_nesting (int amount = 1) alter { m_nesting_level += amount; }
        void        decrease_nesting (int amount = 1) alter { m_nesting_level -= amount; }

        // utility
        TK_Status   start_compression (void) alter      { return m_accumulator.start_compression(); }
        TK_Status   stop_compression (void) alter       { return m_accumulator.stop_compression(true); }
        TK_Status   start_decompression (void) alter                { return m_accumulator.start_decompression(); }
        TK_Status   stop_decompression (bool force = false) alter   { return m_accumulator.stop_decompression(force); }
        virtual void    empty_lists (void) alter;

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

};

//TODO Documentation 

#ifndef BSTREAM_DISABLE_ASCII
class PutTab
{
public:
	PutTab(BStreamFileToolkit* tk) : m_tk(tk)
	{
		int n_tabs = m_tk->GetTabs();
		m_tk->SetTabs(++n_tabs);
	}

	~PutTab()
	{
		int n_tabs = m_tk->GetTabs();
		m_tk->SetTabs(--n_tabs);
	}

private:
	BStreamFileToolkit* m_tk;
};
class Outdent
{
public:
	Outdent(BStreamFileToolkit* tk, int n_tabs = 1) : m_tk(tk)
	{
		int cur_tabs = m_tk->GetTabs();
		if( cur_tabs >= n_tabs )
		{
			m_tabs = n_tabs;
			m_tk->SetTabs(cur_tabs-n_tabs);
		}
		else
		{
			m_tabs = cur_tabs;
			m_tk->SetTabs(0);
		}
	}

	~Outdent()
	{
		int cur_tabs = m_tk->GetTabs();
		m_tk->SetTabs(cur_tabs+m_tabs);
	}

private:
	BStreamFileToolkit* m_tk;
	int					m_tabs;
};

#endif //BSTREAM_DISABLE_ASCII

#endif //BBINFILETK_TOOLKIT
