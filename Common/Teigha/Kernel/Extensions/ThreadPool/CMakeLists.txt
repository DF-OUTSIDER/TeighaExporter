#
#  ThreadPool library
#

if(NOT WINCE)

tkernel_sources(${TD_THREADPOOL_LIB}
	ThreadPoolModule.cpp
	../../Include/RxThreadPoolService.h
	)

if(ODA_SHARED AND MSVC)
tkernel_sources(${TD_THREADPOOL_LIB}
        ThreadPool.rc
	)
endif(ODA_SHARED AND MSVC)

include_directories()

tkernel_tx(${TD_THREADPOOL_LIB} ${TD_ROOT_LIB} ${TD_ALLOC_LIB})

tkernel_project_group(${TD_THREADPOOL_LIB} "Extensions")

else(NOT WINCE)
set(TD_THREADPOOL_LIB)
endif(NOT WINCE)
