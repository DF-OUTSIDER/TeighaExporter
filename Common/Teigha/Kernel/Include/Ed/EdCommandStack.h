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

#ifndef _ODEDCOMMANDSTACK_H_INCLUDED_
#define _ODEDCOMMANDSTACK_H_INCLUDED_



#define ODRX_COMMAND_DOCK      OD_T("ODRX_REGISTERED_COMMANDS")

typedef void (*OdRxFunctionPtr) ();

class OdEdCommand;
class OdEdCommandContext;

#include "RxObject.h"

class OdRxIterator;

/** \details
  This template class is a specialization of the OdSmartPtr class for OdRxIterator object pointers. 
*/
typedef OdSmartPtr<OdRxIterator> OdRxIteratorPtr;

class OdRxDictionary;
// class OdEdUIContext;

#include "TD_PackPush.h"

class OdEdCommand;

typedef void (OdEdCommandFunction) (OdEdCommandContext* pCmdCtx);

/** \details
  This template class is a specialization of the OdSmartPtr class for OdEdCommand object pointers. 
*/
typedef OdSmartPtr<OdEdCommand> OdEdCommandPtr;

/** \details
    This class represents custom commands within a Teigha client application. 
    
    \remarks
    A custom command is be created by deriving a class from OdEdCommand, and overriding
    the functions in that class. 
    
    The custom command is registered in the global command stack by calling 
    OdEdCommandStack::addCommand() on the global command stack, 
    which obtained via the odedRegCmds() external function.

    Corresponding C++ library: TD_Root
    
    <group OdEd_Classes>
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdEdCommand : public OdRxObject 
{
protected:
  OdEdCommand();
public:
  ODRX_DECLARE_MEMBERS(OdEdCommand);

  ~OdEdCommand();
  enum
  {
    kModal            = 0x00000000,
    kTransparent      = 0x00000001,
    kUsePickset       = 0x00000002,
    kRedraw           = 0x00000004,
    kNoPerspective    = 0x00000008,
    kNoMultiple       = 0x00000010,
    kNoTilemode       = 0x00000020,
    kNoPaperspace     = 0x00000040,
    kPlotOnly         = 0x00000080,
    kNoOEM            = 0x00000100,
    kUndefined        = 0x00000200,
    kInProgress       = 0x00000400,
    kDefun            = 0x00000800,
    kNoNewStack       = 0x00010000,
    kNoInternalLock   = 0x00020000,
    kDocReadLock      = 0x00080000,
    kDocExclusiveLock = 0x00100000,
    kSession          = 0x00200000,
    kInterruptible    = 0x00400000,
    kNoHistory        = 0x00800000,

    kNoUndoMarker     = 0x01000000,
    kNoBedit          = 0x02000000 
  };

  static OdEdCommandPtr createObject(
    const OdString& sGroupName,
    const OdString& sGlobalName,
    const OdString& sLocalName,
    OdUInt32 commandFlags,
    OdEdCommandFunction *pFunction,
    OdRxModule *pModule = 0
    );

  /** \details
    Returns the group name of this command object. 
    
    \remarks
    This name is used to group sets of registered commands.
  */
  virtual const OdString groupName() const = 0;

  /** \details
    Returns the global (untranslated) name of this command object.
    
    \remarks
    This name must be unique within the groupName of this command object.
  */
  virtual const OdString globalName() const = 0;

  /** \details
    Returns the local (translated) name of this command.
    
    \remarks
    This name must be unique within the groupName of this command object.
  */
    virtual const OdString localName() const;

  /** \details
    Called to execute this command object.
    
    \remarks
    The client's override of this function contains the body of the command.
    
    \param pCommandContext [in]  Pointer to the command context.
    
    \remarks
    The specified command context object is used to interact with the 
    calling application as part of an interactive command.  
            
    \sa
    OdEdCommandContext::userIO
  */
  virtual void execute(OdEdCommandContext* pCommandContext) = 0;

  virtual const OdRxModule* module() const;

  virtual void undefine(bool undefIt);

  virtual OdInt32 flags() const;

  /*
  virtual OdEdUIContext   *UIContext() const = 0;
  virtual int             functionCode() const = 0;
  virtual void            functionCode(int fcode) = 0;
  virtual const HINSTANCE resourceHandle() const = 0;
  */
};


