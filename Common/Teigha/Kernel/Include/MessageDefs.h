/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2018, Open Design Alliance (the "Alliance"). 
// All rights reserved. 
// 
// This software and its documentation and related materials are owned by 
// the Alliance. The software may only be incorporated into application 
// programs owned by members of the Alliance, subject to a signed 
// Membership Agreement and Supplemental Software License Agreement with the
// Alliance. The structure and organization of this software are the valuable  
// trade secrets of the Alliance and its suppliers. The software is also 
// protected by copyright law and international treaty provisions. Application  
// programs incorporating this software must include the following statement 
// with their copyright notices:
//   
//   This application incorporates Teigha(R) software pursuant to a license 
//   agreement with Open Design Alliance.
//   Teigha(R) Copyright (C) 2002-2018 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 
// acknowledge and accept the above terms.
///////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////
// Message codes definition container


//              Code                        Message string
OD_MESSAGE_DEF( sidLayoutNo,                OD_T("Layout%d"))
OD_MESSAGE_DEF( sidDwgFile,                 OD_T(".dwg file"))
OD_MESSAGE_DEF( sidDb,                      OD_T("Database"))
OD_MESSAGE_DEF( sidDbHeader,                OD_T("Database header"))
OD_MESSAGE_DEF( sidObjectsAudited,          OD_T("%d objects audited")) // param: Number of objects
OD_MESSAGE_DEF( sidErrorsFound,             OD_T("Total errors found during audit %d, fixed %d")) // param: Number of errors
OD_MESSAGE_DEF( sidDictionary,              OD_T("%ls Dictionary")) // param: Dictionary name
OD_MESSAGE_DEF( sidTable,                   OD_T("%ls Table")) // param: Table name
OD_MESSAGE_DEF( sidDbLayerZeroMissed,       OD_T("Layer 0 missing"))
OD_MESSAGE_DEF( sidDbLayerZeroBadIndex,     OD_T("Layer 0 has invalid index %d"))
OD_MESSAGE_DEF( sidDbLtMissed,              OD_T("LineType %ls missing")) // param: line type name
OD_MESSAGE_DEF( sidDbLtContsBadIndex,       OD_T("LineType Continuous has invalid index %d"))
OD_MESSAGE_DEF( sidDbSymbolTableMissed,     OD_T("Missing Symbol Table \"%ls\""))
OD_MESSAGE_DEF( sidDbRegAppAcadMissed,      OD_T("RegApp ACAD missing"))
OD_MESSAGE_DEF( sidDbRegAppAcadBadIndex,    OD_T("RegApp ACAD has invalid index %d"))
OD_MESSAGE_DEF( sidDbTxtStStandardMissed,   OD_T("TextStyle Standard missing"))
OD_MESSAGE_DEF( sidDbDimStStandardMissed,   OD_T("DimensionStyle Standard missing"))
OD_MESSAGE_DEF( sidDbMlStStandardMissed,    OD_T("MLineStyle Standard missing"))
OD_MESSAGE_DEF( sidDbVpActiveMissed,        OD_T("Viewport *Active missing"))
OD_MESSAGE_DEF( sidDbLtByBlockName,         OD_T("LineType ByBlock has invalid name \"%ls\""))
OD_MESSAGE_DEF( sidDbLtByBlockInList,       OD_T("LineType ByBlock placed in ordinary list"))
OD_MESSAGE_DEF( sidDbLtByLayerName,         OD_T("LineType ByLayer has invalid name %ls"))
OD_MESSAGE_DEF( sidDbLtByLayerInList,       OD_T("LineType ByLayer placed in ordinary list"))
OD_MESSAGE_DEF( sidDbNamedObjectsDictionaryMissed,OD_T("Named objects Dictionary missing"))
OD_MESSAGE_DEF( sidDbDictionaryMissed,      OD_T("%ls dictionary missing"))   // param: dictionary name (ACAD_MLSTYLE, ACAD_GROUP, ...)
OD_MESSAGE_DEF( sidInvalidLayoutId,         OD_T("Layout Id %ls")) // param: LayoutId
OD_MESSAGE_DEF( sidInvalidLayoutAssoc,      OD_T("Layout %ls associated with invalid BlockTableRecord")) // param: LayoutId
OD_MESSAGE_DEF( sidInvalidLayoutBackPtr,    OD_T("Layout %ls associated with BlockTableRecord %ls that doesn't point back")) // param: LayoutId, RecordId
OD_MESSAGE_DEF( sidSysVar,                  OD_T("System Variable \"%ls\"")) // param: Name of system variable
OD_MESSAGE_DEF( sidLayoutOverallVp,         OD_T("Overall Viewport Id %ls"))
OD_MESSAGE_DEF( sidInvalidObjectId,         OD_T("Object Id"))
OD_MESSAGE_DEF( sidIsNotTextStyleRec,       OD_T("Is not a TextStyleTableRecord"))
OD_MESSAGE_DEF( sidIsNotInTable,            OD_T("Is not in Table"))
OD_MESSAGE_DEF( sidIsShapeFile,             OD_T("Is Shape file"))
OD_MESSAGE_DEF( sidVxRecRef,                OD_T("VXTableRecord reference"))
OD_MESSAGE_DEF( sidGenNamePrefix,           OD_T("$TD_AUDIT_GENERATED"))
OD_MESSAGE_DEF( sidGenNameSymUtilPrefix,    OD_T("$TD_SYMUTILREPAIR_GENERATED"))
// General validation
OD_MESSAGE_DEF( sidVarValidDouble,          OD_T("Not %g"))
OD_MESSAGE_DEF( sidVarValidInt,             OD_T("Not %d"))
OD_MESSAGE_DEF( sidVarValidPositive,        OD_T("Not >0"))
OD_MESSAGE_DEF( sidVarValidZeroPositive,    OD_T("Not >=0"))
OD_MESSAGE_DEF( sidVarValidNegative,        OD_T("Not <0"))
OD_MESSAGE_DEF( sidVarValidZeroNegative,    OD_T("Not <=0"))
OD_MESSAGE_DEF( sidVarValidNonZero,         OD_T("Equal to 0"))
OD_MESSAGE_DEF( sidVarValidRangeInt,        OD_T("Not %d..%d"))
OD_MESSAGE_DEF( sidVarValidRangeDouble,     OD_T("Not %g..%g"))
OD_MESSAGE_DEF( sidVarValidRangeDegree,     OD_T("Not %f..%f degrees"))
OD_MESSAGE_DEF( sidVarValidMinInt,          OD_T("Not >= %d"))
OD_MESSAGE_DEF( sidVarValidMaxInt,          OD_T("Not <= %d"))
OD_MESSAGE_DEF( sidVarValidInvalid,         OD_T("Invalid"))
OD_MESSAGE_DEF( sidVarValid,                OD_T("%ls"))
OD_MESSAGE_DEF( sidVarValidInts2,           OD_T("Not %d or %d"))
OD_MESSAGE_DEF( sidVarValidInts4,           OD_T("Not %d, %d, %d, or %d"))
OD_MESSAGE_DEF( sidVarValidDisparity,       OD_T("Disparity"))
// General default values
OD_MESSAGE_DEF( sidVarDefDouble,            OD_T("Set to %g"))
OD_MESSAGE_DEF( sidVarDefInt,               OD_T("Set to %d"))
OD_MESSAGE_DEF( sidVarDefObj,               OD_T("Set to %ls"))
OD_MESSAGE_DEF( sidVarDefDegree,            OD_T("Set to %f degrees"))
OD_MESSAGE_DEF( sidVarDefColor,             OD_T("Set to %d"))
OD_MESSAGE_DEF( sidVarDefLayer,             OD_T("Set to default Layer \"%ls\""))
OD_MESSAGE_DEF( sidVarDefLinetype,          OD_T("Set to default LineType \"%ls\""))
OD_MESSAGE_DEF( sidVarDefZAxis,             OD_T("Set to (0 0 1)"))
OD_MESSAGE_DEF( sidVarDefViewMode,          OD_T("Set to %#x"))
OD_MESSAGE_DEF( sidVarDefFalse,             OD_T("Set to False"))
OD_MESSAGE_DEF( sidVarDefTrue,              OD_T("Set to True"))
OD_MESSAGE_DEF( sidVarDefNull,              OD_T("Set to Null"))
OD_MESSAGE_DEF( sidVarDefDefault,           OD_T("Set to Default"))
OD_MESSAGE_DEF( sidVarDefCreate,            OD_T("Created"))
OD_MESSAGE_DEF( sidVarDefRemove,            OD_T("Removed"))
OD_MESSAGE_DEF( sidVarDefUpdate,            OD_T("Updated"))
OD_MESSAGE_DEF( sidVarDefRepair,            OD_T("Repaired"))
OD_MESSAGE_DEF( sidVarDefUnerase,           OD_T("UnErased"))
OD_MESSAGE_DEF( sidVarDefLayerColorMethod,  OD_T("Set to ByACI"))
// Color
OD_MESSAGE_DEF( sidColorInvalidIndex,       OD_T("Color index %d"))
OD_MESSAGE_DEF( sidColorInvalidMethod,      OD_T("Color method %d"))
OD_MESSAGE_DEF( sidValidColorIndices,       OD_T("Not 1..255, ByBlock, or ByLayer"))
OD_MESSAGE_DEF( sidValidLayerColorIndices,  OD_T("Not 1..255"))
OD_MESSAGE_DEF( sidValidColorMethods,       OD_T("Not ByLayer, ByBlock, ByPen, Foreground, LayerOff, LayerFrozen, None, ByColor, or ByACI"))
// Objects
OD_MESSAGE_DEF( sidObjProp,                 OD_T("%ls")) // param: Name of object property
OD_MESSAGE_DEF( sidObjPropInt,              OD_T("%ls - %d"))
OD_MESSAGE_DEF( sidObjPropDouble,           OD_T("%ls - %g"))
OD_MESSAGE_DEF( sidObjPropStr,              OD_T("%ls - \"%ls\""))
OD_MESSAGE_DEF( sidObjPropColor,            OD_T("Color value"))
OD_MESSAGE_DEF( sidObjPropLayerId,          OD_T("Layer Id %ls"))
OD_MESSAGE_DEF( sidObjPropLinetypeId,       OD_T("LineType Id %ls"))
OD_MESSAGE_DEF( sidObjPropLinetypeName,     OD_T("LineType name \"%ls\""))
OD_MESSAGE_DEF( sidObjPropPlotStyleNameId,  OD_T("PlotStyleName Id %ls"))
OD_MESSAGE_DEF( sidObjPropReactorId,        OD_T("Reactor Id %ls"))
OD_MESSAGE_DEF( sidObjPropExtDicId,         OD_T("Extension dictionary Id %ls"))
OD_MESSAGE_DEF( sidObjPropOwnerId,          OD_T("Owner Id %ls"))
OD_MESSAGE_DEF( sidObjPropClassVersion,     OD_T("Class version %d"))
OD_MESSAGE_DEF( sidXDataRegAppId,           OD_T("XData item RegApp \"%ls\""))
OD_MESSAGE_DEF( sidXDataHandle,             OD_T("XData handle %ls"))
OD_MESSAGE_DEF( sidXDataInvalidGrcode,      OD_T("XData contains invalid groupcode (%d)"))
OD_MESSAGE_DEF( sidXdataSizeExceeded,       OD_T("XData size (%d) exceeded"))
OD_MESSAGE_DEF( sidXDataGrcode3doubles,     OD_T("XData %d (%g,%g,%g)"))
OD_MESSAGE_DEF( sidXDataGrcode1double,      OD_T("XData %d (%g)"))
OD_MESSAGE_DEF( sidXDataUnpairedBrackets,   OD_T("XData contains unpaired \"{\" \"}\" brackets"))
OD_MESSAGE_DEF( sidInvalidChildId,          OD_T("Unable to open entry object %ls"))
OD_MESSAGE_DEF( sidDicClonFlag,             OD_T("Duplicate record cloning (merge) flag (%d)"))
OD_MESSAGE_DEF( sidObjPropColorByLayer,     OD_T("Set to ByLayer"))
OD_MESSAGE_DEF( sidObjPropPlotStByLayer,    OD_T("Set to ByLayer"))
OD_MESSAGE_DEF( sidObjPropMaterialId,       OD_T("Material Id %ls"))
OD_MESSAGE_DEF( sidObjPropMaterialByLayer,  OD_T("Set to ByLayer"))
OD_MESSAGE_DEF( sidObjPropMaterialGlobal,   OD_T("Set to Global"))

