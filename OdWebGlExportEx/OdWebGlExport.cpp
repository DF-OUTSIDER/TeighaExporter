#include "stdafx.h"
#include <locale.h>
#include <tchar.h>
#include <iostream>
#include <map>
#include <list>
#include "OdaCommon.h"
#include "ExSystemServices.h"
#include "ExHostAppServices.h"
#include "RxDynamicModule.h"
#include "NDSWeb3DModelWriter.h"

#include "DbDatabase.h"
#include "DbBlockTableRecord.h"
#include "DbLayerTableRecord.h"
#include "DbBlockTable.h"
#include "DbBlockReference.h"
//simple entity
#include "DbBlockBegin.h"
#include "DbBlockEnd.h"
#include "DbArc.h"
#include "DbCircle.h"
#include "DbEllipse.h"
#include "DbLine.h"
#include "DbRay.h"
#include "DbSpline.h"
#include "DbXline.h"
#include "DbLight.h"
#include "DbPoint.h"
#include "DbSolid.h"
#include "DbText.h"
#include "DbTrace.h"
#include "DbAttributeDefinition.h"
#include "DbAttribute.h"
#include "DbFace.h"
#include "DbShape.h"
#include "DbSequenceEnd.h"
#include "Db3dPolylineVertex.h"
#include "Db2dVertex.h"
#include "DbPolyFaceMeshVertex.h"
#include "DbPolygonMeshVertex.h"
//other entity
#include "DbViewport.h"
#include "DbTextStyleTableRecord.h"
#include "DbLayerTable.h"

#pragma region 预处理

using namespace std;
using namespace ModelExporter;

#ifndef _TOOLKIT_IN_DLL_
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ModelerModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRecomputeDimBlockModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(BitmapModule);
#if defined(OD_HAS_OPENGL)
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(WinOpenGLModule);
#endif

ODRX_BEGIN_STATIC_MODULE_MAP()
ODRX_DEFINE_STATIC_APPLICATION(OdModelerGeometryModuleName, ModelerModule)
ODRX_DEFINE_STATIC_APPLICATION(OdRecomputeDimBlockModuleName, OdRecomputeDimBlockModule)
ODRX_DEFINE_STATIC_APPMODULE(OdWinBitmapModuleName, BitmapModule)
#if defined(OD_HAS_OPENGL)
ODRX_DEFINE_STATIC_APPMODULE(OdWinOpenGLModuleName, WinOpenGLModule)
#endif
ODRX_END_STATIC_MODULE_MAP()

#endif

class MyServices : public ExSystemServices, public ExHostAppServices
{
protected:
	ODRX_USING_HEAP_OPERATORS(ExSystemServices);
private:

	/************************************************************************/
	/* Compile password support for R18 Drawings                            */
	/************************************************************************/
#if defined(_MSC_VER) && !(defined(_WINRT))
	bool getPassword(const OdString& dwgName, bool isXref, OdPassword& password)
	{
		char buff[_MAX_PATH] = { 0 };
		cout << "Enter password to open drawing: " << endl;
		::CharToOemA((const char*)dwgName, buff);
		cout << "    " << buff << endl;

		cin.get(buff, '\n').get();

		wchar_t pwdBuff[_MAX_PATH] = { 0 };
		// convert to upper case unicode:
		::OemToCharW(buff, pwdBuff);
		::CharUpperW(pwdBuff);
		password = pwdBuff;
		return !password.isEmpty();
	}
#endif
public:
	OdGsDevicePtr gsBitmapDevice(OdRxObject* /*pViewObj*/ = NULL,
		OdDbBaseDatabase* /*pDb*/ = NULL,
		OdUInt32 flags = 0)
	{
		OdGsModulePtr pM;
		if (GETBIT(flags, kFor2dExportRender))
		{
			if (GETBIT(flags, kFor2dExportRenderHLR))
				return OdGsDevicePtr();
			pM = ::odrxDynamicLinker()->loadModule(OdWinOpenGLModuleName);
		}
		if (pM.isNull())
			pM = ::odrxDynamicLinker()->loadModule(OdWinBitmapModuleName);
		if (pM.isNull())
			return OdGsDevicePtr();

		return pM->createBitmapDevice();
	}
};

#pragma endregion

Va3cContainer*												g_pContainer;
map<string, Va3cContainer::Va3cObject*>						g_layerObjects;
map<string, Va3cContainer::Va3cMaterial*>					g_existMaterials;
map<string, Va3cContainer::Va3cMaterial*>					g_existPmiMaterials;
#pragma region simple entities maps
map<string, map<string, list<OdDbBlockBeginPtr>>>			g_blockBeginMap;
map<string, map<string, list<OdDbBlockEndPtr>>>				g_blockEndMap;
map<string, map<string, list<OdDbArcPtr>>>					g_arcMap;
map<string, map<string, list<OdDbCirclePtr>>>				g_circleMap;
map<string, map<string, list<OdDbEllipsePtr>>>				g_ellipseMap;
map<string, map<string, list<OdDbLinePtr>>>					g_lineMap;
map<string, map<string, list<OdDbRayPtr>>>					g_rayMap;
map<string, map<string, list<OdDbSplinePtr>>>				g_splineMap;
map<string, map<string, list<OdDbXlinePtr>>>				g_xlineMap;
map<string, map<string, list<OdDbLightPtr>>>				g_lightMap;
map<string, map<string, list<OdDbPointPtr>>>				g_pointMap;
map<string, map<string, list<OdDbSolidPtr>>>				g_solidMap;
map<string, map<string, list<OdDbTextPtr>>>					g_textMap;
map<string, map<string, list<OdDbTracePtr>>>				g_traceMap;
map<string, map<string, list<OdDbAttributeDefinitionPtr>>>	g_attributeDefinitionMap;
map<string, map<string, list<OdDbAttributePtr>>>			g_attributeMap;
map<string, map<string, list<OdDbFacePtr>>>					g_faceMap;
map<string, map<string, list<OdDbShapePtr>>>				g_shapeMap;
map<string, map<string, list<OdDbSequenceEndPtr>>>			g_sequenceEndMap;
map<string, map<string, list<OdDb3dPolylineVertexPtr>>>		g_3dPolylineVertexMap;
map<string, map<string, list<OdDb2dVertexPtr>>>				g_2dVertexMap;
map<string, map<string, list<OdDbPolyFaceMeshVertexPtr>>>	g_polyFaceMeshVertexMap;
map<string, map<string, list<OdDbPolygonMeshVertexPtr>>>	g_polygonMeshVertexMap;
#pragma endregion

