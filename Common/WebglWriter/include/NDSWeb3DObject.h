#pragma once

#include <string>
#include <vector>
#include <map>
#include <list>
using namespace std;
namespace ModelExporter
{
	const string STR_ElEMENT = "Element";
	const string STR_OBJECT = "obj";
	const string STR_GEOMETRY = "geom";
	const string STR_USERDATA = "userdata";
	const string STR_TEXTURE = "tex";
	const string STR_LINKS = "links";
	const string STR_BUFFER_CHUNKS = "bufferchunks";

	const string STR_METADATA_TYPE_OBJECT = "Object";
	const string STR_FLOAT_METADATA_VERSION = "6.2";
	const string STR_FLOAT_METADATA_VERSION_44 = "4.4";
	const string STR_FLOAT_METADATA_VERSION_45 = "4.5";
	const string STR_METADATA_GENERATOR = "NDS";


	const string STR_MESH = "Mesh";
	const string STR_LINE_STRIP = "LineStrip";
	const string STR_LINE_PIECES = "LinePieces";
	const string STR_LINK = "Link";
	const string STR_GEOMETRY_TYPE = "Geometry";
	const string STR_BUFFER_GEOMETRY_TYPE = "BufferGeometry";
	const string STR_TEXTGEOMETRY_TYPE = "TextGeometry";
	const string STR_THUMBNAIL = "thumbnail";

	const string STR_MESHPHONG_MATERIAL = "MeshPhongMaterial";
	const string STR_MESHSTANDARD_MATERIAL = "MeshStandardMaterial";
	const string STR_LINEBASIC_MATERIAL = "LineBasicMaterial";
	const string STR_POINT_MATERIAL = "PointMaterial";

	const string STR_FILE_FORMAT_JS = ".js";
	const string STR_FILE_FORMAT_BIN = ".bin";
	const string STR_FILE_FORMAT_JPG = ".jpg";

	const string STR_DECAL_MAPPING_PLANAR = "MapPlanar";
	const string STR_DECAL_MAPPING_CYLINDRICAL = "MapCylindrical";
	const string STR_DECAL_MAPPING_SPHERICAL = "MapSpherical";
	const string STR_DECAL_MAPPING_UV = "MapUV";

	const string STR_DECAL_PROJ_FORWARD = "ProjForward";
	const string STR_DECAL_PROJ_BACKWARD = "ProjBackward";
	const string STR_DECAL_PROJ_BOTH = "ProjBoth";

	const float FLOAT_BUMP_RADIO_HUNDRED = 0.01f;
	const float FLOAT_BUMP_RADIO_THUNSAND = 0.001f;
	const int GEO_BUFFER_CHUNK_LIMIT = 40000;
	const float EPSILON_WRITER = 1.0E-6;

	enum eViewDetailLevel
	{
		Undefined = 0,
		Coarse = 1,
		Medium = 2,
		Fine = 3,
	};

	enum eGeometryCategory
	{
		Triangle = 0,
		Line = 1,
		Point =2
	};

	enum eLightsPresetType
	{
		dark = 0,
		gray = 1,
		warm = 2,
		daylight = 3
	};
	enum eUnit
	{
		Meter = 0,
		Centimeter = 1,
		Millimeter = 2,
		Inch = 3,
		Feet = 4
	};

	struct Va3cContainer
	{
		struct Va3cMaterial
		{
			string type;
			string uuid;
			string name;
			string color; // 16777215
			string ambient; //16777215
			string specular; //1118481
			string emissive;
			string emissiveIntensity;
			string reflectivity;
			string refractionRatio;
			string shininess; // 30
			string opacity; // 1
			bool isMetal; // 0 or 1
			string mapDiffuse; // mapDiffuse
			string mapBump;// mapBump
			string bumpScale; // mapBump scale
			string linewidth;
			string pointSize;

			//for MeshStandardMaterial
			string roughness;//0.0 ~ 1.0
			string metalness;//0.0 ~ 1.0

			bool doubleSided;

