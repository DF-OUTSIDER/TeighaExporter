#
# Jpeg library
#

th_sources(${TH_THIRDPARTYRASTER_LIB}
    jfdctfst.c
    jerror.c
    jdmerge.c
    jdpostct.c
    jcsample.c
    jdtrans.c
    jddctmgr.c
    jchuff.c
    jdhuff.c
    jmemnobs.c
    jquant2.c
    jcmaster.c
    jdmainct.c
    jidctint.c
    jmemmgr.c
    jcmainct.c
    jquant1.c
    jcapistd.c
    jdatadst.c
    jfdctflt.c
    jfdctint.c
    jidctfst.c
    jidctflt.c
    jcinit.c
    jdapistd.c
    jdcoefct.c
    jdatasrc.c
    jccoefct.c
    jdsample.c
    jdapimin.c
    jidctred.c
    jcdctmgr.c
    jcprepct.c
    jutils.c
    jctrans.c
    jdmarker.c
    jdphuff.c
    jcphuff.c
    jcomapi.c
    jdcolor.c
    jcmarker.c
    jcapimin.c
    jcparam.c
    jdmaster.c
    jdinput.c
    jccolor.c
    jmorecfg.h
    jchuff.h
    jmemsys.h
    jerror.h
    jpegint.h
    jdhuff.h
    jversion.h
    jdct.h
    jinclude.h
    jpeglib.h
    jconfig.h
	)
			  
if(MSVC)
add_definitions( "/wd4996 /wd4127 /wd4100 /wd4244 /wd4267" )
endif(MSVC)

include_directories(${TH_ROOT}/Jpeg-6b)
				  
th_library(${TH_THIRDPARTYRASTER_LIB})

th_project_group(${TH_THIRDPARTYRASTER_LIB})
