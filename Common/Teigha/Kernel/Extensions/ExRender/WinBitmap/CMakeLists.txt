#
#  WinBitmap library
#

tkernel_sources(${TD_TXV_BITMAP_LIB}
    BitmapModule.cpp
    GsBitmapVectorizer.cpp
    GsBitmapVectorizer.h
    ../CommonDeviceProps.cpp
    ../CommonDeviceProps.h
)

if((MSVC OR BORLAND OR WINDOWS_GCC) AND NOT WINRT)
tkernel_sources(${TD_TXV_BITMAP_LIB}
    ../win/ExGsWin32Device.cpp
    ../win/ExGsWin32Device.h
    ../win/ExGsScreenDevice.cpp
    ../win/ExGsScreenDevice.h
)
endif((MSVC OR BORLAND OR WINDOWS_GCC) AND NOT WINRT)

if(ODA_SHARED AND MSVC)
tkernel_sources(${TD_TXV_BITMAP_LIB}
    WinBitmap.rc
)
endif(ODA_SHARED AND MSVC)

include_directories(
                    ../win
                    ../)

tkernel_txv(${TD_TXV_BITMAP_LIB} ${TD_GI_LIB} ${TD_GS_LIB} ${TD_GE_LIB} ${TD_ROOT_LIB} ${TD_ALLOC_LIB})

tkernel_project_group(${TD_TXV_BITMAP_LIB} "Extensions")

remove_definitions()
