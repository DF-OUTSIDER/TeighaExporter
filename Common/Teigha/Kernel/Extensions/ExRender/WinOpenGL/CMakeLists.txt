#
#  WinOpenGL library
#

if(ODA_HAS_OPENGL)
if(MSVC OR BORLAND OR WINDOWS_GCC)

tkernel_sources(${TD_TXV_OPENGL_LIB}
    win/WinOpenGL.cpp
    win/ExGsOpenGLVectorizeDevice.cpp
    win/ExGsOpenGLVectorizeDevice.h
    ../win/ExGsWin32Device.cpp
    ../win/ExGsWin32Device.h
    ../MetafileStreamBase.h
    ../win/ExGsBitmapDevice.cpp
    ../win/ExGsBitmapDevice.h
)

if(ODA_SHARED AND MSVC)
tkernel_sources(${TD_TXV_OPENGL_LIB}
    win/WinOpenGL.rc
)
endif(ODA_SHARED AND MSVC)

add_definitions(-DWINOPENGL_EXPORTS)

if(NOT WINCE)
add_definitions(-DODA_LINT)
endif(NOT WINCE)

endif(MSVC OR BORLAND OR WINDOWS_GCC)

if (ANDROID)
tkernel_sources(${TD_TXV_OPENGL_LIB}
    Android/EsOpenGLModule.cpp
    Android/ExGsOpenGLVectorizeDevice.cpp
    Android/ExGsOpenGLVectorizeDevice.h
)
endif (ANDROID)
 
if (APPLE)
if (MACOS_X86 OR MACOS_X64)
tkernel_sources(${TD_TXV_OPENGL_LIB}
    Mac/CocoaPBCtx.mm
    Mac/ExGsOpenGLVectorizeDevice.cpp
    Mac/ExGsOpenGLVectorizeDevice.h
    Mac/MacOpenGLModule.cpp
)
else (MACOS_X86 OR MACOS_X64)
tkernel_sources(${TD_TXV_OPENGL_LIB}
    iOS/ExGsOpenGLVectorizeDevice.h
    iOS/ExGsOpenGLVectorizeDevice.cpp
    iOS/EsOpenGLModule.cpp
    iOS/EAGLProto.h
)
endif (MACOS_X86 OR MACOS_X64)
endif (APPLE) 

if(LINUX_X86 OR LINUX_X64)
tkernel_sources(${TD_TXV_OPENGL_LIB}
    X11/ExGsOpenGLVectorizeDevice.cpp
    X11/ExGsOpenGLVectorizeDevice.h
    X11/ExGsXWindowConnection.h
    X11/XOpenGLModule.cpp
)
add_definitions(-DGLX_GLXEXT_LEGACY)
endif(LINUX_X86 OR LINUX_X64) 
 
include_directories(
                    ../win
                    ../
                    ${TKERNEL_ROOT}/Extensions/ExRender
                    ${TKERNEL_ROOT}/Extensions/ExRender/OpenGL
                    )
tkernel_txv( ${TD_TXV_OPENGL_LIB} ${TD_OPENGL_LIB} ${TD_GS_LIB} ${TD_GI_LIB} ${TD_GE_LIB} ${TD_ROOT_LIB} ${TD_ALLOC_LIB} ${ODA_OPENGL_LIBS} )
tkernel_project_group(${TD_TXV_OPENGL_LIB} "Extensions")

endif(ODA_HAS_OPENGL)
