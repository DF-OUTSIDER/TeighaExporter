//
// Copyright (c) 2000 by Tech Soft 3D, LLC.
// The information contained herein is confidential and proprietary to
// Tech Soft 3D, LLC., and considered a trade secret as defined under
// civil and criminal statutes.  Tech Soft 3D shall pursue its civil
// and criminal remedies in the event of unauthorized use or misappropriation
// of its trade secrets.  Use of this information by anyone other than
// authorized employees of Tech Soft 3D, LLC. is granted only under a
// written non-disclosure agreement, expressly prescribing the scope and
// manner of such use.
//
// $Header: //DWF/Working_Area/Willie.Zhu/w3dtk/BPointCloud.h#1 $
//

///
///\file        dwf/w3dtk/BPointCloud.h
///

#ifndef BBINFILETK_BPOINTCLOUD
#define BBINFILETK_BPOINTCLOUD
#include "dwf/w3dtk/BStreamFileToolkit.h"


#ifndef DOXYGEN_SHOULD_SKIP_THIS

/*!
    An experimental implementation of Arithmetic Coding as presented in Siggraph 2002.
    Not yet working, but will likely be included in future releases.
*/
class BPointCloud {
  private:
    class BKDTree *m_kd;            //private helper class defined in BPointCloud.cpp
    class BStreamFileToolkit *m_tk;
    int m_resolution;               //total bits per vertex, i.e. 3 * bits_per_sample
    int m_max_val;                  //(1 << bits_per_sample) - 1
    float m_bbox[6];
    float m_cell_size[3];           //bbox size / m_max_val
    int m_original_point_count;     //the number of points in the original shell (and hence the point map length)
    int m_point_count;              //the number of unique points (possibly less than original shell)
    float *m_points;                //points list, stored in the order intrinsic to the kd tree, not the original shell
    int *m_point_map;
    int m_next_id;
    unsigned char m_scheme;         //format identifier

    void put( class BKDTree *kd, class BPack *pack, int num_bits );
    TK_Status get( class BKDTree *b, class BPack *pack, int num_bits, int i, int j, int k, int span ); 


  public:
    //! constructor
    BPointCloud( class BStreamFileToolkit *tk );
    ~BPointCloud();

    //! specifies the 
    enum scheme {                   
        TKPC_Explicit       = 0x0001
    };    

    //write time functions
    TK_Status InitWrite( int resolution, const float *bbox, int original_point_count, const float *points );
    int GetSizeUpperBound() const;
    void Generate( unsigned char *buffer, int bytes_allocated, int *bytes_used );

    //read time functions
    TK_Status InitRead( int resolution, const float *bbox, int original_point_count );
    TK_Status Parse( const unsigned char *buffer, int size );
    TK_Status GetPoints( float *points );

};

#endif /* DOXYGEN_SHOULD_SKIP_THIS */
#endif /* BBINFILETK_BPOINTCLOUD */