#pragma region 函数声明

string toUtf8(const wchar_t* wchar);
string toUtf8(const wstring& wstr);
string toUtf8(const string& str);
string toString(const int& n);
string toString(const wstring& wstr);
string NewGuid();
bool isSimpleEntity(OdDbEntityPtr& pEntity);
int getGeometryType(OdDbEntityPtr& pEntity);
bool importEntity(OdDbEntityPtr& pEntity, bool bExpoded, string parentLayerName = "");
bool isEntityVisible(OdDbEntityPtr& pEntity);
bool isEntityOutOfRange(OdDbEntityPtr& pEntity);
bool exportMaterial(string& uuid, int type, OdDbEntityPtr& pEntity, OdDbBlockReferencePtr& pParentBlockReference, string lineWidth = "1");
bool isMaterialExsited(int type, const string& color, const string& alpha, const string& lineWidth, string& guid);
void insert2CorrespondingMap(string layerName, string materialUuid, OdDbEntityPtr& pEntity);
void release();

Va3cContainer::Va3cObject* newVa3cObject(OdString name, string type);
Va3cContainer::Va3cGeometry* newVa3cGeometry(eGeometryCategory type, Va3cContainer::Va3cObject* pObject);
bool exportAcDbPoint();
bool exportAcDbLine();
bool exportAcDbArc();
bool exportAcDbCircle();
bool exportAcDbEllipse();
bool exportAcDbSpline();
bool exportOdGeCurve(OdGeCurve3d* pOdcGeCurve, Va3cContainer::Va3cGeometry* pGeometry);
bool exportAcDbSolid();
bool exportAcDbText();
bool exportText(OdDbTextPtr& text, Va3cContainer::Va3cObject* pObject, Va3cContainer::Va3cTextGeometry* pGeometry);

#pragma endregion

/************************************************************************/
/* Main                                                                 */
/************************************************************************/
#if defined(OD_USE_WMAIN)
int wmain(int argc, wchar_t* argv[])
#else
int main(int argc, char* argv[])
#endif
{
	if (argc > 2)
	{
		setlocale(LC_ALL, "");
		setlocale(LC_NUMERIC, "C");

#ifndef _TOOLKIT_IN_DLL_
		ODRX_INIT_STATIC_MODULE_MAP();
#endif

		OdStaticRxObject<MyServices> svcs;
		odInitialize(&svcs);

		cout << "Developed using " << (const char*)svcs.product() << ", " << (const char*)svcs.versionString() << endl;

		try
		{
			OdDbDatabasePtr pDb = svcs.readFile(argv[1]);
			if (pDb.isNull())
				return -1;

			g_pContainer = new Va3cContainer();
			g_pContainer->metadata.generator = "TeighaExporter";
			OdString fileName = pDb->getFilename();
			int index = max(fileName.reverseFind('/'), fileName.reverseFind('\\'));
			fileName = fileName.right(fileName.getLength() - 1 - index);
			g_pContainer->object.name = toUtf8(fileName);
			g_pContainer->object.type = "Root";
			g_pContainer->object.uuid = NewGuid();
			
			OdDbLayerTablePtr lt = pDb->getLayerTableId().safeOpenObject();
			OdDbSymbolTableIteratorPtr it = lt->newIterator();
			while (!it->done())
			{
				OdDbLayerTableRecordPtr layer = (OdDbLayerTableRecordPtr)it->getRecord();
				Va3cContainer::Va3cObject* pObject = newVa3cObject(layer->getName(), string(layer->isA()->name()));
				g_pContainer->object.children.push_back(pObject);
				g_layerObjects[string(layer->getName())] = pObject;
				it->step();
			}

			OdDbBlockTableRecordPtr pLayoutBlocks = pDb->getActiveLayoutBTRId().safeOpenObject(OdDb::kForWrite);
			OdDbObjectIteratorPtr itor = pLayoutBlocks->newIterator();
			itor->start();
			while (!itor->done())
			{
				importEntity(itor->entity(), false);
				itor->step();
			}

			for (auto it = g_existMaterials.begin(); it != g_existMaterials.end(); ++it)
				g_pContainer->materials.push_back(it->second);
			for (auto it = g_existPmiMaterials.begin(); it != g_existPmiMaterials.end(); ++it)
				g_pContainer->pmiMaterials.push_back(it->second);
			exportAcDbLine();
			exportAcDbArc();
			exportAcDbCircle();
			exportAcDbEllipse();
			exportAcDbSpline();
			exportAcDbSolid();
			exportAcDbText();

			if (!NDSWeb3DModelWriter::WriteWeb3DModelFilesVersion6BufferChunks(*g_pContainer, toString(argv[2]), "model.js", "geom.bin", 2))
				cout << "失败！" << endl;

			release();
		}
		catch (OdError& err)
		{
			OdString msg = err.description();
			cout << "Teigha Error: " << (const char*)msg << endl << endl;
			return 0;
		}
		catch (...)
		{
			cout << "Unknown Error." << endl << endl;
			return 0;
		}

		odUninitialize();
	}

	return 0;
}

#pragma region 字符串

string toUtf8(const wchar_t* wchar)
{
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wchar, -1, NULL, 0, NULL, FALSE);
	char *psText;
	psText = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, wchar, -1, psText, dwNum, NULL, FALSE);
	string szDst = psText;

	return toUtf8(szDst);
}

