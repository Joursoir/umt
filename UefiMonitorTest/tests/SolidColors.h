#ifndef UMT_SOLID_COLORS_TEST_H
#define UMT_SOLID_COLORS_TEST_H

#include "UefiMonitorTest.h"

VOID
SolidColorsTestInit (
  IN UMT_CONTEXT *Ctx
  );

VOID
SolidColorsTestDoit (
  IN UMT_CONTEXT *Ctx
  );

VOID
SolidColorsTestTip (
  IN UMT_CONTEXT *Ctx
  );

VOID
SolidColorsTestChangeParam (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        ParamStep
  );

VOID
SolidColorsTestChangeValue (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        ValueStep
  );

#endif /* UMT_SOLID_COLORS_TEST_H */
