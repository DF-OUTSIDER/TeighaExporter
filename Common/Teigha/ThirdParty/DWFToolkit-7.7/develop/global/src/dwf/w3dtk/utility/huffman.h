/*
* Copyright (c) 1998 by Tech Soft 3D, LLC.
* The information contained herein is confidential and proprietary to
* Tech Soft 3D, LLC., and considered a trade secret as defined under
* civil and criminal statutes.	 Tech Soft 3D shall pursue its civil
* and criminal remedies in the event of unauthorized use or misappropriation
* of its trade secrets.  Use of this information by anyone other than
* authorized employees of Tech Soft 3D, LLC. is granted only under a
* written non-disclosure agreement, expressly prescribing the scope and
* manner of such use.
*
* $Id: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/w3dtk/utility/huffman.h#1 $
*/


#ifdef HOOPS_DEFINED
#define VList HVList
#define VHash HVHash
#endif


struct huffman_encoder_8_t;
struct huffman_encoder_16_t;

huffman_encoder_8_t * new_huffman_encoder_8();

huffman_encoder_16_t * new_huffman_encoder_16();

void delete_huffman_encoder_8(huffman_encoder_8_t *he);

void delete_huffman_encoder_16(huffman_encoder_16_t *he);

void huffman_add_data_8(huffman_encoder_8_t *he, int in_n, const char *in_data);

void huffman_add_data_16(huffman_encoder_16_t *he, int in_n, const unsigned short *in_data);

void huffman_create_tables_8(huffman_encoder_8_t *he);


void huffman_create_tables_16(huffman_encoder_16_t *he);

bool huffman_encode_8(huffman_encoder_8_t *he,
					int in_n, const char *in_data,
					int *out_data_bits, char *out_data);


bool huffman_encode_16(huffman_encoder_16_t *he,
					int in_n,	const unsigned short *in_data,
					int *out_data_bits, unsigned short *out_data);

void huffman_decode_8(huffman_encoder_8_t *he, int in_data_bits, const char *in_data, int *out_n, char *out_data); 


void huffman_decode_16(huffman_encoder_16_t *he, int in_data_bits, const unsigned short *in_data, int *out_n, unsigned short *out_data); 







