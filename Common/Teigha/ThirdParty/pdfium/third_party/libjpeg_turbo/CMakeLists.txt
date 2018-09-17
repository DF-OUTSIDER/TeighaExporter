#
# Setup
#

cmake_minimum_required(VERSION 2.8.8)
# Use LINK_INTERFACE_LIBRARIES instead of INTERFACE_LINK_LIBRARIES
if(POLICY CMP0022)
  cmake_policy(SET CMP0022 OLD)
endif()

project(libjpeg-turbo C)
set(VERSION 1.4.90)

if(CYGWIN OR NOT CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
  execute_process(COMMAND "date" "+%Y%m%d" OUTPUT_VARIABLE BUILD)
  string(REGEX REPLACE "\n" "" BUILD ${BUILD})
elseif(WIN32)
  execute_process(COMMAND "wmic.exe" "os" "get" "LocalDateTime" OUTPUT_VARIABLE
    BUILD)
  string(REGEX REPLACE "[^0-9]" "" BUILD "${BUILD}")
  if (BUILD STREQUAL "")
    execute_process(COMMAND "cmd.exe" "/C" "DATE" "/T" OUTPUT_VARIABLE BUILD)
    string(REGEX REPLACE ".*[ ]([0-9]*)[/.]([0-9]*)[/.]([0-9]*).*" "\\3\\2\\1" BUILD "${BUILD}")
  else()
    string(SUBSTRING "${BUILD}" 0 8 BUILD)
  endif()
else()
  message(FATAL_ERROR "Platform not supported by this build system.  Use autotools instead.")
endif()

# This does nothing except when using MinGW.  CMAKE_BUILD_TYPE has no meaning
# in Visual Studio, and it always defaults to Debug when using NMake.
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

message(STATUS "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")

# This only works if building from the command line.  There is currently no way
# to set a variable's value based on the build type when using Visual Studio.
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(BUILD "${BUILD}d")
endif()

message(STATUS "VERSION = ${VERSION}, BUILD = ${BUILD}")

option(WITH_SIMD "Include SIMD extensions" TRUE)
option(WITH_ARITH_ENC "Include arithmetic encoding support" TRUE)
option(WITH_ARITH_DEC "Include arithmetic decoding support" TRUE)
option(WITH_JPEG7 "Emulate libjpeg v7 API/ABI (this makes libjpeg-turbo backward incompatible with libjpeg v6b)" FALSE)
option(WITH_JPEG8 "Emulate libjpeg v8 API/ABI (this makes libjpeg-turbo backward incompatible with libjpeg v6b)" FALSE)
option(WITH_MEM_SRCDST "Include in-memory source/destination manager functions when emulating the libjpeg v6b or v7 API/ABI" TRUE)
option(WITH_TURBOJPEG "Include the TurboJPEG wrapper library and associated test programs" TRUE)
option(WITH_JAVA "Build Java wrapper for the TurboJPEG library" FALSE)
option(WITH_12BIT "Encode/decode JPEG images with 12-bit samples (implies WITH_SIMD=0 WITH_TURBOJPEG=0 WITH_ARITH_ENC=0 WITH_ARITH_DEC=0)" FALSE)
option(ENABLE_STATIC "Build static libraries" TRUE)
#option(ENABLE_SHARED "Build shared libraries" TRUE)

if(WITH_12BIT)
  set(WITH_SIMD FALSE)
  set(WITH_TURBOJPEG FALSE)
  set(WITH_JAVA FALSE)
  set(WITH_ARITH_ENC FALSE)
  set(WITH_ARITH_DEC FALSE)
  set(BITS_IN_JSAMPLE 12)
  message(STATUS "12-bit JPEG support enabled")
else()
  set(BITS_IN_JSAMPLE 8)
endif()

if(WITH_ARITH_ENC)
  set(C_ARITH_CODING_SUPPORTED 1)
  message(STATUS "Arithmetic encoding support enabled")
else()
  message(STATUS "Arithmetic encoding support disabled")
endif()

if(WITH_ARITH_DEC)
  set(D_ARITH_CODING_SUPPORTED 1)
  message(STATUS "Arithmetic decoding support enabled")
else()
  message(STATUS "Arithmetic decoding support disabled")
endif()

if(WITH_TURBOJPEG)
  message(STATUS "TurboJPEG C wrapper enabled")
else()
  message(STATUS "TurboJPEG C wrapper disabled")
endif()

if(WITH_JAVA)
  message(STATUS "TurboJPEG Java wrapper enabled")
else()
  message(STATUS "TurboJPEG Java wrapper disabled")
endif()

set(SO_AGE 0)
if(WITH_MEM_SRCDST)
  set(SO_AGE 1)
endif()

set(JPEG_LIB_VERSION 62)
set(DLL_VERSION ${JPEG_LIB_VERSION})
set(FULLVERSION ${DLL_VERSION}.${SO_AGE}.0)
if(WITH_JPEG8)
  set(JPEG_LIB_VERSION 80)
  set(DLL_VERSION 8)
  set(FULLVERSION ${DLL_VERSION}.0.2)
  message(STATUS "Emulating libjpeg v8 API/ABI")
elseif(WITH_JPEG7)
  set(JPEG_LIB_VERSION 70)
  set(DLL_VERSION 7)
  set(FULLVERSION ${DLL_VERSION}.${SO_AGE}.0)
  message(STATUS "Emulating libjpeg v7 API/ABI")
endif(WITH_JPEG8)

if(WITH_MEM_SRCDST)
  set(MEM_SRCDST_SUPPORTED 1)
  message(STATUS "In-memory source/destination managers enabled")
else()
  message(STATUS "In-memory source/destination managers disabled")
endif()

if(MSVC)
  option(WITH_CRT_DLL
    "Link all libjpeg-turbo libraries and executables with the C run-time DLL (msvcr*.dll) instead of the static C run-time library (libcmt*.lib.)  The default is to use the C run-time DLL only with the libraries and executables that need it."
    FALSE)
  if(NOT WITH_CRT_DLL)
    # Use the static C library for all build types
    foreach(var CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
      CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)
      if(${var} MATCHES "/MD")
        string(REGEX REPLACE "/MD" "/MT" ${var} "${${var}}")
      endif()
    endforeach()
  endif()
  add_definitions(-W3 -wd4996)
endif()

# Detect whether compiler is 64-bit
if(MSVC AND CMAKE_CL_64)
  set(SIMD_X86_64 1)
  set(64BIT 1)
elseif(CMAKE_SIZEOF_VOID_P MATCHES 8)
  set(SIMD_X86_64 1)
  set(64BIT 1)
endif()

if(64BIT)
  message(STATUS "64-bit build")
else()
  message(STATUS "32-bit build")
endif()

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  if(MSVC)
    set(CMAKE_INSTALL_PREFIX_DEFAULT ${CMAKE_PROJECT_NAME})
  else()
    set(CMAKE_INSTALL_PREFIX_DEFAULT ${CMAKE_PROJECT_NAME}-gcc)
  endif()
  if(64BIT)
    set(CMAKE_INSTALL_PREFIX_DEFAULT ${CMAKE_INSTALL_PREFIX_DEFAULT}64)
  endif()
  set(CMAKE_INSTALL_PREFIX "c:/${CMAKE_INSTALL_PREFIX_DEFAULT}" CACHE PATH
    "Directory into which to install libjpeg-turbo (default: c:/${CMAKE_INSTALL_PREFIX_DEFAULT})"
    FORCE)
endif()

message(STATUS "Install directory = ${CMAKE_INSTALL_PREFIX}")

configure_file(jconfig.h.in jconfig.h)
configure_file(jconfigint.h.in jconfigint.h)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR})

