#ifndef UMT_SETTINGS_MENU_H
#define UMT_SETTINGS_MENU_H

#include "UefiMonitorTest.h"

VOID
SettingsMenuInit (
  IN UMT_CONTEXT *Ctx
  );

VOID
SettingsMenuDoit (
  IN UMT_CONTEXT *Ctx
  );

VOID
SettingsMenuTip (
  IN UMT_CONTEXT *Ctx
  );

VOID
SettingsChangeParam (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        ParamStep
  );

VOID
SettingsMenuChangeValue (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        ValueStep
  );

#endif /* UMT_SETTINGS_MENU_H */