// SortEntsTable
OD_MESSAGE_DEF( sidFoundDuplicateKeys,      OD_T("Duplicate keys were found"))

// Hatch
OD_MESSAGE_DEF( sidHatchAssocInvalid,       OD_T("Associative entity %ls"))
OD_MESSAGE_DEF( sidHatchAssocHatch,         OD_T("Associative entity %ls is Hatch"))
OD_MESSAGE_DEF( sidHatchAssocReactor,       OD_T("Associative entity %ls missing Hatch as reactor"))
OD_MESSAGE_DEF( sidHatchRemoveAssoc,        OD_T("Associativity removed"))
OD_MESSAGE_DEF( sidHatchAssocFlag,          OD_T("Associative flag is true but associative entities are missing"))
OD_MESSAGE_DEF( sidHatchNoPatternDef,       OD_T("No Hatch pattern definition"))

OD_MESSAGE_DEF( sidHatchConflictGradFlags,  OD_T("Gradient Hatch with solid flag false and/or pattern type not kPreDefined"))
OD_MESSAGE_DEF( sidHatchGradFlagsFixed,     OD_T("Fixed."))

OD_MESSAGE_DEF( sidInvalidHatchBoundaryData,   OD_T("Hatch has invalid boundary data"))
OD_MESSAGE_DEF( sidNoClosedHatchBoundaryLoops, OD_T("No Closed Hatch Boundary Loops"))

