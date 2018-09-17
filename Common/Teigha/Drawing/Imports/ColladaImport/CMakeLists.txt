#
# TD_ColladaImport library
#

tdrawing_sources(${TD_COLLADA_IMPORT_LIB}
    Source/ColladaEffectImporter.cpp
	Source/ColladaExtraDataHandler.cpp
	Source/ColladaFWLErrorHandler.cpp
	Source/ColladaGeometryImporter.cpp
	Source/ColladaImageImporter.cpp
	Source/ColladaImportImpl.cpp
	Source/ColladaImportImpl.h
	Source/ColladaLightImporter.cpp
	Source/ColladaMaterialCreator.cpp
	Source/ColladaMaterialImporter.cpp
	Source/ColladaNodeImporter.cpp
	Source/ColladaLibraryNodesImporter.cpp
	Source/ColladaDocumentImporter.cpp
	Source/ColladaErrorHandler.cpp
	Source/ColladaImporterBase.cpp
	Source/ColladaSceneGraphCreator.cpp
	Source/ColladaVisualSceneImporter.cpp
	Source/ColladaControllerImporter.cpp
	Include/ColladaEffectImporter.h
	Include/ColladaExtraDataHandler.h
	Include/ColladaFWLErrorHandler.h
	Include/ColladaGeometryImporter.h
	Include/ColladaImageImporter.h
	Include/ColladaImportDef.h
	Include/ColladaImport.h
	Include/ColladaLightImporter.h
	Include/ColladaMaterialCreator.h
	Include/ColladaMaterialImporter.h
	Include/ColladaNodeImporter.h
	Include/ColladaLibraryNodesImporter.h
	Include/ColladaDocumentImporter.h
	Include/ColladaErrorHandler.h
	Include/ColladaImporterBase.h
	Include/ColladaSceneGraphCreator.h
	Include/ColladaVisualSceneImporter.h
	Include//ColladaControllerImporter.h
    )

include_directories(
					Include
					)

if(ODA_SHARED AND MSVC)
tdrawing_sources(${TD_COLLADA_IMPORT_LIB}
				  Source/ColladaImport.rc
				  )
endif(ODA_SHARED AND MSVC)

if(ODA_SHARED)
add_definitions(-DCOLLADAIMPORT_EXPORTS
                -D_LIB
                -DLIBXML_STATIC_FOR_DLL )
else(ODA_SHARED)
  add_definitions(-D_LIB
                -DPCRE_STATIC
                -DLIBXML_STATIC
                -DCOLLADABU_STATIC
                -DCOLLADASW_STATIC
                -DPCRE_STATIC
                -DMATHMLSOLVER_STATIC
                -DLIBBUFFER_STATIC
                -DCOLLADAFRMW_STATIC
                -DGENERATEDSAXPARSER_STATIC
                -DCOLLADASAXFWL_STATIC
  )
endif(ODA_SHARED)

tdrawing_tx(${TD_COLLADA_IMPORT_LIB} ${TD_DB_LIB} ${TD_GS_LIB} ${TD_GI_LIB} ${TD_GE_LIB} ${TD_ROOT_LIB} ${TD_DBROOT_LIB} 
      ${TH_COLLADAStreamWriter} ${TH_COLLADASaxFrameworkLoader} ${TH_COLLADAFramework} ${TH_COLLADABaseUtils} ${TH_GeneratedSaxParser} 
      ${TH_LIBBUFFER} ${TH_PCRE_LIB} ${TH_UTF_LIB} ${TH_MathMLSolver} ${TH_libXML} ${TD_ALLOC_LIB} )  
  
tdrawing_project_group(${TD_COLLADA_IMPORT_LIB} "Imports")
