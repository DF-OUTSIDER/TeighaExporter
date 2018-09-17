/*
 * Copyright (c) by Tech Soft 3D, LLC.
 * The information contained herein is confidential and proprietary to
 * Tech Soft 3D, LLC., and considered a trade secret as defined under
 * civil and criminal statutes.  Tech Soft 3D shall pursue its civil
 * and criminal remedies in the event of unauthorized use or misappropriation
 * of its trade secrets.  Use of this information by anyone other than
 * authorized employees of Tech Soft 3D, LLC. is granted only under a
 * written non-disclosure agreement, expressly prescribing the scope and
 * manner of such use.
 *
 * $Header: //DWF/Working_Area/Willie.Zhu/w3dtk/BEdgeBreaker.h#1 $
 *
 *
 *
 * HEdgeBreaker.h
 *
 * This file describes the public interface to the edgebreaker module, plus some internal 
 * definitions that are shared between one or more edgebreaker source code files
 * 
 * Some notes about the interface:
 * in_out
 *   Parameters with names ending in "in_out" are used for both input and output.
 *   On the way in they say how much was allocated.  On the way out, they say
 *   how much was used.
 * faces
 *   The following is a description of the HOOPS face list (taken from the
 *   standard HOOPS reference manual)...
 *   "The face_list is an array of integers. The first integer is the
 *    number of vertices that should be connected to form the first face. For 
 *    example, "3" for a triangle. The next three integers (in this example) 
 *    are the offsets into the points array at which the three x-y-z's can 
 *    be found. The first point in the points array is considered to be at 
 *    offset zero, so "0, 1, 2" in the face_list array would form the triangle 
 *    from the first three entries in points."
 *   a face list by tristrips is (again from the HOOPS 3dgs reference manual)
 *   "an array of integers that describes the triangle strips. It is a list of an 
 *    unlimited number of independent triangle strips placed contiguously in memory, 
 *    one after another. Each triangle strip begins with the number of vertices 
 *    referenced, and then the list of vertices. Vertices 0-2 form one triangle. 
 *    For every n such that 2 < n <= number of vertices, a new triangle is introduced. 
 *    This new triangle will be [n-2, n-1, n] for even n, and [n-1, n-2, n] for odd n 
 *    (to maintain consistent handedness of the triangles). Degenerate triangles 
 *    (triangles which reference the same vertex more than once) are allowed. The 
 *    length of this array should be given as a number of integers, the number of 
 *    triangles plus 3 per independent triangle strip. "
 * pointmap
 *   Vertex id's, sorted by order of appearance in the compressed stream.  This
 *   array is needed for transmitting vertex attributes (e.g. normals or texture
 *   coordinates) in parallel with their locations.  For example:
 *   for( i = 0 ; i < pointmap_len ; i++ ) {
 *      *out++ = texcoords[ pointmap[i] ].u;
 *	*out++ = texcoords[ pointmap[i] ].v;
 *   }
 * pcount
 *   the **number** of xyz triplets in the array of vertex locations.
 * flen
 *   the number of integers in the face list
 */
#ifndef __BEDGEBREAKER_H__
#define __BEDGEBREAKER_H__

///
///\file        dwf/w3dtk/BEdgeBreaker.h
///

#ifndef BSTREAM_DISABLE_EDGEBREAKER


#define EB_DEFAULT (-1)

typedef struct ET_Bounding_TAG {
    float x1, y1, z1;
    float x2, y2, z2;
} ET_Bounding;

typedef void *(*ET_Malloc_Action)(size_t size, void *user_data);
typedef void(*ET_Free_Action)(void *ptr, void *user_data);
typedef void(*ET_New_Vertex_Action)(int a, int b, int c, void *user_data);

#define HINT_ONE_PIECE		    0x0001
#define HINT_WATERTIGHT		    0x0002
#define HINT_MANIFOLD		    0x0004
#define HINT_NO_HANDLES		    0x0008
#define HINT_NO_BACKWARDS_OR_HOLES  0x0010
#define HINT_INPUT_BY_TRISTRIPS	    0x0020

#define STATUS_ERROR		0
#define STATUS_NORMAL		1
#define STATUS_COMPLETE		2
#define STATUS_WATERTIGHT	3
#define STATUS_TRY_AGAIN	4



typedef struct eb_compress_configs_TAG {
    
    int x_quantization, y_quantization, z_quantization;
    
    int x_quantization_normals, y_quantization_normals, z_quantization_normals;
    
    float point_factor;		
    
    ET_Bounding *bounding;
    int hints;
    
    ET_Malloc_Action malloc_action;
    ET_Free_Action free_action;
    ET_New_Vertex_Action new_vertex_action;
    void *user_data;
    
    int target_version;
} eb_compress_configs;
extern int show_edgebreaker_compress_size( 
			int pcount, 
			int flen, 
			int const *face_data, 
			int *stream_len_out,
			int *pointmap_count_out,
			
			eb_compress_configs const *configs
			);
