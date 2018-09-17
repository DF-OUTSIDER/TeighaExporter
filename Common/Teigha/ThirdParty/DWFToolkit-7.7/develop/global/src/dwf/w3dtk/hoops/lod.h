/*
 * Copyright (c) 1998 by Tech Soft America, LLC.
 * The information contained herein is confidential and proprietary to
 * Tech Soft America, LLC., and considered a trade secret as defined under
 * civil and criminal statutes.  Tech Soft America shall pursue its civil
 * and criminal remedies in the event of unauthorized use or misappropriation
 * of its trade secrets.  Use of this information by anyone other than
 * authorized employees of Tech Soft America, LLC. is granted only under a
 * written non-disclosure agreement, expressly prescribing the scope and
 * manner of such use.
 *
 * $Id: //DWF/Working_Area/Willie.Zhu/w3dtk/hoops/lod.h#1 $
 */

/*
 * This file consolidated from the separate files we had before.
 */


/************************************************************************

  MxMain.h

  This file contains the structures and function prototypes needed for 
  communication with the outside world.  In converting between HOOPS and
  non-hoops versions of the LOD module, this is the only file that should
  need changes (for non-HOOPS representations of 3d models, MxMain.c may
  also need some changes).

  Copyright (C) 1998 Michael Garland.  See "COPYING.txt" for details.
  
 ************************************************************************/


#ifndef MXMAIN_INCLUDED 
#define MXMAIN_INCLUDED

///
///\file        dwf/w3dtk/hoops/lod.h
///

#ifdef MX_DEBUG
#include <assert.h>
#define MX_ASSERT(x) assert(x)
#else
#define MX_ASSERT(x) do if (x) {} while (0)
#endif

#include <stdlib.h>
#include <string.h>

#include "hoops_tools.h"

/* some wrappers to use around memory allocation and freeing */
#define MX_ALLOC(size) malloc (size)
#define MX_FREE(ptr,size) free (ptr)
#define MX_COPY(src,size,dest) (memcpy ((dest), (src),(size)))
#define MX_REALLOC(ptr,oldsize,newsize)         \
{                                               \
    /* brute force. reallocate from scratch */  \
    void *tmp = (ptr);                          \
    ((void *)ptr) = MX_ALLOC((newsize));        \
    MX_COPY((ptr),  tmp, (oldsize));            \
    MX_FREE(tmp, (oldsize));                    \
}
#define ALLOC_ARRAY(p,c,t) p = (t *) MX_ALLOC (c * sizeof (t))

#define local static
#define null 0
#define until(x) while(!(x))
//#define POINTER_SIZED_INT   long    /* fix for 64 bit */
#ifdef _WIN64
#   define POINTER_SIZED_INT __int64
#else
#   define POINTER_SIZED_INT long
#endif

/*
 * LOD-specific error codes
 */
#define HEC_LOD_MODULE                  111 /* must match hpserror.h */
#define ERR_INSUFFICIENT_MEMORY           1

#define MX_ERROR(code,explanation)  /*  \
    (HI_Error( HEC_LOD_MODULE, code, explanation ) ) */


#define MX_USE_DEFAULT          -1

/* for placement policy */
#define MX_PLACE_ENDPOINTS  0
#define MX_PLACE_ENDORMID   1 
#define MX_PLACE_LINE       2
#define MX_PLACE_OPTIMAL    3
#define MX_PLACE_OPTIMAL_BB 4

/* for weighting policy */
#define MX_WEIGHT_UNIFORM       0
#define MX_WEIGHT_AREA          1
#define MX_WEIGHT_ANGLE         2
#define MX_WEIGHT_AVERAGE       3
#define MX_WEIGHT_AREA_UNSCALED 4
#define MX_WEIGHT_RAWNORMALS    5

/* for target units */
#define MX_PERCENT      1
#define MX_LEVEL        2

/* a "-1" in any of these fields indicates that the default should be used */
typedef struct MxConfig_TAG
{
    int     placement_policy;
    int     weighting_policy;
    float   boundary_weight;
    float   compactness_ratio;
    float   meshing_penalty;
    int     will_join_only;
    float   ratio;
    int     max_degree;

} MxConfig;

typedef struct MxShell_TAG {
    float  *points;
    int    pcount;
    int    *faces; /* hoops HC_Insert_Shell() format, but w/o the negative faces */
    int    flen; 
} MxShell;

typedef struct MxShellChain_TAG {
    MxShell         sh;
    int             pointmap_count;
    int             *pointmap;
    struct MxShellChain_TAG *next;
} MxShellChain;


/*
 *  LOD module entry points
 */