string toUtf8(const wstring& wstr)
{
	return toUtf8(wstr.c_str());
}

string toUtf8(const string& str)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);

	wchar_t * pwBuf = new wchar_t[nwLen + 1];
	ZeroMemory(pwBuf, nwLen * 2 + 2);

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, nullptr, 0, nullptr, nullptr);

	char * pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);

	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, nullptr, nullptr);

	std::string retStr(pBuf);

	delete[]pwBuf;
	delete[]pBuf;

	pwBuf = nullptr;
	pBuf = nullptr;

	return retStr;
}

string toString(const int& n)
{
	char tmpStr[512];
	sprintf_s(tmpStr, "%d", n);
	return tmpStr;
}

string toString(const wstring& wstr)
{
	int nLen = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	if (nLen <= 0) return std::string("");
	char* pszDst = new char[nLen];
	if (nullptr == pszDst) return std::string("");
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, pszDst, nLen, nullptr, nullptr);
	pszDst[nLen - 1] = 0;
	std::string strTemp(pszDst);
	delete[] pszDst;

	return strTemp;
}

string NewGuid()
{
	const int GUID_LEN = 64;
	char buffer[GUID_LEN] = { 0 };
	GUID guid;

	if (CoCreateGuid(&guid))
	{
		fprintf(stderr, "create guid error\n");
		return "";
	}
	_snprintf(buffer, sizeof(buffer),
		"%08x%04x%04x%02x%02x%02x%02x%02x%02x%02x%02x",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2],
		guid.Data4[3], guid.Data4[4], guid.Data4[5],
		guid.Data4[6], guid.Data4[7]);

	return buffer;
}

#pragma endregion

bool isSimpleEntity(OdDbEntityPtr& pEntity)
{
	if (pEntity->isKindOf(OdDbBlockBegin::desc())
		|| pEntity->isKindOf(OdDbBlockEnd::desc())
		|| pEntity->isKindOf(OdDbArc::desc())
		|| pEntity->isKindOf(OdDbCircle::desc())
		|| pEntity->isKindOf(OdDbEllipse::desc())
		|| pEntity->isKindOf(OdDbLine::desc())
		|| pEntity->isKindOf(OdDbRay::desc())
		|| pEntity->isKindOf(OdDbSpline::desc())
		|| pEntity->isKindOf(OdDbXline::desc())
		|| pEntity->isKindOf(OdDbLight::desc())
		|| pEntity->isKindOf(OdDbPoint::desc())
		|| pEntity->isKindOf(OdDbSolid::desc())
		|| pEntity->isKindOf(OdDbText::desc())
		|| pEntity->isKindOf(OdDbTrace::desc())
		|| pEntity->isKindOf(OdDbAttributeDefinition::desc())
		|| pEntity->isKindOf(OdDbAttribute::desc())
		|| pEntity->isKindOf(OdDbFace::desc())
		|| pEntity->isKindOf(OdDbShape::desc())
		|| pEntity->isKindOf(OdDbSequenceEnd::desc())
		|| pEntity->isKindOf(OdDb3dPolylineVertex::desc())
		|| pEntity->isKindOf(OdDb2dVertex::desc())
		|| pEntity->isKindOf(OdDbPolyFaceMeshVertex::desc())
		|| pEntity->isKindOf(OdDbPolygonMeshVertex::desc()))
		return true;

	return false;
}

int getGeometryType(OdDbEntityPtr& pEntity)
{
	if (pEntity->isKindOf(OdDbSolid::desc())
		|| pEntity->isKindOf(OdDbFace::desc())
		|| pEntity->isKindOf(OdDbShape::desc()))
		return 0;
	else if (pEntity->isKindOf(OdDbArc::desc())
		|| pEntity->isKindOf(OdDbCircle::desc())
		|| pEntity->isKindOf(OdDbEllipse::desc())
		|| pEntity->isKindOf(OdDbLine::desc())
		|| pEntity->isKindOf(OdDbRay::desc())
		|| pEntity->isKindOf(OdDbSpline::desc())
		|| pEntity->isKindOf(OdDbXline::desc()))
		return 1;
	else if (pEntity->isKindOf(OdDbPoint::desc())
		|| pEntity->isKindOf(OdDb3dPolylineVertex::desc())
		|| pEntity->isKindOf(OdDb2dVertex::desc())
		|| pEntity->isKindOf(OdDbPolyFaceMeshVertex::desc())
		|| pEntity->isKindOf(OdDbPolygonMeshVertex::desc()))
		return 2;
	else if (pEntity->isKindOf(OdDbText::desc()))
		return 3;
	
	return -1;
}

bool importEntity(OdDbEntityPtr& pEntity, bool bExpoded, string layerName/* = ""*/)
{
	if (!isEntityVisible(pEntity))
		return false;

	if (isEntityOutOfRange(pEntity))
		return false;

	if (!bExpoded || !pEntity->layerId().isNull())
	{
		OdDbLayerTableRecordPtr ltr = pEntity->layerId().safeOpenObject();
		layerName = ltr->getName();
	}

	if (isSimpleEntity(pEntity))
	{
		string materialId;
		if (!exportMaterial(materialId, getGeometryType(pEntity), pEntity, OdDbBlockReferencePtr()))
			return false;
		insert2CorrespondingMap(layerName, materialId, pEntity);
	}
	else
	{
		OdRxObjectPtrArray entitySet;
		OdResult res = pEntity->explodeGeometry(entitySet);
		if (OdResult::eOk != res)
			return false;
		for (int i = 0; i < entitySet.size(); ++i)
		{
			OdDbEntityPtr pExplodedEntity = (OdDbEntityPtr)entitySet[i];
			importEntity(pExplodedEntity, true, layerName);
		}
	}
}

