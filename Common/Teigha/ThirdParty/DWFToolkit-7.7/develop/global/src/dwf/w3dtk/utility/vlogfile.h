

#ifndef VLOGFILE_H
#define VLOGFILE_H

#include <stdlib.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#if defined(SOLARIS_SYSTEM) && defined(__cplusplus)
#define VLOG_API extern "C"
#else
#define VLOG_API
#endif

typedef struct {
	const char * name;
	unsigned long n_lines;
	unsigned long n_chars;
	FILE * lines;
	FILE * dir;
	void *(*malloc) (size_t);
	void (*free) (void*);
} vlogfile_t;


VLOG_API vlogfile_t* new_vlogfile(
	const char * log_file_name,
	void *(*vlogfile_malloc) (size_t),
	void (*vlogfile_free) (void *));

VLOG_API void delete_vlogfile( 
	vlogfile_t* vlogfile); 

VLOG_API void vlogfile_add_line( 
	vlogfile_t* vlogfile, 
	const char * line,
	size_t line_length);

VLOG_API const char * vlogfile_get_line(
	vlogfile_t* vlogfile,
	unsigned long line_number,
	unsigned long * line_length);

VLOG_API void vlogfile_free_line(
	vlogfile_t* vlogfile,
	const char * line);

VLOG_API void vlogfile_clear_log(
	vlogfile_t* v);

VLOG_API FILE * vlogfile_open_snapshot(
	vlogfile_t* v,
	const char * filename);

VLOG_API void vlogfile_dump_snapshot(
	vlogfile_t* v,
	FILE * snapshot_file);

VLOG_API void vlogfile_close_snapshot(
	vlogfile_t* v,
	FILE * snapshot_file);


/*DEBUG LOG*/

extern FILE * debug_log_file_handle;
extern char * debug_log_current_time;

#define FPRINTF_HEX_BUFFER(file_handle,data,length) { \
	unsigned int iiiii; \
	for(iiiii=0;iiiii<(length);iiiii++) { \
		const char * HEX = "0123456789ABCDEF"; \
		char t[16]; \
		unsigned char c = (data)[iiiii]; \
		sprintf(t,"<%c%c>", HEX[(((unsigned char)c)>>4)], HEX[(((unsigned char)c)&0x000F)]); \
		fprintf(file_handle, "%s", t); \
		fflush(file_handle); \
	} \
}

#if defined(DEBUG_PRINTS)

#define DEBUG_LOG_PRINT_HEX_BUFFER(data,length) { \
	unsigned int iiiii; \
	for(iiiii=0;iiiii<(length);iiiii++) { \
		const char * HEX = "0123456789ABCDEF"; \
		char t[16]; \
		unsigned char c = (data)[iiiii]; \
		if(c==0x0D || c==0x0A){ \
			sprintf(t,"%c", c); \
		}else \
		if(c<=0x1F){ \
			sprintf(t,"<%c%c>", HEX[(((unsigned char)c)>>4)], HEX[(((unsigned char)c)&0x000F)]); \
		}else \
		if(c>=0x7F){ \
			sprintf(t,"<%c%c>", HEX[(((unsigned char)c)>>4)], HEX[(((unsigned char)c)&0x000F)]); \
		}else \
			sprintf(t,"%c", c); \
		DEBUG_LOG_PRINT_NOTIME(t); \
	} \
}

#define DEBUG_LOG_PRINT_1(item) do { \
	if(debug_log_file_handle != NULL) { \
		fprintf(debug_log_file_handle, "%s ", debug_log_current_time); \
		fprintf(debug_log_file_handle, (item)); \
		fflush(debug_log_file_handle); \
	} \
} while (0)

#define DEBUG_LOG_PRINT_2(format, item) do { \
	if(debug_log_file_handle != NULL) { \
		fprintf(debug_log_file_handle, "%s ", debug_log_current_time); \
		fprintf(debug_log_file_handle, (format), (item)); \
		fflush(debug_log_file_handle); \
	} \
} while (0)

#define DEBUG_LOG_PRINT_3(format, item1, item2) do { \
	if(debug_log_file_handle != NULL) { \
		fprintf(debug_log_file_handle, "%s ", debug_log_current_time); \
		fprintf(debug_log_file_handle, (format), (item1), (item2)); \
		fflush(debug_log_file_handle); \
	} \
} while (0)

