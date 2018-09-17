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
// $Header: //DWF/Working_Area/Willie.Zhu/w3dtk/BPolyPoly.h#1 $
//

#ifndef BPOLYPOLY
#define BPOLYPOLY

///
///\file        dwf/w3dtk/BPolyPoly.h
///

#ifndef BBINFILETK_TOOLKIT
    #include "dwf/w3dtk/BStreamFileToolkit.h"
#endif

//! Handles the TKE_PolyPolyline opcode.
/*! 
    TK_Polypoint provides support for writing/reading the TKE_PolyPolyline opcode objects to/from an 
    HSF file. 
    
    TKE_PolyPolyline is an alternative to TKE_Polyline and TKE_Line.  It is a more compressed way of handling 
    large numbers of polylines and lines.  The idea is that by grouping primitives together, better 
    compression algorithms can be run for  them.  This class utilizes some of the same compression schemes 
    that are implemented in TK_Shell.

    This class is especially optimal for dealing with 2d data.  With all of the 12 combinations ("zero", "constant", 
    "per-primitive" or "unrestricted" along any of the 3 standard axes X, Y and/or Z), we leverage the restrictions that
    we can make to compress as deeply as possible.  There is no provision for point data that is "mostly" 2d, however, 
    so such data will be encoded as 3d.  For this reason, performance will improve if all 2d lines and polylines can be
    put in a segment or segments that are separate from any 3d line data.
    
    The HOOPS/3dGS scene-graph supports 'polyline' and 'polygon' primitives which are both defined by a point count 
    and points array.

*/
class BBINFILETK_API TK_PolyPolypoint : public BBaseOpcodeHandler {
    private:
        TK_PolyPolypoint(const TK_PolyPolypoint&);
        TK_PolyPolypoint& operator=(const TK_PolyPolypoint&);


    protected:
        int             m_point_count;          //!<The number of points
        int             m_points_allocated;     //!<The number of points for which we have allocated space (to save time, we only reallocate the points array if it needs to be expanded, not on every call to Reset()).
        float *         m_points;               //!<The points array.
        int             m_points_num_floats;    //!<The number of independent floating point values required to specify the points array (equals m_point_count*3 for 3d data, but less for 2d)
        int             m_points_num_dimensions;//!<The number of unrestricted dimensions in the points array that must be completely specified.
        int             m_primitive_count;      //!<The number of primitives.  Omitted from read/write (it can be deduced from the lengths array) unless TKPP_HAS_EXPLICIT_PRIMITIVE_COUNT bit is set
        int             m_lengths_allocated;    //!<The number of array elements that we have allocated for primitive lengths.
        int *           m_lengths;              //!<The length of each primitive
        float           m_bbox[6];              //!<A bounding box around the unrestricted dimensions.  If the data is 2d, only the first 4 values are used, and it becomes a bounding rectangle.
        int             m_templen;              //!<the length of all stragglers left behind (e.g. an array of floats of length m_primitive_count if TKPP_PER_PRIMITIVE_X subop bit is set)
        float           m_temprange[2];         //!<1d bounds of the stragglers
        unsigned short  m_subop;                //!<Suboptions.  Among other things, it specifies all 2d restrictions that can be made on the points.  Be careful when testing for the 2d bits, because some have multiple bits set.  For this reason, you should use the form if((m_subop & mask) == value) instead of if(m_subop & bit)
        unsigned char   m_compression_scheme;   //!<If the TKPP_COMPRESSED bit is set in m_subop, this variable tracks the specific compression algorithm used.
        unsigned char * m_workspace;            //!<Raw data after packing
        int             m_workspace_used;       //!<The amount of workspace area used
        int             m_workspace_allocated;  //!<The amount of workspace area allocated
        int             m_bits_per_sample;      //!<the number of bits per sample.  Should be tk.GetNumVertexBits()/3
        int             m_substage;             //!<tracks progress during helper functions
        int             m_progress;             //!<tracks progress during special situations

