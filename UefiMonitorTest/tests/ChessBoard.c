#include <Library/DebugLib.h>
#include <Library/HiiLib.h>
#include <Library/MemoryAllocationLib.h>

#include "tests/ChessBoard.h"

STATIC UINT32 CurrentSideLength = 1;

VOID
ChessBoardTestInit (
  IN UMT_CONTEXT *Ctx
  )
{
  UINT32 I;
  UINT32 J;
  UINT32 ColorSquare;
  UINT32 FirstColorSquare;
  GRAPHICS_CONTEXT *Graphics = Ctx->Graphics;

  FirstColorSquare = UMT_COLOR_WHITE;

  for (J = 0; J < Graphics->Height; J = J + CurrentSideLength)
  {
    if (FirstColorSquare == UMT_COLOR_WHITE)
    {
      FirstColorSquare = UMT_COLOR_BLACK;
      ColorSquare = UMT_COLOR_WHITE;
    } else {
      FirstColorSquare = UMT_COLOR_WHITE;
      ColorSquare = UMT_COLOR_BLACK;
    }

    for (I = 0; I < Graphics->Width; I = I + CurrentSideLength)
    {
      PutRect (Graphics,
               I,
               J,
               I + CurrentSideLength,
               J + CurrentSideLength,
               &gUmtColors[ColorSquare].Color);

      if (ColorSquare == UMT_COLOR_WHITE) {
        ColorSquare = UMT_COLOR_BLACK;
      } else {
        ColorSquare = UMT_COLOR_WHITE;
      }
    }
  }

  if (Ctx->ShowTip) {
    ChessBoardTestTip (Ctx);
  }
}

VOID
ChessBoardTestDoit (
  IN UMT_CONTEXT *Ctx
  )
{

}

VOID
ChessBoardTestTip (
  IN UMT_CONTEXT *Ctx
  )
{
 GRAPHICS_CONTEXT *Graphics;

  Graphics = Ctx->Graphics;

  if (Ctx->ShowTip == FALSE) {
    // Restore
    ChessBoardTestInit (Ctx);
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

VOID
ChessBoardTestChangeParam (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        ParamStep
  )
{

}

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
  ValueOut = CurrentSideLength;

  ValueOut += ValueStep;

  while   ((((Width / ValueOut) * ValueOut != Width) ||
         ((Height / ValueOut) * ValueOut != Height)) &&
         ((ValueOut < Height) && (ValueOut < Width)) &&
         (ValueOut > 1))
  {
    ValueOut += ValueStep;
  }

    if ((ValueOut < Height) && (ValueOut < Width) && (ValueOut > 0))
    {
      CurrentSideLength = ValueOut;
    }

  ChessBoardTestInit (Ctx);
}