string(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_UC)

set(EFFECTIVE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_${CMAKE_BUILD_TYPE_UC}}")
message(STATUS "Compiler flags = ${EFFECTIVE_C_FLAGS}")

set(EFFECTIVE_LD_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CMAKE_EXE_LINKER_FLAGS_${CMAKE_BUILD_TYPE_UC}}")
message(STATUS "Linker flags = ${EFFECTIVE_LD_FLAGS}")

if(WITH_JAVA)
  find_package(Java)
  find_package(JNI)
  if(DEFINED JAVACFLAGS)
    message(STATUS "Java compiler flags = ${JAVACFLAGS}")
  endif()
endif()


#
# Targets
#

set(JPEG_SOURCES jcapimin.c jcapistd.c jccoefct.c jccolor.c jcdctmgr.c jchuff.c
  jcinit.c jcmainct.c jcmarker.c jcmaster.c jcomapi.c jcparam.c jcphuff.c
  jcprepct.c jcsample.c jctrans.c jdapimin.c jdapistd.c jdatadst.c jdatasrc.c
  jdcoefct.c jdcolor.c jddctmgr.c jdhuff.c jdinput.c jdmainct.c jdmarker.c
  jdmaster.c jdmerge.c jdphuff.c jdpostct.c jdsample.c jdtrans.c jerror.c
  jfdctflt.c jfdctfst.c jfdctint.c jidctflt.c jidctfst.c jidctint.c jidctred.c
  jquant1.c jquant2.c jutils.c jmemmgr.c jmemnobs.c)