bool isEntityVisible(OdDbEntityPtr& pEntity)
{
	OdGeExtents3d ext;
	pEntity->getGeomExtents(ext);

	if ( (ext.minPoint().x - ext.maxPoint().x) > 1e-6 )
		return false;
	if ((ext.minPoint().y - ext.maxPoint().y) > 1e-6)
		return false;

	if (pEntity->visibility() == OdDb::kInvisible)
		return false;

	if (pEntity->isKindOf(OdDbViewport::desc()))
	{
		OdDbViewportPtr vp = (OdDbViewportPtr)pEntity;
		if (!vp->isOn())
			return false;
	}

	return true;
}

bool isEntityOutOfRange(OdDbEntityPtr& pEntity)
{
	OdGeExtents3d extents;
	pEntity->getGeomExtents(extents);
	double maxX = extents.maxPoint().x;
	double maxY = extents.maxPoint().y;
	double maxZ = extents.maxPoint().z;
	double minX = extents.minPoint().x;
	double minY = extents.minPoint().y;
	double minZ = extents.minPoint().z;

	if (maxX > FLT_MAX || maxX < -FLT_MAX
		|| maxY > FLT_MAX || maxY < -FLT_MAX
		|| maxZ > FLT_MAX || maxZ < -FLT_MAX
		|| minX > FLT_MAX || minX < -FLT_MAX
		|| minY > FLT_MAX || minY < -FLT_MAX
		|| minZ > FLT_MAX || minZ < -FLT_MAX)
		return true;

	return false;
}

bool exportMaterial(string& uuid, int type, OdDbEntityPtr& pEntity, OdDbBlockReferencePtr& pParentBlockReference, string lineWidth/* = "1"*/)
{
	string strColor;
	OdCmColor odCmColor = pEntity->color();
	OdUInt8 r = odCmColor.red(), g = odCmColor.green(), b = odCmColor.blue();

	if (odCmColor.isByBlock())
	{
		if (pParentBlockReference.isNull())
		{
			r = 255;
			g = 255;
			b = 255;
		}
		else
		{
			odCmColor = pParentBlockReference->color();
			if (odCmColor.isByLayer())
			{
				OdDbLayerTableRecordPtr ltr = pParentBlockReference->layerId().safeOpenObject();
				odCmColor = ltr->color();
			}
			r = odCmColor.red();
			g = odCmColor.green();
			b = odCmColor.blue();
		}
	}
	else if (odCmColor.isByLayer())
	{
		OdDbLayerTableRecordPtr ltr = pEntity->layerId().safeOpenObject();
		odCmColor = ltr->color();
		r = odCmColor.red();
		g = odCmColor.green();
		b = odCmColor.blue();
	}
	int dColor = r << 16 | g << 8 | b;
	strColor = toString(dColor);

	if (!isMaterialExsited(type, strColor, "1", lineWidth, uuid))
	{
		Va3cContainer::Va3cMaterial* pMaterial = new Va3cContainer::Va3cMaterial();
		pMaterial->uuid = NewGuid();
		uuid = pMaterial->uuid;
		if (type == 0)
		{
			pMaterial->type = "MeshBasicMaterial";
		}
		else if (type == 1)
		{
			//test
			//pMaterial->linewidth = lineWidth;
			pMaterial->linewidth = "1";
			pMaterial->type = STR_LINEBASIC_MATERIAL;
		}
		else if (type == 2)
		{
			pMaterial->type = STR_POINT_MATERIAL;
		}
		else if (type == 3)
		{
			pMaterial->type = "MeshBasicMaterial";
		}
		pMaterial->color = strColor;
		pMaterial->opacity = "1";
		if (type == 3)
			g_existPmiMaterials.insert(make_pair(pMaterial->uuid, pMaterial));
		else
			g_existMaterials.insert(make_pair(pMaterial->uuid, pMaterial));
	}

	return true;
}

bool isMaterialExsited(int type, const string& color, const string& alpha, const string& lineWidth, string& guid)
{
	if (type == 3)
	{
		for (auto itor = g_existPmiMaterials.begin(); itor != g_existPmiMaterials.end(); ++itor)
		{
			Va3cContainer::Va3cMaterial* pMaterial = itor->second;

			if (pMaterial->type.compare("MeshBasicMaterial") != 0)
				continue;

			if (pMaterial != nullptr && pMaterial->color.compare(color) == 0 && pMaterial->opacity.compare(alpha) == 0)
			{
				guid = itor->first;
				return true;
			}
		}
	}
	else
	{
		for (auto itor = g_existMaterials.begin(); itor != g_existMaterials.end(); ++itor)
		{
			Va3cContainer::Va3cMaterial* pMaterial = itor->second;

			if (type == 0 && pMaterial->type.compare("MeshBasicMaterial") != 0)
				continue;
			else if (type == 1 && pMaterial->type.compare(STR_LINEBASIC_MATERIAL) != 0)
				continue;
			else if (type == 2 && pMaterial->type.compare(STR_POINT_MATERIAL) != 0)
				continue;

			if (pMaterial != nullptr && pMaterial->color.compare(color) == 0 && pMaterial->opacity.compare(alpha) == 0)
			{
				if (type == eGeometryCategory::Line && lineWidth.compare(pMaterial->linewidth) != 0)
					return false;

				guid = itor->first;
				return true;
			}
		}
	}
	

	return false;
}