#ifdef __cplusplus
extern "C" {
#endif
#ifdef __HP_aCC
extern "C" {
#endif

/* needed by Compute_Optimized_Shell */
extern void HU_LOD_Execute(
    int plist_len, const float *plist,
    int flist_len, const int *flist, 
    MxConfig *cfg_ptr,                  /* pass NULL for all default values */
    int *plist_len_out, float *plist_out, 
    int *flist_len_out, int *flist_out, 
    int *pmap, int *fmap     
);


/* needed by Std_Draw_3d_Polyhedron */
extern MxShellChain *HU_Compute_LOD_Fast(
    int pcount,
    const float *points,
    int flen, 
    const int *faces,
    float ratio,
    int depth 
);

extern MxShellChain *HU_LOD_Chain_Execute(
        int point_count, const float *points, int flistlen, int *flist,
                float ratio, int depth );



extern int  HU_Triangulate_Face (
    float const *points,
    float const *normal,
    int const   *face_list,
    int const   *face_list_end,
    void        ( * triangle_action)
                    (void * info, int convex_triangulation, int v1, int v2, int v3),
    void        *action_info);


#ifdef __cplusplus
}
#endif
#ifdef __HP_aCC
}
#endif

#endif





/************************************************************************

  Standard math include file for the MixKit library.
  Also, type definitions for various vectors and matrices

  Copyright (C) 1998 Michael Garland.  See "COPYING.txt" for details.
  
 ************************************************************************/

#ifndef MXMATH_INCLUDED
#define MXMATH_INCLUDED

#include <math.h>


typedef struct Vec4_TAG
{
    double elt[4];
} Vec4;

typedef struct Vec3_TAG
{
    double elt[3];
} Vec3;

typedef struct Vec2_TAG
{
    double elt[2];
} Vec2;

typedef struct Mat4_TAG
{
    Vec4 row[4];
} Mat4;

typedef struct Mat3_TAG
{
    Vec3 row[3];
} Mat3;

typedef struct Mat2_TAG
{
    Vec2 row[2];
} Mat2;

typedef int MxBool;


/* some operations specific to 3x3 matrices */
extern double invert33( Mat3 *, const Mat3 * );
extern void col33( Vec3 *v_out, const Mat3 *m1, int i );
extern void vecmul33( Vec3 *v_out, const Mat3 *m1, const Vec3 *v1);

/* some operations specific to 4x4 matrices */
extern void adjoint44( Mat4 *out, const Mat4 *in );
extern void matmul44( Mat4 *out, const Mat4 *m1, const Mat4 *m2 );
extern void vecmul44( Vec4 *, const Mat4 * m1, const Vec4 * v);


/* variable length vector operations */

extern void   mxv_add( double *r, const double *u, const double *v, int dim );
extern void   mxv_sub( double *r, const double *u, const double *v, int dim );
extern void   mxv_mul( double *r, const double *u, const double d, int dim );
extern void   mxv_div( double *r, const double *u, const double d, int dim );
extern void   mxv_neg ( double *r, const double *u, int dim);
extern void   mxv_set ( double *r, const double d, int dim);
extern void   mxv_setv ( double *r, const double *u, int dim);
extern double  mxv_dot(const double *u, const double *v, int dim);
extern void   mxv_cross(double *r, const double *u, const double *v, int dim);
extern double  mxv_len(const double *v, int dim);
extern double  mxv_len2(const double *v, int dim); 
extern int  mxv_unitize(double *v, int dim);
extern MxBool mxv_exact_equal(const double *u, const double *v, int dim);
extern MxBool mxv_equal(const double *u, const double *v, int dim);
extern void   mxv_basis(double *r, int b, int dim);



/* test for "not a number" (assumes type double coming in) */
#define isNaN(x) (!((x)==(volatile double)(x)))

/*
 * constants and definitions for cross-platform
 */

/* Some systems use HUGE_VAL instead of HUGE*/
#if !defined(HUGE) && defined(HUGE_VAL)
#define HUGE HUGE_VAL
#endif

/* Handle platforms, such as Win32, which don't define M_PI in <math.h>*/
#ifndef M_PI
#define M_PI 3.141592653589793238462643383279502884197169399375105820974944592308
#endif

/* inline bool FEQ(double a,double b,double eps) { return fabs(a-b)<eps; } */
#define MxFEQ(a,b,eps) ((fabs((a)-(b))<(eps)))

#ifndef MIX_NO_AXIS_NAMES
enum Axis {X=0, Y=1, Z=2, W=3};
#endif


/* from the old mxgeom3d.h */
extern double triangle_area(const Vec3 *, const Vec3 *, const Vec3 * );
extern void  triangle_raw_normal( Vec3 *, const Vec3 *, const Vec3 *, const Vec3 * );
extern int  triangle_normal( Vec3 *, const Vec3 *, const Vec3 *, const Vec3 * );
extern int  triangle_plane( Vec4 *, const Vec3 *, const Vec3 *, const Vec3 * );
extern void  triangle_raw_plane( Vec4 *, const Vec3 *, const Vec3 *, const Vec3 * );
extern double triangle_compactness( const Vec3 *, const Vec3 *, const Vec3 * );
extern void  mx3d_box_corners( Vec3 [], const Vec3 *, const Vec3 * );



