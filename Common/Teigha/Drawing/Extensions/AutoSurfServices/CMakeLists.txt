#
#  AutoSurfServices library
#

tdrawing_sources(${TD_AUTOSURF_LIB}
    StdAfx_AutoSurf.cpp
    AsSurfBodyModule.cpp
    AsSubSurfaces.cpp
    AsSurfBody.cpp
    AsSubSurfaces.h
    AsSurfBody.h
    StdAfx.h
    AsSurfBodyExport.h
    AsSurfBodyModule.h
	)

include_directories()
				  
if(ODA_SHARED AND MSVC)
tdrawing_sources(${TD_AUTOSURF_LIB}
				  AsSurfServices.rc
				  )
endif(ODA_SHARED AND MSVC)
add_definitions(-DASSURFBODY_DLL_EXPORTS)

if(NOT WINCE)
add_definitions(-DODA_LINT)
endif(NOT WINCE)

tdrawing_tx(${TD_AUTOSURF_LIB} ${TD_DB_LIB} ${TD_DBROOT_LIB} ${TD_BR_LIB} ${TD_GE_LIB} ${TD_ROOT_LIB} ${TD_ALLOC_LIB})

tdrawing_project_group(${TD_AUTOSURF_LIB} "Extensions")