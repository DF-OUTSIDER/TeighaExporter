#
# ExRasterSaving library
#

tg_sources ( ${TG_EXRASTERSAVING_LIB}
           RasterSavingModule.cpp
           RasterSavingCmd.cpp
           RasterSaving.h
           RasterSavingCmd.h
           RasterSavingCmdDef.h
           RasterSavingModule.h
	 )
	 
if(MSVC AND ODA_SHARED)
tg_sources ( ${TG_EXRASTERSAVING_LIB}
           StdAfx.cpp
           StdAfx.h
           RasterSaving.rc
         )
endif(MSVC AND ODA_SHARED)

include_directories(${TG_CORE_INCLUDE}
					../../source/Common
					../../source/FileIO
					../../source/Design
					../../source/Elements
					${TG_ROOT}/Extensions/ExServices
					${TKERNEL_ROOT}/Extensions/ExServices)

					
add_definitions(-DTG_EX_RASTERSAVING)

if(NOT WINCE)
add_definitions(-DODA_LINT)
endif(NOT WINCE)

			 
tg_tx ( ${TG_EXRASTERSAVING_LIB} ${TD_DBROOT_LIB} ${TG_DB_LIB} ${TD_GE_LIB} ${TD_GI_LIB} ${TD_ROOT_LIB} ${TD_ALLOC_LIB} )

tg_project_group(${TG_EXRASTERSAVING_LIB} "Extensions")