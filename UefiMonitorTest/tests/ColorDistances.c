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

typedef struct 
{
  INT32 x;
  INT32 y;
  UINT32 Width;
  UINT32 Height;
} UMT_SQUARE;

typedef struct
{
  GRAPHICS_PIXEL_COLOR SquareColor;
  GRAPHICS_PIXEL_COLOR BackgroundColor;
} UMT_COLOR_DISTANCES_COLORS;


STATIC enum UMT_COLOR_DISTANCES_PARAM CurrentParam  = UMT_COLOR_DISTANCES_PARAM_SQUARE_RED;
STATIC UMT_SQUARE Square = {0, 0, 0, 0};
STATIC UMT_COLOR_DISTANCES_COLORS Colors = {{238, 181, 71, 0}, {0, 0, 0, 0}};

VOID
ColorDistancesTestInit (
  IN UMT_CONTEXT *Ctx
  ) 
{
  GRAPHICS_CONTEXT *Graphics = Ctx->Graphics;

  UINT32 MinDimension = MIN(Graphics->Width, Graphics->Height);
  Square.Width = MinDimension / 2;
  Square.Height = MinDimension / 2;
  Square.x = Graphics->Width / 2 - Square.Width / 2;
  Square.y = Graphics->Height / 2 - Square.Height / 2;
  
  PutRect (Graphics, 0, 0, Graphics->Width, Graphics->Height, &Colors.BackgroundColor);

  
  PutRect (Graphics,
           Square.x,
           Square.y,
           Square.x + Square.Width,
           Square.y + Square.Height,
           &Colors.SquareColor);
  
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
                  Colors.SquareColor.Red,
                  (CurrentParam == UMT_COLOR_DISTANCES_PARAM_SQUARE_GREEN) ? L'*' : L' ',
                  Colors.SquareColor.Green,
                  (CurrentParam == UMT_COLOR_DISTANCES_PARAM_SQUARE_BLUE) ? L'*' : L' ',
                  Colors.SquareColor.Blue,
                  (CurrentParam == UMT_COLOR_DISTANCES_PARAM_BACKGROUNG_RED) ? L'*' : L' ',
                  Colors.BackgroundColor.Red,
                  (CurrentParam == UMT_COLOR_DISTANCES_PARAM_BACKGROUNG_GREEN) ? L'*' : L' ',
                  Colors.BackgroundColor.Green,
                  (CurrentParam == UMT_COLOR_DISTANCES_PARAM_BACKGROUNG_BLUE) ? L'*' : L' ',
                  Colors.BackgroundColor.Blue);
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
    Colors.SquareColor.Red = MAX(0, MIN(255, Colors.SquareColor.Red + ValueStep));
    break;
  case UMT_COLOR_DISTANCES_PARAM_SQUARE_GREEN:
    Colors.SquareColor.Green = MAX(0, MIN(255, Colors.SquareColor.Green + ValueStep));
    break;
  case UMT_COLOR_DISTANCES_PARAM_SQUARE_BLUE:
    Colors.SquareColor.Blue = MAX(0, MIN(255, Colors.SquareColor.Blue + ValueStep));
    break;
  case UMT_COLOR_DISTANCES_PARAM_BACKGROUNG_RED:
    Colors.BackgroundColor.Red = MAX(0, MIN(255, Colors.BackgroundColor.Red + ValueStep));
    break;
   case UMT_COLOR_DISTANCES_PARAM_BACKGROUNG_GREEN:
    Colors.BackgroundColor.Green = MAX(0, MIN(255, Colors.BackgroundColor.Green + ValueStep));
    break;
  case UMT_COLOR_DISTANCES_PARAM_BACKGROUNG_BLUE:
    Colors.BackgroundColor.Blue = MAX(0, MIN(255, Colors.BackgroundColor.Blue + ValueStep));
    break;
  default:
    break;
  }

  ColorDistancesTestInit (Ctx);
}
