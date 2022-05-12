#ifndef UMT_GRAYSCALE_TEST_H
#define UMT_GRAYSCALE_TEST_H

#include "UefiMonitorTest.h"

VOID
GrayscaleTestInit (
  IN UMT_CONTEXT *Ctx
  );

VOID
GrayscaleTestDoit (
  IN UMT_CONTEXT *Ctx
  );

VOID
GrayscaleTestTip (
  IN UMT_CONTEXT *Ctx
  );

VOID
GrayscaleTestChangeParam (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        ParamStep
  );

VOID
GrayscaleTestChangeValue (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        ValueStep
  );

#endif /* UMT_GRAYSCALE_TEST_H */
