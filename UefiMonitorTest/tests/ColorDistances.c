#include "ColorDistances.h"
#include <Uefi.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>

enum UMT_COLOR_DISTANCES_PARAM {
  UMT_COLOR_DISTANCES_PARAM_SQUARE_RED = 0,
  UMT_COLOR_DISTANCES_PARAM_SQUARE_GREEN,
  UMT_COLOR_DISTANCES_PARAM_SQUARE_BLUE,
  UMT_COLOR_DISTANCES_PARAM_BACKGROUNG_RED,
  UMT_COLOR_DISTANCES_PARAM_BACKGROUNG_GREEN,
  UMT_COLOR_DISTANCES_PARAM_BACKGROUNG_BLUE
};


STATIC enum UMT_COLOR_DISTANCES_PARAM CurrentParam = UMT_COLOR_DISTANCES_PARAM_SQUARE_RED;
STATIC UINT32 SquareX                              = 0;
STATIC UINT32 SquareY                              = 0;
STATIC UINT32 SquareWidth                          = 0;
STATIC UINT32 SquareHeight                         = 0;
STATIC GRAPHICS_PIXEL_COLOR SquareColor            = {238, 181, 71, 0};
STATIC GRAPHICS_PIXEL_COLOR BackgroundColor        = {0, 0, 0, 0};

VOID
ColorDistancesTestInit (
  IN UMT_CONTEXT *Ctx
  )
{
  GRAPHICS_CONTEXT *Graphics;
  UINT32 MinDimension;

  Graphics = Ctx->Graphics;
  MinDimension = MIN(Graphics->Width, Graphics->Height);
  SquareWidth = MinDimension / 2;
  SquareHeight = MinDimension / 2;
  SquareX = Graphics->Width / 2 - SquareWidth / 2;
  SquareY = Graphics->Height / 2 - SquareHeight / 2;

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
                  SquareColor.Red,
                  (CurrentParam == UMT_COLOR_DISTANCES_PARAM_SQUARE_GREEN) ? L'*' : L' ',
                  SquareColor.Green,
                  (CurrentParam == UMT_COLOR_DISTANCES_PARAM_SQUARE_BLUE) ? L'*' : L' ',
                  SquareColor.Blue,
                  (CurrentParam == UMT_COLOR_DISTANCES_PARAM_BACKGROUNG_RED) ? L'*' : L' ',
                  BackgroundColor.Red,
                  (CurrentParam == UMT_COLOR_DISTANCES_PARAM_BACKGROUNG_GREEN) ? L'*' : L' ',
                  BackgroundColor.Green,
                  (CurrentParam == UMT_COLOR_DISTANCES_PARAM_BACKGROUNG_BLUE) ? L'*' : L' ',
                  BackgroundColor.Blue);
}

VOID
ColorDistancesTestChangeParam (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        ParamStep
  )
{
  if (CurrentParam == 0 && ParamStep < 0) {
    CurrentParam = UMT_COLOR_DISTANCES_PARAM_BACKGROUNG_BLUE;
  } else {
    CurrentParam += ParamStep;
    if (CurrentParam > UMT_COLOR_DISTANCES_PARAM_BACKGROUNG_BLUE)
    {
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
  switch (CurrentParam)
  {
  case UMT_COLOR_DISTANCES_PARAM_SQUARE_RED:
    SquareColor.Red = MAX(0, MIN(255, SquareColor.Red + ValueStep));
    break;
  case UMT_COLOR_DISTANCES_PARAM_SQUARE_GREEN:
    SquareColor.Green = MAX(0, MIN(255, SquareColor.Green + ValueStep));
    break;
  case UMT_COLOR_DISTANCES_PARAM_SQUARE_BLUE:
    SquareColor.Blue = MAX(0, MIN(255, SquareColor.Blue + ValueStep));
    break;
  case UMT_COLOR_DISTANCES_PARAM_BACKGROUNG_RED:
    BackgroundColor.Red = MAX(0, MIN(255, BackgroundColor.Red + ValueStep));
    break;
   case UMT_COLOR_DISTANCES_PARAM_BACKGROUNG_GREEN:
    BackgroundColor.Green = MAX(0, MIN(255, BackgroundColor.Green + ValueStep));
    break;
  case UMT_COLOR_DISTANCES_PARAM_BACKGROUNG_BLUE:
    BackgroundColor.Blue = MAX(0, MIN(255, BackgroundColor.Blue + ValueStep));
    break;
  default:
    break;
  }

  ColorDistancesTestInit (Ctx);
}
