#
#  ZeroMQ based IO implementations
#

tdrawing_sources( ZeroMQ
    ZmqSocket.cpp
    ZmqSocketFs.cpp
    ZmqSocket.h
    ZmqSocketFs.h
    ZmqSocketLib.h
    ZmqSocketLib.cpp
    ZmqBaseIO.cpp
    ZmqBaseIO.h

)

if(ODA_SHARED AND MSVC)
  tdrawing_sources(ZeroMQ module.rc)

endif(ODA_SHARED AND MSVC)


include_directories(${ODA_SOURCE_ROOT}
                    ${TKERNEL_ROOT}/Include
                    ${TDRAWING_ROOT}/Include/Gfx
                    ${TKERNEL_ROOT}/Source/DbRoot
                    ${TKERNEL_ROOT}/Extensions/RxFs
                    ${TKERNEL_ROOT}/Extensions/ExServices
                    ${TKERNEL_ROOT}/Examples/Common
                    ${TH_ROOT}/zmq/include/
                    ${TKERNEL_ROOT}/Extensions/TxHost
                    ${TKERNEL_ROOT}/Extensions/ExRender/GfxSrvc/
)


tdrawing_tx(
  ZeroMQ
  ${TD_EXLIB}
  RxFS
  ${TD_ROOT_LIB}
  ${TD_ALLOC_LIB}
  ${TH_ZMQ_LIB}
)

tdrawing_project_group(ZeroMQ "Extensions")
