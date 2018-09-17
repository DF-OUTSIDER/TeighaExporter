#
# TD_ThreejsJSONExport library
#

tkernel_sources(${TD_THREEJS_JSON_EXPORT_LIB}
  Source/ThreejsJSONExport.cpp
  Source/ThreejsJSONModule.cpp
  Source/ExGsGLES2JsonRendition.cpp
  Source/ExGsGLES2JsonSharingProvider.cpp
  Source/GlesJsonServerBaseImpl.cpp
  Source/GlesJsonServerImpl.cpp
  Source/JsonMetafileConverter.cpp
  Source/JsonObjectFormat.cpp
  Include/ThreejsJSONExport.h
  Include/ThreejsJSONExportDef.h
  Include/ThreejsJSONModule.h
  Include/ExGsGLES2JsonRendition.h
  Include/JsonServer.h
  Include/JsonObjectFormat.h
  Include/ExGsGLES2JsonSharingProvider.h
  Include/JsonMetafileConverter.h
  Include/GlesJsonServerBaseImpl.h
  Include/GlesJsonServerImpl.h
  Include/JsonServerBaseImpl.h
  ${TKERNEL_ROOT}/Extensions/ExRender/TrXml/ExGsGLES2IdRegistratorImpl.cpp
)
    
include_directories(
  ${TKERNEL_ROOT}/Include
  ${TKERNEL_ROOT}/Include/Tr/
  ${TKERNEL_ROOT}/Include/Tr/Vec
  ${TKERNEL_ROOT}/Extensions/ExRender/
  ${TKERNEL_ROOT}/Extensions/ExRender/TrXml
  ${TKERNEL_ROOT}/Extensions/ExRender/TrXml/IO	
  Include
)

if(ODA_SHARED AND MSVC)
  tkernel_sources(${TD_THREEJS_JSON_EXPORT_LIB}
                Source/ThreejsJSONExport.rc
  )
endif(ODA_SHARED AND MSVC)
add_definitions(-DTHREEJSJSONEXPORT_EXPORTS)

if(NOT WINCE)
  add_definitions(-DODA_LINT)
endif(NOT WINCE)

tkernel_tx(${TD_THREEJS_JSON_EXPORT_LIB} ${TR_VEC_LIB} ${TR_BASE_LIB} ${TD_DBROOT_LIB} ${TD_GS_LIB} ${TD_GI_LIB} ${TD_GE_LIB} ${TD_ROOT_LIB} ${TD_ALLOC_LIB} )

tkernel_project_group(${TD_THREEJS_JSON_EXPORT_LIB} "Exports")