/** \details
    This class is the base class for custom classes that receive notification
    of OdEdCommandStack events.
    
    \remarks
    Events consist of the addition and deletion, starting and ending,
    cancellation and failure, and non-recognition of registered commands. 
    
    The command stack consists of groups which contain their respective commands.
    Adding a command adds it to its group. If the group does not exist,
    the group is first created and pushed onto the top of the stack.
    
    \remarks
    The default implementations of all methods in this class do nothing but return.

    Corresponding C++ library: TD_Root

    <group OdEd_Classes>
*/
class FIRSTDLL_EXPORT OdEdCommandStackReactor : public OdRxObject 
{ 
public:
  ODRX_DECLARE_MEMBERS(OdEdCommandStackReactor);
  
  /** \details
    Notification function called whenevever a command has been added to the global command stack.
    
    \param pCommand [in]  Pointer to the command object.
    
	  \remarks
	  This function is called after the operation.
  */
  virtual void commandAdded(OdEdCommand* pCommand);
  
  /** \details
    Notification function called whenevever a command will be removed from the global command stack
    
    \param pCommand [in]  Pointer to the command object.
    
	  \remarks
	  This function is called after the operation.
  */
  virtual void commandWillBeRemoved(OdEdCommand* pCommand);
  
  /** \details
    Notification function called whenevever the execution of a command will start.
    
    \param pCommand [in]  Pointer to the command object.
    
	  \remarks
	  This function is called before the operation.
  */
  virtual void commandWillStart(OdEdCommand* pCommand, OdEdCommandContext* pCmdCtx);
  
  /** \details
    Notification function called whenevever the execution of a command has ended.
    
    \param pCommand [in]  Pointer to the command object.
    
	  \remarks
	  This function is called after the operation.
  */
  virtual void commandEnded(OdEdCommand* pCommand, OdEdCommandContext* pCmdCtx);
  
  /** \details
    Notification function called whenevever the execution of a command has been canceled.
    
    \param pCommand [in]  Pointer to the command object.
    
	  \remarks
	  This function is called after the operation.
  */
  virtual void commandCancelled(OdEdCommand* pCommand, OdEdCommandContext* pCmdCtx);
  
  /** \details
    Notification function called whenevever the execution of a command has failed.
    
    \param pCommand [in]  Pointer to the command object.
    
	  \remarks
	  This function is called after the operation.
  */
  virtual void commandFailed(OdEdCommand* pCommand, OdEdCommandContext* pCmdCtx);
  
  /** \details
    Notification function called whenevever the execution of an unknown command has been attempted.
    
    \param commandName [in]  name of unknown command.
    
	  \remarks
	  This function is called after the operation.
  */
  virtual OdEdCommandPtr unknownCommand(const OdString& commandName, OdEdCommandContext* pCmdCtx);
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdEdCommandStackReactor object pointers. 
*/
typedef OdSmartPtr<OdEdCommandStackReactor> OdEdCommandStackReactorPtr;

/** \details
    This template class is a specialization of the OdArray class for OdEdCommandStackReactor object SmartPointers.
*/
typedef OdArray<OdEdCommandStackReactorPtr> OdEdCommandStackReactorArray;

/** \details
    This exception class to be thrown from OdEdCommandStack::executeCommand(const OdString& cmdName, ...)
    if cmdName was not found.
    <group OdEd_Classes>
*/
class OdEdUnknownCommand {};

/** \details
    This class implements and manages the global command stack.

    \remarks
    The command stack consists of groups which contain their respective commands.
    Adding a command adds it to its group. If the group does not exist,
    the group is first created and pushed onto the top of the stack.
    
    The lookupCommand and executeCommand functions search for command names
    in the group order on the stack.
     
    In the case of duplicate command names, the most recent one added to the stack
    is the command that is returned or executed.
     
    The group order may be changed with popGroupToTop function. 
         
    Corresponding C++ library: TD_Root
    
    <group OdEd_Classes>
*/
class FIRSTDLL_EXPORT OdEdCommandStack : public OdRxObject 
{ 
public:
  ODRX_DECLARE_MEMBERS(OdEdCommandStack);

