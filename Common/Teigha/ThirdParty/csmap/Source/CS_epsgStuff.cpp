/*
 * Copyright (c) 2008, Autodesk, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Autodesk, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY Autodesk, Inc. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Autodesk, Inc. OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

//lint -esym(534,TcsCsvFileBase::SetMinFldCnt)  ignoring return value
//lint -esym(534,TcsCsvFileBase::SetMaxFldCnt)  ignoring return value
//lint -esym(766,..\Include\cs_wkt.h)           unreferenced header, needed for pre-compiled headers
//lint -save -esym(613,crsTblPtr,copTblPtr)     possible use of null pointer, actually a PC-Lint bug
//lint -e514                                    usuaul use of boolean expression (e.g. ok &= )

#include "cs_map.h"
#include "cs_NameMapper.hpp"
//  cs_NameMapper.hpp includes cs_CsvFileSupport.hpp
//  cs_NameMapper.hpp includes csNameMapperSupport.hpp
#include "cs_WktObject.hpp"
#include "cs_wkt.h"

#include "cs_Legacy.h"
#include "cs_EpsgStuff.h"

extern "C" const double cs_Zero;
extern "C" const double cs_One;
extern "C" const double cs_Mone;
extern "C" const struct cs_Prjtab_ cs_Prjtab [];		// Projection Table
extern "C" const struct cs_Prjprm_ csPrjprm [];			// Parameter Table
extern "C" const struct cs_PrjprmMap_ cs_PrjprmMap [];		// Parameter Usage Table


//=============================================================================
// NOTE:  The CodeKeyField is the field ID of the field which carries a
// numeric record identifier which is unique.  In all cases, except the
// Change table, this value is an integral value.  In most cases, the
// table is sorted by this value, but there are exceptions.  Most notable
// exception is the Coordinate Axis table.  For several tables, the indicated
// sorted order is required; most notably the Coordinate Axis, Parameter Usage,
// Parameter Value, and Operation Path tables.
//
// It is the CodeKeyField by which the table is indexed in all cases.  As this
// is a feature of TcsCsvFileBase, the index is actually in text form, and in
// the case of the Change table, in the exact form as it appears in the EPSG
// database.  Some future tweaking in this regard is likely.
//=============================================================================

const TcsEpsgTblMap KcsEpsgTblMap [] =
{
//                                  Field
//                          TableID  Cnt  CodeKeyField                  Table CSV File Name                     Sort Field 1                Sort Field 2              Sort Field 3             Sort Field 4  
	{                  epsgTblAlias,  6,  epsgFldAliasCode,            L"Alias",                                epsgFldAliasCode,           epsgFldNone,              epsgFldNone,             epsgFldNone },
	{                   epsgTblArea, 17,  epsgFldAreaCode,             L"Area",                                 epsgFldAreaCode,            epsgFldNone,              epsgFldNone,             epsgFldNone },
	{                 epsgTblChange,  9,  epsgFldNone,                 L"Change",                               epsgFldChangeId,            epsgFldNone,              epsgFldNone,             epsgFldNone },
	{               epsgTblAxisName,  9,  epsgFldCoordAxisNameCode,    L"Coordinate Axis Name",                 epsgFldCoordAxisNameCode,   epsgFldNone,              epsgFldNone,             epsgFldNone },
	{                   epsgTblAxis,  7,  epsgFldCoordAxisCode,        L"Coordinate Axis",                      epsgFldCoordSysCode,        epsgFldOrder,             epsgFldNone,             epsgFldNone },
	{        epsgTblReferenceSystem, 18,  epsgFldCoordRefSysCode,      L"Coordinate Reference System",          epsgFldCoordRefSysCode,     epsgFldNone,              epsgFldNone,             epsgFldNone },
	{       epsgTblCoordinateSystem, 10,  epsgFldCoordSysCode,         L"Coordinate System",                    epsgFldCoordSysCode,        epsgFldNone,              epsgFldNone,             epsgFldNone },
	{        epsgTblOperationMethod, 11,  epsgFldCoordOpMethodCode,    L"Coordinate_Operation Method",          epsgFldCoordOpMethodCode,   epsgFldNone,              epsgFldNone,             epsgFldNone },
	{         epsgTblParameterUsage,  4,  epsgFldNone,                 L"Coordinate_Operation Parameter Usage", epsgFldCoordOpMethodCode,   epsgFldSortOrder,         epsgFldNone,             epsgFldNone },
	{         epsgTblParameterValue,  6,  epsgFldNone,                 L"Coordinate_Operation Parameter Value", epsgFldCoordOpCode,         epsgFldCoordOpMethodCode, epsgFldParameterCode,    epsgFldNone },
	{              epsgTblParameter,  8,  epsgFldParameterCode,        L"Coordinate_Operation Parameter",       epsgFldParameterCode,       epsgFldNone,              epsgFldNone,             epsgFldNone },
	{          epsgTblOperationPath,  3,  epsgFldNone,                 L"Coordinate_Operation Path",            epsgFldConcatOperationCode, epsgFldOpPathStep,        epsgFldNone,             epsgFldNone },
 	{    epsgTblCoordinateOperation, 20,  epsgFldCoordOpCode,          L"Coordinate_Operation",                 epsgFldCoordOpCode,         epsgFldNone,              epsgFldNone,             epsgFldNone },
	{                  epsgTblDatum, 15,  epsgFldDatumCode,            L"Datum",                                epsgFldDatumCode,           epsgFldNone,              epsgFldNone,             epsgFldNone },
	{            epsgTblDeprecation,  7,  epsgFldDeprecationId,        L"Deprecation",                          epsgFldObjectCode,          epsgFldNone,              epsgFldNone,             epsgFldNone },
	{              epsgTblEllipsoid, 13,  epsgFldEllipsoidCode,        L"Ellipsoid",                            epsgFldEllipsoidCode,       epsgFldNone,              epsgFldNone,             epsgFldNone },
	{           epsgTblNamingSystem,  8,  epsgFldNamingSystemCode,     L"Naming System",                        epsgFldNamingSystemCode,    epsgFldNone,              epsgFldNone,             epsgFldNone },
	{          epsgTblPrimeMeridian, 10,  epsgFldPrimeMeridianCode,    L"Prime Meridian",                       epsgFldPrimeMeridianCode,   epsgFldNone,              epsgFldNone,             epsgFldNone },
	{           epsgTblSupercession,  7,  epsgFldSupersessionId,       L"Supersession",                         epsgFldObjectCode,          epsgFldObjectTableName,   epsgFldSupersessionYear, epsgFldNone },
	{          epsgTblUnitOfMeasure, 12,  epsgFldUomCode,              L"Unit of Measure",                      epsgFldUomCode,             epsgFldNone,              epsgFldNone,             epsgFldNone },
	{         epsgTblVersionHistory,  6,  epsgFldVersionHistoryCode,   L"Version History",                      epsgFldVersionHistoryCode,  epsgFldNone,              epsgFldNone,             epsgFldNone },
	{                epsgTblUnknown,  0,  epsgFldNone,                 L"",                                     epsgFldNone,                epsgFldNone,              epsgFldNone,             epsgFldNone },
};
const TcsEpsgFldMap KcsEpsgFldMap [] =
{
//                                                                 Field
//                       TableID       FieldId                       Nbr    Field Label
	{               epsgTblAlias,      epsgFldAliasCode,               0,  L"ALIAS CODE"                  },
	{               epsgTblAlias,      epsgFldObjectType,              1,  L"OBJECT_TABLE_NAME"           },
	{               epsgTblAlias,      epsgFldObjectCode,              2,  L"OBJECT_CODE"                 },
	{               epsgTblAlias,      epsgFldCodeNamingSystem,        3,  L"NAMING_SYSTEM_CODE"          },
	{               epsgTblAlias,      epsgFldAlias,                   4,  L"ALIAS"                       },
	{               epsgTblAlias,      epsgFldRemarks,                 5,  L"REMARKS"                     },

	{                epsgTblArea,      epsgFldAreaCode,                0,  L"AREA_CODE"                   },
	{                epsgTblArea,      epsgFldAreaName,                1,  L"AREA_NAME"                   },
	{                epsgTblArea,      epsgFldAreaOfUse,               2,  L"AREA_OF_USE"                 },
	{                epsgTblArea,      epsgFldAreaSouthBoundLat,       3,  L"AREA_SOUTH_BOUND_LAT"        },
	{                epsgTblArea,      epsgFldAreaNorthBoundLat,       4,  L"AREA_NORTH_BOUND_LAT"        },
	{                epsgTblArea,      epsgFldAreaWestBoundLng,        5,  L"AREA_WEST_BOUND_LON"         },
	{                epsgTblArea,      epsgFldAreaEastBoundLng,        6,  L"AREA_EAST_BOUND_LON"         },
	{                epsgTblArea,      epsgFldBoundingPolygonFileName, 7,  L"AREA_POLYGON_FILE_REF"       },
	{                epsgTblArea,      epsgFldIsoA2Code,               8,  L"ISO_A2_CODE"                 },
	{                epsgTblArea,      epsgFldIsoA3Code,               9,  L"ISO_A3_CODE"                 },
	{                epsgTblArea,      epsgFldIsoNumericCode,         10,  L"ISO_N_CODE"                  },
	{                epsgTblArea,      epsgFldRemarks,                11,  L"REMARKS"                     },
	{                epsgTblArea,      epsgFldInformationSource,      12,  L"INFORMATION_SOURCE"          },
	{                epsgTblArea,      epsgFldDataSource,             13,  L"DATA_SOURCE"                 },
	{                epsgTblArea,      epsgFldRevisionDate,           14,  L"REVISION_DATE"               },
	{                epsgTblArea,      epsgFldChangeId,               15,  L"CHANGE_ID"                   },
	{                epsgTblArea,      epsgFldDeprecated,             16,  L"DEPRECATED"                  },

	{              epsgTblChange,      epsgFldChangeId,                0,  L"CHANGE_ID"                   },
	{              epsgTblChange,      epsgFldReportDate,              1,  L"REPORT_DATE"                 },
	{              epsgTblChange,      epsgFldDateClosed,              2,  L"DATE_CLOSED"                 },
	{              epsgTblChange,      epsgFldReporter,                3,  L"REPORTER"                    },
	{              epsgTblChange,      epsgFldRequest,                 4,  L"REQUEST"                     },
	{              epsgTblChange,      epsgFldTablesAffected,          5,  L"TABLES_AFFECTED"             },
	{              epsgTblChange,      epsgFldCodesAffected,           6,  L"CODES_AFFECTED"              },
	{              epsgTblChange,      epsgFldComment,                 7,  L"COMMENT"                     },
	{              epsgTblChange,      epsgFldAction,                  8,  L"ACTION"                      },

	{            epsgTblAxisName,      epsgFldCoordAxisNameCode,       0,  L"COORD_AXIS_NAME_CODE"        },
	{            epsgTblAxisName,      epsgFldCoordAxisName,           1,  L"COORD_AXIS_NAME"             },
	{            epsgTblAxisName,      epsgFldDescription,             2,  L"DESCRIPTION"                 },
	{            epsgTblAxisName,      epsgFldRemarks,                 3,  L"REMARKS"                     },
	{            epsgTblAxisName,      epsgFldInformationSource,       4,  L"INFORMATION_SOURCE"          },
	{            epsgTblAxisName,      epsgFldDataSource,              5,  L"DATA_SOURCE"                 },
	{            epsgTblAxisName,      epsgFldRevisionDate,            6,  L"REVISION_DATE"               },
	{            epsgTblAxisName,      epsgFldChangeId,                7,  L"CHANGE_ID"                   },
	{            epsgTblAxisName,      epsgFldDeprecated,              8,  L"DEPRECATED"                  },

	{                epsgTblAxis,      epsgFldCoordAxisCode,           0,  L"COORD_AXIS_CODE"             },
	{                epsgTblAxis,      epsgFldCoordSysCode,            1,  L"COORD_SYS_CODE"              },
	{                epsgTblAxis,      epsgFldCoordAxisNameCode,       2,  L"COORD_AXIS_NAME_CODE"        },
	{                epsgTblAxis,      epsgFldCoordAxisOrientation,    3,  L"COORD_AXIS_ORIENTATION"      },
	{                epsgTblAxis,      epsgFldCoordAxisAbbreviation,   4,  L"COORD_AXIS_ABBREVIATION"     },
	{                epsgTblAxis,      epsgFldUomCode,                 5,  L"UOM_CODE"                    },
	{                epsgTblAxis,      epsgFldOrder,                   6,  L"ORDER"                       },

	{     epsgTblReferenceSystem,      epsgFldCoordRefSysCode,         0,  L"COORD_REF_SYS_CODE"          },
	{     epsgTblReferenceSystem,      epsgFldCoordRefSysName,         1,  L"COORD_REF_SYS_NAME"          },
	{     epsgTblReferenceSystem,      epsgFldAreaOfUseCode,           2,  L"AREA_OF_USE_CODE"            },
	{     epsgTblReferenceSystem,      epsgFldCoordRefSysKind,         3,  L"COORD_REF_SYS_KIND"          },
	{     epsgTblReferenceSystem,      epsgFldCoordSysCode,            4,  L"COORD_SYS_CODE"              },
	{     epsgTblReferenceSystem,      epsgFldDatumCode,               5,  L"DATUM_CODE"                  },
	{     epsgTblReferenceSystem,      epsgFldSourceGeogCrsCode,       6,  L"SOURCE_GEOGCRS_CODE"         },
	{     epsgTblReferenceSystem,      epsgFldProjectionConvCode,      7,  L"PROJECTION_CONV_CODE"        },
	{     epsgTblReferenceSystem,      epsgFldCmpdHorizCrsCode,        8,  L"CMPD_HORIZCRS_CODE"          },
	{     epsgTblReferenceSystem,      epsgFldCmpdVertCrsCode,         9,  L"CMPD_VERTCS_CODE"            },
	{     epsgTblReferenceSystem,      epsgFldCrsScope,               10,  L"CRS_SCOPE"                   },
	{     epsgTblReferenceSystem,      epsgFldRemarks,                11,  L"REMARKS"                     },
	{     epsgTblReferenceSystem,      epsgFldInformationSource,      12,  L"INFORMATION_SOURCE"          },
	{     epsgTblReferenceSystem,      epsgFldDataSource,             13,  L"DATA_SOURCE"                 },
	{     epsgTblReferenceSystem,      epsgFldRevisionDate,           14,  L"REVISION_DATE"               },
	{     epsgTblReferenceSystem,      epsgFldChangeId,               15,  L"CHANGE_ID"                   },
	{     epsgTblReferenceSystem,      epsgFldShowCrs,                16,  L"SHOW_CRS"                    },
	{     epsgTblReferenceSystem,      epsgFldDeprecated,             17,  L"DEPRECATED"                  },

	{    epsgTblCoordinateSystem,      epsgFldCoordSysCode,            0,  L"COORD_SYS_CODE"              },
	{    epsgTblCoordinateSystem,      epsgFldCoordSysName,            1,  L"COORD_SYS_NAME"              },
	{    epsgTblCoordinateSystem,      epsgFldCoordSysType,            2,  L"COORD_SYS_TYPE"              },
	{    epsgTblCoordinateSystem,      epsgFldDimension,               3,  L"DIMENSION"                   },
	{    epsgTblCoordinateSystem,      epsgFldRemarks,                 4,  L"REMARKS"                     },
	{    epsgTblCoordinateSystem,      epsgFldInformationSource,       5,  L"INFORMATION_SOURCE"          },
	{    epsgTblCoordinateSystem,      epsgFldDataSource,              6,  L"DATA_SOURCE"                 },
	{    epsgTblCoordinateSystem,      epsgFldRevisionDate,            7,  L"REVISION_DATE"               },
	{    epsgTblCoordinateSystem,      epsgFldChangeId,                8,  L"CHANGE_ID"                   },
	{    epsgTblCoordinateSystem,      epsgFldDeprecated,              9,  L"DEPRECATED"                  },

	{     epsgTblOperationMethod,      epsgFldCoordOpMethodCode,       0,  L"COORD_OP_METH_CODE"          },
	{     epsgTblOperationMethod,      epsgFldCoordOpMethodName,       1,  L"COORD_OP_METH_NAME"          },
	{     epsgTblOperationMethod,      epsgFldReverseOp,               2,  L"REVERSE_OP"                  },
	{     epsgTblOperationMethod,      epsgFldFormula,                 3,  L"FORMULA"                     },
	{     epsgTblOperationMethod,      epsgFldExample,                 4,  L"EXAMPLE"                     },
	{     epsgTblOperationMethod,      epsgFldRemarks,                 5,  L"REMARKS"                     },
	{     epsgTblOperationMethod,      epsgFldInformationSource,       6,  L"INFORMATION_SOURCE"          },
	{     epsgTblOperationMethod,      epsgFldDataSource,              7,  L"DATA_SOURCE"                 },
	{     epsgTblOperationMethod,      epsgFldRevisionDate,            8,  L"REVISION_DATE"               },
	{     epsgTblOperationMethod,      epsgFldChangeId,                9,  L"CHANGE_ID"                   },
	{     epsgTblOperationMethod,      epsgFldDeprecated,             10,  L"DEPRECATED"                  },

	{      epsgTblParameterUsage,      epsgFldCoordOpMethodCode,       0,  L"COORD_OP_METHOD_CODE"        },
	{      epsgTblParameterUsage,      epsgFldParameterCode,           1,  L"PARAMETER_CODE"              },
	{      epsgTblParameterUsage,      epsgFldSortOrder,               2,  L"SORT_ORDER"                  },
	{      epsgTblParameterUsage,      epsgFldParamSignReversal,       3,  L"PARAM_SIGN_REVERSAL"         },
	
	{      epsgTblParameterValue,      epsgFldCoordOpCode,             0,  L"COORD_OP_CODE"               },
	{      epsgTblParameterValue,      epsgFldCoordOpMethodCode,       1,  L"COORD_OP_METHOD_CODE"        },
	{      epsgTblParameterValue,      epsgFldParameterCode,           2,  L"PARAMETER_CDOE"              },
	{      epsgTblParameterValue,      epsgFldParameterValue,          3,  L"PARAMETER_VALUE"             },
	{      epsgTblParameterValue,      epsgFldParamValueFileRef,       4,  L"PARAM_VALUE_FILE_REF"        },
	{      epsgTblParameterValue,      epsgFldUomCode,                 5,  L"UOM_CODE"                    },

	{           epsgTblParameter,      epsgFldParameterCode,           0,  L"PARAMETER_CODE"              },
	{           epsgTblParameter,      epsgFldParameterName,           1,  L"PARAMETER_NAME"              },
	{           epsgTblParameter,      epsgFldDescription,             2,  L"DESCRIPTION"                 },
	{           epsgTblParameter,      epsgFldInformationSource,       3,  L"INFORMATION_SOURCE"          },
	{           epsgTblParameter,      epsgFldDataSource,              4,  L"DATA_SOURCE"                 },
	{           epsgTblParameter,      epsgFldRevisionDate,            5,  L"REVISION_DATE"               },
	{           epsgTblParameter,      epsgFldChangeId,                6,  L"CHANGE_ID"                   },
	{           epsgTblParameter,      epsgFldDeprecated,              7,  L"DEPRECATED"                  },

	{       epsgTblOperationPath,      epsgFldConcatOperationCode,     0,  L"CONCAT_OPERATION_CODE"       },
	{       epsgTblOperationPath,      epsgFldSingleOperationCode,     1,  L"SINGLE_OPERATION_CODE"       },
	{       epsgTblOperationPath,      epsgFldOpPathStep,              2,  L"OP_PATH_STEP"                },

	{ epsgTblCoordinateOperation,      epsgFldCoordOpCode,             0,  L"COORD_OP_CODE"               },
	{ epsgTblCoordinateOperation,      epsgFldCoordOpName,             1,  L"COORD_OP_NAME"               },
	{ epsgTblCoordinateOperation,      epsgFldCoordOpType,             2,  L"COORD_OP_TYPE"               },
	{ epsgTblCoordinateOperation,      epsgFldSourceCrsCode,           3,  L"SOURCE_CRS_CODE"             },
	{ epsgTblCoordinateOperation,      epsgFldTargetCrsCode,           4,  L"TARGET_CRS_CODE"             },
	{ epsgTblCoordinateOperation,      epsgFldCoordTfmVersion,         5,  L"COORD_TFM_VERSION"           },
	{ epsgTblCoordinateOperation,      epsgFldCoordOpVariant,          6,  L"COORD_OP_VARIANT"            },
	{ epsgTblCoordinateOperation,      epsgFldAreaOfUseCode,           7,  L"AREA_OF_USE_CODE"            },
	{ epsgTblCoordinateOperation,      epsgFldCoordOpScope,            8,  L"COORD_OP_SCOPE"              },
	{ epsgTblCoordinateOperation,      epsgFldCoordOpAccuracy,         9,  L"COORD_OP_ACCURACY"           },
	{ epsgTblCoordinateOperation,      epsgFldCoordOpMethodCode,      10,  L"COORD_OP_METHOD_CODE"        },
	{ epsgTblCoordinateOperation,      epsgFldUomCodeSourceCoordDiff, 11,  L"UOM_CODE_SOURCE_COORD_DIFF"  },
	{ epsgTblCoordinateOperation,      epsgFldUomCodeTargetCoordDiff, 12,  L"UOM_CODE_TARGET_COORD_DIFF"  },
	{ epsgTblCoordinateOperation,      epsgFldRemarks,                13,  L"REMARKS"                     },
	{ epsgTblCoordinateOperation,      epsgFldInformationSource,      14,  L"INFORMATION_SOURCE"          },
	{ epsgTblCoordinateOperation,      epsgFldDataSource,             15,  L"DATA_SOURCE"                 },
	{ epsgTblCoordinateOperation,      epsgFldRevisionDate,           16,  L"REVISION_DATE"               },
	{ epsgTblCoordinateOperation,      epsgFldChangeId,               17,  L"CHANGE_ID"                   },
	{ epsgTblCoordinateOperation,      epsgFldShowOperation,          18,  L"SHOW_OPERATION"              },
	{ epsgTblCoordinateOperation,      epsgFldDeprecated,             19,  L"DEPRECATED"                  },

	{               epsgTblDatum,      epsgFldDatumCode,               0,  L"DATUM_CODE"                  },
	{               epsgTblDatum,      epsgFldDatumName,               1,  L"DATUM_NAME"                  },
	{               epsgTblDatum,      epsgFldDatumType,               2,  L"DATUM_TYPE"                  },
	{               epsgTblDatum,      epsgFldOriginDescription,       3,  L"ORIGIN_DESCRIPTION"          },
	{               epsgTblDatum,      epsgFldRealizationEpoch,        4,  L"REALIZATION_EPOCH"           },
	{               epsgTblDatum,      epsgFldEllipsoidCode,           5,  L"ELLIPSOID_CODE"              },
	{               epsgTblDatum,      epsgFldPrimeMeridianCode,       6,  L"PRIME_MERIDIAN_CODE"         },
	{               epsgTblDatum,      epsgFldAreaOfUseCode,           7,  L"AREA_OF_USE_CODE"            },
	{               epsgTblDatum,      epsgFldDatumScope,              8,  L"DATUM_SCOPE"                 },
	{               epsgTblDatum,      epsgFldRemarks,                 9,  L"REMARKS"                     },
	{               epsgTblDatum,      epsgFldInformationSource,      10,  L"INFORMATION_SOURCE"          },
	{               epsgTblDatum,      epsgFldDataSource,             11,  L"DATA_SOURCE"                 },
	{               epsgTblDatum,      epsgFldRevisionDate,           12,  L"REVISION_DATE"               },
	{               epsgTblDatum,      epsgFldChangeId,               13,  L"CHANGE_ID"                   },
	{               epsgTblDatum,      epsgFldDeprecated,             14,  L"DEPRECATED"                  },

	{         epsgTblDeprecation,      epsgFldDeprecationId,           0,  L"DEPRECATION_ID"              },
	{         epsgTblDeprecation,      epsgFldDeprecationDate,         1,  L"DEPRECATION_DATE"            },
	{         epsgTblDeprecation,      epsgFldChangeId,                2,  L"CHANGE_ID"                   },
	{         epsgTblDeprecation,      epsgFldObjectTableName,         3,  L"OBJECT_TABLE_NAME"           },
	{         epsgTblDeprecation,      epsgFldObjectCode,              4,  L"OBJECT_CODE"                 },
	{         epsgTblDeprecation,      epsgFldReplacedBy,              5,  L"REPLACED_BY"                 },
	{         epsgTblDeprecation,      epsgFldDeprecationReason,       6,  L"DEPRECATION_REASON"          },

	{           epsgTblEllipsoid,      epsgFldEllipsoidCode,           0,  L"ELLIPSOID_CODE"              },
	{           epsgTblEllipsoid,      epsgFldEllipsoidName,           1,  L"ELLIPSOID_NAME"              },
	{           epsgTblEllipsoid,      epsgFldSemiMajorAxis,           2,  L"SEMI_MAJOR_AXIS"             },
	{           epsgTblEllipsoid,      epsgFldUomCode,                 3,  L"UOM_CODE"                    },
	{           epsgTblEllipsoid,      epsgFldInvFlattening,           4,  L"INV_FLATTENING"              },
	{           epsgTblEllipsoid,      epsgFldSemiMinorAxis,           5,  L"SEMI_MINOR_AXIS"             },
	{           epsgTblEllipsoid,      epsgFldEllipsoidShape,          6,  L"ELLIPSOID_SHAPE"             },
	{           epsgTblEllipsoid,      epsgFldRemarks,                 7,  L"REMARKS"                     },
	{           epsgTblEllipsoid,      epsgFldInformationSource,       8,  L"INFORMATION_SOURCE"          },
	{           epsgTblEllipsoid,      epsgFldDataSource,              9,  L"DATA_SOURCE"                 },
	{           epsgTblEllipsoid,      epsgFldRevisionDate,           10,  L"REVISION_DATE"               },
	{           epsgTblEllipsoid,      epsgFldChangeId,               11,  L"CHANGE_ID"                   },
	{           epsgTblEllipsoid,      epsgFldDeprecated,             12,  L"DEPRECATED"                  },

	{        epsgTblNamingSystem,      epsgFldNamingSystemCode,        0,  L"NAMING_SYSTEM_CODE"          },
	{        epsgTblNamingSystem,      epsgFldNamingSystemName,        1,  L"NAMING_SYSTEM_NAME"          },
	{        epsgTblNamingSystem,      epsgFldRemarks,                 2,  L"REMARKS"                     },
	{        epsgTblNamingSystem,      epsgFldInformationSource,       3,  L"INFORMATION_SOURCE"          },
	{        epsgTblNamingSystem,      epsgFldDataSource,              4,  L"DATA_SOURCE"                 },
	{        epsgTblNamingSystem,      epsgFldRevisionDate,            5,  L"REVISION_DATE"               },
	{        epsgTblNamingSystem,      epsgFldChangeId,                6,  L"CHANGE_ID"                   },
	{        epsgTblNamingSystem,      epsgFldDeprecated,              7,  L"DEPRECATED"                  },

	{       epsgTblPrimeMeridian,      epsgFldPrimeMeridianCode,       0,  L"PRIME_MERIDIAN_CODE"         },
	{       epsgTblPrimeMeridian,      epsgFldPrimeMeridianName,       1,  L"PRIME_MERIDIAN_NAME"         },
	{       epsgTblPrimeMeridian,      epsgFldGreenwichLongitude,      2,  L"GREENWICH_LONGITUDE"         },
	{       epsgTblPrimeMeridian,      epsgFldUomCode,                 3,  L"UOM_CODE"                    },
	{       epsgTblPrimeMeridian,      epsgFldRemarks,                 4,  L"REMARKS"                     },
	{       epsgTblPrimeMeridian,      epsgFldInformationSource,       5,  L"INFORMATION_SOURCE"          },
	{       epsgTblPrimeMeridian,      epsgFldDataSource,              6,  L"DATA_SOURCE"                 },
	{       epsgTblPrimeMeridian,      epsgFldRevisionDate,            7,  L"REVISION_DATE"               },
	{       epsgTblPrimeMeridian,      epsgFldChangeId,                8,  L"CHANGE_ID"                   },
	{       epsgTblPrimeMeridian,      epsgFldDeprecated,              9,  L"DEPRECATED"                  },

	{        epsgTblSupercession,      epsgFldSupersessionId,          0,  L"SUPERSESSION_ID"             },
	{        epsgTblSupercession,      epsgFldObjectTableName,         1,  L"OBJECT_TABLE_NAME"           },
	{        epsgTblSupercession,      epsgFldObjectCode,              2,  L"OBJECT_CODE"                 },
	{        epsgTblSupercession,      epsgFldSupercededBy,            3,  L"SUPERSEDED_BY"               },
	{        epsgTblSupercession,      epsgFldSupersessionType,        4,  L"SUPERSESSION_TYPE"           },
	{        epsgTblSupercession,      epsgFldSupersessionYear,        5,  L"SUPERSESSION_YEAR"           },
	{        epsgTblSupercession,      epsgFldRemarks,                 6,  L"REMARKS"                     },

	{       epsgTblUnitOfMeasure,      epsgFldUomCode,                 0,  L"UOM_CODE"                    },
	{       epsgTblUnitOfMeasure,      epsgFldUnitOfMeasName,          1,  L"UNIT_OF_MEAS_NAME"           },
	{       epsgTblUnitOfMeasure,      epsgFldUnitOfMeasType,          2,  L"UNIT_OF_MEAS_TYPE"           },
	{       epsgTblUnitOfMeasure,      epsgFldTargetUomCode,           3,  L"TRAGET_UOM_CODE"             },
	{       epsgTblUnitOfMeasure,      epsgFldFactorB,                 4,  L"FACTOR_B"                    },
	{       epsgTblUnitOfMeasure,      epsgFldFactorC,                 5,  L"FACTOR_C"                    },
	{       epsgTblUnitOfMeasure,      epsgFldRemarks,                 6,  L"REMARKS"                     },
	{       epsgTblUnitOfMeasure,      epsgFldInformationSource,       7,  L"INFORMATION_SOURCE"          },
	{       epsgTblUnitOfMeasure,      epsgFldDataSource,              8,  L"DATA_SOURCE"                 },
	{       epsgTblUnitOfMeasure,      epsgFldRevisionDate,            9,  L"REVISION_DATE"               },
	{       epsgTblUnitOfMeasure,      epsgFldChangeId,               10,  L"CHANGE_ID"                   },
	{       epsgTblUnitOfMeasure,      epsgFldDeprecated,             11,  L"DEPRECATED"                  },

	{      epsgTblVersionHistory,      epsgFldVersionHistoryCode,      0,  L"VERSION_HISTORY_CODE"        },
	{      epsgTblVersionHistory,      epsgFldVersionDate,             1,  L"VERSION_DATE"                },
	{      epsgTblVersionHistory,      epsgFldVersionNumber,           2,  L"VERSION_NUMBER"              },
	{      epsgTblVersionHistory,      epsgFldRemarks,                 3,  L"VERSION_REMARKS"             },
	{      epsgTblVersionHistory,      epsgFldSupercededBy,            4,  L"SUPERCEDED_BY"               },
	{      epsgTblVersionHistory,      epsgFldSupercedes,              5,  L"SUPERCEDES"                  },

	{             epsgTblUnknown,      epsgFldUnknown,                 0,  L"<unknown table>"             }
};

const TcsEpsgCsysTypeMap KcsEpsgCsysTypeMap [] =
{
	{        epsgCsysTypAffine,       L"affine"               },
	{     epsgCsysTypCartesian,       L"Cartesian"            },
	{   epsgCsysTypCylindrical,       L"cylindrical"          },
	{   epsgCsysTypEllipsoidal,       L"ellipsoidal"          },
	{        epsgCsysTypLinear,       L"linear"               },
	{         epsgCsysTypPolar,       L"polar"                },
	{     epsgCsysTypSpherical,       L"spherical"            },
	{      epsgCsysTypVertical,       L"vertical"             },
	{       epsgCsysTypUnknown,       L"<unknown CSys type>"  }
};

const TcsEpsgCrsTypeMap KcsEpsgCrsTypeMap [] =
{
	{       epsgCrsTypCompund,       L"compound"            },
	{   epsgCrsTypEngineering,       L"engineering"         },
	{    epsgCrsTypGeocentric,       L"geocentric"          },
	{  epsgCrsTypGeographic2D,       L"geographic 2D"       },
	{  epsgCrsTypGeographic3D,       L"geographic 3D"       },
	{     epsgCrsTypProjected,       L"projected"           },
	{      epsgCrsTypVertical,       L"vertical"            },
	{       epsgCrsTypUnknown,       L"<unknown CRS type>"  }
};

const TcsEpsgOpTypeMap KcsEpsgOpTypeMap [] =
{
	{         epsgOpTypConversion,   L"conversion"               },
	{     epsgOpTypTransformation,   L"transformation"           },
	{       epsgOpTypConcatenated,   L"concatenated operation"   },
	{            epsgOpTypUnknown,   L"<unknown op type>"        }
};

const TcsEpsgDtmTypeMap KcsEpsgDtmTypeMap [] =
{
	{     epsgDtmTypGeodetic,     L"geodetic"            },
	{     epsgDtmTypVertical,     L"vertical"            },
	{  epsgDtmTypEngineering,     L"engineering"         },
	{      epsgDtmTypUnknown,     L"<unknown dtm type>"  }
};

const TcsEpsgUomTypeMap KcsEpsgUomTypeMap [] =
{
	{     epsgUomTypLinear,     L"Length"              },
	{    epsgUomTypAngular,     L"Angle"               },
	{      epsgUomTypScale,     L"Scale"               },
	{    epsgUomTypUnknown,     L"<unknown UOM type>"  }
};

const TcsEpsgOrntTypeMap KcsEpsgOrntTypeMap [] =
{
	{         epsgOrntEast,     L"South along 90E"     },
	{        epsgOrntNorth,     L"South along 180E"    },
	{         epsgOrntEast,     L"North along 90E"     },
	{        epsgOrntNorth,     L"North along 0E"      },
	{         epsgOrntEast,     L"east"                },
	{         epsgOrntWest,     L"west"                },
	{        epsgOrntNorth,     L"north"               },
	{        epsgOrntSouth,     L"south"               },
	{           epsgOrntUp,     L"up"                  },
	{         epsgOrntDown,     L"down"                },
	{      epsgOrntUnknown,     L"<unknownOrientation>"}
};

// The following maps EPSG Area codes to CS-MAP group names.
// A bit hoekey, but it provides reasonably good results.  Table
// generation dates back to EPSG 6.13; so it probably use an
// update.
const struct TcmAreaToMsiGroupMap
{
	short epsgAreaFrom;
	short epsgAreaTo;
	char msiGroup [16];
} KcmAreaToMsiGroupMap [] =
{
	{  1024,  1024,  "ASIA"      },
	{  1025,  1025,  "EUROPE"    },
	{  1026,  1026,  "AFRICA"    },
	{  1027,  1027,  "PACIFIC"   },
	{  1028,  1028,  "EUROPE"    },
	{  1029,  1029,  "AFRICA"    },
	{  1030,  1030,  "CARIB"     },
	{  1031,  1031,  "POLAR"     },
	{  1032,  1032,  "CARIB"     },
	{  1033,  1033,  "SAMER"     },
	{  1034,  1034,  "ASIA"      },
	{  1035,  1035,  "CARIB"     },
	{  1036,  1036,  "AUSNZ"     },
	{  1037,  1037,  "EUROPE"    },
	{  1038,  1038,  "ASIA"      },
	{  1039,  1039,  "CARIB"     },
	{  1040,  1040,  "MIDEAST"   },
	{  1041,  1041,  "ASIA"      },
	{  1042,  1042,  "CARIB"     },
	{  1043,  1044,  "EUROPE"    },
	{  1045,  1045,  "CAMER"     },
	{  1046,  1046,  "AFRICA"    },
	{  1047,  1047,  "ATLANTIC"  },
	{  1048,  1048,  "ASIA"      },
	{  1049,  1049,  "SAMER"     },
	{  1050,  1050,  "EUROPE"    },
	{  1051,  1051,  "AFRICA"    },
	{  1052,  1052,  "ATLANTIC"  },
	{  1053,  1053,  "SAMER"     },
	{  1054,  1054,  "INDIAN"    },
	{  1055,  1055,  "ASIA"      },
	{  1056,  1056,  "EUROPE"    },
	{  1057,  1058,  "AFRICA"    },
	{  1059,  1059,  "ASIA"      },
	{  1060,  1060,  "AFRICA"    },
	{  1061,  1061,  "CANADA"    },
	{  1062,  1062,  "ATLANTIC"  },
	{  1063,  1063,  "CARIB"     },
	{  1064,  1065,  "AFRICA"    },
	{  1066,  1066,  "SAMER"     },
	{  1067,  1067,  "ASIA"      },
	{  1068,  1068,  "PACIFIC"   },
	{  1069,  1069,  "INDIAN"    },
	{  1070,  1070,  "SAMER"     },
	{  1071,  1071,  "INDIAN"    },
	{  1072,  1072,  "AFRICA"    },
	{  1073,  1073,  "PACIFIC"   },
	{  1074,  1074,  "CAMER"     },
	{  1075,  1075,  "AFRICA"    },
	{  1076,  1076,  "EUROPE"    },
	{  1077,  1077,  "CARIB"     },
	{  1078,  1080,  "EUROPE"    },
	{  1081,  1081,  "AFRICA"    },
	{  1082,  1083,  "CARIB"     },
	{  1084,  1084,  "ASIA"      },
	{  1085,  1085,  "SAMER"     },
	{  1086,  1086,  "AFRICA"    },
	{  1087,  1087,  "CAMER"     },
	{  1088,  1089,  "AFRICA"    },
	{  1090,  1090,  "EUROPE"    },
	{  1091,  1091,  "AFRICA"    },
	{  1092,  1092,  "SAMER"     },
	{  1093,  1093,  "EUROPE"    },
	{  1094,  1094,  "PACIFIC"   },
	{  1095,  1096,  "EUROPE"    },
	{  1097,  1097,  "AFRICA"    },
	{  1098,  1099,  "PACIFIC"   },
	{  1100,  1101,  "AFRICA"    },
	{  1102,  1102,  "ASIA"      },
	{  1103,  1103,  "EUROPE"    },
	{  1104,  1104,  "AFRICA"    },
	{  1105,  1106,  "EUROPE"    },
	{  1107,  1107,  "ATLANTIC"  },
	{  1108,  1109,  "CARIB"     },
	{  1110,  1110,  "PACIFIC"   },
	{  1111,  1111,  "CAMER"     },
	{  1112,  1113,  "AFRICA"    },
	{  1114,  1114,  "SAMER"     },
	{  1115,  1115,  "CARIB"     },
	{  1116,  1116,  "INDIAN"    },
	{  1117,  1117,  "CAMER"     },
	{  1118,  1118,  "ASIA"      },
	{  1119,  1120,  "EUROPE"    },
	{  1121,  1122,  "ASIA"      },
	{  1123,  1124,  "MIDEAST"   },
	{  1125,  1125,  "EUROPE"    },
	{  1126,  1126,  "MIDEAST"   },
	{  1127,  1127,  "EUROPE"    },
	{  1128,  1128,  "CARIB"     },
	{  1129,  1129,  "ASIA"      },
	{  1130,  1130,  "MIDEAST"   },
	{  1131,  1131,  "ASIA"      },
	{  1132,  1132,  "AFRICA"    },
	{  1133,  1133,  "PACIFIC"   },
	{  1134,  1135,  "ASIA"      },
	{  1136,  1136,  "MIDEAST"   },
	{  1137,  1138,  "ASIA"      },
	{  1139,  1139,  "EUROPE"    },
	{  1140,  1140,  "MIDEAST"   },
	{  1141,  1143,  "AFRICA"    },
	{  1144,  1146,  "EUROPE"    },
	{  1147,  1147,  "ASIA"      },
	{  1148,  1148,  "EUROPE"    },
	{  1149,  1149,  "INDIAN"    },
	{  1150,  1150,  "AFRICA"    },
	{  1151,  1151,  "ASIA"      },
	{  1152,  1152,  "INDIAN"    },
	{  1153,  1153,  "AFRICA"    },
	{  1154,  1154,  "EUROPE"    },
	{  1155,  1155,  "PACIFIC"   },
	{  1156,  1156,  "CARIB"     },
	{  1157,  1159,  "AFRICA"    },
	{  1160,  1160,  "CAMER"     },
	{  1161,  1161,  "PACIFIC"   },
	{  1162,  1163,  "EUROPE"    },
	{  1164,  1164,  "ASIA"      },
	{  1165,  1165,  "CARIB"     },
	{  1166,  1166,  "EUROPE"    },
	{  1167,  1167,  "AFRICA"    },
	{  1168,  1168,  "ASIA"      },
	{  1169,  1169,  "AFRICA"    },
	{  1170,  1170,  "PACIFIC"   },
	{  1171,  1171,  "ASIA"      },
	{  1172,  1172,  "EUROPE"    },
	{  1173,  1173,  "CARIB"     },
	{  1174,  1174,  "PACIFIC"   },
	{  1175,  1175,  "AUSNZ"     },
	{  1176,  1176,  "CAMER"     },
	{  1177,  1178,  "AFRICA"    },
	{  1179,  1179,  "NONE"      },
	{  1180,  1180,  "AUSNZ"     },
	{  1181,  1181,  "PACIFIC"   },
	{  1182,  1182,  "EUROPE"    },
	{  1183,  1183,  "MIDEAST"   },
	{  1184,  1184,  "ASIA"      },
	{  1185,  1185,  "PACIFIC"   },
	{  1186,  1186,  "CAMER"     },
	{  1187,  1187,  "PACIFIC"   },
	{  1188,  1189,  "SAMER"     },
	{  1190,  1191,  "PACIFIC"   },
	{  1192,  1193,  "EUROPE"    },
	{  1194,  1194,  "CARIB"     },
	{  1195,  1195,  "MIDEAST"   },
	{  1196,  1196,  "INDIAN"    },
	{  1197,  1198,  "EUROPE"    },
	{  1199,  1199,  "AFRICA"    },
	{  1200,  1202,  "CARIB"     },
	{  1203,  1203,  "PACIFIC"   },
	{  1204,  1204,  "EUROPE"    },
	{  1205,  1205,  "AFRICA"    },
	{  1206,  1206,  "MIDEAST"   },
	{  1207,  1207,  "AFRICA"    },
	{  1208,  1208,  "INDIAN"    },
	{  1209,  1209,  "AFRICA"    },
	{  1210,  1210,  "ASIA"      },
	{  1211,  1212,  "EUROPE"    },
	{  1213,  1213,  "PACIFIC"   },
	{  1214,  1215,  "AFRICA"    },
	{  1216,  1216,  "PACIFIC"   },
	{  1217,  1217,  "EUROPE"    },
	{  1218,  1218,  "ASIA"      },
	{  1219,  1219,  "ATLANTIC"  },
	{  1220,  1220,  "CANADA"    },
	{  1221,  1221,  "AFRICA"    },
	{  1222,  1222,  "SAMER"     },
	{  1223,  1223,  "EUROPE"    },
	{  1224,  1224,  "AFRICA"    },
	{  1225,  1226,  "EUROPE"    },
	{  1227,  1227,  "MIDEAST"   },
	{  1228,  1228,  "PACIFIC"   },
	{  1229,  1229,  "ASIA"      },
	{  1230,  1230,  "AFRICA"    },
	{  1231,  1231,  "ASIA"      },
	{  1232,  1232,  "AFRICA"    },
	{  1233,  1234,  "PACIFIC"   },
	{  1235,  1235,  "CARIB"     },
	{  1236,  1236,  "AFRICA"    },
	{  1237,  1237,  "EUROPE"    },
	{  1238,  1238,  "ASIA"      },
	{  1239,  1239,  "CARIB"     },
	{  1240,  1240,  "PACIFIC"   },
	{  1241,  1241,  "AFRICA"    },
	{  1242,  1242,  "ASIA"      },
	{  1243,  1243,  "MIDEAST"   },
	{  1244,  1244,  "EUROPE"    },
	{  1245,  1246,  "OTHR-US"   },
	{  1247,  1247,  "SAMER"     },
	{  1248,  1248,  "ASIA"      },
	{  1249,  1249,  "PACIFIC"   },
	{  1250,  1250,  "EUROPE"    },
	{  1251,  1251,  "SAMER"     },
	{  1252,  1252,  "ASIA"      },
	{  1253,  1254,  "CARIB"     },
	{  1255,  1255,  "PACIFIC"   },
	{  1256,  1256,  "AFRICA"    },
	{  1257,  1257,  "MIDEAST"   },
	{  1258,  1258,  "EUROPE"    },
	{  1259,  1261,  "AFRICA"    },
	{  1262,  1262,  "WORLD"     },
	{  1263,  1263,  "NONE"      },
	{  1264,  1264,  "EUROPE"    },
	{  1265,  1270,  "SAMER"     },
	{  1271,  1271,  "AFRICA"    },
	{  1272,  1272,  "ASIA"      },
	{  1273,  1273,  "CARIB"     },
	{  1274,  1274,  "SAMER"     },
	{  1275,  1275,  "EUROPE"    },
	{  1276,  1277,  "AFRICA"    },
	{  1278,  1278,  "POLAR"     },
	{  1279,  1282,  "AUSNZ"     },
	{  1283,  1283,  "CANADA"    },
	{  1284,  1284,  "EUROPE"    },
	{  1285,  1285,  "ASIA"      },
	{  1286,  1286,  "EUROPE"    },
	{  1287,  1287,  "ASIA"      },
	{  1288,  1288,  "AFRICA"    },
	{  1289,  1289,  "CANADA"    },
	{  1290,  1290,  "AFRICA"    },
	{  1291,  1291,  "ASIA"      },
	{  1292,  1293,  "SAMER"     },
	{  1294,  1299,  "EUROPE"    },
	{  1300,  1300,  "MIDEAST"   },
	{  1301,  1301,  "EUROPE"    },
	{  1302,  1302,  "ASIA"      },
	{  1303,  1303,  "SAMER"     },
	{  1304,  1304,  "ASIA"      },
	{  1305,  1306,  "EUROPE"    },
	{  1307,  1309,  "ASIA"      },
	{  1310,  1310,  "MIDEAST"   },
	{  1311,  1313,  "SAMER"     },
	{  1314,  1314,  "EUROPE"    },
	{  1315,  1315,  "AFRICA"    },
	{  1316,  1316,  "ASIA"      },
	{  1317,  1318,  "AFRICA"    },
	{  1319,  1320,  "SAMER"     },
	{  1321,  1321,  "EUROPE"    },
	{  1322,  1322,  "CARIB"     },
	{  1323,  1324,  "OTHR-US"   },
	{  1325,  1325,  "OTHR-NA"   },
	{  1326,  1327,  "EUROPE"    },
	{  1328,  1328,  "ASIA"      },
	{  1329,  1329,  "AFRICA"    },
	{  1330,  1334,  "OTHR-US"   },
	{  1335,  1335,  "CARIB"     },
	{  1336,  1336,  "CANADA"    },
	{  1337,  1337,  "OTHR-US"   },
	{  1338,  1338,  "MIDEAST"   },
	{  1339,  1339,  "CARIB"     },
	{  1340,  1340,  "MIDEAST"   },
	{  1341,  1341,  "SAMER"     },
	{  1342,  1342,  "AFRICA"    },
	{  1343,  1345,  "EUROPE"    },
	{  1346,  1346,  "MIDEAST"   },
	{  1347,  1347,  "EUROPE"    },
	{  1348,  1348,  "SAMER"     },
	{  1349,  1350,  "OTHR-NA"   },
	{  1351,  1351,  "MIDEAST"   },
	{  1352,  1354,  "EUROPE"    },
	{  1355,  1355,  "ASIA"      },
	{  1356,  1356,  "MIDEAST"   },
	{  1357,  1357,  "EUROPE"    },
	{  1358,  1358,  "SAMER"     },
	{  1359,  1360,  "ASIA"      },
	{  1361,  1361,  "AFRICA"    },
	{  1362,  1362,  "ASIA"      },
	{  1363,  1363,  "MIDEAST"   },
	{  1364,  1364,  "ASIA"      },
	{  1365,  1366,  "AFRICA"    },
	{  1367,  1368,  "CANADA"    },
	{  1369,  1369,  "EUROPE"    },
	{  1370,  1371,  "SAMER"     },
	{  1372,  1378,  "SPCS"      },
	{  1379,  1379,  "OTHR-US"   },
	{  1380,  1411,  "SPCS"      },
	{  1412,  1412,  "OTHR-US"   },
	{  1413,  1417,  "SPCS"      },
	{  1418,  1418,  "OTHR-US"   },
	{  1419,  1419,  "SPCS"      },
	{  1420,  1449,  "CANADA"    },
	{  1450,  1451,  "AFRICA"    },
	{  1452,  1453,  "ASIA"      },
	{  1454,  1463,  "AFRICA"    },
	{  1464,  1467,  "MIDEAST"   },
	{  1468,  1482,  "AFRICA"    },
	{  1483,  1486,  "SAMER"     },
	{  1487,  1488,  "CARIB"     },
	{  1489,  1489,  "AFRICA"    },
	{  1490,  1493,  "MIDEAST"   },
	{  1494,  1498,  "ASIA"      },
	{  1499,  1499,  "SAMER"     },
	{  1500,  1504,  "AUSNZ"     },
	{  1505,  1505,  "AFRICA"    },
	{  1506,  1508,  "CANADA"    },
	{  1509,  1510,  "AFRICA"    },
	{  1511,  1511,  "OTHR-US"   },
	{  1512,  1530,  "EUROPE"    },
	{  1531,  1535,  "CANADA"    },
	{  1536,  1539,  "EUROPE"    },
	{  1540,  1541,  "AFRICA"    },
	{  1542,  1542,  "ASIA"      },
	{  1544,  1545,  "MIDEAST"   },
	{  1546,  1550,  "OTHR-US"   },
	{  1551,  1551,  "CARIB"     },
	{  1552,  1555,  "AFRICA"    },
	{  1556,  1568,  "AUSNZ"     },
	{  1569,  1571,  "MIDEAST"   },
	{  1572,  1574,  "SAMER"     },
	{  1575,  1583,  "AFRICA"    },
	{  1584,  1597,  "ASIA"      },
	{  1598,  1603,  "SAMER"     },
	{  1604,  1607,  "AFRICA"    },
	{  1608,  1614,  "SAMER"     },
	{  1615,  1620,  "AFRICA"    },
	{  1621,  1622,  "SAMER"     },
	{  1623,  1623,  "MIDEAST"   },
	{  1624,  1641,  "EUROPE"    },
	{  1642,  1645,  "AFRICA"    },
	{  1646,  1646,  "EUROPE"    },
	{  1647,  1692,  "ASIA"      },
	{  1693,  1695,  "SAMER"     },
	{  1696,  1697,  "AFRICA"    },
	{  1698,  1702,  "PACIFIC"   },
	{  1703,  1703,  "AFRICA"    },
	{  1706,  1712,  "EUROPE"    },
	{  1713,  1717,  "AFRICA"    },
	{  1718,  1719,  "EUROPE"    },
	{  1720,  1725,  "OTHR-US"   },
	{  1726,  1726,  "AFRICA"    },
	{  1727,  1727,  "SAMER"     },
	{  1728,  1729,  "AFRICA"    },
	{  1730,  1730,  "OTHR-NA"   },
	{  1731,  1734,  "EUROPE"    },
	{  1735,  1738,  "AFRICA"    },
	{  1739,  1740,  "MIDEAST"   },
	{  1741,  1748,  "EUROPE"    },
	{  1749,  1750,  "MIDEAST"   },
	{  1751,  1762,  "SAMER"     },
	{  1763,  1767,  "EUROPE"    },
	{  1768,  1791,  "ASIA"      },
	{  1792,  1797,  "EUROPE"    },
	{  1798,  1804,  "ASIA"      },
	{  1805,  1805,  "EUROPE"    },
	{  1806,  1821,  "SAMER"     },
	{  1822,  1822,  "AFRICA"    },
	{  1823,  1837,  "SAMER"     },
	{  1838,  1849,  "AFRICA"    },
	{  1850,  1850,  "MIDEAST"   },
	{  1851,  1872,  "ASIA"      },
	{  1873,  1873,  "UTMN"      },
	{  1874,  1874,  "UTMS"      },
	{  1875,  1875,  "UTMN"      },
	{  1876,  1876,  "UTMS"      },
	{  1877,  1877,  "UTMN"      },
	{  1878,  1878,  "UTMS"      },
	{  1879,  1879,  "UTMN"      },
	{  1880,  1880,  "UTMS"      },
	{  1881,  1881,  "UTMN"      },
	{  1882,  1882,  "UTMS"      },
	{  1883,  1883,  "UTMN"      },
	{  1884,  1884,  "UTMS"      },
	{  1885,  1885,  "UTMN"      },
	{  1886,  1886,  "UTMS"      },
	{  1887,  1887,  "UTMN"      },
	{  1888,  1888,  "UTMS"      },
	{  1889,  1889,  "UTMN"      },
	{  1890,  1890,  "UTMS"      },
	{  1891,  1891,  "UTMN"      },
	{  1892,  1892,  "UTMS"      },
	{  1893,  1893,  "UTMN"      },
	{  1894,  1894,  "UTMS"      },
	{  1895,  1895,  "UTMN"      },
	{  1896,  1896,  "UTMS"      },
	{  1897,  1897,  "UTMN"      },
	{  1898,  1898,  "UTMS"      },
	{  1899,  1899,  "UTMN"      },
	{  1900,  1900,  "UTMS"      },
	{  1901,  1901,  "UTMN"      },
	{  1902,  1902,  "UTMS"      },
	{  1903,  1903,  "UTMN"      },
	{  1904,  1904,  "UTMS"      },
	{  1905,  1905,  "UTMN"      },
	{  1906,  1906,  "UTMS"      },
	{  1907,  1907,  "UTMN"      },
	{  1908,  1908,  "UTMS"      },
	{  1909,  1909,  "UTMN"      },
	{  1910,  1910,  "UTMS"      },
	{  1911,  1911,  "UTMN"      },
	{  1912,  1912,  "UTMS"      },
	{  1913,  1913,  "UTMN"      },
	{  1914,  1914,  "UTMS"      },
	{  1915,  1915,  "UTMN"      },
	{  1916,  1916,  "UTMS"      },
	{  1917,  1917,  "UTMN"      },
	{  1918,  1918,  "UTMS"      },
	{  1919,  1919,  "UTMN"      },
	{  1920,  1920,  "UTMS"      },
	{  1921,  1921,  "UTMN"      },
	{  1922,  1922,  "UTMS"      },
	{  1923,  1923,  "UTMN"      },
	{  1924,  1924,  "UTMS"      },
	{  1925,  1925,  "UTMN"      },
	{  1926,  1926,  "UTMS"      },
	{  1927,  1927,  "UTMN"      },
	{  1928,  1928,  "UTMS"      },
	{  1929,  1929,  "UTMN"      },
	{  1930,  1930,  "UTMS"      },
	{  1931,  1931,  "UTMN"      },
	{  1932,  1932,  "UTMS"      },
	{  1933,  1933,  "UTMN"      },
	{  1934,  1934,  "UTMS"      },
	{  1935,  1935,  "UTMN"      },
	{  1936,  1936,  "UTMS"      },
	{  1937,  1937,  "UTMN"      },
	{  1938,  1938,  "UTMS"      },
	{  1939,  1939,  "UTMN"      },
	{  1940,  1940,  "UTMS"      },
	{  1941,  1941,  "UTMN"      },
	{  1942,  1942,  "UTMS"      },
	{  1943,  1943,  "UTMN"      },
	{  1944,  1944,  "UTMS"      },
	{  1945,  1945,  "UTMN"      },
	{  1946,  1946,  "UTMS"      },
	{  1947,  1947,  "UTMN"      },
	{  1948,  1948,  "UTMS"      },
	{  1949,  1949,  "UTMN"      },
	{  1950,  1950,  "UTMS"      },
	{  1951,  1951,  "UTMN"      },
	{  1952,  1952,  "UTMS"      },
	{  1953,  1953,  "UTMN"      },
	{  1954,  1954,  "UTMS"      },
	{  1955,  1955,  "UTMN"      },
	{  1956,  1956,  "UTMS"      },
	{  1957,  1957,  "UTMN"      },
	{  1958,  1958,  "UTMS"      },
	{  1959,  1959,  "UTMN"      },
	{  1960,  1960,  "UTMS"      },
	{  1961,  1961,  "UTMN"      },
	{  1962,  1962,  "UTMS"      },
	{  1963,  1963,  "UTMN"      },
	{  1964,  1964,  "UTMS"      },
	{  1965,  1965,  "UTMN"      },
	{  1966,  1966,  "UTMS"      },
	{  1967,  1967,  "UTMN"      },
	{  1968,  1968,  "UTMS"      },
	{  1969,  1969,  "UTMN"      },
	{  1970,  1970,  "UTMS"      },
	{  1971,  1971,  "UTMN"      },
	{  1972,  1972,  "UTMS"      },
	{  1973,  1973,  "UTMN"      },
	{  1974,  1974,  "UTMS"      },
	{  1975,  1975,  "UTMN"      },
	{  1976,  1976,  "UTMS"      },
	{  1977,  1977,  "UTMN"      },
	{  1978,  1978,  "UTMS"      },
	{  1979,  1979,  "UTMN"      },
	{  1980,  1980,  "UTMS"      },
	{  1981,  1981,  "UTMN"      },
	{  1982,  1982,  "UTMS"      },
	{  1983,  1983,  "UTMN"      },
	{  1984,  1984,  "UTMS"      },
	{  1985,  1985,  "UTMN"      },
	{  1986,  1986,  "UTMS"      },
	{  1987,  1987,  "UTMN"      },
	{  1988,  1988,  "UTMS"      },
	{  1989,  1989,  "UTMN"      },
	{  1990,  1990,  "UTMS"      },
	{  1991,  1991,  "UTMN"      },
	{  1992,  1992,  "UTMS"      },
	{  1993,  1994,  "UTMN"      },
	{  1995,  1995,  "UTMS"      },
	{  1996,  1997,  "POLAR"     },
	{  1998,  1998,  "UTMN"      },
	{  1999,  1999,  "UTMS"      },
	{  2000,  2000,  "UTMN"      },
	{  2001,  2001,  "UTMS"      },
	{  2002,  2002,  "UTMN"      },
	{  2003,  2003,  "UTMS"      },
	{  2004,  2004,  "UTMN"      },
	{  2005,  2005,  "UTMS"      },
	{  2006,  2006,  "UTMN"      },
	{  2007,  2007,  "UTMS"      },
	{  2008,  2008,  "UTMN"      },
	{  2009,  2009,  "UTMS"      },
	{  2010,  2010,  "UTMN"      },
	{  2011,  2011,  "UTMS"      },
	{  2012,  2012,  "UTMN"      },
	{  2013,  2013,  "UTMS"      },
	{  2014,  2014,  "UTMN"      },
	{  2015,  2015,  "UTMS"      },
	{  2016,  2016,  "UTMN"      },
	{  2017,  2017,  "UTMS"      },
	{  2018,  2018,  "UTMN"      },
	{  2019,  2019,  "UTMS"      },
	{  2020,  2020,  "UTMN"      },
	{  2021,  2021,  "UTMS"      },
	{  2022,  2022,  "UTMN"      },
	{  2023,  2023,  "UTMS"      },
	{  2024,  2024,  "UTMN"      },
	{  2025,  2025,  "UTMS"      },
	{  2026,  2026,  "UTMN"      },
	{  2027,  2027,  "UTMS"      },
	{  2028,  2028,  "UTMN"      },
	{  2029,  2029,  "UTMS"      },
	{  2030,  2030,  "UTMN"      },
	{  2031,  2031,  "UTMS"      },
	{  2032,  2032,  "UTMN"      },
	{  2033,  2033,  "UTMS"      },
	{  2034,  2034,  "UTMN"      },
	{  2035,  2035,  "UTMS"      },
	{  2036,  2036,  "UTMN"      },
	{  2037,  2037,  "UTMS"      },
	{  2038,  2038,  "UTMN"      },
	{  2039,  2039,  "UTMS"      },
	{  2040,  2040,  "UTMN"      },
	{  2041,  2041,  "UTMS"      },
	{  2042,  2042,  "UTMN"      },
	{  2043,  2043,  "UTMS"      },
	{  2044,  2044,  "UTMN"      },
	{  2045,  2045,  "UTMS"      },
	{  2046,  2046,  "UTMN"      },
	{  2047,  2047,  "UTMS"      },
	{  2048,  2048,  "UTMN"      },
	{  2049,  2049,  "UTMS"      },
	{  2050,  2050,  "UTMN"      },
	{  2051,  2051,  "UTMS"      },
	{  2052,  2052,  "UTMN"      },
	{  2053,  2053,  "UTMS"      },
	{  2054,  2054,  "UTMN"      },
	{  2055,  2055,  "UTMS"      },
	{  2056,  2056,  "UTMN"      },
	{  2057,  2057,  "UTMS"      },
	{  2058,  2058,  "UTMN"      },
	{  2059,  2059,  "UTMS"      },
	{  2060,  2060,  "UTMN"      },
	{  2061,  2061,  "UTMS"      },
	{  2062,  2062,  "UTMN"      },
	{  2063,  2063,  "UTMS"      },
	{  2064,  2064,  "UTMN"      },
	{  2065,  2065,  "UTMS"      },
	{  2066,  2066,  "UTMN"      },
	{  2067,  2067,  "UTMS"      },
	{  2068,  2068,  "UTMN"      },
	{  2069,  2069,  "UTMS"      },
	{  2070,  2070,  "UTMN"      },
	{  2071,  2071,  "UTMS"      },
	{  2072,  2072,  "UTMN"      },
	{  2073,  2073,  "UTMS"      },
	{  2074,  2074,  "UTMN"      },
	{  2075,  2075,  "UTMS"      },
	{  2076,  2076,  "UTMN"      },
	{  2077,  2077,  "UTMS"      },
	{  2078,  2078,  "UTMN"      },
	{  2079,  2079,  "UTMS"      },
	{  2080,  2080,  "UTMN"      },
	{  2081,  2081,  "UTMS"      },
	{  2082,  2082,  "UTMN"      },
	{  2083,  2083,  "UTMS"      },
	{  2084,  2084,  "UTMN"      },
	{  2085,  2085,  "UTMS"      },
	{  2086,  2086,  "UTMN"      },
	{  2087,  2087,  "UTMS"      },
	{  2088,  2088,  "UTMN"      },
	{  2089,  2089,  "UTMS"      },
	{  2090,  2090,  "UTMN"      },
	{  2091,  2091,  "UTMS"      },
	{  2092,  2092,  "UTMN"      },
	{  2093,  2093,  "UTMS"      },
	{  2094,  2094,  "UTMN"      },
	{  2095,  2095,  "UTMS"      },
	{  2096,  2096,  "UTMN"      },
	{  2097,  2097,  "UTMS"      },
	{  2098,  2098,  "UTMN"      },
	{  2099,  2099,  "UTMS"      },
	{  2100,  2100,  "UTMN"      },
	{  2101,  2101,  "UTMS"      },
	{  2102,  2102,  "UTMN"      },
	{  2103,  2103,  "UTMS"      },
	{  2104,  2104,  "UTMN"      },
	{  2105,  2105,  "UTMS"      },
	{  2106,  2106,  "UTMN"      },
	{  2107,  2107,  "UTMS"      },
	{  2108,  2108,  "UTMN"      },
	{  2109,  2109,  "UTMS"      },
	{  2110,  2110,  "UTMN"      },
	{  2111,  2111,  "UTMS"      },
	{  2112,  2112,  "UTMN"      },
	{  2113,  2113,  "UTMS"      },
	{  2114,  2114,  "UTMN"      },
	{  2115,  2115,  "UTMS"      },
	{  2116,  2116,  "UTMN"      },
	{  2117,  2117,  "UTMS"      },
	{  2118,  2118,  "UTMN"      },
	{  2119,  2119,  "UTMS"      },
	{  2120,  2121,  "CAMER"     },
	{  2122,  2153,  "UTMN"      },
	{  2154,  2155,  "SPCS"      },
	{  2156,  2165,  "OTHR-US"   },
	{  2166,  2170,  "SPCS"      },
	{  2171,  2174,  "CARIB"     },
	{  2175,  2225,  "SPCS"      },
	{  2226,  2227,  "CANADA"    },
	{  2228,  2250,  "SPCS"      },
	{  2251,  2251,  "CARIB"     },
	{  2252,  2274,  "SPCS"      },
	{  2275,  2281,  "CANADA"    },
	{  2282,  2282,  "AFRICA"    },
	{  2283,  2287,  "AUSNZ"     },
	{  2288,  2289,  "PACIFIC"   },
	{  2290,  2290,  "CANADA"    },
	{  2291,  2291,  "AUSNZ"     },
	{  2292,  2293,  "ASIA"      },
	{  2294,  2294,  "MIDEAST"   },
	{  2295,  2295,  "CARIB"     },
	{  2296,  2296,  "AFRICA"    },
	{  2297,  2298,  "OTHR-US"   },
	{  2299,  2306,  "EUROPE"    },
	{  2307,  2310,  "SAMER"     },
	{  2311,  2312,  "AFRICA"    },
	{  2313,  2313,  "CANADA"    },
	{  2314,  2314,  "ASIA"      },
	{  2315,  2315,  "SAMER"     },
	{  2316,  2324,  "AFRICA"    },
	{  2325,  2325,  "SAMER"     },
	{  2326,  2326,  "EUROPE"    },
	{  2327,  2329,  "MIDEAST"   },
	{  2330,  2340,  "EUROPE"    },
	{  2341,  2341,  "AFRICA"    },
	{  2342,  2344,  "EUROPE"    },
	{  2345,  2345,  "MIDEAST"   },
	{  2346,  2346,  "WORLD"     },
	{  2347,  2347,  "AFRICA"    },
	{  2348,  2349,  "ASIA"      },
	{  2350,  2353,  "AFRICA"    },
	{  2354,  2354,  "ASIA"      },
	{  2355,  2357,  "SAMER"     },
	{  2358,  2361,  "ASIA"      },
	{  2362,  2362,  "MIDEAST"   },
	{  2363,  2363,  "SAMER"     },
	{  2364,  2365,  "PACIFIC"   },
	{  2366,  2368,  "EUROPE"    },
	{  2369,  2369,  "INDIAN"    },
	{  2370,  2370,  "EUROPE"    },
	{  2371,  2371,  "AFRICA"    },
	{  2372,  2372,  "EUROPE"    },
	{  2373,  2374,  "OTHR-US"   },
	{  2375,  2376,  "CANADA"    },
	{  2377,  2383,  "OTHR-US"   },
	{  2384,  2384,  "CANADA"    },
	{  2385,  2385,  "CAMER"     },
	{  2386,  2386,  "ATLANTIC"  },
	{  2387,  2390,  "OTHR-US"   },
	{  2391,  2392,  "MIDEAST"   },
	{  2393,  2393,  "AFRICA"    },
	{  2394,  2398,  "EUROPE"    },
	{  2399,  2403,  "SAMER"     },
	{  2404,  2404,  "MIDEAST"   },
	{  2405,  2405,  "ASIA"      },
	{  2406,  2406,  "MIDEAST"   },
	{  2407,  2407,  "ATLANTIC"  },
	{  2408,  2409,  "ASIA"      },
	{  2410,  2410,  "CANADA"    },
	{  2411,  2411,  "ASIA"      },
	{  2412,  2412,  "OTHR-US"   },
	{  2413,  2414,  "CARIB"     },
	{  2415,  2417,  "CANADA"    },
	{  2418,  2418,  "CARIB"     },
	{  2419,  2419,  "CAMER"     },
	{  2420,  2421,  "EUROPE"    },
	{  2424,  2424,  "OTHR-US"   },
	{  2425,  2526,  "ASIA"      },
	{  2527,  2529,  "SPCS"      },
	{  2530,  2547,  "EUROPE"    },
	{  2548,  2555,  "AFRICA"    },
	{  2556,  2573,  "ATLANTIC"  },
	{  2574,  2574,  "AFRICA"    },
	{  2575,  2576,  "AUSNZ"     },
	{  2577,  2589,  "ASIA"      },
	{  2590,  2591,  "AFRICA"    },
	{  2592,  2594,  "ASIA"      },
	{  2595,  2595,  "AFRICA"    },
	{  2596,  2597,  "SAMER"     },
	{  2598,  2600,  "AFRICA"    },
	{  2601,  2601,  "EUROPE"    },
	{  2602,  2603,  "MIDEAST"   },
	{  2604,  2652,  "ASIA"      },
	{  2653,  2663,  "EUROPE"    },
	{  2664,  2731,  "ASIA"      },
	{  2732,  2746,  "NONE"      },
	{  2747,  2756,  "EUROPE"    },
	{  2757,  2770,  "ASIA"      },
	{  2771,  2771,  "AFRICA"    },
	{  2772,  2778,  "ASIA"      },
	{  2779,  2779,  "EUROPE"    },
	{  2780,  2780,  "ASIA"      },
	{  2781,  2783,  "MIDEAST"   },
	{  2784,  2784,  "CANADA"    },
	{  2785,  2791,  "AFRICA"    },
	{  2792,  2804,  "EUROPE"    },
	{  2805,  2805,  "SAMER"     },
	{  2806,  2806,  "MIDEAST"   },
	{  2807,  2809,  "INDIAN"    },
	{  2810,  2816,  "PACIFIC"   },
	{  2817,  2818,  "POLAR"     },
	{  2819,  2823,  "PACIFIC"   },
	{  2824,  2824,  "CARIB"     },
	{  2825,  2827,  "AFRICA"    },
	{  2828,  2829,  "CARIB"     },
	{  2830,  2830,  "WORLD"     },
	{  2831,  2832,  "CANADA"    },
	{  2833,  2875,  "EUROPE"    },
	{  2876,  2876,  "ASIA"      },
	{  2879,  2879,  "EUROPE"    },
	{  2880,  2880,  "POLAR"     },
	{  2881,  2888,  "EUROPE"    },
	{  2889,  2889,  "AUSNZ"     },
	{  2890,  2895,  "CARIB"     },
	{  2896,  2897,  "ASIA"      },
	{  2898,  2898,  "EUROPE"    },
	{  2899,  2947,  "AUSNZ"     },
	{  2948,  2950,  "OTHR_US"   },
	{  2951,  2955,  "ASIA"      },
	{  2956,  2957,  "MIDEAST"   },
	{  2958,  2960,  "OTHR-US"   },
	{  2961,  2961,  "EUROPE"    },
	{  2962,  2966,  "SAMER"     },
	{  2967,  2972,  "AFRICA"    },
	{  2973,  2980,  "OTHR-US"   },
	{  2981,  2981,  "AFRICA"    },
	{  2982,  2985,  "ASIA"      },
	{  2986,  2986,  "AUSNZ"     },
	{  2987,  2987,  "AFRICA"    },
	{  2988,  2989,  "EUROPE"    },
	{  2990,  2990,  "AUSNZ"     },
	{  2991,  3081,  "POLAR"     },
	{  3082,  3091,  "SAMER"     },
	{  3092,  3104,  "EUROPE"    },
	{  3105,  3105,  "SAMER"     },
	{  3106,  3107,  "MIDEAST"   },
	{  3108,  3110,  "PACIFIC"   },
	{  3111,  3111,  "EUROPE"    },
	{  3112,  3112,  "SAMER"     },
	{  3113,  3113,  "AFRICA"    },
	{  3114,  3117,  "OTHR-NA"   },
	{  3118,  3118,  "CARIB"     },
	{  3119,  3119,  "EUROPE"    },
	{  3120,  3138,  "PACIFIC"   },
	{  3139,  3139,  "AUSNZ"     },
	{  3140,  3141,  "MIDEAST"   },
	{  3142,  3142,  "AFRICA"    },
	{  3143,  3143,  "CARIB"     },
	{  3144,  3146,  "SAMER"     },
	{  3147,  3167,  "AFRICA"    },
	{  3168,  3170,  "EUROPE"    },
	{  3171,  3171,  "AFRICA"    },
	{  3172,  3172,  "PACIFIC"   },
	{  3173,  3175,  "EUROPE"    },
	{  3176,  3178,  "SAMER"     },
	{  3179,  3180,  "AFRICA"    },
	{  3181,  3181,  "OTHR-US"   },
	{  3182,  3184,  "ATLANTIC"  },
	{  3185,  3186,  "CARIB"     },
	{  3187,  3187,  "ATLANTIC"  },
	{  3188,  3188,  "PACIFIC"   },
	{  3189,  3189,  "INDIAN"    },
	{  3190,  3198,  "PACIFIC"   },
	{  3199,  3199,  "EUROPE"    },
	{  3200,  3200,  "ASIA"      },
	{  3201,  3202,  "PACIFIC"   },
	{  3203,  3203,  "ASIA"      },
	{  3204,  3205,  "POLAR"     },
	{  3206,  3206,  "OTHR-NA"   },
	{  3207,  3207,  "CARIB"     },
	{  3208,  3209,  "PACIFIC"   },
	{  3210,  3212,  "EUROPE"    },
	{  3213,  3213,  "AFRICA"    },
	{  3214,  3214,  "CARIB"     },
	{  3215,  3215,  "SAMER"     },
	{  3216,  3216,  "CARIB"     },
	{  3217,  3217,  "ASIA"      },
	{  3218,  3218,  "CARIB"     },
	{  3219,  3219,  "CAMER"     },
	{  3220,  3220,  "AFRICA"    },
	{  3221,  3222,  "ATLANTIC"  },
	{  3223,  3223,  "SAMER"     },
	{  3224,  3224,  "EUROPE"    },
	{  3225,  3225,  "ASIA"      },
	{  3226,  3226,  "AFRICA"    },
	{  3227,  3227,  "SAMER"     },
	{  3228,  3228,  "ASIA"      },
	{  3229,  3229,  "SAMER"     },
	{  3230,  3231,  "AFRICA"    },
	{  3232,  3232,  "CAMER"     },
	{  3233,  3233,  "AFRICA"    },
	{  3234,  3234,  "EUROPE"    },
	{  3235,  3235,  "CARIB"     },
	{  3236,  3236,  "EUROPE"    },
	{  3237,  3237,  "EUROPE"    },
	{  3238,  3238,  "AFRICA"    },
	{  3239,  3240,  "CARIB"     },
	{  3241,  3241,  "SAMER"     },
	{  3242,  3242,  "AFRICA"    },
	{  3243,  3243,  "CAMER"     },
	{  3244,  3244,  "AFRICA"    },
	{  3245,  3245,  "AFRICA"    },
	{  3246,  3246,  "EUROPE"    },
	{  3247,  3247,  "SAMER"     },
	{  3248,  3248,  "EUROPE"    },
	{  3249,  3250,  "AFRICA"    },
	{  3251,  3251,  "ASIA"      },
	{  3252,  3252,  "AFRICA"    },
	{  3253,  3254,  "EUROPE"    },
	{  3255,  3255,  "PACIFIC"   },
	{  3256,  3256,  "CAMER"     },
	{  3257,  3258,  "AFRICA"    },
	{  3259,  3259,  "SAMER"     },
	{  3260,  3260,  "CARIB"     },
	{  3261,  3261,  "CAMER"     },
	{  3262,  3262,  "EUROPE"    },
	{  3263,  3263,  "EUROPE"    },
	{  3264,  3264,  "AFRICA"    },
	{  3265,  3266,  "ASIA"      },
	{  3267,  3267,  "MIDEAST"   },
	{  3268,  3268,  "EUROPE"    },
	{  3269,  3269,  "MIDEAST"   },
	{  3270,  3271,  "AFRICA"    },
	{  3272,  3272,  "EUROPE"    },
	{  3273,  3274,  "INDIAN"    },
	{  3275,  3275,  "EUROPE"    },
	{  3276,  3276,  "CARIB"     },
	{  3277,  3277,  "AFRICA"    },
	{  3278,  3278,  "CAMER"     },
	{  3279,  3279,  "CARIB"     },
	{  3280,  3281,  "AFRICA"    },
	{  3282,  3282,  "ASIA"      },
	{  3283,  3283,  "AFRICA"    },
	{  3284,  3284,  "EUROPE"    },
	{  3285,  3285,  "AUSNZ"     },
	{  3286,  3286,  "CAMER"     },
	{  3287,  3287,  "AFRICA"    },
	{  3288,  3288,  "MIDEAST"   },
	{  3289,  3289,  "ASIA"      },
	{  3290,  3290,  "CAMER"     },
	{  3291,  3291,  "ASIA"      },
	{  3292,  3292,  "SAMER"     },
	{  3293,  3293,  "EUROPE"    },
	{  3294,  3294,  "CARIB"     },
	{  3295,  3295,  "EUROPE"    },
	{  3296,  3296,  "ASIA"      },
	{  3297,  3298,  "CARIB"     },
	{  3299,  3299,  "CANADA"    },
	{  3300,  3300,  "CARIB"     },
	{  3301,  3301,  "PACIFIC"   },
	{  3302,  3302,  "AFRICA"    },
	{  3303,  3303,  "MIDEAST"   },
	{  3304,  3304,  "AFRICA"    },
	{  3305,  3305,  "EUROPE"    },
	{  3306,  3306,  "AFRICA"    },
	{  3307,  3307,  "EUROPE"    },
	{  3308,  3308,  "AFRICA"    },
	{  3309,  3309,  "AFRICA"    },
	{  3310,  3310,  "ASIA"      },
	{  3311,  3311,  "AFRICA"    },
	{  3312,  3312,  "SAMER"     },
	{  3313,  3313,  "EUROPE"    },
	{  3314,  3314,  "MIDEAST"   },
	{  3315,  3315,  "ASIA"      },
	{  3316,  3316,  "AFRICA"    },
	{  3317,  3317,  "ASIA"      },
	{  3318,  3318,  "ASIA"      },
	{  3319,  3319,  "AFRICA"    },
	{  3320,  3321,  "PACIFIC"   },
	{  3322,  3322,  "ASIA"      },
	{  3323,  3323,  "CARIB"     },
	{  3324,  3324,  "EUROPE"    },
	{  3325,  3325,  "MIDEAST"   },
	{  3326,  3326,  "SAMER"     },
	{  3327,  3327,  "SAMER"     },
	{  3328,  3328,  "ASIA"      },
	{  3329,  3330,  "CARIB"     },
	{  3331,  3331,  "AFRICA"    },
	{  3332,  3332,  "MIDEAST"   },
	{  3333,  3333,  "EUROPE"    },
	{  3334,  3335,  "ASIA"      },
	{  3336,  3336,  "MIDEAST"   },
	{  3337,  3337,  "INDIAN"    },
	{  3338,  3338,  "AUSNZ"     },
	{  3339,  3339,  "EUROPE"    },
	{  3340,  3340,  "INDIAN"    },
	{  3341,  3341,  "ASIA"      },
	{  3342,  3342,  "CARIB"     },
	{  3343,  3343,  "EUROPE"    },
	{  3344,  3344,  "AUSNZ"     },
	{  3355,  3356,  "SAMER"     },
	{  3357,  3360,  "OTHR-US"   },
	{  3361,  3361,  "CAMER"     },
	{  3362,  3371,  "EUROPE"    },
	{  3372,  3375,  "OTHR-US"   },
	{  3372,  3375,  "OTHR-US"   },
	{  3376,  3384,  "ASIA"      },
	{  3385,  3385,  "EUROPE"    },
	{  3387,  3389,  "MIDEAST"   },
	{  3387,  3390,  "MIDEAST"   },
	{  3391,  3391,  "WORLD"     },
	{  3392,  3396,  "EUROPE"    },
	{  3397,  3397,  "MIDEAST"   },
	{  3398,  3401,  "PACIFIC"   },
	{  3402,  3402,  "AFRICA"    },
	{  3402,  3402,  "AFRICA"    },
	{  3403,  3403,  "ASIA"      },
	{  3404,  3407,  "CANADA"    },
	{  3408,  3408,  "EUROPE"    },
	{  3409,  3417,  "CANADA"    },
	{  3418,  3418,  "SAMER"     },
	{  3419,  3420,  "OTHR-NA"   },
	{  3421,  3422,  "SAMER"     },
	{  3423,  3426,  "CAMER"     },
	{  3427,  3428,  "SAMER"     },
	{  3429,  3429,  "EUROPE"    },
	{  3430,  3435,  "PACIFIC"   },
	{  3436,  3448,  "SAMER"     },
	{  3449,  3460,  "EUROPE"    },
	{  3461,  3462,  "CAMER"     },
	{  3463,  3463,  "WORLD"     },
	{  3464,  3465,  "AFRICA"    },
	{     0,     0,  ""          }
};

//=============================================================================
// Stand alone functions.  Will work without a TcsEpsgdataSetV6 object, but
// results would not have much use without one.
const wchar_t* GetEpsgTableName (EcsEpsgTable tblId)
{
	const wchar_t* namePtr = 0;
	const TcsEpsgTblMap* tblPtr;
	
	for (tblPtr = KcsEpsgTblMap;tblPtr->TableId != epsgTblUnknown;++tblPtr)
	{
		if (tblPtr->TableId == tblId)
		{
			namePtr = tblPtr->TableName;
			break;
		}
	}
	return namePtr;
}

EcsEpsgTable GetEpsgTableId (const wchar_t* tableName)
{
	EcsEpsgTable tableId = epsgTblUnknown;
	const TcsEpsgTblMap* tblPtr;
	
	for (tblPtr = KcsEpsgTblMap;tblPtr->TableId != epsgTblUnknown;++tblPtr)
	{
		if (!CS_wcsicmp (tableName,tblPtr->TableName))
		{
		    tableId = tblPtr->TableId;
			break;
		}
	}
	return tableId;
}

EcsEpsgField GetEpsgCodeFieldId (EcsEpsgTable tableId)
{
	EcsEpsgField codeFieldId (epsgFldUnknown);

	const TcsEpsgTblMap* tblPtr;
	
	for (tblPtr = KcsEpsgTblMap;tblPtr->TableId != epsgTblUnknown;++tblPtr)
	{
		if (tblPtr->TableId == tableId)
		{
		    codeFieldId = tblPtr->CodeKeyFieldId;
			break;
		}
	}
	return codeFieldId;
}

short GetEpsgCodeFieldNbr (EcsEpsgTable tableId)
{
	short fldNbr (-1);
	
	EcsEpsgField codeFieldId = GetEpsgCodeFieldId (tableId);
	if (codeFieldId != epsgFldUnknown)
	{
		fldNbr = GetEpsgFieldNumber (tableId,codeFieldId);
	}
	return fldNbr;
}

short GetEpsgFieldNumber (EcsEpsgTable tableId,EcsEpsgField fieldId)
{
	short rtnValue = TcsEpsgDataSetV6::GetFldNbr (tableId,fieldId);
	return rtnValue;
}

EcsCsysType GetEpsgCsysType (const wchar_t* csysTypeName)
{
	EcsCsysType rtnValue;
	const TcsEpsgCsysTypeMap* tblPtr;

	rtnValue = epsgCsysTypUnknown;
	for (tblPtr = KcsEpsgCsysTypeMap;tblPtr->CsysType != epsgCsysTypUnknown;++tblPtr)
	{
		if (!CS_wcsicmp (csysTypeName,tblPtr->CsysTypeName))
		{
			rtnValue = tblPtr->CsysType;
			break;
		}
	}
	return rtnValue;
}

EcsCrsType GetEpsgCrsType (const wchar_t* crsTypeName)
{
	EcsCrsType rtnValue;
	const TcsEpsgCrsTypeMap* tblPtr;

	rtnValue = epsgCrsTypUnknown;
	for (tblPtr = KcsEpsgCrsTypeMap;tblPtr->CrsType != epsgCrsTypUnknown;++tblPtr)
	{
		if (!CS_wcsicmp (crsTypeName,tblPtr->CrsTypeName))
		{
			rtnValue = tblPtr->CrsType;
			break;
		}
	}
	return rtnValue;
}

EcsOpType GetEpsgOpType (const wchar_t* opTypeName)
{
	EcsOpType rtnValue;
	const TcsEpsgOpTypeMap* tblPtr;

	rtnValue = epsgOpTypUnknown;
	for (tblPtr = KcsEpsgOpTypeMap;tblPtr->OpType != epsgOpTypUnknown;++tblPtr)
	{
		if (!CS_wcsicmp (opTypeName,tblPtr->OpTypeName))
		{
			rtnValue = tblPtr->OpType;
			break;
		}
	}
	return rtnValue;
}

EcsDtmType GetEpsgDtmType (const wchar_t* dtmTypeName)
{
	EcsDtmType rtnValue;
	const TcsEpsgDtmTypeMap* tblPtr;

	rtnValue = epsgDtmTypUnknown;
	for (tblPtr = KcsEpsgDtmTypeMap;tblPtr->DtmType != epsgDtmTypUnknown;++tblPtr)
	{
		if (!CS_wcsicmp (dtmTypeName,tblPtr->DtmTypeName))
		{
			rtnValue = tblPtr->DtmType;
			break;
		}
	}
	return rtnValue;
}

EcsUomType GetEpsgUomType (const wchar_t* uomTypeName)
{
	EcsUomType rtnValue;
	const TcsEpsgUomTypeMap* tblPtr;

	rtnValue = epsgUomTypUnknown;
	for (tblPtr = KcsEpsgUomTypeMap;tblPtr->UomType != epsgUomTypUnknown;++tblPtr)
	{
		if (!CS_wcsicmp (uomTypeName,tblPtr->UomTypeName))
		{
			rtnValue = tblPtr->UomType;
			break;
		}
	}
	return rtnValue;
}

EcsOrientation GetOrientation (const wchar_t* orntTypeName)
{
	EcsOrientation rtnValue;
	const TcsEpsgOrntTypeMap* tblPtr;

	unsigned cmpCount;
	wchar_t cmpBufr [32];

	rtnValue = epsgOrntUnknown;
	for (tblPtr = KcsEpsgOrntTypeMap;tblPtr->OrntType != epsgOrntUnknown;++tblPtr)
	{
		cmpCount = static_cast<unsigned>(wcslen (tblPtr->OrntTypeName));
		if (cmpCount >= wcCount (cmpBufr))
		{
			cmpCount = wcCount (cmpBufr) - 1;
		}
		wcsncpy (cmpBufr,orntTypeName,cmpCount);
		cmpBufr [cmpCount] = L'\0';
		if (CS_wcsicmp (cmpBufr,tblPtr->OrntTypeName) == 0)
		{
			rtnValue = tblPtr->OrntType;
			break;
		}
	}
	return rtnValue;
}
//newPage//
//=============================================================================
// TcsEpsgDataSetV6  -  An EPSG dataset based on the version 6 model.
//
// An image of the dataset in .csv form is expected to reside in the directory
// provided to the constructor.  The object is intended to be a read only object
// but nothing specific was done to preclude changing the underlying tables or
// writing changes back to the .csv files.  There are just no member functions
// at this time to support such operation.
//
//=============================================================================
// Static Constants, Variables, and Member Functions
//
// Converts an EPSG table ID & field ID to a .CSV field number.
short TcsEpsgDataSetV6::GetFldNbr (EcsEpsgTable tableId,EcsEpsgField fieldId)
{
	short rtnValue = -1;
	const TcsEpsgFldMap* tblPtr;

	for (tblPtr = KcsEpsgFldMap;tblPtr->TableId != epsgTblUnknown;++tblPtr)
	{
		if (tblPtr->TableId == tableId && tblPtr->FieldId == fieldId)
		{
			rtnValue = tblPtr->FieldNbr;
			break;
		}
	}
	return rtnValue;
}
// Converts an EPSG table ID & field ID to a .CSV field label.
short TcsEpsgDataSetV6::GetFldName (std::wstring& fieldName,EcsEpsgTable tableId,EcsEpsgField fieldId)
{
	short rtnValue = -1;
	const TcsEpsgFldMap* tblPtr;

	fieldName.clear ();
	for (tblPtr = KcsEpsgFldMap;tblPtr->TableId != epsgTblUnknown;++tblPtr)
	{
		if (tblPtr->TableId == tableId && tblPtr->FieldId == fieldId)
		{
			fieldName = tblPtr->FieldName;
			rtnValue = tblPtr->FieldNbr;
			break;
		}
	}
	return rtnValue;
}
//=============================================================================
// Construction, Destruction, & Assignment
TcsEpsgDataSetV6::TcsEpsgDataSetV6 (const wchar_t* databaseFolder,const wchar_t* revLevel)
																	:
																  Ok             (true),
																  DatabaseFolder (databaseFolder),
																  FailMessage    (),
																  EpsgTables     ()
{
	const TcsEpsgTblMap* tblPtr = KcsEpsgTblMap;

	for (tblPtr = KcsEpsgTblMap;tblPtr->TableId != epsgTblUnknown;++tblPtr)
	{
		TcsEpsgTable* nextTable = new TcsEpsgTable (*tblPtr,DatabaseFolder.c_str ());
		if (nextTable->IsOk ())
		{
			EpsgTables.insert (std::make_pair(tblPtr->TableId,nextTable));	//lint !e534 (ignoring return value)
		}
		else
		{
			std::wstring csvStMesg;
			TcsCsvStatus& csvStatus = nextTable->GetCsvStatus ();
			csvStMesg = csvStatus.GetMessage ();
			FailMessage += csvStMesg + L"\n";
			Ok = false;
		}
	}							//lint !e593    cutodial pointer (newTable) not free'ed
	if (Ok)
	{
		if (revLevel == 0)
		{
			DetermineRevisionLevel ();		//lint !e534
		}
		else
		{
			RevisionLevel = revLevel;
		}
	}
}
TcsEpsgDataSetV6::TcsEpsgDataSetV6 (const TcsEpsgDataSetV6& source) : Ok             (source.Ok),
																	  RevisionLevel  (source.RevisionLevel),
																	  DatabaseFolder (source.DatabaseFolder),
																	  FailMessage    (source.FailMessage),
																	  EpsgTables     (source.EpsgTables)
{
}
TcsEpsgDataSetV6::~TcsEpsgDataSetV6 (void)
{
	std::map<EcsEpsgTable,TcsEpsgTable*>::iterator itr;
	
	for (itr = EpsgTables.begin ();itr != EpsgTables.end ();++itr)
	{
		TcsEpsgTable* tblPtr = itr->second;
		delete tblPtr;
	}
}
TcsEpsgDataSetV6& TcsEpsgDataSetV6::operator= (const TcsEpsgDataSetV6& rhs)
{
	if (&rhs != this)
	{
		Ok             = rhs.Ok;
		RevisionLevel  = rhs.RevisionLevel;
		DatabaseFolder = rhs.DatabaseFolder;
		FailMessage    = rhs.FailMessage;
		EpsgTables     = rhs.EpsgTables;		// ouch!!!
	}
	return *this;
}
//=============================================================================
// Public Named Member Functions
const TcsEpsgTable* TcsEpsgDataSetV6::GetTablePtr (EcsEpsgTable tableId) const
{
	const TcsEpsgTable* tblPtr (0);
	std::map<EcsEpsgTable,TcsEpsgTable*>::const_iterator tblItr;

	// Can't use the [] operator as it inserts if the key is not found, thus
	// the std::map can't be a constant.  If we don't have a const version
	// of this function, virtually nothing else can be const.

	tblItr = EpsgTables.find (tableId);
	if (tblItr != EpsgTables.end ())
	{
		tblPtr = tblItr->second;
	}
	return tblPtr;
}
TcsEpsgTable* TcsEpsgDataSetV6::GetTablePtr (EcsEpsgTable tableId)
{
	TcsEpsgTable* tblPtr (0);
	std::map<EcsEpsgTable,TcsEpsgTable*>::iterator tblItr;

	tblItr = EpsgTables.find (tableId);
	if (tblItr != EpsgTables.end ())
	{
		tblPtr = tblItr->second;
	}
	return tblPtr;
}
//=============================================================================
// High Level API Functions
bool TcsEpsgDataSetV6::IsOk (void)
{
	// Is this really necessary???
	return Ok;
}	//lint !e1762
bool TcsEpsgDataSetV6::IsOk (void) const
{
	return Ok;
}
std::wstring TcsEpsgDataSetV6::GetFailMessage (void) const
{
	return FailMessage;
}
const wchar_t* TcsEpsgDataSetV6::GetRevisionLevel (void) const
{
	const wchar_t* wcPtr;
	
	wcPtr = RevisionLevel.c_str ();
	return wcPtr;
}
unsigned TcsEpsgDataSetV6::GetRecordCount (EcsEpsgTable tableId) const
{
	unsigned recordCount (0);

	const TcsEpsgTable* epsgTblPtr = GetTablePtr (tableId);
	if (epsgTblPtr != 0)
	{
		recordCount = epsgTblPtr->RecordCount ();
	}
	return recordCount;
}
bool TcsEpsgDataSetV6::GetFieldByIndex (std::wstring& result,EcsEpsgTable tableId,EcsEpsgField fieldId,unsigned recNbr) const
{
	bool ok (false);
	short fldNbr;
	const TcsEpsgTable* epsgTblPtr;

	epsgTblPtr = GetTablePtr (tableId);
	if (epsgTblPtr != 0)
	{
		fldNbr = GetFldNbr (tableId,fieldId);
		if (fldNbr >= 0)
		{
			TcsCsvStatus csvStatus (epsgTblPtr->GetCsvStatus ());
			// I can't figure out why a cast pointer needs to be used here, but
			// it won't compile anyother way (under Microsoft VC++ 9.0 anyway).
			ok = static_cast<const TcsCsvFileBase*>(epsgTblPtr)->GetField (result,recNbr,fldNbr,csvStatus);
		}
	}
	return ok;
}
bool TcsEpsgDataSetV6::GetCodeByIndex (TcsEpsgCode& epsgCode,EcsEpsgTable tableId,EcsEpsgField fieldId,unsigned recNbr) const
{
	bool ok (false);
	short fldNbr;
	const TcsEpsgTable* epsgTblPtr;
	std::wstring fieldData;

	epsgTblPtr = GetTablePtr (tableId);
	if (epsgTblPtr != 0)
	{
		fldNbr = GetFldNbr (tableId,fieldId);
		if (fldNbr >= 0)
		{
			TcsCsvStatus csvStatus (epsgTblPtr->GetCsvStatus ());
			ok = static_cast<const TcsCsvFileBase*>(epsgTblPtr)->GetField (fieldData,recNbr,fldNbr,csvStatus);
			if (ok)
			{
				epsgCode = TcsEpsgCode (fieldData.c_str ());
				ok = epsgCode.IsValid ();
			}
		}
	}
	return ok;
}
bool TcsEpsgDataSetV6::GetFieldByCode (std::wstring& result,EcsEpsgTable tableId,EcsEpsgField fieldId,const TcsEpsgCode& epsgCode) const
{
	bool ok (false);
	const TcsEpsgTable* epsgTblPtr;

	epsgTblPtr = GetTablePtr (tableId);
	if (epsgTblPtr != 0)
	{
		ok = epsgTblPtr->GetField (result,epsgCode,fieldId);
	}
	return ok;
}
bool TcsEpsgDataSetV6::GetFieldByCode (TcsEpsgCode& result,EcsEpsgTable tableId,EcsEpsgField fieldId,const TcsEpsgCode& epsgCode) const
{
	bool ok (false);
	const TcsEpsgTable* epsgTblPtr;

	epsgTblPtr = GetTablePtr (tableId);
	if (epsgTblPtr != 0)
	{
		ok = epsgTblPtr->GetAsEpsgCode (result,epsgCode,fieldId);
	}
	return ok;
}
bool TcsEpsgDataSetV6::GetFieldByCode (double& result,EcsEpsgTable tableId,EcsEpsgField fieldId,
																		   const TcsEpsgCode& epsgCode) const
{
	bool ok (false);
	
	wchar_t* chkPtr;
	const TcsEpsgTable* epsgTblPtr;

	double lclResult;

	std::wstring fldData;

	lclResult = HUGE_VAL;
	epsgTblPtr = GetTablePtr (tableId);
	if (epsgTblPtr != 0)
	{
		ok = epsgTblPtr->GetField (fldData,epsgCode,fieldId);
		if (ok)
		{
			lclResult = wcstod (fldData.c_str (),&chkPtr);
			if ((*chkPtr != '\0') && (*chkPtr != L' '))
			{
				lclResult = HUGE_VAL;
			}
			ok = (fabs (lclResult) < HUGE_VAL);
		}
	}
	result = lclResult;
	return ok;
}

bool TcsEpsgDataSetV6::IsDeprecated (EcsEpsgTable tableId,const TcsEpsgCode& epsgCode) const
{
	bool deprecated (false);
	const TcsEpsgTable* epsgTblPtr;

	epsgTblPtr = GetTablePtr (tableId);
	if (epsgTblPtr != 0)
	{
		deprecated = epsgTblPtr->IsDeprecated (epsgCode);
	}
	return deprecated;
}
bool TcsEpsgDataSetV6::GetCsMapEllipsoid (struct cs_Eldef_& ellipsoid,const TcsEpsgCode& epsgCode) const
{
	bool ok (false);

	TcsEpsgCode uomCode;

	double eRad (0.0);
	double pRad (0.0);
	double ecent (0.0);
	double flattening (0.0);
	double rFlattening (0.0);
	double eSq (0.0);
	std::wstring fldData;
	std::wstring elpName;

	const TcsEpsgTable* epsgTblPtr = GetTablePtr (epsgTblEllipsoid);
	ok = (epsgTblPtr != 0);
	if (ok)
	{
		ok = epsgTblPtr->GetField (elpName,epsgCode,epsgFldEllipsoidName);
	}
	if (ok)
	{
		ok  = epsgTblPtr->GetAsEpsgCode (uomCode,epsgCode,epsgFldUomCode);
		if (ok)
		{
			ok = epsgTblPtr->GetField (fldData,epsgCode,epsgFldSemiMajorAxis);
			if (ok)
			{
				ok = FieldToReal (eRad,9001UL,fldData.c_str (),uomCode);
			}
		}
		if (ok)
		{
			ok = epsgTblPtr->GetField (fldData,epsgCode,epsgFldSemiMinorAxis);
			if (ok)
			{
				ok = FieldToReal (pRad,9001UL,fldData.c_str (),uomCode);
			}
		}
		if (ok)
		{
			ok = epsgTblPtr->GetField (fldData,epsgCode,epsgFldInvFlattening);
			if (ok)
			{
				ok = FieldToReal (rFlattening,9000UL,fldData.c_str (),9000UL);
			}
		}
		if (ok)
		{
			ok = (eRad != 0.0) && (pRad != 0.0 || rFlattening != 0.0);
		}
		if (ok)
		{
			if (rFlattening != 0.0)
			{
				flattening = 1.0 / rFlattening;
				pRad = eRad * (1.0 - flattening);
			}
			else
			{
				flattening = 1.0 - (pRad / eRad);		//lint !e414   possible divide by zero
				rFlattening = 1.0 / flattening;
			}
			eSq = (2.0 * flattening) - (flattening * flattening);
			ecent = sqrt (eSq);
		}
		if (ok)
		{
			char revisionLevel [64];
			wcstombs (revisionLevel,RevisionLevel.c_str (),sizeof (revisionLevel));
			sprintf (ellipsoid.key_nm,"EPSG::%lu",static_cast<unsigned long>(epsgCode));
			ellipsoid.group [0] = '\0';
			ellipsoid.fill [0] = '\0';
			ellipsoid.fill [1] = '\0';
			ellipsoid.e_rad = eRad;
			ellipsoid.p_rad = pRad;
			ellipsoid.flat = flattening;
			ellipsoid.ecent = ecent;
			wcstombs (ellipsoid.name,elpName.c_str (),sizeof (ellipsoid.name));
			sprintf (ellipsoid.source,"Converted from EPSG %s by CS-MAP",revisionLevel);
			ellipsoid.protect = 0;
			ellipsoid.epsgNbr = static_cast<short>(epsgCode);
			ellipsoid.wktFlvr = wktFlvrEpsg;
			ellipsoid.fill01 = 0;
			ellipsoid.fill02 = 0;
			ellipsoid.fill03 = 0;
			ellipsoid.fill04 = 0;
			ellipsoid.fill05 = 0;
		}
	}
	return ok;
}
// Find the first CRS table record of the given type whose datum field carries
// indicated datum code.  Specifically used to find the first CRS of a
// geographic type based on the specified datum.
bool TcsEpsgDataSetV6::LocateGeographicBase (TcsEpsgCode& geographicBase,EcsCrsType crsType,
																		 const TcsEpsgCode& datumCode) const
{
	bool ok (false);
	bool deprecated (false);
	unsigned recordNumber;
	unsigned invalidRecordNbr;
	TcsEpsgCode geogCode (0UL);
	std::wstring fldData;

	invalidRecordNbr = TcsCsvFileBase::GetInvalidRecordNbr ();
	recordNumber = invalidRecordNbr;				// to keep lint happy.

	const TcsEpsgTable* crsTblPtr = GetTablePtr (epsgTblReferenceSystem);
	ok = (crsTblPtr != 0);
	if (ok)
	{
		// EpsgLocateFirst function does a linear search of the entire table.
		recordNumber = crsTblPtr->EpsgLocateFirst (epsgFldDatumCode,datumCode);
		ok  = (recordNumber != invalidRecordNbr);
	}
	while (ok)
	{
		deprecated = crsTblPtr->IsDeprecated (recordNumber);
		if (!deprecated)
		{
			ok = crsTblPtr->GetField (fldData,recordNumber,epsgFldCoordRefSysKind);
			if (ok)
			{
				EcsCrsType crsKind = GetEpsgCrsType (fldData.c_str ());
				if (crsKind == crsType)
				{
					ok = crsTblPtr->GetAsEpsgCode (geogCode,recordNumber,epsgFldCoordRefSysCode);
					if (ok)
					{
						break;
					}
				}
			}
		}
		recordNumber = crsTblPtr->EpsgLocateNext (recordNumber,epsgFldDatumCode,datumCode);
		ok = (recordNumber != invalidRecordNbr);
	}
	geographicBase = geogCode;
	return ok;
}
// In the case where this function is traditionally used, i.e. transformations
// as opposed to conversions, there can be several different operations for a
// given source and target CRS.  These are distinguished by a variant number.
// All transformations have a variant number, and they begin with one.  If you
// don't know the variation you want, you can always use the default value of
// the variant parameter which is 1.  Refer to the TcsOpVariants objects for
// a better way to deal with this situation.
TcsEpsgCode TcsEpsgDataSetV6::LocateOperation (EcsOpType opType,const TcsEpsgCode& sourceCode,
																const TcsEpsgCode& targetCode,
																long variant) const
{
	bool ok;
	bool deprecated;

	unsigned recordNbr;
	unsigned invalidRecordNbr;

	TcsEpsgCode operationCode (0UL);
	TcsEpsgCode recSrcCode;
	TcsEpsgCode recTrgCode;
	long recVarCode;
	std::wstring fldData;

	invalidRecordNbr = TcsCsvFileBase::GetInvalidRecordNbr ();

	const TcsEpsgTable* copTblPtr = GetTablePtr (epsgTblCoordinateOperation);
	ok = (copTblPtr != 0);

	if (ok)
	{
		recordNbr = copTblPtr->EpsgLocateFirst (epsgFldSourceCrsCode,sourceCode);
		ok = (recordNbr != invalidRecordNbr);
		while (ok)
		{
			deprecated = copTblPtr->IsDeprecated (recordNbr);
			if (deprecated)
			{
				recordNbr = copTblPtr->EpsgLocateNext (recordNbr,epsgFldSourceCrsCode,sourceCode);
				ok = (recordNbr != invalidRecordNbr);
				continue;
			}

			ok  = copTblPtr->GetAsEpsgCode (recSrcCode,recordNbr,epsgFldSourceCrsCode);
			ok &= copTblPtr->GetAsEpsgCode (recTrgCode,recordNbr,epsgFldTargetCrsCode);
			ok &= copTblPtr->GetAsLong (recVarCode,recordNbr,epsgFldCoordOpVariant);
			ok &= copTblPtr->GetField (fldData,recordNbr,epsgFldCoordOpType);
			EcsOpType recOpType = GetEpsgOpType (fldData.c_str ());
			if (ok && (recOpType == opType) && (recSrcCode == sourceCode) &&
											   (recTrgCode == targetCode) &&
											   (variant == 0 || recVarCode == variant))
			{
				ok = copTblPtr->GetAsEpsgCode (operationCode,recordNbr,epsgFldCoordOpCode);
				break;
			}
			recordNbr = copTblPtr->EpsgLocateNext (recordNbr,epsgFldSourceCrsCode,sourceCode);
			ok = (recordNbr != invalidRecordNbr);
		}
	}
	return ok ? operationCode : TcsEpsgCode(0UL);
}
// The following is used to locate the specific record in the ParameterValue
// table.  There is no unique record key, so the value returned is a
// record number.  This is done to reduce duplicated code, as this is not
// a trivial operation.
unsigned TcsEpsgDataSetV6::LocateParameterValue (const TcsEpsgCode& opCode,const TcsEpsgCode& opMethCode,
																		   const TcsEpsgCode& prmCode) const
{
	bool ok;
	
	unsigned recordNumber;
	unsigned invalidRecordNbr;
	unsigned rtnValue;
	
	TcsEpsgCode recOprCode;
	TcsEpsgCode recMthCode;
	TcsEpsgCode recPrmCode;

	invalidRecordNbr = TcsCsvFileBase::GetInvalidRecordNbr ();
	rtnValue = invalidRecordNbr;

	const TcsEpsgTable* prmTblPtr = GetTablePtr (epsgTblParameterValue);
	ok = (prmTblPtr != 0);

	if (ok)
	{
		recordNumber = prmTblPtr->EpsgLocateFirst (epsgFldCoordOpCode,opCode);
		ok = (recordNumber != invalidRecordNbr);
		for (unsigned idx = recordNumber;ok;idx += 1)			//lint !e440    loop variable (idx) does not appear in conditional expression
		{
			// I think this is superfluous.  I believe an operation code uniquely
			// identifies a set of parameters; and the operation method code
			// is redundant.  However, the database is structured this way, so we
			// stick to it so that a future revision to the database doesn't
			// break this code.
			ok  = prmTblPtr->GetAsEpsgCode (recOprCode,idx,epsgFldCoordOpCode);
			ok &= prmTblPtr->GetAsEpsgCode (recMthCode,idx,epsgFldCoordOpMethodCode);
			if (ok)
			{
				ok = (recOprCode == opCode);
			}
			if (!ok)
			{
				// We've looked at all records associated with this
				// operation code, and didn't find the item of interest.
				// We've failed, and we're done.
				// NOTE: we are assuming here that the table is properly
				// sorted.
				break;
			}

			// Verify that the current record pertains to the specific operation
			// method that we have been given.
			if (recMthCode != opMethCode)
			{
				// Nope!  Continue on to the next record.
				continue;
			}

			// See if this is the specific parameter we have been asked to
			// extract.
			ok = prmTblPtr->GetAsEpsgCode (recPrmCode,idx,epsgFldParameterCode);
			if (ok && recPrmCode == prmCode)
			{
				// Yup!  Continue on to the next record.  Save the record number
				// and break the current loop.
				rtnValue = idx;
				break;
			}
		}
	}
	return rtnValue;
}
bool TcsEpsgDataSetV6::GetParameterFileName (std::wstring& parameterFileName,const TcsEpsgCode& opCode,
									 										 const TcsEpsgCode& opMethCode,
																			 const TcsEpsgCode& prmCode) const
{
	bool ok;

	unsigned recordNumber;
	unsigned invalidRecordNbr;

	invalidRecordNbr = TcsCsvFileBase::GetInvalidRecordNbr ();
	recordNumber = invalidRecordNbr;

	const TcsEpsgTable* prmTblPtr = GetTablePtr (epsgTblParameterValue);
	ok = (prmTblPtr != 0);

	if (ok)
	{
		recordNumber = LocateParameterValue (opCode,opMethCode,prmCode);
		ok = (recordNumber != invalidRecordNbr);
	}
	if (ok)
	{
		// Extract the file name field value and return it.
		ok = prmTblPtr->GetField (parameterFileName,recordNumber,epsgFldParamValueFileRef);
	}
	return ok;
}
bool TcsEpsgDataSetV6::GetParameterValue (double& parameterValue,const TcsEpsgCode& opCode,
																 const TcsEpsgCode& opMethCode,
																 const TcsEpsgCode& prmCode,
																 const TcsEpsgCode& trgUomCode) const
{
	bool ok;

	unsigned recordNumber;
	unsigned invalidRecordNbr;

	TcsEpsgCode recUomCode (0UL);

	std::wstring fldData;

	invalidRecordNbr = TcsCsvFileBase::GetInvalidRecordNbr ();

	const TcsEpsgTable* prmTblPtr = GetTablePtr (epsgTblParameterValue);
	ok = (prmTblPtr != 0);

	if (ok)
	{
		recordNumber = LocateParameterValue (opCode,opMethCode,prmCode);
		ok = (recordNumber != invalidRecordNbr);
		if (ok)
		{
			ok = prmTblPtr->GetField (fldData,recordNumber,epsgFldParameterValue);
			if (ok)
			{
				ok = prmTblPtr->GetAsEpsgCode (recUomCode,recordNumber,epsgFldUomCode);
			}
			if (ok)
			{
				ok = FieldToReal (parameterValue,trgUomCode,fldData.c_str (),recUomCode);
			}
		}
	}
	return ok;
}
bool TcsEpsgDataSetV6::GetCsMapDatum (struct cs_Dtdef_& datum,struct cs_Eldef_& ellipsoid,
															  const TcsEpsgCode& epsgDtmCode,
															  unsigned variant) const
{
	bool ok (false);

	EcsDtmType dtmType (epsgDtmTypNone);

	TcsEpsgCode ellpCode;
	TcsEpsgCode baseCode;
	TcsEpsgCode dtmOpCode;

	std::wstring dtmName;
	std::wstring fldData;

	// The basic scheme here is to find the transformation from the provided
	// datum code to WGS84 (i.e. 6326/4326).  When provided with a datum code
	// value of 6326, obviously, this will fail.  So we intercept this now
	// and provide the canned result declared in CSdata.c.
	if (epsgDtmCode == 6326UL)
	{
		struct cs_Dtdef_* dtDefPtr = CS_dtdef ("WGS84");
		memcpy (&datum,dtDefPtr,sizeof (datum));
		CS_free (dtDefPtr);
		struct cs_Eldef_* elDefPtr = CS_eldef ("WGS84");
		memcpy (&ellipsoid,elDefPtr,sizeof (ellipsoid));
		CS_free (elDefPtr);
		return true;
	}

	// Prepare for failure by ensuring the returned data is invalid.
	memset (&datum,'\0',sizeof (datum));
	memset (&ellipsoid,'\0',sizeof (ellipsoid));

	// Get pointers to the required tables.
	const TcsEpsgTable* dtmTblPtr = GetTablePtr (epsgTblDatum);
	const TcsEpsgTable* crsTblPtr = GetTablePtr (epsgTblReferenceSystem);
	const TcsEpsgTable* copTblPtr = GetTablePtr (epsgTblCoordinateOperation);
	const TcsEpsgTable* pmrTblPtr = GetTablePtr (epsgTblPrimeMeridian);
	const TcsEpsgTable* pruTblPtr = GetTablePtr (epsgTblParameterUsage);
	const TcsEpsgTable* prmTblPtr = GetTablePtr (epsgTblParameterValue);
	ok = (dtmTblPtr != 0) && (crsTblPtr != 0) && (copTblPtr != 0) && (pmrTblPtr != 0) &&
																	 (pruTblPtr != 0) &&
																	 (prmTblPtr != 0);
	if (ok)
	{
		// Get the type of the datum.  If it's not "geodetic", we don't want to
		// deal with it; not yet anyway.
		ok = dtmTblPtr->GetField (fldData,epsgDtmCode,epsgFldDatumType);	//lint !e613 (use of null pointer)
		if (ok)
		{
			dtmType = GetEpsgDtmType (fldData.c_str ());
			ok = (dtmType == epsgDtmTypGeodetic);
		}
	}
	if (ok)
	{
		// Do the ellipsoid.
		ok = dtmTblPtr->GetAsEpsgCode (ellpCode,epsgDtmCode,epsgFldEllipsoidCode);		//lint !e613 (use of null pointer)
		if (ok)
		{
			ok = GetCsMapEllipsoid (ellipsoid,ellpCode);
		}
	}

	// Now to the datum specifics.  This can get rather tricky.  There may be
	// several variants, and there may be two or more operations in each
	// variant.  To address this, there is a TcsOpVariants object.
	if (ok)
	{
		ok = dtmTblPtr->GetField (dtmName,epsgDtmCode,epsgFldDatumName);		//lint !e613 (use of null pointer)

		// Need to "locate" the geographic coordinate reference system which
		// references this datum.  That is, search the CRS table for a
		// geographic reference system which is referenced to the datum
		// whose definition we are building.
		ok = LocateGeographicBase (baseCode,epsgCrsTypGeographic2D,epsgDtmCode);
		if (ok && baseCode.IsNotValid ())
		{
			// No 2D system, see if there is a 3D system.
			ok = LocateGeographicBase (baseCode,epsgCrsTypGeographic3D,epsgDtmCode);
			if (ok)
			{
				ok = baseCode.IsValid ();
			}
		}

		// At this time, it is convenient to build all the non numeric stuff in
		// the datum definition.
		if (ok)
		{
			char revisionLevel [64];
			wcstombs (revisionLevel,RevisionLevel.c_str (),sizeof (revisionLevel));
			sprintf (datum.key_nm,"EPSG::%lu",static_cast<unsigned long>(epsgDtmCode));
			strncpy (datum.ell_knm,ellipsoid.key_nm,sizeof (datum.ell_knm));
			datum.ell_knm [sizeof (datum.ell_knm) - 1] = '\0';
			datum.group [0] = '\0';
			datum.locatn [0] = '\0';
			datum.cntry_st [0] = '\0';
			datum.fill [0] = '\0';
			datum.fill [1] = '\0';
			datum.fill [2] = '\0';
			datum.fill [3] = '\0';
			datum.fill [4] = '\0';
			datum.fill [5] = '\0';
			datum.fill [6] = '\0';
			datum.fill [7] = '\0';
			wcstombs (datum.name,dtmName.c_str (),sizeof (datum.name));
			sprintf (datum.source,"Converted from EPSG %s by CS-MAP",revisionLevel);
			datum.protect = 0;
			datum.epsgNbr = static_cast<short>(epsgDtmCode);
			datum.wktFlvr = wktFlvrEpsg;
			datum.fill01 = 0;
			datum.fill02 = 0;
			datum.fill03 = 0;
			datum.fill04 = 0;
		}

		// We have the base geographic system for this datum. We no construct a
		// TcsOpVariants to obtain a list of all variants which will convert the
		// datum of this base to WGS84.
		if (ok)
		{
			TcsOpVariants dtmOpVariants (*this,baseCode,4326UL);
			ok = (dtmOpVariants.GetVariantCount () != 0);
			if (ok)
			{
				// If the user has specified a variant, we use that one.
				if (variant >= 1L)
				{
					dtmOpCode = dtmOpVariants.GetOprtnCodeByVariant (variant);
				}
				else
				{
					// Select the "best" of the variations available.  "Best" in
					// this case is defined by the TcsOpVariants object.
					dtmOpCode = dtmOpVariants.GetBestOprtnCode (*this);
					ok = dtmOpCode.IsValid ();
				}
			}
			if (ok)
			{
				datum.fill03 = static_cast<short>(dtmOpCode);
			}

			// OK, construct a CS-MAP datum definition based on the specific
			// operation code determined above.
			ok = AddDatumParameterValues (datum,dtmOpCode);
		}
	}
	return ok;
}
bool TcsEpsgDataSetV6::AddDatumParameterValues (struct cs_Dtdef_& datum,const TcsEpsgCode& operationCode) const
{
	bool ok;
	bool coordFrame (false);
	short to84_via (cs_DTCTYP_NONE);
	short csPrmCount;

	TcsEpsgCode opMthCode;

	double parmValue;

	csPrmCount = 0;

	// Get pointers to the required tables.
	const TcsEpsgTable* copTblPtr = GetTablePtr (epsgTblCoordinateOperation);
	ok = (copTblPtr != 0);
	if (ok)
	{
		to84_via = DetermineCsMapDatumMethod (operationCode,coordFrame);
		ok = (to84_via != 0);
		//if (!ok)
		//{
		//	to84_via = DetermineCsMapDatumMethod (operationCode,coordFrame);
		//}
	}

	if (ok)
	{
		
		ok = copTblPtr->GetAsEpsgCode (opMthCode,operationCode,epsgFldCoordOpMethodCode);
	}
	
	// Determine the number and nature of the parameters required.
	if (ok)
	{
		datum.fill04 = static_cast<short>(opMthCode);
		switch (opMthCode) {
		case 9603:				// Geocentric Translation
			to84_via = cs_DTCTYP_GEOCTR;
			csPrmCount = 3;
			break;
		case 9604:				// Molodensky
			to84_via = cs_DTCTYP_MOLO;
			csPrmCount = 3;
			break;
		case 9605:				// Abridged Molodensky
			to84_via = cs_DTCTYP_MOLO;
			csPrmCount = 3;
			break;
		case 9606:				// Position Vector
			to84_via = cs_DTCTYP_BURS;
			csPrmCount = 7;
			break;
		case 9607:				// Coordinate Frame
			to84_via = cs_DTCTYP_BURS;
			csPrmCount = 7;
			coordFrame = true;
			break;

		case 9613:				// NADCON
		case 9614:				// NTv1
		case 9615:				// NTv2
		case 9634:				// Maritime TRANSFORM
		case 9655:				// French
		case 9633:				// OSNT
		default:
			csPrmCount = 0;
			break;
		}
	}

	// OK, we now add the parameter values.
	if (ok && csPrmCount >= 3)
	{
		ok = GetParameterValue (parmValue,operationCode,opMthCode,8605UL,9001UL);
		if (ok)
		{
			datum.delta_X = parmValue;
		}
		ok = GetParameterValue (parmValue,operationCode,opMthCode,8606UL,9001UL);
		if (ok)
		{
			datum.delta_Y = parmValue;
		}
		ok = GetParameterValue (parmValue,operationCode,opMthCode,8607UL,9001UL);
		if (ok)
		{
			datum.delta_Z = parmValue;
		}
	}
				
	if (ok && csPrmCount >= 6)
	{
		ok = GetParameterValue (parmValue,operationCode,opMthCode,8608UL,9104UL);
		if (ok)
		{
			datum.rot_X = parmValue;
			if (coordFrame) datum.rot_X *= -1.0;
		}
		ok = GetParameterValue (parmValue,operationCode,opMthCode,8609UL,9104UL);
		if (ok)
		{
			datum.rot_Y = parmValue;
			if (coordFrame) datum.rot_Y *= -1.0;
		}
		ok = GetParameterValue (parmValue,operationCode,opMthCode,8610UL,9104UL);
		if (ok)
		{
			datum.rot_Z = parmValue;
			if (coordFrame) datum.rot_Z *= -1.0;
		}
	}	
	if (ok && csPrmCount >= 7)
	{
		ok = GetParameterValue (parmValue,operationCode,opMthCode,8611UL,9202UL);
		if (ok)
		{
			datum.bwscale = parmValue;
		}
	}
	if (ok)
	{
		datum.to84_via = to84_via;
	}
	return ok;
}
// The following extracts the revision level field from the last record in
// the Version History table.  Should be the revision of the dataset we are
// actually using, regardless of the file name(s) or their location.
bool TcsEpsgDataSetV6::DetermineRevisionLevel (void)
{
	bool ok (false);
	unsigned recNbr;
	std::wstring fieldData;

	const TcsEpsgTable* tblPtr = GetTablePtr (epsgTblVersionHistory);
	if (tblPtr != 0)
	{
		recNbr = tblPtr->RecordCount() - 1;
		ok = GetFieldByIndex (fieldData,epsgTblVersionHistory,epsgFldVersionNumber,recNbr);
		if (ok)
		{
			RevisionLevel = fieldData;
		}
	}
	return ok;
}
// Note that the following table is used to map the source datums.
// That is, the file name is simply used to determine what EPSG
// is converting from.  The conversion method may be converting
// to something other than WGS84, but we only need to know what
// EPSG is converting from.
struct TcsGsbNameToCodeMap 
{
	char gsbName [64];
	short to84Code;
} KcsGsbNameToCodeMap [] =
{
	{	"A66 National (13.09.01).gsb",    cs_DTCTYP_AGD66  },
	{	"BETA2007.gsb",                   cs_DTCTYP_DHDN   },
	{	"CHENyx06.gsb",                   cs_DTCTYP_CHENYX },
	{	"CGQ77-83.gsb",                   cs_DTCTYP_NONE   },
	{	"CGQ77-98.gsb",                   cs_DTCTYP_ATS77  },
	{	"NAD83-98.gsb",                   cs_DTCTYP_CSRS   },
	{	"National 84 (02.07.01).gsb",     cs_DTCTYP_AGD84  },
	{	"NB7783v2.gsb",                   cs_DTCTYP_ATS77  },
	{	"NS7783v2.gsb",                   cs_DTCTYP_ATS77  },
	{	"PE7783v2.gsb",                   cs_DTCTYP_ATS77  },
	{	"nt_0599.gsb",                    cs_DTCTYP_AGD66  },
	{	"NTv2_0.gsb",                     cs_DTCTYP_NAD27  },
	{	"nzgd2kgrid0005.gsb",             cs_DTCTYP_NZGD49 },
	{	"QUE27-83.gsb",                   cs_DTCTYP_NAD27  },
	{	"QUE27-98.gsb",                   cs_DTCTYP_NAD27  },
	{	"rgf93_ntf.gsb",                  cs_DTCTYP_RGF93  },
	{	"SEAust_21_06_00.gsb",            cs_DTCTYP_AGD66  },
	{	"SK27-98.gsb",                    cs_DTCTYP_NAD27  },
	{	"SK83-98.gsb",                    cs_DTCTYP_NAD83  },
	{	"sped2et.gsb",                    cs_DTCTYP_ED50   },
	{	"SPED2ETV2.gsb",                  cs_DTCTYP_ED50   },
	{	"tas_1098.gsb",                   cs_DTCTYP_AGD66  },
	{	"SK8398",                         cs_DTCTYP_NAD83  },
	{	"vic_0799.gsb",                   cs_DTCTYP_AGD66  },
	{	"wa_0400.gsb",                    cs_DTCTYP_AGD84  },
	{	"wa_0700.gsb",                    cs_DTCTYP_AGD84  },
	{	"\0",                             cs_DTCTYP_NONE   }		//lint !e840    use of nul character with a string literal
};
short TcsEpsgDataSetV6::DetermineCsMapDatumMethod (const TcsEpsgCode& operationCode,bool& coordFrame) const
{
	bool ok;
	short to84_via (cs_DTCTYP_NONE);

	TcsEpsgCode opMthCode;

	std::wstring fileName;

	char fileNameC [64];

	coordFrame = false;

	// Get pointers to the required tables.
	const TcsEpsgTable* copTblPtr = GetTablePtr (epsgTblCoordinateOperation);
	const TcsEpsgTable* prmTblPtr = GetTablePtr (epsgTblParameterValue);
	ok = (copTblPtr != 0) && (prmTblPtr != 0);

	// Get the EPSG operation method.
	if (ok)
	{
		ok = copTblPtr->GetAsEpsgCode (opMthCode,operationCode,epsgFldCoordOpMethodCode);	//lint !e613  (possible use of null pointer)
	}

	// Map the method to a CS-MAP to84_via value.	
	if (ok)
	{
		if (opMthCode == 9603UL)			// Geocentric Translation
		{
			to84_via = cs_DTCTYP_GEOCTR;
		}
		else if (opMthCode == 9604UL)		// Molodensky
		{			
			to84_via = cs_DTCTYP_MOLO;
		}
		else if (opMthCode == 9605UL)		// Abridged Molodensky
		{
			to84_via = cs_DTCTYP_MOLO;
		}
		else if (opMthCode == 9606UL)		// Position Vector
		{
			to84_via = cs_DTCTYP_BURS;
		}
		else if (opMthCode == 9607UL)		// Coordinate Frame
		{
			to84_via = cs_DTCTYP_BURS;
			coordFrame = true;
		}
		else if (opMthCode ==  9613UL)		// NADCON
		{
			// Get the file name parameter.
			ok = GetParameterFileName (fileName,operationCode,opMthCode,8656UL);
			wcstombs (fileNameC,fileName.c_str(),sizeof (fileNameC));
		}
		else if (opMthCode ==  9614UL)		// NTv1
		{
			to84_via = cs_DTCTYP_NAD27;
		}
		else if (opMthCode ==  9615UL)		// NTv2
		{
			ok = GetParameterFileName (fileName,operationCode,opMthCode,8656UL);
			if (ok)
			{
				wcstombs (fileNameC,fileName.c_str(),sizeof (fileNameC));
				for (int idx = 0;KcsGsbNameToCodeMap[idx].gsbName [0] != '\0';idx++)
				{
					if (!CS_stricmp (fileNameC,KcsGsbNameToCodeMap[idx].gsbName))
					{
						to84_via = KcsGsbNameToCodeMap[idx].to84Code;
						break;
					}
				}
			}
		}
		else if (opMthCode ==  9634UL)		// Maritime TRANSFORM
		{
			to84_via = cs_DTCTYP_ATS77;
		}
		else if (opMthCode ==  9655UL)		// French
		{
			// There is only one datum that uses this technique.
			to84_via = cs_DTCTYP_RGF93;
		}
		else if (opMthCode ==  9633UL)		// OSNT
		{
			// CS-MAP implements this datum transformation in a very screwy
			// way.  This needs to be adjusted when this piece of legacy is
			// properly addressed.
			to84_via = cs_DTCTYP_NONE;
		}
		else
		{
			// All other EPSG methods are unsupported.  Fortunately, the number
			// and importance of these definitions is rather small.
			to84_via = cs_DTCTYP_NONE;
		}
	}
	return to84_via;
}
bool TcsEpsgDataSetV6::GetCsMapCoordsys (struct cs_Csdef_& coordsys,struct cs_Dtdef_& datum,
																	struct cs_Eldef_& ellipsoid,
																	const TcsEpsgCode& crsEpsgCode) const
{
	bool ok (false);

	TcsEpsgCode dtmEpsgCode;
	TcsEpsgCode areaOfUseCode;
	TcsEpsgCode horzUomCode;
	TcsEpsgCode vertUomCode;

	EcsCrsType crsType (epsgCrsTypNone);

	std::wstring crsName;

	// Initialize the return values so there is no chance of
	// confusion on an error condition.
	memset (&coordsys,'\0',sizeof (coordsys));
	memset (&datum,'\0',sizeof (datum));
	memset (&ellipsoid,'\0',sizeof (ellipsoid));

	// Get pointers to the required tables.
	const TcsEpsgTable* crsTblPtr  = GetTablePtr (epsgTblReferenceSystem);
	const TcsEpsgTable* areaTblPtr = GetTablePtr (epsgTblArea);
	ok = (crsTblPtr != 0) && (areaTblPtr != 0);

	// Verify that the provided code is OK, and the referenced CRS is not
	// deprecated.
	if (ok)
	{
		bool deprecated = crsTblPtr->IsDeprecated (crsEpsgCode);
		ok = !deprecated;
	}

	// We'll need to know the type of this definition.
	if (ok)
	{
		crsType = GetCrsType (crsEpsgCode);
		ok = (crsType == epsgCrsTypProjected) || (crsType == epsgCrsTypGeographic2D);
	}

	// Extract the reference datum code and pass it on to GetCsMapDatum.
	// GetCsMapDatum also does the ellipsoid for us.
	if (ok)
	{
		ok = GetReferenceDatum (dtmEpsgCode,crsEpsgCode);
		if (ok)
		{
			// We extracted a valid datum code, so we'll use the GetCsMapdatum
			// function to get the datum definition for us.  This also gets the
			// ellipsoid definition.
			ok = GetCsMapDatum (datum,ellipsoid,dtmEpsgCode);
		}
	}

	// Now for anything common to both geographic and projected.
	// Determine the quad value to be used.
	if (ok)
	{
		short quad;
		double factor;

		// CS-MAP's quad determines the order of the axes and the direction.
		ok = GetCoordsysQuad (quad,horzUomCode,vertUomCode,crsEpsgCode);
		if (ok)
		{
			coordsys.quad = (crsType == epsgCrsTypGeographic2D) ? -quad : quad;
			coordsys.epsg_qd = coordsys.quad;
		}

		// Determine the units by extracting a CS-MAP unit name which produces
		// the same conversion factor as that associated with the horizontal
		// unit we extracted from the EPSG coordinate system definition.
		if (ok)
		{
			if (crsType == epsgCrsTypProjected)
			{
				ok = GetUomToMeters (factor,horzUomCode);
				if (ok)
				{
					const char* cp = CS_unitluByFactor (cs_UTYP_LEN,factor);
					ok = (cp != 0);
					if (ok)
					{
						CS_stncp (coordsys.unit,cp,sizeof (coordsys.unit));
					}
				}
			}
			else if (crsType == epsgCrsTypGeographic2D ||
					 crsType == epsgCrsTypGeographic3D)
			{ 
				ok = GetUomToDegrees (factor,horzUomCode);
				if (ok)
				{
					const char* cp = CS_unitluByFactor (cs_UTYP_ANG,factor);
					ok = (cp != 0);
					if (ok)
					{
						CS_stncp (coordsys.unit,cp,sizeof (coordsys.unit));
					}
				}
			}
			else
			{
				ok = false;
			}
		}

		// Set the EPSG code value in the definition.
		coordsys.epsgNbr = static_cast<short>(crsEpsgCode);
	}

	if (ok)
	{
		char msgTemp [64];

		ok = crsTblPtr->GetField (crsName,crsEpsgCode,epsgFldCoordRefSysName);
		if (ok)
		{
			sprintf (coordsys.key_nm,"EPSG::%lu",static_cast<unsigned long>(crsEpsgCode));
			if (static_cast<unsigned long>(dtmEpsgCode) >= 6001 &&
				static_cast<unsigned long>(dtmEpsgCode) <= 6099)
			{
				// Here if the datum is one of the EPSG definitions which simply
				// reference an ellipsoid.  We extract the ellipsoid definition
				// from the datum definition already processed.
				coordsys.dat_knm [0] = '\0';
				CS_stncp (coordsys.elp_knm,ellipsoid.key_nm,sizeof (coordsys.elp_knm));
			}
			else
			{
				CS_stncp (coordsys.dat_knm,datum.key_nm,sizeof (coordsys.dat_knm));
				coordsys.elp_knm [0] = '\0';
			}
			wcstombs (coordsys.desc_nm,crsName.c_str (),sizeof (coordsys.desc_nm));
			wcstombs (msgTemp,RevisionLevel.c_str (),sizeof (msgTemp));
			sprintf (coordsys.source,"Converted from EPSG %s by CS-MAP",msgTemp);
			coordsys.epsgNbr = static_cast<short>(crsEpsgCode);
			coordsys.wktFlvr = wktFlvrEpsg;
		}
	}

	// Extract the area of use information from EPSG and enter it into the
	// the appropriate fields in coordsys.
	if (ok)
	{
		bool mmOk;
		double minLng, minLat;
		double maxLng, maxLat;
		// We don't do the xy_min or xy_max; so we turn them off.
		coordsys.xy_min [0] = cs_Zero;
		coordsys.xy_min [1] = cs_Zero;
		coordsys.xy_max [0] = cs_Zero;
		coordsys.xy_max [1] = cs_Zero;

		// We turn off the ll_min and ll_max in case there's a failure
		// in the code below.
		ok = crsTblPtr->GetAsEpsgCode (areaOfUseCode,crsEpsgCode,epsgFldAreaOfUseCode);
		if (ok)
		{
			// Get the EPSG useful range values.
			mmOk  = areaTblPtr->GetAsReal (minLng,areaOfUseCode,epsgFldAreaWestBoundLng);
			mmOk &= areaTblPtr->GetAsReal (maxLng,areaOfUseCode,epsgFldAreaEastBoundLng);
			mmOk &= areaTblPtr->GetAsReal (minLat,areaOfUseCode,epsgFldAreaSouthBoundLat);
			mmOk &= areaTblPtr->GetAsReal (maxLat,areaOfUseCode,epsgFldAreaNorthBoundLat);

			// See if they are all Ok and consistent.
			if (mmOk)
			{
				mmOk  = (minLng >= -180.0) && (minLng <= 180.0);
				mmOk &= (minLat >=  -90.0) && (minLat <=  90.0);
				mmOk &= (maxLng >= -180.0) && (maxLng <= 180.0);
				mmOk &= (maxLat >=  -90.0) && (maxLat <=  90.0);
				if (mmOk)
				{
					//?? This might fail on a legal definition.  Don't know how EPSG
					// wraps around the 180 degree crack.  Don't know right off hand
					// any definitions which do (Alaska, maybe).
					mmOk &= (minLng < maxLng)  && (minLat < maxLat);
				}
			}
			if (mmOk)
			{
				coordsys.ll_min [0] = minLng;
				coordsys.ll_min [1] = minLat;
				coordsys.ll_max [0] = maxLng;
				coordsys.ll_max [1] = maxLat;
			}
		}
	}
	
	// Do the stuff specific to the type.
	if (ok && crsType == epsgCrsTypGeographic2D)
	{
		ok = GeographicCoordsys (coordsys,crsEpsgCode,horzUomCode);
	}
	else if (ok && crsType == epsgCrsTypProjected)
	{
		ok = ProjectedCoordsys (coordsys,crsEpsgCode,horzUomCode);
	}
	return ok;
}
bool TcsEpsgDataSetV6::GetReferenceDatum (TcsEpsgCode& dtmEpsgCode,const TcsEpsgCode& crsEpsgCode) const
{
	bool ok;
	TcsEpsgCode baseCode;
	EcsCrsType crsType (epsgCrsTypNone);

	const TcsEpsgTable* crsTblPtr = GetTablePtr (epsgTblReferenceSystem);
	const TcsEpsgTable* dtmTblPtr = GetTablePtr (epsgTblDatum);

	dtmEpsgCode.Invalidate ();
	ok = (crsTblPtr != 0) && (dtmTblPtr != 0);
	if (ok)
	{
		// We don't convert deprecated definitions.
		ok = !crsTblPtr->IsDeprecated (crsEpsgCode);		//lint !e613 (use of null pointer)
	}
	if (ok)
	{
		// Get the type (kind) of the CRS.
		crsType = GetCrsType (crsEpsgCode);
		ok = (crsType == epsgCrsTypProjected) || (crsType == epsgCrsTypGeographic2D);
	}

	// Extract the reference datum code.
	if (ok)
	{
		if (crsType == epsgCrsTypProjected)
		{
			// For the projected type, we first need to locate the CRS given as
			// the base.
			ok = crsTblPtr->GetAsEpsgCode (baseCode,crsEpsgCode,epsgFldSourceGeogCrsCode);		//lint !e613 (use of null pointer)
			if (ok)
			{
				// OK, extract the datum code of the referenced geographic
				// CRS record.  We could verify that the type is geographic.
				ok = crsTblPtr->GetAsEpsgCode (dtmEpsgCode,baseCode,epsgFldDatumCode);		//lint !e613 (use of null pointer)
			}
		}
		else if (crsType == epsgCrsTypGeographic2D)
		{
			ok = crsTblPtr->GetAsEpsgCode (dtmEpsgCode,crsEpsgCode,epsgFldDatumCode);		//lint !e613 (use of null pointer)
		}
		ok = dtmEpsgCode.IsValid ();
	}
	return ok;
}
// All EPSG longitude parameters are relative to the prime meridian.  So, it is
// important that the prime meridian be made easily available.
bool TcsEpsgDataSetV6::GetPrimeMeridian (double& primeMeridian,const TcsEpsgCode& crsEpsgCode) const
{
	bool ok;
	TcsEpsgCode dtmEpsgCode;
	TcsEpsgCode pmrEpsgCode;
	TcsEpsgCode uomEpsgCode;
	std::wstring fldData;
	
	const TcsEpsgTable* dtmTblPtr = GetTablePtr (epsgTblDatum);
	const TcsEpsgTable* pmrTblPtr = GetTablePtr (epsgTblPrimeMeridian);
	const TcsEpsgTable* uomTblPtr = GetTablePtr (epsgTblUnitOfMeasure);
	ok = (dtmTblPtr != 0) && (pmrTblPtr != 0) && (uomTblPtr != 0);

	primeMeridian = -360.0;			// sure to be noticed if ever used by mistake.
	
	// Extract the datum reference code from the indicated crs entry.
	if (ok)
	{
		ok = GetReferenceDatum (dtmEpsgCode,crsEpsgCode);
	}

	// Use this datum code to extract the prime meridian in degrees.
	if (ok)
	{
		ok = dtmTblPtr->GetAsEpsgCode (pmrEpsgCode,dtmEpsgCode,epsgFldPrimeMeridianCode);		//lint !e613 (use of null pointer)
		if (ok)
		{
			ok = pmrTblPtr->GetField (fldData,pmrEpsgCode,epsgFldGreenwichLongitude);		//lint !e613 (use of null pointer)
			if (ok)
			{
				ok = pmrTblPtr->GetAsEpsgCode (uomEpsgCode,pmrEpsgCode,epsgFldUomCode);		//lint !e613 (use of null pointer)
			}
			if (ok)
			{
				ok = FieldToDegrees (primeMeridian,fldData.c_str (),uomEpsgCode);
			}
		}
	}
	return ok;
}
bool TcsEpsgDataSetV6::GeographicCoordsys (struct cs_Csdef_& coordsys,const TcsEpsgCode& crsEpsgCode,
																	  const TcsEpsgCode& horzUomCode) const
{
	bool ok;

	double uomFactor;

	double primeMeridian;			// in degrees

	// Here for Geographic specific stuff.
	// Get pointers to the required tables.
	const TcsEpsgTable* crsTblPtr = GetTablePtr (epsgTblReferenceSystem);
	const TcsEpsgTable* dtmTblPtr = GetTablePtr (epsgTblDatum);
	const TcsEpsgTable* uomTblPtr = GetTablePtr (epsgTblUnitOfMeasure);
	const TcsEpsgTable* pmrTblPtr = GetTablePtr (epsgTblPrimeMeridian);
	const TcsEpsgTable* sysTblPtr = GetTablePtr (epsgTblCoordinateSystem);
	const TcsEpsgTable* axsTblPtr = GetTablePtr (epsgTblAxis);

	ok = (crsTblPtr != 0) && (dtmTblPtr != 0) && (uomTblPtr != 0) && (pmrTblPtr != 0) &&
																	 (sysTblPtr != 0) &&
																	 (axsTblPtr != 0);

	// Initialize the following to default values.
	if (ok)
	{
		coordsys.org_lat  = 0.0;
		coordsys.x_off    = 0.0;
		coordsys.y_off    = 0.0;
		coordsys.scl_red  = 1.0;
		coordsys.unit_scl = 1.0;
		coordsys.map_scl  = 1.0;
		coordsys.scale    = 1.0;
		coordsys.order    = 0;
		coordsys.zones    = 0;
	}

	if (ok)
	{
		CS_stncp (coordsys.prj_knm,"LL",sizeof (coordsys.prj_knm));
		CS_stncp (coordsys.group,"LL",sizeof (coordsys.group));

		ok = GetUomToDegrees (uomFactor,horzUomCode);
		if (ok)
		{
			const char* cp = CS_unitluByFactor (cs_UTYP_ANG,uomFactor);
			ok = (cp != 0);
			if (ok)
			{
				CS_stncp (coordsys.unit,cp,sizeof (coordsys.unit));
				coordsys.unit_scl = CS_unitlu (cs_UTYP_ANG,cp);
			}
		}
	}

	if (ok)
	{
		// Parm1 and parm2 are the longitude range parameters.  EPSG doesn't
		// support this feature, so we use the CS-MAP default values.  The
		// remainder of the parameters are not used in geographic systems.
		coordsys.prj_prm1 = cs_Zero;
		coordsys.prj_prm2 = cs_Zero;
	}
	
	if (ok)
	{
		// Org_lng is the prime meridian associated with this definition.  In
		// EPSG, the prime meridian code is stored in the datums table.
		ok = GetPrimeMeridian (primeMeridian,crsEpsgCode);
		if (ok)
		{
			coordsys.org_lng = primeMeridian;
		}
	}

	// The rest is pretty easy.
	if (ok)
	{
		coordsys.zero [0] = 1.0E-12 * coordsys.unit_scl;
		coordsys.zero [1] = 1.0E-12 * coordsys.unit_scl;
	}
	return ok;
}
// This function should only be called by the GetCsMapCoordsys function, as
// it assumes that all the stuff common to both Geographic and Projected
// coordinate systems has already been taken care of and the valid results
// are in the definition structure passed to it.
//lint -save -esym(613,prjTblPtr)
bool TcsEpsgDataSetV6::ProjectedCoordsys (struct cs_Csdef_& coordsys,const TcsEpsgCode& crsEpsgCode,
																	 const TcsEpsgCode& horzUomCode) const
{
	bool ok (false);

	TcsEpsgCode copEpsgCode;
	TcsEpsgCode mthEpsgCode;
	TcsEpsgCode areaOfUseCode;

	const struct cs_Prjtab_* prjTblPtr (0);
	const struct cs_Prjprm_* prmTblPtr (0);
	const struct cs_PrjprmMap_* useTblPtr (0);

	double toMeters (cs_Zero);			// intentionally a really bogus value
	double primeMeridian (5000.0);		// intentionally a really bogus value

	// Here for Projective specific stuff.
	// Get pointers to the required tables.
	const TcsEpsgTable* crsTblPtr = GetTablePtr (epsgTblReferenceSystem);
	const TcsEpsgTable* copTblPtr = GetTablePtr (epsgTblCoordinateOperation);
	const TcsEpsgTable* uomTblPtr = GetTablePtr (epsgTblUnitOfMeasure);
	const TcsEpsgTable* sysTblPtr = GetTablePtr (epsgTblCoordinateSystem);
	const TcsEpsgTable* axsTblPtr = GetTablePtr (epsgTblAxis);

	ok = (crsTblPtr != 0) && (copTblPtr != 0) && (uomTblPtr != 0) && (sysTblPtr != 0) &&
																	 (axsTblPtr != 0);

	// Get the system UOM to Meters factor, we'll need that in various places.
	if (ok)
	{
		ok = GetUomToMeters (toMeters,horzUomCode);
	}

	// Get the the prime meridian.  We need this as it applies to all the
	// longitude parameters.
	if (ok)
	{
		ok = GetPrimeMeridian (primeMeridian,crsEpsgCode);
	}

	// Determine the group to which this definition should belong.  This is
	// table driven and not very precise.
	if (ok)
	{
		// Set a default in case what appears below fails.
		CS_stncp (coordsys.group,"EPSGPRJ",sizeof (coordsys.group));
		
		// Get the AreaOfUse code from the 
		ok = crsTblPtr->GetAsEpsgCode (areaOfUseCode,crsEpsgCode,epsgFldAreaOfUseCode);
		if (ok)
		{
			short shrtEpsgCode = static_cast<short>(areaOfUseCode);
			const TcmAreaToMsiGroupMap* tblPtr;

			for (tblPtr = KcmAreaToMsiGroupMap;tblPtr->epsgAreaFrom != 0;tblPtr += 1)
			{
				if (shrtEpsgCode >= tblPtr->epsgAreaFrom &&
					shrtEpsgCode <= tblPtr->epsgAreaTo)
				{
					CS_stncp (coordsys.group,tblPtr->msiGroup,sizeof (coordsys.group));
					break;
				}
			}
		}
	}

	// Extract the Coordinate Operation code and the Coordinate Operation
	// Method code.
	if (ok)
	{
		ok = crsTblPtr->GetAsEpsgCode (copEpsgCode,crsEpsgCode,epsgFldProjectionConvCode);
		if (ok)
		{
			ok = copTblPtr->GetAsEpsgCode (mthEpsgCode,copEpsgCode,epsgFldCoordOpMethodCode);
		}
	}

	if (ok)
	{
		// Use the EPSG operation method code to locate the projection in the
		// CS-MAP projection table which matches this projection.  If we can't
		// find it in the projection table, than CS-MAP doesn't support that
		// projection method, and we have failed.  Note that there is _NOT_ a
		// one to one correspondence between EPSG projection methods and
		// CS-MAP projections.
		for (prjTblPtr = cs_Prjtab;prjTblPtr->code != cs_PRJCOD_END;prjTblPtr += 1)
		{
			if (mthEpsgCode == prjTblPtr->epsg)
			{
				break;
			}
		}
		ok = (prjTblPtr->code != cs_PRJCOD_END);
	}

	if (ok && mthEpsgCode == 9807UL)	// Transverse Mercator
	{
		double orgLat;
		double sclRed;
		double xxxOff;
		double prjPrm1;

		// This is a Transverse Mercator based system.  See if it is a
		// definition of a UTM zone.  If so, we use the cs_PRJCOD_UTM entry
		// in the CS-MAP projection table.
		if (CS_stristr (coordsys.desc_nm,"UTM") != 0)
		{
			// The name contains "UTM" and the projection is Transverse
			// Mercator; a good prospect for a UTM zone definition.  We do
			// this separately as CS-MAP has a projection code specifically
			// for UTM zones.
			ok  = GetParameterValue (orgLat,copEpsgCode,mthEpsgCode,8801UL,9102UL);
			ok &= GetParameterValue (sclRed,copEpsgCode,mthEpsgCode,8805UL,9201UL);
			ok &= GetParameterValue (xxxOff,copEpsgCode,mthEpsgCode,8806UL,horzUomCode);
			if (ok &&
			    (fabs (orgLat) < 1.0E-05) &&
				(fabs (sclRed - 0.9996) < 1.0E-07) &&
				(fabs ((xxxOff * toMeters) - 500000.00) < 1.0E-04)
			   )
			{
				// Still looks good.  Central meridian must be a valid UTM value.
				ok  = GetParameterValue (prjPrm1,copEpsgCode,mthEpsgCode,8802UL,9102UL);
				if (ok)
				{
					int utmZone = static_cast<int>(prjPrm1 + 183.00001) / 6;
					if (utmZone >= 1 && utmZone <= 60)
					{
						// OK, I'm satisfied that we have a UTM zone.
						for (prjTblPtr = cs_Prjtab;prjTblPtr->code != cs_PRJCOD_END;prjTblPtr += 1)
						{
							if (prjTblPtr->code == cs_PRJCOD_UTM)
							{
								break;
							}
						}
						ok = (prjTblPtr->code != cs_PRJCOD_END);
					}
				}
			}
		}
	}
	// For the remainder of this execution, mthEpsgCode has the EPSG Operation
	// Method code, and prjTblPtr points to the corresponding CS-MAP projection
	// table entry.  These are heavily relied upon below.
						
	// Deal with the projection and parameters.
	if (ok)
	{
		unsigned char csMapPrmCode;

		short prjCode;
		ulong32_t prjFlags;
		TcsEpsgCode epsgPrmCode;

		// Capture the stuff that applies to most every projection or can be
		// determined from the flag's word in the projection table.  Some of
		// this may be overwritten below where we deal with special situations.
		// (There always has to be some special situations, don't you know;
		// else a whole bunch of us would be out of a job.)
		//
		// Set up some default values to keep the logic below clean.
		coordsys.org_lng = cs_Zero;
		coordsys.org_lat = cs_Zero;
		coordsys.scl_red = cs_One;
		coordsys.x_off   = cs_Zero;
		coordsys.y_off   = cs_Zero;

		// Populate coordsys with what we can having a pointer to the appropriate
		// entry of the projection table.
		prjCode = prjTblPtr->code;
		prjFlags = prjTblPtr->flags;
		CS_stncp (coordsys.prj_knm,prjTblPtr->key_nm,sizeof (coordsys.prj_knm));
		if ((prjFlags & cs_PRJFLG_ORGLNG) == 0UL)
		{
			// The projection requires an origin longitude which is not carried
			// as projection parameter 1.
			if (mthEpsgCode == 9819UL)		// Krovak
			{
				coordsys.org_lng = cs_Zero;
			}
			else
			{
				epsgPrmCode = 8802UL;				// Longitude of Natural Origin
				if (mthEpsgCode == 9802UL ||		// Lambert Conformal Conic 2SP
					mthEpsgCode == 9803UL ||		// Lambert Conformal Conic; Belgium (2SP)
					mthEpsgCode == 9822UL ||		// Albers Equal Area Conic
					mthEpsgCode == 1051UL)			// Lambert Conformal Conic Michigan
				{
					epsgPrmCode = 8822UL;			// Longitude of False Origin 
				}
				ok &= GetParameterValue (coordsys.org_lng,copEpsgCode,mthEpsgCode,epsgPrmCode,9102UL);
			}
			coordsys.org_lng += primeMeridian;
		}
		if ((prjFlags & cs_PRJFLG_ORGLAT) == 0UL)
		{
			// The projection requires an origin latitude.
			epsgPrmCode = 8801UL;				// Latitude of Natural Origin
			if (mthEpsgCode == 9802UL ||		// Lambert Conformal Conic 2SP
			    mthEpsgCode == 9803UL ||		// Lambert Conformal Conic; Belgium (2SP)
			    mthEpsgCode == 9822UL ||		// Albers Equal Area Conic
			    mthEpsgCode == 1051UL)			// Lambert Conformal Conic Michigan
			{
				epsgPrmCode = 8821UL;			// Latitude of False Origin
			}
			else if (mthEpsgCode == 9819UL)		// Krovak Oblique Conic
			{
				epsgPrmCode = 8811UL;			// Latitude of Projection Center
			}
			ok &= GetParameterValue (coordsys.org_lat,copEpsgCode,mthEpsgCode,epsgPrmCode,9102UL);
		}
		if ((prjFlags & cs_PRJFLG_SCLRED) != 0UL)
		{
			epsgPrmCode = 8805UL;				// Scale Factor at Natural Origin
			if (mthEpsgCode == 9819UL)			// Krovak Oblique Conic
			{
				epsgPrmCode = 8819UL;			// Latitude of Pseudo Standard Parallel			
			}
			else if (mthEpsgCode == 9812UL ||	// Hotine Oblique Mercator (Variant A)
					 mthEpsgCode == 9813UL ||	// Laborde Oblique Mercator
					 mthEpsgCode == 9815UL)		// Hotine Oblique Mercator (Variant B)
			{
				epsgPrmCode = 8815UL;			// Scale Factor on Initial Line
			}
			ok &= GetParameterValue (coordsys.scl_red,copEpsgCode,mthEpsgCode,epsgPrmCode,9201UL);
		}
		if ((prjFlags & cs_PRJFLG_ORGFLS) == 0UL)
		{
			epsgPrmCode = 8806UL;				// False Easting
			if (mthEpsgCode == 9802UL ||		// Lambert Conformal Conic - 2SP
			    mthEpsgCode == 9803UL ||		// Lambert Conformal Conic - 2SP Belgium
			    mthEpsgCode == 9816UL ||		// Tunisia Mining Grid
			    mthEpsgCode == 9822UL ||		// Albers Equal Area
			    mthEpsgCode == 9830UL ||		// Polar Stereographic - Variant C
   			    mthEpsgCode == 1051UL)			// Lambert Conformal Conic Michigan

			{
				epsgPrmCode = 8826UL;			// Easting at False Origin
			}
			else if (mthEpsgCode == 9815UL)		// Hotine Oblique Mercator (Variant B)
			{
				epsgPrmCode = 8816UL;			// Easting at Projection Centre
			}

			ok &= GetParameterValue (coordsys.x_off,copEpsgCode,mthEpsgCode,epsgPrmCode,horzUomCode);
			epsgPrmCode = 8807UL;				// False Northing
			if (mthEpsgCode == 9802UL ||		// Lambert Conformal Conic - 2SP
			    mthEpsgCode == 9803UL ||		// Lambert Conformal Conic - 2SP Belgium
			    mthEpsgCode == 9816UL ||		// Tunisia Mining Grid
			    mthEpsgCode == 9822UL ||		// Albers Equal Area
			    mthEpsgCode == 9830UL ||		// Polar Stereographic - Variant C
   			    mthEpsgCode == 1051UL)			// Lambert Conformal Conic Michigan

			{
				epsgPrmCode = 8827UL;			// Northing at False Origin
			}
			else if (mthEpsgCode == 9815UL)		// Hotine Oblique Mercator (Variant B)
			{
				epsgPrmCode = 8817UL;			// Northing at Projection Centre
			}
			ok &= GetParameterValue (coordsys.y_off,copEpsgCode,mthEpsgCode,epsgPrmCode,horzUomCode);
		}

		// We finish this by simply adding the appropriate projection parameter
		// values.  This is table driven and works for most projections.  The special
		// situations handling below will overwrite this default processing as is
		// appropriate.

		// We first locate the appropriate entry in the parameter usage
		// table.
		for (useTblPtr = cs_PrjprmMap;useTblPtr->prj_code != cs_PRJCOD_END;useTblPtr += 1)
		{
			if (useTblPtr->prj_code == prjCode)
			{
				break;
			}
		}
		ok = (useTblPtr->prj_code != cs_PRJCOD_END);

		// We now loop through the parameter use specification and process
		// each parameter required by this particular projection. 
		if (ok)
		{
			int prmIdx;
			double prmTemp;

			for (prmIdx = 0;useTblPtr->prm_types [prmIdx] != 0;prmIdx += 1)
			{
				csMapPrmCode = useTblPtr->prm_types [prmIdx];
				prmTblPtr = &csPrjprm [csMapPrmCode];
				epsgPrmCode = prmTblPtr->epsg;
				if (epsgPrmCode == 0UL)
				{
					// This can happen for the projections for which there are
					// different parameterizations between EPSG and CS-MAP.  We
					// assume here that any discrepancy here will be dealt with
					// below where we have projection specific code.
					continue;
				}
				
				// Some kludges to adjust to EPSG idosyncrasies.  Of course
				// there are no such things as CS-MAP idosyncrasies :>)
				if (mthEpsgCode == 9805UL &&		// Mercator (Variant B)
				    epsgPrmCode == 8832UL)			// Latitude of Standard Parallel
				{
					// For the Mercator 2SP projection, there really is only
					// one standard parallel, but the EPSG parameterization
					// uses the 1st standard parallel parameter for THE
					// standard parallel.
					epsgPrmCode = 8823UL;			// Latitude of First Standard Parallel
				}

				// Get the parameter value.
				switch (prmTblPtr->log_type) {
				case cs_PRMLTYP_LNG:
					// Longitude needs special processing as we need to apply any
					// prime meridian which may be active in EPSG.  In CS-MAP
					// all longitude type parameter values are referenced to
					// Greenwich by definition.
					ok = GetParameterValue (prmTemp,copEpsgCode,mthEpsgCode,epsgPrmCode,9102UL);
					prmTemp += primeMeridian;
					break;
				case cs_PRMLTYP_LAT:
					// We need to make sure the units are degrees.  CS-MAP
					// requires all angular parameters to be in degrees.
					ok = GetParameterValue (prmTemp,copEpsgCode,mthEpsgCode,epsgPrmCode,9102UL);
					break;
				case cs_PRMLTYP_AZM:
				case cs_PRMLTYP_ANGD:
					// Same as latitude, with the exception that we adjust the
					// value to be between -180 and +180.  EPSG likes to use
					// the 0 to +360 range.
					ok = GetParameterValue (prmTemp,copEpsgCode,mthEpsgCode,epsgPrmCode,9102UL);
					if (prmTemp > 180.0)
					{
						prmTemp -= 360.0;
					}
					break;
				case cs_PRMLTYP_CMPLXC:
					// I don't think there are any of these in EPSG yet.
					ok = GetParameterValue (prmTemp,copEpsgCode,mthEpsgCode,epsgPrmCode,9203UL);
					break;
				case cs_PRMLTYP_ZNBR:
					// I don't think there are any of these in EPSG yet.  At
					// least not for the set of operation methods currently
					// supported by CS-MAP.
					ok  = GetParameterValue (prmTemp,copEpsgCode,mthEpsgCode,8802UL,9102UL);
					if (ok)
					{
						prmTemp = (double)((int)(prmTemp + 183.00001) / 6);		//lint !e653   (possible loss of fraction)
					}
					break;
				case cs_PRMLTYP_HSNS:
					// I don't think there are, or ever will be, any of these
					// in EPSG.
					ok = GetParameterValue (prmTemp,copEpsgCode,mthEpsgCode,8807UL,horzUomCode);
					if (ok)
					{
						prmTemp = (prmTemp > cs_One) ? cs_One : cs_Mone;
					}
					break;
				case cs_PRMLTYP_GHGT:
					// I don't think there are any of these in EPSG yet.  Geoid
					// heightrameter values are always in meters.
					ok = GetParameterValue (prmTemp,copEpsgCode,mthEpsgCode,epsgPrmCode,9001UL);
					break;
				case cs_PRMLTYP_ELEV:
					// I don't think there are any of these in EPSG yet.
					// Average elevation values (in CS-MAP) are always in the
					// system unit.
					ok = GetParameterValue (prmTemp,copEpsgCode,mthEpsgCode,epsgPrmCode,horzUomCode);
					break;
				case cs_PRMLTYP_AFCOEF:
					// These parameter types exist in EPSG and CS-MAP, but the
					// methods that use them in EPSG are not supported by
					// CS-MAP and vice versa.  So, for now anyway, this code
					// should never be executed.
					ok = GetParameterValue (prmTemp,copEpsgCode,mthEpsgCode,epsgPrmCode,9203UL);
					break;
				case cs_PRMLTYP_XYCRD:
					// There may be some of these in EPSG, but only for methods
					// which are not supported by CS-MAP.  So for now, this
					// code should never be executed.
					ok = GetParameterValue (prmTemp,copEpsgCode,mthEpsgCode,epsgPrmCode,horzUomCode);
					break;
				case cs_PRMLTYP_SCALE:
					// Scale values in CS-MAP projected definitions are always
					// at the unity level.
					ok = GetParameterValue (prmTemp,copEpsgCode,mthEpsgCode,epsgPrmCode,9201UL);
					break;
				case cs_PRMLTYP_NONE:
				default:
					// We provide a zero (successfully) for all other parameter
					// values.  They __SHOULD__ be adjusted in the projection
					// specific code below.
					ok = true;
					prmTemp = cs_Zero;
					break;
				}

				// Stuff the parameter into the definition structure.
				if (ok)
				{
					*(&coordsys.prj_prm1 + prmIdx) = prmTemp;
				}
			}
		}
	}

	// That should do it for most all projections.  Of course there are some
	// special situations which we deal with here.
	if (ok)
	{
		if (prjTblPtr->code == cs_PRJCOD_UTM)
		{
			double prjPrm1;
			double yyyOff;
			// It was determined above that this is a UTM zone.  We make the
			// proper adjustments here to convert it to a CS-MAP UTM projection.
			ok  = GetParameterValue (prjPrm1,copEpsgCode,mthEpsgCode,8802UL,9102UL);
			ok &= GetParameterValue (yyyOff,copEpsgCode,mthEpsgCode,8807UL,horzUomCode);
			if (ok)
			{
				int utmZone = static_cast<int>(prjPrm1 + 183.00001) / 6;
				ok = (utmZone >= 1 && utmZone <= 60);
				if (ok)
				{
					CS_stncp (coordsys.prj_knm,"UTM",sizeof (coordsys.prj_knm));
					coordsys.prj_prm1 = static_cast<double>(utmZone);
					coordsys.prj_prm2 = (fabs (yyyOff) <= cs_One) ? cs_One : cs_Mone;
				}
			}
		}
		else if (mthEpsgCode == 9808UL)			// South oriented Transverse Mercator.
		{
			coordsys.quad = 1;
		}
		else if (mthEpsgCode == 9812UL ||		// Hotine Oblique Mercator (variant A)
				 mthEpsgCode == 9815UL)			// Hotine Oblique Mercator (variant B)
		{
			double alpha;						// azimuth of geodesic at projection center
			double gamma;						// azimuth of geodesic at natural origin

			// Hotine Oblique Mercator/Rectified Skew Orthomorphic.  EPSG has
			// only one projection variation, while CS-MAP has two.  This is
			// true as the EPSG parameterization requires two angles; the
			// CS-MAP parameterizations only one angle.  This is possible,
			// as given on of these angles, you can compute the other.  You
			// just need to know which one you have; thus two projection
			// variatiuons in CS-MAP.
			
			// So, we get both of the angle parameters provided in the
			// EPSG database, and based on the values thereof, we select
			// a CS-MAP projection code.  In the EPSG database, 8813 is
			// the azimuth of the central geodesic at the projection center,
			// and 8814 is the rotation angle applied to the skew grid in
			// order to cause the Y grid axis to be true north.  When both
			// of these values are equal, you have the Hotine Mercator
			// variation; when they are not equal, you have the Rectified
			// Skew Orthomorphic variation.			
			ok  = GetParameterValue (alpha,copEpsgCode,mthEpsgCode,8813UL,9102UL);
			ok &= GetParameterValue (gamma,copEpsgCode,mthEpsgCode,8814UL,9102UL);
			alpha = CS_adj180 (alpha);
			gamma = CS_adj180 (gamma);
			if (fabs (alpha - gamma) < 1.0E-02)
			{
				// The two angles are equal, we have the Hotine Oblique
				// Mercator variation.
				CS_stncp (coordsys.prj_knm,"HOM1XY",sizeof (coordsys.prj_knm));
				coordsys.prj_prm3 = alpha;
			}
			else
			{
				// This is a Rectified Skew Orthomorphic
				CS_stncp (coordsys.prj_knm,"RSKEW",sizeof (coordsys.prj_knm));
				coordsys.prj_prm3 = alpha;
			}
		}
		else if (mthEpsgCode == 9819UL)			// Krovak Conformal Conic.
		{
			double oblqPoleCoLat;

			// This gets strange.  The parameterization of this projection
			// outside of CS-MAP is weird.  The parameter is called:
			//
			// 8813: Azimuth of initial line
			// The azimuthal direction (north zero, east of north being
			// positive) of the great circle which is the centre line of an
			// oblique projection. The azimuth is given at the projection
			// centre.
			//
			// This value is obvisouly not a real parameter that someone
			// would choose.  What it really is is the co-latitude of the
			// oblique pole on the gaussian surface.  This is calculatable
			// if you are given a real parameter, such as the latitude of
			// the oblique pole on the basic ellipsoid.  That is the
			// parameter CS-MAP expects.
			//
			// Since there is only one basic coordinate system which uses
			// this projection, the following code works real fine and will
			// last a long time.
			//
			// The standard code above should have properly placed most of
			// the correct values.
			ok  = GetParameterValue (oblqPoleCoLat,copEpsgCode,mthEpsgCode,8813UL,9102UL);
			if (ok && (fabs (coordsys.org_lat - 49.5) < 1.0E-03) &&
					  (fabs (oblqPoleCoLat - 30.28814) < 1.0E-03)
			   )
			{
				// This is indeed the Krovak, actually the only CRS which
				// uses this projection.  So, we can now do this, rather
				// confidently.
				coordsys.org_lng  = -17.66666666667;		// Actually, meridian of Ferro.
				coordsys.org_lat  = 49.5;					// Latitude at which the gaussian radius is computed.
				coordsys.prj_prm1 = 42.5;					// Longitude of the oblique pole relative to ther Ferro meridian)
				coordsys.prj_prm2 = 59.75759855555555555;	// Latitude of the oblique pole (Riga, Estonia).
				coordsys.prj_prm3 = 78.5;					// Standard parallel on the oblique code.
				coordsys.scl_red  = 0.9999;					// 1:10,000
				coordsys.quad     = 1;						// X increases to the east, Y increases to the north
			}
			else
			{
				// This isn't the Krovak we know, thus we can it now.
				ok = false;
			}
		}
		else if (mthEpsgCode == 9802UL ||		// Lambert Conformal Conic (2SP)
				 mthEpsgCode == 9803UL ||		// Lambert Conformal Conic (2SP Belgium)
				 mthEpsgCode == 9817UL ||		// Lambert Conformal Conic, Near Conformal
				 mthEpsgCode == 9822UL ||		// Albers Equal Area Conic
				 mthEpsgCode == 9826UL)			// Lambert Conic Conformal (West Orientated)
		{
			double prmTemp;

			// For conics of the two standard parallel variety, we make sure
			// that the northern most of the two parallels is in prj_prm1.
			// That is a CS-MAP convention.  It doesn't make much difference
			// to the mathmatics, but it makes comparing two definitions a
			// bit problematical.
			if (coordsys.prj_prm1 < coordsys.prj_prm2)
			{
				prmTemp = coordsys.prj_prm1;
				coordsys.prj_prm1 = coordsys.prj_prm2;
				coordsys.prj_prm2 = prmTemp;
			}
		}
		else
		{
			ok = true;
		}
	}

	if (ok)
	{
		// Finally, in certain cases we need to tweak the group code.
		if (!CS_stricmp (coordsys.group,"SPCS"))
		{
			if (!CS_stricmp (coordsys.dat_knm,"NAD27"))
			{
				CS_stncp (coordsys.group,"SPCS27",sizeof (coordsys.group));
			}
			else if (!CS_stricmp (coordsys.dat_knm,"NAD83"))
			{
				if (!CS_stricmp (coordsys.unit,"FOOT"))
				{
					CS_stncp (coordsys.group,"SPCS83F",sizeof (coordsys.group));
				}
				else if (!CS_stricmp (coordsys.unit,"IFOOT"))
				{
					CS_stncp (coordsys.group,"SPCS83I",sizeof (coordsys.group));
				}
				else
				{
					CS_stncp (coordsys.group,"SPCS83",sizeof (coordsys.group));
				}
			}
			else if (!CS_stricmp (coordsys.dat_knm,"HPGN"))
			{
				if (!CS_stricmp (coordsys.unit,"FOOT"))
				{
					CS_stncp (coordsys.group,"SPCSHPF",sizeof (coordsys.group));
				}
				else if (!CS_stricmp (coordsys.unit,"IFOOT"))
				{
					CS_stncp (coordsys.group,"SPCSHPI",sizeof (coordsys.group));
				}
				else
				{
					CS_stncp (coordsys.group,"SPCSHP",sizeof (coordsys.group));
				}
			}
			else
			{
				CS_stncp (coordsys.group,"OTHR-US",sizeof (coordsys.group));
			}
		}
	}
	return ok;
}
//lint -restore
//=============================================================================
// Protected Support Functions
//=============================================================================
// Private Support Functions
///////////////////////////////////////////////////////////////////////////////
// General, and Protected, Support functions
EcsCrsType TcsEpsgDataSetV6::GetCrsType (const TcsEpsgCode& crsCode) const
{
	bool ok;
	EcsCrsType crsType (epsgCrsTypUnknown);
	std::wstring fldData;

	const TcsEpsgTable* crsTblPtr = GetTablePtr (epsgTblReferenceSystem);
	if (crsTblPtr != 0)
	{
		ok = crsTblPtr->GetField (fldData,crsCode,epsgFldCoordRefSysKind);
		if (ok)
		{
			crsType = GetEpsgCrsType (fldData.c_str ());
		}
	}
	return crsType;
}
EcsUomType TcsEpsgDataSetV6::GetUomFactor (double& uomFactor,const TcsEpsgCode& uomCode) const
{
	bool ok (false);
	EcsUomType uomType (epsgUomTypUnknown);
	double factorB;
	double factorC;
	std::wstring unitType;

	// We adopt the convention that UOM code 9000 means the associated
	// value is unitless.
	if (uomCode == 9000UL)
	{
		uomType =  epsgUomTypNone;
		uomFactor = 1.00000;
	}
	else
	{
		const TcsEpsgTable* uomTblPtr = GetTablePtr (epsgTblUnitOfMeasure);
		if (uomTblPtr != 0)
		{
			ok = uomTblPtr->GetField (unitType,uomCode,epsgFldUnitOfMeasType);
			if (ok)
			{
				uomType = GetEpsgUomType (unitType.c_str ());
				if (uomType != epsgUomTypUnknown)
				{
					if ((uomCode >= 9107UL && uomCode <= 9108) ||
						(uomCode >= 9110UL && uomCode <= 9111) ||
						(uomCode >= 9115UL && uomCode <= 9112)
					   )
					{
						// EPSG does not provide factor values for these codes.
						factorB = PI;
						factorC = 180.0;
						ok = true;
					}
					else
					{
						ok  = uomTblPtr->GetAsReal (factorB,uomCode,epsgFldFactorB);
						ok &= uomTblPtr->GetAsReal (factorC,uomCode,epsgFldFactorC);
					}
					if (ok)
					{
						ok = (fabs (factorC) > 1.0E-12);
					}
					if (ok)
					{
						uomFactor = (factorB / factorC);
					}
				}
			}
		}
	}
	return uomType;
}
bool TcsEpsgDataSetV6::GetUomToDegrees (double& toDegrees,const TcsEpsgCode& uomCode) const
{
	bool ok (false);
	EcsUomType uomType;
	double uomFactor;

	uomType = GetUomFactor (uomFactor,uomCode);
	if (uomType == epsgUomTypAngular)
	{
		toDegrees = uomFactor * 57.29577951308238;
		ok = true;
	}
	return ok;
}
bool TcsEpsgDataSetV6::GetUomToMeters (double& toMeters,const TcsEpsgCode& uomCode) const
{
	bool ok (false);
	EcsUomType uomType;
	double uomFactor;

	uomType = GetUomFactor (uomFactor,uomCode);
	if (uomType == epsgUomTypLinear)
	{
		toMeters = uomFactor;
		ok = true;
	}
	return ok;
}
bool TcsEpsgDataSetV6::GetUomToUnity (double& toUnity,const TcsEpsgCode& uomCode) const
{
	bool ok (false);
	EcsUomType uomType;
	double uomFactor;

	uomType = GetUomFactor (uomFactor,uomCode);
	if (uomType == epsgUomTypScale)
	{
		toUnity = uomFactor;
		ok = true;
	}
	return ok;
}
bool TcsEpsgDataSetV6::ConvertUnits (double& value,const TcsEpsgCode& trgUomCode,const TcsEpsgCode& srcUomCode) const
{
	bool ok (true);

	EcsUomType srcType;
	EcsUomType trgType;
	
	double srcFactor;
	double trgFactor;

	if (srcUomCode != trgUomCode)
	{
		srcType = GetUomFactor (srcFactor,srcUomCode);
		trgType = GetUomFactor (trgFactor,trgUomCode);
		ok = (srcType != epsgUomTypUnknown && srcType == trgType);
		if (ok)
		{
			if (srcType != epsgUomTypNone)
			{
				value *= (srcFactor / trgFactor);
			}
		}
	}
	return ok;
}
// This function converts field value presented as a wchar_t string to a real
// value.  This function is smart enough to know of the different
// representations of angular and scale values in the EPSG database.  Thus,
// hopefully, all the logic required to deal with these different
// representations is isolated in this function.
//
// This function is considered private; examine the public and protected
// functions for a function more useful to your purpose.
bool TcsEpsgDataSetV6::FieldToReal (double& result,const TcsEpsgCode& trgUomCode,
												   const wchar_t* fldData,
												   const TcsEpsgCode& srcUomCode) const
{
	bool ok (false);

	EcsUomType srcType;
	EcsUomType trgType;

	wchar_t* endPtr;
	double realValue (0.0);
	double srcFactor;
	double trgFactor;
	double degFactor;
	
	result = 0.0;
	srcType = GetUomFactor (srcFactor,srcUomCode);
	trgType = GetUomFactor (trgFactor,trgUomCode);
	ok = (srcType != epsgUomTypUnknown && srcType == trgType);
	if (ok)
	{
		if (srcType == epsgUomTypAngular)
		{
			ok = FieldToDegrees (realValue,fldData,srcUomCode);
			srcType = GetUomFactor (degFactor,9102UL);
			realValue *= (degFactor / srcFactor);
		}
		else
		{
			realValue = wcstod (fldData,&endPtr);
			while (iswspace (*endPtr)) endPtr++;
			ok = (*endPtr == L'\0');
		}
	}
	if (ok)
	{
		if (srcUomCode != trgUomCode)
		{
			result = realValue * (srcFactor / trgFactor);
		}
		else
		{
			result = realValue;
		}
	}
	return ok;
}
// There is a ton of duplicated code in the following function.  My apologies,
// but I chose duplicate code over a huge pot of spaghetti which would be the
// alternative.
bool TcsEpsgDataSetV6::FieldToDegrees (double& result,const wchar_t* field,const TcsEpsgCode& uomCode) const
{
	//lint -save -esym(644,minus,minutes,seconds)		(posibly not initialized)
	bool ok;
	double realValue;
	wchar_t* wcPtr;
	wchar_t* endPtr;
	wchar_t wrkBufr [64];

	ok = false;								// intermediate ok
	
	result = realValue = 0.0;
	if (uomCode == 9110UL)
	{
		// sexagesimal DMS -> +DDD.MMSSsssssss
		bool minus (false);
		size_t fillCnt (0);
		long degrees (0L);
		long minutes (0L);
		double seconds (0.0);

		wchar_t wcDegrees [16];
		wchar_t wcMinutes [16];
		wchar_t wcSeconds [32];

		// Get a copy of the field we are to convert in a place where we
		// modify same.
		wcsncpy (wrkBufr,field,64);
		wrkBufr [63] = L'\0';
		fillCnt = 63 - wcslen (wrkBufr);
		
		// It is common for values to not have a decimal point, which makes
		// the deciphering of values in formats other than straight decimal
		// notation difficult.  We try to fix this problem here.
		wcPtr = wcschr (wrkBufr,L'.');
		if (wcPtr == 0)
		{
			// No decimal point, we add one.
			wcscat (wrkBufr,L".");
			fillCnt -= 1;
		}

		// The following is required as below we assume that the minutes
		// and seconds characters are there.
		wcsncat (wrkBufr,L"00000000000000000",fillCnt);
		wrkBufr [63] = L'\0';

		wcPtr = wcschr (wrkBufr,L'.');
		if (wcPtr != 0)
		{
			*wcPtr++ = L'\0';
			wcsncpy (wcDegrees,wrkBufr,16);
			wcDegrees [15] = L'\0';
			degrees = wcstol (wcDegrees,&endPtr,10);
			ok = (*endPtr == L'\0');
			if (ok)
			{
				minus = degrees < 0L;
				if (minus)
				{
					degrees = -degrees;
				}
				ok = (degrees < 360L);
			}
			if (ok)
			{
				wcMinutes [0] = *wcPtr++;
				wcMinutes [1] = *wcPtr++;
				wcMinutes [2] = L'\0';
				minutes = wcstol (wcMinutes,&endPtr,10);
				ok = (*endPtr == L'\0') && (minutes < 60L);
			}
			if (ok)
			{
				wcSeconds [0] = *wcPtr++;
				wcSeconds [1] = *wcPtr++;
				wcSeconds [2] = L'.';
				wcsncpy (&wcSeconds [3],wcPtr,28);
				wcSeconds [31] = L'\0';
				seconds = wcstod (wcSeconds,&endPtr);
				ok = (*endPtr == L'\0') && (seconds < 60.0);
			}
			if (ok)
			{
				realValue = static_cast<double>(degrees) + static_cast<double>(minutes) / 60.0 + seconds / 3600.0;
				if (minus)
				{
					realValue = -realValue;
				}
			}
		}
	}
	else if (uomCode == 9111UL)
	{
		// sexagesimal DM -> +DDD.MMmmmmmm
		bool minus (false);
		size_t fillCnt (0);
		long degrees (0L);
		double minutes (0.0);

		wchar_t wcDegrees [16];
		wchar_t wcMinutes [32];

		wcsncpy (wrkBufr,field,64);
		wrkBufr [63] = L'\0';
		fillCnt = 63 - wcslen (wrkBufr);
		wcsncat (wrkBufr,L"00000000000000000",fillCnt);
		wrkBufr [63] = L'\0';

		wcPtr = wcschr (wrkBufr,L'.');
		if (wcPtr != 0)
		{
			*wcPtr++ = L'\0';
			wcsncpy (wcDegrees,wrkBufr,16);
			wcDegrees [15] = L'\0';
			degrees = wcstol (wcDegrees,&endPtr,10);
			ok = (*endPtr == L'0');
			if (ok)
			{
				minus = degrees < 0L;
				if (minus)
				{
					degrees = -degrees;
				}
				ok = (degrees < 360L);
			}
			if (ok)
			{
				wcMinutes [0] = *wcPtr++;
				wcMinutes [1] = *wcPtr++;
				wcMinutes [2] = L'.';
				wcsncpy (wcMinutes,wcPtr,28);
				wcMinutes [31] = L'\0';
				minutes = wcstod (wcMinutes,&endPtr);
				ok = (*endPtr == L'0' && minutes < 60.0);
			}
			if (ok)
			{
				realValue = static_cast<double>(degrees) + minutes / 60.0;
				if (minus)
				{
					realValue = -realValue;
				}
			}
		}
	}
	else if (uomCode == 9107UL || uomCode == 9121UL)
	{
		// Degrees (int), minutes (int), seconds (real).  Not really used in
		// the tables pre se; the tables usually use double field type for
		// numeric values and the 9110 & 9111 types are usually used.
		bool minus (false);
		long degrees (0L);
		long minutes (0L);
		double seconds (0.0);

		// Get a copy of the field we are to convert in a place where we
		// modify same.  Not really necessary in this case, but what the heck.
		wcsncpy (wrkBufr,field,64);

		minutes = 0L;
		seconds = 0.0;
		degrees = wcstol (wrkBufr,&endPtr,10);
		ok = (degrees >= -360L && degrees <= 360L);
		if (ok)
		{
			minus = degrees < 0L;
			if (minus)
			{
				degrees = -degrees;
			}
		}
		if (ok && *endPtr != L'\0')
		{
			// We assume a single non-space delimter character, we care
			// not what that character is.  Space can be used.  Allowing
			// space(s) enables this code to valid for 9121.
			endPtr++;
			minutes = wcstol (endPtr,&endPtr,10);
			ok = (minutes >= 0L && minutes < 60L);
		}
		if (ok && *endPtr != L'\0')
		{
			seconds = wcstod (endPtr,&endPtr);
			// There is supposed to be a delimiter, we allow it or none.
			if (*endPtr != L'\0') endPtr += 1;
			ok = (*endPtr == L'\0') && (seconds >= 0.0 && seconds < 60.0);
		}
		if (ok)
		{
			realValue = static_cast<double>(degrees) + static_cast<double>(minutes) / 60.0 + seconds / 3600.0;
			if (minus)
			{
				realValue = -realValue;
			}
		}
	}
	else if (uomCode == 9108UL)
	{
		// Degrees (int), minutes (int), seconds (real).  Not really used in
		// the tables pre se; the tables usually use double field type for
		// numeric values and the 9110 & 9111 types are usually used.
		// sexagesimal DMS -> +DDD.MMSSsssssss
		bool minus (false);
		long degrees (0L);
		long minutes (0L);
		double seconds (0.0);

		// Get a copy of the field we are to convert in a place where we
		// modify same.  Not really necessary, but whatthe heck.
		wcsncpy (wrkBufr,field,64);

		minutes = 0L;
		seconds = 0.0;
		degrees = wcstol (wrkBufr,&endPtr,10);
		ok = (*endPtr != L'\0' && degrees >= 0L && degrees <= 360L);
		if (ok)
		{
			// We assume a single non-space delimter character, we care
			// not what that character is.  Space can be used.
			endPtr++;
			minutes = wcstol (endPtr,&endPtr,10);
			ok = (*endPtr != L'\0' && minutes >= 0L && minutes < 60L);
		}
		if (ok)
		{
			seconds = wcstod (endPtr,&endPtr);
			ok = (*endPtr != L'\0') && (seconds >= 0.0 && seconds < 60.0);
		}
		if (ok)
		{
			// Need to extract the hemisphere (i.e. sign).  We'll skip any
			// white space which may be there.
			while (iswspace (*endPtr)) endPtr++;
			switch (*endPtr) {
			case L'N':	minus = false; break;
			case L'n':	minus = false; break;
			case L'E':	minus = false; break;
			case L'e':	minus = false; break;
			case L'S':	minus = true;  break;
			case L's':	minus = true;  break;
			case L'W':	minus = true;  break;
			case L'w':	minus = true;  break;
			default:
				ok = false;
			}
		}
		if (ok)
		{
			realValue = static_cast<double>(degrees) + static_cast<double>(minutes) / 60.0 + seconds / 3600.0;
			if (minus)
			{
				realValue = -realValue;
			}
		}
	}
	else if ((uomCode >= 9101UL && uomCode <= 9106UL) ||
			 (uomCode == 9109UL) ||
			 (uomCode >= 9112UL && uomCode <= 9114UL))
	{
		// 9101 -> radians
		// 9102 -> degrees
		// 9103 -> arc-minute
		// 9104 -> arc-second
		// 9105 -> grad
		// 9106 -> gon
		// 9109 -> microradian
		// 9112 -> centesimal minute
		// 9113 -> centesimal second
		// 9114 -> mil_6400
		double uomFactor;
		realValue = wcstod (field,&endPtr);
		ok = (*endPtr == L'\0' &&  fabs (realValue) < 360.0);
		if (ok)
		{
			ok = GetUomToDegrees (uomFactor,uomCode);
			if (ok && uomFactor != 0.0)
			{
				realValue = realValue * uomFactor;
			}
		}
	}
	else if (uomCode == 9115UL)
	{
		// Degrees (int), minutes (real).  Not really used in the tables per se;
		// the tables usually use double field type for numeric values and the
		// 9110 & 9111 types are usually used.
		bool minus (false);
		long degrees (0L);
		double minutes (0.0);

		// Get a copy of the field we are to convert in a place where we
		// modify same.  Not really necessary, but whatthe heck.
		wcsncpy (wrkBufr,field,64);

		minutes = 0.0;
		degrees = wcstol (wrkBufr,&endPtr,10);
		ok = (degrees >= -360L && degrees <= 360L);
		if (ok)
		{
			minus = degrees < 0L;
			if (minus)
			{
				degrees = -degrees;
			}
		}
		if (ok && *endPtr != L'\0')
		{
			minutes = wcstod (endPtr,&endPtr);
			// There is supposed to be a delimiter, we allow it or none.
			if (*endPtr != L'\0') endPtr += 1;
			ok = (*endPtr == L'\0') && (minutes >= 0.0 && minutes < 60.0);
		}
		if (ok)
		{
			realValue = static_cast<double>(degrees) + minutes / 60.0;
			if (minus)
			{
				realValue = -realValue;
			}
		}
	}
	else if (uomCode == 9116UL)
	{
		bool minus (false);

		realValue = wcstod (field,&endPtr);
		ok = (*endPtr != L'\0' &&  realValue >= 0.0 && realValue < 360.0);
		if (ok)
		{
			// Need to extract the hemisphere (i.e. sign).  We'll skip any
			// white space which may be there.
			while (iswspace (*endPtr)) endPtr++;
			switch (*endPtr) {
			case L'N':	minus = false; break;
			case L'n':	minus = false; break;
			case L'E':	minus = false; break;
			case L'e':	minus = false; break;
			case L'S':	minus = true;  break;
			case L's':	minus = true;  break;
			case L'W':	minus = true;  break;
			case L'w':	minus = true;  break;
			default:
				ok = false;
				break;
			}
		}
		if (ok && minus)
		{
			realValue = -realValue;
		}
	}
	else if (uomCode == 9117UL)
	{
		bool minus (false);

		// Get a copy of the field we are to convert in a place where we
		// modify same.  Not really necessary, but what the heck.
		wcsncpy (wrkBufr,field,64);

		// Need to extract the hemisphere (i.e. sign).  We'll skip any
		// white space which may be there.
		ok = true;
		endPtr = wrkBufr;
		while (iswspace (*endPtr)) endPtr++;
		switch (*endPtr) {
		case L'N':	minus = false; break;
		case L'n':	minus = false; break;
		case L'E':	minus = false; break;
		case L'e':	minus = false; break;
		case L'S':	minus = true;  break;
		case L's':	minus = true;  break;
		case L'W':	minus = true;  break;
		case L'w':	minus = true;  break;
		default:
			ok = false;
			break;
		}
		if (ok)
		{
			endPtr += 1;
			realValue = wcstod (field,&endPtr);
			ok = (*endPtr == L'\0' &&  realValue >= 0.0 && realValue < 360.0);
		}
		if (ok && minus)
		{
			realValue = -realValue;
		}
	}
	else if (uomCode == 9118UL)
	{
		// Degrees (int), minutes (real), hemisphere.  Not really used in the
		// tables per se; the tables usually use double field type for numeric
		// values and the 9110 & 9111 types are usually used.
		bool minus (false);
		long degrees (0L);
		double minutes (0.0);

		// Get a copy of the field we are to convert in a place where we
		// modify same.  Not really necessary, but whatthe heck.
		wcsncpy (wrkBufr,field,64);

		minutes = 0.0;
		degrees = wcstol (wrkBufr,&endPtr,10);
		ok = (degrees >= 0L && degrees <= 360L);
		if (ok)
		{
			minus = degrees < 0L;
			if (minus)
			{
				degrees = -degrees;
			}
		}
		if (ok && *endPtr != L'\0')
		{
			minutes = wcstod (endPtr,&endPtr);
			// There is supposed to be a delimiter, we allow it or none.
			if (*endPtr != L'\0') endPtr += 1;
			ok = (*endPtr == L'\0') && (minutes >= 0.0 && minutes < 60.0);
		}
		if (ok)
		{
			// Need to extract the hemisphere (i.e. sign).  We'll skip any
			// white space which may be there.
			while (iswspace (*endPtr)) endPtr++;
			switch (*endPtr) {
			case L'N':	minus = false; break;
			case L'n':	minus = false; break;
			case L'E':	minus = false; break;
			case L'e':	minus = false; break;
			case L'S':	minus = true;  break;
			case L's':	minus = true;  break;
			case L'W':	minus = true;  break;
			case L'w':	minus = true;  break;
			default:
				ok = false;
				break;
			}
		}
		if (ok)
		{
			realValue = static_cast<double>(degrees) + minutes / 60.0;
			if (minus)
			{
				realValue = -realValue;
			}
		}
	}
	else if (uomCode == 9119UL)
	{
		// Degrees (int), minutes (real), hemisphere.  Not really used in the
		// tables per se; the tables usually use double field type for numeric
		// values and the 9110 & 9111 types are usually used.
		bool minus (false);
		long degrees;
		double minutes;

		// Get a copy of the field we are to convert in a place where we
		// modify same.  Not really necessary, but what the heck.
		wcsncpy (wrkBufr,field,wcCount (wrkBufr));
		wrkBufr[wcCount(wrkBufr) - 1] = L'\0';

		// Need to extract the hemisphere (i.e. sign).  We'll skip any
		// white space which may be there.
		ok = true;
		degrees = 0L;
		minutes = 0.0;
		endPtr = wrkBufr;
		while (iswspace (*endPtr)) endPtr++;
		switch (*endPtr) {
		case L'N':	minus = false; break;
		case L'n':	minus = false; break;
		case L'E':	minus = false; break;
		case L'e':	minus = false; break;
		case L'S':	minus = true;  break;
		case L's':	minus = true;  break;
		case L'W':	minus = true;  break;
		case L'w':	minus = true;  break;
		default:
			ok = false;
			break;
		}
		if (ok)
		{
			degrees = wcstol (wrkBufr,&endPtr,10);
			ok = (degrees >= 0L && degrees <= 360L);
			if (ok)
			{
				minus = degrees < 0L;
				if (minus)
				{
					degrees = -degrees;
				}
			}
		}
		if (ok && *endPtr != L'\0')
		{
			minutes = wcstod (endPtr,&endPtr);
			ok = (*endPtr == L'\0') && (minutes >= 0.0 && minutes < 60.0);
		}
		if (ok)
		{
			realValue = static_cast<double>(degrees) + minutes / 60.0;
			if (minus)
			{
				realValue = -realValue;
			}
		}
	}
	if (ok)
	{
		result = realValue;
	}
	return ok;
	//lint -restore
}
// Perhaps the following function should also return the vertical orientation,
// but the orientation for all systems I've ever seen has always been "up".
//lint -save -esym(550,axisThree)	  (axisThree set, but not used)
bool TcsEpsgDataSetV6::GetCoordsysQuad (short& quad,TcsEpsgCode& horzUom,TcsEpsgCode& vertUom,const TcsEpsgCode& crsEpsgCode) const
{
	bool ok;
	bool ok3 (false);
	bool swap (false);

	short myQuad;
	unsigned recordNumber;

	TcsEpsgCode sysEpsgCode;
	TcsEpsgCode uomCodeOne;
	TcsEpsgCode uomCodeTwo;
	TcsEpsgCode uomCodeThree;

	EcsOrientation axisOne (epsgOrntUnknown);
	EcsOrientation axisTwo (epsgOrntUnknown);
	EcsOrientation axisThree (epsgOrntUnknown);
	EcsOrientation axisTmp;

	std::wstring fldData;

	myQuad = 0;
	const TcsEpsgTable* crsTblPtr = GetTablePtr (epsgTblReferenceSystem);
	const TcsEpsgTable* axsTblPtr = GetTablePtr (epsgTblAxis);
	ok = (crsTblPtr != 0) && (axsTblPtr != 0);
	unsigned invalidRecordNbr = TcsCsvFileBase::GetInvalidRecordNbr ();

	// Prepare for an error of some sort.
	quad = 0;
	horzUom.Invalidate ();
	vertUom.Invalidate ();
	uomCodeOne.Invalidate ();
	uomCodeTwo.Invalidate ();
	uomCodeThree.Invalidate ();

	// OK, extract the information the best we can.  In the event of a two
	// dimensional system, this function still returns true, but vertUom is
	// left in the Invalid state.	
	if (ok)
	{
		// Get the Coordinate System Code (different from the CRS code).
		ok = crsTblPtr->GetAsEpsgCode (sysEpsgCode,crsEpsgCode,epsgFldCoordSysCode);		//lint !e613   (possible use of null pointer)
		if (ok)
		{
			// We have the coordinate system code; we need to get the first two
			// axes out of the table.  Note that we rely heavily on the sorted
			// order of the table here.  That is, the first one we encounter
			// is expected to have an "Order" value of 1. 
			recordNumber = axsTblPtr->EpsgLocateFirst (epsgFldCoordSysCode,sysEpsgCode);
			ok = (recordNumber != invalidRecordNbr);
			if (ok)
			{
				ok = axsTblPtr->GetField (fldData,recordNumber,epsgFldCoordAxisOrientation);
				if (ok)
				{
					axisOne = GetOrientation (fldData.c_str ());
				}
				if (ok)
				{
					ok = axsTblPtr->GetAsEpsgCode (uomCodeOne,recordNumber,epsgFldUomCode);
				}
			}
			if (ok)
			{
				recordNumber = axsTblPtr->EpsgLocateNext (recordNumber,epsgFldCoordSysCode,sysEpsgCode);
				ok = (recordNumber != invalidRecordNbr);
				if (ok)
				{
					ok = axsTblPtr->GetField (fldData,recordNumber,epsgFldCoordAxisOrientation);
					if (ok)
					{
						axisTwo = GetOrientation (fldData.c_str ());
					}
					if (ok)
					{
						ok = axsTblPtr->GetAsEpsgCode (uomCodeTwo,recordNumber,epsgFldUomCode);
					}
				}
			}
			if (ok)
			{
				// We get the third axis if it's there, even if we don't do
				// anything with it.
				recordNumber = axsTblPtr->EpsgLocateNext (recordNumber,epsgFldCoordSysCode,sysEpsgCode);
				ok3 = (recordNumber != invalidRecordNbr);
				if (ok3)
				{
					ok3 = axsTblPtr->GetField (fldData,recordNumber,epsgFldCoordAxisOrientation);
					if (ok3)
					{
						axisThree = GetOrientation (fldData.c_str ());
					}
					if (ok3)
					{
						ok3 = axsTblPtr->GetAsEpsgCode (uomCodeThree,recordNumber,epsgFldUomCode);
					}
				}
			}
		}
	}
	if (ok)
	{
		ok  = (axisOne != epsgOrntUnknown) && (axisTwo != epsgOrntUnknown);
		ok &= (uomCodeOne.IsValid () && uomCodeTwo.IsValid ());
	}
	if (ok)
	{
		swap = (axisOne == epsgOrntNorth) || (axisOne == epsgOrntSouth);
		if (swap)
		{
			axisTmp = axisOne;
			axisOne = axisTwo;
			axisTwo = axisTmp;
		}

		if (axisOne == epsgOrntEast && axisTwo == epsgOrntNorth)
		{
			myQuad = 1;
		}
		else if (axisOne == epsgOrntWest && axisTwo == epsgOrntNorth)
		{
			myQuad = 2;
		}
		else if (axisOne == epsgOrntWest && axisTwo == epsgOrntSouth)
		{
			myQuad = 3;
		}
		else if (axisOne == epsgOrntEast && axisTwo == epsgOrntSouth)
		{
			myQuad = 4;
		}
	}
	ok = (myQuad != 0);
	if (ok && swap)
	{
		myQuad = -myQuad;
	}
	if (ok)
	{
		ok = (uomCodeOne == uomCodeTwo);
	}
	if (ok)
	{
		quad = myQuad;
		horzUom = uomCodeOne;
		if (ok3)
		{
			vertUom = uomCodeThree;
		}
	}
	return ok;
}
//lint -restore
