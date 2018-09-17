#
# PDFium module
#

set (td_pdfium_source
  PdfiumWrapperImpl.cpp
  PdfiumWrapperImpl1.cpp
)
SOURCE_GROUP ("Source Files" FILES ${td_pdfium_source})

set (td_pdfium_include
  PdfiumWrapperImpl.h
)
SOURCE_GROUP ("Header Files" FILES ${td_pdfium_include})

set (td_pdfium_common
  ${TDRAWING_ROOT}/Extensions/PdfUnderlayCommon/PdfDirectWrapper.h
  ${TDRAWING_ROOT}/Extensions/PdfUnderlayCommon/PdfDirectWrapper.cpp
  ${TDRAWING_ROOT}/Extensions/PdfUnderlayCommon/PdfUnderlay.h
  ${TDRAWING_ROOT}/Extensions/PdfUnderlayCommon/PdfUnderlay.cpp
  ${TDRAWING_ROOT}/Extensions/PdfUnderlayCommon/PdfUnderlayFileImpl.cpp
  ${TDRAWING_ROOT}/Extensions/PdfUnderlayCommon/PdfUnderlayFileImpl.h
  ${TDRAWING_ROOT}/Extensions/PdfUnderlayCommon/PdfUnderlayHostPEImpl.cpp
  ${TDRAWING_ROOT}/Extensions/PdfUnderlayCommon/PdfUnderlayHostPEImpl.h
  ${TDRAWING_ROOT}/Extensions/PdfUnderlayCommon/PdfUnderlayItemImpl.cpp
  ${TDRAWING_ROOT}/Extensions/PdfUnderlayCommon/PdfUnderlayItemImpl.h
  ${TDRAWING_ROOT}/Extensions/PdfUnderlayCommon/OdColorAdjuster.h
  ${TDRAWING_ROOT}/Extensions/PdfUnderlayCommon/OdColorAdjuster.cpp
)
SOURCE_GROUP ("Common" FILES ${td_pdfium_common})

tdrawing_sources(${TD_PDFIUMMODULE_LIB}
  ${td_pdfium_source}
  ${td_pdfium_include}
  ${td_pdfium_common}
)

include_directories(${TDRAWING_ROOT}/
                    ${TDRAWING_ROOT}/Extensions/PdfUnderlayCommon
                    ${TDRAWING_ROOT}/Extensions/PdfiumModule
                    ${TDRAWING_ROOT}/Source/database
                    ${TH_ROOT}/pdfium
                    ${TH_ROOT}/pdfium/third_party
                    ${TH_ROOT}/freetype/freetype-${FREETYPE_VER}/include
                    ${TH_ROOT}/freetype/freetype-${FREETYPE_VER}/include/freetype
                    )

IF(WIN32)
add_definitions(-DNOMINMAX)
ENDIF()

IF(ODA_SHARED)
#  #add_definitions(-DFPDFSDK_EXPORTS)
ENDIF()

add_definitions(-DPDFEXPORT_EXPORTS)

#IF(NOT ODA_SHARED)
set (pdfium_libs ${TH_FT_LIB} ${TH_THIRDPARTYRASTER_LIB} ${TH_ZLIB_LIB})
#ENDIF()

tdrawing_tx(${TD_PDFIUMMODULE_LIB} ${TD_DB_LIB} ${TD_DBROOT_LIB} ${TD_GE_LIB} ${TD_ROOT_LIB} ${TH_PDFIUM_LIB} ${pdfium_libs} ${TD_ALLOC_LIB} )

tdrawing_project_group(${TD_PDFIUMMODULE_LIB} "PDF")

set_property(TARGET ${TD_PDFIUMMODULE_LIB} APPEND_STRING PROPERTY COMPILE_FLAGS " -std=c++11")