void insert2CorrespondingMap(string layerName, string materialUuid, OdDbEntityPtr& pEntity)
{
	if (pEntity->isKindOf(OdDbBlockBegin::desc()))
		g_blockBeginMap[layerName][materialUuid].push_back((OdDbBlockBeginPtr)pEntity->clone());
	else if (pEntity->isKindOf(OdDbBlockEnd::desc()))
		g_blockEndMap[layerName][materialUuid].push_back((OdDbBlockEndPtr)pEntity->clone());
	else if (pEntity->isKindOf(OdDbArc::desc()))
		g_arcMap[layerName][materialUuid].push_back((OdDbArcPtr)pEntity->clone());
	else if (pEntity->isKindOf(OdDbCircle::desc()))
		g_circleMap[layerName][materialUuid].push_back((OdDbCirclePtr)pEntity->clone());
	else if (pEntity->isKindOf(OdDbEllipse::desc()))
		g_ellipseMap[layerName][materialUuid].push_back((OdDbEllipsePtr)pEntity->clone());
	else if (pEntity->isKindOf(OdDbLine::desc()))
		g_lineMap[layerName][materialUuid].push_back((OdDbLinePtr)pEntity->clone());
	else if (pEntity->isKindOf(OdDbRay::desc()))
		g_rayMap[layerName][materialUuid].push_back((OdDbRayPtr)pEntity->clone());
	else if (pEntity->isKindOf(OdDbSpline::desc()))
		g_splineMap[layerName][materialUuid].push_back((OdDbSplinePtr)pEntity->clone());
	else if (pEntity->isKindOf(OdDbXline::desc()))
		g_xlineMap[layerName][materialUuid].push_back((OdDbXlinePtr)pEntity->clone());
	else if (pEntity->isKindOf(OdDbLight::desc()))
		g_lightMap[layerName][materialUuid].push_back((OdDbLightPtr)pEntity->clone());
	else if (pEntity->isKindOf(OdDbPoint::desc()))
		g_pointMap[layerName][materialUuid].push_back((OdDbPointPtr)pEntity->clone());
	else if (pEntity->isKindOf(OdDbSolid::desc()))
		g_solidMap[layerName][materialUuid].push_back((OdDbSolidPtr)pEntity->clone());
	else if (pEntity->isKindOf(OdDbText::desc()))
		g_textMap[layerName][materialUuid].push_back((OdDbTextPtr)pEntity->clone());
	else if (pEntity->isKindOf(OdDbTrace::desc()))
		g_traceMap[layerName][materialUuid].push_back((OdDbTracePtr)pEntity->clone());
	else if (pEntity->isKindOf(OdDbAttributeDefinition::desc()))
		g_attributeDefinitionMap[layerName][materialUuid].push_back((OdDbAttributeDefinitionPtr)pEntity->clone());
	else if (pEntity->isKindOf(OdDbAttribute::desc()))
		g_attributeMap[layerName][materialUuid].push_back((OdDbAttributePtr)pEntity->clone());
	else if (pEntity->isKindOf(OdDbFace::desc()))
		g_faceMap[layerName][materialUuid].push_back((OdDbFacePtr)pEntity->clone());
	else if (pEntity->isKindOf(OdDbShape::desc()))
		g_shapeMap[layerName][materialUuid].push_back((OdDbShapePtr)pEntity->clone());
	else if (pEntity->isKindOf(OdDbSequenceEnd::desc()))
		g_sequenceEndMap[layerName][materialUuid].push_back((OdDbSequenceEndPtr)pEntity->clone());
	else if (pEntity->isKindOf(OdDb3dPolylineVertex::desc()))
		g_3dPolylineVertexMap[layerName][materialUuid].push_back((OdDb3dPolylineVertexPtr)pEntity->clone());
	else if (pEntity->isKindOf(OdDb2dVertex::desc()))
		g_2dVertexMap[layerName][materialUuid].push_back((OdDb2dVertexPtr)pEntity->clone());
	else if (pEntity->isKindOf(OdDbPolyFaceMeshVertex::desc()))
		g_polyFaceMeshVertexMap[layerName][materialUuid].push_back((OdDbPolyFaceMeshVertexPtr)pEntity->clone());
	else if (pEntity->isKindOf(OdDbPolygonMeshVertex::desc()))
		g_polygonMeshVertexMap[layerName][materialUuid].push_back((OdDbPolygonMeshVertexPtr)pEntity->clone());
}

Va3cContainer::Va3cObject* newVa3cObject(OdString name, string type)
{
	Va3cContainer::Va3cObject* pObject = new Va3cContainer::Va3cObject();
	pObject->name = toUtf8(name);
	pObject->type = toUtf8(type);
	pObject->uuid = NewGuid();
	return pObject;
}

Va3cContainer::Va3cGeometry* newVa3cGeometry(eGeometryCategory type, Va3cContainer::Va3cObject* pObject)
{
	Va3cContainer::Va3cGeometry* pGeometry = new Va3cContainer::Va3cGeometry();
	pGeometry->uuid = NewGuid();
	pGeometry->data.nGeometryCategory = type;
	pObject->geometry = pGeometry->uuid;
	g_pContainer->geometries.push_back(pGeometry);
	return pGeometry;
}

