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

#include "StdAfx.h"
#include "OdaCommon.h"
#include "DwfImportImpl.h"
#include "DwfCallbacks.h"
#include "RxVariantValue.h"
#include "DbSpatialFilter.h"
#include "DbIndex.h"

#include "DbEllipse.h"
#include "DbPoint.h"
#include "DbMText.h"
#include "DbBlockReference.h"
#include "Db2dPolyline.h"
#include "Db2dVertex.h"
#include "DbText.h"
#include "DbViewportTable.h"
#include "DbViewportTableRecord.h"
#include "DbViewTable.h"
#include "DbViewTableRecord.h"
#include "DbBlockTable.h"
#include "DbViewport.h"
#include "OdCharMapper.h"
#include "DbCircle.h"
#include "DbArc.h"

#include "Ge/GeEllipArc2d.h"
#include "Ge/GeScale3d.h"
#include "GiContextForDbDatabase.h"
#include "Gi/GiTextStyle.h"

#include "DbPolyline.h"
//#include "DbHatch.h"

using namespace TD_DWF_IMPORT;

TOOLKIT_EXPORT OdString oddbConvertTextToCodePage(const OdString& source, OdCodePageId sourceId, OdCodePageId destId);

// ATTENTION: for DWFTolkit v6 & v7 compatibility
#ifdef _DWF_7_IMPORTER_INCLUDED_  // DWF toolkit 7
#ifdef EMCC
#define ellmajor() major_version()
#define ellminor() minor_version()
#else
#define ellmajor() major()
#define ellminor() minor()
#endif
#endif
namespace TD_DWF_IMPORT {
namespace dwfImp
{
  // Convert WT_Unts to OdDb::UnitsValue
  // for unknown units, return 0;
  //
  OdDb::UnitsValue convertUnits(const WT_Units& units)
  {
    static const char* const Units[] = 
    {
      "", /* 0	Unspecified (No units) */
        "Inches",
        "Feet",
        "Miles",
        "Millimeters",
        "Centimeters",
        "Meters",
        "Kilometers",
        "Microinches",
        "Mils",
        "Yards",
        "Angstroms",
        "Nanometers",
        "Microns",
        "Decimeters",
        "Decameters",
        "Hectometers",
        "Gigameters",
        "Astronomical Units",
        "Light Years",
        "Parsecs"
    };
    for (int i = 0; i < int(sizeof(Units)/sizeof(Units[0])); ++i)
      if (units.units() == Units[i]) 
        return static_cast<OdDb::UnitsValue>(i);
      return OdDb::kUnitsUndefined;
  }
}

// units & transformation matrix
//
WT_Result DwfCallbackManager::process_units(WT_Units & units, WT_File & file)
{
  WT_Units::default_process( units, file );
  DwfExtentManager& mngExtent = importer(file)->extent();
  if (!mngExtent._useUnits)
  {
    const WT_Matrix& m = units.application_to_dwf_transform();
    OdGeMatrix3d mm;
    for (int i = 0; i < 4; ++i)
      for (int j = 0; j < 4; ++j)
        mm(i, j) = m(i, j);
    if (!mm.isSingular())
    {
      mm.transposeIt();
      if (!mm.inverse(mngExtent._units, 1.175494351e-38)) // FLT_MIN))
      {
        ODA_FAIL_ONCE_X(TDWF);
        mm.inverse(mngExtent._units, OdGeContext::gZeroTol.equalPoint());
      }
      mngExtent._useUnits = true;
    }
  }
  if (!mngExtent._collectBounds)
  {
    importer(file)->database()->setINSUNITS(dwfImp::convertUnits(units));
  }
  return WT_Result::Success;
}

WT_Result DwfCallbackManager::process_named_view(WT_Named_View & namedView, WT_File& file )
{
  WT_Named_View::default_process( namedView, file );
  DwfImporter* imp = importer(file);
  DwfExtentManager& mngExtent = imp->extent();
  if (mngExtent._collectBounds)
    return WT_Result::Success;
	WT_Logical_Box box = *namedView.view();
	OdGePoint3d maxpt = mngExtent.transformPoint( box.maxpt() );
	OdGePoint3d minpt = mngExtent.transformPoint( box.minpt() );
  OdString name = dwfImp::normalizeObjectName(namedView.name());
  if (name.isEmpty() || box.minpt().m_x == box.maxpt().m_x || box.minpt().m_y == box.maxpt().m_y)
    return WT_Result::Success; // Real situation (cf. "sto22762.dwf" )
  //
  //if (namedView.name() != "INITIAL")
  //  return WT_Result::Success;
  //ODA_ASSERT_ONCE_X(TDWF, name == "INITIAL");
  // next was moved into DwfCallbackManager::process_view
  if (mngExtent._useUnits && name == "INITIAL")
  {
    // CORE-10319
    OdGePoint3dArray pnts;
  
    //OdGePoint3d minPt(dwfP.m_x, dwfP.m_y, 0);
    //OdGePoint3d maxPt(dwfP2.m_x, dwfP2.m_y, 0);
    //OdGePoint3dArray pnts;
    //pnts.push_back(minPt);
    //pnts.push_back(OdGePoint3d(minPt.x, maxPt.y, 0));
    //pnts.push_back(maxPt);
    //pnts.push_back(OdGePoint3d(maxPt.x, minPt.y, 0));
    //OdGeMatrix3d mx = mngExtent._units;
    //pnts[0].transformBy(mx);
    //pnts[1].transformBy(mx);
    //pnts[2].transformBy(mx);
    //pnts[3].transformBy(mx);
    WT_Logical_Point dwfP = box.minpt();
    WT_Logical_Point dwfP2 = box.maxpt();
    pnts.push_back(mngExtent.transformPoint(dwfP));
    pnts.push_back(mngExtent.transformPoint(WT_Logical_Point(dwfP.m_x, dwfP2.m_y)));
    pnts.push_back(mngExtent.transformPoint(dwfP2));
    pnts.push_back(mngExtent.transformPoint(WT_Logical_Point(dwfP2.m_x, dwfP.m_y)));
  
    OdGeVector3d xA = pnts[3] - pnts[0];
    OdGeVector3d yA = pnts[1] - pnts[0];
    OdGeVector3d zA = xA.crossProduct(yA);
    OdGeMatrix3d mx1;
    mx1.setCoordSystem(OdGePoint3d::kOrigin, xA.normal(), yA.normal(), zA);
    mx1.invert();
  
    //mx = mx1 * mx;
    //mngExtent.m_matrix = mx;
    
    OdDbLayoutPtr layout = imp->blocks().currentLayout(OdDb::kForWrite);
    OdString nameLayout = layout->getLayoutName();
    OdDbObjectId idBlock = layout->getBlockTableRecordId();
    if (   idBlock == imp->database()->getModelSpaceId() // TODO check for all
        || !nameLayout.find(L"Model")) // for BoxRed_23_.dwf
    {
      //ODA_ASSERT_ONCE_X(TDWF, mngExtent.m_matrix == OdGeMatrix3d::kIdentity); // test
      //mngExtent.m_matrix = mx1; // use as mngExtent.m_matrix * mngExtent._units; // = mx;

      mngExtent._units = mx1 *  mngExtent._units;

      // recalculate
      maxpt = mngExtent.transformPoint(box.maxpt());
      minpt = mngExtent.transformPoint(box.minpt());
      OdGePoint3d center((maxpt + minpt.asVector()) / 2);

      // see alse DwfCallbackManager::process_view(WT_View& view, WT_File& file)
      if (idBlock == imp->database()->getModelSpaceId())
      {
        OdDbViewportTablePtr t = imp->database()->getViewportTableId().safeOpenObject();
        OdDbViewportTableRecordPtr vp = t->getActiveViewportId().safeOpenObject(OdDb::kForWrite);
        vp->setHeight(fabs(maxpt.y - minpt.y));
        vp->setWidth(fabs(maxpt.x - minpt.x));
        vp->setCenterPoint(OdGePoint2d(center.x, center.y));
      }
      else
      {
        OdDbObjectId vpId = layout->overallVportId();
        // if this is newly created layout, it may contain no overall viewport
        if (vpId.isNull())
          vpId = imp->blocks().addViewport();
        OdDbViewportPtr vp = vpId.safeOpenObject(OdDb::kForWrite);
        vp->setHeight(fabs(maxpt.y - minpt.y));
        vp->setWidth(fabs(maxpt.x - minpt.x));
        vp->setCenterPoint(center);
        vp->setViewHeight(vp->height());
        vp->setViewCenter(OdGePoint2d(center.x, center.y));
        layout->setActiveViewportId(vpId);
      }
      return WT_Result::Success;
    }
  }
  //
  OdDbViewTablePtr vt = imp->database()->getViewTableId().safeOpenObject(OdDb::kForWrite);
	OdDbViewTableRecordPtr newView;
  if ( vt->has( name ) )
  {
    newView = vt->getAt( name ).safeOpenObject( OdDb::kForWrite );
  }
  else
  {
    newView = OdDbViewTableRecord::createObject();
    newView->setName( name );
    vt->add( newView );
  }
	newView->setHeight( fabs(maxpt.y - minpt.y) );
	newView->setWidth( fabs(maxpt.x - minpt.x) );
	newView->setCenterPoint( OdGePoint2d( ( maxpt.x + minpt.x ) / 2, ( maxpt.y + minpt.y ) / 2 ) );
  newView->setTarget( OdGePoint3d::kOrigin );
  newView->setLensLength( 50 );
  newView->setViewDirection( OdGeVector3d::kZAxis );
	newView->setIsPaperspaceView( true );
	return WT_Result::Success;
}

WT_Result DwfCallbackManager::process_view(WT_View& view, WT_File& file )
{
  WT_View::default_process( view, file );
  DwfImporter* imp = importer(file);
  DwfExtentManager& mngExtent = imp->extent();
  if (mngExtent._collectBounds)
    return WT_Result::Success;
  
  OdDbLayoutPtr layout = imp->blocks().currentLayout(OdDb::kForWrite);

  if ( !mngExtent.isValid() )
    return WT_Result::Success;

  WT_Logical_Box box = view.view();
	OdGePoint3d maxpt = mngExtent.transformPoint(box.maxpt());
	OdGePoint3d minpt = mngExtent.transformPoint(box.minpt());
	OdGePoint3d center((maxpt + minpt.asVector())/2);
	
  OdDbObjectId idBlock = layout->getBlockTableRecordId();

  if (idBlock == imp->database()->getModelSpaceId())
  {
    OdDbViewportTablePtr t = imp->database()->getViewportTableId().safeOpenObject();
    OdDbViewportTableRecordPtr vp = t->getActiveViewportId().safeOpenObject(OdDb::kForWrite);
    vp->setHeight(fabs(maxpt.y - minpt.y));
    vp->setWidth(fabs(maxpt.x - minpt.x));
    vp->setCenterPoint(OdGePoint2d(center.x, center.y));
  }
  else
  {
  	OdDbObjectId vpId = layout->overallVportId();
    // if this is newly created layout, it may contain no overall viewport
    if ( vpId.isNull() ) vpId = imp->blocks().addViewport();

	  OdDbViewportPtr vp = vpId.safeOpenObject(OdDb::kForWrite);
    vp->setHeight(fabs(maxpt.y - minpt.y));
    vp->setWidth(fabs(maxpt.x - minpt.x));
    vp->setCenterPoint(center);
    vp->setViewHeight(vp->height());
    vp->setViewCenter(OdGePoint2d(center.x, center.y));
    layout->setActiveViewportId(vpId);
  }

  return WT_Result::Success;
}


WT_Result DwfCallbackManager::process_filled_ellipse(WT_Filled_Ellipse& ellipse, WT_File& file)
{
  DwfExtentManager& mngExtent = importer(file)->extent();
  if (mngExtent._collectBounds)
  {
    mngExtent.updateBounds(ellipse, file);
    return WT_Result::Success;
  }
	OdDbHatchPtr pHatch = importer(file)->blocks().addHatch(file);
	OdGePoint3d c = mngExtent.transformPoint(ellipse.position());
	OdGePoint2d center(c.x, c.y);
	double a = ellipse.tilt_radian();
	OdGeVector2d ax1(cos(a), sin(a));
	OdGeVector2d ax2(cos(a + OdaPI2), sin(a + OdaPI2));
	
	OdGeEllipArc2d* cirArc = new OdGeEllipArc2d(center, ax1, ax2,
                                              mngExtent.transformSize(ellipse.ellmajor()),
                                              mngExtent.transformSize(ellipse.ellminor()));

	EdgeArray edgePtrs;
	edgePtrs.push_back(cirArc);
	pHatch->appendLoop(OdDbHatch::kDefault, edgePtrs);
	return WT_Result::Success;
}

static WT_Result addPoint( DwfImporter* importer, const WT_Logical_Point& p, WT_File& file )
{
  WT_Logical_Point pp[2];
  pp[0] = pp[1] = p;
  WT_Point_Set pl( 2, pp, WD_False );
  importer->lines().addPolyline( pl, file );
  return WT_Result::Success;
}

static WT_Result addFlatEllipse( DwfImporter* importer, const WT_Ellipse& ell, WT_File& file )
{
  WT_Logical_Point pp[2];
  pp[0] = pp[1] = ell.position();
  int dx = int( cos( ell.tilt_radian() ) * ell.ellmajor() );
  int dy = int( sin( ell.tilt_radian() ) * ell.ellmajor() );
  pp[0].m_x -= dx;
  pp[0].m_y -= dy;
  pp[1].m_x += dx;
  pp[1].m_y += dy;
  WT_Point_Set pl( 2, pp, WD_False );
  importer->lines().addPolyline( pl, file );
  return WT_Result::Success;
}

WT_Result DwfCallbackManager::process_outline_ellipse(WT_Outline_Ellipse& ell, WT_File& file)
{
  WT_Outline_Ellipse::default_process( ell, file );
  DwfExtentManager& mngExtent = importer(file)->extent();
  if (mngExtent._collectBounds)
  {
    mngExtent.updateBounds( ell, file );
    return WT_Result::Success;
  }
  
  if ( ell.ellmajor() == 0 ) return addPoint( importer(file), ell.position(), file );
  if ( ell.ellminor() == 0 ) return addFlatEllipse( importer(file), ell, file );
  
	OdGePoint3d c = mngExtent.transformPoint(ell.position());
  double a = ell.tilt_radian();
	double major = mngExtent.transformSize(ell.ellmajor());
	double minor = mngExtent.transformSize(ell.ellminor());
  bool bCorrectAxes = fabs(major) < fabs(minor);
  bool fullEllipse = ( ( ell.end() - ell.start() ) % 65536 ) == 0;
  double startAngle = fullEllipse ? 0. : ( Oda2PI * ell.start() / 65536. );
  double endAngle = fullEllipse ? Oda2PI : ( Oda2PI * ell.end() / 65536. );
  if (bCorrectAxes)
  {
    std::swap(major, minor);
    a += OdaPI2;
    if (!fullEllipse)
    {
      startAngle -= OdaPI2;
      endAngle -= OdaPI2;
    }
  }

  if (ell.ellmajor() == ell.ellminor())
  {
    if (fullEllipse)
    {
      OdDbCirclePtr circ = OdDbCircle::createObject();
      importer(file)->blocks().addEntity(circ, file);
      circ->setCenter(c);
      circ->setRadius(major);
    }
    else
    {
      OdDbArcPtr arc = OdDbArc::createObject();
      importer(file)->blocks().addEntity(arc, file);
      arc->setCenter(c);
      arc->setRadius(major);
      arc->setStartAngle(startAngle + a);
      arc->setEndAngle(endAngle + a);
    }
  }
  else
  {
    OdDbEllipsePtr pEllipse = OdDbEllipse::createObject();
    importer(file)->blocks().addEntity(pEllipse, file);
    pEllipse->set(c, OdGeVector3d::kZAxis, // Center, normal,
      OdGeVector3d( cos(a)*major, sin(a)*major, 0 ), // Major axis,
      fabs(minor / major)); // Radius ratio
    // start & end angles
    pEllipse->setStartParam(startAngle);
    pEllipse->setEndParam(endAngle);
  }
	return WT_Result::Success;
}

static int image_count = 0xffff;
WT_Result DwfCallbackManager::process_image(WT_Image& image, WT_File& file)
{
  DwfExtentManager& mngExtent = importer(file)->extent();
  if (mngExtent._collectBounds)
  {
    mngExtent.updateBounds(image, file);
    return WT_Result::Success;
  }
  if (image.format() == WT_Image::Group3X_Mapped)
  {
    image.convert_group_3X_to_mapped();
  }
	importer(file)->images().addImage(image.data(), image.data_size(), 
                                    image.columns(), image.rows(), // TOCHECK maybe it is in reverse order too (see CORE-13332) 
                                    image.format(), image.color_map(), image.min_corner(), image.max_corner(), 
                                    image.identifier() != 0 ? image.identifier() : image_count++, file);
	return WT_Result::Success;
}

WT_Result DwfCallbackManager::process_png_group4_image(WT_PNG_Group4_Image& image, WT_File& file)
{
  DwfExtentManager& mngExtent = importer(file)->extent();
  if (mngExtent._collectBounds)
  {
    mngExtent.updateBounds(image, file);
    return WT_Result::Success;
  }

  OdUInt16 width = image.columns(), height = image.rows();
  double dWidth = image.max_corner().m_x - image.min_corner().m_x,
         dHeight = image.max_corner().m_y - image.min_corner().m_y;

  double dRatio = (dWidth > 0) ? (dHeight / dWidth) : 2,
         ratio = double(height) / width;
  bool bSwapped = ((dRatio > 1) != (ratio > 1));
  ODA_ASSERT_ONCE_X(TDWF,    bSwapped == (image.identifier() != 0) // image.m_identifier
                          || dwfImp::WTS2ODS(file.filename()).makeLower().find(L".dwfx") > 0); // Xps
  if (bSwapped)
    std::swap(width, height); // CORE-13332 CORE-13333

	importer(file)->images().addImage(image.data(), image.data_size(), 
                                    width, height,
                                    image.format(), image.color_map(), image.min_corner(), image.max_corner(), 
		                                image.identifier() != 0 ? image.identifier() : image_count++, file);
	return WT_Result::Success;
}

WT_Result DwfCallbackManager::process_layer(WT_Layer& layer, WT_File& file)
{
  DwfExtentManager& mngExtent = importer(file)->extent();
  if (mngExtent._collectBounds)
    return WT_Result::Success;
  file.rendition().layer() = layer;
	importer(file)->blocks().setCurrentLayer(layer);
	return WT_Result::Success;
}

WT_Result DwfCallbackManager::process_object_node(WT_Object_Node& node, WT_File& file)
{
  WT_Object_Node::default_process( node, file );
  DwfExtentManager& mngExtent = importer(file)->extent();
  if (mngExtent._collectBounds) return WT_Result::Success;
	importer(file)->blocks().setCurrentGroup(node.object_node_num(), 
    dwfImp::normalizeObjectName(node.object_node_name()));
	return WT_Result::Success;
}

// remove duplicate points from the array, for the hatch to load correctly (cr2909)
static void removeDuplicateVertices(OdGePoint2dArray& vertexPts)
{
  for (unsigned i = 0;; ++i)
  {
    if (i >= vertexPts.size()) break; // to avoid "optimization"
    OdGePoint2d p = vertexPts[i];
    for (unsigned j = i + 1;; ++j)
    {
      if (j >= vertexPts.size()) break; // to avoid "optimization"
      if (vertexPts[j] == p)
      {
        vertexPts.removeAt(j);
        --j;
      }
      else break; // remove only consequent vertices
    }
  }
}

WT_Result DwfCallbackManager::process_polygon(WT_Polygon& poly, WT_File& file)
{
  DwfExtentManager& mngExtent = importer(file)->extent();
  if (mngExtent._collectBounds)
  {
    mngExtent.updateBounds(poly, file);
    return WT_Result::Success;
  }
  // if degenerate geometry - do not make hatch
  if (poly.count() <= 2 || !file.rendition().fill().fill())
  {
    importer(file)->lines().addPolyline(poly, file);
    return WT_Result::Success;
  }
	
	OdGePoint2dArray vertexPts;
	OdGeDoubleArray vertexBulges;
	vertexPts.resize(poly.count());
	for (int i = 0; i < poly.count(); ++i)
	{
		OdGePoint3d p = mngExtent.transformPoint(poly.points()[i]);
		vertexPts[i].set(p.x, p.y);
	}
  removeDuplicateVertices(vertexPts);
  if (vertexPts.size() > 2)
  {
    OdDbHatchPtr pHatch = importer(file)->blocks().addHatch(file);
  	pHatch->appendLoop(OdDbHatch::kExternal | OdDbHatch::kPolyline, vertexPts, vertexBulges);
  }
	return WT_Result::Success;
}

WT_Result DwfCallbackManager::process_polymarker(WT_Polymarker& pm, WT_File& file)
{
  DwfExtentManager& mngExtent = importer(file)->extent();
  if (mngExtent._collectBounds)
  {
    mngExtent.updateBounds(pm, file);
    return WT_Result::Success;
  }
	for (int i = 0; i < pm.count(); ++i)
	{
		OdDbPointPtr pPoint = OdDbPoint::createObject();
		pPoint->setPosition(mngExtent.transformPoint(pm.points()[i]));
		importer(file)->blocks().addEntity(pPoint, file);
	}
	return WT_Result::Success;
}

WT_Result DwfCallbackManager::process_polytriangle(WT_Polytriangle& pt, WT_File& file)
{
  DwfExtentManager& mngExtent = importer(file)->extent();
  if (mngExtent._collectBounds)
  {
    mngExtent.updateBounds(pt, file);
    return WT_Result::Success;
  }
	// polytiangle is filled - create hatch
	OdGePoint2dArray vertexPts;
	OdGeDoubleArray vertexBulges;
	vertexPts.reserve(pt.count());
	// make contour from triangle strip
	// even points - one side
	int i = 0;
	for (; i < pt.count(); i += 2)
	{
		OdGePoint3d p = mngExtent.transformPoint(pt.points()[i]);
		vertexPts.push_back(OdGePoint2d(p.x,p.y));
	}
	// odd points, in reversed order - other side
	--i;
  if (i >= pt.count())
    i -= 2;
	for (; i > 0; i-= 2)
	{
		OdGePoint3d p = mngExtent.transformPoint(pt.points()[i]);
		vertexPts.push_back(OdGePoint2d(p.x,p.y));
	}
  removeDuplicateVertices(vertexPts);
  if (vertexPts.size() > 2)
  {
    OdDbHatchPtr pHatch = importer(file)->blocks().addHatch(file);
  	pHatch->appendLoop(OdDbHatch::kExternal | OdDbHatch::kPolyline, vertexPts, vertexBulges);
  }
	return WT_Result::Success;	
}

WT_Result DwfCallbackManager::process_contour_set(WT_Contour_Set& cs, WT_File& file)
{
  DwfExtentManager& mngExtent = importer(file)->extent();
  if (mngExtent._collectBounds)
  {
    mngExtent.updateBounds(cs, file);
    return WT_Result::Success;
  }
	OdDbHatchPtr pHatch = importer(file)->blocks().addHatch(file);
	OdGeDoubleArray vertexBulges;
	int n = 0; // pass through index
	for (int i = 0; i < cs.contours(); i++)
	{
		OdGePoint2dArray vertexPts;
		vertexPts.resize(cs.counts()[i]);
		for (int j = 0; j < cs.counts()[i]; j++)
		{
			OdGePoint3d p = mngExtent.transformPoint(cs.points()[n++]);
			vertexPts[j].set(p.x, p.y);
		}
    removeDuplicateVertices(vertexPts);
    if (vertexPts.size() > 2)
  		pHatch->appendLoop(OdDbHatch::kExternal | OdDbHatch::kPolyline, vertexPts, vertexBulges);
	}
	ODA_ASSERT(n == cs.total_points());
	return WT_Result::Success;
}

static bool operator >(const WT_RGBA32& c1, const WT_RGBA32& c2)
{
  if (c1.m_rgb.r > c2.m_rgb.r)
    return true;
  if (c1.m_rgb.r < c2.m_rgb.r)
    return false;
  if (c1.m_rgb.g > c2.m_rgb.g)
    return true;
  if (c1.m_rgb.g < c2.m_rgb.g)
    return false;
  return c1.m_rgb.b > c2.m_rgb.b;
}
// bubble sort
static void sort_points(WT_Logical_Point pts[],  WT_RGBA32 cols[])
{
  if (cols[0] > cols[1])
  {
    std::swap(cols[0], cols[1]);
    std::swap(pts[0], pts[1]);
  }
  if (cols[1] > cols[2])
  {
    std::swap(cols[1], cols[2]);
    std::swap(pts[1], pts[2]);
  }
  if (cols[0] > cols[1])
  {
    std::swap(cols[0], cols[1]);
    std::swap(pts[0], pts[1]);
  }
}

// _units matrix must be uniscale ortho, but due to tolerance issues it may be somewhat different (and Z axis may be 1.0 length)
static void normalizeMatrix(OdGeMatrix3d& m)
{
  OdGeVector3d x, y, z;
  OdGePoint3d origin;
  m.getCoordSystem(origin, x, y, z);
  double xLen = x.length();
  double yLen = y.length();
  double zLen = z.length();
  if (!OdZero(zLen) && !OdZero(yLen))
  {
     z *= xLen/zLen;
     y *= xLen/yLen;
     m.setCoordSystem(origin, x, y, z);
  }
}
WT_Result DwfCallbackManager::process_gouraud_polytriangle(WT_Gouraud_Polytriangle& gpt, WT_File& file)
{
  DwfExtentManager& mngExtent = importer(file)->extent();
  if (mngExtent._collectBounds)
  {
    mngExtent.updateBounds(gpt, file);
    return WT_Result::Success;
  }
  if (!(bool)OdRxVariantValue(importer(file)->properties()->getAt(L"ProcessGradients")))
    return WT_Result::Success;
  if (gpt.count() != 3)
  {
    ODA_FAIL_ONCE();
    return WT_Result::Success;
  }
  OdDbHatchPtr h = OdDbHatch::createObject();
  h->setDatabaseDefaults(importer(file)->database());
  WT_Logical_Point pts[3] = { gpt.points()[0], gpt.points()[1], gpt.points()[2] };
  WT_RGBA32 cols[3] = {gpt.colors()[0], gpt.colors()[1], gpt.colors()[2]};
  sort_points(pts, cols);
  OdGePoint2dArray verts;
  verts.append(OdGePoint2d(pts[0].m_x, pts[0].m_y));
  verts.append(OdGePoint2d(pts[1].m_x, pts[1].m_y));
  verts.append(OdGePoint2d(pts[2].m_x, pts[2].m_y));
  OdGeDoubleArray bulges;
  bulges.insert(bulges.begin(), 3, 0);
  h->appendLoop(OdDbHatch::kDefault|OdDbHatch::kPolyline, verts, bulges);
  if (cols[0].m_whole == cols[2].m_whole)
  {
    h->setPattern(OdDbHatch::kPreDefined, L"SOLID");
    // create solid fill hatch;
    OdCmColor c;
    c.setRGB(cols[0].m_rgb.r, cols[0].m_rgb.g, cols[0].m_rgb.b);
    h->setColor(c);
  }
  else
  {
    // gradient angle
    double a = 0; 
    // if some of the 3 colors are coincident, then the gradient is orthogonal 
    // to the line passing through the corresponding points
    if (cols[0].m_whole == cols[1].m_whole)
    {
      a = OD_ATAN2(double(pts[1].m_y - pts[0].m_y), double(pts[1].m_x - pts[0].m_x)) + OdaPI2;
    }
    else if (cols[1].m_whole == cols[2].m_whole)
    {
      a = OD_ATAN2(double(pts[2].m_y - pts[1].m_y), double(pts[2].m_x - pts[1].m_x)) + OdaPI2;
    }
    else // the most generic case
    {
      double Kc = 1.0;
      // we assume that the original gradient was bicolor, and we may interpolate by any color component
      if (cols[2].m_rgb.r != cols[1].m_rgb.r)
      {
        Kc = double(cols[2].m_rgb.r - cols[0].m_rgb.r)/(cols[2].m_rgb.r - cols[1].m_rgb.r);
      }
      else if (cols[2].m_rgb.g != cols[1].m_rgb.g)
      {
        Kc = double(cols[2].m_rgb.g - cols[0].m_rgb.g)/(cols[2].m_rgb.g - cols[1].m_rgb.g);
      }
      else if (cols[2].m_rgb.b != cols[1].m_rgb.b)
      {
        Kc = double(cols[2].m_rgb.b - cols[0].m_rgb.b)/(cols[2].m_rgb.b - cols[1].m_rgb.b);
      }
      else
      {
        ODA_FAIL_ONCE(); // seems to be impossible, because we test colors on coincidence 
        return WT_Result::Success;
      }
      double Y = Kc*(pts[2].m_y - pts[1].m_y) - (pts[2].m_y - pts[0].m_y);
      double X = (pts[2].m_x - pts[0].m_x) - Kc*(pts[2].m_x - pts[1].m_x);
      a = OD_ATAN2(X, Y);
    }
    h->setHatchObjectType(OdDbHatch::kGradientObject);
    h->setGradient(OdDbHatch::kPreDefinedGradient, L"LINEAR");
    OdCmColor gradientColors[2];
    gradientColors[0].setRGB(cols[2].m_rgb.r, cols[2].m_rgb.g, cols[2].m_rgb.b);
    gradientColors[1].setRGB(cols[0].m_rgb.r, cols[0].m_rgb.g, cols[0].m_rgb.b);
    OdGeVector2d direction(cos(a), sin(a));
    if (OdGeVector2d(pts[2].m_x, pts[2].m_y).dotProduct(direction) > OdGeVector2d(pts[0].m_x, pts[0].m_y).dotProduct(direction))
      std::swap(gradientColors[1], gradientColors[0]);
    double gradientValues[2] = {0.0, 1.0};
    h->setGradientColors(2, gradientColors, gradientValues);
    h->setGradientAngle(a);
  }
  importer(file)->blocks().addEntity(h, file);
  
  if (!mngExtent._useUnits) // gradient in the old file?
  {
    ODA_FAIL_ONCE();
  }
  OdGeMatrix3d m(mngExtent._units);
  normalizeMatrix(m);
  h->transformBy(m);
	return WT_Result::Success;
}

WT_Result DwfCallbackManager::process_font(WT_Font& font, WT_File& file)
{
  WT_Font::default_process( font, file );
  if (importer(file)->extent()._collectBounds)
    return WT_Result::Success;
	importer(file)->fonts().setFontStyle(file.rendition().font());
	return WT_Result::Success;
}

namespace txt
{
  void setScoring(OdString& str, WT_Font& font, const OdChar* underscore, const OdChar* overscore)
  {
    //???
    // according to whip specs, scoring may be really complex, 
    // but i've never saw one, and specs are not clear enough to write code
    // so we import obvious cases
    // (anyway, Autocad never exports scoring)
    if (font.style().underlined() || font.flags() & 8) 
      str = OdString(underscore) + str;
    if (font.flags() & 0x10) str = OdString(overscore) + str;
  }
  