extern int edgebreaker_compress( 
			int pcount, 
			float const *points, 
			float const *normals, 
			int flen, 
			
			int const *fdata_in,		
			int *stream_len_in_out,
			
			void *stream_out,		
			int *pointmap_len_in_out,
			
			int *pointmap_out,		
			
			eb_compress_configs const *configs	
			);

typedef struct eb_decompress_configs_TAG {
    			
    ET_Bounding const *bounding;
    
    ET_Malloc_Action malloc_action;
    ET_Free_Action free_action;
    ET_New_Vertex_Action new_vertex_action;
    void *user_data;
} eb_decompress_configs;
extern int show_edgebreaker_decompress_size( 
			int stream_len, 
			void const *stream, 
			int *pcount_out, 
			int *normal_count_out, 
			int *flen_out 
			);
extern int edgebreaker_decompress( 
			int stream_len, 
			
			void const *stream,		
			int *pcount_in_out, 
			float *points_out, 
			float *normals_out, 
                        
                        bool *by_tristrips_out,
                        
			int *flen_in_out,
			
			int *faces_out,
			
			eb_decompress_configs const *configs		
			);







    


typedef struct {
    char scheme;    
    char mtable_scheme;
    char points_scheme;
    char normals_scheme; 
    int opslen;	    
    int mtablelen;  
    int pointslen;  
    int pcount;	    
} edgebreaker_header0;


typedef struct {
    edgebreaker_header0 v0;
    int normalslen;
} edgebreaker_header1;


typedef struct EDGEBREAKER_HEADER_TAG {
    char scheme;    
    char mtable_scheme;
    char points_scheme;
    char normals_scheme; 
    int opslen;	    
    int mtablelen;  
    int pointslen;  
    int pcount;	    
    int normalslen;
} edgebreaker_header;


#define MTABLE_HAS_LENGTHS	            0x1
#define MTABLE_HAS_M2STACKOFFSETS           0x2
#define MTABLE_HAS_M2GATEOFFSETS            0x4
#define MTABLE_HAS_DUMMIES	            0x8
#define MTABLE_HAS_PATCHES	            0x10
#define MTABLE_HAS_BOUNDING	            0x20
#define MTABLE_HAS_QUANTIZATION	            0x40
#define MTABLE_HAS_QUANTIZATION_NORMALS     0x80

typedef struct {
    int flags;
    
    int *mlengths;
    int mlengths_used;
    int mlengths_allocated;
    
    int *m2stackoffsets;
    int m2stackoffsets_used;
    int m2stackoffsets_allocated;
    
    int *m2gateoffsets;
    int m2gateoffsets_used;
    int m2gateoffsets_allocated;
    
    int *dummies;
    int dummies_used;
    int dummies_allocated;
    
    int *patches;
    int patches_used;
    int patches_allocated;
    
    ET_Bounding *bounding;
    
    int x_quantization;
    int y_quantization;
    int z_quantization;
    
    int x_quantization_normals;
    int y_quantization_normals;
    int z_quantization_normals;
} mtable_info;



typedef struct HALF_EDGE_TAG {
    int start;
    int twin;
} half_edge;


typedef struct {
    half_edge *edges;
    int allocated;
    int used;
    int *visitations;  
    
    int visitations_used;
} half_edge_array;

typedef struct {
    int *data;
    int allocated;
    int used;
} int_stack;

typedef struct ACTION_TABLE_TAG {
    ET_Malloc_Action malloc_action;
    ET_Free_Action free_action;
    ET_New_Vertex_Action new_vertex_action;
    void *user_data;
} ET_Action_Table;
#define EA_FREE(ptr) (actions->free_action(ptr,actions->user_data))
#define EA_MALLOC(size) (actions->malloc_action(size,actions->user_data))

typedef struct {
    unsigned int *data; 
    unsigned int *rdata;
    int allocated;	
    int used;		
    int bit;		
    int rused;		
    int rbit;		
    int can_reallocate; 
    int status;		
    
    unsigned int mask[33]; 
    
    unsigned int range[33]; 
} varstream;


typedef struct {
    int *loops;          
    int *loops_edges;    
    int loops_used;      
    int loops_edges_allocated; 

    int np_allocated;
    int *P;              
    int *N;              
} loop_table;



typedef short INT16;
typedef int INT32;

    
#define CASE_C 0
#define CASE_L 1
#define CASE_E 2
#define CASE_R 3
#define CASE_S 4
#define CASE_M 5
#define CASE_M2 6

