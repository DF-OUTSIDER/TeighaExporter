#
#  WinGLES2 library
#

if((APPLE AND NOT MACOS_X86 AND NOT MACOS_X64) OR ANDROID OR GLES2EMUL_ENABLED OR GLES2COMPATIBILITY_ENABLED)

# iOS-specific sources
if(APPLE AND NOT MACOS_X86 AND NOT MACOS_X64)
set(gles2_platform_sources
    iOS/ExGsGLES2LocalContext.cpp
    iOS/ExGsGLES2LocalContextImpl.mm
    iOS/ExGsGLES2LocalContextImpl.h
)
set(gles2_platform_libraries GLESv2)
endif(APPLE AND NOT MACOS_X86 AND NOT MACOS_X64)

# Android-specific sources
if(ANDROID)
set(gles2_platform_sources
    Android/ExGsGLES2LocalContext.cpp
)
# EGL is available only since 9 platform
set(gles2_platform_libraries GLESv2 EGL android)
endif(ANDROID)

# Windows-specific (emulator) sources
if(GLES2EMUL_ENABLED)
set(gles2_platform_sources
    win/ExGsGLES2LocalContext.cpp
)
add_definitions(-DGLES2EMUL_ENABLED)
set(gles2_platform_libraries libGLESv2 libEGL)
endif(GLES2EMUL_ENABLED)

# Emscripten-specific sources
if(EMCC)
set(gles2_platform_sources
    emcc/ExGsGLES2LocalContext.cpp
)
set(gles2_platform_libraries libGLESv2 libEGL)
endif(EMCC)

# Compatible PC platforms specific sources
if(GLES2COMPATIBILITY_ENABLED)
if(MSVC OR BORLAND)
set(gles2_platform_sources
    win/ExGsGLES2LocalContext.cpp
    compatibility/gles2_ext.cpp
    ${TKERNEL_ROOT}/Include/Tr/render/gl2/compatibility/gles2_ext.h
)
endif(MSVC OR BORLAND)
if(LINUX_X86 OR LINUX_X64 AND NOT EMCC)
set(gles2_platform_sources
    X11/ExGsGLES2LocalContext.cpp
    X11/ExGsXWindowConnection.h
    compatibility/gles2_ext.cpp
    ${TKERNEL_ROOT}/Include/Tr/render/gl2/compatibility/gles2_ext.h
)
endif(LINUX_X86 OR LINUX_X64 AND NOT EMCC)
if(APPLE AND (MACOS_X86 OR MACOS_X64))
set(gles2_platform_sources
    Mac/ExGsGLES2LocalContext.cpp
    Mac/CocoaOGLOffscreen.mm
    Mac/CocoaOGLOnscreen.mm
    compatibility/gles2_ext.cpp
    ${TKERNEL_ROOT}/Include/Tr/render/gl2/compatibility/gles2_ext.h
)
endif(APPLE AND (MACOS_X86 OR MACOS_X64))
set(gles2_platform_libraries ${ODA_OPENGL_LIBS})
endif(GLES2COMPATIBILITY_ENABLED)

# Source files
tkernel_sources(${TD_TXV_GLES2_LIB}
    ExGsGLES2LocalRenditionGsClient.h
    ExGsGLES2LocalRenditionGsClient.cpp
    ExGsGLES2LocalSharingProvider.h
    ExGsGLES2LocalSharingProvider.cpp
    GLES2Module.cpp
    ${gles2_platform_sources}
)

if(ODA_SHARED AND MSVC)
tkernel_sources(${TD_TXV_GLES2_LIB}
    win/ExGsGLES2RSTestDialog.h
    win/ExGsGLES2RSTestDialog.cpp
    win/ExtWinRes.h
    win/WinGLES2.rc
)
set(gles2_platform_libraries ${gles2_platform_libraries} ${TR_EXTBAR_LIB})
endif(ODA_SHARED AND MSVC)

if(GLES2EMUL_ENABLED)
include_directories(
    ${TKERNEL_ROOT}/Include/Tr
    ${TKERNEL_ROOT}/Include/Tr/vec
    ${TKERNEL_ROOT}/Include/Tr/render
    ${TKERNEL_ROOT}/Include/Tr/render/gl2
    ${TKERNEL_ROOT}/Include/Tr/render/gl2/compatibility
    ${TKERNEL_ROOT}/Include/Tr/extbar
    ${TKERNEL_ROOT}/Extensions/ExRender
    ${TKERNEL_ROOT}/Extensions/ExRender/WinGLES2/win/gles_sdk/include)
if(WIN64)
link_directories(${TKERNEL_ROOT}/Extensions/ExRender/WinGLES2/win/gles_sdk/x86-64)
else(WIN64)
link_directories(${TKERNEL_ROOT}/Extensions/ExRender/WinGLES2/win/gles_sdk/x86)
endif(WIN64)
else(GLES2EMUL_ENABLED)
include_directories(
    ${TKERNEL_ROOT}/Include/Tr
    ${TKERNEL_ROOT}/Include/Tr/vec
    ${TKERNEL_ROOT}/Include/Tr/render
    ${TKERNEL_ROOT}/Include/Tr/render/gl2
    ${TKERNEL_ROOT}/Include/Tr/render/gl2/compatibility
    ${TKERNEL_ROOT}/Include/Tr/extbar
    ${TKERNEL_ROOT}/Extensions/ExRender
)
endif(GLES2EMUL_ENABLED)

tkernel_txv(${TD_TXV_GLES2_LIB} ${TR_VEC_LIB} ${TR_GL2_LIB} ${TR_BASE_LIB} ${TD_GS_LIB} ${TD_GI_LIB} ${TD_SPATIALINDEX_LIB} ${TD_GE_LIB} ${TD_ROOT_LIB} ${TD_ALLOC_LIB} ${TH_FT_LIB} ${gles2_platform_libraries})

tkernel_project_group(${TD_TXV_GLES2_LIB} "Extensions")

remove_definitions()

endif((APPLE AND NOT MACOS_X86 AND NOT MACOS_X64) OR ANDROID OR GLES2EMUL_ENABLED OR GLES2COMPATIBILITY_ENABLED)
