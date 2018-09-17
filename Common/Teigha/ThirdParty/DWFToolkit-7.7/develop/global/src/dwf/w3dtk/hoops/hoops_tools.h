#ifndef HOOPSUTILITY
#define HOOPSUTILITY

///
///\file        dwf/w3dtk/hoops/hoops_tools.h
///

///////////////////////////////////////////////////////
//
#ifdef  DWFTK_STATIC
#ifndef HOOPSUTILITY_STATIC_LIB
#define HOOPSUTILITY_STATIC_LIB
#endif
#endif

#ifdef  DWFTK_EXPORT_API
#ifndef HOOPSUTILITY_EXPORTS
#define HOOPSUTILITY_EXPORTS
#endif
#endif
///////////////////////////////////////////////////////

#ifdef _MSC_VER
    #ifndef HOOPSUTILITY_STATIC_LIB
        #ifdef HOOPSUTILITY_EXPORTS
            #define HOOPSUTILITY_API  __declspec (dllexport)
        #else
            #define HOOPSUTILITY_API  __declspec (dllimport)
        #endif
    #else
        #define HOOPSUTILITY_API
    #endif
#else
    #define HOOPSUTILITY_API
#endif


typedef struct { float x, y, z; } HT_Point;
typedef struct { float x, y, z; } HT_Vector;

typedef int (* HT_Collapse_Check_Function) (HT_Point const * points,
                                            HT_Vector const * normals,
                                            int parameter_width, float const * parameters,
                                            int a, int b,
                                            double tolerance_squared,
                                            double normal_distance_squared,
                                            double parameter_distance_squared);

#define LOD_Algorithm_Fast 0
#define LOD_Algorithm_Nice 1

#ifdef __cplusplus
extern "C" {
#endif
#ifdef __HP_aCC
extern "C" {
#endif

void HOOPSUTILITY_API HU_GenerateShellLOD (
        int                     pcount,
        HT_Point const *        points,
        int                     flistlen,
        int const *             flist,
        double                  ratio,
        int                     algorithm,
        int *                   pcount_out,
        HT_Point *              points_out,
        int *                   flistlen_out,
        int *                   flist_out);


void HOOPSUTILITY_API HU_OptimizeShell (
        int                     in_point_count,
        HT_Point const *        in_points,
        HT_Vector const *       in_normals,         /* null if not interested */
        int                     in_parameter_width,
        float const *           in_parameters,      /* null if not interested */
        int                     in_face_list_length,
        int const *             in_face_list,
        HT_Collapse_Check_Function  collapsible,
        double                  tolerance,
        double                  normal_tolerance,
        double                  parameter_tolerance,
        int                     orphan_elimination,
        int *                   out_point_count,
        HT_Point *              out_points,
        int *                   out_face_list_length,
        int *                   out_face_list,
        int *                   vertex_mapping,     /* null if not interested */
        int *                   face_mapping);

#ifdef __cplusplus
}
#endif
#ifdef __HP_aCC
}
#endif


#endif


