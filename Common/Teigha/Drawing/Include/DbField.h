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


#ifndef OD_DBFIELD_H
#define OD_DBFIELD_H

#include "TD_PackPush.h"

#include "DbObject.h"
#include "DbFieldValue.h"
#include "OdValue.h"

class OdDbField;
typedef OdSmartPtr<OdDbField> OdDbFieldPtr;

/** \details
    This template class is a specialization of the OdArray class for OdDbField object pointers.
*/
typedef OdArray<OdDbFieldPtr> OdDbFieldArray;


/** \details
  Corresponding C++ library: TD_Db
  <group !!RECORDS_tdrawings_apiref>
*/
      typedef struct OdFd
{
  enum EvalFields
  {
    kEvalRecursive = 1
  };
  
} OdFd;

class OdDbField;

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbField object pointers.
*/
typedef OdSmartPtr<OdDbField> OdDbFieldPtr;

//*************************************************************************
// OdDbField
//*************************************************************************

/** \details
    This class represents Field objects in an OdDbDatabase instance.
    
    \remarks
    Field objects store both the Field expression and their evaluated values.
    
    Fields can be evaluated by an evaluator to any one of the following data types: Long,         
    Double, String, Date, Point, 3dPoint, ObjectId, Buffer, and Resbuf.       
    
    <group OdDb_Classes>
*/
class DBENT_EXPORT OdDbField : public OdDbObject
{
public:
  enum State
  {
    kInitialized        = 0x01,       // Field was initialized by the evaluator.
    kCompiled           = 0x02,       // Field was compiled.
    kModified           = 0x04,       // Field was modified, but not evaluated.
    kEvaluated          = 0x08,       // Field was evaluated.
    kHasCache           = 0x10,       // Field has evaluated cache.
    kHasFormattedString = 0x20        // Internal use only. 
  };
  
  enum EvalOption
  {
    kDisable            = 0,          // Do not evaluate field.
    kOnOpen             = 0x01,       // Evaluate field when a drawing is opened.
    kOnSave             = 0x02,       // Evaluate field when a drawing is saved.
    kOnPlot             = 0x04,       // Evaluate field when a drawing is printed.
    kOnEtransmit        = 0x08,       // Evaluate field when a drawing is etransmitted.
    kOnRegen            = 0x10,       // Evaluate field when a drawing is regenerated.
    kOnDemand           = 0x20,       // Evaluate field when demanded.
    kAutomatic          = (kOnOpen | kOnSave | kOnPlot |  kOnEtransmit | kOnRegen | kOnDemand) // Evaluate automatically.
  };
  
  enum EvalContext
  {
    kOpen               = 0x01,       // Field is being evaluated when a drawing is opened.
    kSave               = 0x02,       // Field is being evaluated when a drawing is saved.
    kPlot               = 0x04,       // Field is being evaluated when a drawing is printed.
    kEtransmit          = 0x08,       // Field is being evaluated when a drawing is etransmitted.
    kRegen              = 0x10,       // Field is being evaluated when a drawing is regenerated.
    kDemand             = 0x20,       // Field is being evaluated when demanded.
    kPreview            = 0x40        // Field is being evaluated when a drawing is previewed.
  };
  
  enum EvalStatus
  {
    kNotYetEvaluated    = 0x01,       // Evaluation not performed.
    kSuccess            = 0x02,       // Evaluation complete: Successful.
    kEvaluatorNotFound  = 0x04,       // Evaluator for the field not found.
    kSyntaxError        = 0x08,       // Evaluation complete: Syntax error in field code.
    kInvalidCode        = 0x10,       // Evaluation complete: Invalid field code.
    kInvalidContext     = 0x20,       // Evaluation complete: Invalid context to evaluate field.
    kOtherError         = 0x40        // Evaluation complete: Other evaluation error.
  };
  
