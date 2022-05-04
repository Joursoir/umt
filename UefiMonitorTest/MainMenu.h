#ifndef UMT_MAIN_MENU_H
#define UMT_MAIN_MENU_H

#include "UefiMonitorTest.h"

VOID
MainMenuInit (
  IN UMT_CONTEXT *Ctx
  );

VOID
MainMenuDoit (
  IN UMT_CONTEXT *Ctx
  );

VOID
MainMenuTip (
  IN UMT_CONTEXT *Ctx
  );

VOID
MainMenuChangeValue (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        ValueStep
  );

#endif /* UMT_MAIN_MENU_H */