OD_MESSAGE_DEF( sidHatchBndDupVerts,        OD_T("Polyline Hatch boundary has duplicated vertices"))

// MText
OD_MESSAGE_DEF( sidMTextLinespacingStyle,   OD_T("Line spacing style (%d)"))
OD_MESSAGE_DEF( sidMTextAttachPt,           OD_T("Attachment point (%d)"))
OD_MESSAGE_DEF( sidMTextDrawDir,            OD_T("Drawing direction (%d)"))
OD_MESSAGE_DEF( sidMTextLinespacingFactor,  OD_T("Line spacing factor (%g)"))
OD_MESSAGE_DEF( sidMTextXDir,               OD_T("X-axis direction"))
OD_MESSAGE_DEF( sidMTextExtDic,             OD_T("Extension dictionary entry \"%ls\" %ls"))
// MLine
OD_MESSAGE_DEF( sidMLineJustification,      OD_T("Justification (%d)"))
OD_MESSAGE_DEF( sidMLineScale,              OD_T("Scale (%g)"))
OD_MESSAGE_DEF( sidMLineStyleId,            OD_T("MLineStyle Id %ls"))
OD_MESSAGE_DEF( sidMLineStyleNumElements,   OD_T("The number of elements in MLineStyle (%d)"))
OD_MESSAGE_DEF( sidMLineStFillColor,        OD_T("Fill color"))
OD_MESSAGE_DEF( sidMLineStStartAngle,       OD_T("Start angle %g"))
OD_MESSAGE_DEF( sidMLineStEndAngle,         OD_T("End angle %g"))
OD_MESSAGE_DEF( sidMLineStSegmentsNum,      OD_T("Segments amount %d"))
OD_MESSAGE_DEF( sidMLineStElemLinetypeId,   OD_T("Element(%d) lineType Id %ls"))
OD_MESSAGE_DEF( sidMLineStElemColor,        OD_T("Element(%d) has invalid color"))
OD_MESSAGE_DEF( sidMLineStyleName,          OD_T("MLineStyle name \"%ls\""))

// Raster Image
OD_MESSAGE_DEF( sidNumClipPoints,           OD_T("Number of points in clipping boundary %d"))
OD_MESSAGE_DEF( sidImgInvalidReactorId,     OD_T("An image entity's reactor \"%ls\""))