			string transmissionFilter;  //transmission filter
            string illumination; /* 0		Color on and Ambient off
                                               1		Color on and Ambient on
                                               2		Highlight on
                                               3		Reflection on and Ray trace on
                                               4		Transparency: Glass on
                                                        Reflection: Ray trace on
                                                5		Reflection: Fresnel on and Ray trace on
                                                6		Transparency: Refraction on
                                                        Reflection: Fresnel off and Ray trace on
                                                7		Transparency: Refraction on
                                                        Reflection: Fresnel on and Ray trace on
                                                8		Reflection on and Ray trace off
                                                9		Transparency: Glass on
                                                        Reflection: Ray trace off
                                                10		Casts shadows onto invisible surfaces*/
            string mapSpecular;      //specular map

			Va3cMaterial()
			{
				isMetal = true;
				doubleSided = false;
				color = "16777215"; //default color				
			}
		};
		struct Va3cDecal
		{
			string uuid;
			string mappingType;
			string projectionType;
			float origin[3];
			float vecX[3];
			float vecY[3];
			string opacity;
			string textureFile;
		};

		struct Va3cBox3D
		{
			float minPt[3];
			float maxPt[3];
			Va3cBox3D()
			{
				minPt[2] = minPt[1] = minPt[0] = maxPt[2] = maxPt[1] = maxPt[0] = 0.0f;
			}
		};

		struct Va3cGeometryData
		{
			vector<float> vertices; // millimetres
			vector<unsigned char> vertexColors;
			vector<float> normals;
			vector<float> uvs;
			vector<int> indices; // indices into Vertices + Materials

			vector<int> skinIndices;
			vector<float> skinWeights;
			int influencesPerVertex;

			eGeometryCategory nGeometryCategory;
			Va3cBox3D box3D;
						
			int triangles;
			int points;
			int vertexIndexBase;
			Va3cGeometryData()
			{
				influencesPerVertex = 2;
				nGeometryCategory = Triangle;
				triangles = 0;
				points = 0;
				vertexIndexBase = 0;
			}
		};
		struct Va3cBone
		{
			int parent;
			float pos[3];
			float rotq[4];
			float scl[3];
			string name;
			Va3cBone()
			{
				parent = -1;
			}
		};
		struct Va3cBoneKey
		{
			float time;
			float pos[3];
			float rotq[4];
			float scl[3];
		};
		struct Va3cHierarchy
		{
			int parent;
			list<Va3cBoneKey> keys;
		};
		struct Va3cSkeletonAnimation
		{
			float length;//动画时长
			int fps; //帧率
			string name;
			string uuid;
			vector<Va3cHierarchy> hierarchys;
			Va3cSkeletonAnimation()
			{
				length = 0;
				fps = 0;
			}
		};
		struct Va3cMorphTarget
		{
			string name;
			vector<float> vertices;
		};
		struct Va3cMorphInfo
		{
			string uuid;
			float duration;
			int fps;
		};
		struct Va3cTextFont
		{
			string name;//字体名称
			float size;//实际物理大小
			string weight;//是否加粗：normal  bold 
			string style;//是否斜体: normal italic
			Va3cTextFont()
			{
				size = 4.23; //default 4.23 mm (12pt)
				weight = "normal";
				style = "normal";
			}

		};
		
		struct Va3cGeometry
		{
			string uuid;
			string type; // "Geometry" "TextGeometry"
			Va3cGeometryData data;
			vector<Va3cBone> bones;
			vector<Va3cSkeletonAnimation> animations;
			vector<Va3cMorphTarget> morphTargets;
			vector<Va3cMorphInfo> morphInfos;
			Va3cGeometry()
			{
				type.assign(STR_BUFFER_GEOMETRY_TYPE);
			}
		};
		
		struct Va3cTextGeometry :public Va3cGeometry
		{
			string text;
			Va3cTextFont textFont; 
			bool isChina;
			Va3cTextGeometry()
			{
				isChina = false;
				Va3cGeometry();
				type.assign(STR_TEXTGEOMETRY_TYPE);
			}
		};

		struct Va3cSubGeometry
		{
			Va3cGeometry *parent;
			int id;
			int refCount;
			int firstLeafNode;
			Va3cGeometryData data;
			Va3cSubGeometry()
			{
				parent = NULL;
				id = 0;
				refCount = 0;
				firstLeafNode = -1;
			}
			~Va3cSubGeometry()
			{
				parent = NULL;
			}
		};

