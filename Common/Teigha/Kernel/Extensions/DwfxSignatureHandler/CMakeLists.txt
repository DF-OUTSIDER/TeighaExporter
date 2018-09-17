#
#  DWFXSIGNHANDLER library
#
tkernel_sources(${TD_DWFXSIGNHANDLER_LIB}
    OdDwfxSignatureHandlerImpl.cpp
    OdDwfxSignatureHandlerImpl.h
    DwfxSignatureHandlerModule.cpp
    DwfxSignatureHandlerModule.h
    )

include_directories(
                    ${TKERNEL_ROOT}/Include
                   )
                  
if(ODA_SHARED)
tkernel_sources(${TD_DWFXSIGNHANDLER_LIB}
                  DwfxSignatureHandler.rc
                  )
endif(ODA_SHARED)
add_definitions(-DDWFXSIGNHANDLER_EXPORTS)

tkernel_tx(${TD_DWFXSIGNHANDLER_LIB} ${TD_ROOT_LIB} ${TD_ALLOC_LIB})

tkernel_project_group(${TD_DWFXSIGNHANDLER_LIB} "Extensions")
