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
  UINT32 Flag;
  UINT32 Flag_Color;
  GRAPHICS_CONTEXT *Graphics = Ctx->Graphics;

  Flag_Color = 0;

  for (J = 0; J < Graphics->Height; J = J + CurrentSideLength)
  {
    if (Flag_Color == 0)
    {
      Flag_Color = 1;
      Flag = 0;
    } else {
      Flag_Color = 0;
      Flag = 1;
    }

    for (I = 0; I < Graphics->Width; I = I + CurrentSideLength)
    {
      if (Flag == 0) {
        PutRect (Graphics,
                 I,
                 J,
                 I + CurrentSideLength,
                 J + CurrentSideLength,
                 &gUmtColors[UMT_COLOR_WHITE].Color);
        Flag = 1;
      } else {
        PutRect (Graphics,
                 I,
                 J,
                 I + CurrentSideLength,
                 J + CurrentSideLength,
                 &gUmtColors[UMT_COLOR_BLACK].Color);
        Flag = 0;
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
                      Graphics->Height - 15 - 134,
                      430, Graphics->Height - 15,
                      3,
                      &gUmtColors[UMT_COLOR_WHITE].Color,
                      &gUmtColors[UMT_COLOR_NAVY].Color);

  DrawHiiStringF (Graphics,
                  25,
                  Graphics->Height - 15 - 124,
                  &gUmtColors[UMT_COLOR_NAVY].Color,
                  STRING_TOKEN (STR_CHESSBOARD_TITLE), gUmtHiiHandle);

  DrawHiiStringF (Graphics,
                  25,
                  Graphics->Height - 15 - 104,
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

  if (ValueStep > 0) {
    ValueOut++;

    while ((((Width / ValueOut) * ValueOut != Width) ||
           ((Height / ValueOut) * ValueOut != Height)) &&
           ((ValueOut < Height) && (ValueOut < Width)))
    {
      ValueOut++;
    }
  } else {
    if (ValueOut == 1) {
      return;
    }

    ValueOut--;

    while ((((Width / ValueOut) * ValueOut != Width) ||
           ((Height / ValueOut) * ValueOut != Height)) &&
           (ValueOut > 1))
    {
      ValueOut--;
    }
  }
    if ((ValueOut < Height) && (ValueOut < Width) && (ValueOut > 0))
    {
      CurrentSideLength = ValueOut;
    }

  ChessBoardTestInit (Ctx);
}