		struct Va3cMatrix
		{
			Va3cMatrix()
			{
				for (int i = 0; i < 16; ++i)
					matrix[i] = 0.0f;
				matrix[0] = matrix[5] = matrix[10] = matrix[15] = 1.0f;
			}
			Va3cMatrix(const float value[16])
			{
				for (int i = 0; i < 16; ++i)
					matrix[i] = value[i];
			}
			Va3cMatrix RightMultiply(Va3cMatrix &rhs)
			{
				Va3cMatrix lhs;
				for (int row = 0; row < 4; ++row)
				{
					for (int col = 0; col < 4; ++col)
					{
						lhs.matrix[row*4+col] = matrix[row*4]  *rhs.matrix[col] +
							                    matrix[row*4+1]*rhs.matrix[4+col] +
												matrix[row*4+2]*rhs.matrix[8+col] +
												matrix[row*4+3]*rhs.matrix[12+col];
					}
				}
				return lhs;
			}
			void LeftMultiply(const float point[3], float pointOut[3]) // xyz, w=1
			{
				for (int col = 0; col < 3; ++col)
				{
					pointOut[col] = point[0]*matrix[col]+
									point[1]*matrix[4+col]+
									point[2]*matrix[8+col]+
									matrix[12+col];
				}
			}

			void LeftMultiply(const Va3cBox3D &box3D, Va3cBox3D &box3DOut)
			{
				const float (&minPoint)[3] = box3D.minPt;
				const float (&maxPoint)[3] = box3D.maxPt;
				float points[8][3] = {{minPoint[0], minPoint[1], minPoint[2]},
									  {maxPoint[0], minPoint[1], minPoint[2]},
									  {maxPoint[0], maxPoint[1], minPoint[2]},
									  {minPoint[0], maxPoint[1], minPoint[2]},
									  {minPoint[0], minPoint[1], maxPoint[2]},
									  {maxPoint[0], minPoint[1], maxPoint[2]},
									  {maxPoint[0], maxPoint[1], maxPoint[2]},
									  {minPoint[0], maxPoint[1], maxPoint[2]}};
				float (&newMinPt)[3] = box3DOut.minPt;
				float (&newMaxPt)[3] = box3DOut.maxPt;
				LeftMultiply(points[0], newMinPt);
				LeftMultiply(points[0], newMaxPt);
				float newPoint[3] = {0};
				for (int kk = 1; kk < 8; ++kk)
				{
					LeftMultiply(points[kk], newPoint);
					for (int nn = 0; nn < 3; ++nn)
					{
						if (newPoint[nn] < newMinPt[nn])
							newMinPt[nn] = newPoint[nn];
						else if (newPoint[nn] > newMaxPt[nn])
							newMaxPt[nn] = newPoint[nn];
					}
				}
			}

			float matrix[16]; 
		};

		struct Va3cPrimitive // bounding volume hierarchy primitives
		{
			Va3cSubGeometry *geom;
			string matrixId;    // transformation
			Va3cBox3D box3D;    // transformed geom bounding box
			string material;
			string bodyId; 
			string componentId;
			float surfaceArea;
			int leafNodeId;
			Va3cPrimitive()
			{
				geom = NULL;
				surfaceArea = 0.0f;
				leafNodeId = -1;
			}
			~Va3cPrimitive()
			{
				geom = NULL;
			}
		};

		struct Va3cObject
		{
			string uuid;
			string name; // BIM <document name>
			string type; // Object3D
			string linkID;
			string linkPath;
			float matrix[16]; // [1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1]
			bool bHasMatrix;
			bool bVisible;
			list<Va3cObject*> children;

			bool isTangentEdgeObject;
			// The following are only on the children:
			string geometry;
			string material;
			list<string> decals;
			map<string, string> userData;

			string propertyfile; //the name of property file

			Va3cObject()
			{
				type.assign("Object3D");
				bHasMatrix = false;
				bVisible = true;

				isTangentEdgeObject = false;

				memset(matrix,0, sizeof(float)*16);
				matrix[0] = 1.0;
				matrix[5] = 1.0;
				matrix[10] = 1.0;
				matrix[15] = 1.0;
			}

			~Va3cObject()
			{
				for (list<Va3cObject*>::iterator iterChild = children.begin(); iterChild != children.end(); ++iterChild)
				{
					if (*iterChild)
					{
						delete *iterChild;
						*iterChild = NULL;
					}
				}
				children.clear();
			}
		};