bool exportAcDbPoint()
{
	try
	{
		for (auto it = g_pointMap.begin(); it != g_pointMap.end(); ++it)
		{
			string layerName = it->first;
			if (g_layerObjects.find(layerName) != g_layerObjects.end())
			{
				for (auto itor = it->second.begin(); itor != it->second.end(); ++itor)
				{
					string materialId = itor->first;
					list<OdDbPointPtr>& points = itor->second;

					Va3cContainer::Va3cObject* pObject = newVa3cObject(OdDbPoint::desc()->name(), STR_LINE_PIECES);
					Va3cContainer::Va3cGeometry* pGeometry = newVa3cGeometry(eGeometryCategory::Point, pObject);
					pObject->material = materialId;

					for (auto it = points.begin(); it != points.end(); ++it)
					{
						OdDbPointPtr& point = *it;
						OdGePoint3d position = point->position();
						pGeometry->data.vertices.push_back(position.x);
						pGeometry->data.vertices.push_back(position.y);
						pGeometry->data.vertices.push_back(position.z);
						pGeometry->data.points += 1;
					}

					g_pContainer->object.children.push_back(pObject);
				}
			}
		}

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool exportAcDbLine()
{
	try
	{
		for (auto it = g_lineMap.begin(); it != g_lineMap.end(); ++it)
		{
			string layerName = it->first;
			if (g_layerObjects.find(layerName) != g_layerObjects.end())
			{
				for (auto itor = it->second.begin(); itor != it->second.end(); ++itor)
				{
					string materialId = itor->first;
					list<OdDbLinePtr>& lines = itor->second;

					Va3cContainer::Va3cObject* pObject = newVa3cObject(OdDbLine::desc()->name(), STR_LINE_PIECES);
					Va3cContainer::Va3cGeometry* pGeometry = newVa3cGeometry(eGeometryCategory::Line, pObject);
					pObject->material = materialId;

					for (auto it = lines.begin(); it != lines.end(); ++it)
					{
						OdDbLinePtr& line = *it;
						OdGeCurve3d* pOdcGeCurve;
						line->getOdGeCurve(pOdcGeCurve);
						if (!pOdcGeCurve)
							continue;
						exportOdGeCurve(pOdcGeCurve, pGeometry);
					}

					g_layerObjects[layerName]->children.push_back(pObject);
				}
			}
		}

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool exportAcDbArc()
{
	try
	{
		for (auto it = g_arcMap.begin(); it != g_arcMap.end(); ++it)
		{
			string layerName = it->first;
			if (g_layerObjects.find(layerName) != g_layerObjects.end())
			{
				for (auto itor = it->second.begin(); itor != it->second.end(); ++itor)
				{
					string materialId = itor->first;
					list<OdDbArcPtr>& arcs = itor->second;

					Va3cContainer::Va3cObject* pObject = newVa3cObject(OdDbArc::desc()->name(), STR_LINE_PIECES);
					Va3cContainer::Va3cGeometry* pGeometry = newVa3cGeometry(eGeometryCategory::Line, pObject);
					pObject->material = materialId;

					for (auto it = arcs.begin(); it != arcs.end(); ++it)
					{
						OdDbArcPtr& arc = *it;
						OdGeCurve3d* pOdcGeCurve;
						arc->getOdGeCurve(pOdcGeCurve);
						if (!pOdcGeCurve)
							continue;
						exportOdGeCurve(pOdcGeCurve, pGeometry);
					}

					g_layerObjects[layerName]->children.push_back(pObject);
				}
			}
		}

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool exportAcDbCircle()
{
	try
	{
		for (auto it = g_circleMap.begin(); it != g_circleMap.end(); ++it)
		{
			string layerName = it->first;
			if (g_layerObjects.find(layerName) != g_layerObjects.end())
			{
				for (auto itor = it->second.begin(); itor != it->second.end(); ++itor)
				{
					string materialId = itor->first;
					list<OdDbCirclePtr>& circles = itor->second;

					Va3cContainer::Va3cObject* pObject = newVa3cObject(OdDbCircle::desc()->name(), STR_LINE_PIECES);
					Va3cContainer::Va3cGeometry* pGeometry = newVa3cGeometry(eGeometryCategory::Line, pObject);
					pObject->material = materialId;

					for (auto it = circles.begin(); it != circles.end(); ++it)
					{
						OdDbCirclePtr& circle = *it;
						OdGeCurve3d* pOdcGeCurve;
						circle->getOdGeCurve(pOdcGeCurve);
						if (!pOdcGeCurve)
							continue;
						exportOdGeCurve(pOdcGeCurve, pGeometry);
					}

					g_layerObjects[layerName]->children.push_back(pObject);
				}
			}
		}

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool exportAcDbEllipse()
{
	try
	{
		for (auto it = g_ellipseMap.begin(); it != g_ellipseMap.end(); ++it)
		{
			string layerName = it->first;
			if (g_layerObjects.find(layerName) != g_layerObjects.end())
			{
				for (auto itor = it->second.begin(); itor != it->second.end(); ++itor)
				{
					string materialId = itor->first;
					list<OdDbEllipsePtr>& ellipses = itor->second;

					Va3cContainer::Va3cObject* pObject = newVa3cObject(OdDbEllipse::desc()->name(), STR_LINE_PIECES);
					Va3cContainer::Va3cGeometry* pGeometry = newVa3cGeometry(eGeometryCategory::Line, pObject);
					pObject->material = materialId;

					for (auto it = ellipses.begin(); it != ellipses.end(); ++it)
					{
						OdDbEllipsePtr& ellipse = *it;
						OdGeCurve3d* pOdcGeCurve;
						ellipse->getOdGeCurve(pOdcGeCurve);
						if (!pOdcGeCurve)
							continue;
						exportOdGeCurve(pOdcGeCurve, pGeometry);
					}

					g_layerObjects[layerName]->children.push_back(pObject);
				}
			}
		}

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool exportAcDbSpline()
{
	try
	{
		for (auto it = g_splineMap.begin(); it != g_splineMap.end(); ++it)
		{
			string layerName = it->first;
			if (g_layerObjects.find(layerName) != g_layerObjects.end())
			{
				for (auto itor = it->second.begin(); itor != it->second.end(); ++itor)
				{
					string materialId = itor->first;
					list<OdDbSplinePtr>& splines = itor->second;

					Va3cContainer::Va3cObject* pObject = newVa3cObject(OdDbSpline::desc()->name(), STR_LINE_PIECES);
					Va3cContainer::Va3cGeometry* pGeometry = newVa3cGeometry(eGeometryCategory::Line, pObject);
					pObject->material = materialId;

					for (auto it = splines.begin(); it != splines.end(); ++it)
					{
						OdDbSplinePtr& spline = *it;
						OdGeCurve3d* pOdcGeCurve;
						spline->getOdGeCurve(pOdcGeCurve);
						if (!pOdcGeCurve)
							continue;
						exportOdGeCurve(pOdcGeCurve, pGeometry);
					}

					g_layerObjects[layerName]->children.push_back(pObject);
				}
			}
		}

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool exportOdGeCurve(OdGeCurve3d* pOdcGeCurve, Va3cContainer::Va3cGeometry* pGeometry)
{
	OdGePoint3dArray points;
	pOdcGeCurve->getSamplePoints(nullptr, 0, points);
	int segmentCount = points.size() - 1;
	if (segmentCount < 1)
		return false;
	for (int i = 0; i < segmentCount; ++i)
	{
		OdGePoint3d startPoint = points[i];
		OdGePoint3d endPoint = points[i + 1];
		pGeometry->data.vertices.push_back((float)startPoint.x);
		pGeometry->data.vertices.push_back((float)startPoint.y);
		pGeometry->data.vertices.push_back((float)startPoint.z);
		pGeometry->data.vertices.push_back((float)endPoint.x);
		pGeometry->data.vertices.push_back((float)endPoint.y);
		pGeometry->data.vertices.push_back((float)endPoint.z);
		pGeometry->data.indices.push_back(pGeometry->data.points);
		pGeometry->data.indices.push_back(pGeometry->data.points + 1);
		pGeometry->data.points += 2;
		pGeometry->data.triangles += 1;
	}

	if (pOdcGeCurve->isClosed())
	{
		OdGePoint3d startPoint = points[points.size() - 1];
		OdGePoint3d endPoint = points[0];
		pGeometry->data.vertices.push_back((float)startPoint.x);
		pGeometry->data.vertices.push_back((float)startPoint.y);
		pGeometry->data.vertices.push_back((float)startPoint.z);
		pGeometry->data.vertices.push_back((float)endPoint.x);
		pGeometry->data.vertices.push_back((float)endPoint.y);
		pGeometry->data.vertices.push_back((float)endPoint.z);
		pGeometry->data.indices.push_back(pGeometry->data.points);
		pGeometry->data.indices.push_back(pGeometry->data.points + 1);
		pGeometry->data.points += 2;
		pGeometry->data.triangles += 1;
	}

	return true;
}

bool exportAcDbSolid()
{
	try
	{
		for (auto it = g_solidMap.begin(); it != g_solidMap.end(); ++it)
		{
			string layerName = it->first;
			if (g_layerObjects.find(layerName) != g_layerObjects.end())
			{
				for (auto itor = it->second.begin(); itor != it->second.end(); ++itor)
				{
					string materialId = itor->first;
					list<OdDbSolidPtr>& solids = itor->second;

					Va3cContainer::Va3cObject* pObject = newVa3cObject(OdDbSolid::desc()->name(), STR_MESH);
					Va3cContainer::Va3cGeometry* pGeometry = newVa3cGeometry(eGeometryCategory::Triangle, pObject);
					pObject->material = materialId;

					for (auto it = solids.begin(); it != solids.end(); ++it)
					{
						OdDbSolidPtr& solid = *it;
						OdGePoint3d point1, point2, point3, point4;
						solid->getPointAt(0, point1);
						solid->getPointAt(1, point2);
						solid->getPointAt(2, point3);
						solid->getPointAt(3, point4);

						int curNbPoints = pGeometry->data.points;
						pGeometry->data.vertices.push_back((float)point1.x);
						pGeometry->data.vertices.push_back((float)point1.y);
						pGeometry->data.vertices.push_back((float)point1.z);
						pGeometry->data.vertices.push_back((float)point2.x);
						pGeometry->data.vertices.push_back((float)point2.y);
						pGeometry->data.vertices.push_back((float)point2.z);
						pGeometry->data.vertices.push_back((float)point3.x);
						pGeometry->data.vertices.push_back((float)point3.y);
						pGeometry->data.vertices.push_back((float)point3.z);
						pGeometry->data.vertices.push_back((float)point4.x);
						pGeometry->data.vertices.push_back((float)point4.y);
						pGeometry->data.vertices.push_back((float)point4.z);
						pGeometry->data.indices.push_back(curNbPoints);
						pGeometry->data.indices.push_back(curNbPoints + 1);
						pGeometry->data.indices.push_back(curNbPoints + 2);
						pGeometry->data.indices.push_back(curNbPoints + 1);
						pGeometry->data.indices.push_back(curNbPoints + 2);
						pGeometry->data.indices.push_back(curNbPoints + 3);
						pGeometry->data.points += 4;
						pGeometry->data.triangles += 2;

						//立方体
						if (solid->thickness() > 1e-6)
						{
							OdGePoint3d point5 = point1 + solid->thickness() * solid->normal();
							OdGePoint3d point6 = point2 + solid->thickness() * solid->normal();
							OdGePoint3d point7 = point3 + solid->thickness() * solid->normal();
							OdGePoint3d point8 = point4 + solid->thickness() * solid->normal();
							pGeometry->data.vertices.push_back((float)point5.x);
							pGeometry->data.vertices.push_back((float)point5.y);
							pGeometry->data.vertices.push_back((float)point5.z);
							pGeometry->data.vertices.push_back((float)point6.x);
							pGeometry->data.vertices.push_back((float)point6.y);
							pGeometry->data.vertices.push_back((float)point6.z);
							pGeometry->data.vertices.push_back((float)point7.x);
							pGeometry->data.vertices.push_back((float)point7.y);
							pGeometry->data.vertices.push_back((float)point7.z);
							pGeometry->data.vertices.push_back((float)point8.x);
							pGeometry->data.vertices.push_back((float)point8.y);
							pGeometry->data.vertices.push_back((float)point8.z);
							pGeometry->data.indices.push_back(curNbPoints);
							pGeometry->data.indices.push_back(curNbPoints + 2);
							pGeometry->data.indices.push_back(curNbPoints + 4);
							pGeometry->data.indices.push_back(curNbPoints + 2);
							pGeometry->data.indices.push_back(curNbPoints + 4);
							pGeometry->data.indices.push_back(curNbPoints + 6);
							pGeometry->data.indices.push_back(curNbPoints);
							pGeometry->data.indices.push_back(curNbPoints + 1);
							pGeometry->data.indices.push_back(curNbPoints + 4);
							pGeometry->data.indices.push_back(curNbPoints + 1);
							pGeometry->data.indices.push_back(curNbPoints + 4);
							pGeometry->data.indices.push_back(curNbPoints + 5);
							pGeometry->data.indices.push_back(curNbPoints + 2);
							pGeometry->data.indices.push_back(curNbPoints + 3);
							pGeometry->data.indices.push_back(curNbPoints + 6);
							pGeometry->data.indices.push_back(curNbPoints + 3);
							pGeometry->data.indices.push_back(curNbPoints + 6);
							pGeometry->data.indices.push_back(curNbPoints + 7);
							pGeometry->data.indices.push_back(curNbPoints + 1);
							pGeometry->data.indices.push_back(curNbPoints + 3);
							pGeometry->data.indices.push_back(curNbPoints + 5);
							pGeometry->data.indices.push_back(curNbPoints + 3);
							pGeometry->data.indices.push_back(curNbPoints + 5);
							pGeometry->data.indices.push_back(curNbPoints + 7);
							pGeometry->data.indices.push_back(curNbPoints + 4);
							pGeometry->data.indices.push_back(curNbPoints + 5);
							pGeometry->data.indices.push_back(curNbPoints + 6);
							pGeometry->data.indices.push_back(curNbPoints + 5);
							pGeometry->data.indices.push_back(curNbPoints + 6);
							pGeometry->data.indices.push_back(curNbPoints + 7);
							pGeometry->data.points += 4;
							pGeometry->data.triangles += 10;
						}
					}

					g_layerObjects[layerName]->children.push_back(pObject);
				}
			}
		}

		return true;
	}
	catch (OdError err)
	{
		return false;
	}
}

bool exportAcDbText()
{
	try
	{
		for (auto it = g_textMap.begin(); it != g_textMap.end(); ++it)
		{
			for (auto itor = it->second.begin(); itor != it->second.end(); ++itor)
			{
				string materialId = itor->first;
				list<OdDbTextPtr>& texts = itor->second;

				for (auto it = texts.begin(); it != texts.end(); ++it)
				{
					Va3cContainer::Va3cObject* pObject = newVa3cObject(OdDbText::desc()->name(), STR_MESH);
					Va3cContainer::Va3cTextGeometry* pGeometry = new Va3cContainer::Va3cTextGeometry();
					pGeometry->uuid = NewGuid();
					pGeometry->type = "TextGeometry";
					g_pContainer->pmiGeometries.push_back(pGeometry);
					pObject->geometry = pGeometry->uuid;
					pObject->material = materialId;

					OdDbTextPtr& text = *it;
					if (!exportText(text, pObject, pGeometry))
						continue;

					g_pContainer->pmiObject.children.push_back(pObject);
				}
			}
		}

		return true;
	}
	catch (OdError err)
	{
		string desc = err.description();
		return false;
	}
}

bool exportText(OdDbTextPtr& text, Va3cContainer::Va3cObject* pObject, Va3cContainer::Va3cTextGeometry* pGeometry)
{
	string textString;
	for (int i = 0; i < text->textString().getLength(); ++i)
	{
		unsigned short c = text->textString()[i];
		if (c >= 0x4E00 && c <= 0x9FA5)
			pGeometry->isChina = true;
		char buf[64] = { 0 };
		sprintf(buf, "\\u%04X", c);
		textString += buf;
	}
	if (textString.empty())
		return false;
	pGeometry->text = textString;
	OdDbTextStyleTableRecordPtr font = text->textStyle().safeOpenObject(OdDb::kForRead);
	OdString fontName;
	bool bBold, bItalic;
	int charset, pitchAndFamily;
	font->font(fontName, bBold, bItalic, charset, pitchAndFamily);
	pGeometry->textFont.name = toUtf8(fontName);
	pGeometry->textFont.size = text->widthFactor() * text->height();
	pGeometry->textFont.style = bItalic ? "italic" : "normal";
	pGeometry->textFont.weight = bBold ? "bold" : "normal";

	OdGePoint3d position = text->position();
	OdGeVector3d normal = text->normal();
	OdGeVector3d direction = OdGeVector3d(cos(text->rotation()), sin(text->rotation()), 0);
	pObject->bHasMatrix = true;
	float u[3] = { direction.x, direction.y, direction.z };
	float n[3] = { normal.x, normal.y, normal.z };
	float v[3] = { n[1] * u[2] - n[2] * u[1]/*ny*uz - nz*uy*/,
				   n[2] * u[0] - n[0] * u[2]/*nz*ux - nx*uz*/,
				   n[0] * u[1] - n[1] * u[0]/*nx*uy - ny*ux*/ };

	pObject->matrix[0] = u[0];
	pObject->matrix[1] = u[1];
	pObject->matrix[2] = u[2];
	pObject->matrix[3] = 0;

	pObject->matrix[4] = v[0];
	pObject->matrix[5] = v[1];
	pObject->matrix[6] = v[2];
	pObject->matrix[7] = 0;

	pObject->matrix[8] = n[0];
	pObject->matrix[9] = n[1];
	pObject->matrix[10] = n[2];
	pObject->matrix[11] = 0;


	pObject->matrix[12] = position.x;
	pObject->matrix[13] = position.y;
	pObject->matrix[14] = position.z;
	pObject->matrix[15] = 1;

	return true;
}

void release()
{
	g_blockBeginMap.clear();
	g_blockEndMap.clear();
	g_arcMap.clear();
	g_circleMap.clear();
	g_ellipseMap.clear();
	g_lineMap.clear();
	g_rayMap.clear();
	g_splineMap.clear();
	g_xlineMap.clear();
	g_lightMap.clear();
	g_pointMap.clear();
	g_solidMap.clear();
	g_textMap.clear();
	g_traceMap.clear();
	g_attributeDefinitionMap.clear();
	g_attributeMap.clear();
	g_faceMap.clear();
	g_shapeMap.clear();
	g_sequenceEndMap.clear();
	g_3dPolylineVertexMap.clear();
	g_2dVertexMap.clear();
	g_polyFaceMeshVertexMap.clear();
	g_polygonMeshVertexMap.clear();
}