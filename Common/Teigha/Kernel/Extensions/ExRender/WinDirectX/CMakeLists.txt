#
#  WinDirectX library
#

if(MSVC)
if(NOT WINCE)

tkernel_sources(${TD_TXV_DIRECTX_LIB}
    WinDirectXModule.cpp
    ExGiDirectXGeometry.cpp
    ExGsDirectXDevice.cpp
    ../CommonDeviceProps.cpp
    ../TtfFontsCache.cpp
    ../win/ExGsWin32Device.cpp
    ../win/ExGsBitmapDevice.cpp
    ExGsDirectXDevice.h
    ExGiDirectXGeometry.h
    D3DStateManager.h
    DxErr.h
    D3DInclude.h
    ../CommonDeviceProps.h
    ../TtfFontsCache.h
    ../ExFPPrecisionFix.h
    ../win/ExGsWin32Device.h
    ../win/ExGsBitmapDevice.h
)

if(ODA_SHARED AND MSVC)
tkernel_sources(${TD_TXV_DIRECTX_LIB}
    WinDirectX.rc
)
endif(ODA_SHARED AND MSVC)

include_directories(
                    ../win
                    ../
                    "${DXSDK_DIR}"
                   )

tkernel_txv(${TD_TXV_DIRECTX_LIB} ${TD_GS_LIB} ${TD_GI_LIB} ${TD_GE_LIB} ${TD_ROOT_LIB} ${TD_ALLOC_LIB})

tkernel_project_group(${TD_TXV_DIRECTX_LIB} "Extensions")

endif(NOT WINCE)
endif(MSVC)
