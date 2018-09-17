#
# TD_ColladaExport library
#

tkernel_sources(${TD_COLLADA_EXPORT_LIB}
	Source/AssetExporter.cpp
	Source/ColladaExport.cpp
	Source/ColladaLightTraits.cpp
	Source/ColladaModule.cpp
	Source/ColladaExportView.cpp
	Source/EffectExporter.cpp
	Source/GeometryExporter.cpp
	Source/ImageExporter.cpp
	Source/LightExporter.cpp
	Source/MaterialExporter.cpp
	Source/VisualSceneExporter.cpp
	Include/AssetExporter.h
	Include/ColladaExport.h
	Include/ColladaExportDef.h
	Include/ColladaExportView.h
	Include/ColladaLightTraits.h
	Include/ColladaModule.h
	Include/EffectExporter.h
	Include/GeometryExporter.h
	Include/ImageExporter.h
	Include/LightExporter.h
	Include/MaterialExporter.h
	Include/VisualSceneExporter.h
    )

include_directories(
					Include
					)

if(ODA_SHARED AND MSVC)
tkernel_sources(${TD_COLLADA_EXPORT_LIB}
				  Source/ColladaExport.rc
				  )
endif(ODA_SHARED AND MSVC)

add_definitions(-DCOLLADAEXPORT_EXPORTS)

if(ODA_SHARED)
#  add_definitions(-DCOLLADASW_EXPORT_API
#                  -DCOLLADABU_EXPORT_API
#                  -DLIBBUFFER_EXPORT_API
#                  -DPCRE_EXPORT_API
#                  )
else(ODA_SHARED)
  add_definitions(-DCOLLADABU_STATIC
                  -DCOLLADASW_STATIC
                  -DPCRE_STATIC
                  -DMATHMLSOLVER_STATIC
                  -DLIBBUFFER_STATIC)
endif(ODA_SHARED)

tkernel_tx(${TD_COLLADA_EXPORT_LIB} ${TD_GS_LIB} ${TD_GI_LIB} ${TD_GE_LIB} ${TD_ROOT_LIB} ${TD_DBROOT_LIB} 
      ${TH_COLLADAStreamWriter} ${TH_COLLADABaseUtils} ${TH_LIBBUFFER} ${TH_PCRE_LIB} ${TH_UTF_LIB}  ${TD_ALLOC_LIB})
  
tkernel_project_group(${TD_COLLADA_EXPORT_LIB} "Exports")