// View, Vp
OD_MESSAGE_DEF( sidViTilemodeCorners,       OD_T("Tilemode Viewport corners (%g,%g)..(%g,%g)"))
OD_MESSAGE_DEF( sidViTilemodeCornersValid,  OD_T("Not within (0,0)..(1,1)"))
OD_MESSAGE_DEF( sidViTilemodeCornersDef,    OD_T("Set to (0,0)..(1,1)"))
OD_MESSAGE_DEF( sidViSnapXIncr,             OD_T("Snap X increment %g"))
OD_MESSAGE_DEF( sidViSnapYIncr,             OD_T("Snap Y increment %g"))
OD_MESSAGE_DEF( sidViGridXIncr,             OD_T("Grid X increment %g"))
OD_MESSAGE_DEF( sidViGridYIncr,             OD_T("Grid Y increment %g"))
OD_MESSAGE_DEF( sidViViewWidth,             OD_T("View width %g"))
OD_MESSAGE_DEF( sidViViewHeight,            OD_T("View height %g"))
OD_MESSAGE_DEF( sidViLensLength,            OD_T("Lens length %g"))
OD_MESSAGE_DEF( sidViViewDirection,         OD_T("View direction"))
OD_MESSAGE_DEF( sidViViewMode,              OD_T("View mode %d"))
OD_MESSAGE_DEF( sidViRenderMode,            OD_T("Render mode %d"))
OD_MESSAGE_DEF( sidViTwistAngle,            OD_T("Twist angle %g"))
OD_MESSAGE_DEF( sidVpHeight,                OD_T("Height (%g)"))
OD_MESSAGE_DEF( sidVpWidth,                 OD_T("Width (%g)"))
OD_MESSAGE_DEF( sidVpUCS,                   OD_T("Viewport saved UCS"))
OD_MESSAGE_DEF( sidVpUCSInvalidXAxis,       OD_T("UCS X-Axis non-unit length"))
OD_MESSAGE_DEF( sidVpUCSInvalidYAxis,       OD_T("UCS Y-Axis non-unit length"))
OD_MESSAGE_DEF( sidVpUCSNonPerpendicular,   OD_T("Non-perpendicular UCS X and Y axes"))
OD_MESSAGE_DEF( sidVpCenter,                OD_T("Viewport center (%g. %g)"))
OD_MESSAGE_DEF( sidVpCenterDef,             OD_T("Set to (0, 0)"))
OD_MESSAGE_DEF( sidResized,                 OD_T("Resized"))
// Tables, recs
OD_MESSAGE_DEF( sidTblVxOneEmptyRec,        OD_T("There is only one empty VX record"))
OD_MESSAGE_DEF( sidTblBlockBeginInvalid,    OD_T("BlockBegin Id"))
OD_MESSAGE_DEF( sidTblBlockBeginErased,     OD_T("BlockBegin Id is erased"))
OD_MESSAGE_DEF( sidTblBlockBeginNotOurs,    OD_T("BlockBegin owner Id is invalid %ls"))
OD_MESSAGE_DEF( sidTblBlockEndInvalid,      OD_T("BlockEnd Id"))
OD_MESSAGE_DEF( sidTblBlockEndErased,       OD_T("BlockEnd Id is erased"))
OD_MESSAGE_DEF( sidTblBlockEndNotOurs,      OD_T("BlockEnd owner Id is invalid %ls"))
OD_MESSAGE_DEF( sidTblBlockInvalidSortents, OD_T("Object Id %ls is not an OdDbSortentsTable object"))
OD_MESSAGE_DEF( sidRecSpaceDuplicateEntry,  OD_T("Duplicate entry of \"%ls\" in BlockRecord Table"))
OD_MESSAGE_DEF( sidRecSpaceNameInvalid,     OD_T("\"%ls\" record name \"%ls\""))
OD_MESSAGE_DEF( sidRecNameEmpty,            OD_T("Record name is empty"))
OD_MESSAGE_DEF( sidRecNameInvalidChars,     OD_T("Character(s) in record name"))
OD_MESSAGE_DEF( sidRecCommentInvalidChars,  OD_T("Character(s) in record comment"))
OD_MESSAGE_DEF( sidRecDuplicateName,        OD_T("Duplicate record name \"%ls\""))
OD_MESSAGE_DEF( sidRecXRefBlockInvalid,     OD_T("Is externally dependent on an Xref, but Xref BlockRecord (%ls)"))
OD_MESSAGE_DEF( sidRecXRefBlockIdNotNull,   OD_T("Is not externally dependent on an Xref, but XRefBlockId (%ls) is not Null"))
OD_MESSAGE_DEF( sidRecXRefBlockIdInvalid,   OD_T("Is externally dependent on an Xref, but XRefBlockId (%ls)"))
OD_MESSAGE_DEF( sidRecXRefInvalidFlag,      OD_T("Is not externally dependent on an Xref, but XRefBlockId (%ls) is valid and name has vertical bar"))
OD_MESSAGE_DEF( sidRecXRefDepNameInvalid,   OD_T("Is externally dependent on an Xref, but name (%ls) has no vertical bar"))
OD_MESSAGE_DEF( sidRecXRefBreakCycle,       OD_T("Breaking circular reference from \"%ls\" to \"%ls\""))
OD_MESSAGE_DEF( sidRecXRefBindNotFound,     OD_T("Cannot bind Xref \"%ls\", it is not found. Please reload or detach it, and try again."))
OD_MESSAGE_DEF( sidRecXRefBindUnloaded,     OD_T("Cannot bind Xref \"%ls\", it is unloaded. Please reload or detach it, and try again."))
OD_MESSAGE_DEF( sidRecNameVertBar,          OD_T("Non Xref-dependent record contains vertical bar."))
OD_MESSAGE_DEF( sidRecRemoveDependence,     OD_T("Removed dependence"))
OD_MESSAGE_DEF( sidRecAttrDefFalse,         OD_T("hasAttributeDefinitions flag is false, but record has AttributeDefinitions"))
OD_MESSAGE_DEF( sidRecAttrDefTrue,          OD_T("hasAttributeDefinitions flag is true, but record has no AttributeDefinitions"))
OD_MESSAGE_DEF( sidRecAttrDefAlwaysFalse,   OD_T("Layout BlockTableRecord has alerted hasAttributeDefinitions flag"))
OD_MESSAGE_DEF( sidRecLtSegmentsNum,        OD_T("Dash count less than 2"))
OD_MESSAGE_DEF( sidRecLtBadSegmentType,     OD_T("Bad complex LineType element type"))
OD_MESSAGE_DEF( sidRecLtContPattern,        OD_T("Continuous pattern length is not zero"))
OD_MESSAGE_DEF( sidRecLtDescription,        OD_T("Description unprintable"))
OD_MESSAGE_DEF( sidRecTxtStLastHeight,      OD_T("Last height %g"))
OD_MESSAGE_DEF( sidRecTxtStScaleFactor,     OD_T("Scale factor %g not in range"))
OD_MESSAGE_DEF( sidRecTxtStTextSize,        OD_T("Text size %g not in range"))
OD_MESSAGE_DEF( sidRecVxNullVp,             OD_T("Viewport Id is Null"))
OD_MESSAGE_DEF( sidRecVxBadVp,              OD_T("Viewport Id"))
OD_MESSAGE_DEF( sidRecvInvalidFileSection,  OD_T("File section \"%ls\" has invalid data"))
OD_MESSAGE_DEF( sidUnknownSysVar,           OD_T("Unknown System Variable %ls"))
OD_MESSAGE_DEF( sidUnknownSymbolTable,      OD_T("Unknown Symbol Table %ls"))
OD_MESSAGE_DEF( sidIgnored,                 OD_T("Ignored"))