#define DEBUG_LOG_PRINT_4(format, item1, item2, item3) do { \
	if(debug_log_file_handle != NULL) { \
		fprintf(debug_log_file_handle, "%s ", debug_log_current_time); \
		fprintf(debug_log_file_handle, (format), (item1), (item2), (item3)); \
		fflush(debug_log_file_handle); \
	} \
} while (0)

#define DEBUG_LOG_PRINT_5(format, item1, item2, item3, item4) do { \
	if(debug_log_file_handle != NULL) { \
		fprintf(debug_log_file_handle, "%s ", debug_log_current_time); \
		fprintf(debug_log_file_handle, (format), (item1), (item2), (item3), (item4)); \
		fflush(debug_log_file_handle); \
	} \
} while (0)

#define DEBUG_LOG_PRINT_6(format, item1, item2, item3, item4, item5) do { \
	if(debug_log_file_handle != NULL) { \
		fprintf(debug_log_file_handle, "%s ", debug_log_current_time); \
		fprintf(debug_log_file_handle, (format), (item1), (item2), (item3), (item4), (item5)); \
		fflush(debug_log_file_handle); \
	} \
} while (0)

#define DEBUG_LOG_PRINT_TODO(item) do { \
	if(debug_log_file_handle != NULL) { \
		fprintf(debug_log_file_handle, "%s ", debug_log_current_time); \
		fprintf(debug_log_file_handle, "TODO %s %d : %s\n", __FILE__, __LINE__, (item)); \
		fflush(debug_log_file_handle); \
	} \
} while (0)

#define DEBUG_LOG_PRINT_HCS_BUFFER(message, prefix) do { \
	if(debug_log_file_handle != NULL) { \
		fprintf(debug_log_file_handle, "%s %s", debug_log_current_time, prefix); \
		hcs_apro_fprint_hcs_buffer(debug_log_file_handle, message); \
		fflush(debug_log_file_handle); \
	} \
} while (0)

#define DEBUG_LOG_PRINT_NOTIME(item) do { \
	if(debug_log_file_handle != NULL) { \
		fprintf(debug_log_file_handle, "%s",(item)); \
		fflush(debug_log_file_handle); \
	} \
} while (0)

#define UPDATE_DEBUG_LOG_TIME() do { \
	time_t tmp_time; \
	time(&tmp_time); \
	strcpy(debug_log_current_time,ctime((const time_t*)&tmp_time)); \
	debug_log_current_time[strlen(debug_log_current_time)-strlen("\n")] = '\0'; \
} while (0)

#else 

#define DEBUG_LOG_PRINT_HEX_BUFFER(data,length) do { (void)(data); (void)(length); } while (0);
#define DEBUG_LOG_PRINT_1(item) do { ((void)(item)); } while (0)
#define DEBUG_LOG_PRINT_2(format, item) do { ((void)(format)); ((void)(item));} while (0)
#define DEBUG_LOG_PRINT_3(format, item1, item2) do { ((void)(format)); ((void)(item1)); ((void)(item2));} while (0)
#define DEBUG_LOG_PRINT_4(format, item1, item2, item3) do { ((void)(format)); ((void)(item1)); ((void)(item2)); ((void)(item3));} while (0)
#define DEBUG_LOG_PRINT_5(format, item1, item2, item3, item4) do { ((void)(format)); ((void)(item1)); ((void)(item2)); ((void)(item3)); ((void)(item4)); } while (0)
#define DEBUG_LOG_PRINT_6(format, item1, item2, item3, item4, item5) do { ((void)(format)); ((void)(item1)); ((void)(item2)); ((void)(item3)); ((void)(item4)); ((void)(item5)); } while (0)
#define DEBUG_LOG_PRINT_TODO(item) do { void item } while (0)
#define DEBUG_LOG_PRINT_HCS_BUFFER(item, prefix) do { ((void)(item));((void)(prefix));} while (0)
#define DEBUG_LOG_PRINT_NOTIME(item) do { ((void)(item)); } while (0)

#define UPDATE_DEBUG_LOG_TIME() do { ; } while (0);

#endif



#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /*VLOGFILE_H*/


