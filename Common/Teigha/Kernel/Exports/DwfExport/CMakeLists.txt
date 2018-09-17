#
# TD_Dwf7Export library
#
IF(NOT WINCE AND NOT WINRT)

tkernel_sources(${TK_DWF_EXPORT_LIB}
    Source/Dwf7ExportImpl.cpp
	../DwfExport/Source/DwfDrawObject.cpp
	../DwfExport/Source/Dwf2Dc.cpp
    Source/3dDwfDrawObject.cpp
	../DwfExport/Source/DwfSupport.cpp
    Source/3dDwfExportImpl.cpp
	../DwfExport/Source/DwfExport.cpp
	../DwfExport/Include/DwfExport.h
	Include/DwfExportDef.h
    Include/DwfExportParams.h
    Include/DwfExportImpl.h
    Include/3dDwfDrawObject.h
	../DwfExport/Include/DwfDrawObject.h
    Include/3dDwfExportImpl.h
    Include/Dwf7ExportImpl.h
	../DwfExport/Include/DwfSupport.h
    )

include_directories(
                    ${TKERNEL_ROOT}/Exports/2dExport/Include
					Include
					${TKERNEL_ROOT}/Exports/DwfExport/Include
					)
				  
if(ODA_SHARED AND MSVC)
tkernel_sources(${TK_DWF_EXPORT_LIB}
				  ../DwfExport/Source/DWFexport.rc
				  )
endif(ODA_SHARED AND MSVC)
if(ODA_SHARED)
add_definitions(-DDWFEXPORT_EXPORTS
				-DDWFTK_BUILD_EXPAT
				-DDWF_TOOLKIT_7_6)
else(ODA_SHARED)
add_definitions(-DDWFCORE_STATIC
				-DDWFTK_STATIC
				-DDWFTK_BUILD_EXPAT
				-DDWF_TOOLKIT_7_6
				-DCLIENT_BUILD)
endif(ODA_SHARED)

if(NOT WINCE)
add_definitions(-DODA_LINT)
endif(NOT WINCE)

tkernel_tx(${TK_DWF_EXPORT_LIB} ${TD_2D_EXPORT_LIB} ${TD_ROOT_LIB} ${TD_GS_LIB} ${TD_DBROOT_LIB} ${TD_ROOT_LIB} ${TD_GI_LIB} ${TD_GE_LIB} ${TD_ROOT_LIB} 
                           ${TH_DWF7_TOOLKIT_LIB} ${TH_DWF7_W3DTK_LIB} ${TH_DWF7_WHIPTK_LIB} ${TH_DWF7_CORE_LIB} ${TD_ALLOC_LIB} ${TH_ZLIB_LIB} ${ODA_WCHAR_LIB})

tkernel_project_group(${TK_DWF_EXPORT_LIB} "Exports")

else(NOT WINCE AND NOT WINRT)
set(TK_DWF_EXPORT_LIB)
endif(NOT WINCE AND NOT WINRT)
