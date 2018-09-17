#
#  EXTIMEPROVIDERWIN library
#
if(MSVC OR BORLAND)
tkernel_sources(${TD_EXTIMEPROVIDERWIN_LIB}
    ExTimeProviderWin.cpp
    ExTimeProviderWin.h
    )

include_directories(
                    ${TKERNEL_ROOT}/Include
                   )
                  
if(ODA_SHARED)
tkernel_sources(${TD_EXTIMEPROVIDERWIN_LIB}
                  ExTimeProviderWin.rc
                  )
endif(ODA_SHARED)
add_definitions(-DEXTIMEPROVIDERWIN_EXPORTS)

tkernel_tx(${TD_EXTIMEPROVIDERWIN_LIB} ${TD_ROOT_LIB} ${TD_ALLOC_LIB} ${TH_CONDITIONAL_LIBCRYPTO} ws2_32.lib)

tkernel_project_group(${TD_EXTIMEPROVIDERWIN_LIB} "Extensions")
endif(MSVC OR BORLAND)
