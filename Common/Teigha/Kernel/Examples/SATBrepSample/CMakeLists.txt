#
#  SATBrepSample executable
#

tkernel_sources(SATBrepSample
	SATBrepSample.cpp
	)

include_directories(
					${TKERNEL_ROOT}/Extensions/ExServices
					../Common)
					
if(ODA_SHARED)
set ( od_satbreapsample_libs  ${TD_EXLIB} ${TD_BR_LIB}
                              ${TD_ACISBLDR_LIB} ${TD_ROOT_LIB} ${TD_GE_LIB}
)
else(ODA_SHARED)
set ( od_satbreapsample_libs  ${TD_BR_LIB}
                              ${TD_EXLIB} ${TD_ACISBLDR_LIB}
                              ${TD_GE_LIB} ${TD_ROOT_LIB}
)
endif(ODA_SHARED)					

if(MSVC)
tkernel_sources(SATBrepSample
				  SATBrepSample.rc
				 )
endif(MSVC)

tkernel_executable(SATBrepSample ${od_satbreapsample_libs} ${ODA_ADD_SISL} ${TD_ALLOC_LIB} ${TH_CONDITIONAL_LIBCRYPTO}) 

tkernel_project_group(SATBrepSample "Examples")
