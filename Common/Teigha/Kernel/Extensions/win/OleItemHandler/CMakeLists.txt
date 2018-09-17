#
#  OdOleItemHandler library
#
tkernel_sources(${TD_OLEITEMHDR_LIB}
    OleItemHandlerModule.cpp
    OleItemHandler_dll.cpp
    OleItemHandlerImpl.cpp
    ExEmbedRaster.cpp
    COMErrorCtx.cpp
    OleItemHandlerModule.h
    OleItemHandlerImpl.h
    ExEmbedRaster.h
    COMErrorCtx.h
    ${TKERNEL_ROOT}/Extensions/OleSsItemHandler/CompDocData.h
	)
if(ODA_SHARED AND MSVC)
tkernel_sources(${TD_OLEITEMHDR_LIB}
	OleItemHandler.rc
	)
endif(ODA_SHARED AND MSVC)

include_directories(
                    ${TKERNEL_ROOT}/Extensions/OleSsItemHandler
)

add_definitions(-DOLEITEMHANDLER_EXPORTS)

tkernel_tx(${TD_OLEITEMHDR_LIB} ${TD_DBROOT_LIB} ${TD_GE_LIB} ${TD_ROOT_LIB} ${TD_ALLOC_LIB})

tkernel_project_group(${TD_OLEITEMHDR_LIB} "Extensions")

