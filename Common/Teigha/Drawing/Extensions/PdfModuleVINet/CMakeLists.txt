#
#  PdfModuleVINet library
#
if(MSVC90 OR MSVC10 OR MSVC11 OR MSVC14)
set (td_pdfmodulevinet_include
  PdfModuleVINet.h
  PdfModuleVINetVersion.h
)
SOURCE_GROUP ("Header Files" FILES ${td_pdf_include})

set (td_pdfmodulevinet_source
  PdfModuleVINet.cpp
  ${TKERNEL_ROOT}/../PdfModuleVI/PdfModuleVINet/PdfModuleVINetVerify.cpp
)
SOURCE_GROUP ("Source Files" FILES ${td_pdf_source})

tdrawing_sources(${TD_PDFMODULEVINET_LIB}
  ${td_pdfmodulevinet_include}
  ${td_pdfmodulevinet_source}
  ${td_pdfmodulevinet_resources}
  )
  
  include_directories(${TDRAWING_ROOT}
                    ${TDRAWING_ROOT}/Include
					${TDRAWING_ROOT}/Extensions/PdfModuleVINet)

tdrawing_sources(${TD_PDFMODULEVINET_LIB}
				  PdfModuleVINet.rc
				  )
  
set(CMAKE_MANIFEST 1) 

tdrawing_tx(${TD_PDFMODULEVINET_LIB} ${TD_DB_LIB} ${TD_ROOT_LIB} ${TD_ALLOC_LIB} Version.lib)

tdrawing_project_group(${TD_PDFMODULEVINET_LIB} "PDF")
add_dependencies ( ${TD_PDFMODULEVINET_LIB} ${TD_PDFKEY_VI_LIB} )

endif(MSVC90 OR MSVC10 OR MSVC11 OR MSVC14)