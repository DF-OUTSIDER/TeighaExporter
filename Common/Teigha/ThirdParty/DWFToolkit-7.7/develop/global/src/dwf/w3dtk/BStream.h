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
// $Header: //DWF/Working_Area/Willie.Zhu/w3dtk/BStream.h#1 $
//

//! Contains definitions of global variables, functions and enumerated types.
/*! \file BStream.h
    The HOOPS/Stream Toolkit utilizes several global enumerated types.  The functions which black box support for writing
    and reading HOOPS Stream Files are also global, and are defined in BStream.h
*/

#include "dwf/Toolkit.h"

#ifndef BBINFILETKHEADER
#define BBINFILETKHEADER

#ifdef __ia64
    #ifndef __HP_aCC
        #error C++ compiler required
    #endif
#else
    #ifndef __cplusplus
        #error C++ compiler required
    #endif
#endif

//
// Required for non-Win32 builds
// This pulls in the ./configure settings
//
#ifdef  HAVE_CONFIG_H
#include "dwf/config.h"
#endif

#include "OdPlatformSettings.h"
#ifdef ODA_BIGENDIAN
#define STREAM_BIGENDIAN
#else
#undef STREAM_BIGENDIAN
#endif


//////////////////////////////////////////////////////////////////////////
//
// Recognize the newer DWF Toolkit pre-processor macros
// 
//
#ifdef  DWFTK_STATIC
#ifndef BSTREAM_STATIC_LIB
#define BSTREAM_STATIC_LIB
#endif
#ifndef HOOPSUTILITY_STATIC_LIB
#define HOOPSUTILITY_STATIC_LIB
#endif
#endif

#ifdef  DWFTK_READ_ONLY
#ifndef BSTREAM_READONLY
#define BSTREAM_READONLY
#endif
#endif

#ifdef  DWFTK_EXPORT_API
#ifndef BBINFILETK_EXPORTS
#define BBINFILETK_EXPORTS
#endif
#ifndef HOOPSUTILITY_EXPORTS
#define HOOPSUTILITY_EXPORTS
#endif
#endif

//
// For DWF, we want this disabled always!
//
#ifndef BSTREAM_DISABLE_REPULSE_COMPRESSION
#define BSTREAM_DISABLE_REPULSE_COMPRESSION
#endif
//////////////////////////////////////////////////////////////////////////


/*! 
    \def BBINFILETK_API2 resolves to whatever special keywords are required to make a function or class
    part of the externally supported API.
*/
#if defined(_MSC_VER) && !defined(BSTREAM_STATIC_LIB)
    #ifdef BBINFILETK_EXPORTS
        #define BBINFILETK_API2  __declspec (dllexport)
    #else
        #define BBINFILETK_API2  __declspec (dllimport)
    #endif
#else
    #define BBINFILETK_API2
#endif


/*!
	\def BBINFILETK_API Under most circumstances, simply resolves to the same thing as BBINFILETK_API2. 
*/
#ifdef TEST_RIG_1
    #define BBINFILETK_API
#else
    #define BBINFILETK_API BBINFILETK_API2
#endif


/*! \def TK_File_Format_Version
		 the HSF specification version that this toolkit supports (along with any previous versions) 
*/
#define TK_File_Format_Version       1555   // 15.55