  enum FieldCodeFlag
  {
    kRawCode            = 0x00,       // Returns the raw main field code. Ignored by setFieldCode().
    kFieldCode          = 0x01,       // Returns the raw field code with child codes substituted. Ignored by setFieldCode().
    kEvaluatedText      = 0x02,       // Returns the evaluated text. Ignored by setFieldCode().
    kEvaluatedChildren  = 0x04,       // Returns the field code with evaluated text for child fields. Ignored by setFieldCode().
    kObjectReference    = 0x08,       // Returns embedded text as references to child fields. Ignored by setFieldCode().
    kAddMarkers         = 0x10,       // Returns embedded text as field codes enclosed in field markers. Ignored by setFieldCode().
    kEscapeBackslash    = 0x20,       // Converts single backslashes to double backslashes. Ignored by setFieldCode().
    kStripOptions       = 0x40,       // Strips the standard options from the field code. Ignored by setFieldCode().
    kPreserveFields     = 0x80,       // Internal use only. Ignored by getFieldCode().
    kTextField          = 0x100,      // Treats the field as text with embedded fields. Ignored by getFieldCode().
    kPreserveOptions    = 0x200,      // Ignored by getFieldCode(). For internal use.
    kDetachChildren     = 0x400,      // Ignored by getFieldCode(). For internal use.
    kChildObjectReference = 0x800,    // Get child fields as object references. For internal use. 
    kForExpression      = 0x1000      // Get the value in a format used in arithmatic expressions (e.g. vector child values are put in square brackets). For internal use. Ignored by setFieldCode().
  };
  
  enum FilingOption
  {
    kSkipFilingResult   = 0x01        // Don't file field value.
  };
  
public:
  ODDB_DECLARE_MEMBERS(OdDbField);
  
  OdDbField();

  // OdDbField(const OdString& fieldCode, bool bTextField = false);
  // ~OdDbField(void);
  
  /** \details
    Sets this Field object as a property of the specified object, and
    adds it to the database.
    
    \param pOwner [in]  Pointer to the Owner object.
    \param propertyName [in]  Name of the property.
    
    \remarks
    The specified object must be database resident.
  */
  OdResult setInObject(
    OdDbObject* pOwner, 
    const OdString& propertyName);
  
  /** \details
    Adds this Field object and its Child objects to the specified database.
    
    \param pDb [in]  Pointer to the database in which to post.
  */
  virtual OdResult postInDatabase(
    OdDbDatabase* pDb);
  
  /** \details
    Returns the state of this Field object.
    
    \remarks
    state() returns a combination of zero or more of the following: 
    
    <table>
    Name                   Value    Description
    kInitialized           0x01     Field was initialized by the evaluator.
    kCompiled              0x02     Field was compiled.
    kModified              0x04     Field code was modified, but not evaluated.
    kEvaluated             0x08     Field was evaluated.
    kHasCache              0x10     Field has evaluated cache.
    kHasFormattedString    0x20     Internal use only. 
    </table>
  */
  OdDbField::State state() const;
  
  /** \details
    Returns the evaluation status of this Field object.
    
    \remarks
    evaluationStatus() returns a combination of one or more of the following: 
    
    <table>
    Name                   Value    Description
    kNotYetEvaluated       0x01     Evaluation not complete.
    kSuccess               0x02     Evaluation complete: Successful.
    kEvaluatorNotFound     0x04     Evaluator for the field not found.
    kSyntaxError           0x08     Evaluation complete: Field code syntax error.
    kInvalidCode           0x10     Evaluation complete: Invalid field code.
    kInvalidContext        0x20     Evaluation complete: Invalid context to evaluate field.
    kOtherError            0x40     Evaluation complete: Other evaluation error.
    </table>
  */
  OdDbField::EvalStatus evaluationStatus() const; 
                                          // OdUInt32* errCode = 0, 
                                         // OdString* errMsg = 0) const;

  /** \details
    Returns the evaluation option of this Field object.
    
    \remarks
    evaluationOption() returns a combination of one or more of the following: 
    
    <table>
    Name                   Value    Description
    kDisable               0x00     Do not evaluate field.
    kOnOpen                0x01     Evaluate field when a drawing is opened.
    kOnSave                0x02     Evaluate field when a drawing is saveed.
    kOnPlot                0x04     Evaluate field when a drawing is printed.
    kOnEtransmit           0x08     Evaluate field when a drawing is etransmitted.
    kOnRegen               0x10     Evaluate field when a drawing is regenerated.
    kOnDemand              0x20     Evaluate field when demanded.
    kAutomatic             0x3F     Evaluate field automatically.
    </table>
  */
  OdDbField::EvalOption evaluationOption() const;
  
