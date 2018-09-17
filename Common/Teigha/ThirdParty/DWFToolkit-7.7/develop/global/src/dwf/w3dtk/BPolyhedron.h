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
// $Header: //DWF/Working_Area/Willie.Zhu/w3dtk/BPolyhedron.h#1 $
//
#ifndef BBINFILETK_POLYHEDRON
#define BBINFILETK_POLYHEDRON

///
///\file        dwf/w3dtk/BPolyhedron.h
///

//! Base class for shell and mesh
/*! 
	Shells and Meshes both have attributes that can bind to faces, edges and vertices
	and may optionally be present.  This base class helps allows the two to share
	the code to parse and execute those features that are common between the two.
	Refer to ::HC_Insert_Shell and Section 2.8 of the HOOPS/3dGS Programming Guide for more details.
*/
class BBINFILETK_API TK_Polyhedron : public BBaseOpcodeHandler {
    private:
        TK_Polyhedron(const TK_Polyhedron&);
        TK_Polyhedron& operator=(const TK_Polyhedron&);

	public:

		/*! \enum Exists
		  Flags for declaring existence of local vertex attributes in mp_exists 
		*/
		enum Exists {                   
			Vertex_None         = 0x0000,  //!< no local vertex attributes
			Vertex_Normal       = 0x0001,  //!< explicit normals
			Vertex_Parameter    = 0x0002,  //!< parameters (for texturing, etc...)
			Vertex_Face_Color   = 0x0004,  //!< vertex color, to be applied to interpolated faces
			Vertex_Edge_Color   = 0x0008,  //!< vertex color, to be applied to interpolated edges
			Vertex_Marker_Color = 0x0010,  //!< vertex color, to be applied to marker
			Vertex_Face_Index   = 0x0020,  //!< vertex color-by-index, to be applied to interpolated faces 
			Vertex_Edge_Index   = 0x0040,  //!< vertex color-by-index, to be applied to interpolated edges
			Vertex_Marker_Index = 0x0080,  //!< vertex color-by-index, to be applied to marker
			Vertex_Any_Color    = 0x00FC,  //!< multi-bit alias to quickly test for any color or color-by-index
			Vertex_Marker_Visibility = 0x0100,
			Vertex_Marker_Size  = 0x0200,
			Vertex_Marker_Symbol= 0x0400
		};

		/*! \enum Face_Exists
			 Flags for declaring existence of local face attributes in mp_face_exists
		*/
		enum Face_Exists {
			Face_None         = 0x0000,  //!< no local face attributes
			Face_Color        = 0x0001,  //!< self-explanatory
			Face_Index        = 0x0002,  //!< face color-by-index
			Face_Any_Color    = 0x0003,  //!< multi-bit alias to quickly test for color or color-by-index
			Face_Normal       = 0x0004,  //!< self-explanatory
			Face_Visibility   = 0x0008,  //!< self-explanatory
			Face_Pattern      = 0x0010   //!< self-explanatory
		};

		/*! \enum Edge_Exists
			 Flags for declaring existence of local edge attributes in mp_edge_exists
		*/
		enum Edge_Exists {
			Edge_None         = 0x0000,  //!< no local face attributes
			Edge_Color        = 0x0001,  //!< self-explanatory
			Edge_Index        = 0x0002,  //!< face color-by-index
			Edge_Any_Color    = 0x0003,  //!< multi-bit alias to quickly test for color or color-by-index
			Edge_Normal       = 0x0004,  //!< self-explanatory
			Edge_Visibility   = 0x0008,  //!< self-explanatory
			Edge_Pattern      = 0x0010,  //!< self-explanatory
			Edge_Weight       = 0x0020,  //!< edge thickness, given as a proportion of the default
			Fake_Edge         = 0x8000  //for edges (if any) that do not exist on the write side, but will be introduced due to triangulation
		};

		/*! \enum Suboptions contains the meanings of the bits put into mp_subop
			mp_subop is a bit field containing information about the shell or mesh.
		 */
		enum Suboptions {
			TKSH_STANDARD                 = 0x00, //!< no special suboptions
			TKSH_COMPRESSED_POINTS        = 0x01, //!< points are compressed
			TKSH_COMPRESSED_FACES         = 0x02, //!< faces are compressed
			TKSH_TRISTRIPS                = 0x04, //!< face list represents tristrips
			TKSH_HAS_OPTIONALS            = 0x08, //!< optionals exist; refer to #Opt_Opcode
			TKSH_FIRSTPASS                = 0x10, //!< this is not a refinement of a previously-seen entity
			TKSH_BOUNDING_ONLY            = 0x20, //!< a bounding box representation only.  
			TKSH_CONNECTIVITY_COMPRESSION = 0x40, //!< the polyedra is compressed using the 'connectivity compression' algorithm
			TKSH_EXPANDED                 = 0x80  //!< expanded suboptions exist; refer to #Suboptions2 
		};

		/*! \enum Suboptions2 contains the meanings of the bits put into mp_subop2
			mp_subop2 is a bit field containing information about the shell or mesh.
			It is assumed to be zero unless mp_subop contains the TKSH_EXPANDED bit.
		*/
		enum Suboptions2 {
			TKSH2_COLLECTION              = 0x0001,  //!< instead of a shell, this opcode contains a TKE_Terminate-terminated list of opcodes to be inserted into a shell's LOD.  Incompatible with TKSH_FIRSTPASS.
			TKSH2_NULL                    = 0x0002,  //!< an empty shell used only as a placeholder for later revisiting
			TKSH2_HAS_NEGATIVE_FACES      = 0x0004,  //!< negative faces exist
			TKSH2_GLOBAL_QUANTIZATION     = 0x0008   //!< polyhedra points are globally quantized
		};

