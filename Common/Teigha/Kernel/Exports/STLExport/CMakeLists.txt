#
# TD_STLExport library
#

tkernel_sources(${TD_STL_EXPORT_LIB}
	Source/STLExport.cpp
	Source/STLModule.cpp
	Include/STLModule.h
	Include/STLExport.h
	Include/STLExportDef.h
    )

include_directories(
					Include
					)

if(ODA_SHARED AND MSVC)
tkernel_sources(${TD_STL_EXPORT_LIB}
				  Source/STLExport.rc
				  )
endif(ODA_SHARED AND MSVC)
add_definitions(-DDYNBLOCKS_DLL_EXPORTS)

if(NOT WINCE)
add_definitions(-DODA_LINT)
endif(NOT WINCE)

tkernel_tx(${TD_STL_EXPORT_LIB} ${TD_DBROOT_LIB} ${TD_GS_LIB} ${TD_GI_LIB} ${TD_GE_LIB} ${TD_ROOT_LIB} ${TD_ALLOC_LIB} )

tkernel_project_group(${TD_STL_EXPORT_LIB} "Exports")
