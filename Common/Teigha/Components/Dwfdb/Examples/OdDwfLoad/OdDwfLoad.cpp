
#include "OdaCommon.h"
#include "DwfHostAppServices.h"
#include "StaticRxObject.h"
#include "DwfDatabase.h"
#include "DwfObjectImpl.h"
#include "RxDynamicModule.h"
#include "DynamicLinker.h"
#include "ExSystemServices.h"
#include "Gs/GsBaseInclude.h"
#include "diagnostics.h"

#include "dwfcore/String.h"
#include "dwfcore/paging/MemoryManager.h"

#include "ExPrintConsole.h"

#define STL_USING_IOSTREAM
#include "OdaSTL.h"
//#define  STD(a)  std:: a

/************************************************************************/
/* Define a module map for statically linked modules                    */
/************************************************************************/
#if !defined(_TOOLKIT_IN_DLL_)
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDbRootModuleObject);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(DwfDbModule);
ODRX_BEGIN_STATIC_MODULE_MAP()
ODRX_DEFINE_STATIC_APPMODULE(OdDbRootModuleName, OdDbRootModuleObject)
ODRX_DEFINE_STATIC_APPMODULE(OdDwfDbModuleName, DwfDbModule)
ODRX_END_STATIC_MODULE_MAP() 
#endif

class OdExDwfSystemServices : public ExSystemServices
{
public:
	OdExDwfSystemServices() {}
};

class MyDwfServices : public OdExDwfSystemServices, public OdDwfHostAppServices
{
protected:
	ODRX_USING_HEAP_OPERATORS(OdExDwfSystemServices);
};

#if 0
static class MemLeakCatcher
{
public:
  MemLeakCatcher()
  {
    //_CrtSetBreakAlloc(354);
    // marker to see mem leaks
    char* pLeakMarker = (char *) odrxAlloc(100);
    memset(pLeakMarker, 0, 100);
    strcpy(pLeakMarker, "Marker_Start");

    long allocReqNum;
    _CrtIsMemoryBlock(pLeakMarker, 100, &allocReqNum, NULL, NULL);
    
    //return;
    //long num = allocReqNum + 1;
    ////test for check memleak catcher
    //_CrtSetBreakAlloc(num); // num in {} in dump of _CrtDumpMemoryLeaks() (num < 453(by MarkerStart) are useless to check)

    //pLeakMarker = (char *)odrxAlloc(200);
    //memset(pLeakMarker, 0, 200);
    //strcpy(pLeakMarker, "Marker_1");
  }
  ~MemLeakCatcher()
  {
    // unrecommnded (uses to remove Dwf Toolkit internal memory leaks at end of application only)
    DWFCore::DWFStringTable::Purge();
    //
    _CrtDumpMemoryLeaks();
  }
} s_catcher;
#endif

static int LoadTeighaDWFx(const OdString& szSource)
{
  int nRes = 0;
  odPrintConsoleString(L"\nOdDwfLoad sample program\n");

  /********************************************************************/
  /* Create a custom Services instance.                               */
  /********************************************************************/
  OdStaticRxObject<MyDwfServices> svcs;

#if !defined(_TOOLKIT_IN_DLL_) || defined(__MWERKS__)
  ODRX_INIT_STATIC_MODULE_MAP();
#endif
  /**********************************************************************/
  /* Initialize Teigha for .dwf files                                  */
  /**********************************************************************/
  odrxInitialize(&svcs);
  odgsInitialize();
  //::odrxDynamicLinker()->loadModule(OdDbRootModuleName, false); // workaround
  ::odrxDynamicLinker()->loadModule(OdDwfDbModuleName, false);

  /**********************************************************************/

  try {
    OdDwfDatabasePtr pDb = svcs.readFile(szSource);

    if (!pDb.isNull()) {

      OdDwfObjectId objId = pDb->currentLayoutId();
      nRes++;

      odPrintConsoleString(L"Got layout id: %ls\n", objId.getHandle().ascii().c_str());

      OdDwfViewPtr pView = OdDwfView::cast(objId.openObject());
      OdDwfModelViewPtr pModelView = OdDwfModelView::cast(pView);
      if (pModelView.get())
      {
        odPrintConsoleString(L"Dump model view properties:\n");
        OdStringArray keys = pModelView->propertyKeys();
        for (int idx = 0, sz = (int) keys.size(); idx < sz; idx++)
          odPrintConsoleString(L"  prop: name = \"%ls\", value = \"%ls\"\n", 
                               pModelView->propertyName(keys[idx]).c_str(),
                               pModelView->property(keys[idx]).c_str());
      }
      if (pView.get())
        pView->checkLoad();
    }
  }
  catch (const OdError& e) {
    odPrintConsoleString(L"\nException Caught: %ls\n", e.description().c_str());
  }
  catch (...)
  {
    odPrintConsoleString(L"\nUnknown Exception Caught\n");
  }

  // unrecommnded (uses to remove Dwf Toolkit internal memory leaks at end of application only)
  DWFCore::DWFStringTable::Purge();
  //_CrtDumpMemoryLeaks();

  /**********************************************************************/
  /* Uninitialize Initialize Teigha for .dwf files                      */
  /**********************************************************************/

  ::odrxDynamicLinker()->unloadUnreferenced();
  odgsUninitialize();
  ::odrxUninitialize();

  return nRes;
}

////////////////////////////////////////////////////////////////////////////////////

static void MyAssert(const char* expr, const char* fileName, int nLine)
{
  OdString sMsg, sExpr(expr), sFileName(fileName);
  sMsg.format(L"assertion expression: \"%ls\"\nfile: \"%ls\"\nline: %d", sExpr.c_str(), sFileName.c_str(), nLine);
  odPrintConsoleString(sMsg.c_str());
}

static bool MyCheckAssertGroup(const char* pGroup)
{
  return !pGroup || !*pGroup; // asserts of base group are enabled only (default)
                              //return true; // asserts of all groups are enabled
}

#if defined(OD_USE_WMAIN)
int wmain(int argc, wchar_t* argv[])
#else
int main(int argc, char* argv[])
#endif
{
  /**********************************************************************/
  /* Set customized assert function                                     */
  /**********************************************************************/
  odSetAssertFunc(MyAssert);
  odSetCheckAssertGroupFunc(MyCheckAssertGroup);

  /**********************************************************************/
  /* Parse Command Line strings                                         */
  /**********************************************************************/

  int nRes = 0;
  if (argc == 2)
  {
    OdString sFile(argv[1]);
    nRes = LoadTeighaDWFx(sFile);
  }
  else
    odPrintConsoleString(L"Use as: OdDwfLoad  file_path.dwf");

  odPrintConsoleString(L"\n\nPress ENTER to continue...\n");
  std::cin.get(); // system("pause");

  return nRes;
}