/* MXMATH_INCLUDED*/
#endif





/************************************************************************

  MxType.h

  This file contains most of the data types needed by the LOD module:
  Block, Heap
  Vertex, Face, Edge
  VertexList, FaceList, EdgeList

  Copyright (C) 1998 Michael Garland.  See "COPYING.txt" for details.
  
 ************************************************************************/

#ifndef MXTYPE_INCLUDED 
#define MXTYPE_INCLUDED

#include <stdlib.h>

/* 
 * blocks are arrays that automatically resize
 */
typedef struct Block_TAG
{
    int allocated;
    int used;
    int size_each;
    void *data;

} Block;


/*
 * function declarations for manipulating blocks
 */
extern void block_init    ( Block *, int size_each_in );
extern void block_cleanup ( Block * );
extern void resetb  ( Block * );
extern int  addb    ( Block *, const void *elem );
extern int  addpb   ( Block *, const void *elem );
extern void clobberb( Block *, const void *elem, int i );
extern void swapb   ( Block *, int i, int j );
extern void removeb ( Block *, int which );
extern void resizeb ( Block *, int size_in );
extern void chopb   ( Block * );
extern int  isvalidb( const Block *, int which );

#define lengthb(b) ((b)->used)
#define getb(b,i)  ( (char *)(b)->data + ((i) * (b)->size_each) )
#define getpb(b,i) ( *( (void **) getb ( (b), (i) ) ) )


/* heap nodes */
typedef struct MxHeapable_TAG
{
    double import;  /* priority */
    int token;      /* place in heap */
    void *payload;  /* data */

} MxHeapable;

/*
 * functions for manipulating mxheapables
 */
/* inline bool is_in_heap( MxHeapable *h ) { return h->token != -47; } */
/* inline void not_in_heap( MxHeapable *h ) { h->token = -47; } */
/* inline int get_heap_pos( MxHeapable *h ) { return h->token; } */
/* inline void set_heap_pos( MxHeapable *h, int t ) { h->token=t; } */
/* inline void  set_heap_key( MxHeapable *h, double k ) { h->import=k; } */
/* inline double get_heap_key( const MxHeapable *h ) { return h->import; } */
/* inline void mxheapable_init( MxHeapable *h ) { h->import = 0.0f; h->token = -47; h->payload = NULL; } */

#define is_in_heap(h) ( (h)->token != -47 )
#define not_in_heap(h) ( (h)->token = -47 )
#define get_heap_pos(h) ( (h)->token )
#define set_heap_pos(h,t) ( (h)->token = t )
#define set_heap_key(h,k) ( (h)->import = k )
#define get_heap_key(h) ( (h)->import )
#define mxheapable_init(h) { (h)->import = 0.0f; (h)->token = -47; (h)->payload = NULL; }



/* the heap */
typedef struct MxHeap_TAG
{
    Block data; /* a dynamically growable array of heapables. */

} MxHeap;


extern void mxheap_init   ( MxHeap * );
extern void mxheap_cleanup( MxHeap * ) ; 
extern void inserth       ( MxHeap *, MxHeapable * );
extern void updateh       ( MxHeap *, MxHeapable * );
extern void *extracth     ( MxHeap * );
extern void removeh       ( MxHeap *, MxHeapable * );
extern void *itemh        ( MxHeap *, int );

/* inline int sizeh( MxHeap *m )  { return lengthb( &(m->data) ); } */
#define sizeh(m) ( lengthb( &((m)->data) ) )




typedef int MxVertexID;
typedef int MxFaceID;


/* 
 *  Type declarations for MxVertex, MxProxy, MxFace, MxEdge
 *  and associated helper functions
 */

typedef Vec3 MxVertex;

/* needed for storing multiple vertices with different attributes */
typedef struct MxProxy_TAG
{
    MxVertexID prev, next; 

} MxProxy;

typedef struct MxEdge_TAG
{
    MxVertexID v1, v2;

} MxEdge ;

typedef struct MxFace_TAG
{
    MxVertexID v[3];

} MxFace;


/* inline MxVertexID opposite_vertex( MxEdge *e, MxVertexID v )
{
    if( v == e->v1 ) return e->v2;
    else { return e->v1; }
}*/
#define opposite_vertex(e,v) (((v)==(e)->v1)?(e)->v2:(e)->v1)


extern void       mxface_init( MxFace *, MxVertexID, MxVertexID, MxVertexID );
extern int        face_find_vertex( const MxFace *, MxVertexID );
extern MxVertexID face_opposite_vertex( const MxFace *, MxVertexID, MxVertexID );
extern MxBool     face_is_inorder( const MxFace *, MxVertexID, MxVertexID );
extern int        face_remap_vertex( MxFace *, MxVertexID, MxVertexID );



