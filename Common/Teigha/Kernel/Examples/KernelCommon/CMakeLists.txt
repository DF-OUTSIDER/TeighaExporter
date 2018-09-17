
set (oda_exlib_exservices_src
    ${TKERNEL_ROOT}/Extensions/ExServices/OdFileBuf.cpp
    ${TKERNEL_ROOT}/Extensions/ExServices/RxSystemServicesImpl.cpp
    ${TKERNEL_ROOT}/Extensions/ExServices/ExStringIO.cpp
    ${TKERNEL_ROOT}/Extensions/ExServices/ExKWIndex.cpp
	)
	
SOURCE_GROUP("Source Files\\ExServices" FILES ${oda_exlib_exservices_src}) 
	
set ( oda_exlib_exrender_src
    ${TKERNEL_ROOT}/Extensions/ExRender/BmpTilesGen.cpp
	)
	
SOURCE_GROUP("Source Files\\ExRender" FILES ${oda_exlib_exrender_src}) 
	
set ( oda_exlib_odreadex_src
	)
	
SOURCE_GROUP("Source Files\\OdReadEx" FILES ${oda_exlib_odreadex_src}) 
	
set ( oda_exlib_editor_src
	)
	
SOURCE_GROUP("Source Files\\Editor" FILES ${oda_exlib_editor_src}) 
	
set ( oda_exlib_common_src
	)
	
SOURCE_GROUP("Source Files\\Common" FILES ${oda_exlib_editor_src}) 

tkernel_sources(${TD_EXLIB}
				  ${oda_exlib_exservices_src}
				  ${oda_exlib_exrender_src}
				  ${oda_exlib_odreadex_src}
				  ${oda_exlib_editor_src}
				  ${oda_exlib_common_src}
				  )
				  
include_directories(${TKERNEL_ROOT}/Extensions/ExRender
                    ${TKERNEL_ROOT}/Extensions/ExServices
                   )

tkernel_library(${TD_EXLIB})

tkernel_project_group(${TD_EXLIB} "Examples")
