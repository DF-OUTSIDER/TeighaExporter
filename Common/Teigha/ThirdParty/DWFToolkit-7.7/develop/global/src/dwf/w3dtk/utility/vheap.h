
#ifndef __VHEAP_INCLUDE__
#define __VHEAP_INCLUDE__

///
///\file        dwf/w3dtk/utility/vheap.h
///

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* 
 * HT_VHeap and HT_IHeap
 *
 * IHeap tracks integers.
 * VHeap tracks "void *"'s
 *
 * NOTES
 * a) IHeap uses its integers directly for a reverse lookup table.  Therefore:
 *  1) integers must be non-negative
 *  2) they must be confined to some reasonable range
 * b) IHeap is primarily intended to track array indices, which would satisfy
 *  both of the above requirements
 */ 

/* vheap depends on vhash */
#include "vhash.h"

typedef struct iheap_s {
    int used;
    int allocated;
    long *items;
    float *values;
    int *lookup;
} iheap_t;

typedef struct vheap_s {
    iheap_t *iheap;
    vhash_t *vhash;
    vhash_t *reverse_vhash;
    int next_index;
} vheap_t;

iheap_t *new_iheap(void);
void delete_iheap( iheap_t *heap );
void iheap_insert( iheap_t *heap, int item, float value );
void iheap_update( iheap_t *heap, int item, float value );
int iheap_kill( iheap_t *heap, int item );
int iheap_pop( iheap_t *heap, long *item, float *value );
int iheap_peek( iheap_t *heap, long *item, float *value );

vheap_t *new_vheap(void);
void delete_vheap( vheap_t *heap );
void vheap_insert( vheap_t *heap, void const *item, float value );
void vheap_update( vheap_t *heap, void const *item, float value );
int vheap_kill( vheap_t *heap, void const *item );
int vheap_pop( vheap_t *heap, void **item, float *value );
int vheap_peek( vheap_t *heap, void **item, float *value );


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */
#endif /* __VHEAP_INCLUDE__ */