  /** \details
    Sets the evaluation option for this Field object.
    
    \param evaluationOption [in]  Evaluation option.
    
    \remarks
    evaluationStatus must be  a combination of one or more of the following: 
    
    <table>
    Name                   Value    Description
    kDisable               0        Do not evaluate field.
    kOnOpen                0x01     Evaluate field when a drawing is opened.
    kOnSave                0x02     Evaluate field when a drawing is saved.
    kOnPlot                0x04     Evaluate field when a drawing is printed.
    kOnEtransmit           0x08     Evaluate field when a drawing is etransmitted.
    kOnRegen               0x10     Evaluate field when a drawing is regenerated.
    kOnDemand              0x20     Evaluate field when demanded.
    kAutomatic             0x3F     Evaluate field automatically.
    </table>
  */
  OdResult setEvaluationOption(
    OdDbField::EvalOption evaluationOption);
    
  /** \details
    Returns the filing option of this Field object.
    
    \remarks
    filingOption() returns zero or more of the following: 
    
    <table>
    Name                   Value    Description
    kSkipFilingResult      0x01     Do not file field value.
    </table>
  */
  OdDbField::FilingOption filingOption() const;
  
  /** \details
    Sets the filing option for this Field object.
    
    \remarks
    filingOption must be zero or more of the following: 
    
    <table>
    Name                   Value    Description
    kSkipFilingResult      0x01     Do not file field value.
    </table>
  */
  OdResult setFilingOption(
    OdDbField::FilingOption filingOption);
  
  /** \details
    Returns the ID of the evaluator for this Field object.
    
    \remarks
    Returns an empty string if no evaluator has been set for this Field object, or if
    the evaluator cannot be found.
  */
  OdString evaluatorId() const;

  /** \details
    Sets the ID of the evaluator for this Field object.

    \param evaluatorId [in]  Evaluator ID.
  */
  OdResult setEvaluatorId(
    const OdString& evaluatorId);
  
  /** \details
    Returns true if and only if this Field object is text with child fields.
  */
  bool isTextField() const;

  // OdResult convertToTextField (void);

  // OdString getFieldCode     (OdDbField::FieldCodeFlag nFlag, OdArray<OdDbField*>* pChildFields = 0, 
  //                           OdDb::OpenMode mode = OdDb::kForRead) const;
  
  /** \details
    Returns the field code of this Field object in the specified form..
    
    \param flags [in]  Field code flags.
    
    \remarks
    flags() returns a combination of one or more of the following:
    
    <table>
    Name                   Value    Description
    kFieldCode             0x01     Returns the raw field code. 
    kEvaluatedText         0x02     Returns the evaluated text.
    kEvaluatedChildren     0x04     Returns the field code with evaluated text for child fields.
    kObjectReference       0x08     Returns the field code as an object reference.
    kAddMarkers            0x10     Includes markers around field codes.
    kEscapeBackslash       0x20     Converts single backslashes to double backslashes.
    kStripOptions          0x40     Strips the standard options from the field code.
    kPreserveFields        0x80     Internal use only. Ignored.
    kTextField             0x100    Treats the field as text with embedded fields. Ignored.
    </table>
  */
  OdString getFieldCode(
    OdDbField::FieldCodeFlag flags);

  //                      OdArray<OdDbField*>* pChildFields = 0, 
  //                      OdDb::OpenMode mode = OdDb::kForRead) const;


  /** \details
    Sets the field code of this Field object in the specified form.
    
    \param fieldCode [in]  Field code.
    \param flags [in]  Field code flags.
    \param pChildFields [in]  Pointer to an array of child fields.
    
    \remarks
    Child fields are the embedded fields of text fields, and 
    nested fields of non-text fields.
    
    flags must be a combination of one or more of the following:
    
    <table>
    Name                   Value    Description
    kFieldCode             0x01     Returns the raw field code. Ignored.
    kEvaluatedText         0x02     Returns the evaluated text. Ignored. 
    kEvaluatedChildren     0x04     Returns the field code with evaluated text for child fields. Ignored.
    kObjectReference       0x08     Returns the field code as an object reference. Ignored.
    kAddMarkers            0x10     Includes markers around field codes. Ignored.
    kEscapeBackslash       0x20     Converts single backslashes to double backslashes. Ignored.
    kStripOptions          0x40     Strips the standard options from the field code. Ignored.
    kPreserveFields        0x80     Internal use only.
    kTextField             0x100    Treats the field as text with embedded fields.
    </table>
  */
  OdResult setFieldCode(
    const OdString& fieldCode, 
    OdDbField::FieldCodeFlag flags = (OdDbField::FieldCodeFlag) 0,
    OdDbFieldArray* pChildFields = 0);
  