/* 
 *  Type declarations for MxColor, MxNormal, and MxTexcoord 
 *  (all are optionally included in MxModel )
 *  and associated helper functions
 */

typedef struct MxColor_TAG
{
    unsigned char r, g, b, a;
    unsigned int word;

} MxColor;

typedef struct MxNormal_TAG
{
    double dir[3];

} MxNormal;

typedef struct MxTexCoord_TAG
{
    double u[2];

} MxTexCoord;


#define ftop(x) ( (unsigned char)(((x)>1.0f?1.0f:(x))*255.0f) )
#define ptof(x) ( (x) / 255.0f )

extern void mxcolor_init( MxColor *, double, double, double  );
extern void mxtexcoord_init( MxTexCoord *, double, double );
extern void mxnormal_init( MxNormal *, double, double, double );

/*
 * Vertex, Face, and Edge lists, plus associated utility functions 
 */

typedef Block MxFaceList;      /* holds type MxFaceID */
typedef Block MxVertexList;    /* holds type MxVertexID */

/* inline MxFaceID fl_get_face( MxFaceList *fl, int i ){return *((MxFaceID *) getb( fl, i ));} */
/* inline MxVertexID vl_get_vertex( MxVertexList *vl, int i ){  return *((MxVertexID *) getb( vl, i ));} */
#define fl_get_face(fl,i) ( *( (MxFaceID *) getb( fl, i ) ) )
#define vl_get_vertex(vl,i) ( *( (MxVertexID *) getb( vl, i ) ) )

extern MxBool fl_find_face( MxFaceList *fl, MxFaceID fid, int *index );
extern MxBool vl_find_vertex( MxVertexList *vl, MxFaceID vid, int *index );
                        
#endif





/************************************************************************

  MxModel

  Copyright (C) 1998 Michael Garland.  See "COPYING.txt" for details.
  
 ************************************************************************/

#ifndef MXSTDMODEL_INCLUDED 
#define MXSTDMODEL_INCLUDED


/* 
 * some useful definitions for the model type 
 */
#define MX_UNBOUND 0x0
#define MX_PERFACE 0x1
#define MX_PERVERTEX 0x2
#define MX_MAX_BINDING 0x2

#define MX_NORMAL_MASK   0x3
#define MX_COLOR_MASK    (0x3<<2)
#define MX_TEXTURE_MASK  (0x3<<4)
#define MX_ALL_MASK      (MX_NORMAL_MASK|MX_COLOR_MASK|MX_TEXTURE_MASK)




typedef struct vertex_data_TAG  
{
    unsigned char mark, tag;             /* Internal tag bits*/
    unsigned char user_mark, user_tag;   /* External tag bits*/

} vertex_data;


typedef struct face_data_TAG  
{
    unsigned char mark, tag;             /* Internal tag bits*/
    unsigned char user_mark, user_tag;   /* External tag bits*/

} face_data;


typedef struct MxPairContraction_TAG
{
    MxVertexID v1, v2;
    double dv1[3], dv2[3];  /* dv2 is not really necessary*/

    Block delta_faces; /* holds type int */
    Block dead_faces; /* holds type int */

} MxPairContraction;

extern void mxpaircontraction_init( MxPairContraction *mpc );
extern void mxpaircontraction_cleanup( MxPairContraction *mpc );

/* inline int get_delta_faces( MxPairContraction *mpc, int i ) { return *((int *) getb (&(mpc->delta_faces), i ) ); } */
/* inline int get_dead_faces( MxPairContraction *mpc, int i ) { return *((int *) getb (&(mpc->dead_faces), i ) ); } */
#define get_delta_faces(mpc,i) ( *((int *) getb (&((mpc)->delta_faces), (i) ) ) )
#define get_dead_faces(mpc,i) ( *((int *) getb (&((mpc)->dead_faces), (i) ) ) )


typedef MxPairContraction MxPairExpansion;

/* Masks for internal tag bits*/
#define MX_VALID_FLAG 0x01
#define MX_PROXY_FLAG 0x02
#define MX_TOUCHED_FLAG 0x04

typedef struct MxModel_TAG
{
    unsigned char cbinding, nbinding, tbinding;
    unsigned int flags;
    
    /* all mapping lists hold type int */
    Block face_map1;    /* maps original polygons to this model's original triangles */
    Block face_map2;    /* maps this model's original triangles to output triangles */
    Block vertex_map;   /* maps original vertices to output vertices */

    /* Required blocks*/
    Block vertices;     /* stores type MxVertex */
    Block faces;        /* stores type MxFace */
    /* Optional blocks*/
    Block *normals;     /* stores type MxNormal */
    Block *colors;      /* stores type MxColor */
    Block *tcoords;     /* stores type MxTexCoord */

    int binding_mask;
    Block v_data; /* holds type vertex_data */
    Block f_data; /* holds type face_data */
    Block face_links;

} MxModel;

