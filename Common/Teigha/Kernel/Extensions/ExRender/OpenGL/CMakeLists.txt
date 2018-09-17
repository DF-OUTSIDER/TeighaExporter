#
#  OpenGL library
#

if(ODA_HAS_OPENGL)

tkernel_sources(${TD_OPENGL_LIB}
    GsOpenGLVectorizer.cpp
    ExOpenGLMetafileReader.cpp
    ExOpenGLMetafileStream.cpp
    ExOpenGLMetafileWriter.cpp
    GsOpenGLStreamVectorizer.cpp
    ../CommonDeviceProps.cpp
    ../mono_dxt1_compressor.cpp
    ../TtfFontsCache.cpp
    GsOpenGLStreamVectorizer.h
    GsOpenGLVectorizer.h
    ExOpenGLMetafileReader.h
    ExOpenGLMetafileStream.h
    ExOpenGLMetafileWriter.h
    OpenGLMetafileStream.h
    CommonOpenGLProps.h
    GsOpenGLDefs.h
    GsOpenGLInclude.h
    PseudoGLDefs.h
    OpenGL_ES.h
    OpenGLExtensions.h
    ../CommonDeviceProps.h
    ../ExFPPrecisionFix.h
    ../ExGsHelpers.h
    ../MetafileTransformStack.h
    ../mono_dxt1_compressor.h
    ../TtfFontsCache.h
)

include_directories(
                    ../win
                    ..)
if(ANDROID)
  SET_SOURCE_FILES_PROPERTIES(ExOpenGLMetafileStream.cpp PROPERTIES COMPILE_FLAGS "-O0")
endif()

tkernel_library(${TD_OPENGL_LIB} ${TD_GS_LIB} ${TD_GI_LIB} ${TD_GE_LIB} ${TD_ROOT_LIB} ${TD_ALLOC_LIB} ${ODA_OPENGL_LIBS} )
tkernel_project_group(${TD_OPENGL_LIB} "Extensions")

endif(ODA_HAS_OPENGL)
