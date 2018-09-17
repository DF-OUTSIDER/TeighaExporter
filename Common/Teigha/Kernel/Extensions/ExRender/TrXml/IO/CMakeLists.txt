#
#  TrXmlIO library
#

set(tr_xml_include_files
    ${TKERNEL_ROOT}/Extensions/ExRender/TrXml/IO/UnivReadFiler.h
    ${TKERNEL_ROOT}/Extensions/ExRender/TrXml/IO/BinaryGlesParser.h
    ${TKERNEL_ROOT}/Extensions/ExRender/TrXml/IO/XmlGlesParser.h
    ${TKERNEL_ROOT}/Extensions/ExRender/TrXml/IO/BaseGlesParser.h
    ${TKERNEL_ROOT}/Extensions/ExRender/TrXml/IO/XmlGlesLoader.h
    ${TKERNEL_ROOT}/Extensions/ExRender/TrXml/IO/UnivWriteFiler.h
    ${TKERNEL_ROOT}/Extensions/ExRender/TrXml/IO/GlesBinaryServerImpl.h
    ${TKERNEL_ROOT}/Extensions/ExRender/TrXml/IO/GlesXmlServerImpl.h
    ${TKERNEL_ROOT}/Extensions/ExRender/TrXml/IO/GlesServerBaseImpl.h
    ${TKERNEL_ROOT}/Extensions/ExRender/TrXml/XmlServerBaseImpl.h
    ${TKERNEL_ROOT}/Extensions/ExRender/TrXml/IO/OdFakeDbStub.h
    ${TKERNEL_ROOT}/Extensions/ExRender/TrXml/IO/PathEntryDefs.h
    ${TKERNEL_ROOT}/Extensions/ExRender/TrXml/IO/CloudTools.h
    )
    
set(tr_xml_src_files
    ${TKERNEL_ROOT}/Extensions/ExRender/TrXml/IO/UnivReadFiler.cpp
    ${TKERNEL_ROOT}/Extensions/ExRender/TrXml/IO/BinaryGlesParser.cpp
    ${TKERNEL_ROOT}/Extensions/ExRender/TrXml/IO/XmlGlesParser.cpp
    ${TKERNEL_ROOT}/Extensions/ExRender/TrXml/IO/XmlGlesLoader.cpp
    ${TKERNEL_ROOT}/Extensions/ExRender/TrXml/IO/UnivWriteFiler.cpp
    ${TKERNEL_ROOT}/Extensions/ExRender/TrXml/IO/GlesBinaryServerImpl.cpp
    ${TKERNEL_ROOT}/Extensions/ExRender/TrXml/IO/GlesXmlServerImpl.cpp
    ${TKERNEL_ROOT}/Extensions/ExRender/TrXml/IO/GlesServerBaseImpl.cpp
    )

# Library sources

tkernel_sources(${TR_XML_IO_LIB}
    ${tr_xml_include_files}
    ${tr_xml_src_files}
)

include_directories(
    ${TKERNEL_ROOT}/Extensions/ExRender/TrXml
    ${TKERNEL_ROOT}/Extensions/ExRender/TrXml/IO
    ${TKERNEL_ROOT}/Include/Tr
    ${TKERNEL_ROOT}/Include/Tr/vec
    ${TKERNEL_ROOT}/Extensions/ExRender
    ${TKERNEL_ROOT}/ThirdParty/tinyxml
    ${TKERNEL_ROOT}/Extensions/ExServices
)

tkernel_library(${TR_XML_IO_LIB}
       ${TR_VEC_LIB}
       ${TR_BASE_LIB}
       ${TD_ROOT_LIB}
       ${TD_ALLOC_LIB}
       ${TH_TINYXML_LIB})

tkernel_project_group(${TR_XML_IO_LIB} "Extensions")

remove_definitions()