		// https://github.com/mrdoob/three.js/wiki/JSON-Model-format-3

		// for the faces, we will use
		// triangle with material
		// 00 00 00 10 = 2
		// 2, [vertex_index, vertex_index, vertex_index], [material_index]     // e.g.:
		//
		//2, 0,1,2, 0

		struct Va3cVector3Key
		{
			float value[3];
			float time;
		};

		struct Va3cVector4Key
		{
			float value[4];
			float time;
		};

		struct Va3cVector3Track
		{
			string name;
			list<Va3cVector3Key> keys;
		};

		struct Va3cVector4Track
		{
			string name;
			list<Va3cVector4Key> keys;
		};

		struct Va3cSurface
		{
			string type; //"plane","cylinder","cone","sphere","torus","nurbs", "other"
		};
		struct Va3cPlane:Va3cSurface
		{
			float planeOrigin[3];
			float planeNormal[3]; //used when the type is plane
			Va3cPlane()
			{
				type = "plane";
			}
		};
		struct Va3cCylinder:Va3cSurface{
			float radius;    
			float origin[3]; 
			float axis[3];
			Va3cCylinder()
			{
				type = "cylinder";
			}
		};
		struct Va3cFace{
			string id;
			int TriIndexRange[2]; //start and end tri index in the geometry contain the face
			
			string area; //face area
			string perimeter; //face perimeter
			
			Va3cSurface* pSurf;
			Va3cFace()
			{
				pSurf = NULL;
			}
			~Va3cFace()
			{
				if(pSurf)
				{
					delete pSurf;
					pSurf = NULL;
				}					 
			}
		};

		struct Va3cFaceGroup{
			string refMeshUUID;
			list<Va3cFace *> faces;

			Va3cFaceGroup()
			{

			}

			~Va3cFaceGroup()
			{
				for (list<Va3cFace*>::iterator iterChild = faces.begin(); iterChild != faces.end(); ++iterChild)
				{
					if (*iterChild)
					{
						delete *iterChild;
						*iterChild = NULL;
					}
				}
				faces.clear();
			}

		};

		struct Va3cCurve
		{
			string type; //"line","circle","ellipse","nurbs", "other"
		};

		struct Va3cCircle:Va3cCurve
		{
			float radius;
			float center[3];
			Va3cCircle()
			{
				type = "circle";
			}
		};
		struct Va3cEdge{
			string id;
			int linesegIndexRange[2]; // start and end lineseg index in the geometry contain the edge

			string length; //edge length

			Va3cCurve* pCurve;
			Va3cEdge()
			{
				pCurve = NULL;
			}
			~Va3cEdge()
			{
				if(pCurve)
				{
					delete pCurve;
					pCurve = NULL;
				}					 
			}

		};

		struct Va3cEdgeGroup{
			string refMeshUUID;
			list<Va3cEdge *> edges;

			Va3cEdgeGroup()
			{

			}

			~Va3cEdgeGroup()
			{
				for (list<Va3cEdge*>::iterator iterChild = edges.begin(); iterChild != edges.end(); ++iterChild)
				{
					if (*iterChild)
					{
						delete *iterChild;
						*iterChild = NULL;
					}
				}
				edges.clear();
			}

		};

		struct Va3cBody{
			string uuid;
			list<Va3cFaceGroup*> faceGroups;
			list<Va3cEdgeGroup*> edgeGroups;

			Va3cBody()
			{

			}
			~Va3cBody()
			{
				for (list<Va3cFaceGroup*>::iterator iterChild = faceGroups.begin(); iterChild != faceGroups.end(); ++iterChild)
				{
					if (*iterChild)
					{
						delete *iterChild;
						*iterChild = NULL;
					}
				}
				faceGroups.clear();

				for (list<Va3cEdgeGroup*>::iterator iterChild = edgeGroups.begin(); iterChild != edgeGroups.end(); ++iterChild)
				{
					if (*iterChild)
					{
						delete *iterChild;
						*iterChild = NULL;
					}
				}
				edgeGroups.clear();
			}

		};
		//硬动画定义
		struct Va3cSolidAnimation
		{
			string fps;
			string name;
			string uuid;
			string duration;
			Va3cObject *pAnimationObject;

