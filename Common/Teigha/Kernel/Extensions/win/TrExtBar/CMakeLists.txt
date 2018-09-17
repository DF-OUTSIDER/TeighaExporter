#
#  TrExtBar library
#

# ExtBar sources

set(extbar_include_files
    ${TKERNEL_ROOT}/Include/Tr/extbar/ExtBar.h
)

set(extbar_source_include_files
    ExtBarImpl.h
)

set(extbar_source_files
    ExtBar.cpp
)

# Library sources

tkernel_sources(${TR_EXTBAR_LIB}
    ${extbar_include_files}
    ${extbar_source_include_files}
    ${extbar_source_files}
)

include_directories(
    ${TKERNEL_ROOT}/Include/Tr/extbar
)

tkernel_library(${TR_EXTBAR_LIB})

tkernel_project_group(${TR_EXTBAR_LIB} "Extensions")

remove_definitions()
