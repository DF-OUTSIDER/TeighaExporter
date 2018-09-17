#
# OpenJPEG library
#

th_sources(${TH_LIBOPENJPEG20_LIB}
           bio.c
           cio.c
           dwt.c
           event.c
           function_list.c
           image.c
           invert.c
           j2k.c
           jp2.c
           mct.c
           mqc.c
           openjpeg.c
           opj_clock.c
           pi.c
           sparse_array.c
           sparse_array.h
           t1.c
           t2.c
           tcd.c
           tgt.c
           thread.c
	)
	
if(WINRT)
  add_definitions(-DSKIP_OS_SECURE_RANDOM -DNO_GET_ENVIRONMENT)
endif()

add_definitions (-DOPJ_STATIC)

th_library(${TH_LIBOPENJPEG20_LIB} ${TD_ALLOC_LIB})

th_project_group(${TH_LIBOPENJPEG20_LIB} "PDFium")
