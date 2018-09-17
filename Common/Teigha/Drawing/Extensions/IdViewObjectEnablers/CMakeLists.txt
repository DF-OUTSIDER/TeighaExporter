#
#  AcIdViewObj library
#

tdrawing_sources(${TD_ID_LIB}
    IdBlockReference.cpp
    IdViewCircle.cpp
    IdViewLine.cpp
    IdViewEllipse.cpp
    IdViewSpline.cpp
    IdViewArc.cpp
    IdComplexEntity.cpp
    TD_Id.cpp
    IdViewSpline.h
    IdComplexEntity.h
    IdViewArc.h
    IdBlockReference.h
    IdViewCircle.h
    IdViewLine.h
    IdViewEllipse.h
	)

include_directories()
				  
if(ODA_SHARED AND MSVC)	
tdrawing_sources(${TD_ID_LIB}
				  IdViewObj.rc
				  )
endif(ODA_SHARED AND MSVC)	
add_definitions(-DID_DLL_EXPORTS)

if(NOT WINCE)
add_definitions(-DODA_LINT)
endif(NOT WINCE)

tdrawing_tx(${TD_ID_LIB} ${TD_DB_LIB} ${TD_GI_LIB} ${TD_GE_LIB} ${TD_ROOT_LIB} ${TD_ALLOC_LIB})

tdrawing_project_group(${TD_ID_LIB} "Extensions")