set (synergy_obj_source
  SynergyObjDPW.cpp
  DbAssocViewBorderActionParam.cpp
  DbAssocViewLabelActionParam.cpp
  DbAssocViewRepActionBody.cpp
  DbAssocViewRepActionParam.cpp
  DbAssocViewRepHatchActionParam.cpp
  DbAssocViewRepHatchManager.cpp
  DbAssocViewStyleActionParam.cpp
  DbAssocViewSymbolActionParam.cpp
  DbIdwStream.cpp
  DbIdwVersion.cpp
)
SOURCE_GROUP ("Source Files" FILES ${synergy_obj_source})

set (synergy_obj_source_impl
  Impl/DbImpAssocViewBorderActionParam.cpp
  Impl/DbImpAssocViewLabelActionParam.cpp
  Impl/DbImpAssocViewRepActionBody.cpp
  Impl/DbImpAssocViewRepActionParam.cpp
  Impl/DbImpAssocViewRepHatchActionParam.cpp
  Impl/DbImpAssocViewRepHatchManager.cpp
  Impl/DbImpAssocViewStyleActionParam.cpp
  Impl/DbImpAssocViewSymbolActionParam.cpp
  Impl/DbImpIdwStream.cpp
  Impl/DbImpIdwVersion.cpp
)
SOURCE_GROUP ("Source Files\\impl" FILES ${synergy_obj_source_impl})

set (synergy_obj_include
  SynergyObjDPWExports.h
  DbAssocViewBorderActionParam.h
  DbAssocViewLabelActionParam.h
  DbAssocViewRepActionBody.h
  DbAssocViewRepActionParam.h
  DbAssocViewRepHatchActionParam.h
  DbAssocViewRepHatchManager.h
  DbAssocViewStyleActionParam.h
  DbAssocViewSymbolActionParam.h
  DbIdwStream.h
  DbIdwVersion.h
)
SOURCE_GROUP ("Header Files" FILES ${synergy_obj_include})

set (synergy_obj_include_impl
  Impl/DbImpAssocViewBorderActionParam.h
  Impl/DbImpAssocViewLabelActionParam.h
  Impl/DbImpAssocViewRepActionBody.h
  Impl/DbImpAssocViewRepActionParam.h
  Impl/DbImpAssocViewRepHatchActionParam.h
  Impl/DbImpAssocViewRepHatchManager.h
  Impl/DbImpAssocViewStyleActionParam.h
  Impl/DbImpAssocViewSymbolActionParam.h
  Impl/DbImpIdwStream.h
  Impl/DbImpIdwVersion.h
)
SOURCE_GROUP ("Header Files\\impl" FILES ${synergy_obj_include_impl})

tdrawing_sources(${TD_SYNERGY_OBJ_LIB}
  ${synergy_obj_source}
  ${synergy_obj_source_impl}
  ${synergy_obj_include}
  ${synergy_obj_include_impl}
)

include_directories(
  ${TDRAWING_ROOT}/Include
  ${TDRAWING_ROOT}/Extensions/SynergyObjDPW
  ${TDRAWING_ROOT}/Extensions/SynergyObjDPW/Impl
  ${TDRAWING_ROOT}/Extensions/DbConstraints
  ${TDRAWING_ROOT}/Include/ModelDocObj
)

if(ODA_SHARED AND MSVC)
tdrawing_sources(${TD_SYNERGY_OBJ_LIB} SynergyObjDPW.rc)
endif(ODA_SHARED AND MSVC)

add_definitions(-DSYNERGYOBJDPW_DLL_EXPORTS)

tdrawing_tx(${TD_SYNERGY_OBJ_LIB} ${TD_MODELDOCOBJ_LIB} ${TD_DB_CONSTRAINTS_LIB} ${TD_DB_LIB} ${TD_ROOT_LIB} ${TD_ALLOC_LIB})

tdrawing_project_group(${TD_SYNERGY_OBJ_LIB} "Extensions")
