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

#ifndef _SPAPARAMETERS_H_
#define _SPAPARAMETERS_H_

#include "TD_PackPush.h"

#include "OdHeap.h"

/** \details
    This container class holds ACIS triangulation (rendering) parameters. 
    <group Other_Classes>
*/
class SPAParameters
{
public:
  
  enum SPA_ADJUST
  {
    SPA_ADJUST_NONE,
    SPA_ADJUST_NON_GRID,
    SPA_ADJUST_ALL
  } adjust_mode;

  enum SPA_GRID
  {
    SPA_GRID_NONE,		// No grids at all.
    SPA_GRID_INTERIOR,	// Grids in interior.
    SPA_GRID_TO_EDGES,	// Allow grid to divide model edges.
    SPA_GRID_ONE_DIR		// Grid only in one direction, u or v
  } grid_mode;

  enum SPA_TRIANG_MODE {
    SPA_TRIANG_NONE,
    SPA_TRIANG_ALL,
    SPA_TRIANG_FRINGE_1,
    SPA_TRIANG_FRINGE_2,
    SPA_TRIANG_FRINGE_3,
    SPA_TRIANG_FRINGE_4
  } triang_mode;

  double surface_tol;
  double normal_tol;
  double max_edge_length;
  double grid_aspect_ratio;
  int max_grid_lines;
  int min_u_grid_lines;
  int min_v_grid_lines;
  bool bUseFacetRES;


  SPAParameters()
    : adjust_mode(SPA_ADJUST_NONE)
  , surface_tol(-1)     // use bounding box diagonal
  , normal_tol(15.0)
  , max_edge_length(0.) // ignored
  , grid_aspect_ratio(0.) // ignored
  , grid_mode(SPA_GRID_INTERIOR)
  , triang_mode(SPA_TRIANG_ALL)
  , max_grid_lines(512)
  , min_u_grid_lines(0)
  , min_v_grid_lines(0)
  , bUseFacetRES(false)
  {
  }

  /** \remarks
    bUseFacetRES is NOT part of the comparison.
  */
  bool operator ==(const SPAParameters other) const 
  {
    return other.adjust_mode == adjust_mode &&
      other.surface_tol == surface_tol &&
      other.normal_tol == normal_tol &&
      other.max_edge_length == max_edge_length &&
      other.grid_aspect_ratio == grid_aspect_ratio &&
      other.grid_mode == grid_mode &&
      other.triang_mode == triang_mode &&
      other.max_grid_lines == max_grid_lines &&
      other.min_u_grid_lines == min_u_grid_lines &&
      other.min_v_grid_lines == min_v_grid_lines;
    // bUseFacetRES skipped
  }
};

#include "TD_PackPop.h"

#endif // _SPAPARAMETERS_H_
