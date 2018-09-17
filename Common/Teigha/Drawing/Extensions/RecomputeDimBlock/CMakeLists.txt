#
#  RecomputeDimBlock library
#

tdrawing_sources(${TD_RDIMBLK_LIB}
    OdRecomputeDimBlockModule.cpp
    OdRadialDimRecomputor.cpp
    OdOrdinateDimRecomputor.cpp
    OdArcDimRecomputor.cpp
    Od2LineAngularDimRecomputor.cpp
    OdRecomputorEngine.cpp
    OdDiametricDimRecomputor.cpp
    OdAlignedRecomputorEngine.cpp
    OdAlignedDimRecomputor.cpp
    OdAngularRecomputorEngine.cpp
    OdRadialRecomputorEngine.cpp
    Od3PointAngularDimRecomputor.cpp
    OdDimRecomputor.cpp
    OdRadialDimLargeRecomputor.cpp
    OdRotatedDimRecomputor.cpp
    OdBreakDimEngine.cpp
    OdMLeaderRecomputor.cpp
    OdOrdinateDimRecomputor.h
    OdAngularRecomputorEngine.h
    OdDiametricDimRecomputor.h
    OdAlignedDimRecomputor.h
    Od2LineAngularDimRecomputor.h
    OdRecomputeDimBlockModule.h
    OdRadialRecomputorEngine.h
    OdRadialDimRecomputor.h
    OdRadialDimLargeRecomputor.h
    OdRecomputorEngine.h
    OdArcDimRecomputor.h
    Od3PointAngularDimRecomputor.h
    OdAlignedRecomputorEngine.h
    OdDimRecomputor.h
    OdRotatedDimRecomputor.h
    OdBreakDimEngine.h
    OdMLeaderRecomputor.h
	)

include_directories()

add_definitions(-DRECOMPUTEDIMBLOCK_EXPORTS)

if(NOT WINCE)
add_definitions(-DODA_LINT)
endif(NOT WINCE)

if(ODA_SHARED AND MSVC)	
tdrawing_sources(${TD_RDIMBLK_LIB}
					RecomputeDimBlock.rc
				 )
endif(ODA_SHARED AND MSVC)	
				  
tdrawing_tx(${TD_RDIMBLK_LIB} ${TD_DB_LIB} ${TD_DBROOT_LIB} ${TD_GE_LIB} ${TD_ROOT_LIB} ${TD_ALLOC_LIB})

tdrawing_project_group(${TD_RDIMBLK_LIB} "Extensions")