   /** \details
    Adds the specified reactor to this object's reactor list.

    \param pReactor [in]  Pointer to the reactor object.
   */
  virtual void addReactor(OdEdCommandStackReactor* pReactor) = 0;

  /** \details
    Removes the specified transient reactor from this object's reactor list.

    \param pReactor [in]  Pointer to the reactor object.
  */
  virtual void removeReactor(OdEdCommandStackReactor* pReactor) = 0;

  /** \details
    Returns this object's transient reactors.
  */
  virtual OdEdCommandStackReactorArray getCommandStackReactors() const = 0;

  /** \details
    Adds the specified command object to this Stack object.

    \param pCommand [in]  Pointer to the command object.
    
    \remarks
    If the group associated with the command does not exist, 
    the group is first created and pushed onto the top of the stack.
    
    The globalName and localName must be unique within the groupName of this command object.
  */
  virtual void addCommand(OdEdCommand* pCommand) = 0;

  /** \details
    Adds the specified command object to this Stack object.

    \param pCommand [in]  Pointer to the command object.
    
    \remarks
    If the group associated with the command does not exist, 
    the group is first created and pushed onto the top of the stack.
    
    The globalName and localName must be unique within the groupName of this command object.
  */
  virtual OdEdCommand* addCommand(
    const OdString& sGroupName,
    const OdString& sGlobalName,
    const OdString& sLocalName,
    OdUInt32 commandFlags,
    OdEdCommandFunction *pFunction,
    OdRxModule *pModule = 0);
  
  /** \details
    Returns an Iterator object that can be 
    used to traverse the OdEdCommand objects in this Stack object.
    
    \remarks
    The traversal is by global name in group order.
  */
  virtual OdRxIteratorPtr newIterator() = 0;

  /** \details
    Returns an Iterator object that can be 
    used to traverse the OdEdCommand objects in this Stack object.

    \param groupName [in]  Group name.

    \remarks
    If groupName is specified, only OdEdCommand objects in the specified
    group will be traversed.
  */
  virtual OdRxIteratorPtr newIterator(const OdString& groupName) = 0;


  /** \details
    Returns an Iterator object that can be used to traverse the 
    group objects in this Stack object.

    \remarks
    Each group is an OdRxDictionary object.
  */
  virtual OdRxIteratorPtr newGroupIterator() = 0;
  
  enum LookupFlags
  {
    kGlobal       = 0x01, // Search global (untranslated) names.
    kLocal        = 0x02, // Search local (translated) names.
    kSpecifedGroup= 0x04, // Search in specified group only.
    kUndefed      = 0x08, // Do not fire unknownCommand() event.
    kThrowUnknown = 0x10  // Throw OdEdUnknownCommand exception if command was not found.
  };
  /** \details
    Returns the OdEdCommand object with the specified name in this Stack object. 

    \param commandName [in]  Name of the command to find.
    \param lookupFlags [in]  Specified where command lookup is to look.
    \param groupName [in]  Group name.
    
    \returns
    Returns a SmartPointer to the OdEdCommand object if successful, otherwise a null SmartPointer.
\remarks
    This function searches for command names in the group order on the stack.
    
    If groupName is specified, only that group will be searched.

    lookupFlags must be a combination of one or more of the following:
    
    <table>
    Name              Value     Description
    kGlobal           0x01      Search global (untranslated) names.
    kLocal            0x02      Search local (translated) names.
    kSpecifedGroup    0x04      Search in specified group only.
    kUndefed          0x08      Indicates whether or not to return commands that are currently marked as undefined.
    </table>
    
  */
  virtual OdEdCommandPtr lookupCmd(
    const OdString& commandName,
    int lookupFlags = kGlobal|kLocal,
    const OdString& groupName = OdString::kEmpty) = 0;