#define TONEXT4(i) (3 - (((i)-1)%4))

#define MAXVAL(bits) ((1<<(bits))-1)

#define BIG_FLOAT (1e20f)
#define DEFAULT_QUANTIZATION (11)
#define DEFAULT_POINT_FACTOR (1.5f)
#define DEFAULT_HINTS (0)

#define POINTSIZE 12 

#define GARBAGE_VERTEX	((int)0x80808080)
#define DUMMY_VERTEX    ((int)0x80000003)
#define GARBAGE ((int)0x80808080)           
#define VERTEX_SPECIAL(x) ((unsigned int)(x) & 0x80000000)
#define INVALIDATE_VERTEX(x) ((x) |= 0x80000000) 
#define EA_VERTEX_INDEX(x) ((x) & ~0x80000000)
#define PROXY_VERTEX_INDEX(proxy_hash,v) (VERTEX_SPECIAL(v)?lookup_vertex((proxy_hash),(v)):v)

#define GARBAGE_EDGE ((int)0x80808080)
#define MULTIPLE_EDGE ((int)0x80000001)
#define EDGE_SPECIAL(x) ((unsigned int)(x) & 0x80000000)
#define EDGE_INVALID(x) ((unsigned int)(x) & 0x80000000)
#define INVALIDATE_EDGE(x) ((x) |= 0x80000000) 
#define REAL_EDGE_INDEX(x) ((x) & ~0x80000000)


#define I2V(x) ((void *)(POINTER_SIZED_INT)(x))
#define V2I(x) ((int)(POINTER_SIZED_INT)(void *)(x))




#define HALF_EDGE_INIT(h) ((h)->start=(h)->twin=GARBAGE_EDGE)


#define HNEXT(i) (3*((i)/3) + ((i)+1)%3)
#define HPREV(i) (3*((i)/3) + ((i)+2)%3)


    
extern void mtable_info_init( mtable_info *m );
extern void mtable_info_free( mtable_info *m );
extern int lookup_vertex(const struct vhash_s *proxy_hash, int v);
extern void predict( 
    half_edge_array const *ea, 
    int ei,                     
    int third_vertex_unknown,   
    char const *touched,
    struct vhash_s const *proxy_hash,
    int const *quantized_points,
    int *prediction_out );
extern int old_predict( 
    int const *associations, 
    int const *points, 
    ET_Bounding const *bounding, 
    int x_quantization,
    int y_quantization,
    int z_quantization,
    int *out );
extern int old_pack_points( 
    mtable_info *mtable, int *associations, edgebreaker_header *hptr,
    int original_pointcount, int const *pointmap, float const *points, 
    int buffsize, void *buffer_out, 
    eb_compress_configs const *configs );
extern int old_unpack_points( 
    int const *associations, 
    edgebreaker_header const *hptr,
    void const *diffs_in, 
    float *points_out, 
    ET_Bounding const *bounding, 
    int x_quantization, int y_quantization, int z_quantization );
extern int old_unpack_normals( 
    int const *associations, 
    edgebreaker_header const *hptr,
    void const *diffs_in, 
    float *normals_out, 
    int x_quantization_normals, int y_quantization_normals, int z_quantization_normals );
extern int int_stack_init( int_stack *s );
extern int int_stack_expand( int_stack *s );
extern void int_stack_free( int_stack *s );
extern int int_stack_pop( int_stack *s );
extern int int_stack_pop_internal( int_stack *s, int offset, int *out );
extern void int_stack_push( int_stack *s, int n );
extern void int_stack_push_if_unique( int_stack *s, int n );
extern int validate_associations( 
	int const *associations, 
	int pointcount );
extern int half_edge_array_init( half_edge_array *ea, int initial_size );
extern void half_edge_array_free( half_edge_array *ea );
extern int half_edge_array_expand( half_edge_array *ea );
extern int half_edge_array_append( half_edge_array *ea, half_edge **out );
extern int validate_edge ( half_edge_array const *ea, half_edge const *a, loop_table const *loops );
extern void *default_malloc( size_t size, void *user_data );
extern void default_free( void *ptr, void *user_data );
extern void default_new_vertex( int a, int b, int c, void *user_data );
extern void vsinit_write( varstream *vs, int size, void *pointer );

extern void vsinit_read( varstream *vs, int size, void *pointer );
extern void vsfree( varstream *vs );
extern void vsswap( varstream *vs );
extern void vsput( varstream *vs, const int *numbits_array, int val );
extern int vsget( varstream *vs, const int *numbits_array );



extern ET_Action_Table *actions;
extern int g_hints;

#endif 
#endif 


