#
#  SATConverter executable
#

tkernel_sources(SATConverter
	SATConverter.cpp
	)

include_directories(
					${TKERNEL_ROOT}/Extensions/ExServices
					../Common)
					
if(ODA_SHARED)
set ( od_satconverter_libs ${TD_EXLIB} ${TD_ACISBLDR_LIB} ${TD_GE_LIB} ${TD_ROOT_LIB}
)
else(ODA_SHARED)
set ( od_satconverter_libs ${TD_EXLIB} ${TD_ACISBLDR_LIB} ${TD_GE_LIB} ${TD_ROOT_LIB}
)
endif(ODA_SHARED)

if(MSVC)
tkernel_sources(SATConverter
				  SATConverter.rc
				 )
endif(MSVC)

tkernel_executable(SATConverter ${od_satconverter_libs} ${ODA_ADD_SISL} ${TD_ALLOC_LIB} ${TH_CONDITIONAL_LIBCRYPTO}) 

tkernel_project_group(SATConverter "Examples")