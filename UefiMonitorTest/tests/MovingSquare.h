#ifndef UMT_MOVING_SQUARE_TEST_H
#define UMT_MOVING_SQUARE_TEST_H

#include "UefiMonitorTest.h"

VOID
MovingSquareTestInit (
  IN UMT_CONTEXT *Ctx
  );

VOID
MovingSquareTestDoit (
  IN UMT_CONTEXT *Ctx
  );

VOID
MovingSquareTestTip (
  IN UMT_CONTEXT *Ctx
  );

VOID
MovingSquareTestChangeParam (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        ParamStep
  );

VOID
MovingSquareTestChangeValue (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        ValueStep
  );

#endif
