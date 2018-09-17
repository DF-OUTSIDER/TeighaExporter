#
# TD_PdfImport library
#
if(NOT WINCE AND NOT WINRT)

tdrawing_sources(${TD_PDF_IMPORT_LIB}
    Include/PdfImport.h
    Source/PdfImportImpl.cpp
    Source/PdfImportImpl.h
    Source/PdfProperties.h
    Source/DbDeviceDriver.h
    Source/DbDeviceDriver.cpp
    Source/PdfImportFunctions.cpp
    Source/ProgressiveRenderer.cpp
    Source/RenderStatus.cpp
    Source/RenderStatus.h
    Source/PdfImportFunctions.h
    Source/ProgressiveRenderer.h
    Source/ImageRenderer.h
    Source/ImageRenderer.cpp
    )
			  
include_directories(
    Include
    ${TKERNEL_ROOT}/Imports/PdfImport/Include
    ${TKERNEL_ROOT}/Imports/PdfImport/Source
    ${TH_ROOT}/pdfium
    ${TH_ROOT}/pdfium/third_party
    ${TH_ROOT}/freetype/freetype-${FREETYPE_VER}/include
    ${TH_ROOT}/freetype/freetype-${FREETYPE_VER}/include/freetype
    ${TDRAWING_ROOT}/Imports/ImportBase
    )

add_definitions(-DPDFEXPORT_EXPORTS)

if(ODA_SHARED)
add_definitions(-DPDFIMPORT_EXPORTS)
#-DFPDFSDK_EXPORTS
else(ODA_SHARED)
add_definitions()
endif(ODA_SHARED)

IF(WIN32)
add_definitions(-DNOMINMAX)
ENDIF()

# PDFium definitions
include(${TH_ROOT}/pdfium/definitions.txt)

set (pdfium_libs ${TH_FT_LIB} ${TH_THIRDPARTYRASTER_LIB} ${TH_ZLIB_LIB})

#if(ODA_SHARED)
tdrawing_tx(${TD_PDF_IMPORT_LIB} ${TD_GS_LIB} ${TD_DB_LIB} ${TD_DBROOT_LIB} ${TD_GI_LIB} ${TD_GE_LIB} ${TD_ROOT_LIB} ${TH_PDFIUM_LIB} ${pdfium_libs} ${TD_ALLOC_LIB})
#endif(ODA_SHARED)

tdrawing_project_group(${TD_PDF_IMPORT_LIB} "Imports")

set_property(TARGET ${TD_PDF_IMPORT_LIB} APPEND_STRING PROPERTY COMPILE_FLAGS " -std=c++11")

endif(NOT WINCE AND NOT WINRT)
