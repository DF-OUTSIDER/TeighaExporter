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
#include "COLLADAFWTypes.h"
#include "COLLADAFWGeometry.h"
#include "COLLADAFWTriangles.h"
#include "COLLADAFWTristrips.h"
#include "COLLADAFWTrifans.h"
#include "COLLADAFWPolygons.h"
#include "COLLADAFWUniqueId.h"
#include "ColladaDocumentImporter.h"
#include "ColladaGeometryImporter.h"


namespace TD_COLLADA_IMPORT
{
	bool isSupportedMeshPrimitive(const COLLADAFW::MeshPrimitive* meshPrimitive)
	{
		COLLADAFW::MeshPrimitive::PrimitiveType type = meshPrimitive->getPrimitiveType();
		return    (type == COLLADAFW::MeshPrimitive::TRIANGLES) 
			|| (type == COLLADAFW::MeshPrimitive::TRIANGLE_FANS) 
			|| (type == COLLADAFW::MeshPrimitive::TRIANGLE_STRIPS) 
			|| (type == COLLADAFW::MeshPrimitive::POLYGONS) 
			|| (type == COLLADAFW::MeshPrimitive::POLYLIST); 
	}

  GeometryImporter::GeometryImporter( DocumentImporter* documentImporter )
    : ImporterBase(documentImporter)
  {}

  GeometryImporter::~GeometryImporter()
  {}

	bool GeometryImporter::import( const COLLADAFW::Geometry* pGeometry )
	{
    bool bSuccess = true;
		if ( pGeometry->getType() == COLLADAFW::Geometry::GEO_TYPE_MESH )
		{
			bSuccess = importMesh(pGeometry);
		}
		return bSuccess;
	}

	bool GeometryImporter::importMesh( const COLLADAFW::Geometry* pGeometry )
	{
		COLLADAFW::Mesh* pMesh = (COLLADAFW::Mesh*) pGeometry;

		bool bSuccess = true;
		if ( pMesh->getPolygonsPolygonCount() > 0 || pMesh->getTrianglesTriangleCount() > 0 || pMesh->getTristripsTriangleCount() > 0 || pMesh->getTrifansTriangleCount() > 0 )
		{
			bSuccess = importDbSubDMesh(pMesh);
		}

 		return bSuccess;
	}


