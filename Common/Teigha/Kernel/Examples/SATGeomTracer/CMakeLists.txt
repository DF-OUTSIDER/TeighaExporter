#
#  SATGeomTracer executable
#

tkernel_sources(SATGeomTracer
    SATGeomTracer.cpp
    BrepTrace.cpp
    WorldGeomSample.h
    BrepTrace.h
    StdAfx.h
	)

include_directories(
					${TKERNEL_ROOT}/Extensions/ExServices
					../Common)
					
if(ODA_SHARED)
set ( od_satgeomtracer_libs  ${TD_EXLIB} ${TD_BREPRENDERER_LIB} ${TD_BR_LIB}
                             ${TD_ACISBLDR_LIB} ${TD_ROOT_LIB} ${TD_GE_LIB}
)
else(ODA_SHARED)
set ( od_satgeomtracer_libs  ${TD_BREPRENDERER_LIB} ${TD_SPATIALINDEX_LIB} ${TD_BR_LIB}
                             ${TD_EXLIB} ${TD_ACISBLDR_LIB}
                             ${TD_GE_LIB} ${TD_ROOT_LIB} ${TH_CONDITIONAL_LIBCRYPTO}
)
endif(ODA_SHARED)					

if(MSVC)
tkernel_sources(SATGeomTracer
				  SATGeomTracer.rc
				 )
endif(MSVC)

tkernel_executable(SATGeomTracer ${od_satgeomtracer_libs} ${ODA_ADD_SISL}  ${TD_ALLOC_LIB}) 

tkernel_project_group(SATGeomTracer "Examples")