//!  Table of format changes, commented values are informational (were never included in a release,
//    but are mentioned in the spec as test points for format changes)
#ifdef CREATE_FILE_FORMAT_HISTORY_TABLE
    BBINFILETK_API int TK_File_Format_History[] = {
		//  Format   Release     Notes
		//------------------------------------------------------------------------------------------
		//	1555	//			fix to some color types
		//	1550	//			colors can now have Geo_Extended2, shadow map/simple reflection rendering options
		//	1520	//			geometry reference
		//	1515	//			shadow map options, selection level, display list levels, simple reflection, infinite line opcode
		//	1510	//			maximum extent mode
		//	1505	//			screen range, depth images
			1500,	// 1500
		//	1415	//			obsscuration use octree
		//	1410	//			simple shadow opacity, anisotropic texture filter
		//	1405	//			join cutoff angle, ordered drawing heuristics
			1401,	// 1410		internal polyline selection limit
			1400,	// 1400
		//	1345	//			string format color maps
		//	1340	//			rgba image compression
		//  1335	//			general displacement
		//	1330	//			priority
		//	1325	//			text greeking mode, extended text renderer & text preference
		//	1320	//			Heuristic_Detail_Selection, nonculled edge visibility
		//	1315	//			distance threshold LOD option, new fallback type "bounding none"
		//	1310	//			depth peeling
		//	1305	//			cutting section, adjacent edge visibility, force grayscale
			1300,	// 1300
		//	1220	//			font bold/italic/renderer, cut geometry color locks
		//	1215	//			circular center
		//	1210	//			geometry-level attributes, text character attributes, unicode32, image tint
		//	1205	//			obscuration pixel threshold, subpixel culling, hard edge angle
			1200,	// 1200
		//	1175	//			explicit parameter width, uncompressed parameters
		//	1170	//			camera relative lights, simple shadow auto mode
		//	1165	//			extended texture definitions
		//	1160	//			glyph definitions & line styles
		//	1155	//			spheres, texture downsamping, compressed dictionary, image reference
		//	1150	//			conditions, new hlr options (color, weight)
			1105,	//			additional polyhedron attributes
			1100,	// 1100
		//	1002	//			simple shadow options, text region
		//	1001	//			camera near_limit, mask transform, image size/scale, new texture options
			1000,	// 1000
		//	 908	//			threshold type in LOD options
		//	 907	//			polar normals
		//	 906	//			edgebreaker changes
		//	 905	//			hlr silhouette cleanup, unicode_options
			 900,	//  900
		//	 806	//			bounding box inheritance, cut geometry options, fast hidden line,
		//						hidden line render faces, depth range, gradient window patterns,
		//						additional NURBS surface controls
		//	 805	//			polycylinder attributes, ExRef, URL
			 800,	//  800
			 710,	//  760		new transparency controls, dictionary format 2
		//	 705,	//			PolyCylinder, Extended Heuristics, XML, LoD collections, Shell Regions
		//	 702,	//			PolyPolypoint
			 700,	//  700
		//	 652,	//			null shell
			 651,	//  661
		//	 650,	//			NURBS Surface, Cylinder, Extended Rendering_Options, Defined Fonts, Selectability
		//						extensive polyhedron data & compression changes
		//	 610,	//			View
		//	 600,	//			Stream version generally synched to Hoops version
		//	 601,	//			support longer text string
			 105,	//  600		Generate_Dictionary changed to Suppress_Dictionary
			 100,	//  500		Initial release
			   0	//			[list termination]
    };
#else
    extern BBINFILETK_API int TK_File_Format_History[];
#endif


#define TK_DEFAULT_BUFFER_SIZE     32768    //!< default amount of the internal memory buffer used for file processing



#ifdef HSTREAM_READONLY
    #ifndef BSTREAM_READONLY
        #define BSTREAM_READONLY
    #endif
#endif
#ifdef BSTREAM_READONLY
    extern char const stream_readonly[];
#endif
#ifdef BSTREAM_DISABLE_QUANTIZATION
    extern char const stream_disable_quantization[];
#endif
#ifdef BSTREAM_DISABLE_EDGEBREAKER
    extern char const stream_disable_edgebreaker[];
#endif
#ifdef BSTREAM_DISABLE_REPULSE_COMPRESSION
    extern char const stream_disable_repulse_compression[];
#endif
#ifdef BSTREAM_DISABLE_ASCII
	extern char const stream_disable_ascii[];
#endif


#include <math.h>
#include <string.h>
#include <stdio.h>

#ifdef NEED_BOOL_TYPE
    #undef NEED_BOOL_TYPE
    typedef int bool;

    const bool true = !0;
    const bool false = 0;
#endif


/*! ID_Key is just a pointer sized long integer by which it is assumed that the graphics system
    can uniquely identify its contents.  Declared as 64-bit on win64 systems 
    because a long is not big enough to store a pointer there (unlike all other 64-bit systems
    we have supported in the past */