	bool GeometryImporter::importDbSubDMesh(COLLADAFW::Mesh* pMesh)
	{
    OdGePoint3dArray vertexArray; 	    
    OdInt32Array     faceArray;
    OdResult res = eOk;
		if ( importDbSubDMeshData(vertexArray, faceArray, pMesh) )
    {
      OdDbBlockTableRecordPtr pBTR = this->getDocumentImporter()->getGeometryTmpBTR();
      OdDbSubDMeshPtr pSubDMesh = OdDbSubDMesh::createObject();
      pSubDMesh->setDatabaseDefaults(pBTR->database());
      OdDbObjectId objId = pBTR->appendOdDbEntity(pSubDMesh);
      OdResult res = pSubDMesh->setSubDMesh(vertexArray, faceArray, 0);
      if (eOk == res)
      {

        OdGePoint3dArray  txtArrSrc;
        OdGeVector3dArray normArrSrc;
        if ( !importDbSubDMeshDataNormals(pMesh, pSubDMesh, normArrSrc) )
          res = eInvalidInput;

        int iSourceIndex = 0; //It should be tested when several images in a material.
        if(!importDbSubDMeshDataUVCoords(pMesh, pSubDMesh, iSourceIndex, txtArrSrc))
          res = eInvalidInput;

        if (isImportTextures())
        {
          OdGePoint3dArray  vertArrNew, txtArrNew;
          OdGeVector3dArray normArrNew;
          OdInt32Array faceArrayNew;

          if (vertexArray.size()>0)
          {          
            COLLADAFW::MeshPrimitiveArray& meshPrimitiveArray = pMesh->getMeshPrimitives();
            size_t faceIndex = 0;

            OdUInt32 count = (OdUInt32)meshPrimitiveArray.getCount();
            vertArrNew.reserve(count);
            txtArrNew.reserve(count);
            normArrNew.reserve(count);
            faceArrayNew.reserve(count + count/2);
            for ( size_t i = 0; i < count; ++i)
            {
              const COLLADAFW::MeshPrimitive* meshPrimitive = meshPrimitiveArray[i];
              if ( ! meshPrimitive )
                continue;
              switch ( meshPrimitive->getPrimitiveType() )
              {
              case COLLADAFW::MeshPrimitive::TRIANGLES:
                {
                  const COLLADAFW::Triangles* triangles = (const COLLADAFW::Triangles*) meshPrimitive;

                  const COLLADAFW::UIntValuesArray& positionIndices = triangles->getPositionIndices();
                  const COLLADAFW::UIntValuesArray& normalIndices   = triangles->getNormalIndices();
                  const COLLADAFW::IndexListArray& uvIndexArray     = triangles->getUVCoordIndicesArray();
                  if (uvIndexArray.getCount() > 0)
                  {
                    const COLLADAFW::UIntValuesArray& uvIndices = uvIndexArray[iSourceIndex]->getIndices();

                    size_t iCount = positionIndices.getCount();
                    for (size_t iCurrentTriangle = 0; iCurrentTriangle < iCount; iCurrentTriangle += 3)
                    {
                      faceArrayNew.append(3);
                      for (size_t iCurrentIndex = 0; iCurrentIndex < 3; iCurrentIndex++)
                      {
                        faceArrayNew.append(vertArrNew.append(vertexArray[positionIndices[iCurrentTriangle + iCurrentIndex]]));
                        if (normArrSrc.size()>0)
                          normArrNew.append(normArrSrc[normalIndices[iCurrentTriangle + iCurrentIndex]]);
                        if (txtArrSrc.size()>0)
                        {
                          txtArrNew.append(txtArrSrc[uvIndices[iCurrentTriangle + iCurrentIndex]]);
                        }
                      }
                    }
                    if(txtArrSrc.size() > 0)
                    {
                      addObjectMaterialIdPair(objId, meshPrimitive->getMaterialId());
                    }
                  }
                  break;
                }
              case COLLADAFW::MeshPrimitive::TRIANGLE_STRIPS:
              case COLLADAFW::MeshPrimitive::TRIANGLE_FANS:
                {
                  ODA_ASSERT_ONCE(false); //detect this case.
                  break;
                }
              case COLLADAFW::MeshPrimitive::POLYGONS:
                {
                  const COLLADAFW::Polygons* polygons = (const COLLADAFW::Polygons*) meshPrimitive;
                  const COLLADAFW::UIntValuesArray& positionIndices =  polygons->getPositionIndices();
                  const COLLADAFW::UIntValuesArray& normalIndices   =  polygons->getNormalIndices();
  
                  const COLLADAFW::IndexListArray& uvIndexArray = polygons->getUVCoordIndicesArray();
                  if (uvIndexArray.getCount() > 0)
                  {
                    const COLLADAFW::UIntValuesArray& uvIndices = uvIndexArray[iSourceIndex]->getIndices();
                    const COLLADAFW::IntValuesArray& faceVertexCountArray = polygons->getGroupedVerticesVertexCountArray();
                    size_t currentIndex = 0;
                    for ( size_t j = 0, countPl = faceVertexCountArray.getCount() ; j < countPl; ++j )
                    {
                      OdUInt32 faceVertexCount = (OdUInt32)faceVertexCountArray[j];
                      if ( faceVertexCount <= 0 )
                        continue;
                      unsigned int* pVertInd = (unsigned int*)(&positionIndices[currentIndex]);
                      unsigned int* pNormInd = (unsigned int*)(&normalIndices[currentIndex]);
                      unsigned int* pTextInd = (unsigned int*)(&uvIndices[currentIndex]);

                      faceArrayNew.append(faceVertexCount);
                      for (size_t ind = 0; ind < faceVertexCount; ++ind)
                      {
                        faceArrayNew.append(vertArrNew.append(vertexArray[*(pVertInd + ind)]));
                        if (normArrSrc.size()>0)
                          normArrNew.append(normArrSrc[*(pNormInd + ind)]);
                        if (txtArrSrc.size() > 0)
                        {
                          txtArrNew.append(txtArrSrc[*(pTextInd + ind)]);
                        }
                      }
                      if (txtArrSrc.size() > 0)
                      {
                        addObjectMaterialIdPair(objId, meshPrimitive->getMaterialId());
                      }
                      currentIndex += faceVertexCount;
                      ++faceIndex;
                    }
                  }
                  break;
                }
              }
            }
          }

          if (vertArrNew.size()>0)
          {
            res = pSubDMesh->setSubDMesh(vertArrNew, faceArrayNew, 0);
            if (eOk == res && txtArrNew.size()>0 )
            {
              res = pSubDMesh->setVertexTextureArray(txtArrNew);
              if (eOk == res && normArrNew.size()>0)
                res = pSubDMesh->setVertexNormalArray(normArrNew);
            }            
          }
        }//if (isImportTextures())

        if (eOk == res)
        {
          handleObjectReferences(pMesh, objId);
        }
      }
    }
    else
      res = eInvalidInput;
		return eOk == res;
	}


