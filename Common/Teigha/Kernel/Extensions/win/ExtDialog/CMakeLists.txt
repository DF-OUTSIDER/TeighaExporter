#
#  FileDlgExt library
#

if(MSVC)
tkernel_sources(${TD_DB_FILEDLG_EXT_LIB}
    FileDlgExt.rc
    res/FileDlgExt.rc2
    PreviewFileDlg.cpp
    FileDlgExt.cpp
    StdAfx.cpp
    FileDlgExt.h
    StdAfx.h
    Resource.h
    PreviewFileDlg.h
	)

include_directories()
				  
add_definitions(-DFILEEXT_EXPORT)

if(DEFINED ODA_MFC_ADDITIONAL_DEFINITIONS)
add_definitions(${ODA_MFC_ADDITIONAL_DEFINITIONS})
endif(DEFINED ODA_MFC_ADDITIONAL_DEFINITIONS)

set(CMAKE_MFC_FLAG ${ODA_MFC_FLAG}) 

tkernel_component(${TD_DB_FILEDLG_EXT_LIB})

tkernel_project_group(${TD_DB_FILEDLG_EXT_LIB} "Extensions")
endif(MSVC)