#ifdef _WIN64
#   define  ID_Key      __int64
#else
#   define  ID_Key      long
#endif



#ifndef alter
#   define alter   //!< complementary to const, indicates we thought about it instead of a forgotten "const"       
#endif


//! Codes which can be either passed to various toolkit functions, or indicate the result of a toolkit function call 
/*! \enum TK_Status
    Various toolkit functions can take arguments which control processing behavior.  Most functions also provide a return
    value of type TK_Status to indicate success, failure or other information about processing status.
*/
enum TK_Status {
    TK_Normal   = 0, /*!< When used as input: read through to a 'termination' code.  When used as output:  function call succeeded */        
    TK_Error    = 1, /*!< A return value; function call failed */ 
    TK_Pause    = 2, /*!< An input value; instructs toolkit to stop reading at a 'pause' code */
    TK_Single   = 3, /*!< An input value; instructs toolkit to stop reading after the first object is completed */
    TK_Pending  = 4, /*!< A return value; the toolkit is waiting for (needs) more data */
    TK_Revisit  = 5, /*!< A return value; interpret function succeeded, but we will run it through again later */
    TK_Complete = 6, /*!< A return value; processing is complete */
    TK_Version  = 7, /*!< A return value; version mis-match detected, processing halted */
    TK_NotFound = 8, /*!< A return value; failed to find a match */
    TK_Abort    = 9  /*!< A return value; non-fatal error. callback requested stop, or fall back to plan B (internal) */
};


//! Options which control how the HOOPS Stream File is generated
/*! \enum TK_File_Write_Options
	An HSF file has a variety of properties, and the toolkit utilizes default logic when generating a file.  
    This enumerated type gives the user high-level control over file compression, tagging and content.  These
	options are set by calling BStreamFileToolkit::SetWriteFlags

*/
enum TK_File_Write_Options {
    TK_Suppress_LOD               = 0x00000002, /*!< No LODS are exported, only the original shells are written out (single pass); implicitly disables dictionary */
    TK_Full_Resolution_Vertices   = 0x00000004, /*!< Data fidelity is required for geometry; this causes the full single-precision floating point information to be exported; does not affect LODs or lossless compression of connectivity information */
    TK_Full_Resolution_Normals    = 0x00000008, /*!< Data fidelity is required for normals; as in above description of TK_Full_Resolution_Points */
    TK_First_LOD_Is_Bounding_Box  = 0x00000010, /*!< Adds a new LOD to represent the lowest resolution of objects, which is a bounding box representation */
    TK_Force_Tags                 = 0x00000020, /*!< All appropriate objects (geometries,segments,includes) will be tagged */
    TK_Disable_Priority_Heuristic = 0x00000040, /*!< Disables organization of multiresolution objects according to heuristic cost/benifit.  */
    TK_Disable_Global_Compression = 0x00000100, /*!< Inline lossless compression will not be used */
    TK_Disable_Instancing         = 0x00000200, /*!< Object instancing will not be used */
    TK_Generate_Dictionary        = 0x00000400, /*!< Enables random-access by creation of the file dictionary */
    TK_Connectivity_Compression   = 0x00000800, /*!< Enables connectivity compression  */
    TK_Disable_Tristrips          = 0x00001000, /*!< Disables triangle strips, and the triangulation it requires, in shells.  Incurs a heavy read-time computational cost.  */
    TK_Disable_Compound_Primitives = 0x00002000, /*!< Disables the use of OpcodeHandlers that encode multiple primitives (since they cannot be properly tagged) */
    TK_Global_Quantization         = 0x00004000,  /*!< Causes any required quantization to be global (bbox of scene) instead of local (bbox of individual geometry) .  Regardless of this flag, however, local quantization applies until the first TKE_Bounding_Info.  HStreamFiletoolkit sends a TKE_Bounding_Info automatically, but BStreamFiletoolkit does not. */
    TK_Full_Resolution_Parameters  = 0x00008000,  /*!< Data fidelity is required for parameters (a.k.a texture coordinates); as in above description of TK_Full_Resolution_Points */
    TK_Full_Resolution = (TK_Full_Resolution_Vertices|TK_Full_Resolution_Normals|TK_Full_Resolution_Parameters) /*!< for convenience, a combination of 3 other options. */
};


