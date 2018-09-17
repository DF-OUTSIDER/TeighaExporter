#
#  XmlGLES2View executable
#

# Xml Reader sources

set(reader_include_files
    ${TKERNEL_ROOT}/Extensions/ExRender/TrXml/IO/UnivReadFiler.h
    ${TKERNEL_ROOT}/Extensions/ExRender/TrXml/IO/PathEntryDefs.h
    ${TKERNEL_ROOT}/Extensions/ExRender/TrXml/IO/BaseGlesParser.h
    ${TKERNEL_ROOT}/Extensions/ExRender/TrXml/IO/BinaryGlesParser.h
    ${TKERNEL_ROOT}/Extensions/ExRender/TrXml/IO/XmlGlesParser.h 
    ${TKERNEL_ROOT}/Extensions/ExRender/TrXml/IO/XmlGlesLoader.h 
)

set(reader_source_files
    ${TKERNEL_ROOT}/Extensions/ExRender/TrXml/IO/UnivReadFiler.cpp
    ${TKERNEL_ROOT}/Extensions/ExRender/TrXml/IO/BinaryGlesParser.cpp
    ${TKERNEL_ROOT}/Extensions/ExRender/TrXml/IO/XmlGlesParser.cpp
    ${TKERNEL_ROOT}/Extensions/ExRender/TrXml/IO/XmlGlesLoader.cpp
)

SOURCE_GROUP("Header Files\\reader" FILES ${reader_include_files})
SOURCE_GROUP("Source Files\\reader" FILES ${reader_source_files})

# ExClip sources (for testing purposes)

if(EXISTS ${TKERNEL_ROOT}/Source/Gi/ExClip)
set(exclip_include_files
    ${TKERNEL_ROOT}/Source/Gi/ExClip/ExClipChains.h
    ${TKERNEL_ROOT}/Source/Gi/ExClip/ExClipContext.h
    ${TKERNEL_ROOT}/Source/Gi/ExClip/ExClipDefs.h
    ${TKERNEL_ROOT}/Source/Gi/ExClip/ExClipLog.h
    ${TKERNEL_ROOT}/Source/Gi/ExClip/ExClipPCtx.h
    ${TKERNEL_ROOT}/Source/Gi/ExClip/ExClipPoly.h
    ${TKERNEL_ROOT}/Source/Gi/ExClip/ExClipSBase.h
    ${TKERNEL_ROOT}/Source/Gi/ExClip/ExClipSections.h
    ${TKERNEL_ROOT}/Source/Gi/ExClip/ExClipSpace.h
)
SOURCE_GROUP("Header Files\\ExClip" FILES ${exclip_include_files})
set(exclip_source_files
    ${TKERNEL_ROOT}/Source/Gi/ExClip/ExClipContext.cpp
    ${TKERNEL_ROOT}/Source/Gi/ExClip/ExClipCurve.cpp
    ${TKERNEL_ROOT}/Source/Gi/ExClip/ExClipLog.cpp
    ${TKERNEL_ROOT}/Source/Gi/ExClip/ExClipPoly.cpp
    ${TKERNEL_ROOT}/Source/Gi/ExClip/ExClipSBase.cpp
    ${TKERNEL_ROOT}/Source/Gi/ExClip/ExClipSections.cpp
    ${TKERNEL_ROOT}/Source/Gi/ExClip/ExClipShape.cpp
    ${TKERNEL_ROOT}/Source/Gi/ExClip/ExClipSpace.cpp
    ${TKERNEL_ROOT}/Source/Gi/ExClip/ExClipSPlane.cpp
    ${TKERNEL_ROOT}/Source/Gi/ExClip/ExClipSPoly.cpp
)
SOURCE_GROUP("Source Files\\ExClip" FILES ${exclip_source_files})
set(exclip_test_files
    ${exclip_include_files}
    ${exclip_source_files}
    ExClipClient.cpp
)
endif(EXISTS ${TKERNEL_ROOT}/Source/Gi/ExClip)

# Platform Local Renderer sources

if(GLES2EMUL_ENABLED)

set(platform_source_files
    ${TKERNEL_ROOT}/Extensions/ExRender/WinGLES2/win/ExGsGLES2LocalContext.cpp
)
SOURCE_GROUP("Source Files\\gl2_win" FILES ${platform_source_files})