if(WITH_ARITH_ENC OR WITH_ARITH_DEC)
  set(JPEG_SOURCES ${JPEG_SOURCES} jaricom.c)
endif()

if(WITH_ARITH_ENC)
  set(JPEG_SOURCES ${JPEG_SOURCES} jcarith.c)
endif()

if(WITH_ARITH_DEC)
  set(JPEG_SOURCES ${JPEG_SOURCES} jdarith.c)
endif()

if(WITH_SIMD)
  add_definitions(-DWITH_SIMD)
  add_subdirectory(simd)
  if(SIMD_X86_64)
    set(JPEG_SOURCES ${JPEG_SOURCES} simd/jsimd_x86_64.c)
  else()
    set(JPEG_SOURCES ${JPEG_SOURCES} simd/jsimd_i386.c)
  endif()
  # This tells CMake that the "source" files haven't been generated yet
  set_source_files_properties(${SIMD_OBJS} PROPERTIES GENERATED 1)
else()
  set(JPEG_SOURCES ${JPEG_SOURCES} jsimd_none.c)
  message(STATUS "Not using SIMD acceleration")
endif()

if(WITH_JAVA)
  add_subdirectory(java)
  set(ENABLE_SHARED TRUE)
endif()

if(ENABLE_SHARED)
  add_subdirectory(sharedlib)
endif()

if(ENABLE_STATIC OR WITH_TURBOJPEG)
  add_library(jpeg-static STATIC ${JPEG_SOURCES} ${SIMD_OBJS})
  if(NOT MSVC)
    set_target_properties(jpeg-static PROPERTIES OUTPUT_NAME jpeg)
  endif()
  if(WITH_SIMD)
    add_dependencies(jpeg-static simd)
  endif()
endif()