		/*! \enum Opt_Opcode
			 The supported optional fields for local attributes.
			 Note: Odd values indicate that an attribute is present on all vertices (or all faces or all edges) */
		enum Opt_Opcode {
			OPT_TERMINATE                   = 0,  //!< terminate the list of optional attributes
			OPT_ALL_NORMALS_COMPRESSED      = 1,  //!< set compressed normals on all vertices
			OPT_NORMALS_COMPRESSED          = 2,  //!< set compressed normals on some vertices
			OPT_ALL_PARAMETERS_COMPRESSED   = 3,  //!< set compressed parameters (texture coordinates) on all vertices
			OPT_PARAMETERS_COMPRESSED       = 4,  //!< set compressed parameters on some vertices
			OPT_ALL_VFCOLORS                = 5,  //!< set colors on all vertices, to apply to drawing of interpolated faces
			OPT_VERTEX_FCOLORS              = 6,  //!< set colors on some vertices, to apply to drawing of interpolated faces
			OPT_ALL_VECOLORS                = 7,  //!< set colors on all vertices, to apply to drawing of interpolated edges
			OPT_VERTEX_ECOLORS              = 8,  //!< set colors on some vertices, to apply to drawing of interpolated edges
			OPT_ALL_VMCOLORS                = 9,  //!< set colors on all vertices, to apply to drawing of vertex_markers 
			OPT_VERTEX_MCOLORS              = 10, //!< set colors on some vertices, to apply to drawing of vertex_markers 
			OPT_ALL_VFINDICES               = 11, //!< set colors-by-index on all vertices, to apply to drawing of interpolated faces
			OPT_VERTEX_FINDICES             = 12, //!< set colors-by-index on some vertices, to apply to drawing of interpolated faces
			OPT_ALL_VEINDICES               = 13, //!< set colors-by-index on all vertices, to apply to drawing of interpolated edges
			OPT_VERTEX_EINDICES             = 14, //!< set colors-by-index on some vertices, to apply to drawing of interpolated edges
			OPT_ALL_VMINDICES               = 15, //!< set colors-by-index on all vertices, to apply to drawing of vertex_markers 
			OPT_VERTEX_MINDICES             = 16, //!< set colors-by-index on some vertices, to apply to drawing of vertex_markers 
			OPT_ALL_NORMALS                 = 17, //!< set normals on all vertices 
			OPT_NORMALS                     = 18, //!< set normals on some vertices 
			OPT_ALL_NORMALS_POLAR           = 19, //!< set normals (specified as polar coordinates) on all vertices 
			OPT_NORMALS_POLAR               = 20, //!< set normals (specified as polar coordinates) on some vertices 
			OPT_ALL_VMVISIBILITIES          = 21, //!< put explicit local visibility setting on all vertex markers
			OPT_VERTEX_MARKER_VISIBILITIES  = 22, //!< put explicit local visibility setting on some vertex markers 
			OPT_ALL_VMSYMBOLS               = 23, //!< put explicit local marker symbol setting on all vertices 
			OPT_VERTEX_MARKER_SYMBOLS       = 24, //!< put explicit local marker symbol setting on some vertices 
			OPT_ALL_VMSIZES                 = 25, //!< put explicit local size setting on all vertex markers
			OPT_VERTEX_MARKER_SIZES         = 26, //!< put explicit local size setting on some vertex markers 
			OPT_PARAMETERS					= 27, //!< set parameters (texture coordinates) on some vertices
			OPT_ALL_PARAMETERS				= 28, //!< set parameters (texture coordinates) on all vertices
			/**/
			OPT_ALL_FACE_COLORS             = 33, //!< set colors on all faces
			OPT_FACE_COLORS                 = 34, //!< set colors on some faces
			OPT_ALL_FACE_INDICES            = 35, //!< set colors-by-index on all faces
			OPT_FACE_INDICES                = 36, //!< set colors-by-index on some faces
			OPT_ALL_FACE_NORMALS_POLAR      = 37, //!< set normals (specified as polar coordinates) on all faces 
			OPT_FACE_NORMALS_POLAR          = 38, //!< set normals (specified as polar coordinates) on some faces 
			OPT_ALL_FACE_VISIBILITIES       = 39, //!< put explicit local visibility setting on all faces
			OPT_FACE_VISIBILITIES           = 40, //!< put explicit local visibility setting on some faces 
			OPT_ALL_FACE_PATTERNS           = 41, //!< put explicit local pattern setting on all faces
			OPT_FACE_PATTERNS               = 42, //!< put explicit local pattern setting on some faces 
			/**/
			OPT_FACE_REGIONS                = 51,  //!< apply region identifiers to some faces (for the purposes of grouping faces so that their attributes can be set at once)
			/**/
			OPT_ALL_EDGE_COLORS             = 71, //!< set colors on all edges 
			OPT_EDGE_COLORS                 = 72, //!< set colors on some edges 
			OPT_ALL_EDGE_INDICES            = 73, //!< set colors-by-index on all edges 
			OPT_EDGE_INDICES                = 74, //!< set colors-by-index on some edges 
			OPT_ALL_EDGE_NORMALS_POLAR      = 75, //!< set normals (specified as polar coordinates) on all edges  
			OPT_EDGE_NORMALS_POLAR          = 76, //!< set normals (specified as polar coordinates) on some edges  
			OPT_ALL_EDGE_VISIBILITIES       = 77, //!< put explicit local visibility setting on all edges 
			OPT_EDGE_VISIBILITIES           = 78, //!< put explicit local visibility setting on some edges  
			OPT_ALL_EDGE_PATTERNS           = 79, //!< put explicit local pattern setting on all edges 
			OPT_EDGE_PATTERNS               = 80, //!< put explicit local pattern setting on some edges  
			OPT_ALL_EDGE_WEIGHTS            = 81, //!< put explicit local thickness setting on all edges 
			OPT_EDGE_WEIGHTS                = 82, //!< put explicit local thickness setting on some edges  
			/**/
			OPT_RESERVED_SPATIAL			= 90,
			OPT_ENTITY_ATTRIBUTES           = 99
		};


		//see TK_Shell for GetFacesLength, GetFaces, GetLodLevel, SetFaces, SetLodLevel

		/*! \return a pointer to the array of vertex locations */
		float const *GetPoints() const { return mp_points; };
		/*! \return a pointer to the array of vertex locations */
		float *GetPoints() { return mp_points; };
		/*! \return the number of points in the polyhedron */
		int GetPointCount() const { return mp_pointcount; };
		/*! \return a pointer to the existence array, a bitmask indicating presence/absence of attributes */
		unsigned int const *GetExists() const { return mp_exists; };
		/*! \return a pointer to the existence array, a bitmask indicating presence/absence of attributes */
		unsigned int *GetExists() { return mp_exists; };
		/*! \return a pointer to the existence array for faces, mp_face_exists, indicating face attributes */
		unsigned int const *GetFaceExists() const { return mp_face_exists; };
		/*! \return a pointer to the existence array for faces, mp_face_exists, indicating face attributes */
		unsigned int *GetFaceExists() { return mp_face_exists; };
		/*! \return the id by which the graphics knows this piece of geometry (could be pointer or integer) */
		ID_Key GetKey() const { return mp_key; };
		/*! \return the suboption that will be transmitted to indicate formatting options */
		unsigned char GetSubop() const { return mp_subop; };
		/*! \return the 2nd suboption that will be transmitted to indicate formatting options */
		unsigned short GetSubop2() const { return mp_subop2; };
		/*! \return a pointer to the bounding box (x_min, y_min, z_min, x_max, y_max, z_max) */
		const float *GetBounding() const { return mp_bbox; };
		/*! \return a pointer to the array of vertex normals */
		float const *GetVertexNormals() const { return mp_normals; };
		/*! \return a pointer to the array of vertex normals */
		float *GetVertexNormals() { return mp_normals; };
		/*! \return a pointer to the array of vertex parameters */
		float const *GetVertexParameters() const { return mp_params; };
		/*! \return a pointer to the array of vertex parameters */
		float *GetVertexParameters() { return mp_params; };
		/*! \return a pointer to the array of vertex colors (as applied to the faces) */
		float const *GetVertexFaceColors() const { return mp_vfcolors; };
		/*! \return a pointer to the array of vertex colors (as applied to the faces) */
		float *GetVertexFaceColors() { return mp_vfcolors; };
		/*! \return a pointer to the array of vertex colors (as applied to the edges) */
		float const *GetVertexEdgeColors() const { return mp_vecolors; };
		/*! \return a pointer to the array of vertex colors (as applied to the edges) */
		float *GetVertexEdgeColors() { return mp_vecolors; };
		/*! \return a pointer to the array of vertex colors (as applied to the markers) */
		float const *GetVertexMarkerColors() const { return mp_vmcolors; };
		/*! \return a pointer to the array of vertex colors (as applied to the markers) */
		float *GetVertexMarkerColors() { return mp_vmcolors; };
		/*! \return the first non-null pointer it among the following: mp_vmcolors (a.k.a. VertexMarkerColors),
		   mp_vfcolors (a.k.a. VertexFaceColors),  mp_vecolors (a.k.a. VertexEdgeColors) */
		float const *GetVertexColors() const { 
			if( mp_vmcolors != 0 ) return mp_vmcolors;
			else if( mp_vfcolors != 0 ) return mp_vfcolors;
			else if( mp_vecolors != 0 ) return mp_vecolors;
			else return 0;
		};
		/*! \return the first non-null pointer it among the following: mp_vmcolors (a.k.a. VertexMarkerColors),
		   mp_vfcolors (a.k.a. VertexFaceColors),  mp_vecolors (a.k.a. VertexEdgeColors) */
		float *GetVertexColors() { 
			if( mp_vmcolors != 0 ) return mp_vmcolors;
			else if( mp_vfcolors != 0 ) return mp_vfcolors;
			else if( mp_vecolors != 0 ) return mp_vecolors;
			else return 0;
		};
		/*! \return a pointer to the array of vertex colors-by-index (as applied to the faces) */
		float const *GetVertexFaceIndices() const { return mp_vfindices; };
		/*! \return a pointer to the array of vertex colors-by-index (as applied to the faces) */
		float *GetVertexFaceIndices() { return mp_vfindices; };
		/*! \return a pointer to the array of vertex colors-by-index (as applied to the edges) */
		float const *GetVertexEdgeIndices() const { return mp_veindices; };
		/*! \return a pointer to the array of vertex colors-by-index (as applied to the edges) */
		float *GetVertexEdgeIndices() { return mp_veindices; };
		/*! \return a pointer to the array of vertex colors-by-index (as applied to the markers) */
		float const *GetVertexMarkerIndices() const { return mp_vmindices; };
		/*! \return a pointer to the array of vertex colors-by-index (as applied to the markers) */
		float *GetVertexMarkerIndices() { return mp_vmindices; };
		/*! \return the first non-null pointer it among the following: mp_vmindices (a.k.a. VertexMarkerIndices),
		   mp_vfindices (a.k.a. VertexFaceIndices),  mp_veindices (a.k.a. VertexEdgeIndices) */
		float const *GetVertexIndices() const { 
			if( mp_vmindices != 0 ) return mp_vmindices;
			else if( mp_vfindices != 0 ) return mp_vfindices;
			else if( mp_veindices != 0 ) return mp_veindices;
			else return 0;
		};
		/*! \return the first non-null pointer it among the following: mp_vmindices (a.k.a. VertexMarkerIndices),
		   mp_vfindices (a.k.a. VertexFaceIndices),  mp_veindices (a.k.a. VertexEdgeIndices) */
		float *GetVertexIndices() { 
			if( mp_vmindices != 0 ) return mp_vmindices;
			else if( mp_vfindices != 0 ) return mp_vfindices;
			else if( mp_veindices != 0 ) return mp_veindices;
			else return 0;
		};
		/*! \return a pointer to the array of face colors */
		float const *GetFaceColors() const { return mp_fcolors; };
		/*! \return a pointer to the array of face colors */
		float *GetFaceColors() { return mp_fcolors; };
		/*! \return a pointer to the array of face colors-by-index */
		float const *GetFaceIndices() const { return mp_findices; };
		/*! \return a pointer to the array of face colors-by-index */
		float *GetFaceIndices() { return mp_findices; };
		/*! \return a pointer to the array of face regiions */
		int const * GetFaceRegions() const { return mp_fregions; };
		/*! \return a pointer to the array of face regions, which may be modified in-place */
		int alter * GetFaceRegions() alter { return mp_fregions; };

