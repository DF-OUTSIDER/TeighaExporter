if(TG_ROOT)

set (td_dgn_import_source
      DgnImportTables.cpp
      DgnImportGeneric.cpp
      DgnImportImpl.cpp
      DgnImportLS.cpp
      DgnImportDimensions.cpp
      DgnImportPatterns.cpp
      DgnImportContext.cpp
      DgnImportCommon.cpp
      DgnImportLine.cpp
      DgnImportLight.cpp
      DgnImportArc.cpp
      DgnImportText.cpp
      DgnImportLineString.cpp
      DgnImportSharedCell.cpp
      DgnImportRasterAttach.cpp
      DgnImportEllipse.cpp
      DgnImportMesh.cpp
      DgnImportXRef.cpp
      DgnImportComplexShape.cpp
      DgnImportComplexString.cpp
      DgnImport3dObject.cpp
      DgnImportMultiline.cpp
      DgnImportDimAssoc.cpp
      DgnImportTableElement.cpp
      ${TG_ROOT}/Extensions/ExServices/ExDgnHostAppServices.cpp
)


if(MSVC AND ODA_SHARED)
set (td_dgn_import_source
  ${td_dgn_import_source}
    DgnImport.rc 
)
endif(MSVC AND ODA_SHARED)

SOURCE_GROUP ("Source Files" FILES ${td_dgn_import_source})

set (td_dgn_import_include
      DgnImport.h
      DgnImportImpl.h
      DgnImportCommon.h
      DgnImportContext.h
      DgnImportPatterns.h
      DgnImportDimensions.h
      DgnImportLS.h
      DgnImportPE.h
      DgnImportLine.h
      DgnImportLight.h
      DgnImportArc.h
      DgnImportText.h
      DgnImportLineString.h
      DgnImportSharedCell.h
      DgnImportRasterAttach.h      
      DgnImportEllipse.h
      DgnImportMesh.h
      DgnImportBSplineCurve.h
      DgnImportOle.h
      DgnImportCellHeader.h
      DgnImportXRef.h
      DgnImportTextNode.h
      DgnImportComplexShape.h
      DgnImportComplexString.h
      DgnImportShape.h
      DgnImport3dObject.h
      DgnImportMultiline.h
      DgnImportDimAssoc.h
      DgnImportTableElement.h
)

SOURCE_GROUP ("Header Files" FILES ${td_dgn_import_include})

tdrawing_sources(${TD_DGN_IMPORT_LIB}
  ${td_dgn_import_source}
  ${td_dgn_import_include}
          )

include_directories(${TKERNEL_ROOT}/Extensions/ExServices
                    ${TDRAWING_ROOT}/Extensions/ExServices
                    ${TG_ROOT}/include
                    ${TG_ROOT}/Extensions/ExServices
                    ${TKERNEL_ROOT}/Extensions/BrepBuilderFiller/Include
                    )


tdrawing_tx(${TD_DGN_IMPORT_LIB} ${TG_DB_LIB} ${TD_GS_LIB} ${TD_DGNLS_LIB} ${TD_DB_LIB} ${TD_DBROOT_LIB} ${TD_GI_LIB} ${TD_BREPBUILDERFILLER_LIB} ${TD_BREPBUILDER_LIB} ${TD_BR_LIB} ${TD_GE_LIB} ${TD_ROOT_LIB} ${TD_ALLOC_LIB})
									
tdrawing_project_group(${TD_DGN_IMPORT_LIB} "Imports")
else(TG_ROOT)

message("Warning: TG_ROOT undefined, DGNImport disabled")

endif(TG_ROOT)
