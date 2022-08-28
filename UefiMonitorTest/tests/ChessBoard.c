#include <Library/DebugLib.h>
#include <Library/HiiLib.h>
#include <Library/MemoryAllocationLib.h>

#include "UefiMonitorTest.h"

STATIC UINT32 CurrentSideLength = 1;

STATIC
VOID
ChessBoardTestInit (
  IN UMT_CONTEXT *Ctx
  )
{
  UINT32 I;
  UINT32 J;
  enum UMT_COLORS ColorSquare;
  enum UMT_COLORS FirstColorSquare;
  enum UMT_COLORS TempColor;
  GRAPHICS_CONTEXT *Graphics = Ctx->Graphics;

  FirstColorSquare = UMT_COLOR_WHITE;
  ColorSquare      = UMT_COLOR_BLACK;

  for (J = 0; J < Graphics->Height; J += CurrentSideLength)
  {
    TempColor = FirstColorSquare;
    FirstColorSquare = ColorSquare;
    ColorSquare = TempColor;

    for (I = 0; I < Graphics->Width; I += CurrentSideLength)
    {
      PutRect (Graphics,
               I,
               J,
               I + CurrentSideLength,
               J + CurrentSideLength,
               &gUmtColors[ColorSquare].Color);

      ColorSquare = ((ColorSquare == UMT_COLOR_WHITE) ? UMT_COLOR_BLACK : UMT_COLOR_WHITE);
    }
  }

  if (Ctx->ShowTip) {
    gChessBoardTest.Tip(Ctx);
  }
}

STATIC
VOID
ChessBoardTestDoit (
  IN UMT_CONTEXT *Ctx
  )
{

}

STATIC
VOID
ChessBoardTestTip (
  IN UMT_CONTEXT *Ctx
  )
{
  GRAPHICS_CONTEXT *Graphics;

  Graphics = Ctx->Graphics;

  if (Ctx->ShowTip == FALSE) {
    // Restore
    gChessBoardTest.Init(Ctx);
    return;
  }

  DrawRectWithBorder (Graphics,
                      15,
                      Graphics->Height - 15 - 124,
                      430, Graphics->Height - 15,
                      3,
                      &gUmtColors[UMT_COLOR_WHITE].Color,
                      &gUmtColors[UMT_COLOR_NAVY].Color);

  DrawHiiStringF (Graphics,
                  25,
                  Graphics->Height - 15 - 114,
                  &gUmtColors[UMT_COLOR_NAVY].Color,
                  STRING_TOKEN (STR_CHESSBOARD_TITLE), gUmtHiiHandle);

  DrawHiiStringF (Graphics,
                  25,
                  Graphics->Height - 15 - 94,
                  &gUmtColors[UMT_COLOR_BLACK].Color,
                  STRING_TOKEN (STR_CHESSBOARD_MSG), gUmtHiiHandle,
                  CurrentSideLength);
}

STATIC
VOID
ChessBoardTestChangeParam (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        ParamStep
  )
{

}

STATIC
VOID
ChessBoardTestChangeValue (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        ValueStep
  )
{
  UINT32 ValueOut;
  UINT32 Height;
  UINT32 Width;

  Height   = Ctx->Graphics->Height;
  Width    = Ctx->Graphics->Width;
  ValueOut = CurrentSideLength + ValueStep;

  if (ValueOut == 0) {
    return;
  }

  while ((((Width / ValueOut) * ValueOut != Width) ||
         ((Height / ValueOut) * ValueOut != Height)) &&
         ((ValueOut < Height) && (ValueOut < Width)) &&
         (ValueOut > 1))
  {
    ValueOut += ValueStep;
  }

  if ((ValueOut >= Height) || (ValueOut >= Width))
  {
    return;
  }

  CurrentSideLength = ValueOut;
  gChessBoardTest.Init(Ctx);
}

CONST UMT_STATE_ACTIONS gChessBoardTest = {
  .Init         = ChessBoardTestInit,
  .Doit         = ChessBoardTestDoit,
  .Tip          = ChessBoardTestTip,
  .ChangeParam  = ChessBoardTestChangeParam,
  .ChangeValue  = ChessBoardTestChangeValue,
};
