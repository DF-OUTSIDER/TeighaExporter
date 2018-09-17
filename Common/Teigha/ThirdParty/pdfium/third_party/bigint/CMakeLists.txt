#
# Big Integer Library
#

th_sources(${TH_BIGINT_LIB}
           BigInteger.cc
           BigInteger.hh
           BigIntegerLibrary.hh
           BigIntegerUtils.cc
           BigIntegerUtils.hh
           BigUnsigned.cc
           BigUnsigned.hh
           BigUnsignedInABase.cc
           BigUnsignedInABase.hh
           NumberlikeArray.hh
	)
	
if(WINRT)
  add_definitions(-DSKIP_OS_SECURE_RANDOM -DNO_GET_ENVIRONMENT)
endif()

th_library(${TH_BIGINT_LIB} ${TD_ALLOC_LIB})

th_project_group(${TH_BIGINT_LIB} "PDFium")
