/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2017, Open Design Alliance (the "Alliance"). 
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
//   Teigha(R) Copyright (C) 2002-2017 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 
// acknowledge and accept the above terms.
///////////////////////////////////////////////////////////////////////////////

/************************************************************************/
/* This console application reads a DWG/DXF file and saves it to        */
/* a PDF file with the specified export options                         */
/*                                                                      */
/*                                                                      */
/* Calling sequence:                                                    */
/*                                                                      */
/*    OdPdfExportEx <source file> <target file> <options>               */
/*                                                                      */
/************************************************************************/


#ifdef WD_WIN_SYSTEM
#include <io.h>
#include <direct.h>
#endif

#include <stdlib.h>
#include <algorithm>
#include <locale.h>

#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std:: a

#include "OdaCommon.h"
#include "ExSystemServices.h"
#include "RxDynamicModule.h"

#include "DbDatabase.h"
#include "DgDatabase.h"
#include "DbBlockTableRecord.h"
#include "DbDictionary.h"
#include "DbPlotSettingsValidator.h"

#include "PdfExport.h"

#ifdef ODA_WINDOWS
#include <tchar.h>
#endif

#ifdef OD_HAVE_CONSOLE_H_FILE
#include <console.h>
#endif

#include <string>
using namespace std;

using namespace TD_PDF_2D_EXPORT;


#ifndef _TOOLKIT_IN_DLL_

/************************************************************************/
/* Define a module map for statically linked modules:                   */
/************************************************************************/

ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgnModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgn7IOModuleImpl);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ModelerModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRecomputeDimBlockModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(RasterExportModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(BitmapModule);
#if defined(OD_HAS_OPENGL)
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(WinOpenGLModule);
#endif
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ExRasterModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdPsPlotStyleServicesImpl);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdPlotSettingsValidatorModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRasterProcessingServicesImpl);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdPdfExportModule);

ODRX_BEGIN_STATIC_MODULE_MAP()
ODRX_DEFINE_STATIC_APPLICATION(L"TG_Db", OdDgnModule)
ODRX_DEFINE_STATIC_APPLICATION(L"TG_Dgn7IO", OdDgn7IOModuleImpl)
ODRX_DEFINE_STATIC_APPMODULE(OdModelerGeometryModuleName, ModelerModule)
ODRX_DEFINE_STATIC_APPLICATION(OdRecomputeDimBlockModuleName, OdRecomputeDimBlockModule)
ODRX_DEFINE_STATIC_APPMODULE(OdRasterExportModuleName, RasterExportModule)
ODRX_DEFINE_STATIC_APPMODULE(OdWinBitmapModuleName, BitmapModule)
#if defined(OD_HAS_OPENGL)
ODRX_DEFINE_STATIC_APPMODULE(OdWinOpenGLModuleName, WinOpenGLModule)
#endif
ODRX_DEFINE_STATIC_APPMODULE(RX_RASTER_SERVICES_APPNAME, ExRasterModule)
ODRX_DEFINE_STATIC_APPMODULE(OdPlotSettingsValidatorModuleName, OdPlotSettingsValidatorModule)
ODRX_DEFINE_STATIC_APPLICATION(ODPS_PLOTSTYLE_SERVICES_APPNAME, OdPsPlotStyleServicesImpl)
ODRX_DEFINE_STATIC_APPMODULE(OdRasterProcessorModuleName, OdRasterProcessingServicesImpl)
ODRX_DEFINE_STATIC_APPMODULE(OdPdfExportModuleName, OdPdfExportModule)
ODRX_END_STATIC_MODULE_MAP()

#endif

//----------------------------------------------------------------------------------


void SetPdfExportParam(PDFExportParams& params, OdDbDatabasePtr& pDb, const OdStreamBufPtr& pBuffer);
void SetPdfExportParam(PDFExportParams& params, OdDgDatabasePtr& pDb, const OdStreamBufPtr& pBuffer);