			list<Va3cVector3Track *> translationTracks;
			list<Va3cVector3Track *> scaleTracks;
			list<Va3cVector4Track *> quaternionTracks;

			Va3cSolidAnimation() {
				pAnimationObject = NULL;
			}

			~Va3cSolidAnimation()
			{

				if (pAnimationObject)
				{
					delete pAnimationObject;
					pAnimationObject = NULL;
				}

				for (list<Va3cVector3Track *>::iterator iterChild = translationTracks.begin(); iterChild != translationTracks.end(); ++iterChild)
				{
					if (*iterChild)
					{
						delete *iterChild;
						*iterChild = NULL;
					}
				}
				translationTracks.clear();

				for (list<Va3cVector3Track *>::iterator iterChild = scaleTracks.begin(); iterChild != scaleTracks.end(); ++iterChild)
				{
					if (*iterChild)
					{
						delete *iterChild;
						*iterChild = NULL;
					}
				}
				scaleTracks.clear();

				for (list<Va3cVector4Track *>::iterator iterChild = quaternionTracks.begin(); iterChild != quaternionTracks.end(); ++iterChild)
				{
					if (*iterChild)
					{
						delete *iterChild;
						*iterChild = NULL;
					}
				}
				quaternionTracks.clear();

			}


		};





		struct Metadata
		{
			string type;//  "Object"
			string version;// 4.3
			string generator; //  "Revit Exporter"
			Metadata()
			{
				type.assign("Object");
				version = STR_FLOAT_METADATA_VERSION;
			}
		};

		Metadata metadata;
		Va3cObject object;
		list<Va3cGeometry*> geometries;
		list<Va3cMaterial*> materials;

		Va3cObject pmiObject;
		list<Va3cGeometry*> pmiGeometries;
		list<Va3cMaterial*> pmiMaterials;



		list<Va3cBody*> brepBodies;

		list<Va3cDecal*> decals;
		list<Va3cSolidAnimation*> solidAnimtaions;
		string lightsPreset;
		string fullPath;
		string buffer;
		long bufferlenth;
		bool compress;
		eUnit unit;
		string propertyfile; //the name of property file
		Va3cContainer()
		{
			bufferlenth = 0;
			compress = false;
			unit = Millimeter;
		}

		~Va3cContainer()
		{
			for (list<Va3cGeometry*>::iterator iterChild = geometries.begin(); iterChild != geometries.end(); ++iterChild)
			{
				if (*iterChild)
				{
					delete *iterChild;
					*iterChild = NULL;
				}
			}
			geometries.clear();

			for (list<Va3cMaterial*>::iterator iterChild = materials.begin(); iterChild != materials.end(); ++iterChild)
			{
				if (*iterChild)
				{
					delete *iterChild;
					*iterChild = NULL;
				}
			}
			materials.clear();
			for(list<Va3cGeometry*>::iterator iterChild = pmiGeometries.begin();iterChild != pmiGeometries.end();++iterChild)
			{
				if(*iterChild)
				{
					delete *iterChild;
					*iterChild = NULL;
				}
			}
			pmiGeometries.clear();
			for (list<Va3cMaterial*>::iterator iterChild = pmiMaterials.begin(); iterChild != pmiMaterials.end(); ++iterChild)
			{
				if (*iterChild)
				{
					delete *iterChild;
					*iterChild = NULL;
				}
			}
			pmiMaterials.clear();

			for(list<Va3cDecal*>::iterator iterChild = decals.begin(); iterChild != decals.end(); ++ iterChild)
			{
				if(*iterChild)
				{
					delete *iterChild;
					*iterChild = NULL;
				}
			}
			decals.clear();

			for (list<Va3cSolidAnimation*>::iterator iterChild = solidAnimtaions.begin(); iterChild != solidAnimtaions.end(); ++iterChild)
			{
				if (*iterChild)
				{
					delete *iterChild;
					*iterChild = NULL;
				}
			}
			solidAnimtaions.clear();

			for (list<Va3cBody*>::iterator iterChild = brepBodies.begin(); iterChild != brepBodies.end(); ++iterChild)
			{
				if (*iterChild)
				{
					delete *iterChild;
					*iterChild = NULL;
				}
			}
			brepBodies.clear();

		}
	};
}

