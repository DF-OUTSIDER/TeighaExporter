#
#  EXTIMEPROVIDER library
#

tkernel_sources(${TD_EXTIMEPROVIDER_LIB}
    ExTimeProvider.cpp
    ExTimeProvider.h
    )

include_directories(
                    ${TKERNEL_ROOT}/Include
                   )
                  
if(ODA_SHARED)
tkernel_sources(${TD_EXTIMEPROVIDER_LIB}
                  ExTimeProvider.rc
                  )
endif(ODA_SHARED)
add_definitions(-DEXTIMEPROVIDER_EXPORTS)

tkernel_tx(${TD_EXTIMEPROVIDER_LIB} ${TD_ROOT_LIB} ${TD_ALLOC_LIB} ${TH_CONDITIONAL_LIBCRYPTO})

tkernel_project_group(${TD_EXTIMEPROVIDER_LIB} "Extensions")