// Dimension
OD_MESSAGE_DEF( sidInvalidDimOverridesData, OD_T("Dimension overrides data with group code %d"))
OD_MESSAGE_DEF( sidInvalidDimOverrides,     OD_T("Dimension overrides"))
OD_MESSAGE_DEF( sidDimBlockRotation,        OD_T("Dimension block rotation (%g)"))
OD_MESSAGE_DEF( sidDimObliqueAngle,         OD_T("Dimension oblique angle (%g)"))
OD_MESSAGE_DEF( sidDimTextRotation,         OD_T("Dimension text rotation (%g)"))
OD_MESSAGE_DEF( sidDimTextStyleNull,        OD_T("Dimension TextStyle Id is Null"))
OD_MESSAGE_DEF( sidDimIsNotTextStyleRec,    OD_T("Dimension TextStyleTableRecord"))

// Leader
OD_MESSAGE_DEF( sidLeaderAnntEnbl,          OD_T("Annotation enabled but Id is Null"))
OD_MESSAGE_DEF( sidLeaderAnntDisbl,         OD_T("Annotation disabled but Id is Not Null"))
OD_MESSAGE_DEF( sidLeaderAnntDisable,       OD_T("Disable annotation"))
OD_MESSAGE_DEF( sidLeaderAnntToNull,        OD_T("Set to Null"))
OD_MESSAGE_DEF( sidLeaderAnntId,            OD_T("Annotation Id %ls"))
OD_MESSAGE_DEF( sidDimStyle,                OD_T("DimensionStyle %ls"))
OD_MESSAGE_DEF( sidLeaderArrowHead,         OD_T("Arrow head block"))
// Text
OD_MESSAGE_DEF( sidTextHeight,              OD_T("Height (%g)"))
OD_MESSAGE_DEF( sidTextWidthFactor,         OD_T("WidthFactor (%g)"))
OD_MESSAGE_DEF( sidTextObliqueAngle,        OD_T("ObliqueAngle (%g)"))
OD_MESSAGE_DEF( sidTextTextStyle,           OD_T("TextStyle (%ls)"))
OD_MESSAGE_DEF( sidTextMirrorFlag,          OD_T("Mirroring invalid bits"))
// DimAssoc
OD_MESSAGE_DEF( sidDimAssocDimId,           OD_T("Dimension Id %ls"))
// Group
OD_MESSAGE_DEF( sidGroupEntryReactor,       OD_T("Entity %ls in group does not have group as persistent reactor"))
OD_MESSAGE_DEF( sidGroupDefAddReactor,      OD_T("Add persistent reactor"))
// Polylines
OD_MESSAGE_DEF( sidPolyVertNumErr,          OD_T("Has only %d vertices"))
OD_MESSAGE_DEF( sidPolyVertNumAdd,          OD_T("%d vertices added"))
OD_MESSAGE_DEF( sidPolyVertLayerMatch,      OD_T("Layer %ls doesn't match to owner"))
OD_MESSAGE_DEF( sidPolyVertLinetypeMatch,   OD_T("LineType %ls doesn't match to owner"))
OD_MESSAGE_DEF( sidPolyVertColorMatch,      OD_T("Color %d doesn't match to owner"))
OD_MESSAGE_DEF( sidPolyVertValid,           OD_T("Should match to owner"))
OD_MESSAGE_DEF( sidPolylineSurfType,        OD_T("Curves and smooth surface type (%d)"))
OD_MESSAGE_DEF( sidPolyBulgeErr,            OD_T("Has %d vertices, but %d bulges"))
OD_MESSAGE_DEF( sidPolyBulgeValid,          OD_T("num bulges > num vertices"))
OD_MESSAGE_DEF( sidPolyBulgeDef,            OD_T("Set num bulges to num vertices"))
OD_MESSAGE_DEF( sidPolyWidthErr,            OD_T("Has %d vertices, but %d widths"))
OD_MESSAGE_DEF( sidPolyWidthValid,          OD_T("num widths > num vertices"))
OD_MESSAGE_DEF( sidPolyWidthDef,            OD_T("Set num widths to num vertices"))
OD_MESSAGE_DEF( sidPolyIdentifiersErr,      OD_T("Has %d vertices, but %d vertex identifiers"))
OD_MESSAGE_DEF( sidPolyIdentifiersValid,    OD_T("num identifiers > num vertices"))
OD_MESSAGE_DEF( sidPolyIdentifiersDef,      OD_T("Set num identifiers to num vertices"))
OD_MESSAGE_DEF( sidPolyFitType,             OD_T("Fit type"))
// Containers
OD_MESSAGE_DEF( sidCntrEntryInvalid,        OD_T("Entry object %ls"))
OD_MESSAGE_DEF( sidCntrCrossRefInvalid,     OD_T("Cross references %ls in entities list"))
OD_MESSAGE_DEF( sidCntrMultipleEntry,       OD_T("Multiple entries of reference %ls"))
OD_MESSAGE_DEF( sidCntrLastRefMatch,        OD_T("Last entity %ls does not match to last reference %ls in container"))
OD_MESSAGE_DEF( sidCntrEntryTypeInvalid,    OD_T("Invalid use of entity %ls"))
OD_MESSAGE_DEF( sidCntrSubentTypeInvalid,   OD_T("Invalid subentity type %ls"))
OD_MESSAGE_DEF( sidPolyfaceMeshVertexIndex, OD_T("Invalid vertex index"))
OD_MESSAGE_DEF( sidPolyfaceMeshHasNoFaces,  OD_T("Polyface Mesh has no valid faces"))
OD_MESSAGE_DEF( sidPolyfaceMeshVertexAfterFace, OD_T("Polyface Mesh vertex after face"))
OD_MESSAGE_DEF( sidPolyfaceMeshVertexNumInvalid, OD_T("Polyface Mesh vertex's number"))
OD_MESSAGE_DEF( sidPolyfaceMeshFaceNumInvalid  , OD_T("Polyface Mesh face's number"))