		//!informs the TK_Polyhedron of the id by which this piece of geometry is known
		TK_Status SetKey( BStreamFileToolkit &tk, ID_Key key ) alter { set_last_key( tk, key ); mp_key = key; return TK_Normal; };
		/*!sets the suboption to be used when transmitting. See also InitSubop
		  \param subop a set of bit flags as documented in enum TK_Polyhedron::Suboptions */
		TK_Status SetSubop( unsigned char subop ) alter { mp_subop = subop; return TK_Normal; };
		/*!sets the 2nd suboption to be used when transmitting. See also InitSubop
		  \param subop2 a set of bit flags as documented in enum TK_Polyhedron::Suboptions2 */
		TK_Status SetSubop2( unsigned short subop2 ) alter { mp_subop2 = subop2; return TK_Normal; };
		/*!initializes some bits in the suboption according to the write flags in the toolkit.
		  \param tk the toolkit. 
				  \param lodlevel which lod variant is to be encoded.  
		  \param is_firstpass indicates whether or not this particular object has been seen before in the file. */
		TK_Status InitSubop( BStreamFileToolkit &tk, int lodlevel, bool is_firstpass ) alter;
		/*!Set the bounding box for the object. 
						\param bbox the bounding volume, in order minx,miny,minz,maxx,maxy,maxz */
		TK_Status SetBounding( float const *bbox ) alter { mp_bbox[0] = bbox[0]; mp_bbox[1] = bbox[1]; mp_bbox[2] = bbox[2]; mp_bbox[3] = bbox[3]; mp_bbox[4] = bbox[4]; mp_bbox[5] = bbox[5]; return TK_Normal; };
		/*!Allocates the vertex locations, and fills them with the specified values.  Be careful not to call this function
		   more than once, as it can cause a memory leak.
		   \param count the number of points in the polyhedron
		   \param points a pointer to an array of xyz triplets to specify vertex locations. */
		TK_Status SetPoints( int count, float const *points=0 ) alter;
		/*!Allocates the vertex normals, and fills with values, and sets the flags in mp_exists.
		   \param normals can be null, in which case normals are left untouched (assumed to be specified at a later
			point.  Normally, however, "normals" should refer to the xyz triplets for the values of the vertex normals.
			These values will be copied, and the appropriate bits in mp_exists will be updated. \return TK_Normal or TK_Error to indicate success or failure. */
		TK_Status SetVertexNormals( float const *normals=0 ) alter;
		/*!Allocates the vertex parameters (a.k.a. texture coordinates), and fills with values and sets mp_exists.
		   \param parameters can be null, in which case parameters are left untouched (assumed to be specified at a later
			point.  Normally, however, "parameters" should refer to the stu triplets (again, triplets, not pairs) for the 
			values of the vertex parameters. These values will be copied, and the appropriate bits in mp_exists will 
			be updated.  If "width" is  <= 0, this function will use the value from mp_paramwidth.
		   \param width specifies the number of floating point values per parameter.  For example, a single 2d texture
			will have 2, whereas two 3d textures will have 6.
		   \return TK_Normal or TK_Error to indicate success or failure. */
		TK_Status SetVertexParameters( float const *parameters=0, int width=0 ) alter;
		/*!Allocates the vertex colors (as applied to faces), and fills with values and sets the flags in mp_exists.
			 Allocated to a length of mp_pointcount, regardless of the number of values that are intended to be specified.
		   \param colors can be null, in which case colors are left untouched (assumed to be specified at a later
			point.  Normally, however, "colors" should refer to the rgb triplets for the 
			values of the vertex colors. These values will be copied, and the appropriate bits in mp_exists will 
			be updated. 
		   \return TK_Normal or TK_Error to indicate success or failure. */
		TK_Status SetVertexFaceColors( float const *colors=0 ) alter;
		/*!Allocates the vertex colors (as applied to edges), and fills with values and sets the flags in mp_exists.
			Allocated to a length of mp_pointcount, regardless of the number of values that are intended to be specified.
		   \param colors can be null, in which case colors are left untouched (assumed to be specified at a later
			point.  Normally, however, "colors" should refer to the rgb triplets for the 
			values of the vertex colors. These values will be copied, and the appropriate bits in mp_exists will 
			be updated. 
		   \return TK_Normal or TK_Error to indicate success or failure. */
		TK_Status SetVertexEdgeColors( float const *colors=0 ) alter;
		/*!Allocates the vertex colors (as applied to markers), and fills with values and sets the flags in mp_exists.
			Allocated to a length of mp_pointcount, regardless of the number of values that are intended to be specified.
		   \param colors can be null, in which case colors are left untouched (assumed to be specified at a later
			point.  Normally, however, "colors" should refer to the rgb triplets for the 
			values of the vertex colors. These values will be copied, and the appropriate bits in mp_exists will 
			be updated. 
		   \return TK_Normal or TK_Error to indicate success or failure. */
		TK_Status SetVertexMarkerColors( float const *colors=0 ) alter;
		/*!Allocates the vertex colors in all 3 arrays (faces, edges, markers).  Also handles 
			setting bits in mp_exists (the existence array) if input is non-null.  Equivalent to SetVertexFaceColors,
			SetVertexFaceColors and SetVertexMarkerColors called in succession.
			Allocated to a length of mp_pointcount, regardless of the number of values that are intended to be specified.
		   \param colors can be null, in which case colors are left untouched (assumed to be specified at a later
			point.  Normally, however, "colors" should refer to the rgb triplets for the 
			values of the vertex colors. These values will be copied, and the appropriate bits in mp_exists will 
			be updated. 
		   \return TK_Normal or TK_Error to indicate success or failure. */
		TK_Status SetVertexColors( float const *colors=0 ) alter;
		/*!Allocates the vertex colors-by-index (as applied to faces), and fills with values and sets the 
			flags in mp_exists. Assumes the prior existence of a color table in the current segment.
			Allocated to a length of mp_pointcount, regardless of the number of values that are intended to be specified.
		   \param indices can be null, in which case the indices are left untouched (assumed to be specified at a later
			point.  Normally, however, "indices" should refer to the color indices (individual floats.  Thus, the array
			size is mp_pointcount*sizeof(float)).  These values will be copied, and the appropriate bits in mp_exists will 
			be updated. 
		   \return TK_Normal or TK_Error to indicate success or failure. */
		TK_Status SetVertexFaceIndices( float const *indices=0 ) alter;
		/*!Allocates the vertex colors-by-index (as applied to edges), and fills with values and sets the 
			flags in mp_exists. Assumes the prior existence of a color table in the current segment.
			Allocated to a length of mp_pointcount, regardless of the number of values that are intended to be specified.
		   \param indices can be null, in which case indices are left untouched (assumed to be specified at a later
			point.  Normally, however, "indices" should refer to the color indices (individual floats.  Thus, the array
			size is mp_pointcount*sizeof(float)).  These values will be copied, and the appropriate bits in mp_exists will 
			be updated. 
		   \return TK_Normal or TK_Error to indicate success or failure. */
		TK_Status SetVertexEdgeIndices( float const *indices=0 ) alter;
		/*!Allocates the vertex colors-by-index (as applied to markers), and fills with values and sets the 
			flags in mp_exists. Assumes the prior existence of a color table either in or inherited by the current segment.
			Allocated to a length of mp_pointcount, regardless of the number of values that are intended to be specified.
		   \param indices can be null, in which case indices are left untouched (assumed to be specified at a later
			point.  Normally, however, "indices" should refer to the color indices (individual floats.  Thus, the array
			size is mp_pointcount*sizeof(float)).  These values will be copied, and the appropriate bits in mp_exists will 
			be updated. 
		   \return TK_Normal or TK_Error to indicate success or failure. */
		TK_Status SetVertexMarkerIndices( float const *indices=0 ) alter;
		/*!Allocates the vertex colors-by-index in all 3 arrays (faces, edges, markers).  Also handles 
			setting bits in mp_exists (the existence array) if input is non-null.  Equivalent to SetVertexFaceIndices,
			SetVertexFaceIndices and SetVertexMarkerIndices called in succession.
			All arrays allocated to a length of mp_pointcount, regardless of the number of values that are intended to be specified.
		   \param indices can be null, in which case indices are left untouched (assumed to be specified at a later
			point.  Normally, however, "indices" should refer to the color indices (individual floats.  Thus, the array
			size is mp_pointcount*sizeof(float)).  These values will be copied, and the appropriate bits in mp_exists will 
			be updated. 
		   \return TK_Normal or TK_Error to indicate success or failure. */
		TK_Status SetVertexIndices( float const *indices=0 ) alter;
		/*!Allocates the vertex marker visibilities, and fills with values and sets the flags in mp_exists.
			Allocated to a length of mp_pointcount, regardless of the number of values that are intended to be specified.
		   \param visibilities can be null, in which case the contents of the visibility array are left untouched 
			(assumed to be specified at a later point).  If non-null, should to be at least of length mp_pointcount
		   \return TK_Normal or TK_Error to indicate success or failure. */
		TK_Status SetVertexMarkerVisibilities( char const *visibilities=0 ) alter;
		/*!Allocates the vertex marker sizes, and fills with values and sets the flags in mp_exists.
			Marker sizes are specified as a proportion of their default size.
			Allocated to a length of mp_pointcount, regardless of the number of values that are intended to be specified.
		   \param sizes can be null, in which case the contents of the sizes array are left untouched 
			(assumed to be specified at a later point).  If non-null, should to be at least of length mp_pointcount
		   \return TK_Normal or TK_Error to indicate success or failure. */
		TK_Status SetVertexMarkerSizes( float const *sizes=0 ) alter;
		/*!Allocates the vertex marker symbols, and fills with values and sets the flags in mp_exists.
			Symbol interpretations are as specified in TKE_Marker_Symbol.
			Allocated to a length of mp_pointcount, regardless of the number of values that are intended to be specified.
		   \param symbols can be null, in which case the contents of the symbols array are left untouched 
			(assumed to be specified at a later point).  If non-null, should to be at least of length mp_pointcount
		   \return TK_Normal or TK_Error to indicate success or failure. */
		TK_Status SetVertexMarkerSymbols( char const *symbols=0 ) alter;
		/*!Allocates the vertex marker symbol strings, and initializes to empty.
			Allocated to a length of mp_pointcount, regardless of the number of values that are intended to be specified.
		   \return TK_Normal or TK_Error to indicate success or failure. */
		TK_Status SetVertexMarkerSymbolStrings (void) alter;
		/*!Allocates the face colors, and fills with values and sets the flags in mp_face_exists.
			Allocates length of mp_facecount, regardless of the number of values that are intended to be specified.
		   \param colors can be null, in which case colors are left untouched (assumed to be specified at a later
			point).  If non-null, it should be of length mp_facecount
		   \return TK_Normal or TK_Error to indicate success or failure. */
		TK_Status SetFaceColors( float const *colors=0 ) alter;
		/*!Allocates the face normals, and fills with values and sets the flags in mp_face_exists.
			Allocates length of mp_facecount, regardless of the number of values that are intended to be specified.
		   \param normals can be null, in which case the normals are left untouched (assumed to be specified at a later
			point). If non-null, it should be of length mp_facecount
		   \return TK_Normal or TK_Error to indicate success or failure. */
		TK_Status SetFaceNormals( float const *normals=0 ) alter;
		/*!Allocates the face visibility settings, fills with values and sets the flags in mp_face_exists.
			Allocates length of mp_facecount, regardless of the number of values that are intended to be specified.
		   \param visibilities can be null, in which case colors are left untouched (assumed to be specified at a later
			point). If non-null, it should be of length mp_facecount
		   \return TK_Normal or TK_Error to indicate success or failure. */
		TK_Status SetFaceVisibilities( char const *visibilities=0 ) alter;
		/*!Allocates the face pattern settings (i.e. per face, which pattern to choose), fills with values 
			and sets the flags in mp_face_exists.
			Allocates length of mp_facecount, regardless of the number of values that are intended to be specified.
		   \param patterns can be null, in which case colors are left untouched (assumed to be specified at a later
			point). If non-null, it should be of length mp_facecount
		   \return TK_Normal or TK_Error to indicate success or failure. */
		TK_Status SetFacePatterns( char const *patterns=0 ) alter;
		/*!Allocates the face colors-by-index, and fills with values and sets the flags in mp_face_exists.
			Allocates length of mp_facecount, regardless of the number of values that are intended to be specified (thus
			the array size is mp_facecount*sizeof(float))).
		   \param indices can be null, in which case indices are left untouched (assumed to be specified at a later
			point). If non-null, These values will be copied, and the appropriate bits in 
			mp_face_exists will be updated. 
		   \return TK_Normal or TK_Error to indicate success or failure. */        
		TK_Status SetFaceIndices( float const *indices=0 ) alter;
		/*!Allocates an array of integers to specify a region identifier per face.  
			\param regions are the identifiers.  If set as null, the array will default to all zero */
		TK_Status SetFaceRegions (int const * regions=0) alter;
		/*!Allocates the edge colors, and fills with values and sets the flags in mp_edge_exists.
			Allocates length of mp_edgecount, regardless of the number of values that are intended to be specified.
			Note (as with all SetEdge* methods) that if the value for mp_edgecount is unknown (left at its 
			initial value of -1), it is calculated during the process of EnumerateEdges, called from SetEdgeExists,
			called automatically from this function if necessary.
		   \param colors can be null, in which case colors are left untouched (assumed to be specified at a later
			point).  If non-null, it should be of length mp_edgecount.  
		   \return TK_Normal or TK_Error to indicate success or failure. */
		TK_Status SetEdgeColors( float const *colors=0 ) alter;
		/*!Allocates the edge colors-by-index, and fills with values and sets the flags in mp_edge_exists.
			Allocates length of mp_edgecount, regardless of the number of values that are intended to be specified.
		   \param indices can be null, in which case colors-by-index are left untouched 
			(assumed to be specified at a later
			point).  If non-null, it should be of length mp_edgecount.  
		   \return TK_Normal or TK_Error to indicate success or failure. */
		TK_Status SetEdgeIndices( float const *indices=0 ) alter;
		/*!Allocates the edge normals, and fills with values and sets the flags in mp_edge_exists.
			Allocates length of mp_edgecount, regardless of the number of values that are intended to be specified.
		   \param normals can be null, in which case normals are left untouched (assumed to be specified at a later
			point).  If non-null, it should be of length mp_edgecount.  
		   \return TK_Normal or TK_Error to indicate success or failure. */
		TK_Status SetEdgeNormals( float const *normals=0 ) alter;
		/*!Allocates the edge visibility settings, and fills with values and sets the flags in mp_edge_exists.
			For each edge, non-zero indicates that that edge is visibile.
			Allocates length of mp_edgecount, regardless of the number of values that are intended to be specified.
		   \param visibilities can be null, in which case visibility settings are left untouched (assumed to be specified at a later
			point).  If non-null, it should be of length mp_edgecount.  
		   \return TK_Normal or TK_Error to indicate success or failure. */
		TK_Status SetEdgeVisibilities( char const *visibilities=0 ) alter;
		/*!Allocates the edge patterns, and fills with values and sets the flags in mp_edge_exists.
			Allocates length of mp_edgecount, regardless of the number of values that are intended to be specified.
		   \param patterns can be null, in which case patterns are left untouched (assumed to be specified at a later
			point).  If non-null, it should be of length mp_edgecount.  
		   \return TK_Normal or TK_Error to indicate success or failure. */
		TK_Status SetEdgePatterns( char const *patterns=0 ) alter;
		/*!Allocates the edge pattern strings, and initializes to empty.
			Allocates length of mp_edgecount, regardless of the number of values that are intended to be specified.
		   \return TK_Normal or TK_Error to indicate success or failure. */
		TK_Status SetEdgePatternStrings (void) alter;
		/*!Allocates the edge weights, and fills with values and sets the flags in mp_edge_exists.
			Allocates length of mp_edgecount, regardless of the number of values that are intended to be specified.
		   \param weights can be null, in which case weights are left untouched (assumed to be specified at a later
			point).  If non-null, it should be of length mp_edgecount.  
		   \return TK_Normal or TK_Error to indicate success or failure. */
		TK_Status SetEdgeWeights( float const *weights=0 ) alter;