	bool GeometryImporter::importDbSubDMeshData( OdGePoint3dArray& vertexArray, OdInt32Array& faceArray, COLLADAFW::Mesh* pMesh)
	{
    const COLLADAFW::MeshVertexData& meshPositions = pMesh->getPositions();
    OdUInt32 positionsCount = (OdUInt32)meshPositions.getValuesCount();
    size_t i = 0;
    vertexArray.resize(positionsCount / 3);
    if ( meshPositions.getType() == COLLADAFW::MeshVertexData::DATA_TYPE_DOUBLE )
    {
      const COLLADAFW::DoubleArray* positionsArray = meshPositions.getDoubleValues();
      for ( OdUInt32 iArr = 0; i < positionsCount; ++iArr, i+=3)
      {
        vertexArray[iArr].x = (*positionsArray)[i];
        vertexArray[iArr].y = (*positionsArray)[i+1];
        vertexArray[iArr].z = (*positionsArray)[i+2];
      }
    }
    else
    {
      const COLLADAFW::FloatArray* positionsArray = meshPositions.getFloatValues();
      for ( OdUInt32 iArr = 0; i < positionsCount; ++iArr, i+=3)
      {
        vertexArray[iArr].x = (*positionsArray)[i];
        vertexArray[iArr].y = (*positionsArray)[i+1];
        vertexArray[iArr].z = (*positionsArray)[i+2];
      }
    }
    

    COLLADAFW::MeshPrimitiveArray& meshPrimitiveArray =  pMesh->getMeshPrimitives();
    size_t faceIndex = 0;
    size_t count = meshPrimitiveArray.getCount();
    for ( i = 0; i < count; ++i )
    {
      const COLLADAFW::MeshPrimitive* meshPrimitive = meshPrimitiveArray[i];
      if ( ! meshPrimitive )
        continue;
      switch (meshPrimitive->getPrimitiveType())
      {
      case COLLADAFW::MeshPrimitive::TRIANGLES:
        {
          const COLLADAFW::UIntValuesArray& positionIndices =  ((const COLLADAFW::Triangles*) meshPrimitive)->getPositionIndices();
          OdUInt32 iCount = (OdUInt32)positionIndices.getCount();
          OdUInt32 oldSize = faceArray.size();
          faceArray.resize(oldSize + iCount + iCount/3);
          for ( OdUInt32 iN = oldSize, j = 0; j < iCount; j+=3 )
          {
            faceArray[iN++] = 3;
            faceArray[iN++] = positionIndices[j];
            faceArray[iN++] = positionIndices[j + 1];
            faceArray[iN++] = positionIndices[j + 2];
          }
          break;
        }
      case COLLADAFW::MeshPrimitive::TRIANGLE_STRIPS:
      case COLLADAFW::MeshPrimitive::TRIANGLE_FANS:
        {
          ODA_ASSERT_ONCE(false); //detect this case.
          break;
        }
      case COLLADAFW::MeshPrimitive::POLYGONS:
        {
          const COLLADAFW::Polygons* polygons = (const COLLADAFW::Polygons*) meshPrimitive;
          const COLLADAFW::UIntValuesArray& positionIndices =  polygons->getPositionIndices();
          const COLLADAFW::IntValuesArray& faceVertexCountArray = polygons->getGroupedVerticesVertexCountArray();
          OdInt32 iCurrentIndex = 0;
          OdInt32 iCount = (OdInt32)faceVertexCountArray.getCount();
          OdInt32 faceVertexCount = 0;
          faceArray.reserve(faceArray.size() + iCount);
          for ( size_t j = 0; j < iCount; ++j )
          {
            faceVertexCount = faceVertexCountArray[j];
            if ( faceVertexCount < 1 )
              continue;
            faceArray.push_back(faceVertexCount);
            faceVertexCount = iCurrentIndex + faceVertexCount;
            for ( ; iCurrentIndex < faceVertexCount; iCurrentIndex++ )
              faceArray.push_back(positionIndices[iCurrentIndex]);
          }
          break;
        }
      default:
        continue;
      }
    }
		return true;
	} 