// Block reference
OD_MESSAGE_DEF( sidBlkRefBlockRecErased,    OD_T("BlockTableRecord %ls is erased"))
OD_MESSAGE_DEF( sidBlkRefBlockRecInvalid,   OD_T("BlockTableRecord %ls"))
OD_MESSAGE_DEF( sidMissingBlockRecord,      OD_T("BlockTableRecord %ls is missing"))

OD_MESSAGE_DEF( sidBlkRefScaleHasZero,      OD_T("Scale has zero component (%g %g %g)"))
OD_MESSAGE_DEF( sidBlkRefScaleValid,        OD_T("Scale must have no zero components"))

// Shapes
OD_MESSAGE_DEF( sidUnknownShapeName,        OD_T("Can't resolve Shape name \"%ls\""))
OD_MESSAGE_DEF( sidNullShapeFile,           OD_T("Shape Style (font file) is not set"))
OD_MESSAGE_DEF( sidNullShapeNumber,         OD_T("Shape number is not set"))
OD_MESSAGE_DEF( sidCantGetShapeName,        OD_T("Can't get Shape name without \"%ls\""))

// Ellipse
OD_MESSAGE_DEF( sidNormalDirectionIsInvalid,OD_T("Normal direction (%g %g %g)"))
OD_MESSAGE_DEF( sidPerpToMajor,             OD_T("Must be perpendicular to Major Axis"))
OD_MESSAGE_DEF( sidAdjusted,                OD_T("Adjusted"))
OD_MESSAGE_DEF( sidEllipseRatio,            OD_T("Ratio %lf"))
OD_MESSAGE_DEF( sidEllipseEqualAngle,       OD_T("Start angle (%lf) and end angle are equal (degenerate geometry)"))

// Spline
OD_MESSAGE_DEF( sidCoincidentControlPoints, OD_T("Control points of spline are coincident"))
OD_MESSAGE_DEF( sidInvalidKnotVector,       OD_T("Knot vector of spline"))

// XRecord
OD_MESSAGE_DEF( sidXRecordInvalidGrcode,    OD_T("XRecord contains invalid groupcode"))
OD_MESSAGE_DEF( sidXRecordHandle,           OD_T("XRecord handle %ls"))
OD_MESSAGE_DEF( sidXRecord3doubles,         OD_T("XRecord %d (%g,%g,%g)"))
OD_MESSAGE_DEF( sidXRecord1double,          OD_T("XRecord %d (%g)"))

// MInsertBlock
OD_MESSAGE_DEF( sidMInsertNumRows,          OD_T("Number of rows (%d)"))
OD_MESSAGE_DEF( sidMInsertNumColumns,       OD_T("Number of columns (%d)"))

// 2d Entities
OD_MESSAGE_DEF( sidNormalized,              OD_T("Normalized"))
OD_MESSAGE_DEF( sidThickness,               OD_T("Invalid Thickness"))
OD_MESSAGE_DEF( sidZeroed,                  OD_T("Set to 0"))

