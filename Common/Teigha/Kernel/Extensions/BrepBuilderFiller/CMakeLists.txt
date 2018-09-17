#
#  BREPBUILDERFILLER library
#
tkernel_sources(${TD_BREPBUILDERFILLER_LIB}
	Source/BrepBuilderFillerModule.cpp
	Include/BrepBuilderFillerModule.h
	Include/BrepBuilderFillerDef.h
	Include/BrepBuilderInitialData.h
	Include/IMaterialAndColorHelper.h
	Source/BaseMaterialAndColorHelper.cpp
	Include/BaseMaterialAndColorHelper.h
	Include/BrepBuilderInitialParams.h
)

include_directories(
	Include
	${TKERNEL_ROOT}/Include
)

if(ODA_SHARED AND MSVC)
tkernel_sources(${TD_BREPBUILDERFILLER_LIB}
	Source/BrepBuilderFiller.rc
)
endif(ODA_SHARED AND MSVC)
add_definitions(-DBREPBUILDERFILLER_EXPORTS)

if(NOT WINCE)
add_definitions(-DODA_LINT)
endif(NOT WINCE)

tkernel_tx(${TD_BREPBUILDERFILLER_LIB}
	${TD_GI_LIB}
	${TD_GE_LIB}
	${TD_BR_LIB}
	${TD_BREPBUILDER_LIB}
	${TD_ROOT_LIB}
	${TD_DBROOT_LIB}
	${TD_ALLOC_LIB}
)

tkernel_project_group(${TD_BREPBUILDERFILLER_LIB} "Extensions")