		/*!Allocates the array of integers on which bits indicate the presence of attributes on vertices.
			Allocates length of mp_pointcount, regardless of the number of values that are intended to be specified.
		   \param vals can be null, in which case colors are left untouched.  If non-null, it should be of length mp_pointcount.  
		   \return TK_Normal or TK_Error to indicate success or failure. */
		TK_Status SetExists( unsigned int const *vals=0 ) alter;
		/*!Allocates the array of integers on which bits indicate the presence of attributes on faces.
			Allocates length of mp_facecount, regardless of the number of values that are intended to be specified.
		   \param vals can be null, in which case colors are left untouched.  If non-null, it should be of length mp_facecount.  
		   \return TK_Normal or TK_Error to indicate success or failure. */
		TK_Status SetFaceExists( unsigned int const *vals=0 ) alter;
		/*!Allocates the array of integers on which bits indicate the presence of attributes on edges.
			Allocates length of mp_edgecount, regardless of the number of values that are intended to be specified.
			Note that if the value for mp_edgecount is unknown (left at its initial value of -1), it is calculated 
			during the process of EnumerateEdges, which is called from within this function.
		   \param vals can be null, in which case colors are left untouched.  If non-null, it should be of length mp_edgecount.  
		   \return TK_Normal or TK_Error to indicate success or failure. */
		TK_Status SetEdgeExists( unsigned int const *vals=0 ) alter;