#if defined(OD_USE_WMAIN)
int wmain(int argc, wchar_t* argv[])
#else
int main(int argc, char* argv[])
#endif
{
#ifdef OD_HAVE_CCOMMAND_FUNC
	argc = ccommand(&argv);
#endif

	if (argc > 2)
	{
		setlocale(LC_ALL, "");
		setlocale(LC_NUMERIC, "C");

#ifndef _TOOLKIT_IN_DLL_
		ODRX_INIT_STATIC_MODULE_MAP();
#endif

		OdStaticRxObject<ExSystemServices> svcs;
		odInitialize(&svcs);
		OdStaticRxObject<MyDwgServices> dwgSvcs;
		OdStaticRxObject<MyDgnServices> dgnSvcs;

		OdString inputFile = argv[1];
		if (inputFile.getLength() < 4)
			return -1;
		wstring ext = inputFile.right(4);
		transform(ext.begin(), ext.end(), ext.begin(), towlower);
		bool bDwg;
		if (ext.compare(L".dwg") == 0 || ext.compare(L".dxf") == 0)
			bDwg = true;
		else if (ext.compare(L".dgn") == 0)
			bDwg = false;
		else
			return -1;
		OdString outputFile = argv[2];

		try
		{
			OdPdfExportModulePtr pModule = ::odrxDynamicLinker()->loadApp(OdPdfExportModuleName);
			OdPdfExportPtr exporter = pModule->create();
			OdStreamBufPtr pBuffer = odSystemServices()->createFile(outputFile.c_str(), Oda::kFileWrite, Oda::kShareDenyNo, Oda::kCreateAlways);
			PDFExportParams params;

			if (bDwg)
			{
				OdDbDatabasePtr pDb = dwgSvcs.readFile(inputFile);
				SetPdfExportParam(params, pDb, pBuffer);

				OdUInt32 errCode = exporter->exportPdf(params);
				if (errCode != 0)
				{
					OdString errMes = exporter->exportPdfErrorCode(errCode);
					printf("\nexportPdf error returned : 0x%x. \n%s", (unsigned)errCode, (const char*)errMes);
				}
			}
			else
			{
				::odrxDynamicLinker()->loadModule(L"TG_Db", false);
				OdDgDatabasePtr pDb = dgnSvcs.readFile(inputFile);
				SetPdfExportParam(params, pDb, pBuffer);

				OdUInt32 errCode = exporter->exportPdf(params);
				if (errCode != 0)
				{
					OdString errMes = exporter->exportPdfErrorCode(errCode);
					printf("\nexportPdf error returned : 0x%x. \n%s", (unsigned)errCode, (const char*)errMes);
				}
			}
		}
		catch (OdError& err)
		{
			OdString msg = err.description();
			STD(cout) << "Teigha Error: " << (const char*)msg << STD(endl) << STD(endl);
			return 0;
		}
		catch (...)
		{
			STD(cout) << "Unknown Error." << STD(endl) << STD(endl);
			return 0;
		}

		odUninitialize();
	}

	return 0;
}

