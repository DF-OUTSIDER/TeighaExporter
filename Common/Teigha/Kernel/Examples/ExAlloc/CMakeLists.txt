#
#  Ex_Alloc library
#

tkernel_sources(${TD_EX_ALLOC_LIB}
    ExAlloc.cpp
 #   ExAllocOp.cpp
    QhMutex.h
    QuickHeap.h
	${TKERNEL_ROOT}/Include/OdAlloc.h
	${TKERNEL_ROOT}/Include/OdAllocExport.h

	)

include_directories(
					)

if(MSVC OR BORLAND)
if(CMAKE_CL_64)
tkernel_sources(${TD_EX_ALLOC_LIB}
				  ExAlloc64.def
				  )
else(CMAKE_CL_64)
tkernel_sources(${TD_EX_ALLOC_LIB}
				  ExAlloc.def
				  )
endif(CMAKE_CL_64)	
endif(MSVC OR BORLAND)

if(ODA_SHARED AND MSVC)	
tkernel_sources(${TD_EX_ALLOC_LIB}
				  ExAlloc.rc
				  )
endif(ODA_SHARED AND MSVC)	

add_definitions(-DALLOC_DLL_EXPORTS)

if(NOT WINCE)
add_definitions(-DODA_LINT)
endif(NOT WINCE)

if(MSVC)
add_definitions( "/wd4996" )
endif(MSVC)

tkernel_tx(${TD_EX_ALLOC_LIB}) 

tkernel_project_group(${TD_EX_ALLOC_LIB} "Examples")
