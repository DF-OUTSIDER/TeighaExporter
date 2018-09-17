#
#  DwfxSignatureSample executable
#

tkernel_sources(DwfxSignatureSample
	DwfxSignatureSample.cpp
                   ${TKERNEL_ROOT}/Extensions/ExServices/ExSystemServices.h
	)

include_directories(
					${TKERNEL_ROOT}/Extensions/ExServices
					../Common)
					
if(ODA_SHARED)
set ( DwfxSignatureSample_libs  ${TD_EXLIB} ${TD_ROOT_LIB})
else(ODA_SHARED)
  if(TD_DWFXSIGNHANDLERWIN_VERSION)
    set ( DwfxSignatureSample_libs  ${TD_DWFXSIGNHANDLERWIN_LIB} ${TD_EXLIB} ${TD_ROOT_LIB} ${TH_CONDITIONAL_LIBCRYPTO} cryptxml.lib)
  else(TD_DWFXSIGNHANDLERWIN_VERSION)
    set ( DwfxSignatureSample_libs  ${TD_DWFXSIGNHANDLER_LIB} ${TD_EXLIB} ${TD_ROOT_LIB} ${TH_CONDITIONAL_LIBCRYPTO})
  endif(TD_DWFXSIGNHANDLERWIN_VERSION)
endif(ODA_SHARED)					

if(MSVC)
tkernel_sources(DwfxSignatureSample
				  DwfxSignatureSample.rc
				 )
endif(MSVC)

tkernel_executable(DwfxSignatureSample ${DwfxSignatureSample_libs} ${TD_ALLOC_LIB})

tkernel_project_group(DwfxSignatureSample "Examples")