  /** \details
    Executes the specified command in this Stack object. 
    
    \remarks
    OdEdCommandStackReactor objects in the reactor list of this Stack object will be notified.

    \param pCommand [in]  Pointer to the command object to execute.
    \param pCmdCtx [in]  Pointer to the command context.

    \remarks
    The specified command context object is used to interact with the 
    calling application as part of an interactive command.  

    This function searches for both global and local command names in the group order on the stack.

    \sa
    OdEdCommand::execute.
  */
  virtual void executeCommand(OdEdCommand* pCmd, OdEdCommandContext* pCmdCtx) = 0;

  /** \details
    Executes the specified command in this Stack object. 

    \param cmdName [in]  Name of the command to find.
    \param pCmdCtx [in]  Pointer to the command *context*.
    \param lookupFlags [in]  Specified where command lookup is to look.
    \param groupName [in]  Group *name*.
    
    \remarks
    Lookups a command object registred in this Stack object by a cpecified cmdName,
    lookupFlags and groupName and executes it. If command was not found then method fires
    OdEdCommandStackReactor::unknownCommand() event that may provide back a command object
    for the given name. So if a command object was provided by one of the reactors then
    implementation of this method executs it. If one was provided by neither of the reactors and
    kThrowUnknown flag was specified in lookupFlags then OdEdUnknownCommand would be thrown.
    If kThrowUnknown flag was not specified the method just exits.
    
    If groupName is specified, only that group will be searched.

    lookupFlags must be a combination of one or more of the following:
    
    <table>
    Name              Value     Description
    kGlobal           0x01      Search global (untranslated) names.
    kLocal            0x02      Search local (translated) names.
    kSpecifedGroup    0x04      Search in specified group only.
    kUndefed          0x08      Indicates whether or not to return commands that are currently marked as undefined.
    kThrowUnknown     0x10      Throw OdEdUnknownCommand exception if command was not found.
    </table>   

    \sa
    OdEdCommandStack::lookupCmd,
    OdEdCommandStackReactor::unknownCommand,
    OdEdCommand::execute.
  */
  virtual void executeCommand(const OdString& cmdName,
                              OdEdCommandContext* pCmdCtx,
                              int lookupFlags = kGlobal|kLocal,
                              const OdString& groupName = OdString::kEmpty) = 0;

  /** \details
    Removes the command with the specified group and global names from this Stack object.

    \param groupName [in]  Group name.
    \param globalName [in]  Global (untranslated) name.
    
    \remarks
    If a group is emptied, it is removed from this Stack object.
    
    If the specified command is not found, this function does nothing but return.
  */
  virtual void removeCmd(const OdString& groupName, const OdString& globalName) = 0;

  
  virtual void removeCmd(const OdEdCommand* pCommand) = 0;

  /** \details
    Removes the specified group and its commands from this Stack object.

    \param groupName [in]  Group name.

    \remarks
    If the specified group is not found, this function does nothing but return.
  */
  virtual void removeGroup(const OdString& groupName) = 0;
  /** \details
    This function pops the cmdGroupName command group to the top of the 
    command stack making it the first group to be searched when resolving 
    application defined command names.

    \param cmdGroupName [in]  Name of the command group to pop to the top of the command stack.

    \remarks
    If the specified group is not found, this function returns eKeyNotFound.
  */
  virtual OdResult popGroupToTop(const OdString& cmdGroupName) = 0;
};
/** \details
  This template class is a specialization of the OdSmartPtr class for OdEdCommandStack object pointers. 
*/
typedef OdSmartPtr<OdEdCommandStack> OdEdCommandStackPtr;

/** \details
    Returns the global command stack.
*/
FIRSTDLL_EXPORT OdEdCommandStackPtr odedRegCmds();


#include "TD_PackPop.h"

#endif //#ifndef _ODEDCOMMANDSTACK_H_INCLUDED_
