#ifndef UMT_GRADIENTS_TEST_H
#define UMT_GRADIENTS_TEST_H

#include "UefiMonitorTest.h"

VOID
GradientsTestInit (
  IN UMT_CONTEXT *Ctx
  );

VOID
GradientsTestDoit (
  IN UMT_CONTEXT *Ctx
  );

VOID
GradientsTestTip (
  IN UMT_CONTEXT *Ctx
  );

VOID
GradientsTestChangeParam (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        ParamStep
  );

VOID
GradientsTestChangeValue (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        ValueStep
  );

#endif /* UMT_GRADIENTS_TEST_H */