void SetPdfExportParam(PDFExportParams& params, OdDbDatabasePtr& pDb, const OdStreamBufPtr& pBuffer)
{
	int nSolidHatchExpType = 2;
	int nOtherHatchExpType = 1;
	bool bZoomToExtents = true;
	bool bEmbededTTF = false;
	bool bEmbededOptimizedTTF = false;
	bool bTTFAsAGeometry = true;
	bool bSHXAsAGeometry = true;
	bool bEnableLayersSupport = false;
	bool bIncludeOffLayers = false;
	bool bExtentsBoundingBox = false;
	bool bEncoded = true;
	bool bASCIIhexEncoded = false;
	bool bUseHiddenLineAlgo = true;
	bool bUseSimpleGeomOpt = true;
	bool bSearchableTTF = true;
	bool bSearchableSHX = true;
	bool bHyperlinks = false;
	bool bLinearized = false;
	bool bMeasuring = false;
	bool bMonoPalette = false;
	OdUInt16 iGeomRes = 4800;
	OdUInt16 iColorRes = 600;
	OdUInt16 iBWRes = 4800;
	PDFExportParams::ColorPolicy cp = PDFExportParams::kGrayscale;
	PDFExportParams::PDF_A_mode  pdfAmode = PDFExportParams::kPDFA_None;
	double width = 0.0f;
	double height = 0.0f;

	params.setDatabase(pDb);
	params.setVersion(PDFExportParams::kPDFv1_5);
	params.setOutput(pBuffer);
	params.setExportFlags(PDFExportParams::PDFExportFlags(
		(bEmbededTTF ? PDFExportParams::kEmbededTTF : 0) |
		(bEmbededOptimizedTTF ? PDFExportParams::kEmbededOptimizedTTF : 0) |
		(bTTFAsAGeometry ? PDFExportParams::kTTFTextAsGeometry : 0) |
		(bSHXAsAGeometry ? PDFExportParams::kSHXTextAsGeometry : 0) |
		(bEnableLayersSupport ? PDFExportParams::kEnableLayers : 0) |
		(bIncludeOffLayers ? PDFExportParams::kIncludeOffLayers : 0) |
		(bUseSimpleGeomOpt ? PDFExportParams::kSimpleGeomOptimization : 0) |
		(bEncoded ? PDFExportParams::kFlateCompression : 0) |
		(bASCIIhexEncoded ? PDFExportParams::kASCIIHexEncoding : 0) |
		(bUseHiddenLineAlgo ? PDFExportParams::kUseHLR : 0) |
		(bHyperlinks ? PDFExportParams::kExportHyperlinks : 0) |
		(bLinearized ? PDFExportParams::kLinearized : 0) |
		(bMeasuring ? PDFExportParams::kMeasuring : 0) |
		(bZoomToExtents ? PDFExportParams::kZoomToExtentsMode : 0)));
	params.setSearchableTextType((PDFExportParams::SearchableTextType)((bSearchableSHX ? PDFExportParams::kSHX : 0) | (bSearchableTTF ? PDFExportParams::kTTF : 0)));
	if (!bExtentsBoundingBox)
		SETBIT(params.m_reserved1, 1, 1);
	params.setTitle("Batch PDF File");
	params.setAuthor("OdPdfTestEx");
	params.setCreator("Teigha");
	params.setGeomDPI(iGeomRes);
	params.setColorImagesDPI(iColorRes);
	params.setBWImagesDPI(iBWRes);
	params.setArchived(pdfAmode);
	params.setOtherHatchesExportType(GETBIT(nOtherHatchExpType, 1) ? PDFExportParams::kDrawing : PDFExportParams::kBitmap);
	if (cp != PDFExportParams::kNoPolicy)
		params.setColorPolicy(cp);
	switch (nSolidHatchExpType)
	{
		case 1: // as a drawing (vectorizer)
		{
			params.setSolidHatchesExportType(PDFExportParams::kDrawing);
			break;
		}
		case 2: // as a pdf path
		{
			params.setSolidHatchesExportType(PDFExportParams::kPdfPaths);
			break;
		}
		case 0: // as a bitmap
		default:
		{
			params.setSolidHatchesExportType(PDFExportParams::kBitmap);
			break;
		}
	}

	OdDbBlockTableRecordPtr pLayoutBlock = pDb->getActiveLayoutBTRId().safeOpenObject();
	OdDbLayoutPtr pLayout = pLayoutBlock->getLayoutId().safeOpenObject(OdDb::kForWrite);
	pLayout->setPrintLineweights(false);
	pLayout->setScaleLineweights(false);
	params.layouts().push_back(pLayout->getLayoutName());
	if (pLayout->modelType())
	{
		OdGeExtents3d xts;
		pDb->getModelSpaceId().safeOpenObject()->getGeomExtents(xts);
		width = xts.maxPoint().x - xts.minPoint().x;
		height = xts.maxPoint().y - xts.minPoint().y;

		if (width > 10e10 || height > 10e10)
		{
			pLayout->getGeomExtents(xts);
			width = xts.maxPoint().x - xts.minPoint().x;
			height = xts.maxPoint().y - xts.minPoint().y;
		}
	}
	else
		pLayout->getPlotPaperSize(width, height);

	OdGsPageParams pageParams; // in mm
	if (width > height)
		pageParams.set(min(width, 297), min(height, 210));
	else
		pageParams.set(min(width, 210), min(height, 297));
	params.pageParams().push_back(pageParams);
}

