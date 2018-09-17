#
#  DbCryptModule library
#

tdrawing_sources(${TD_DbCryptModule_LIB}
    OdDbCryptImpl.cpp
    OdDbCryptImpl.h
    DbCryptModule.cpp
    DbCryptModule.h
    )

include_directories(
                    ${TKERNEL_ROOT}/Include
                    ${TDRAWING_ROOT}/Include
                   )
                  
if(ODA_SHARED AND MSVC)
tdrawing_sources(${TD_DbCryptModule_LIB}
                  DbCryptModule.rc
                  )
endif(ODA_SHARED AND MSVC)

add_definitions(-DDbCryptModule_EXPORTS)

if(MSVC)
  tdrawing_tx(${TD_DbCryptModule_LIB} ${TD_DB_LIB} ${TD_ROOT_LIB} ${TD_ALLOC_LIB} ${TH_CONDITIONAL_LIBCRYPTO} crypt32.lib)
else(MSVC)
  tdrawing_tx(${TD_DbCryptModule_LIB} ${TD_DB_LIB} ${TD_ROOT_LIB} ${TD_ALLOC_LIB} ${TH_CONDITIONAL_LIBCRYPTO})
endif(MSVC)

tdrawing_project_group(${TD_DbCryptModule_LIB} "Extensions")