        /*!Efficiently searches the points array for any restrictions that can be made on the points in order to aid compression
           \param tk the toolkit \return a status value to indicate success or failure */
        TK_Status analyze_dimensionality( BStreamFileToolkit & tk ) alter;
        /*!Calculates the number of independent floating point values needed to encode the points array
           \param tk the toolkit \return a status value to indicate success or failure  */
        TK_Status figure_num_floats (BStreamFileToolkit & tk) alter;
        /*!Computes the primitive count if such has not been done already
           \param tk the toolkit \return a status value to indicate success or failure  */
        TK_Status validate_primitive_count (BStreamFileToolkit & tk) alter;
        /*!A small helper to reallocate the lengths array in the event that we run out of space.
           \param tk the toolkit \return a status value to indicate success or failure  */
        TK_Status expand_lengths (BStreamFileToolkit & tk) alter;
        /*!Reorganizes the values in m_points so that unrestricted dimensions appear first, followed by whatever floats are needed to encode the restricted dimensions.  Intended for use during the write phase.,
           \param tk the toolkit \return a status value to indicate success or failure  */
        TK_Status mangle (BStreamFileToolkit & tk) alter;
        /*!Undoes during the read phase everything that mangle() does at write time.
           \param tk the toolkit \return a status value to indicate success or failure  */
        TK_Status unmangle (BStreamFileToolkit & tk) alter;
        /*!compresses the m_points into m_workspace, an array of unsigned char's according to the CS_TRIVIAL compression scheme
           \param tk the toolkit \return a status value to indicate success or failure  */
        TK_Status compute_trivial_points (BStreamFileToolkit & tk) alter;
        /*!uncompresses m_workspace back into m_points according to CS_TRIVIAL
           \param tk the toolkit \return a status value to indicate success or failure  */
        TK_Status process_trivial_points (BStreamFileToolkit & tk) alter;
        /*!compresses the m_points into m_workspace, an array of unsigned char's according to the CS_LINE_EXTEND compression scheme.  CS_LINE_EXTEND is currently unused because of poor performance.
           \param tk the toolkit \return a status value to indicate success or failure  */
        TK_Status compute_line_extend_points (BStreamFileToolkit & tk) alter;
        /*!uncompresses m_workspace back into m_points according to CS_LINE_EXTEND.  CS_LINE_EXTEND is currently unused because of poor performance.
           \param tk the toolkit \return a status value to indicate success or failure  */
        TK_Status process_line_extend_points (BStreamFileToolkit & tk) alter;
        /*!writes the points array according to whatever compression scheme was put into m_workspace
           \param tk the toolkit \return a status value to indicate success or failure  */
        TK_Status write_compressed_points (BStreamFileToolkit & tk) alter;
        /*!reads the m_workspace, which will later be decompressed into m_points according to whatever m_compression_scheme was specified
           \param tk the toolkit \return a status value to indicate success or failure  */
        TK_Status read_compressed_points (BStreamFileToolkit & tk) alter;
        /*!if m_points can be rearranged into 2d (see TK_PolyPolypoint::mangle), there will be a 1d array (possibly of 0 or 1 length) of "leftovers".  write it.
           \param tk the toolkit \return a status value to indicate success or failure  */
        TK_Status write_trivial_leftovers (BStreamFileToolkit & tk) alter;
        /*!read the "leftovers" (see (see TK_PolyPolypoint::write_trivial_leftovers)
           \param tk the toolkit \return a status value to indicate success or failure  */
        TK_Status read_trivial_leftovers (BStreamFileToolkit & tk) alter;

    public:
		/*! constructor */
        TK_PolyPolypoint (unsigned char opcode);
        ~TK_PolyPolypoint ();

        TK_Status   Read (BStreamFileToolkit & tk) alter;
        TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
        TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		TK_Status   Execute (BStreamFileToolkit & tk) alter;    /* non-default for tag processing */
        void        Reset (void) alter;

        /*! Sets the point array.  Allocates a buffer which will hold 'point_count' float triplets, and if specified,
            copies the 'points' into the buffer 
            \param point_count the aggregate total number of points in this instance of the opcode.
            \param points if provided, the actual floating-point values of the vertices. 
             \return a status value to indicate success or failure. */
        TK_Status   SetPoints (int point_count, float const * points = 0) alter;
        /*! Sets the point array.  Allocates a buffer which will hold 'primitive_count' integers, and if specified,
            copies the 'points' into the buffer.
            \param primitive_count the number of polylines that this instance of the opcode handler contains.
            \param lengths if provided, an array with point_count values to provide the length (number of vertices) 
                    of each polyline.  If not provided, all lengths are initialized to zero (just assume that they will
                    be set at some other time. 
             \return a status value to indicate success or failure. */
        TK_Status   SetLengths (int primitive_count, int const * lengths = 0) alter;

        /*! \enum Suboptions a few control values, and a lot of bits to indicate degenerate dimensions
            (which can be either omitted completely or only partially specified). */
        enum Suboptions {
            TKPP_COMPRESSED                     = 0x0001,
            TKPP_HAS_EXPLICIT_PRIMITIVE_COUNT   = 0x0002,
            TKPP_ONE_PRIMITIVE_ONLY             = 0x0004,
            TKPP_GLOBAL_QUANTIZATION            = 0x0008,
            //break in sequence
            TKPP_ZERO_X                         = 0x0100,
            TKPP_SAME_X                         = 0x0200,
            TKPP_PER_PRIMITIVE_X                = 0x0300, //multibit
            TKPP_ZERO_Y                         = 0x0400,
            TKPP_SAME_Y                         = 0x0800,
            TKPP_PER_PRIMITIVE_Y                = 0x0C00, //multibit
            TKPP_ZERO_Z                         = 0x1000,
            TKPP_SAME_Z                         = 0x2000,
            TKPP_PER_PRIMITIVE_Z                = 0x3000, //multibit
            TKPP_EXPANDED                       = 0x8000  //reserved for future use
        };
        #define TKPP_X_2D_MASK 0x0300
        #define TKPP_Y_2D_MASK 0x0C00
        #define TKPP_Z_2D_MASK 0x3000
        #define TKPP_ANY_2D_MASK 0x3F00
        #define TKPP_EXPLICIT_PRIMITIVE_MASK 0x0006
};

#endif //BPOLYPOLY

