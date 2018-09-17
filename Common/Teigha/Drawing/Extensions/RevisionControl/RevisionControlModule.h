#ifndef REVISION_CONTROLE_MODULE_
#define REVISION_CONTROLE_MODULE_

#include <OdaCommon.h>
#include "RxModule.h"

/** \details
  Corresponding C++ library: RevisionControl
  <group Extension_Classes>
*/
class RevisionControlModule : public OdRxModule {
  protected:

  void initApp();

  void uninitApp();
};


#endif // REVISION_CONTROLE_MODULE_