  template<class NumberArray> 
  void GeometryImporter::setPolygonMeshUVVertices( const NumberArray& uvArray, size_t stride, size_t startPosition, size_t vertsCount, OdGePoint3dArray& vertexTextureArray )
  {
    size_t uvIndex = startPosition;
    switch ( stride )
    {
    case 1:
      {
        for ( OdUInt32 i = 0; i < vertsCount; ++i)
        {
          vertexTextureArray[i].x = uvArray[uvIndex++];
          vertexTextureArray[i].y = 0.0f;
          vertexTextureArray[i].z = 0.0f;
        }
       break;
      }
    case 2:
      {
        for ( OdUInt32 i = 0; i < vertsCount; ++i)
        {
          vertexTextureArray[i].x = uvArray[uvIndex++];
          vertexTextureArray[i].y = 1.0f - uvArray[uvIndex++]; // Ac uses inverted coord
          vertexTextureArray[i].z = 0.0f;
        }
        break;
      }
    case 3:
      {
        for ( OdUInt32 i = 0; i < vertsCount; ++i)
        {
          vertexTextureArray[i].x = uvArray[uvIndex++];
          vertexTextureArray[i].y = 1.0f - uvArray[uvIndex++]; // Ac uses inverted coord
          vertexTextureArray[i].z = uvArray[uvIndex++];
        }
        break;
      }
    case 4:
      {
        for ( OdUInt32 i = 0; i < vertsCount; ++i)
        {
          vertexTextureArray[i].x = uvArray[uvIndex++];
          vertexTextureArray[i].y = 1.0f - uvArray[uvIndex++]; // Ac uses inverted coord
          vertexTextureArray[i].z = uvArray[uvIndex++];
          uvIndex++;
        }
        break;
      }
    default:
      ODA_ASSERT(false);
    }
  }