		/*!Adds an object to the set of items to be sent as part of the collection.
			Caller retains ownership of the object, and is responsible for deleting it
			during the write phase.  This function automatically sets the TKSH2_COLLECTION
			bit, converting this shell to a collection 
		   \param tk the toolkit \param obj the object to be appended */
		TK_Status AppendObject( BStreamFileToolkit & tk, BBaseOpcodeHandler *obj );
		/*!Pops an object off the list of collection parts.  
		   \return the object to caller (presumably so that it can be deleted) */
		BBaseOpcodeHandler *PopObject();

		/*! \return true if and only if there are some attributes set on any of the faces or vertices */
		bool HasOptionals() { return
			HasVertexAttributes() ||
			HasFaceAttributes() ||
			HasEdgeAttributes() ||
			!(mp_subop2 && 0x011) && mp_num_collection_parts > 0;// 0x01 matches TKSH2_COLLECTION
		};
		/*! \return true if and only if at least one vertex normal has been set */
		bool HasVertexNormals() const { return mp_normals != 0; };
		/*! \return true if and only if at least one vertex parameter has been set */
		bool HasVertexParameters() const { return mp_params != 0; };
		/*! \return true if and only if at least one vertex color has been set (to apply to an interpolated face) */
		bool HasVertexFaceColors() const { return mp_vfcolors != 0; };
		/*! \return true if and only if at least one vertex color has been set (to apply to an interpolated edge) */
		bool HasVertexEdgeColors() const { return mp_vecolors != 0; };
		/*! \return true if and only if at least one vertex color has been set (to apply to a marker) */
		bool HasVertexMarkerColors() const { return mp_vmcolors != 0; };
		/*! \return true if and only if at least one vertex color has been set (to apply to anything) */
		bool HasVertexColors() const { return 
				mp_vfcolors != 0 || 
				mp_vecolors != 0 ||
				mp_vmcolors != 0;
		};
		/*! \return true if and only if at least one vertex color-by-index has been set (to apply to an interpolated face) */
		inline bool HasVertexFaceIndices() const { return mp_vfindices != 0; };
		/*! \return true if and only if at least one vertex color-by-index has been set (to apply to an interpolated edge) */
		inline bool HasVertexEdgeIndices() const { return mp_veindices != 0; };
		/*! \return true if and only if at least one vertex color-by-index has been set (to apply to a marker) */
		inline bool HasVertexMarkerIndices() const { return mp_vmindices != 0; };
		/*! \return true if and only if at least one vertex color-by-index has been set (to apply to anything) */
		inline bool HasVertexIndices() const { return 
				mp_vfindices != 0 || 
				mp_veindices != 0 ||
				mp_vmindices != 0;
		};
		/*! \return true if and only if at least one explicit vertex marker visibility has been set */
		inline bool HasVertexMarkerVisibilities() const { return mp_vmvisibilities != 0; };
		/*! \return true if and only if at least one explicit vertex marker symbol has been set */
		inline bool HasVertexMarkerSymbols() const { return mp_vmsymbols != 0; };
		/*! \return true if and only if at least one explicit vertex marker size has been set */
		inline bool HasVertexMarkerSizes() const { return mp_vmsizes != 0; };
		/*! \return true if 
					HasVertexColors() or HasVertexIndices() or HasVertexNormals() or HasVertexParameters() or 
					HasVertexMarkerVisibilities() or HasVertexMarkerSymbols() or HasVertexMarkerSizes() */
		inline bool HasVertexAttributes() const { return
			HasVertexColors() ||
			HasVertexIndices() ||
			HasVertexNormals() ||
			HasVertexParameters() ||
			HasVertexMarkerVisibilities() ||
			HasVertexMarkerSymbols() ||
			HasVertexMarkerSizes();
		};
		/*! \return true if there are face colors set on the current polyhedron */
		inline bool HasFaceColors() const { return mp_fcolors != 0; };
		/*! \return true if and only if at least one face color-by-index has been set */
		inline bool HasFaceIndices() const { return mp_findices != 0; };
		/*! \return true if and only if at least one explicit face normal has been set */
		inline bool HasFaceNormals() const { return mp_fnormals != 0; };
		/*! \return true if and only if at least one explicit face visibility has been set */
		inline bool HasFaceVisibilities() const { return mp_fvisibilities != 0; };
		/*! \return true if and only if at least one face pattern has been set */
		inline bool HasFacePatterns() const { return mp_fpatterns != 0; };
		/*! \return true if and only if at least one face region has been set */
		inline bool HasFaceRegions() const { return mp_fregions != 0; };
		/*! \return true if HasFaceColors(), HasFaceIndices(), HasFaceRegions(), HasFaceVisibilities(), 
				HasFacePatterns(), or HasFaceRegions() */
		inline bool HasFaceAttributes() const { return 
			HasFaceColors() || 
			HasFaceIndices() || 
			HasFaceNormals() ||
			HasFaceVisibilities() ||
			HasFacePatterns() ||
			HasFaceRegions();
		};
        
		/*! \return true if there are edge colors set on the polyhedron */
		inline bool HasEdgeColors() const { return mp_ecolors != 0; }; 
		/*! \return true if colors-by-index are set on the polyhedron edges. */
		inline bool HasEdgeIndices() const { return mp_eindices != 0; }; 
		/*! \return true if explicit normals are set on the polyhedron edges */
		inline bool HasEdgeNormals() const { return mp_enormals != 0; };
		/*! \return true if explicit edge visibilities are set on the polyhedron edges */
		inline bool HasEdgeVisibilities() const { return mp_evisibilities != 0; }; 
		/*! \return true if edge patterns are set on the polyhedron edges */
		inline bool HasEdgePatterns() const { return mp_epatterns != 0; };
		/*! \return true if explicit weights are set on the polyhedron edges */
		inline bool HasEdgeWeights() const { return mp_eweights != 0; };
		/*! \returns true if any edge attributes are set: colors, indices, normals, visibilities, patterns, or weights */
		inline bool HasEdgeAttributes() const { return 
			HasEdgeColors() || 
			HasEdgeIndices() || 
			HasEdgeNormals() ||
			HasEdgeVisibilities() ||
			HasEdgePatterns() ||
			HasEdgeWeights();
		};

