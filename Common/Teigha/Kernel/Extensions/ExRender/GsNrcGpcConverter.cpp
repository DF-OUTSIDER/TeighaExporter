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

#include "Ge/avgpc.h"

class OdGsNrcGpcConverter
{
  public:
    static gpc_polygon getNrcClipAsGpcPolygon(int nrcContours, const int* pNrcCounts, const OdGePoint2dArray &nrcPoints)
    {
      gpc_polygon result;
      result.num_contours = 0;
      result.contour = NULL;
      result.hole = NULL;

      if (nrcContours < 1)
        return result;

      result.num_contours = 1;
      result.hole = ::gpc_alloc<int>(1);
      result.hole[0] = 0;
      result.contour = ::gpc_alloc<gpc_vertex_list>(1);
      result.contour[0].num_vertices = pNrcCounts[0];
      result.contour[0].vertex = ::gpc_alloc<gpc_vertex>(pNrcCounts[0]);

      int i;
      for (i = 0; i < pNrcCounts[0]; i++)
      {
        result.contour[0].vertex[i].x = nrcPoints[i].x;
        result.contour[0].vertex[i].y = nrcPoints[i].y;
      }

      if (nrcContours == 1)
      {
        // Fix self-intersections inside contour
        gpc_polygon zero;
        zero.num_contours = 0;
        zero.contour = NULL;
        zero.hole = NULL;
        gpc_polygon res;
        ::gpc_polygon_clip(GPC_XOR, &zero, &result, &res);
        ::gpc_free_polygon(&zero);
        ::gpc_free_polygon(&result);
        result = res;
      }

      int j;
      for (j = 1; j < nrcContours; j++)
      {
        int zero = 0;

        gpc_polygon next;
        next.num_contours = 1;
        next.hole = &zero;
        gpc_vertex_list vList;
        next.contour = &vList;
        vList.num_vertices = pNrcCounts[j];
        OdGePoint2dArray vertices;
        vertices.resize(vList.num_vertices);
        vList.vertex = (gpc_vertex*)vertices.asArrayPtr();

        int k;
        for (k = 0; k < vList.num_vertices; k++, i++)
        {
          vList.vertex[k].x = nrcPoints[i].x;
          vList.vertex[k].y = nrcPoints[i].y;
        }

        gpc_polygon res;
        ::gpc_polygon_clip(GPC_XOR, &result, &next, &res);
        ::gpc_free_polygon(&result);
        result = res;
      }

      return result;
    }

    static gpc_tristrip getGpcPolygonAsTriangulation(gpc_polygon *poly)
    {
      gpc_tristrip result;
      ::gpc_polygon_to_tristrip(poly, &result);
      return result;
    }

    static int getGpcTriangulationAsNrcClip(const gpc_tristrip *trgl, OdIntArray &nrcCounts, OdGePoint2dArray &nrcPoints)
    {
      nrcCounts.resize((OdUInt32)trgl->num_strips);
      int nVerts = 0, i;
      for (i = 0; i < trgl->num_strips; i++)
      {
        nVerts += trgl->strip[i].num_vertices;
      }
      nrcPoints.resize((OdUInt32)nVerts);
      int *pCounts = nrcCounts.asArrayPtr();
      OdGePoint2d *pPoints = nrcPoints.asArrayPtr();
      for (i = 0; i < trgl->num_strips; i++, pCounts++)
      {
        *pCounts = trgl->strip[i].num_vertices;
        for (int j = 0; j < trgl->strip[i].num_vertices; j++, pPoints++)
        {
          pPoints->x = trgl->strip[i].vertex[j].x;
          pPoints->y = trgl->strip[i].vertex[j].y;
        }
      }
      return trgl->num_strips;
    }

    static int getGpcPolygonAsNrcClip(const gpc_polygon *poly, OdIntArray &nrcCounts, OdGePoint2dArray &nrcPoints)
    {
      nrcCounts.resize((OdUInt32)poly->num_contours);
      int nVerts = 0, i;
      for (i = 0; i < poly->num_contours; i++)
      {
        nVerts += poly->contour[i].num_vertices;
      }
      nrcPoints.resize((OdUInt32)nVerts);
      int *pCounts = nrcCounts.asArrayPtr();
      OdGePoint2d *pPoints = nrcPoints.asArrayPtr();
      for (i = 0; i < poly->num_contours; i++, pCounts++)
      {
        *pCounts = poly->contour[i].num_vertices;
        for (int j = 0; j < poly->contour[i].num_vertices; j++, pPoints++)
        {
          pPoints->x = poly->contour[i].vertex[j].x;
          pPoints->y = poly->contour[i].vertex[j].y;
        }
      }
      return poly->num_contours;
    }
};

//
