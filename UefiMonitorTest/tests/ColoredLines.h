#ifndef UMT_COLORED_LINES_TEST_H
#define UMT_COLORED_LINES_TEST_H

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
ColoredLinesTestInit (
  IN UMT_CONTEXT *Ctx
  );

VOID
ColoredLinesTestDoit (
  IN UMT_CONTEXT *Ctx
  );

VOID
ColoredLinesTestTip (
  IN UMT_CONTEXT *Ctx
  );

VOID
ColoredLinesTestChangeParam (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        ParamStep
  );

VOID
ColoredLinesTestChangeValue (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        ValueStep
  );

#endif /* UMT_COLORED_LINES_TEST_H */
