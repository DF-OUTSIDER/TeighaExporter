#
#  OdGeoData library
#

tdrawing_sources(${TD_GEODATAEX_LIB}
    OdGeoDataModule.cpp
    OdGeoDataModule.h
    OdGeoDataPEImpl.cpp
    OdGeoDataPEImpl.h
    OdDbGeoCoordinateSystemImpl.cpp
    OdDbGeoCoordinateSystemImpl.h
    OdDbGeoCoordinateSystemPEImpl.cpp
    OdDbGeoCoordinateSystemPEImpl.h
)

include_directories(
)

if(ODA_SHARED AND MSVC)	
tdrawing_sources(${TD_GEODATAEX_LIB}
    OdGeoData.rc
)
endif(ODA_SHARED AND MSVC)

add_definitions(-DODGEODATAEX_EXPORTS)

if(NOT WINCE)
add_definitions(-DODA_LINT)
endif(NOT WINCE)

tdrawing_tx(${TD_GEODATAEX_LIB} ${TD_DB_LIB} ${TD_GE_LIB} ${TD_ROOT_LIB} ${TH_CSMAP_LIB} ${TH_TINYXML_LIB} ${TD_ALLOC_LIB})

tdrawing_project_group(${TD_GEODATAEX_LIB} "Extensions")