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
* $Id: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/w3dtk/utility/vbsp.h#1 $
*/

#ifndef VBSP_H
#define VBSP_H

#include <assert.h>

enum VBSPStatus {
	VBSP_STATUS_STOP		= 0,
	VBSP_STATUS_CONTINUE	= 1,
	VBSP_STATUS_SOMETHING	= 2,
};
	
enum PartitionType {
	BSP_NONE	= 0x0,
	BSPX 		= 0x1,
	BSPY 		= 0x2,
	BSPZ 		= 0x4,
	BSP_GARBAGE	= 0x8
};

//#define VBSP_MEM_LOGGING 1

#ifdef VBSP_MEM_LOGGING
#include <stdlib.h>
#include <stdio.h>
#endif // VBSP_MEM_LOGGING

#include <string.h>
#include "cache_memory_object.h"

// forward declaration
template <typename T> class VBSP;
	
template <typename T> class BSPNode : public CacheMemoryObject {

public:
	BSPNode ( VBSP<T> * tree_in, const float bounds_in[6], int max_depth_in, PartitionType partition_type_in ) {
		tree = tree_in;
		memcpy(bounds, bounds_in, 6 * sizeof(float));
		max_depth = max_depth_in;
		partition_type = partition_type_in;
		remaining_dimensions = BSPX | BSPY | BSPZ;
		
		contents_allocated = 0;
		contents_used = 0;
		contents = 0; // NULL

		unsorted_used = 0;
		unsorted = (T *)operator new (tree->GetUnsortedLimit() * sizeof(T));
#ifdef VBSP_MEM_LOGGING
		char str[4096];
		sprintf(str, "+%x\t%x\tNEW unsorted\n", unsorted, this);
		tree->Write(str);
#endif // VBSP_MEM_LOGGING
		unsorted_bounds = (float const **)operator new (tree->GetUnsortedLimit() * sizeof(float const *));
#ifdef VBSP_MEM_LOGGING
		sprintf(str, "+%x\t%x\tNEW unsorted_bounds\n", unsorted_bounds, this);
		tree->Write(str);
#endif // VBSP_MEM_LOGGING
		
		int i;
		for ( i = 0 ; i < tree->GetUnsortedLimit() ; i++ ) {
			unsorted_bounds[i] = 0; // NULL
		}
		//unsorted_bounds = 0; // NULL
		
		kids[0] = kids[1] = kids[2] = 0; // NULL
		
		collision = false;
	};
	
	~BSPNode () {
		if (contents) {
#ifdef VBSP_MEM_LOGGING
			char str[4096];
			sprintf(str, "-%x\t%x\tDELETE contents\n", contents, this);
			tree->Write(str);
#endif // VBSP_MEM_LOGGING
			operator delete (contents, contents_allocated * sizeof(T));
		}
		if (unsorted) {
#ifdef VBSP_MEM_LOGGING
			char str[4096];
			sprintf(str, "-%x\t%x\tDELETE unsorted\n", unsorted, this);
			tree->Write(str);
#endif // VBSP_MEM_LOGGING
			operator delete (unsorted, tree->GetUnsortedLimit() * sizeof(T));
		}
		if (unsorted_bounds) {
			int i;
			for ( i = 0 ; i < tree->GetUnsortedLimit() ; i++ ) {
				if (unsorted_bounds[i]) {
#ifdef VBSP_MEM_LOGGING
					char str[4096];
					sprintf(str, "-%x\t%x\tDELETE unsorted_bounds[i]\n", unsorted_bounds[i], this);
					tree->Write(str);
#endif // VBSP_MEM_LOGGING
					operator delete ((void *)unsorted_bounds[i], 6 * sizeof(float));
				}
			}

#ifdef VBSP_MEM_LOGGING
			char str[4096];
			sprintf(str, "-%x\t%x\tDELETE unsorted_bounds\n", unsorted_bounds, this);
			tree->Write(str);
#endif // VBSP_MEM_LOGGING
			operator delete (unsorted_bounds, tree->GetUnsortedLimit() * sizeof(float const *));
		}
		if (kids[0]) {
#ifdef VBSP_MEM_LOGGING
			char str[4096];
			sprintf(str, "-%x\t%x\tDELETE kids[0]\n", kids[0], this);
			tree->Write(str);
#endif // VBSP_MEM_LOGGING
			//operator delete (kids[0], sizeof(BSPNode<T>));
			delete kids[0];
		}
		if (kids[1]) {
#ifdef VBSP_MEM_LOGGING
			char str[4096];
			sprintf(str, "-%x\t%x\tDELETE kids[1]\n", kids[1], this);
			tree->Write(str);
#endif // VBSP_MEM_LOGGING
			//operator delete (kids[1], sizeof(BSPNode<T>));
			delete kids[1];
		}
		if (kids[2]) {
#ifdef VBSP_MEM_LOGGING
			char str[4096];
			sprintf(str, "-%x\t%x\tDELETE kids[2]\n", kids[2], this);
			tree->Write(str);
#endif // VBSP_MEM_LOGGING
			//operator delete (kids[2], sizeof(BSPNode<T>));
			delete kids[2];
		}
	};
	
	void AppendContents ( T data ) {
		if (contents_used == contents_allocated) {
			T *temp;
			
			if (contents_allocated < 4)
				contents_allocated = 4;
			else
				contents_allocated = (int)((float)contents_allocated * 1.5 + 4.0);
				
			temp = (T *)operator new (contents_allocated * sizeof(T));
#ifdef VBSP_MEM_LOGGING
			char str[4096];
			sprintf(str, "+%x\t%x\tNEW contents\n", temp, this);
			tree->Write(str);
#endif // VBSP_MEM_LOGGING
			if (contents) {
				memcpy(temp, contents, contents_used * sizeof(T));
#ifdef VBSP_MEM_LOGGING
				sprintf(str, "-%x\t%x\tDELETE used contents\n", contents, this);
				tree->Write(str);
#endif // VBSP_MEM_LOGGING
				operator delete (contents, contents_used * sizeof(T));
			}
			contents = temp;
		}
		contents[contents_used++] = data;
	};
	
	bool AppendUnsorted ( T item, float const item_bounds[6] ) {
		if (unsorted_used >= tree->GetUnsortedLimit())
			return false;
		unsorted[unsorted_used] = item;
		if (item_bounds != (float *)item) {

			float *bbox_copy = (float *)operator new (6 * sizeof(float));
#ifdef VBSP_MEM_LOGGING
			char str[4096];
			sprintf(str, "+%x\t%x\tNEW bbox\n", bbox_copy, this);
			tree->Write(str);
#endif // VBSP_MEM_LOGGING
			memcpy(bbox_copy, item_bounds, 6 * sizeof(float));
			unsorted_bounds[unsorted_used] = bbox_copy;
		}
		else
			unsorted_bounds[unsorted_used] = 0; // NULL
		unsorted_used++;
		return true;	
	};
	
	void SortUnsorted () {
		const float *item_bbox;
		T *unsorted_copy = unsorted;
		unsorted = 0; // NULL - avoid stack overflow

		int i;
		for ( i = 0 ; i < unsorted_used ; i++ ) {
			bool free_item_bounds = false;
			if (unsorted_bounds[i] != 0) {
				free_item_bounds = true;
				item_bbox = unsorted_bounds[i];
			}
			else
				item_bbox = (float *)unsorted_copy[i];

			InsertRecursive(unsorted_copy[i], item_bbox);
			
			if (free_item_bounds) {
#ifdef VBSP_MEM_LOGGING
				char str[4096];
				sprintf(str, "-%x\t%x\tDELETE used unsorted_bounds[i]\n", unsorted_bounds[i], this);
				tree->Write(str);
#endif // VBSP_MEM_LOGGING
				operator delete ((float *)unsorted_bounds[i], 6 * sizeof(float));
				unsorted_bounds[i] = 0; // NULL
			}
		}
#ifdef VBSP_MEM_LOGGING
		char str[4096];
		sprintf(str, "-%x\t%x\tDELETE used unsorted\n", unsorted_copy, this);
		tree->Write(str);
#endif // VBSP_MEM_LOGGING
		operator delete (unsorted_copy, tree->GetUnsortedLimit() * sizeof(T));

#ifdef VBSP_MEM_LOGGING
		sprintf(str, "-%x\t%x\tDELETE used unsorted_bounds\n", unsorted_bounds, this);
		tree->Write(str);
#endif // VBSP_MEM_LOGGING
		operator delete (unsorted_bounds, tree->GetUnsortedLimit() * sizeof(float const *));
		unsorted_bounds = 0; // NULL		
		unsorted_used = 0;
	};
	
	PartitionType GetNextDimension () {
		if (remaining_dimensions == BSP_NONE)
			return BSP_NONE;
			
		PartitionType next_dimension = partition_type;
		do {
			int next_dimension_int = next_dimension;
			next_dimension_int <<= 1;
			next_dimension = (PartitionType)next_dimension_int;
			if (next_dimension == BSP_GARBAGE)
				next_dimension = BSPX;
		} while (!(next_dimension & remaining_dimensions));
		
		return next_dimension;
	};
	
	void InsertRecursive ( T item, float const item_bounds[6] ) {
		if (max_depth == 0) {
			AppendContents(item);
			return;
		}
		if (unsorted) {
			if (AppendUnsorted(item, item_bounds))
				return;
			SortUnsorted();
		}
		
		int dim_max_offset = -1, dim_min_offset = -1;
		
		switch(partition_type) {
			case BSPX: {
				dim_max_offset = 3;
				dim_min_offset = 0;
			} break;
			case BSPY: {
				dim_max_offset = 4;
				dim_min_offset = 1;
			} break;
			case BSPZ: {
				dim_max_offset = 5;
				dim_min_offset = 2;
			} break;
			default:
				assert(0);
		}
		
		float cutoffs[2];
		float bounds_save;
		
		cutoffs[0] = bounds[dim_min_offset] + tree->GetLowCutoff() * (bounds[dim_max_offset] - bounds[dim_min_offset]);
		cutoffs[1] = bounds[dim_min_offset] + tree->GetHighCutoff() * (bounds[dim_max_offset] - bounds[dim_min_offset]);
		if (item_bounds[dim_max_offset] < cutoffs[1]) {
			/* fits on the left/bottom/front */
			bounds_save = bounds[dim_max_offset];
			bounds[dim_max_offset] = cutoffs[1];
			
			if (kids[0] == 0) {
				kids[0] = new BSPNode<T>(tree, bounds, (max_depth == -1 ? max_depth : max_depth - 1), GetNextDimension());
#ifdef VBSP_MEM_LOGGING
				char str[4096];
				sprintf(str, "+%x\t%x\tNEW kids[0]\n", kids[0], this);
				tree->Write(str);
#endif // VBSP_MEM_LOGGING
			}
			
			kids[0]->InsertRecursive(item, item_bounds);
			bounds[dim_max_offset] = bounds_save;
		}
		else if (item_bounds[dim_min_offset] > cutoffs[0]) {
			/* fits on the right/top/back */
			bounds_save = bounds[dim_min_offset];
			bounds[dim_min_offset] = cutoffs[0];
			
			if (kids[1] == 0) {
				kids[1] = new BSPNode<T>(tree, bounds, (max_depth == -1 ? max_depth : max_depth - 1), GetNextDimension());
#ifdef VBSP_MEM_LOGGING
				char str[4096];
				sprintf(str, "+%x\t%x\tNEW kids[1]\n", kids[1], this);
				tree->Write(str);
#endif // VBSP_MEM_LOGGING
			}
			
			kids[1]->InsertRecursive(item, item_bounds);
			bounds[dim_min_offset] = bounds_save;
		}
		else {
			if (!tree->GetSortLosers()) 
				AppendContents(item);
			else {
				remaining_dimensions &= ~partition_type;
				if (remaining_dimensions == BSP_NONE) {
					/* item_bounds crosses the entire overlap region, as well as the overlap regions
					 * of all other dimensions.	 it belongs at this level */
					AppendContents(item);
				}
				else {
					if (kids[2] == 0) {
						kids[2] = new BSPNode<T>(tree, bounds, (max_depth == -1 ? max_depth : max_depth - 1), GetNextDimension());
#ifdef VBSP_MEM_LOGGING
						char str[4096];
						sprintf(str, "+%x\t%x\tNEW kids[2]\n", kids[2], this);
						tree->Write(str);
#endif // VBSP_MEM_LOGGING
					}

					kids[2]->InsertRecursive(item, item_bounds);
				}
			}
		}
	};

	VBSPStatus MapFunctionRecursive ( VBSPStatus (*function) (T, void *), void * user_data ) {
		int i;

		for ( i = 0 ; i < contents_used ; i++ ) {
			if (function (contents[i], user_data) == VBSP_STATUS_STOP)
				return VBSP_STATUS_STOP;
		}

		for ( i = 0 ; i < unsorted_used ; i++ ) {
			if (function (unsorted[i], user_data) == VBSP_STATUS_STOP)
				return VBSP_STATUS_STOP;
		}
	
		if (kids[0])
			if (kids[0]->MapFunctionRecursive(function, user_data) == VBSP_STATUS_STOP)
				return VBSP_STATUS_STOP;
		if (kids[1])
			if (kids[1]->MapFunctionRecursive(function, user_data) == VBSP_STATUS_STOP)
				return VBSP_STATUS_STOP;
		if (kids[2])
			if (kids[2]->MapFunctionRecursive(function, user_data) == VBSP_STATUS_STOP)
				return VBSP_STATUS_STOP;
		return VBSP_STATUS_CONTINUE;
	};

	VBSPStatus PerNodeMapFunctionRecursive ( VBSPStatus(*function)(const int, T *, void *), void * user_data ) {
		if (unsorted_used) {
			if (function(unsorted_used, unsorted, user_data) == VBSP_STATUS_STOP)
				return VBSP_STATUS_STOP;
		}
		else if (contents_used) {
			if (function(contents_used, contents, user_data) == VBSP_STATUS_STOP)
				return VBSP_STATUS_STOP;
		}

		if (kids[0])
			if (kids[0]->PerNodeMapFunctionRecursive(function, user_data) == VBSP_STATUS_STOP)
				return VBSP_STATUS_STOP;
		if (kids[1])
			if (kids[1]->PerNodeMapFunctionRecursive(function, user_data) == VBSP_STATUS_STOP)
				return VBSP_STATUS_STOP;
		if (kids[2])
			if (kids[2]->PerNodeMapFunctionRecursive(function, user_data) == VBSP_STATUS_STOP)
				return VBSP_STATUS_STOP;

		return VBSP_STATUS_CONTINUE;
	};
	
protected:
	
	VBSP<T> *tree;
	float bounds[6];
	int max_depth;
	PartitionType partition_type;
	
	int remaining_dimensions;
	
	int contents_used;
	int contents_allocated;
	T *contents;
	
	int unsorted_used;
	T *unsorted;
	float const **unsorted_bounds;
	
	BSPNode<T> *kids[3];
	
	bool collision;
};

