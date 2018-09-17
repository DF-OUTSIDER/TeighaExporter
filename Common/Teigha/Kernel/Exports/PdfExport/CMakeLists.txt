#
# TD_PdfExport library
#

tkernel_sources(${TD_PDF_EXPORT_LIB}
    Source/PDFShxGeomStore.cpp
    Source/PdfExportImpl.cpp
    Source/PDF2dExportDevice.cpp
    Source/PDFType3Optimizer.cpp
    Source/Pdf2Dc.cpp
    Source/PdfAux.cpp
    Source/PDF2dExportView.cpp
    Source/PdfExport.cpp
    Source/PDFLinearizer.cpp
    Source/PdfExportClipper.cpp
    Source/PdfExportGiDrawablePE.cpp
    Source/PrcContextForPdfExport.cpp
    Source/PdfExportImplXObject.cpp
    Source/PdfExportParamsHolder.cpp
    Source/PdfExportImplBase.cpp
    Include/PdfAux.h
    Include/PdfExport.h
    Include/PdfExportImpl.h
    Include/Pdf2dExportDevice.h
    Include/PdfShxGeomStore.h
    Include/PdfType3Optimizer.h
    Include/PdfExportVersion.h
    Include/PdfExportParams.h
    Include/Pdf2dExportView.h
    Include/PdfExportCommon.h
    Include/PdfExportDef.h
    Include/Pdf2PrcExportParams.h
    Include/PdfExportGiDrawablePE.h
    Include/PdfLinearizer.h
    Include/PdfExportClipper.h
    Include/PdfExportImplXObject.h
    Include/PdfExportParamsHolder.h
    Include/PdfExportImplBase.h
    Include/PdfExportParamsForXObject.h
    )
			  
include_directories(
                    ${TKERNEL_ROOT}/Exports/2dExport/Include
					Include
					PDFToolkit)
				  
if(ODA_SHARED AND MSVC)
tkernel_sources(${TD_PDF_EXPORT_LIB}
				  Source/PdfExport.rc
				  )
endif(ODA_SHARED AND MSVC)
add_definitions(-DPDFEXPORT_EXPORTS)

if(NOT WINCE)
add_definitions(-DODA_LINT)
endif(NOT WINCE)

if(ODA_SHARED)
#  add_definitions(
#                  -DPCRE_EXPORT_API
#                  )
else(ODA_SHARED)
  add_definitions(-DPCRE_STATIC
					)
endif(ODA_SHARED)

tkernel_tx(${TD_PDF_EXPORT_LIB} ${TD_2D_EXPORT_LIB} ${TD_PDF_TOOLKIT_LIB} ${TD_DBROOT_LIB} ${TD_ROOT_LIB} ${TD_GS_LIB} ${TD_BR_LIB} ${TD_GI_LIB}
		 ${TD_GE_LIB} ${TD_ROOT_LIB} ${TH_QPDF_LIB} ${TH_PCRE_LIB} ${TD_ALLOC_LIB} ${TH_STSFLIB_LIB} ${TH_ZLIB_LIB})

tkernel_project_group(${TD_PDF_EXPORT_LIB} "Exports")