add_definitions(-DGLES2EMUL_ENABLED)
set(platform_libraries libGLESv2 libEGL)
set(platform_includes  ${TKERNEL_ROOT}/Extensions/ExRender/WinGLES2/win/gles_sdk/include)
if(WIN64)
link_directories(${TKERNEL_ROOT}/Extensions/ExRender/WinGLES2/win/gles_sdk/x86-64)
else(WIN64)
link_directories(${TKERNEL_ROOT}/Extensions/ExRender/WinGLES2/win/gles_sdk/x86)
endif(WIN64)

endif(GLES2EMUL_ENABLED)

if(GLES2COMPATIBILITY_ENABLED)

set(platform_include_files
    ${TKERNEL_ROOT}/Include/Tr/render/gl2/compatibility/gles2_ext.h
)
SOURCE_GROUP("Header Files\\gl2_win" FILES ${platform_include_files})

set(platform_source_files
    ${TKERNEL_ROOT}/Extensions/ExRender/WinGLES2/compatibility/gles2_ext.cpp
    ${TKERNEL_ROOT}/Extensions/ExRender/WinGLES2/win/ExGsGLES2LocalContext.cpp
)
SOURCE_GROUP("Source Files\\gl2_win" FILES ${platform_source_files})

set(platform_libraries ${ODA_OPENGL_LIBS})

endif(GLES2COMPATIBILITY_ENABLED)

# Application sources

tkernel_sources(XmlGLES2View
    WinMain.cpp
    AppClient.h
    ThreadPool.cpp
    XmlViewClient.cpp
    ${exclip_test_files}
    ${reader_include_files}
    ${reader_source_files}
    ${platform_include_files}
    ${platform_source_files}
)

include_directories(${TKERNEL_ROOT}
                    ${TKERNEL_ROOT}/Include
                    ${TKERNEL_ROOT}/ThirdParty/tinyxml
                    ${TKERNEL_ROOT}/Include/Tr
                    ${TKERNEL_ROOT}/Include/Tr/vec
                    ${TKERNEL_ROOT}/Include/Tr/render
                    ${TKERNEL_ROOT}/Include/Tr/render/gl2
                    ${TKERNEL_ROOT}/Include/Tr/render/gl2/vec
                    ${TKERNEL_ROOT}/Include/Tr/render/gl2/compatibility
                    ${TKERNEL_ROOT}/Include/Tr/extbar
                    ${TKERNEL_ROOT}/Extensions/ExRender
                    ${TKERNEL_ROOT}/Extensions/ExRender/TrXml
                    ${TKERNEL_ROOT}/Extensions/ExRender/TrXml/IO
                    ${TKERNEL_ROOT}/Extensions/ExServices
                    ${platform_includes}
)

if(ODA_SHARED)
set ( od_xmlgles2view_ex_libs ${TD_ROOT_LIB} ${TD_GE_LIB} ${TR_BASE_LIB} ${TR_GL2_LIB} ${platform_libraries} )
else(ODA_SHARED)
set ( od_xmlgles2view_ex_libs ${TD_GE_LIB} ${TD_SPATIALINDEX_LIB} ${TD_ROOT_LIB} ${TR_BASE_LIB} ${TR_GL2_LIB}
                              ${TH_CONDITIONAL_LIBCRYPTO} ${platform_libraries} )
endif(ODA_SHARED)

if(MSVC)
tkernel_sources(XmlGLES2View XmlGLES2View.rc)
endif(MSVC)

tkernel_executable(XmlGLES2View ${od_xmlgles2view_ex_libs} ${TR_EXTBAR_LIB}
                           ${ODA_ADD_SISL} 
                           ${TD_EXLIB}
                           ${TD_ROOT_LIB}
                           ${TD_ALLOC_LIB}
                           ${TH_TINYXML_LIB}
                           ${TD_SPATIALINDEX_LIB}
)

SET_TARGET_PROPERTIES(XmlGLES2View PROPERTIES LINK_FLAGS /entry:"wWinMainCRTStartup")
set_target_properties(XmlGLES2View PROPERTIES WIN32_EXECUTABLE 1)

tkernel_project_group(XmlGLES2View "Examples")

remove_definitions()