  void setSpacing(OdString& str, WT_Font& font)
  {
    // DWF spacing is much more flexible - from 1/1024 to 64
    // DWG allows only 0.75 - 4
    if (font.spacing() == WT_Font_Option_Spacing(1024))
      return;
    double spacing = double(font.spacing()) / 1024;
    if (spacing > 4) spacing = 4;
    if (spacing < 0.75) spacing = 0.75;
    OdChar buf[20];
    odSprintf(buf, 20, OD_T("\\T%.3g;"), spacing);
    str = buf + str;
  }

  void setOblique(OdString& str, WT_Font& font)
  {
    if (font.oblique().oblique() == 0) return;
    OdChar buf[20];
    odSprintf(buf, 20, OD_T("\\Q%d;"), int(360. / (2<<15) * font.oblique()));
    str = OdString(buf) + str;
  }
  
  void setWidthFactor(OdString& str, WT_Font& font)
  {
    if (font.width_scale().width_scale() == 1024) return;
    OdChar buf[20];
    odSprintf(buf, 20, OD_T("\\W%.3g;"), 1. / 1024 * font.width_scale());
    str = buf + str;
  }
}

static double od_distance( const WT_Logical_Point& p1, const WT_Logical_Point& p2 )
{
  return sqrt( double(p2.m_y - p1.m_y)*(p2.m_y - p1.m_y) + double(p2.m_x - p1.m_x)*(p2.m_x - p1.m_x) );
}

static OdString obliqueFormat(double oblique)
{
  OdString oblFormat;
  oblFormat.format(L"\\Q%g;", oblique*180/OdaPI);
  return oblFormat;
}
static OdString widthFormat(double dWidthFactor)
{
  OdString wdFormat;
  wdFormat.format(L"\\W%g;", dWidthFactor);
  return wdFormat;
}

static void adjustTextByBounds( DwfImporter* importer, OdDbMTextPtr pText, const WT_Logical_Point* bounds )
{
  double horz = OD_ATAN2( double(bounds[1].m_y - bounds[0].m_y), double(bounds[1].m_x - bounds[0].m_x) );
  pText->setRotation( horz );
  double vert = OD_ATAN2( double(bounds[3].m_y - bounds[0].m_y), double(bounds[3].m_x - bounds[0].m_x) );
  double oblique = OdaPI2 - (vert - horz);
  // normalize angle
  if (oblique > OdaPI)
    oblique -= Oda2PI;
  else if (oblique < -OdaPI)
    oblique += Oda2PI;
  // check for upside-down
  if (oblique < -OdaPI2 || oblique > OdaPI2)
  {
    horz += OdaPI;
    pText->setRotation(horz);
    if (oblique > 0)
      oblique -= OdaPI;
    else
      oblique += OdaPI;
    pText->setContents(obliqueFormat(oblique) + pText->contents() );
  }
  else
    pText->setContents(obliqueFormat(oblique) + pText->contents());

  double h = importer->fonts().getTextHeight( od_distance(bounds[3],bounds[0]) * sin(vert-horz) );
  pText->setTextHeight(h);

  double w = importer->extent().transformSize( od_distance(bounds[1],bounds[0]) );
  if (!OdZero(pText->actualWidth()))
  {
    pText->setContents(widthFormat(w / pText->actualWidth()) + pText->contents());
  }
}

WT_Result DwfCallbackManager::process_text(WT_Text& text, WT_File& file)
{
  DwfImporter* imp = importer(file);
  if (imp->extent()._collectBounds) 
  {
    imp->extent().updateBounds(text, file);
    return WT_Result::Success;
  }

  OdString str = dwfImp::WTS2ODS(text.string());
  OdGePoint3d ptPos = imp->extent().transformPoint(text.position());

  OdDbMTextPtr pText = OdDbMText::createObject();
  pText->setAttachment(OdDbMText::kBottomLeft);
  double h = imp->fonts().getTextHeight(file.rendition().font().height().height());
  if (!OdZero(h))
    pText->setTextHeight(h);
  if ((file.rendition().font().flags() & FLAG_PATH_TEXT) == 0) // PATH_TEXT
    pText->setRotation(Oda2PI / (2<<15) * file.rendition().font().rotation().rotation());
  OdGeVector3d offset;
  OdDbObjectId tsId = imp->database()->getTEXTSTYLE();
  OdGiTextStyle ts;
  giFromDbTextStyle(tsId, ts);
  if (ts.isTtfFont())
  {
    // ptPos is at a baseline. Need to shift it to the bottom
    OdStaticRxObject<OdGiContextForDbDatabase> giContext;
    giContext.setDatabase( imp->database(), false );
    OdGePoint3d ptMin, ptMax;
    ts.setTextSize(h);
    giContext.textExtentsBox(ts, str.c_str(), str.getLength(), kOdGiRawText, ptMin, ptMax);
    offset.y = ptMin.y;

    WT_Text_HAlign::WT_HAlign halign = file.rendition().text_halign().hAlign();
    if (halign != WT_Text_HAlign::Left)
    {
      const WT_Logical_Box& bnds = ((WT_Drawable*)&text)->bounds(&file);
      double width_ = imp->extent().transformPoint(bnds.m_max).x - imp->extent().transformPoint(bnds.m_min).x;
      double width = ptMax.x - ptMin.x; // bnds.m_max.m_x - bnds.m_min.m_x;
      //width *= ascentRatio;
      ODA_ASSERT_ONCE_X(TDWF, width > 0);
      if (width > 0)
      {
        switch (halign)
        {
        case WT_Text_HAlign::Center:
          offset.x = - width / 2; break; // CORE-13304
        case WT_Text_HAlign::Right:
          offset.x = - width; break;
        }
      }
    }
    WT_Text_VAlign::WT_VAlign valign = file.rendition().text_valign().vAlign();
    if (valign != WT_Text_VAlign::Baseline)
    {
      ////const WT_Logical_Box& bnds = ((WT_Drawable*)&text).bounds(&file);
      ////double height_ = imp->extent().transformPoint(bnds.m_max).y - imp->extent().transformPoint(bnds.m_min).y;
      //double height = ptMax.y - ptMin.y;
      ////height *= ascentRatio;
      //ODA_ASSERT_ONCE_X(TDWF, height > 0);
      //if (height > 0)
      //{
        ODA_FAIL_ONCE_X(TDWF); // TODO
      //}
    }
  }

  pText->setTextStyle(tsId);
  txt::setScoring(str, file.rendition().font(), OD_T("\\L"), OD_T("\\O"));
  txt::setSpacing(str, file.rendition().font());
  const WT_Logical_Point* bounds = text.bounds().bounds();

  if ((file.rendition().font().flags() & FLAG_PATH_TEXT) && bounds
    && (bounds[0].m_x != bounds[1].m_x || bounds[0].m_y != bounds[1].m_y)
    && (bounds[0].m_x != bounds[3].m_x || bounds[0].m_y != bounds[3].m_y))
  {
    pText->setContents(str);
    adjustTextByBounds( imp, pText, bounds );
  }
  else
  {
    txt::setOblique(str, file.rendition().font());
    txt::setWidthFactor(str, file.rendition().font());
    pText->setContents(str);
  }
  offset.rotateBy(pText->rotation(), OdGeVector3d::kZAxis);
  ptPos += offset;
  pText->setLocation(ptPos);

  imp->blocks().addEntity(pText, file);

	return WT_Result::Success;
}

WT_Result DwfCallbackManager::process_viewport( WT_Viewport& vp, WT_File& file )
{
  WT_Viewport::default_process( vp, file );
  DwfImporter* imp = importer(file);
  DwfExtentManager& mngExtent = imp->extent();
  if (mngExtent._collectBounds && vp.contour() != 0)
  {
    mngExtent.updateBounds(const_cast<WT_Contour_Set&>(*vp.contour()), file);
    if (vp.contour() != 0) 
    {
      mngExtent.setClipRect(const_cast<WT_Contour_Set&>(*vp.contour()).bounds());
    }
    else
    {
      mngExtent.setClipRect(WT_Logical_Box(0,0,0,0));
    }
    return WT_Result::Success;
  }
  
  OdDbLayoutPtr layout = imp->blocks().currentLayout();
  OdDbObjectId idLayoutBlockTableRec = layout->getBlockTableRecordId();
  OdDbBlockTableRecordPtr pBlock = imp->blocks().currentBlock();
  if (pBlock.get() && pBlock->objectId() != idLayoutBlockTableRec)
  {
    // CORE-8033
    OdDbPolylinePtr pPolyline;
    OdDbHatchPtr pHatch;
    bool areOtherMissing = true;
    for (OdDbObjectIteratorPtr pItr = pBlock->newIterator(); !pItr->done(); pItr->step())
    {
      if (pPolyline.isNull() && (pPolyline = OdDbPolyline::cast(pItr->entity())).get())
        continue;
      if (pHatch.isNull() && (pHatch = OdDbHatch::cast(pItr->entity())).get())
        continue;
      areOtherMissing = false;
      break;
    }
    if (   areOtherMissing && pPolyline.get() && pHatch.get()
        && pHatch->patternType() == OdDbHatch::kPreDefined
        && pHatch->patternName() == L"SOLID")
    {
      // only clip polyline and solid hatch are here
      pPolyline->upgradeOpen();
      pPolyline->erase();
      pHatch->upgradeOpen();
      //pHatch->setHatchStyle(OdDbHatch::kNormal);
      pHatch->erase();
    }
  }

	// DWF viewport corresponds to the DWG block with clip object attached
	importer(file)->blocks().setCurrentBlock(idLayoutBlockTableRec);
  // making new current block we override grouping
	importer(file)->blocks().setCurrentGroup(-1);
  // "(Viewport)" opcode => closing viewport
	if (vp.contour() == 0) return WT_Result::Success;

  // else create block
	OdDbBlockTableRecordPtr block = OdDbBlockTableRecord::createObject();
	OdDbBlockTablePtr pTable = importer(file)->database()->getBlockTableId().safeOpenObject(OdDb::kForWrite);
  {
    static int index = 1;
    block->setName(dwfImp::generateName(OD_T("Viewport"), pTable, index));
  }
	OdDbObjectId blockId = pTable->add(block);

  // add insert to current layout
	OdDbBlockReferencePtr pBlkRef = OdDbBlockReference::createObject();
  // make insert visible
  WT_Visibility oldVisibility = file.rendition().visibility();
  file.rendition().visibility() = WD_True;
  importer(file)->blocks().addEntity(pBlkRef, file);
  file.rendition().visibility() = oldVisibility;
  pBlkRef->setColorIndex( 7 ); // viewport does not use color
	pBlkRef->setBlockTableRecord(blockId);
  pBlkRef->setScaleFactors(OdGeScale3d::kIdentity);
  pBlkRef->setPosition(OdGePoint3d::kOrigin);

  importer(file)->blocks().setCurrentBlock(block);

  // add clip object
	OdDbSpatialFilterPtr filter = OdDbSpatialFilter::createObject();
	OdGePoint2dArray bounds;
  int count = vp.contour()->counts()[0];
    // in dwg first and last point of the boundary must not coincide
  if (count > 2 && vp.contour()->points()[0] == vp.contour()->points()[count - 1])
    --count;
  bounds.resize(count);
  for (int i = 0; i < count; i++)
  {
    OdGePoint3d p = mngExtent.transformPoint(vp.contour()->points()[i]);
    bounds[i].set(p.x, p.y); 
  }
  filter->setDefinition(bounds);
	OdDbIndexFilterManager::addFilter(pBlkRef, filter);

  return WT_Result::Success;
}

WT_Result DwfCallbackManager::process_polyline(WT_Polyline& pl, WT_File& file)
{
  DwfExtentManager& mngExtent = importer(file)->extent();
  if (mngExtent._collectBounds)
  {
    mngExtent.updateBounds(pl, file);
    return WT_Result::Success;
  }
  importer(file)->lines().addPolyline(pl, file);
	return WT_Result::Success;
}

WT_Result DwfCallbackManager::process_gouraud_polyline(WT_Gouraud_Polyline& gpl, WT_File& file)
{
  DwfExtentManager& mngExtent = importer(file)->extent();
  if (mngExtent._collectBounds)
  {
    mngExtent.updateBounds(gpl, file);
    return WT_Result::Success;
  }
  // if polytriangles are processed then their bounds are ignored
  if ((bool)OdRxVariantValue(importer(file)->properties()->getAt(L"ProcessGradients")))
    return WT_Result::Success;
  if (gpl.count() == 0)
    return WT_Result::Success;
	OdDb2dPolylinePtr p2dPl = OdDb2dPolyline::createObject();
	importer(file)->blocks().addEntity(p2dPl, file);
	for (int i = 0; i < gpl.count(); i++)
	{
		OdDb2dVertexPtr pV = OdDb2dVertex::createObject();
    OdInt32 rgb = gpl.colors()[i].m_whole;
    OdCmColor c;
    c.setRGB(ODGETRED(rgb), ODGETGREEN(rgb), ODGETBLUE(rgb));
    pV->setColor(c);
		pV->setPosition(mngExtent.transformPoint(gpl.points()[i]));
    p2dPl->appendVertex(pV);
	}
	return WT_Result::Success;
}

WT_Result DwfCallbackManager::process_fill_pattern(WT_Fill_Pattern& fp, WT_File& file)
{
  if (importer(file)->extent()._collectBounds) return WT_Result::Success;
  // default processing does not do that, why?
  file.rendition().fill_pattern() = fp;
	return WT_Result::Success;
}

bool isPaperAccordant(double wPaper, double hPaper,
                      double resPaperWidth, double resPaperHeight, // via layout->getPlotPaperSize
                      double dCoeff = 0.01,
                      double dCoeffY = 0.0); // 0.0 means to use dCoeff

WT_Result DwfCallbackManager::process_plot_info(WT_Plot_Info& pi, WT_File& file )
{
  WT_Plot_Info::default_process( pi, file );
  DwfImporter* imp = importer(file);
  DwfExtentManager& mngExtent = imp->extent();
  if (!mngExtent._collectBounds)
    return WT_Result::Success;
  //OdGePoint3d lowerLeft(pi.lower_left().m_x, pi.lower_left().m_y, 0.0),
  //            upperRight(pi.upper_right().m_x, pi.upper_right().m_y, 0.0);
  double paperWidth = pi.paper_width(),
         paperHeight = pi.paper_height();
  OdGePoint3d ptCenter((pi.lower_left().m_x + pi.upper_right().m_x) / 2.0,
                       (pi.lower_left().m_y + pi.upper_right().m_y) / 2.0, 0.0);
  OdDbPlotSettings::PlotPaperUnits paperUnits = pi.paper_units() == WT_Plot_Info::Inches ? OdDbPlotSettings::kInches : OdDbPlotSettings::kMillimeters;

//#if 0
//  if (paperUnits == OdDbPlotSettings::kInches && !imp->currentLayoutName().find(L"Model"))
//  {
//    double dScale = (mngExtent._units).scale();
//    paperWidth *= dScale;
//    paperHeight *= dScale;
//    ptCenter *= dScale;
//  }
//#else
  if (paperUnits == OdDbPlotSettings::kInches && mngExtent._paperInfo._units == OdDbPlotSettings::kInches)
  // CORE-11881 problem with back exported DWF // && !imp->currentLayoutName().find(L"Model"))
  {
    double width = paperWidth / kMmPerInch,
           height = paperHeight / kMmPerInch;
    if (isPaperAccordant(width, height, mngExtent._paperInfo._width, mngExtent._paperInfo._height, 0.1))
    {
      // CORE-10319 problem with BoxRed_23_renamed_1.dwf
      paperWidth = width;
      paperHeight = height;
      ptCenter /= kMmPerInch;
    }
  }
//#endif

  mngExtent.setPaperInfo(paperWidth, paperHeight, ptCenter, paperUnits);
  return WT_Result::Success;
}

WT_Result DwfCallbackManager::process_background( WT_Background& bg, WT_File& file )
{
  WT_Background::default_process( bg, file );
  if ( !importer(file)->extent()._collectBounds )
  {
    // alpha channel is zeroed for WinGDI
    importer(file)->properties()->putAt( OD_T("Background"), OdRxVariantValue( 
      dwfImp::exchangeRBColorComponents(OdUInt32(bg.color().rgba().m_whole & 0xffffff)) ) );
  }
  return WT_Result::Success;
}
}
