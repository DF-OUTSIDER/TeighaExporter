#
# TG_ExDgnDumper library
#

tg_sources ( ${TG_EXDGNDUMPER_LIB}
  ExDgnDumperModule.cpp
  ExDgnElementDumperPE.cpp
  ExDgnDumperModule.h
  ExDgnDumper.h
  ExDgnElementDumperPE.h
	 )
	 
if(MSVC AND ODA_SHARED)
tg_sources ( ${TG_EXDGNDUMPER_LIB}
                   ExDgnDumper.rc )
endif(MSVC AND ODA_SHARED)

include_directories(${TG_CORE_INCLUDE}
					${CMAKE_CURRENT_SOURCE_DIR}
					../Elements
					../Common
					../FileIO)
					
add_definitions(-DODA_TG_EXPORTS)	
			 
tg_tx (${TG_EXDGNDUMPER_LIB} ${TG_DB_LIB} ${TD_GS_LIB} ${TD_GE_LIB} ${TD_GI_LIB} ${TD_DBROOT_LIB} ${TD_ROOT_LIB} ${TD_ALLOC_LIB} )

tg_project_group(${TG_EXDGNDUMPER_LIB} "Extensions")