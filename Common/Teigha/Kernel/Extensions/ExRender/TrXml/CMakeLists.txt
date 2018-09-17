#
#  XmlGLES2 library
#

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/ExGsGLES2IdRegistratorImpl.cpp)

tkernel_sources(${TR_XML_LIB}
    ExGsGLES2IdRegistratorImpl.cpp
    ExGsGLES2XmlRendition.cpp
    ExGsGLES2XmlSharingProvider.cpp
    GLES2Module.cpp
    ExGsGLES2IdRegistratorImpl.h
    ExGsGLES2XmlExports.h
    ExGsGLES2XmlRendition.h
    ExGsGLES2XmlSharingProvider.h
    XmlServer.h
  )

if(ODA_SHARED AND MSVC)
tkernel_sources(${TR_XML_LIB}
  XmlGLES2.rc
  )
endif(ODA_SHARED AND MSVC)

add_definitions(-DODGLES2XML_EXPORTS)

include_directories(
        ${TKERNEL_ROOT}/Include/Tr
        ${TKERNEL_ROOT}/Include/Tr/vec
        ${TKERNEL_ROOT}/Extensions/ExRender
  )

tkernel_txv(${TR_XML_LIB}
       ${TR_VEC_LIB}
       ${TR_BASE_LIB}
       ${TD_GS_LIB}
       ${TD_GI_LIB}
       ${TD_GE_LIB}
       ${TD_ROOT_LIB}
       ${TD_ALLOC_LIB})

tkernel_project_group(${TR_XML_LIB} "Extensions")

remove_definitions()

endif (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/ExGsGLES2IdRegistratorImpl.cpp)

add_subdirectory(IO)
