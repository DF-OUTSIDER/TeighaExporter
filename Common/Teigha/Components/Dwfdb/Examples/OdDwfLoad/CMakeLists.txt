
set(dwf_load_sources 
    OdDwfLoad.cpp
    ${TKERNEL_ROOT}/Extensions/ExServices/RxSystemServicesImpl.cpp
    ${TKERNEL_ROOT}/Extensions/ExServices/OdFileBuf.cpp
)

tcomponents_sources(OdDwfLoad
                    ${dwf_load_sources}
)

include_directories(
                    ${TCOMPONENTS_ROOT}/Dwfdb/Include
                    ${TKERNEL_ROOT}/Examples/Common
                    ${TKERNEL_ROOT}/Include
                    ${TKERNEL_ROOT}/Extensions/ExServices
                    ${TKERNEL_ROOT}
)

if(ODA_SHARED AND MSVC)
tcomponents_sources(OdDwfLoad OdDwfLoad.rc)
endif(ODA_SHARED AND MSVC)

if(MSVC)
  set( ODA_DWF7_DEPS rpcrt4.lib )
endif(MSVC)

if(ODA_SHARED)
tcomponents_executable(OdDwfLoad 
                       ${TDWF_DB_LIB} ${TH_DWF7_TOOLKIT_LIB} ${TH_DWF7_CORE_LIB} ${ODA_DWF7_DEPS} ${TH_DWF7_WHIPTK_LIB} ${TH_DWF7_W3DTK_LIB} ${TH_THIRDPARTYRASTER_LIB}
                       ${TD_DBROOT_LIB} ${TD_GS_LIB} ${TD_SPATIALINDEX_LIB} ${TD_GI_LIB} ${TD_GE_LIB} ${TD_ROOT_LIB} ${TD_ALLOC_LIB}
                       ${ODA_ADD_SISL} ${ODA_WCHAR_LIB} ${TH_ZLIB_LIB} 	       
                       ${TH_CONDITIONAL_LIBCRYPTO} ${CRYPTO_LINK_LIBS}
)
else(ODA_SHARED)
tcomponents_executable(OdDwfLoad
                       ${TDWF_DB_LIB}  ${ODA_DWF7_DEPS}
                       ${TD_DBROOT_LIB} ${TD_GS_LIB} ${TD_GI_LIB} ${TD_SPATIALINDEX_LIB} ${TD_GE_LIB} ${TD_ROOT_LIB} ${TD_ALLOC_LIB}
                       ${ODA_ADD_SISL} ${ODA_WCHAR_LIB}
                       ${TH_DWF7_TOOLKIT_LIB} ${TH_DWF7_WHIPTK_LIB} ${TH_DWF7_W3DTK_LIB}  ${TH_DWF7_CORE_LIB} ${TH_THIRDPARTYRASTER_LIB}
                       ${TH_CONDITIONAL_LIBCRYPTO} ${CRYPTO_LINK_LIBS} ${TH_ZLIB_LIB} ${TH_FT_LIB}
)
endif(ODA_SHARED)

tcomponents_project_group(OdDwfLoad "Dwfdb/Examples")
