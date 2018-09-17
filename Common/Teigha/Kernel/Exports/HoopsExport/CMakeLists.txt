cmake_policy(SET CMP0015 OLD)

set (td_hoops_export_source
  Source/HoopsExportDevice.cpp
  Source/HoopsExport.cpp
  Source/HoopsExportImpl.cpp
  Source/HoopsDc.cpp
  Source/HoopsCommon.cpp
  Source/HoopsExportView.cpp
)
SOURCE_GROUP ("Source Files" FILES ${td_hoops_export_source})

set (td_hoops_export_include
  Include/HoopsCommon.h
  Include/HoopsStringsDef.h
  Include/HoopsExportImpl.h
  Include/HoopsResultEx.h
  Include/HoopsExportDevice.h
  Include/HoopsExport.h
  Include/HoopsExportParams.h
  Source/resource.h
  Include/HoopsExportCommon.h
  Include/HoopsResultExCodes.h
  Include/HoopsExportDef.h
  Include/HoopsExportView.h
)
SOURCE_GROUP ("Header Files" FILES ${td_hoops_export_include})

if(ODA_SHARED AND MSVC)
set (td_hoops_export_resources
  Source/HoopsExport.rc
)
SOURCE_GROUP ("Resource Files" FILES ${td_hoops_export_resources})
endif(ODA_SHARED AND MSVC)

tkernel_sources(${TD_HOOPS_EXPORT_LIB}
		  ${td_hoops_export_source}
		  ${td_hoops_export_include}
		  ${td_hoops_export_resources}
           )

if (MSVC)
include_directories(
                Include
                ${TG_ROOT}/Include
                $ENV{HOOPS_INSTALL_DIR}/Dev_Tools
                $ENV{HOOPS_INSTALL_DIR}/Dev_Tools/hoops_3dgs/source
                $ENV{HOOPS_INSTALL_DIR}/Dev_Tools/hoops_stream/source
				$ENV{HOOPS_INSTALL_DIR}/Dev_Tools/base_stream/source/stream_common
                $ENV{HOOPS_INSTALL_DIR}/Dev_Tools/utility/include
		   )

link_directories(
                $ENV{HOOPS_INSTALL_DIR}/Dev_Tools/hoops_3dgs/lib/nt_i386_vc11
                $ENV{HOOPS_INSTALL_DIR}/Dev_Tools/hoops_stream/lib/nt_i386_vc11
                $ENV{HOOPS_INSTALL_DIR}/Dev_Tools/utility/lib/nt_i386_vc11
				$ENV{HOOPS_INSTALL_DIR}/Dev_Tools/base_stream/lib/nt_i386_vc11
                )
else(MSVC)

include_directories(
                Include
                ${TG_ROOT}/Include
                $ENV{HOOPS_INSTALL_DIR}/Dev_Tools
                $ENV{HOOPS_INSTALL_DIR}/Dev_Tools/hoops_3dgs/source
                $ENV{HOOPS_INSTALL_DIR}/Dev_Tools/hoops_stream/source
                $ENV{HOOPS_INSTALL_DIR}/Dev_Tools/utility/source
                $ENV{HOOPS_INSTALL_DIR}/Dev_Tools/base_stream/source/stream_common
		   )
endif(MSVC)

tkernel_tx(${TD_HOOPS_EXPORT_LIB} ${TD_GS_LIB} ${TD_GI_LIB} ${TD_GE_LIB} ${TD_ROOT_LIB} ${TD_DBROOT_LIB} ${TD_ALLOC_LIB}
 ${TH_HOOPS_LIB} )


tkernel_project_group(${TD_HOOPS_EXPORT_LIB} "Hoops")