// Recover
OD_MESSAGE_DEF( sidRecvStart,               OD_T("Recover .dwg file."))
OD_MESSAGE_DEF( sidRecvSecLocHeader,        OD_T("Header section-locator"))
OD_MESSAGE_DEF( sidRecvSecLocClasses,       OD_T("Classes section-locator"))
OD_MESSAGE_DEF( sidRecvSecLocObjectMap,     OD_T("ObjectMap section-locator"))
OD_MESSAGE_DEF( sidRecvSecLoc,              OD_T("Section-locator records"))
OD_MESSAGE_DEF( sidRecvBadCrc,              OD_T("CRC does not match in %ls"))
OD_MESSAGE_DEF( sidRecvSkip,                OD_T("Skip"))
OD_MESSAGE_DEF( sidRecvBadAddr,             OD_T("%ls address %08X"))
OD_MESSAGE_DEF( sidRecvFileTooShort,        OD_T("File too short"))
OD_MESSAGE_DEF( sidRecvFailed,              OD_T("Failed"))
OD_MESSAGE_DEF( sidRecvFinishFailed,        OD_T("Recovery failed."))
OD_MESSAGE_DEF( sidRecvStartAudit,          OD_T("Audit recovered Database."))
OD_MESSAGE_DEF( sidRecvRestoreObjectMap,    OD_T("Handle table (ObjectMap) was reconstructed from objects."))
OD_MESSAGE_DEF( sidRecvTotalObs,            OD_T("Total objects in the handle table found %d."))
OD_MESSAGE_DEF( sidRecvLoadedObs,           OD_T("Loaded objects %d (with errors %d). Invalid objects %d."))
OD_MESSAGE_DEF( sidRecvErrorsFound,         OD_T("Total errors found during recover: %d."))
OD_MESSAGE_DEF( sidRecvDbHeaderErr,         OD_T("Database header has errors"))
OD_MESSAGE_DEF( sidRecvSections,            OD_T("Sections"))
OD_MESSAGE_DEF( sidRecvObjectMap,           OD_T("ObjectMap"))
OD_MESSAGE_DEF( sidRecvHeader,              OD_T("Header"))
OD_MESSAGE_DEF( sidRecv2ndHeader,           OD_T("Second header"))
OD_MESSAGE_DEF( sidRecvClasses,             OD_T("Classes"))
OD_MESSAGE_DEF( sidRecvInvalidHandseed,     OD_T("Database handseed (%ls) is less than next available handle (%ls)"))
OD_MESSAGE_DEF( sidRecvObject,              OD_T("Object %ls"))
OD_MESSAGE_DEF( sidRecvObjFreeSpaceBadSize, OD_T("ObjFreeSpace data does not match section-locator ObjFreeSpace size"))
OD_MESSAGE_DEF( sidRecvObjFreeSpaceErr,     OD_T("ObjFreeSpace section"))
OD_MESSAGE_DEF( sidRecvTemplateErr,         OD_T("Template section"))
OD_MESSAGE_DEF( sidRecvPreviewImgErr,       OD_T("Preview image"))
OD_MESSAGE_DEF( sidRecvUnknownSecErr,       OD_T("Unknown section"))
OD_MESSAGE_DEF( sidRecv2ndHdrErr,           OD_T("Second header after ObjFreeSpace data"))
OD_MESSAGE_DEF( sidRecv2ndHdrAddrErr,       OD_T("Second header by address from file header (ObjFreeSpace end)"))
OD_MESSAGE_DEF( sidRecvSs2ndHdr,            OD_T("Start sentinel of SecondHeader not found"))
OD_MESSAGE_DEF( sidRecvResize,              OD_T("Resize"))
OD_MESSAGE_DEF( sidRecvReconstruct,         OD_T("Reconstruct"))
OD_MESSAGE_DEF( sidRecvObjectMapErr,        OD_T("ObjectMap has errors"))
OD_MESSAGE_DEF( sidRecvRecover,             OD_T("Recover"))
OD_MESSAGE_DEF( sidRecvAddObj,              OD_T("Add"))
OD_MESSAGE_DEF( sidRecvBadRemainsEntry,     OD_T("Remains entry"))
OD_MESSAGE_DEF( sidRecvClassMissed,         OD_T("Class %ls is missing"))
OD_MESSAGE_DEF( sidRecvAddClass,            OD_T("Add class"))
OD_MESSAGE_DEF( sidRecvClassListErr,        OD_T("Classes list"))
OD_MESSAGE_DEF( sidRecvObjInvalid,          OD_T("Object %ls"))
OD_MESSAGE_DEF( sidRecvObjInvalidOffset,    OD_T("Object %ls has invalid offset %08X"))
OD_MESSAGE_DEF( sidRecvObjInvalidSize,      OD_T("Object %ls has invalid size %d"))
OD_MESSAGE_DEF( sidRecvObjAddrInvalidSize,  OD_T("Object at address %08X has invalid size %d"))
OD_MESSAGE_DEF( sidRecvObjHandleUnmatched,  OD_T("Object %ls has handle different from ObjectMap Handle %ls"))
OD_MESSAGE_DEF( sidRecvObjInvalidData,      OD_T("Object %ls has invalid data"))
OD_MESSAGE_DEF( sidRecvSkippedRec,          OD_T("Record %ls is skipped"))
OD_MESSAGE_DEF( sidRecvRootObjInvalid,      OD_T("Root object %ls")) // class name
OD_MESSAGE_DEF( sidRecvBadRootTables,       OD_T("Unable to recover root tables"))
OD_MESSAGE_DEF( sidRecvBadDictionary,       OD_T("%ls (%ls)"))  // 1 - dic class, 2 - dic name
OD_MESSAGE_DEF( sidRecvBadRecClass,         OD_T("Invalid class type of %ls"))
OD_MESSAGE_DEF( sidRecvBadMlineStandard,    OD_T("MLineStyle Standard"))
OD_MESSAGE_DEF( sidRecvBadPlotStNormal,     OD_T("PlotStyle Normal"))
OD_MESSAGE_DEF( sidRecvRegAppAcad,          OD_T("RegApp ACAD"))
OD_MESSAGE_DEF( sidRecvLinetypeByBlock,     OD_T("LineType ByBlock"))
OD_MESSAGE_DEF( sidRecvLinetypeByLayer,     OD_T("LineType ByLayer"))
OD_MESSAGE_DEF( sidRecvLinetypeContinuous,  OD_T("LineType Continuous"))
OD_MESSAGE_DEF( sidRecvLayerZero,           OD_T("Layer Zero"))
OD_MESSAGE_DEF( sidRecvPaperSpaceLayout,    OD_T("Paper Space has no Layout"))
OD_MESSAGE_DEF( sidRecvModelSpaceLayout,    OD_T("Model Space has no Layout"))
OD_MESSAGE_DEF( sidRecvPaperSpaceInvalid,   OD_T("Paper Space"))
OD_MESSAGE_DEF( sidRecvModelSpaceInvalid,   OD_T("Model Space"))
OD_MESSAGE_DEF( sidRecvBadSectionAddr,      OD_T("Unable to recover address of section %ls"))
OD_MESSAGE_DEF( sidRecvGrDataLength,        OD_T("GrData of entity %ls has invalid length %d"))
OD_MESSAGE_DEF( sidCantRestoreProxy,        OD_T("Can't restore %ls from Proxy"))
OD_MESSAGE_DEF( sidEmptyProxyEntity,        OD_T("Empty proxy entity %ls"))
OD_MESSAGE_DEF( sidUnexpectedProxyFormat,   OD_T("Proxy object of AC21 version uses unexpected format of binary data"))
// Recover R12
OD_MESSAGE_DEF( sidRecvHeaderErr,           OD_T("File header has errors"))
OD_MESSAGE_DEF( sidRecv2ndHeaderErr,        OD_T("Second file header has errors"))
OD_MESSAGE_DEF( sidRecvTableRecsErr,        OD_T("Table %ls records section has error(s)"))
OD_MESSAGE_DEF( sidRecvTableRecordErr,      OD_T("%ls record (index = %d) has error: %ls"))
OD_MESSAGE_DEF( sidRecvEntityErr,           OD_T("Load entity %ls on address %X error"))
OD_MESSAGE_DEF( sidRecvDuplHandle,          OD_T("Duplicate or invalid handle %ls"))
OD_MESSAGE_DEF( sidRecvBadRecIndex,         OD_T("Invalid record index %d"))
// Warnings
OD_MESSAGE_DEF( sidNoDimBlockGenerated,     OD_T("Dimension block missing. %ls will be removed.")) // param: Dimension description
OD_MESSAGE_DEF( sidUnknownDXFversion,       OD_T("Unknown DXF file version."))  // Not used anymore
OD_MESSAGE_DEF( sidBinaryIncompatible,      OD_T("Module %ls has incompatible version"))
OD_MESSAGE_DEF( sidErrorUnloadingModule,    OD_T("Error unloading module \"%ls\"\nOdError: %ls"))
OD_MESSAGE_DEF( sidErrorNotThatKindOfClass, OD_T("Object of class %ls can't be cast to %ls."))
OD_MESSAGE_DEF( sidViewHasInvalidLensLength,OD_T("Paper Space View \"%ls\" has lens length value other than 50.0 (%g). Acad crashes on such dxf files. It will be saved as 50.0."))
OD_MESSAGE_DEF( sidNoCPConversionTable,     OD_T("Code page conversion is impossible because conversion table for \"%ls\" is missing."))
// Table
OD_MESSAGE_DEF( sidTableStyle,              OD_T("TableStyle (%ls)"))