extern void mxmodel_init( MxModel *m, int nvert, int nface);
extern void mxmodel_cleanup( MxModel *m );


/*
 * Simple access routines
 */
/* inline MxVertex *model_vertex( MxModel *m, int i ) { return (MxVertex *) getb ( &(m->vertices), i ); } */
/* inline MxFace *model_face( MxModel *m, int i ) { return (MxFace *) getb ( &(m->faces), i ); } */
/* inline MxVertex *model_corner( MxModel *m, MxFaceID f, short i) { return model_vertex( m, model_face(m, f)->v[i]); } */
/* inline MxNormal *model_normal( MxModel *m, int i ) { return (MxNormal *) getb ( m->normals, i ); } */
/* inline MxColor *model_color( MxModel *m, int i ) { return (MxColor *) getb ( m->colors, i ); } */
/* inline MxTexCoord *model_texcoord( MxModel *m, int i ) { return (MxTexCoord *) getb ( m->tcoords, i ); } */
/* inline int model_vertex_count(MxModel *m) { return lengthb( &(m->vertices) ); } */
/* inline int model_face_count(MxModel *m) { return lengthb( &(m->faces) ); } */
#define model_vertex(m,i)   ( (MxVertex *) getb ( &((m)->vertices), (i) ) )
#define model_face(m,i)     ( (MxFace *) getb ( &((m)->faces), (i) ) ) 
#define model_corner(m,f,i) ( model_vertex( (m), model_face((m), (f))->v[(i)]) )
#define model_normal(m,i)   ( (MxNormal *) getb ( (m)->normals, (i) ) )
#define model_color(m,i)    ( (MxColor *) getb ( m->colors, i ) )
#define model_texcoord(m,i) ( (MxTexCoord *) getb ( m->tcoords, i ) )
#define model_vertex_count(m) ( lengthb( &((m)->vertices) ) )
#define model_face_count(m) ( lengthb( &((m)->faces) ) )
#define model_vertex_map_entry(m,i)  ( (POINTER_SIZED_INT) getpb ( &((m)->vertex_map), (i) ) )
extern int model_valid_face_count( MxModel *m );
extern int model_valid_vertex_count( MxModel *m );


/* 
 * Accessors for internal tag and mark bits 
 */
/* inline vertex_data *get_v_data( MxModel *m, int i ) { return (vertex_data *) getb ( &(m->v_data), i ); }; */
/* inline int v_check_tag( MxModel *m, MxVertexID i, int tag ) { return get_v_data( m,i )->tag & tag; } */
/* inline void v_set_tag( MxModel *m, MxVertexID i, int tag ) { get_v_data( m, i )->tag |= tag; } */
/* inline void v_unset_tag( MxModel *m, MxVertexID i, int tag ) { get_v_data( m, i )->tag &= ~tag; } */
/* inline unsigned char get_vmark( MxModel *m, MxVertexID i )  { return get_v_data( m, i )->mark; } */
/* inline void vmarkc( MxModel *m, MxVertexID i, unsigned char c ) { get_v_data( m, i )->mark = c; } */
#define get_v_data(m,i)    ( (vertex_data *) getb ( &((m)->v_data), (i) ) )
#define v_check_tag(m,i,t) ( get_v_data( (m), (i) )->tag & (t) )
#define v_set_tag(m,i,t)   ( get_v_data( (m), (i) )->tag |= (t) )
#define v_unset_tag(m,i,t) ( get_v_data( (m), (i) )->tag &= ~(t) )
#define get_vmark(m,i)     ( get_v_data( (m), (i) )->mark )
#define vmarkc(m,i,c)      ( get_v_data( (m), (i) )->mark = (c) )

/* inline face_data *get_f_data( MxModel *m, int i ) { return (face_data *) getb ( &(m->f_data), i ); }; */
/* inline int f_check_tag( MxModel *m, MxFaceID i, int tag )  { return get_f_data( m, i )->tag & tag; } */
/* inline void f_set_tag( MxModel *m, MxFaceID i, int tag ) { get_f_data( m, i )->tag |= tag; } */
/* inline void f_unset_tag( MxModel *m, MxFaceID i, int tag ) { get_f_data( m, i )->tag &= ~tag; } */
/* inline unsigned char get_fmark( MxModel *m, MxFaceID i )  { return get_f_data( m, i )->mark; } */
/* inline void fmark( MxModel *m, MxFaceID i, unsigned char c ) { get_f_data( m, i )->mark = c; } */
#define get_f_data(m,i)    ( (face_data *) getb ( &((m)->f_data), (i) ) )
#define f_check_tag(m,i,t) ( get_f_data( (m), (i) )->tag & (t) )
#define f_set_tag(m,i,t)   ( get_f_data( (m), (i) )->tag |= (t) )
#define f_unset_tag(m,i,t) ( get_f_data( (m), (i) )->tag &= ~(t) )
#define get_fmark(m,i)     ( get_f_data( (m), (i) )->mark )
#define fmark(m,i,c)       ( get_f_data( (m), (i) )->mark = (c) )