  void GeometryImporter::fillPolygonMeshMapPerSet( const COLLADAFW::MeshVertexData& uvCoordinates,
                                                   const COLLADAFW::MeshVertexData::InputInfosArray& inputInfos,
                                                   size_t sourceIndex,
                                                   OdDbSubDMeshPtr pSubDMesh,
                                                   OdGePoint3dArray& vertexTextureArray)
  {
    ODA_ASSERT(inputInfos.getCount() == 1);
    const COLLADAFW::MeshVertexData::InputInfos* inputInfo = inputInfos[ sourceIndex ];
    
    size_t stride = inputInfo->mStride;
    size_t vertsCount = inputInfo->mLength / stride;

    OdInt32 iNumVert = 0;
    if((eOk == pSubDMesh->numOfVertices(iNumVert) && (vertsCount == iNumVert)) || isImportTextures() )
    {
      // calculate first index position
      size_t startPosition = 0;
      for ( size_t i = 0; i < sourceIndex; ++i)
      {
        const COLLADAFW::MeshVertexData::InputInfos* inputInfo = inputInfos[ i ];
        startPosition += inputInfo->mLength;
      }

      vertexTextureArray.resize((OdUInt32)vertsCount);
      if ( uvCoordinates.getType() == COLLADAFW::MeshVertexData::DATA_TYPE_DOUBLE )
      {
        const COLLADAFW::DoubleArray& uvArray = *uvCoordinates.getDoubleValues();
        setPolygonMeshUVVertices(uvArray, stride, startPosition, vertsCount, vertexTextureArray);
      }
      else
      {
        const COLLADAFW::FloatArray& uvArray = *uvCoordinates.getFloatValues();
        setPolygonMeshUVVertices(uvArray, stride, startPosition, vertsCount, vertexTextureArray);
      }

      if((vertexTextureArray.size() > 0)&& (vertsCount == iNumVert))
        pSubDMesh->setVertexTextureArray(vertexTextureArray);
    }
    else
      printf("Collada import. Number of vertexes is not equal number of texture coordinates.");
  }

  bool GeometryImporter::importDbSubDMeshDataUVCoords(COLLADAFW::Mesh* pMesh, OdDbSubDMeshPtr pSubDMesh, int iSourceIndex, OdGePoint3dArray& vertexTextureArray)
  {
    const COLLADAFW::MeshVertexData& uvCoordinates = pMesh->getUVCoords();
    size_t iUVCoordinatesCount = uvCoordinates.getValuesCount();
    if (iUVCoordinatesCount > 0)
    {
      const COLLADAFW::MeshVertexData::InputInfosArray& uvInputInfos = uvCoordinates.getInputInfosArray();
      fillPolygonMeshMapPerSet( uvCoordinates, uvInputInfos, iSourceIndex, pSubDMesh, vertexTextureArray);
    }
    return true;
  }

  bool GeometryImporter::importDbSubDMeshDataNormals(COLLADAFW::Mesh* pMesh, OdDbSubDMeshPtr pSubDMesh, OdGeVector3dArray& vertexNormalsArray)
  {
    if ( !pMesh->hasNormals() )
      return true;

    // fill in the normals
    const COLLADAFW::MeshVertexData& meshNormals = pMesh->getNormals();
    size_t normalCount = meshNormals.getValuesCount()/3;

    OdInt32 iNumVert = 0;
    if((eOk == pSubDMesh->numOfVertices(iNumVert) && (normalCount == iNumVert)) || isImportTextures() )
    {
      vertexNormalsArray.resize((OdUInt32)normalCount);
      if ( meshNormals.getType() == COLLADAFW::MeshVertexData::DATA_TYPE_DOUBLE )
      {
        const COLLADAFW::DoubleArray* normalsArray = meshNormals.getDoubleValues();
        for ( OdUInt32 i = 0; i < normalCount; ++i)
        {          
          vertexNormalsArray[i].x = (*normalsArray)[i*3];
          vertexNormalsArray[i].y = (*normalsArray)[i*3 + 1];
          vertexNormalsArray[i].z = (*normalsArray)[i*3 + 2];
        }
      }
      else
      {
        const COLLADAFW::FloatArray* normalsArray = meshNormals.getFloatValues();
        for ( OdUInt32 i = 0; i < normalCount; ++i)
        {
          vertexNormalsArray[i].x = (*normalsArray)[i*3];
          vertexNormalsArray[i].y = (*normalsArray)[i*3 + 1];
          vertexNormalsArray[i].z = (*normalsArray)[i*3 + 2];
        }
      }

      if((vertexNormalsArray.size() > 0) && (normalCount == iNumVert))
        pSubDMesh->setVertexNormalArray(vertexNormalsArray);
    }
    else
      printf("Collada import. Number of vertexes is not equal number of normals coordinates.");
    
    return true;
  }
}

