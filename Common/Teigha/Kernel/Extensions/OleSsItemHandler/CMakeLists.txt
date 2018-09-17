#
#  OdOleSsItemHandler library
#
if(TH_OLESS) # AND NOT WINCE)

tkernel_sources(${TD_OLESSITEMHDR_LIB}
           OleSsItemHandlerModule.cpp
           OleSsItemHandler_dll.cpp
           OleSsItemHandlerImpl.cpp
           OleSsItemHandlerModule.h
           OleSsItemHandlerImpl.h
           CompDocData.h
)

if(ODA_SHARED AND MSVC)
tkernel_sources(${TD_OLESSITEMHDR_LIB}
           OleSsItemHandler.rc
)

endif(ODA_SHARED AND MSVC)

include_directories()

add_definitions(-DOLESSITEMHANDLER_EXPORTS)

tkernel_tx(${TD_OLESSITEMHDR_LIB}
      ${TD_DBROOT_LIB}
      ${TD_GS_LIB}
      ${TD_GI_LIB}
      ${TD_GE_LIB}
      ${TD_ROOT_LIB}
      ${TD_ALLOC_LIB}
      ${TH_OLESS_LIB} 
)

tkernel_project_group(${TD_OLESSITEMHDR_LIB} "Extensions")

else(TH_OLESS) # AND NOT WINCE)
set(TD_OLESSITEMHDR_LIB)
endif(TH_OLESS) # AND NOT WINCE)
