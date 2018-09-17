#
#  AeciIb library
#

tdrawing_sources(${TD_AECIIB_LIB}
    AeciIbModule.cpp
    AeciIbExport.h
    DbEmbeddedRasterImage.cpp
    DbEmbeddedRasterImageDef.cpp
    DbEmbeddedImageData.cpp
    DbEmbeddedRasterImage.h
    DbEmbeddedRasterImageDef.h
    DbEmbeddedImageData.h
	)

include_directories()
				  
if(ODA_SHARED AND MSVC)	
tdrawing_sources(${TD_AECIIB_LIB}
				  AeciIb.rc
				  )
endif(ODA_SHARED AND MSVC)	
add_definitions(-DAECIIB_DLL_EXPORTS)

if(NOT WINCE)
add_definitions(-DODA_LINT)
endif(NOT WINCE)

tdrawing_tx(${TD_AECIIB_LIB} ${TD_DB_LIB} ${TD_GE_LIB} ${TD_ROOT_LIB} ${TD_ALLOC_LIB})

tdrawing_project_group(${TD_AECIIB_LIB} "Extensions")