  /** \details
    Returns the number of child fields in this Field object.
  */
  OdUInt32  childCount() const;
  
  /** \details
    Returns the specified child field in this Field object, and opens it in the specified mode.
    
    \param childIndex [in]  Child index.
    \param openMode [in]   Mode to apply to the child field.
  */
  OdDbFieldPtr getChild(
    OdUInt32 childIndex, 
    OdDb::OpenMode openMode);

  /** \details
    Returns the output format for this Field object.
  */
  OdString getFormat() const;

  /** \details
    Sets the output format for this Field object.
    
    \param format [in]  Output format.
    
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  OdResult setFormat(
    const OdString& format);
          

  /** \details
    Returns the evaluated Field object. 

    \remarks
    After creating a field object and setting the field code, 
    call this method to evaluate the field.
    If pDb is null, the field's database will be used if the field is a database resident. 
    If it is null and the field is not a database resident, evaluation will fail if the 
    field requires a database.

    \param nContext [in]  Context in which the field is evaluated; the context flag can be one of the 
    \param predefined OdDbField::EvalContext enum flags or a user-defined context flag.
    \param pDb [in] Database to be used for the evaluation; can be null.
    \param pNumFound [out] Pointer to receive the number of fields evaluated; can be null if not required.  
    \param pNumEvaluated [out] Pointer to receive the number of fields evaluated; can be null if this information is not required. 
  */
  OdResult evaluate(
    int nContext, 
    OdDbDatabase* pDb = 0, 
    int* pNumFound = 0, 
    int* pNumEvaluated = 0);
  
  
  /** \details
    Returns the data type of this Field object in the specified form.
    
    \remarks
    Returns kUnknown if the field has yet to be evaluated.

    \returns
    Returns one of the following:
    
    <table>
    Name                            Value
    OdFieldValue::kUnknown          0
    OdFieldValue::kLong             0x01
    OdFieldValue::kDouble           0x02
    OdFieldValue::kString           0x04
    OdFieldValue::kDate             0x08
    OdFieldValue::kPoint            0x10
    OdFieldValue::k3dPoint          0x20
    OdFieldValue::kObjectId         0x40
    OdFieldValue::kBuffer           0x80
    OdFieldValue::kResbuf           0x100
    </table>
  */
  OdValue::DataType dataType() const;

  /** \details
    Returns the field evaluation string using the output format for this Field object.
    
    \remarks
    Returns an empty string if not successful.
  */
  OdString getValue() const;
  
  /** \details
    Returns the field evaluation string using the output format for this Field object.
    
    \param pValue [in]  Receives a SmartPointer to the evaluation string.
    
    \remarks
    Returns eOk if successful, or returns an appropriate error code if unsuccessful.

    \remarks
    The string pointed to by pValue must be freed by the caller.
  */
  OdResult getValue(
    OdFieldValue& pValue) const;
            
  bool  hasHyperlink() const;

  OdResult getHyperlink(OdString& psName,
                        OdString& psDescription, 
                        OdString& psSubLocation,
                        OdString& psDisplayString) const;

  OdResult setHyperlink(const OdString& psName, 
                        const OdString& psDescription, 
                        const OdString& psSubLocation = OdString::kEmpty);

  OdResult removeHyperlink();
  
  /** \details
    Returns the data for the specified key of this Field object.
    
    \param key [in]  Key.
  */
  OdFieldValue getData(
    const OdString& key) const;
  
  /** \details
    Sets the data for the specified key of this Field object.
    
    \param key [in]  Key.
    \param pData [in]  Pointer to the object containing the data. 
  */
  OdResult setData(
    const OdString& key, 
    const OdFieldValue& pData);

public:
  // Base class overrides

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;
  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;
  
  virtual void subClose();
};

#define TEXT_FIELD_NAME OD_T("TEXT")

DBENT_EXPORT OdDbFieldPtr copyTextFieldToObject(OdDbFieldPtr srcField, OdDbObject* pObj);
DBENT_EXPORT OdDbFieldPtr copyFieldWithChild(OdDbFieldPtr srcField);

#include "TD_PackPop.h"
#endif // OD_DBFIELD_H
