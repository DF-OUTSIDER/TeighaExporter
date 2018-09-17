#
#  WinGDI library
#

if(WINGDI_MODULE_ENABLED)

tkernel_sources(${TD_TXV_GDI_LIB}
    WinGDI.cpp
    ExGsGDIVectorizeDevice.h
    ExGsGDIVectorizeDevice.cpp
    ExGiGDIGeometry.h
    ExGiGDIGeometry.cpp
    ExGiGDIViewportClipper.h
    ExGiGDIViewportClipper.cpp
    ../win/ExGsWin32Device.h
    ../win/ExGsWin32Device.cpp
    ../win/ExGsBitmapDevice.h
    ../win/ExGsBitmapDevice.cpp
    ../CommonDeviceProps.h
    ../CommonDeviceProps.cpp
    ../TtfFontsCache.h
    ../TtfFontsCache.cpp
    ../ExGsGradientRender.h
    ../ExGsGradientRender.cpp
)

if(ODA_SHARED AND MSVC)
tkernel_sources(${TD_TXV_GDI_LIB}
    WinGDI.rc
)
endif(ODA_SHARED AND MSVC)

include_directories(
                    ../win
)

tkernel_txv(${TD_TXV_GDI_LIB} ${TD_GS_LIB} ${TD_GI_LIB} ${TD_GE_LIB} ${TD_ROOT_LIB} ${TD_ALLOC_LIB})

tkernel_project_group(${TD_TXV_GDI_LIB} "Extensions")

endif(WINGDI_MODULE_ENABLED)