void SetPdfExportParam(PDFExportParams& params, OdDgDatabasePtr& pDb, const OdStreamBufPtr& pBuffer)
{
	int nSolidHatchExpType = 2;
	int nOtherHatchExpType = 1;
	bool bZoomToExtents = true;
	bool bEmbededTTF = false;
	bool bEmbededOptimizedTTF = false;
	bool bTTFAsAGeometry = true;
	bool bSHXAsAGeometry = true;
	bool bEnableLayersSupport = false;
	bool bIncludeOffLayers = false;
	bool bExtentsBoundingBox = false;
	bool bEncoded = true;
	bool bASCIIhexEncoded = false;
	bool bUseHiddenLineAlgo = true;
	bool bUseSimpleGeomOpt = true;
	bool bSearchableTTF = true;
	bool bSearchableSHX = true;
	bool bHyperlinks = false;
	bool bLinearized = false;
	bool bMeasuring = false;
	bool bMonoPalette = false;
	OdUInt16 iGeomRes = 4800;
	OdUInt16 iColorRes = 600;
	OdUInt16 iBWRes = 4800;
	PDFExportParams::ColorPolicy cp = PDFExportParams::kGrayscale;
	PDFExportParams::PDF_A_mode  pdfAmode = PDFExportParams::kPDFA_None;
	double width = 0.0f;
	double height = 0.0f;

	params.setDatabase(pDb);
	params.setVersion(PDFExportParams::kPDFv1_5);
	params.setOutput(pBuffer);
	params.setExportFlags(PDFExportParams::PDFExportFlags(
		(bEmbededTTF ? PDFExportParams::kEmbededTTF : 0) |
		(bEmbededOptimizedTTF ? PDFExportParams::kEmbededOptimizedTTF : 0) |
		(bTTFAsAGeometry ? PDFExportParams::kTTFTextAsGeometry : 0) |
		(bSHXAsAGeometry ? PDFExportParams::kSHXTextAsGeometry : 0) |
		(bEnableLayersSupport ? PDFExportParams::kEnableLayers : 0) |
		(bIncludeOffLayers ? PDFExportParams::kIncludeOffLayers : 0) |
		(bUseSimpleGeomOpt ? PDFExportParams::kSimpleGeomOptimization : 0) |
		(bEncoded ? PDFExportParams::kFlateCompression : 0) |
		(bASCIIhexEncoded ? PDFExportParams::kASCIIHexEncoding : 0) |
		(bUseHiddenLineAlgo ? PDFExportParams::kUseHLR : 0) |
		(bHyperlinks ? PDFExportParams::kExportHyperlinks : 0) |
		(bLinearized ? PDFExportParams::kLinearized : 0) |
		(bMeasuring ? PDFExportParams::kMeasuring : 0) |
		(bZoomToExtents ? PDFExportParams::kZoomToExtentsMode : 0)));
	params.setSearchableTextType((PDFExportParams::SearchableTextType)((bSearchableSHX ? PDFExportParams::kSHX : 0) | (bSearchableTTF ? PDFExportParams::kTTF : 0)));
	if (!bExtentsBoundingBox)
		SETBIT(params.m_reserved1, 1, 1);
	params.setTitle("Batch PDF File");
	params.setAuthor("OdPdfTestEx");
	params.setCreator("Teigha");
	params.setGeomDPI(iGeomRes);
	params.setColorImagesDPI(iColorRes);
	params.setBWImagesDPI(iBWRes);
	params.setArchived(pdfAmode);
	params.setOtherHatchesExportType(GETBIT(nOtherHatchExpType, 1) ? PDFExportParams::kDrawing : PDFExportParams::kBitmap);
	if (cp != PDFExportParams::kNoPolicy)
		params.setColorPolicy(cp);
	switch (nSolidHatchExpType)
	{
		case 1: // as a drawing (vectorizer)
		{
			params.setSolidHatchesExportType(PDFExportParams::kDrawing);
			break;
		}
		case 2: // as a pdf path
		{
			params.setSolidHatchesExportType(PDFExportParams::kPdfPaths);
			break;
		}
		case 0: // as a bitmap
		default:
		{
			params.setSolidHatchesExportType(PDFExportParams::kBitmap);
			break;
		}
	}

	OdDgElementId activeModelId = pDb->getActiveModelId();
	if (activeModelId.isNull())
	{
		// MKU 24\11\2010 #8852 (case: /dgn/V7_V8legacy/Regression/out/ExLoadToModel_dll/Building/Dgn/BSI300AE9-Atrium.dgn
		//  If Model is is invalid let's try to get & use a Default.
		activeModelId = pDb->getDefaultModelId();
		pDb->setActiveModelId(activeModelId);
	}

	if (activeModelId.isNull())
	{
		STD(cout) << "Incorrect Active Model Id\n";
		throw OdError(eInvalidInput);
	}

	OdDgModelPtr pModel = OdDgModel::cast(activeModelId.openObject(OdDg::kForRead));
	if (pModel.isNull())
	{
		STD(cout) << "Can not open Active Model\n";
		throw OdError(eInvalidInput);
	}

	OdGeExtents3d ext;
	pModel->getGeomExtents(ext);
	width = ext.maxPoint().x - ext.minPoint().x;
	height = ext.maxPoint().y - ext.minPoint().y;
	OdGsPageParams pageParams; // in mm
	if (width > height)
		pageParams.set(min(width, 297), min(height, 210));
	else
		pageParams.set(min(width, 210), min(height, 297));
	params.pageParams().push_back(pageParams);
}