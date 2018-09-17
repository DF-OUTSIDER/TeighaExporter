#
#  RasterExport library
#


TKERNEL_TX_LIB (TD_RASTER_EXPORT_LIB      TD_RasterExport)

tkernel_sources(${TD_RASTER_EXPORT_LIB}
           Source/RasterExportModule.cpp
           Source/RasterExportCmd.cpp
           Include/RasterExport.h
           Source/RasterExportCmd.h
           Source/RasterExportCmdDef.h
           Source/RasterExportModule.h
           Source/GetRasterFilePath.h
          )

if(ODA_SHARED AND MSVC)
tkernel_sources(${TD_RASTER_EXPORT_LIB}
           Source/StdAfx.cpp
           Source/StdAfx.h
           Source/RasterExport.rc
          )
endif(ODA_SHARED AND MSVC)
add_definitions(-DRASTEREXPORT_EXPORTS)

if(NOT WINCE)
add_definitions(-DODA_LINT)
endif(NOT WINCE)

tkernel_tx(${TD_RASTER_EXPORT_LIB} ${TD_DBROOT_LIB} ${TD_GI_LIB} ${TD_GE_LIB} ${TD_ROOT_LIB} ${TD_ALLOC_LIB} )

tkernel_project_group(${TD_RASTER_EXPORT_LIB} "Exports")

