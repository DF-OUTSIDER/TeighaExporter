/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2018, Open Design Alliance (the "Alliance"). 
// All rights reserved. 
// 
// This software and its documentation and related materials are owned by 
// the Alliance. The software may only be incorporated into application 
// programs owned by members of the Alliance, subject to a signed 
// Membership Agreement and Supplemental Software License Agreement with the
// Alliance. The structure and organization of this software are the valuable  
// trade secrets of the Alliance and its suppliers. The software is also 
// protected by copyright law and international treaty provisions. Application  
// programs incorporating this software must include the following statement 
// with their copyright notices:
//   
//   This application incorporates Teigha(R) software pursuant to a license 
//   agreement with Open Design Alliance.
//   Teigha(R) Copyright (C) 2002-2018 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 
// acknowledge and accept the above terms.
///////////////////////////////////////////////////////////////////////////////

/*
===========================================================================

Project:   Generic Polygon Clipper

           A new algorithm for calculating the difference, intersection,
           exclusive-or or union of arbitrary polygon sets.

File:      gpc.h
Author:    Alan Murta (email: gpc@cs.man.ac.uk)
Version:   2.32
Date:      17th December 2004

Copyright: (C) Advanced Interfaces Group,
           University of Manchester.

           This software is free for non-commercial use. It may be copied,
           modified, and redistributed provided that this copyright notice
           is preserved on all copies. The intellectual property rights of
           the algorithms used reside with the University of Manchester
           Advanced Interfaces Group.

           You may not use this software, in whole or in part, in support
           of any commercial product without the express consent of the
           author.

           There is no warranty or other guarantee of fitness of this
           software for any purpose. It is provided solely "as is".

ODA Notice:  This software has been licensed by the Open Design Alliance.  Any
             use outside of Teigha-based member applications requires the user 
			 to obtain a separate license.

===========================================================================
*/

#ifndef __gpc_h
#define __gpc_h

#include <stdio.h>
#include "Ge/GeExport.h"

#ifdef __cplusplus
extern "C" 
{
#endif

/*
===========================================================================
                               Constants
===========================================================================
*/

/* Increase GPC_EPSILON to encourage merging of near coincident edges    */

#define GPC_EPSILON (DBL_EPSILON)

#define GPC_VERSION "2.32"


/*
===========================================================================
                           Public Data Types
===========================================================================
*/

typedef enum                        /* Set operation type                */
{
  GPC_DIFF,                         /* Difference                        */
  GPC_INT,                          /* Intersection                      */
  GPC_XOR,                          /* Exclusive or                      */
  GPC_UNION                         /* Union                             */
} gpc_op;

typedef struct                      /* Polygon vertex structure          */
{
  double              x;            /* Vertex x component                */
  double              y;            /* vertex y component                */
} gpc_vertex;

typedef struct                      /* Vertex list structure             */
{
  int                 num_vertices; /* Number of vertices in list        */
  gpc_vertex         *vertex;       /* Vertex array pointer              */
} gpc_vertex_list;

typedef struct                      /* Polygon set structure             */
{
  int                 num_contours; /* Number of contours in polygon     */
  int                *hole;         /* Hole / external contour flags     */
  gpc_vertex_list    *contour;      /* Contour array pointer             */
} gpc_polygon;

typedef struct                      /* Tristrip set structure            */
{
  int                 num_strips;   /* Number of tristrips               */
  gpc_vertex_list    *strip;        /* Tristrip array pointer            */
} gpc_tristrip;


/*
===========================================================================
                       Public Function Prototypes
===========================================================================
*/
//DD:EXPORT_ON

void GE_TOOLKIT_EXPORT gpc_read_polygon (FILE            *infile_ptr, 
                              int              read_hole_flags,
                              gpc_polygon     *polygon);

void GE_TOOLKIT_EXPORT gpc_write_polygon (FILE            *outfile_ptr,
                              int              write_hole_flags,
                              gpc_polygon     *polygon);

void GE_TOOLKIT_EXPORT gpc_add_contour (gpc_polygon     *polygon,
                              gpc_vertex_list *contour,
                              int              hole);

void GE_TOOLKIT_EXPORT gpc_polygon_clip (gpc_op           set_operation,
                              gpc_polygon     *subject_polygon,
                              gpc_polygon     *clip_polygon,
                              gpc_polygon     *result_polygon);

/* ODA extension
Returns 1 if returned result_polygon is the same as one of operands (depending on set_operation)
*/
int GE_TOOLKIT_EXPORT gpc_polygon_clip_ex (gpc_op           set_operation,
                              gpc_polygon     *subject_polygon,
                              gpc_polygon     *clip_polygon,
                              gpc_polygon     *result_polygon);

void GE_TOOLKIT_EXPORT gpc_tristrip_clip (gpc_op           set_operation,
                              gpc_polygon     *subject_polygon,
                              gpc_polygon     *clip_polygon,
                              gpc_tristrip    *result_tristrip);

void GE_TOOLKIT_EXPORT gpc_polygon_to_tristrip (gpc_polygon     *polygon,
                              gpc_tristrip    *tristrip);

void GE_TOOLKIT_EXPORT gpc_free_polygon (gpc_polygon     *polygon);

void GE_TOOLKIT_EXPORT gpc_free_tristrip (gpc_tristrip    *tristrip);

#ifdef __cplusplus
} // extern "C"

#include "OdAlloc.h"
/* ODA extension
Allocate memory for polygon elements (compatible with gpc_free_polygon function).
*/
template <class Type>
Type *gpc_alloc(int num_structs)
{ return (Type*)::odrxAlloc(sizeof(Type) * num_structs); }

#endif

//DD:EXPORT_OFF

#endif // __gpc_h

/*
===========================================================================
                           End of file: gpc.h
===========================================================================
*/
