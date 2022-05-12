#ifndef UMT_CHESSBOARD_TEST_H
#define UMT_CHESSBOARD_TEST_H

#include "UefiMonitorTest.h"

VOID
ChessBoardTestInit (
  IN UMT_CONTEXT *Ctx
  );

VOID
ChessBoardTestDoit (
  IN UMT_CONTEXT *Ctx
  );

VOID
ChessBoardTestTip (
  IN UMT_CONTEXT *Ctx
  );

VOID
ChessBoardTestChangeParam (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        ParamStep
  );

VOID
ChessBoardTestChangeValue (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        ValueStep
  );

#endif /* UMT_CHESSBOARD_TEST_H */
