

#ifndef VCONF_H
#define VCONF_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "vhash.h"
#include "vlist.h"

#if defined(SOLARIS_SYSTEM) && defined(__cplusplus)
#define VCONF_API extern "C"
#else
#define VCONF_API
#endif

#define VCONF_FILE_STYLE_FLAT		0
#define VCONF_FILE_STYLE_CHUNKED 	1

typedef struct vconf_s {
	const char * filename;
	vhash_t * options_hash;
	vlist_t * options_list;
	char delimiter;
	int style;
	int chunk;
	void *(VHASH_CDECL *malloc) (size_t);
	void (VHASH_CDECL *free) (void*);
} vconf_t;


VCONF_API vconf_t* new_vconf(
	void *(VHASH_CDECL *vconf_malloc) (size_t),
	void (VHASH_CDECL *vconf_free) (void *));

VCONF_API void delete_vconf(
	vconf_t* vconf); 

VCONF_API void vconf_clear_options(
	vconf_t* vconf);

VCONF_API void vconf_set_delimiter(
	vconf_t* vconf, 
	char delimiter);

VCONF_API int vconf_set_file_style(
	vconf_t* vconf, 
	int style);

VCONF_API unsigned int vconf_chunk_count(
	vconf_t* vconf);

VCONF_API int vconf_set_chunk(
	vconf_t* vconf, 
	unsigned int chunk);

VCONF_API int vconf_read_file(
	vconf_t* vconf, 
	const char * filename);

VCONF_API int vconf_write_file(
	vconf_t* vconf, 
	const char * filename);

VCONF_API const char * vconf_get_filename(
	vconf_t* vconf);

VCONF_API vhash_t * vconf_get_option_hash(
	vconf_t* vconf);

VCONF_API const char * vconf_get_option(
	vconf_t* vconf, 
	const char * option);

VCONF_API void vconf_set_option(
	vconf_t* vconf, 
	const char * option,
	const char * value);


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /*VCONF_H*/