	protected:
		//data members prefixed with "mp_", not "m_", to avoid potential naming conflicts with TK_Shell and/or TK_Mesh

#ifndef DOXYGEN_SHOULD_SKIP_THIS

		float *			mp_points;       /*!< Array of floats which denote the point coordinates. */
		float *			mp_normals;     /*!< Vertex normals array */
		float *			mp_params;      /*!< Parameter array */
		float *			mp_vfcolors;    /*!< Vertex face colors array */
		float *			mp_vecolors;    /*!< Vertex edge colors array */
		float *			mp_vmcolors;    /*!< Vertex marker colors array */
		float *			mp_vfindices;   /*!< Vertex face colors-by-index array */
		float *			mp_veindices;   /*!< Vertex edge colors-by-index array */
		float *			mp_vmindices;   /*!< Vertex marker colors-by-index array */
		char *			mp_vmvisibilities;/*!< Vertex marker visibilities array */
		float *			mp_vmsizes;     /*!< Vertex marker sizes array */
		unsigned char * mp_vmsymbols;   /*!< Vertex marker symbols array */
		char **			mp_vmsymbolstrings;   /*!< Vertex marker symbols strings array */
		float *			mp_fcolors;      /*!< Face colors array */
		float *			mp_findices;    /*!< Face colors-by-index array */
		int *			mp_fregions;    /*!< Face region array */
		float *			mp_fnormals;    /*!< Face normals array */
		char *			mp_fvisibilities;/*!< Face visibilities array */
		char *			mp_fpatterns;   /*!< Face patterns array */
		float *			mp_ecolors;      /*!< Edge colors array */
		float *			mp_eindices;    /*!< Edge colors-by-index array */
		float *			mp_enormals;    /*!< Edge normals array */
		char *			mp_evisibilities;/*!< Edge visibilities array */
		unsigned char *	mp_epatterns;   /*!< Edge patterns array */
		char **			mp_epatternstrings;   /*!< Edge pattern strings array */
		float *			mp_eweights;    /*!< Edge weights array */

		unsigned int *	mp_exists;      /*!< Indicates existence of particular attributes per vertex */
		unsigned int *	mp_face_exists; /*!< Indicates existence of particular attributes per face */
		unsigned int *	mp_edge_exists; /*!< Indicates existence of particular attributes per edge */

		int				mp_pointcount;   /*!< The number of points.  Used for memory allocation */
		int				mp_normalcount; /*!< Number of explicitly set normals */
		int				mp_paramcount;  /*!< Number of explicitly set vertex parameters */
		char			mp_paramwidth;  /*!< Number of floating point numbers set on each vertex for the parameters */
		int				mp_vfcolorcount;/*!< Number of explicitly set vertex face colors */
		int				mp_vecolorcount;/*!< Number of explicitly set vertex edge colors */
		int				mp_vmcolorcount;/*!< Number of explicitly set vertex marker colors */
		int				mp_vfindexcount;/*!< Number of explicitly set vertex face color indices */
		int				mp_veindexcount;/*!< Number of explicitly set vertex edge color indices */
		int				mp_vmindexcount;/*!< Number of explicitly set vertex marker color indices */
		int				mp_vmvisibilitycount;/*!< Number of explicitly set vertex marker visibilities */
		int				mp_vmsizecount; /*!< Number of explicitly set vertex marker sizes */
		int				mp_vmsymbolcount;/*!< Number of explicitly set vertex marker symbols */
		int				mp_facecount;    /*!< The number of faces.  Used for memory allocation, -1 if not known */
		int				mp_fcolorcount; /*!< Number of explicitly set face colors */
		int				mp_findexcount; /*!< Number of explicitly set face colors-by-index */
		int				mp_fnormalcount;/*!< Number of explicitly set face normals */
		int				mp_fpatterncount;/*!< Number of explicitly set face patterns */
		int				mp_fvisibilitycount;/*!< Number of explicitly set face visibilities*/
		int				mp_edgecount;    /*!< The number of faces.  Used for memory allocation, -1 if not known */
		int				mp_ecolorcount; /*!< Number of explicitly set edge colors */
		int				mp_eindexcount; /*!< Number of explicitly set edge colors-by-index */
		int				mp_enormalcount;/*!< Number of explicitly set edge normals */
		int				mp_epatterncount;/*!< Number of explicitly set edge patterns */
		int				mp_eweightcount;/*!< Number of explicitly set edge weights */
		int				mp_evisibilitycount;/*!< Number of explicitly set face visibilities*/
		int *			mp_edge_enumeration;/*!< see member function EnumerateEdges */

		int				mp_stage;       /*!< Tracks top-level progress during read/write in case we need to return TK_Pending and restart */
		int				mp_substage;    /*!< Tracks progress within helper functions during read/write in case we need to return TK_Pending and restart */
		int				mp_progress;    /*!< Tracks progress during the writing of a specific array of values */
		int				mp_subprogress; /*!< Tracks additional progress during the writing of a specific array of values */
		unsigned char	mp_optopcode;   /*!< The sub-opcode of the optional attribute currently being written.  Value should 
											 be one of the Opt_Opcode values (e.g. OPT_NORMALS or OPT_ALL_FACE_COLORS). */

		unsigned short	mp_present;     /*!< Indicates which attributes are mentioned on any vertex */
		unsigned short	mp_face_present;/*!< Indicates which attributes are mentioned on any face */
		unsigned short	mp_edge_present;/*!< Indicates which attributes are mentioned on any edge */
		unsigned char	mp_bits_per_sample; /*!< The number of bits to use for each sample. */
		float			mp_bbox[256];   /*!< The bounding box of the object. Used for quantization. allocated super-size for parameters with mp_paramwidth > 3 */
		ID_Key			mp_key;         /*!< The hoops key of the object */
		TK_Instance *	mp_instance;    /*!< Instance to be generated instead of full object, if available */
		float			mp_matrix[16];  /*!< Current transform for instance comparison/generation */
		unsigned char *	mp_workspace;   /*!< Scratch space */  
		int				mp_workspace_allocated;/*!< The amount of scratch space allocated */    
		int				mp_workspace_used;/*!< The amount of scratch space used */    
		unsigned char	mp_compression_scheme;/*!< ID for the compression method of the currently reading/writing member */
		unsigned char	mp_subop;       /*!< some additional overall info on the phon */
		unsigned short	mp_subop2;      /*!< some more additional overall info on the phon */
		int				mp_num_collection_parts;/*!< if a lod contains non-polyhedron geometry, it is a collection, and this variable indicates how many objects it contains */
		BBaseOpcodeHandler **mp_collection_parts;/*!< if a lod contains non-polyhedron geometry, it is a collection, and this variable contains an array of BOpcodeHandler objects */

		/*! Each edge can be identified by a linear set of identifiers [0..edgecount], or by its 
			starting and ending vertex indices.  For any given edge id, 
			mp_edge_enumeration[id*2], mp_edge_enumeration[id*2 + 1] will contain the start and
			end vertices.  See member variable mp_edge_enumeration ().  Any subclass of TK_Polyhedron
			that can potentially have edge attributes should overload this function. */
		virtual	TK_Status EnumerateEdges() { return TK_Error; }; 
		/*! If any edges are due to be created due to triangulation (and it contained faces >3 in length), the Interpret()
			phase may need to know which edges aren't really pre-existing, so that it can avoid querying the attributes.
			This function sets the Fake_Edge bit in mp_edge_exists for that purpose.  By default, it does nothing.  At
			the time of this writing, it had been overloaded by HOpcodeShell only */
		virtual	TK_Status LabelFakeEdges() { return TK_Normal; }; 
		/*! Given the attributes of the local polyhedorn and the settings in the toolkit, decide whether or not 
			to allow this polyhedron to look for instances (previously seen identical objects from which to make
			a copy), or to register itself on the list of instances to be matched.  
			\returns true if instancing should be allowed */
		virtual		bool AllowInstancing(BStreamFileToolkit & tk) const;

		TK_Polyhedron(unsigned char op);
		~TK_Polyhedron();

