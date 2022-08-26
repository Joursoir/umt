#include "ColorDistances.h"
#include <Uefi.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>

enum UMT_COLOR_DISTANCES_PARAM {
  UMT_COLOR_DISTANCES_PARAM_SQUARE_RED = 0,
  UMT_COLOR_DISTANCES_PARAM_SQUARE_GREEN,
  UMT_COLOR_DISTANCES_PARAM_SQUARE_BLUE,
  UMT_COLOR_DISTANCES_PARAM_BACKGROUND_RED,
  UMT_COLOR_DISTANCES_PARAM_BACKGROUND_GREEN,
  UMT_COLOR_DISTANCES_PARAM_BACKGROUND_BLUE,
  UMT_COLOR_DISTANCES_PARAM_END
};

STATIC enum UMT_COLOR_DISTANCES_PARAM CurrentParam = UMT_COLOR_DISTANCES_PARAM_SQUARE_RED;
STATIC UINT8 mColors[UMT_COLOR_DISTANCES_PARAM_END] = {
  238,
  181,
  71,
  0,
  0,
  0
};

VOID
ColorDistancesTestInit (
  IN UMT_CONTEXT *Ctx
  )
{
  GRAPHICS_CONTEXT *Graphics;
  UINT32 MinDimension;
  UINT32 SquareWidth;
  UINT32 SquareHeight;
  UINT32 SquareX;
  UINT32 SquareY;
  GRAPHICS_PIXEL_COLOR BackgroundColor;
  GRAPHICS_PIXEL_COLOR SquareColor;

  Graphics              = Ctx->Graphics;
  MinDimension          = MIN(Graphics->Width, Graphics->Height);
  SquareWidth           = MinDimension / 2;
  SquareHeight          = MinDimension / 2;
  SquareX               = Graphics->Width / 2 - SquareWidth / 2;
  SquareY               = Graphics->Height / 2 - SquareHeight / 2;
  BackgroundColor.Red   = mColors[UMT_COLOR_DISTANCES_PARAM_BACKGROUND_RED];
  BackgroundColor.Green = mColors[UMT_COLOR_DISTANCES_PARAM_BACKGROUND_GREEN];
  BackgroundColor.Blue  = mColors[UMT_COLOR_DISTANCES_PARAM_BACKGROUND_BLUE];
  SquareColor.Red       = mColors[UMT_COLOR_DISTANCES_PARAM_SQUARE_RED];
  SquareColor.Green     = mColors[UMT_COLOR_DISTANCES_PARAM_SQUARE_GREEN];
  SquareColor.Blue      = mColors[UMT_COLOR_DISTANCES_PARAM_SQUARE_BLUE];

  PutRect (Graphics, 0, 0, Graphics->Width, Graphics->Height, &BackgroundColor);

  PutRect (Graphics,
           SquareX,
           SquareY,
           SquareX + SquareWidth,
           SquareY + SquareHeight,
           &SquareColor);

  if (Ctx->ShowTip) {
    ColorDistancesTestTip(Ctx);
  }
}

VOID
ColorDistancesTestDoit (
  IN UMT_CONTEXT *Ctx
  ) {}

VOID
ColorDistancesTestTip (
  IN UMT_CONTEXT *Ctx
  )
{
  GRAPHICS_CONTEXT *Graphics;

  Graphics = Ctx->Graphics;

  if (Ctx->ShowTip == FALSE) {
    ColorDistancesTestInit (Ctx);
    return;
  }

  DrawRectWithBorder (Graphics,
                      15,
                      Graphics->Height - 15 - 304,
                      535, Graphics->Height - 15,
                      3,
                      &gUmtColors[UMT_COLOR_WHITE].Color,
                      &gUmtColors[UMT_COLOR_NAVY].Color);

  DrawHiiStringF (Graphics,
                  25,
                  Graphics->Height - 15 - 294,
                  &gUmtColors[UMT_COLOR_NAVY].Color,
                  STRING_TOKEN (STR_COLOR_DISTANCES_TITLE), gUmtHiiHandle);

  DrawHiiStringF (Graphics,
                  25,
                  Graphics->Height - 15 - 274,
                  &gUmtColors[UMT_COLOR_BLACK].Color,
                  STRING_TOKEN (STR_COLOR_DISTANCES_MSG), gUmtHiiHandle,
                  (CurrentParam == UMT_COLOR_DISTANCES_PARAM_SQUARE_RED) ? L'*' : L' ',
                  mColors[UMT_COLOR_DISTANCES_PARAM_SQUARE_RED],
                  (CurrentParam == UMT_COLOR_DISTANCES_PARAM_SQUARE_GREEN) ? L'*' : L' ',
                  mColors[UMT_COLOR_DISTANCES_PARAM_SQUARE_GREEN],
                  (CurrentParam == UMT_COLOR_DISTANCES_PARAM_SQUARE_BLUE) ? L'*' : L' ',
                  mColors[UMT_COLOR_DISTANCES_PARAM_SQUARE_BLUE],
                  (CurrentParam == UMT_COLOR_DISTANCES_PARAM_BACKGROUND_RED) ? L'*' : L' ',
                  mColors[UMT_COLOR_DISTANCES_PARAM_BACKGROUND_RED],
                  (CurrentParam == UMT_COLOR_DISTANCES_PARAM_BACKGROUND_GREEN) ? L'*' : L' ',
                  mColors[UMT_COLOR_DISTANCES_PARAM_BACKGROUND_GREEN],
                  (CurrentParam == UMT_COLOR_DISTANCES_PARAM_BACKGROUND_BLUE) ? L'*' : L' ',
                  mColors[UMT_COLOR_DISTANCES_PARAM_BACKGROUND_BLUE]);
}

VOID
ColorDistancesTestChangeParam (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        ParamStep
  )
{
  if (CurrentParam == 0 && ParamStep < 0) {
    CurrentParam = UMT_COLOR_DISTANCES_PARAM_BACKGROUND_BLUE;
  } else {
    CurrentParam += ParamStep;
    if (CurrentParam > UMT_COLOR_DISTANCES_PARAM_BACKGROUND_BLUE) {
      CurrentParam = UMT_COLOR_DISTANCES_PARAM_SQUARE_RED;
    }
  }

  ColorDistancesTestInit (Ctx);
}

VOID
ColorDistancesTestChangeValue (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        ValueStep
  )
{
  mColors[CurrentParam] = MAX(0, MIN(255, mColors[CurrentParam] + ValueStep));

  ColorDistancesTestInit (Ctx);
}
