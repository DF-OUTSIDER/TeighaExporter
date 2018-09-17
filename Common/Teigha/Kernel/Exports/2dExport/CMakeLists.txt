#
# 2dExport
#

tkernel_sources(${TD_2D_EXPORT_LIB} 
    Source/2dExportDevice.cpp
    Source/2dSupport.cpp
    Include/2dExportDevice.h
    Include/2dSupport.h

    )
    
include_directories(
	${TKERNEL_ROOT}/Include/
  ${TKERNEL_ROOT}/Exports/2dExport/Include
)

tkernel_library(${TD_2D_EXPORT_LIB})

tkernel_project_group(${TD_2D_EXPORT_LIB} "Exports")

