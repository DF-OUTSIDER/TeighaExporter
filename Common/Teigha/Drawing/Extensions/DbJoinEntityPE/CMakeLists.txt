#
# TD_DbJoinEntityPE library
#

tdrawing_sources(${TD_DBJOINENTITYPE_LIB}
	Source/DbJoinEntityPEModule.cpp
	Source/DbJoinEntityPECurves.cpp
	Include/DbJoinEntityPEModule.h
	Include/DbJoinEntityPECurves.h
	Include/DbJoinEntityPEDef.h
    )

include_directories(
					Include
					)

if(ODA_SHARED AND MSVC)
tdrawing_sources(${TD_DBJOINENTITYPE_LIB}
				  Source/DbJoinEntityPE.rc
				  )
endif(ODA_SHARED AND MSVC)
add_definitions(-DDYNBLOCKS_DLL_EXPORTS)

if(NOT WINCE)
add_definitions(-DODA_LINT)
endif(NOT WINCE)

tdrawing_tx(${TD_DBJOINENTITYPE_LIB} ${TD_DB_LIB} ${TD_DBROOT_LIB} ${TD_GE_LIB} ${TD_ROOT_LIB} ${TD_ALLOC_LIB} )

tdrawing_project_group(${TD_DBJOINENTITYPE_LIB} "Extensions")
