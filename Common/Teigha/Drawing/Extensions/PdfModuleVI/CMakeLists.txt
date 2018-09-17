#
# PDFModuleVI module
#


set (td_pdfmodulevi_source
  PdfDirectWrapperImpl.cpp
  VgWrappers.cpp
  InternalTests.cpp
)
SOURCE_GROUP ("Source Files" FILES ${td_pdfmodulevi_source})

set (td_pdfmodulevi_include
  PdfDirectWrapperImpl.h
  VgWrappers.h
)
SOURCE_GROUP ("Header Files" FILES ${td_pdfmodulevi_include})

set (td_pdfmodulevi_resources
  PdfUnderlay.rc
)
SOURCE_GROUP ("Resource Files" FILES ${td_pdfmodulevi_resources})

set (td_pdfmodulevi_common
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
SOURCE_GROUP ("Common" FILES ${td_pdfmodulevi_common})

tdrawing_sources(${TD_PDFMODULEVI_LIB}
  ${td_pdfmodulevi_source}
  ${td_pdfmodulevi_include}
  ${td_pdfmodulevi_resources}
  ${td_pdfmodulevi_common}
)

include_directories(${TDRAWING_ROOT}/
                    ${TDRAWING_ROOT}/Extensions/PdfUnderlayCommon
                    ${TDRAWING_ROOT}/Extensions/PdfModuleVI
                    ${TDRAWING_ROOT}/Source/database
                    )


add_definitions(-DPDFEXPORT_EXPORTS)


tdrawing_tx(${TD_PDFMODULEVI_LIB} ${TD_DB_LIB} ${TD_DBROOT_LIB} ${TD_GE_LIB} ${TD_ROOT_LIB} ${TD_ALLOC_LIB} ${PDF_FLYSDK} )

tdrawing_project_group(${TD_PDFMODULEVI_LIB} "PDF")