		TK_Status	Read (BStreamFileToolkit & tk) alter;
		TK_Status	Write (BStreamFileToolkit & tk) alter;
		void		Reset (void) alter;

		virtual		TK_Status	read_trivial_points (BStreamFileToolkit & tk) alter; /*!< internal use only */
		virtual		TK_Status	write_trivial_points (BStreamFileToolkit & tk) alter; /*!< internal use only */

		TK_Status	read_vertex_normals (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	read_vertex_normals_compressed (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	read_vertex_normals_all (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	read_vertex_normals_compressed_all (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	read_vertex_parameters (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	read_vertex_parameters_all (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	read_vertex_colors_all (BStreamFileToolkit & tk, unsigned char op ) alter; /*!< internal use only */
		TK_Status	read_vertex_colors (BStreamFileToolkit & tk, unsigned char op ) alter; /*!< internal use only */
		TK_Status	read_vertex_indices_all (BStreamFileToolkit & tk, unsigned char op ) alter; /*!< internal use only */
		TK_Status	read_vertex_indices (BStreamFileToolkit & tk, unsigned char op ) alter; /*!< internal use only */
		TK_Status	read_vertex_marker_visibilities (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	read_vertex_marker_symbols (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	read_vertex_marker_sizes (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	read_face_colors (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	read_face_colors_all (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	read_face_indices (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	read_face_indices_all (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	read_face_regions (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	read_face_normals (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	read_face_visibilities (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	read_face_patterns (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	read_edge_colors (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	read_edge_indices (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	read_edge_normals (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	read_edge_visibilities (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	read_edge_patterns (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	read_edge_weights (BStreamFileToolkit & tk) alter; /*!< internal use only */
		virtual		TK_Status read_collection (BStreamFileToolkit & tk) alter; /*!< internal use only */

		TK_Status	write_vertex_normals (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	write_vertex_normals_compressed (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	write_vertex_normals_all (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	write_vertex_normals_compressed_all (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	write_vertex_parameters (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	write_vertex_parameters_all (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	write_vertex_colors (BStreamFileToolkit & tk, unsigned char op ) alter; /*!< internal use only */
		TK_Status	write_vertex_colors_all (BStreamFileToolkit & tk, unsigned char op ) alter; /*!< internal use only */
		TK_Status	write_vertex_indices (BStreamFileToolkit & tk, unsigned char op ) alter;  /*!< internal use only */
		TK_Status	write_vertex_indices_all (BStreamFileToolkit & tk, unsigned char op ) alter;  /*!< internal use only */
		TK_Status	write_vertex_marker_visibilities (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	write_vertex_marker_symbols (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	write_vertex_marker_sizes (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	write_face_colors (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	write_face_colors_all (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	write_face_indices (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	write_face_indices_all (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	write_face_regions (BStreamFileToolkit & tk) alter;  /*!< internal use only */
		TK_Status	write_face_normals (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	write_face_visibilities (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	write_face_patterns (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	write_edge_colors (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	write_edge_indices (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	write_edge_normals (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	write_edge_visibilities (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	write_edge_patterns (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	write_edge_weights (BStreamFileToolkit & tk) alter; /*!< internal use only */
		virtual		TK_Status write_collection (BStreamFileToolkit & tk, bool write_tag = false, int variant = 0) alter; /*!< internal use only */

		/*! converts from array of cartesian [x,y,z] to [theta,phi].  Normals must be pre-normalized to length 1, Passing the same pointer for in and out is ok. */
		void normals_cartesian_to_polar( unsigned int *exists, int flag, int count, const float *in, float *out );  
		/*! converts from array of [theta,phi] to cartesian [x,y,z].  Passing the same pointer for in and out is ok.*/
		void normals_polar_to_cartesian( unsigned int *exists, int flag, int count, const float *in, float *out );  

		/*! internal use only --
			write the vertex normals as currently described in mp_normals, mp_exists, and mp_normalcount.  
			Dispatches to one of the helper functions.  \param tk the toolkit \returns success or failure */
		TK_Status	write_vertex_normals_main (BStreamFileToolkit & tk) alter; 
		/*! internal use only --
			write the vertex parameters (a.k.a texture coordinates) as currently described in mp_params, 
			mp_exists, and mp_paramcount. Called by TK_Polyhedron::write().  Dispatches to one of the 
			helper functions.  \param tk the toolkit \returns success or failure */
		TK_Status	write_vertex_parameters_main (BStreamFileToolkit & tk) alter; 
		/*! internal use only --
			write the vertex colors (by value) as currently described in mp_exists, mp_vfcolors, mp_vecolors,
			mp_vmcolors, mp_vecolorcount, mp_vmcolorcount and mp_vfcolorcount.  Called by TK_Polyhedron::write().
			Dispatches to one of the helper functions.  \param tk the toolkit \param op determines which of
			[face, edge, marker] vertex colors we are currently writing \returns success or failure */
		TK_Status	write_vertex_colors_main (BStreamFileToolkit & tk, unsigned char base_op ) alter;
		/*! internal use only --
			write the vertex colors (by index) as currently described in mp_exists, mp_vfindices, mp_veindices,
			mp_vmindices, mp_veindexcount, mp_vmindexcount and mp_vfindexcount.  Called by TK_Polyhedron::write().
			Dispatches to one of the helper functions.  \param tk the toolkit \param op determines which of
			[face, edge, marker] vertex colors we are currently writing \returns success or failure */
		TK_Status	write_vertex_indices_main (BStreamFileToolkit & tk, unsigned char base_op ) alter; 
		/*! internal use only --
			write the face colors (by value) as currently described in mp_exists, mp_fcolors, mp_fcolorcount.  
			Called by TK_Polyhedron::write().  Dispatches to one of the helper functions.  
			\param tk the toolkit \returns success or failure */
		TK_Status	write_face_colors_main (BStreamFileToolkit & tk) alter; 
		/*! internal use only --
			write the face colors (by index) as currently described in mp_exists, mp_findices, mp_findexcount.  
			Called by TK_Polyhedron::write().  Dispatches to one of the helper functions.  
			\param tk the toolkit \returns success or failure */
		TK_Status	write_face_indices_main (BStreamFileToolkit & tk) alter;


		TK_Status	ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status	WriteAscii (BStreamFileToolkit & tk) alter;

		virtual		TK_Status	read_trivial_points_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */
		virtual		TK_Status	write_trivial_points_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */

		TK_Status	read_vertex_normals_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */

		TK_Status	read_vertex_normals_compressed_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	read_vertex_normals_all_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	read_vertex_normals_compressed_all_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	read_vertex_parameters_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	read_vertex_parameters_all_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	read_vertex_colors_all_ascii (BStreamFileToolkit & tk, unsigned char op ) alter; /*!< internal use only */
		TK_Status	read_vertex_colors_ascii (BStreamFileToolkit & tk, unsigned char op ) alter; /*!< internal use only */
		TK_Status	read_vertex_indices_all_ascii (BStreamFileToolkit & tk, unsigned char op ) alter; /*!< internal use only */
		TK_Status	read_vertex_indices_ascii (BStreamFileToolkit & tk, unsigned char op ) alter; /*!< internal use only */
		TK_Status	read_vertex_marker_visibilities_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	read_vertex_marker_symbols_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	read_vertex_marker_sizes_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	read_face_colors_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	read_face_colors_all_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	read_face_indices_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	read_face_indices_all_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	read_face_regions_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	read_face_normals_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	read_face_visibilities_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	read_face_patterns_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	read_edge_colors_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	read_edge_indices_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	read_edge_normals_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	read_edge_visibilities_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	read_edge_patterns_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	read_edge_weights_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */

		virtual		TK_Status read_collection_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */

		TK_Status	write_vertex_normals_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	write_vertex_normals_compressed_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	write_vertex_normals_all_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	write_vertex_normals_compressed_all_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	write_vertex_parameters_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	write_vertex_parameters_all_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	write_vertex_colors_ascii (BStreamFileToolkit & tk, unsigned char op ) alter; /*!< internal use only */
		TK_Status	write_vertex_colors_all_ascii (BStreamFileToolkit & tk, unsigned char op ) alter; /*!< internal use only */
		TK_Status	write_vertex_indices_ascii (BStreamFileToolkit & tk, unsigned char op ) alter;  /*!< internal use only */
		TK_Status	write_vertex_indices_all_ascii (BStreamFileToolkit & tk, unsigned char op ) alter;  /*!< internal use only */
		TK_Status	write_vertex_marker_visibilities_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	write_vertex_marker_symbols_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */
		virtual		TK_Status	write_vertex_marker_sizes_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	write_face_colors_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	write_face_colors_all_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	write_face_indices_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	write_face_indices_all_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	write_face_regions_ascii (BStreamFileToolkit & tk) alter;  /*!< internal use only */
		TK_Status	write_face_normals_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	write_face_visibilities_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	write_face_patterns_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	write_edge_colors_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	write_edge_indices_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	write_edge_normals_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	write_edge_visibilities_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	write_edge_patterns_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */
		TK_Status	write_edge_weights_ascii (BStreamFileToolkit & tk) alter; /*!< internal use only */

		virtual		TK_Status write_collection_ascii (BStreamFileToolkit & tk, bool write_tag = false, int variant = 0) alter; /*!< internal use only */


		/*! internal use only --
			write the vertex normals as currently described in mp_normals, mp_exists, and mp_normalcount.  
			Dispatches to one of the helper functions.  \param tk the toolkit \returns success or failure */
		TK_Status	write_vertex_normals_main_ascii (BStreamFileToolkit & tk) alter; 
		/*! internal use only --
			write the vertex parameters (a.k.a texture coordinates) as currently described in mp_params, 
			mp_exists, and mp_paramcount. Called by TK_Polyhedron::write().  Dispatches to one of the 
			helper functions.  \param tk the toolkit \returns success or failure */
		TK_Status	write_vertex_parameters_main_ascii (BStreamFileToolkit & tk) alter; 
		/*! internal use only --
			write the vertex colors (by value) as currently described in mp_exists, mp_vfcolors, mp_vecolors,
			mp_vmcolors, mp_vecolorcount, mp_vmcolorcount and mp_vfcolorcount.  Called by TK_Polyhedron::write().
			Dispatches to one of the helper functions.  \param tk the toolkit \param op determines which of
			[face, edge, marker] vertex colors we are currently writing \returns success or failure */
		TK_Status	write_vertex_colors_main_ascii (BStreamFileToolkit & tk, unsigned char base_op ) alter;
		/*! internal use only --
			write the vertex colors (by index) as currently described in mp_exists, mp_vfindices, mp_veindices,
			mp_vmindices, mp_veindexcount, mp_vmindexcount and mp_vfindexcount.  Called by TK_Polyhedron::write().
			Dispatches to one of the helper functions.  \param tk the toolkit \param op determines which of
			[face, edge, marker] vertex colors we are currently writing \returns success or failure */
		TK_Status	write_vertex_indices_main_ascii (BStreamFileToolkit & tk, unsigned char base_op ) alter; 
		/*! internal use only --
			write the face colors (by value) as currently described in mp_exists, mp_fcolors, mp_fcolorcount.  
			Called by TK_Polyhedron::write().  Dispatches to one of the helper functions.  
			\param tk the toolkit \returns success or failure */
		TK_Status	write_face_colors_main_ascii (BStreamFileToolkit & tk) alter; 
		/*! internal use only --
			write the face colors (by index) as currently described in mp_exists, mp_findices, mp_findexcount.  
			Called by TK_Polyhedron::write().  Dispatches to one of the helper functions.  
			\param tk the toolkit \returns success or failure */
		TK_Status	write_face_indices_main_ascii (BStreamFileToolkit & tk) alter;


		virtual	TK_Status mangle_points (BStreamFileToolkit & tk, int pointmap_count, int *pointmap ) alter; /*!< internal use only */
		virtual	TK_Status mangle_point_attributes (BStreamFileToolkit & tk, int pointmap_count, int *pointmap ) alter; /*!< internal use only */
		virtual	TK_Status mangle_face_attributes (BStreamFileToolkit & tk, int facemap_count, int *facemap ) alter; /*!< internal use only */
		bool has_face_attributes() const; /*!< internal use only */

		/*! Quantizes the normal vectors to on of 2^bits vectors precalculated
			with a point repulsion simulation.  Compression and decompression are
			fast, but the required lookup tables add about 90k to the dll.  It
			should be disabled if dll size is critical.  Corresponds to the 
			compression scheme, CS_REPULSE.
			\param tk The toolkit, needed to report any error conditions.
			\param bits The number of bits to use for each sample.  Only 8 or 10 are currently supported.
			\return The result (success or failure) of the function call. */ 
		TK_Status repulse_compress_normals (BStreamFileToolkit & tk, int bits) alter;
		/*! Looks up a vector to use as the normal.  The last entry in the table is
			reserved to indicate "non-existent", in which case no normal will be set.
			\param tk The toolkit, needed to report any error conditions.
			\param bits The number of bits to use for each sample.  Only 8 or 10 are currently supported.
			\return The result (success or failure) of the function call. */ 
		TK_Status repulse_decompress_normals (BStreamFileToolkit & tk, int bits) alter;

		//!obsolete.  Provided for compatibility.
		void set_points (int count, float const * points = 0) alter { SetPoints( count, points ); };
		//!obsolete.  Provided for compatibility.
		void set_normals (float const * normals = 0) alter { SetVertexNormals( normals ); };
		//!obsolete.  Provided for compatibility.
		void set_parameters (float const * params = 0) alter { SetVertexParameters( params ); };
		//!obsolete.  Provided for compatibility.
		void set_vfcolors (float const * colors = 0) alter { SetVertexFaceColors( colors ); };
		//!obsolete.  Provided for compatibility.
		void set_vecolors (float const * colors = 0) alter { SetVertexEdgeColors( colors ); };
		//!obsolete.  Provided for compatibility.
		void set_vmcolors (float const * colors = 0) alter { SetVertexMarkerColors( colors ); };
		//!obsolete.  Provided for compatibility.
		void set_vfindices (float const * indices = 0) alter { SetVertexFaceIndices( indices ); };
		//!obsolete.  Provided for compatibility.
		void set_veindices (float const * indices = 0) alter { SetVertexEdgeIndices( indices ); };
		//!obsolete.  Provided for compatibility.
		void set_vmindices (float const * indices = 0) alter { SetVertexMarkerIndices( indices ); };
		//!obsolete.  Provided for compatibility.
		void set_fcolors (float const * colors = 0) alter { SetFaceColors( colors ); };
		//!obsolete.  Provided for compatibility.
		void set_findices (float const * indices = 0) alter { SetFaceIndices( indices ); };
		//!obsolete.  Provided for compatibility.
		void set_exists (unsigned int const * exists = 0) alter;
		//!obsolete.  Provided for compatibility.
		void set_face_exists (unsigned int const * exists = 0) alter;
		//!obsolete.  Provided for compatibility.
		void set_facemap (int count, int const * facemap = 0) alter;

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

}; // end declaration of class TK_Polyhedron


//! Handles the TKE_Mesh opcode.
/*! 
	TK_Mesh provides support for writing/reading the TKE_Mesh opcode object to/from an HSF file. 

	The HOOPS/3dGS scene-graph supports 'mesh' primitives which consist of a number of rows, columns
		and a pionts array to define a 3d mesh with fixed topology.  (Picture a distored checkerboard.)
*/
class BBINFILETK_API TK_Mesh : public TK_Polyhedron {
    private:
        TK_Mesh(const TK_Mesh&);
        TK_Mesh& operator=(const TK_Mesh&);


	protected:
		int             m_rows;     /*!< Number of rows in this Mesh object  */
		int             m_columns;  /*!< Number of columns in this Mesh object  */
		/*! overloads from TK_Polyhedron.  Creates the mp_edge_enumeration array that lists all 
			of the pairs of vertices that are connected as edges. */
		TK_Status	    EnumerateEdges(); 

	public:
		TK_Mesh()
			: TK_Polyhedron (TKE_Mesh), m_rows(0), m_columns(0) {}



		TK_Status	ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status	WriteAscii (BStreamFileToolkit & tk) alter;

		TK_Status	Read (BStreamFileToolkit & tk) alter;
		TK_Status	Write (BStreamFileToolkit & tk) alter;
		void        Reset (void) alter;

		/*! Sets number of rows for the mesh.*/
		void  SetRows (const int rows) alter { m_rows = rows; }

		/*! Sets number of columns for the mesh.*/
		void  SetColumns (const int columns) alter { m_columns = columns; }

}; // end declaration of class TK_Mesh

#endif //BBINFILETK_POLYHEDRON


