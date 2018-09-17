#
# TD_SvgExport library
#


tkernel_sources(${TK_SVG_EXPORT_LIB}
	Source/StdAfx_SvgExport.cpp
    Source/SvgExport.cpp
    Source/StdAfx.h
    Include/SvgExport.h
    Source/SvgShxEmbedding.h
    Source/SvgWrappers.h
)
			  
include_directories(
                    ${TKERNEL_ROOT}/Exports/2dExport/Include
					          Include
					          )
					
if(ODA_SHARED AND MSVC)
tkernel_sources(${TK_SVG_EXPORT_LIB}
				  Source/SvgExport.rc
				  )
endif(ODA_SHARED AND MSVC)
add_definitions(-DSVGEXPORT_DLL_EXPORTS)

if(NOT WINCE)
add_definitions(-DODA_LINT)
endif(NOT WINCE)

tkernel_tx(${TK_SVG_EXPORT_LIB} ${TD_2D_EXPORT_LIB} ${TD_DBROOT_LIB} ${TD_GS_LIB} ${TD_GI_LIB} ${TD_GE_LIB} ${TD_ROOT_LIB} ${TD_ALLOC_LIB} )

tkernel_project_group(${TK_SVG_EXPORT_LIB} "Exports")