/* 
 * exported access for tagging and marking 
 */
/* inline int vertex_is_valid( MxModel *m, MxVertexID i ) { return v_check_tag(m,i,MX_VALID_FLAG); } */
/* inline void vertex_mark_valid( MxModel *m, MxVertexID i ) { v_set_tag(m,i,MX_VALID_FLAG); } */
/* inline void vertex_mark_invalid( MxModel *m, MxVertexID i ) { v_unset_tag(m,i,MX_VALID_FLAG); } */
/* inline int face_is_valid( MxModel *m, MxFaceID i )  { return f_check_tag(m,i,MX_VALID_FLAG);} */
/* inline void face_mark_valid( MxModel *m, MxFaceID i ) { f_set_tag(m,i,MX_VALID_FLAG); } */
/* inline void face_mark_invalid( MxModel *m, MxFaceID i ) { f_unset_tag(m,i,MX_VALID_FLAG); } */
#define vertex_is_valid(m,i)     ( v_check_tag((m),(i),MX_VALID_FLAG) )
#define vertex_mark_valid(m,i)   ( v_set_tag((m),(i),MX_VALID_FLAG) )
#define vertex_mark_invalid(m,i) ( v_unset_tag((m),(i),MX_VALID_FLAG) )
#define face_is_valid(m,i)       ( f_check_tag((m),(i),MX_VALID_FLAG) )
#define face_mark_valid(m,i)     ( f_set_tag((m),(i),MX_VALID_FLAG) )
#define face_mark_invalid(m,i)   ( f_unset_tag((m),(i),MX_VALID_FLAG) )

/* 
 * Accessors for external tag and mark bits 
 */
/* inline int vertex_check_tag( MxModel *m, MxVertexID i, int tag ) { return get_v_data( m, i )->user_tag & tag; } */
/* inline void vertex_set_tag( MxModel *m, MxVertexID i, int tag ) { get_v_data( m, i )->user_tag |= tag; } */
/* inline void vertex_unset_tag( MxModel *m, MxVertexID i, int tag ) { get_v_data( m, i )->user_tag &= ~tag;} */
/* inline unsigned char get_vertex_mark( MxModel *m, MxVertexID i ) { return get_v_data( m, i )->user_mark; } */
/* inline void vertex_mark( MxModel *m, MxVertexID i, unsigned char c ) { get_v_data( m, i )->user_mark=c; } */
#define vertex_check_tag(m,i,t) ( get_v_data( (m), (i) )->user_tag & (t) )
#define vertex_set_tag(m,i,t)   ( get_v_data( (m), (i) )->user_tag |= (t) )
#define vertex_unset_tag(m,i,t) ( get_v_data( (m), (i) )->user_tag &= ~(t) )
#define get_vertex_mark(m,i)    ( get_v_data( (m), (i) )->user_mark )
#define vertex_mark(m,i,c )     ( get_v_data( (m), (i) )->user_mark=(c) )

/* inline int face_check_tag( MxModel *m, MxFaceID i, int tag ) { return get_f_data( m, i )->user_tag & tag; } */
/* inline void face_set_tag( MxModel *m, MxFaceID i, int tag ) { get_f_data( m, i )->user_tag |= tag; } */
/* inline void face_unset_tag( MxModel *m, MxFaceID i, int tag ) { get_f_data( m, i )->user_tag &= ~tag;} */
/* inline unsigned char face_mark( MxModel *m, MxFaceID i ) { return get_f_data( m, i )->user_mark; } */
/* inline void face_mark( MxModel *m, MxFaceID i, unsigned char c ) { get_f_data( m, i )->user_mark = c; } */
#define face_check_tag(m,i,t) ( get_f_data( (m), (i) )->user_tag & (t) )
#define face_set_tag(m,i,t)   ( get_f_data( (m), (i) )->user_tag |= (t) )
#define face_unset_tag(m,i,t) ( get_f_data( (m), i )->user_tag &= ~(t) )
#define get_face_mark(m,i)    ( get_f_data( (m), (i) )->user_mark )
#define face_mark(m,i,c)      ( get_f_data( (m), (i) )->user_mark = (c) )


/*
 *  Vertex management
 */

extern MxVertexID add_vertex   ( MxModel *m, double, double, double);
extern MxFaceID   add_face     ( MxModel *m, int, int, int);
extern int        add_color    ( MxModel *m, double, double, double);
extern int        add_normal   ( MxModel *m, double, double, double);
extern int        add_texcoord ( MxModel *m, double, double);
extern void       remove_vertex( MxModel *m, MxVertexID v);
extern void       free_vertex  ( MxModel *m, MxVertexID );
extern MxFaceID   alloc_face   ( MxModel *m, MxVertexID, MxVertexID, MxVertexID );
extern void       init_face    ( MxModel *m, MxFaceID );



