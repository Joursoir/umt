
#ifndef UMT_GRADIENTS_TEST_H
#define UMT_GRADIENTS_TEST_H

#include "UefiMonitorTest.h"

enum UMT_STEPS {
  UMT_STEP_256 = 0,
  UMT_STEP_128,
  UMT_STEP_64,
  UMT_STEP_32,
  UMT_STEP_16,
  UMT_STEP_8,
  UMT_STEP_END
};

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