/*! \enum TK_Dictionary_Options
    This enumerated type gives the user control of specific options to include if a dictionary is generated
*/
enum TK_Dictionary_Options {
    TK_Dictionary_Bounding_Volumes  = 0x00000001  //!< dictionary entries include bounding volume info
};


/*! \enum TK_File_Read_Options 
	Options which control how the HOOPS Stream File is read 
*/
enum TK_File_Read_Options {
    TK_Flag_Unhandled_Opcodes     = 0x00000001,  /*!< Instructs toolkit to return an error for opcodes which are not handled by a custom toolkit during reading; the default is to silently skip over unhandled opcodes */
    TK_Ignore_Version             = 0x00000002,  /*!< Version checking will be bypassed */
    TK_Skip_External_References	  = 0x00000004,  /*!< TKE_External_Reference filespecs will not be read (the strings will still be stored in toolkit by the default handler) */
    TK_Ignore_Bounding_LODs		  = 0x00000008   /*!< Instructs the toolkit to ignore any explicit bounding box lods included in the stream file */
};

/*! \enum TK_Debug_Logging_Options
	Options which control what data is collected when creating a debug log
*/
enum TK_Debug_Logging_Options {
    TK_Logging_Sequence           = 0x00000001,  /*!< Record sequence numbers */
    TK_Logging_Tagging            = 0x00000002,  /*!< Record tag indices */
    TK_Logging_Segment_Names      = 0x00000004   /*!< Record segment names */
};



#include "BOpcodeHandler.h"
#include "BStreamFileToolkit.h"


//! Reads a HOOPS Stream File 
/*! 
    The contents of the HOOPS Stream File are read in. Opcodes are handled via either the default handlers
    registered with the toolkit, or using the custom handlers which have been registered by the developer.
    Typically, the HSF data is mapped to application data structures within the custom opcode handler's 
    Execute method.
    \param filename A character pointer denoting the name of the file to read in.
    \param tk A pointer to the BStreamFileToolkit to use as a basis for reading the file.  This would be passed
              in to read in a customized file that contains user-specified data.  Options to control how the file is
              read may be specified using the BStreamFileToolkit::SetReadFlags() method on the toolkit.
*/
TK_Status BBINFILETK_API TK_Read_Stream_File  (char const * filename, BStreamFileToolkit * tk);

//! Reads a HOOPS Stream File 
/*! 
    The contents of the HOOPS Stream File are read in. Opcodes are handled via either the default handlers
    registered with the toolkit, or using the custom handlers which have been registered by the developer.
    Typically, the HSF data is mapped to application data structures within the custom opcode handler's 
    Execute method.
    \param filename A character pointer denoting the name of the file to read in.
    \param flags Bitwise encoding of options which control how the file is read. #TK_File_Read_Options provides
	             a list of supported options.
*/
TK_Status BBINFILETK_API TK_Read_Stream_File  (char const * filename, int flags = 0);

/*! wide character variant of the standard version of TK_Read_Stream_File */
TK_Status BBINFILETK_API TK_Read_Stream_File  (unsigned short const * filename, BStreamFileToolkit * tk);
/*! wide character variant of the standard version of TK_Read_Stream_File */
TK_Status BBINFILETK_API TK_Read_Stream_File  (unsigned short const * filename, int flags = 0);



//! \b Deprecated; reads a HOOPS Stream File 
/*! 
    The contents of the HOOPS Stream File are read in. Opcodes are handled via either the default handlers
    registered with the toolkit, or using the custom handlers which have been registered by the developer.
    Typically, the HSF data is mapped to application data structures within the custom opcode handler's 
    Execute method.
    This form of the function is provided for backward compatibility.
*/
TK_Status BBINFILETK_API TK_Read_Stream_File  (char const * filename, int flags, BStreamFileToolkit * tk);


#endif