/* inline int normal_binding(MxModel *m) { return ( m->nbinding & m->binding_mask ); } */
/* inline int color_binding(MxModel *m) { return ( m->cbinding & (m->binding_mask >> 2) ); } */
/* inline int texcoord_binding(MxModel *m) { return ( m->tbinding & (m->binding_mask >> 4) ); } */
#define normal_binding(m) ( ( (m)->nbinding & (m)->binding_mask ) )
#define color_binding(m) ( ( (m)->cbinding & ((m)->binding_mask >> 2) ) )
#define texcoord_binding(m) ( ( (m)->tbinding & ((m)->binding_mask >> 4) ) )

extern const char *binding_name( MxModel *m, int );
extern int parse_binding( MxModel *m, const char * );

extern int   compute_face_normal   ( MxModel *m, MxFaceID, double *, MxBool will_unitize );
extern double compute_face_area     ( MxModel *m, MxFaceID );
extern double compute_face_perimeter( MxModel *m, MxFaceID, MxBool *edge_flags );

extern double compute_corner_angle( MxModel *m, MxFaceID, int );


/*
 *  Neighborhood collection and management, vertex normal calculations
 */
extern void mark_neighborhood( MxModel *m, MxVertexID, unsigned short );
extern void collect_unmarked_neighbors( MxModel *m, MxVertexID, MxFaceList * );
extern void partition_marked_neighbors( MxModel *m, MxVertexID, unsigned short pivot,
        MxFaceList *below, MxFaceList *above );

extern void mark_corners( MxModel *m, MxFaceList *faces, unsigned short mark );
extern void collect_unmarked_corners(MxModel *m, MxFaceList *faces,MxVertexList *verts );

extern void collect_edge_neighbors( MxModel *m, MxVertexID, MxVertexID, MxFaceList * );
extern void collect_vertex_star( MxModel *m, MxVertexID v, MxVertexList *verts );

/* extern MxFaceList *neighbors( MxModel *m, MxVertexID v ); */
#define neighbors(m,v) ((MxFaceList *) getpb( &((m)->face_links), (v) ))

extern void compute_vertex_normal( MxModel *m, MxVertexID v, double * );

/*
 * Primitive transformation operations
 */
extern void model_remap_vertex( MxModel *m, MxVertexID from, MxVertexID to );
extern MxVertexID split_edge( MxModel *m, MxVertexID v1, MxVertexID v2, double x, double y, double z );
extern MxVertexID split_edge_simple( MxModel *m, MxVertexID v1, MxVertexID v2 );

extern void flip_edge( MxModel *m, MxVertexID v1, MxVertexID v2 );
extern void split_face4( MxModel *m, MxFaceID f, MxVertexID *newverts );
extern void unlink_face( MxModel *m, MxFaceID f);



/*
 * Contraction and related operations
 */
extern void compact_vertices( MxModel * );
extern void remove_degeneracy( MxModel *, MxFaceList * );

/* Pair contraction interface*/
extern void compute_pair_contraction( MxModel *, MxVertexID, MxVertexID, MxPairContraction *);
extern void apply_pair_contraction( MxModel *, MxPairContraction *);
extern void apply_pair_expansion( MxModel *, MxPairExpansion *);
extern void pair_contract( MxModel *, MxVertexID v1, MxVertexID v2,
        const double *, MxPairContraction *);


/* MXSTDMODEL_INCLUDED*/
#endif





/************************************************************************

  Surface simplification using quadric error metrics

  Copyright (C) 1998 Michael Garland.  See "COPYING.txt" for details.
  
 ************************************************************************/

#ifndef MXQSLIM_INCLUDED 
#define MXQSLIM_INCLUDED

typedef struct MxQSlimEdge_TAG
{
    MxHeapable h;
    MxEdge e;
    double vnew[3];

} MxQSlimEdge;

extern void mxqslimedge_init( MxQSlimEdge *qse );


typedef Block MxEdgeList;   /* holds type MxQSlimEdge* */

/* inline MxQSlimEdge *el_get_edge( MxEdgeList *el, int i ) { return (MxQSlimEdge *) getpb( el, i ); } */
/* inline int el_add_edge( MxEdgeList *el, MxQSlimEdge *q ) { return addpb( el, q ); } */
#define el_get_edge(el,i) ( (MxQSlimEdge *) getpb( el, i ) )
#define el_add_edge(el,q) ( addpb( el, q ) )

extern MxBool find_edge( MxEdgeList *el, const MxQSlimEdge *t, int *index );





