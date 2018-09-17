
tdrawing_sources( ${TD_REVISION_LIB} 
                  RevisionControl.h
                  RevisionControl.cpp
				          RevisionControlGraph.h
                  RevisionControlGraph.cpp 
                  RevisionSerialization.h
  RevisionSerialization.cpp
  RevisionControlModule.h
  RevisionControlModule.cpp
)

if(ODA_SHARED AND MSVC)
tdrawing_sources(${TD_REVISION_LIB} module.rc)

endif(ODA_SHARED AND MSVC)

include_directories(${ODA_SOURCE_ROOT}
                    ${TKERNEL_ROOT}
                    ${TDRAWING_ROOT}
                    ${TKERNEL_ROOT}/Source/root
                    ${TKERNEL_ROOT}/Source/DbRoot
                    ${TDRAWING_ROOT}/Source/Tf
                    ${TKERNEL_ROOT}/Include
					${TH_ROOT}/rapidjson/include
					${TKERNEL_ROOT}/Extensions/ExRender/TrXml/IO
)

tdrawing_tx(
  ${TD_REVISION_LIB}
  ${TD_EXLIB}
  ${TD_ROOT_LIB}
  ${TD_ALLOC_LIB}
  ${TD_TF_LIB}
  ${TD_DB_LIB}
)

tdrawing_project_group(${TD_REVISION_LIB} "Extensions")