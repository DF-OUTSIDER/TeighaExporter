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
// $Header: //DWF/Working_Area/Willie.Zhu/w3dtk/BCompress.h#1 $
//
// This file declares all of the external symbols (functions and lookup tables)
// used in the various compression algorithms in HOOPS/stream
//

///
///\file        dwf/w3dtk/BCompress.h
///

#include "dwf/w3dtk/BStream.h"
#include "dwf/w3dtk/BPolyhedron.h"
#include "dwf/w3dtk/BEdgeBreaker.h"
	

#define CS_INVALID                      0
#define CS_TRIVIAL                      1
#define CS_TRIVIAL_IMPLICIT             2
#define CS_REPULSE                      3
#define CS_NONE                         4
#define CS_EDGEBREAKER                  5
#define CS_LINE_EXTEND                  6
#define CS_TRIVIAL_POLAR		7

extern TK_Status quantize_and_pack_floats ( 
        BStreamFileToolkit & tk,
        int count_in,
	int size_in,
        float const *floats_in, 
        float const *bounding_in,
        unsigned int *exists_in,
        unsigned int mask_in,
	unsigned int bits_per_sample_in,
        float *bounding_out,
        int *workspace_len_in_out,
        int *workspace_used_out,
        unsigned char **workspace_in_out );


extern TK_Status quantize_and_pack_floats ( 
        BStreamFileToolkit & tk,
        int count_in,
	int size_in,
        float const *floats_in, 
        float const *bounding_in,
	unsigned int bits_per_sample_in,
        float *bounding_out,
        int *workspace_len_in_out,
        int *workspace_used_out,
        unsigned char **workspace_in_out );

extern TK_Status unquantize_and_unpack_floats ( 
        BStreamFileToolkit & tk,
        int count_in,
	int size_in,
	unsigned int bits_per_sample_in,
	float const *bounding_in,
        unsigned char *buf_in,
        float **floats_out );

extern TK_Status trivial_compress_points ( 
	BStreamFileToolkit & tk,
	int point_count,
	float const *points, 
	float const *bounding_in,
	unsigned int *exists,
	unsigned int mask,
	int *workspace_len_in_out,
        int *workspace_used_out,
        unsigned char **workspace_in_out,
	float * bounding_out);
extern TK_Status trivial_decompress_points ( 
	BStreamFileToolkit & tk,
	int pointcount,
	void const *buf_in,
	float **points_out,
	float const * bounding );

#define MAXU10 128
#define MAXV10 64
#define MAXU8 64
#define MAXV8 32
extern float normal_vectors10[1024][3];
extern unsigned short closest_points10[MAXU10][MAXV10];
extern float normal_vectors8[256][3];
extern unsigned char closest_points8[MAXU8][MAXV8];


extern const int bounding_faces[30];
extern const int bounding_tristrips[19];
extern const float color_cube[6];
extern const float normal_cube[6];
extern const float polar_bounds[4];