if(WITH_TURBOJPEG)
  set(TURBOJPEG_SOURCES turbojpeg.c transupp.c jdatadst-tj.c jdatasrc-tj.c)
  if(WITH_JAVA)
    set(TURBOJPEG_SOURCES ${TURBOJPEG_SOURCES} turbojpeg-jni.c)
    include_directories(${JAVA_INCLUDE_PATH} ${JAVA_INCLUDE_PATH2})
  endif()

  if(ENABLE_SHARED)
    add_library(turbojpeg SHARED ${TURBOJPEG_SOURCES})
    set_target_properties(turbojpeg PROPERTIES DEFINE_SYMBOL DLLDEFINE)
    if(MINGW)
      set_target_properties(turbojpeg PROPERTIES LINK_FLAGS -Wl,--kill-at)
    endif()
    target_link_libraries(turbojpeg jpeg-static)
    set_target_properties(turbojpeg PROPERTIES LINK_INTERFACE_LIBRARIES "")

    add_executable(tjunittest tjunittest.c tjutil.c)
    target_link_libraries(tjunittest turbojpeg)

    add_executable(tjbench tjbench.c bmp.c tjutil.c rdbmp.c rdppm.c wrbmp.c
      wrppm.c)
    target_link_libraries(tjbench turbojpeg jpeg-static)
    set_property(TARGET tjbench PROPERTY COMPILE_FLAGS
      "-DBMP_SUPPORTED -DPPM_SUPPORTED")
  endif()

  if(ENABLE_STATIC)
    add_library(turbojpeg-static STATIC ${JPEG_SOURCES} ${SIMD_OBJS}
      turbojpeg.c transupp.c jdatadst-tj.c jdatasrc-tj.c)
    if(NOT MSVC)
      set_target_properties(turbojpeg-static PROPERTIES OUTPUT_NAME turbojpeg)
    endif()
    if(WITH_SIMD)
      add_dependencies(turbojpeg-static simd)
    endif()

    add_executable(tjunittest-static tjunittest.c tjutil.c)
    target_link_libraries(tjunittest-static turbojpeg-static)

    add_executable(tjbench-static tjbench.c bmp.c tjutil.c rdbmp.c rdppm.c
      wrbmp.c wrppm.c)
    target_link_libraries(tjbench-static turbojpeg-static jpeg-static)
    set_property(TARGET tjbench-static PROPERTY COMPILE_FLAGS
      "-DBMP_SUPPORTED -DPPM_SUPPORTED")
  endif()
endif()

if(WITH_12BIT)
  set(COMPILE_FLAGS "-DGIF_SUPPORTED -DPPM_SUPPORTED -DUSE_SETMODE")
else()
  set(COMPILE_FLAGS "-DBMP_SUPPORTED -DGIF_SUPPORTED -DPPM_SUPPORTED -DTARGA_SUPPORTED -DUSE_SETMODE")
  set(CJPEG_BMP_SOURCES rdbmp.c rdtarga.c)
  set(DJPEG_BMP_SOURCES wrbmp.c wrtarga.c)
endif()

if(ENABLE_STATIC)
  add_executable(cjpeg-static cjpeg.c cdjpeg.c rdgif.c rdppm.c rdswitch.c
    ${CJPEG_BMP_SOURCES})
  set_property(TARGET cjpeg-static PROPERTY COMPILE_FLAGS ${COMPILE_FLAGS})
  target_link_libraries(cjpeg-static jpeg-static)

  add_executable(djpeg-static djpeg.c cdjpeg.c rdcolmap.c rdswitch.c wrgif.c
    wrppm.c ${DJPEG_BMP_SOURCES})
  set_property(TARGET djpeg-static PROPERTY COMPILE_FLAGS ${COMPILE_FLAGS})
  target_link_libraries(djpeg-static jpeg-static)

  add_executable(jpegtran-static jpegtran.c cdjpeg.c rdswitch.c transupp.c)
  target_link_libraries(jpegtran-static jpeg-static)
  set_property(TARGET jpegtran-static PROPERTY COMPILE_FLAGS "-DUSE_SETMODE")
endif()

add_executable(rdjpgcom rdjpgcom.c)

add_executable(wrjpgcom wrjpgcom.c)

#th_library(${TH_LIBJPEG_TURBO_LIB} ${TD_ALLOC_LIB})

#th_project_group(${TH_LIBJPEG_TURBO_LIB} "PDFium")


#
# Tests
#
#deleted
#############################################################################

#
# Installer
#
#deleted
#############################################################################