OD_MESSAGE_DEF( sidAuditFailed,             OD_T("Audit Failed"))

OD_MESSAGE_DEF( sidRefToBlockIsCyclic,      OD_T("Reference to Block %ls is cyclic"))
OD_MESSAGE_DEF( sidCurrentDimstyle,         OD_T("Database current: %ls")) // param: Dimstyle name

OD_MESSAGE_DEF( sidRequiresIntegerBetween,  OD_T("Requires an integer between %d and %d."))
OD_MESSAGE_DEF( sidRequiresRealBetween,     OD_T("Requires a real between %g and %g."))
OD_MESSAGE_DEF( sidRequiresNOrNOnly,        OD_T("Requires %d or %d only."))
OD_MESSAGE_DEF( sidRequiresModelSpace,      OD_T("%ls only valid in Model Space.")) // param: Name of system variable
OD_MESSAGE_DEF( sidCommandCanceled,         OD_T("Command canceled."))
OD_MESSAGE_DEF( sidCancel,                  OD_T("*Cancel*"))

// Progress meter
OD_MESSAGE_DEF( sidPmSavingTo,              OD_T("Saving to %ls ..."))
OD_MESSAGE_DEF( sidPmAuditing,              OD_T("Auditing..."))
OD_MESSAGE_DEF( sidPmPreloadingR14,         OD_T("Pre-Loading R14 .dwg file..."))
OD_MESSAGE_DEF( sidPmLoadingHandles,        OD_T("Loading Handles..."))
OD_MESSAGE_DEF( sidPmLoadingDWGfile,        OD_T("Loading .dwg file..."))
OD_MESSAGE_DEF( sidPmLoadingFrom,           OD_T("Loading from %s ..."))
OD_MESSAGE_DEF( sidPmRecoveringDWGfile,     OD_T("Recovering .dwg file..."))
OD_MESSAGE_DEF( sidPmRecoveringFileHeader,  OD_T("Recovering file header..."))
OD_MESSAGE_DEF( sidPmRecoveringObjects,     OD_T("Recovering objects..."))
OD_MESSAGE_DEF( sidPmRecoveringObjectMap,   OD_T("Loading Object Map from Objects..."))
OD_MESSAGE_DEF( sidPmLoadingObjectMap,      OD_T("Loading Object Map..."))
OD_MESSAGE_DEF( sidPmLoadingDXFFile,        OD_T("Loading .dxf file..."))
OD_MESSAGE_DEF( sidPmResolvingHeaderRoundtrip, OD_T("Resolving header round trip..."))
OD_MESSAGE_DEF( sidPmUndoingConversion,     OD_T("Undoing conversion"))
OD_MESSAGE_DEF( sidPmDatabaseConversion,    OD_T("Database conversion : "))

OD_MESSAGE_DEF( sidCannotSetVarToThatValue, OD_T("Cannot set %s to that value."))
OD_MESSAGE_DEF( sidDuplicateOwnership,      OD_T("Duplicate ownership of reference %ls"))
OD_MESSAGE_DEF( sidBadDxfHeader,            OD_T("Error in drawing header on line/offset %d."))
OD_MESSAGE_DEF( sidBadDxf,                  OD_T("Syntax error or premature end of file on line/offset %d."))
OD_MESSAGE_DEF( sidLayoutInvalidModelType,  OD_T("Layout has invalid ModelType flag"))

// Font
OD_MESSAGE_DEF( sidFontSubstitution,              OD_T("Substituting font \"%ls\" by font \"%ls\""))
OD_MESSAGE_DEF( sidFontSubstitutedPath,           OD_T("  (font path: \"%ls\")"))
OD_MESSAGE_DEF( sidFontSubstitutionByDefault,     OD_T("Substituting font \"%ls\" by default font."))
OD_MESSAGE_DEF( sidGdtFontSubstitutionByDefault,  OD_T("Substituting font \"%ls\" by default gdt font."))

// Spa
OD_MESSAGE_DEF( sidSpaModelingOperationError,    OD_T("Modeling operation error:\n%ls"))

// 2dConstraintsGroup
OD_MESSAGE_DEF( sidNodesNotSorted,    OD_T("Nodes are not sorted (will be fixed)"))
OD_MESSAGE_DEF( sidSorted,    OD_T("Sorted"))
OD_MESSAGE_DEF( sidInvalidConstraintGeometry,    OD_T("Invalid Constraint Geometry"))

// AssocArray
OD_MESSAGE_DEF( sidAssocActionBodyIsInvalid,         OD_T("Action Body is Invalid"))
OD_MESSAGE_DEF( sidAssArrAbsentSrcBTR,      OD_T("Id Source BTR"))
OD_MESSAGE_DEF( sidAssArrItemProviderId,    OD_T("Entity provider of AssocArrayItem locator(\"%d\", \"%d\", \"%d\")"))

OD_MESSAGE_DEF( sidIdViewObjsDxf,    OD_T("Inventor exported custom objects can't be saved to .dxf. Their geometry will be lost."))

OD_MESSAGE_DEF( sidInvalidDefinitionId, OD_T("Invalid Definition ID"))


