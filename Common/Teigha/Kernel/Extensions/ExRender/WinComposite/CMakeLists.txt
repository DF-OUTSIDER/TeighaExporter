#
#  WinComposite library
#

if(MSVC AND NOT WINCE AND NOT WINRT OR BORLAND)

tkernel_sources(${TD_TXV_COMPOSITE_LIB}
    ../CommonDeviceProps.cpp
    ../win/ExGsWin32Device.cpp
    ../win/ExGsBitmapDevice.cpp
    ../win/ExGsScreenDevice.cpp
    ExGsCompositeDevice.cpp
    ExGsCompositeView.cpp
    ExGsCompositeModuleFactory.cpp
    ExWinGsCompositeDevice.cpp
    ExWinGsCompositeView.cpp
    WinCompositeModule.cpp
    ../CommonDeviceProps.h
    ../win/ExGsWin32Device.h
    ../win/ExGsBitmapDevice.h
    ../win/ExGsScreenDevice.h
    ExGsCompositeDefs.h
    ExGsCompositeDevice.h
    ExGsCompositeView.h
    ExGsCompositeModuleFactory.h
    ExGsCompositeModuleInterface.h
    ExGsCompositeModuleProperty.h
    ExWinGsCompositeDefs.h
    ExWinGsCompositeDevice.h
    ExWinGsCompositeView.h
    ExWinCompositeBitBltOp.h
)

if(ODA_SHARED AND MSVC)
tkernel_sources(${TD_TXV_COMPOSITE_LIB}
    WinComposite.rc
)
endif(ODA_SHARED AND MSVC)

include_directories(
                    ../win
)

tkernel_txv(${TD_TXV_COMPOSITE_LIB} ${TD_GS_LIB} ${TD_GI_LIB} ${TD_GE_LIB} ${TD_ROOT_LIB} ${TD_ALLOC_LIB})

tkernel_project_group(${TD_TXV_COMPOSITE_LIB} "Extensions")

endif(MSVC AND NOT WINCE AND NOT WINRT OR BORLAND)

remove_definitions()
