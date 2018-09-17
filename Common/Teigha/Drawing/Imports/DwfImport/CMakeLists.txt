#
# TD_DwfImport library
#
if(NOT WINCE AND NOT WINRT)

tdrawing_sources(${TD_DWF_IMPORT_LIB}
    Source/W2DImport.cpp
    Source/EmbeddedImageDef.cpp
    Source/DwfEventReactor.cpp
    Source/W2dSumminfo.cpp
    Source/DwfImportImpl.cpp
    Source/ImportCallbacks.cpp
    Source/3dDWFimport.cpp
    Source/FontManager.h
    Source/ImageManager.h
    Source/ExtentManager.h
    Source/DwfCallbacks.h
    Source/LineManager.h
    Source/DwfEventReactor.h
    Include/DwfImport.h
    Source/EmbeddedImageDef.h
    Source/BlockManager.h
    Source/DwfImportImpl.h 
    )
			  
include_directories(
					Include
					${TKERNEL_ROOT}/Imports/DwfImport/Include
					${TKERNEL_ROOT}/Imports/DwfImport/Source
					)
				  
if(ODA_SHARED AND MSVC)
tdrawing_sources(${TD_DWF_IMPORT_LIB}
				  Source/DWF7import.rc
				  )
endif(ODA_SHARED AND MSVC)
if(ODA_SHARED)
add_definitions(-DDWFIMPORT_EXPORTS)
else(ODA_SHARED)
add_definitions(-DDWFCORE_STATIC
                -DDWFTK_STATIC)
endif(ODA_SHARED)

if(NOT WINCE)
add_definitions(-DODA_LINT)
endif(NOT WINCE)

tdrawing_tx(${TD_DWF_IMPORT_LIB} ${TD_GS_LIB} ${TD_DB_LIB} ${TD_DBROOT_LIB} ${TD_GI_LIB} ${TD_GE_LIB} ${TD_ROOT_LIB} 
                                    ${TH_DWF7_TOOLKIT_LIB} ${TH_DWF7_W3DTK_LIB} ${TH_DWF7_WHIPTK_LIB} ${TH_DWF7_CORE_LIB} ${TD_ALLOC_LIB} ${TH_ZLIB_LIB} ${ODA_WCHAR_LIB} )
									
tdrawing_project_group(${TD_DWF_IMPORT_LIB} "Imports")

else(NOT WINCE AND NOT WINRT)
set(TD_DWF_IMPORT_LIB)
endif(NOT WINCE AND NOT WINRT)