typedef struct MxQSlim_TAG
{
    MxEdgeList edge_array;
    Block edge_links; /* stores type MxEdgeList, links to elements of edge_array */
    MxModel *m;
    Block quadrics; /* stores type MxQuadric3 */
    MxHeap heap;

    int valid_verts;
    int valid_faces;

    /* to be copied from MxConfig at initialization */
    int placement_policy;
    int weighting_policy;
    double boundary_weight;
    double compactness_ratio;
    double meshing_penalty;
    MxBool will_join_only;
    int max_degree;

    double local_validity_threshold;
    Mat4 *object_transform;

    void (*contraction_callback)(const MxPairContraction *, double);

} MxQSlim;

extern void   mxqslim_init( MxQSlim *, MxModel *, MxConfig * );
extern void   mxqslim_cleanup( MxQSlim * );

extern MxBool qs_decimate( MxQSlim *, int );
extern void   qs_apply_contraction( MxQSlim *, MxPairContraction * );

extern void   discontinuity_constraint( MxQSlim *, MxVertexID, MxVertexID, MxFaceList *);
extern void   collect_quadrics( MxQSlim * );
extern void   transform_quadrics( MxQSlim *, const Mat4 *);
extern void   constrain_boundaries( MxQSlim * );

extern double  check_local_compactness( MxQSlim *, int v1, int v2, const double *vnew );
extern double  check_local_inversion( MxQSlim *, int v1, const double *vnew );
extern int    check_local_validity( MxQSlim *, int v1, int v2, const double *vnew );
extern void   apply_penalties( MxQSlim *, MxQSlimEdge * );
extern void   create_edge( MxQSlim *, MxVertexID i, MxVertexID j );
extern void   collect_edges( MxQSlim * );

extern void compute_target_placement( MxQSlim *, MxQSlimEdge * );

extern void compute_edge_info( MxQSlim *, MxQSlimEdge * );
extern void update_pre_contract( MxQSlim *, const MxPairContraction * );
extern void update_post_expand( MxQSlim *, const MxPairContraction * );


/* inline MxQuadric3 *qs_get_quadrics( MxQSlim *q, int i ) { return (MxQuadric3 *) getpb ( &(q->quadrics), i ); } */
/* inline MxEdgeList *qs_get_edge_links( MxQSlim *q, int i ) {  return (MxEdgeList *) getpb ( &(q->edge_links), i ); } */
/* inline int edge_count( MxQSlim *q )  { return sizeh( &(q->heap) ); } */
#define qs_get_quadrics(q,i) ( (MxQuadric3 *) getpb ( &((q)->quadrics), (i) ) )
#define qs_get_edge_links(q,i) ( (MxEdgeList *) getpb ( &((q)->edge_links), (i) ) )
/* #define edge_count(q) ( sizeh( &((q)->heap) ) ) */



/* MXQSLIM_INCLUDED*/
#endif





/************************************************************************

  3D Quadric Error Metric

  Copyright (C) 1998 Michael Garland.  See "COPYING.txt" for details.
  
 ************************************************************************/

#ifndef MXQMETRIC3_INCLUDED 
#define MXQMETRIC3_INCLUDED

typedef struct MxQuadric3_TAG
{
    double a2, ab, ac, ad;
    double     b2, bc, bd;
    double         c2, cd;
    double             d2;

    double r;

} MxQuadric3;

/* initialization */
void mxquadric3_init( MxQuadric3 *, double a, double b, double c, double d, double area );
void mxquadric3_init_by_matrix( MxQuadric3 *, const Mat4 *Q, double area ); 

/* manipulation utilities */
extern void quad_copy( MxQuadric3 *, const MxQuadric3 *Q );
extern void quad_add( MxQuadric3 *, const MxQuadric3 *, const MxQuadric3 * );
extern void quad_sub( MxQuadric3 *, const MxQuadric3 *, const MxQuadric3 * );
extern void quad_mul( MxQuadric3 *, const MxQuadric3 *, const double );
extern void quad_scale( MxQuadric3 *, double s ); /* multiply in place */
extern void quad_clear( MxQuadric3 *q );
extern void quad_transform( MxQuadric3 *, const MxQuadric3 *, const Mat4 * );
/*inline void quad_set_area( MxQuadric3 *q, double a ) { q->r=a; }*/
#define quad_set_area(q,a) { (q)->r=(a); }

/* access utilities */
extern double quad_offset( const MxQuadric3 * ); 
extern double quad_area( const MxQuadric3 * );

/* evaluation and optimization */
extern MxBool optimizev ( const MxQuadric3 *, Vec3 *v ); 
extern MxBool optimize3f( const MxQuadric3 *, double *x, double *y, double *z ); 
extern MxBool optimize2v( const MxQuadric3 *, Vec3 *v, const Vec3 *v1, const Vec3 *v2 ); 
extern MxBool optimize3v( const MxQuadric3 *, Vec3 *v, const Vec3 *v1, const Vec3 *v2, const Vec3 *v3 );
extern double quad_evaluate( const MxQuadric3 *, double x, double y, double z );
extern double quad_evaluatev( const MxQuadric3 *q, const double *v );



/* MXQMETRIC3_INCLUDED*/
#endif
