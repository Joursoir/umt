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
MainMenuKeyRight (
  IN UMT_CONTEXT *Ctx
  );

VOID
MainMenuKeyLeft (
  IN UMT_CONTEXT *Ctx
  );

#endif /* UMT_MAIN_MENU_H */