/*
	for all bounding boxes represented as float arrays, the convention is:
		min_x, min_y, min_z, max_x, max_y, max_z
 */

 template <typename T> class VBSP {

public:
	
	/*	- max_depth of -1 means there is no maximum depth - just keep creating nodes as needed
		- defaults represent existing defaults (defines) from hvbsp.cpp in 3dGS
	 */
	VBSP ( float bbox_in[6], int max_depth_in, int unsorted_limit_in = 10, bool sort_losers_in = true, 
		   float low_cutoff_in = 0.40f, float high_cutoff_in = 0.60f ) {
		memcpy(bbox, bbox_in, 6 * sizeof(float));
		max_depth = max_depth_in;
		unsorted_limit = unsorted_limit_in;
		sort_losers = sort_losers_in;
		low_cutoff = low_cutoff_in;
		high_cutoff = high_cutoff_in;

		root = new BSPNode<T>(this, bbox, max_depth, BSPX);

#ifdef VBSP_MEM_LOGGING
		char str[4096];
		sprintf(str, "+%x\t%x\tNEW root BSPNode\n", root, this);
		Write(str);
#endif // VBSP_MEM_LOGGING
	};
	
	~VBSP () {
		if (root) {
#ifdef VBSP_MEM_LOGGING
			char str[4096];
			sprintf(str, "-%x\t%x\tDELETE root BSPNode\n", root, this);
			Write(str);
#endif // MEM_LOGGIING	
			delete root;		
		}
	};
	
	/* 
		if item_bounds is NULL, then the first 24 bytes of the item being inserted into the tree are six floats (arranged in the
		conventional bbox fashion) which represent the bounding box to be used for that item.
	 */
	void Insert ( T item, const float * item_bounds = 0 ) {
		if (item_bounds == 0)
			item_bounds = (float *)item;
			
		root->InsertRecursive(item, item_bounds);
	};

	VBSPStatus MapFunction ( VBSPStatus (*function) (T, void *), void * user_data ) {
		return root->MapFunctionRecursive(function, user_data);
	};

	VBSPStatus PerNodeMapFunction ( VBSPStatus (*function) (const int, T *, void *), void * user_data ) {
		return root->PerNodeMapFunctionRecursive(function, user_data);
	};
	
	inline int GetUnsortedLimit () { return unsorted_limit; };
	inline bool GetSortLosers () { return sort_losers; };
	inline float GetLowCutoff () { return low_cutoff; };
	inline float GetHighCutoff () { return high_cutoff; };


#ifdef VBSP_MEM_LOGGING
	static void Write ( const char * str ) {
		FILE *fp;
		fp = fopen("C:/new_delete_info.txt", "a+");
		fwrite(str, 1, strlen(str), fp);
		fclose(fp);
	}
#endif // VBSP_MEM_LOGGING

		
protected:

	float bbox[6];
	int max_depth;	
	int unsorted_limit;
	bool sort_losers;
	float low_cutoff;
	float high_cutoff;
	BSPNode<T> *root;
}; 


#endif // VBSP_H
