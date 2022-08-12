#ifndef UMT_COLOR_DISTANCES_TEST_H
#define UMT_COLOR_DISTANCES_TEST_H

#include "UefiMonitorTest.h"

VOID
ColorDistancesTestInit (
  IN UMT_CONTEXT *Ctx
  );

VOID
ColorDistancesTestDoit (
  IN UMT_CONTEXT *Ctx
  );

VOID
ColorDistancesTestTip (
  IN UMT_CONTEXT *Ctx
  );

VOID
ColorDistancesTestChangeParam (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        ParamStep
  );

VOID
ColorDistancesTestChangeValue (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        ValueStep
  );

#endif
