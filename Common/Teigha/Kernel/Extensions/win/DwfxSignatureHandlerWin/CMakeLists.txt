#
#  DWFXSIGNHANDLERWIN library
#
if(MSVC)
tkernel_sources(${TD_DWFXSIGNHANDLERWIN_LIB}
    OdDwfxSignatureHandlerWinImpl.cpp
    OdDwfxSignatureHandlerWinImpl.h
    DwfxSignatureHandlerWinModule.cpp
    DwfxSignatureHandlerWinModule.h
    )

include_directories(
                    ${TKERNEL_ROOT}/Include
                   )
                  
if(ODA_SHARED)
tkernel_sources(${TD_DWFXSIGNHANDLERWIN_LIB}
                  DwfxSignatureHandlerWin.rc
                  )
endif(ODA_SHARED)
add_definitions(-DDWFXSIGNHANDLERWIN_EXPORTS)

tkernel_tx(${TD_DWFXSIGNHANDLERWIN_LIB} ${TD_ROOT_LIB} ${TD_ALLOC_LIB} ${TH_CONDITIONAL_LIBCRYPTO} cryptxml.lib)

tkernel_project_group(${TD_DWFXSIGNHANDLERWIN_LIB} "Extensions")
endif(MSVC)
