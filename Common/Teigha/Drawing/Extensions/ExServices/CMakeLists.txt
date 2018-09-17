if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/ExGiRasterImage.cpp)
#
#  RxRasterServices library
#

tdrawing_sources(${TD_RASTER_LIB}
    ExGiRasterImage.cpp
	ExGiRasterImage.h
	)

include_directories()



if (SNOWBOUND)
if (EXISTS $ENV{SNBD_DIR})
include_directories($ENV{SNBD_DIR})
link_directories($ENV{SNBD_DIR})
add_definitions(-DRASTER_SNOW_BOUND)

else (EXISTS $ENV{SNBD_DIR})
message("Error: Environment variable SNBD_DIR (SnowBound SDK) is not set")
endif (EXISTS $ENV{SNBD_DIR})

endif (SNOWBOUND)


if(ODA_SHARED AND MSVC)
tdrawing_sources(${TD_RASTER_LIB}
				RasterServices.rc
				)
endif(ODA_SHARED AND MSVC)

add_definitions(-DEXRASTERDLL_EXPORTS)


if(NOT WINCE)
add_definitions(-DODA_LINT)
endif(NOT WINCE)


tdrawing_tx(${TD_RASTER_LIB} ${TD_ROOT_LIB} ${TH_THIRDPARTYRASTER_LIB} ${TH_ZLIB_LIB} ${TD_ALLOC_LIB})

if(LINUX_X86 OR LINUX_X64)
if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 4.2)
  set_property(TARGET ${TD_RASTER_LIB} APPEND PROPERTY LINK_FLAGS " -Wl,-Bsymbolic-functions")
endif()
endif()

tdrawing_project_group(${TD_RASTER_LIB} "Extensions")
endif (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/ExGiRasterImage.cpp)
