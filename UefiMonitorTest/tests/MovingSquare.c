#include <Uefi.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include "UefiMonitorTest.h"

typedef struct
{
  INT32 x;
  INT32 y;
  UINT32 Width;
  UINT32 Height;
  INT32 dx;
  INT32 dy;
  GRAPHICS_PIXEL_COLOR Color;
} UMT_SQUARE;

enum UMT_RESPONSE_TIME_PARAM {
  UMT_RESPONSE_TIME_PARAM_SQUARE_RED = 0,
  UMT_RESPONSE_TIME_PARAM_SQUARE_GREEN,
  UMT_RESPONSE_TIME_PARAM_SQUARE_BLUE,
  UMT_RESPONSE_TIME_PARAM_BACKGROUND_RED,
  UMT_RESPONSE_TIME_PARAM_BACKGROUND_GREEN,
  UMT_RESPONSE_TIME_PARAM_BACKGROUND_BLUE,
  UMT_RESPONSE_TIME_PARAM_END
};

STATIC UINT8 mColors[UMT_RESPONSE_TIME_PARAM_END] = {
  238,
  181,
  71,
  0,
  0,
  0
};

STATIC UMT_SQUARE Square = {0, 0, 0, 0, 2, 1, {238, 181, 71}};
STATIC GRAPHICS_PIXEL_COLOR BackgroundColor = {0, 0, 0, 0};
STATIC EFI_EVENT FrameTimer;
STATIC enum UMT_RESPONSE_TIME_PARAM CurrentParam = UMT_RESPONSE_TIME_PARAM_SQUARE_RED;

#define MILLI 1000

STATIC
VOID
DrawFrame (
  IN GRAPHICS_CONTEXT *Context
)
{
  GRAPHICS_CONTEXT *Graphics = Context;

  BackgroundColor.Red   = mColors[UMT_RESPONSE_TIME_PARAM_BACKGROUND_RED];
  BackgroundColor.Green = mColors[UMT_RESPONSE_TIME_PARAM_BACKGROUND_GREEN];
  BackgroundColor.Blue  = mColors[UMT_RESPONSE_TIME_PARAM_BACKGROUND_BLUE];

  PutRect (Graphics, 0, 0, Graphics->Width, Graphics->Height, &BackgroundColor);

  UINT32 NextPosX = Square.x + Square.Width + Square.dx;
  UINT32 NextPosY = Square.y + Square.Height + Square.dy;

  Square.dx = (NextPosX > Graphics->Width) || (Square.x + Square.dx < 0) ? -Square.dx : Square.dx;
  Square.dy = (NextPosY > Graphics->Height) || (Square.y + Square.dy < 0) ? -Square.dy : Square.dy;
  Square.x += Square.dx;
  Square.y += Square.dy;

  Square.Color.Red   = mColors[UMT_RESPONSE_TIME_PARAM_SQUARE_RED];
  Square.Color.Blue  = mColors[UMT_RESPONSE_TIME_PARAM_SQUARE_BLUE];
  Square.Color.Green = mColors[UMT_RESPONSE_TIME_PARAM_SQUARE_GREEN];

  PutRect (Graphics,
           Square.x,
           Square.y,
           Square.x + Square.Width,
           Square.y + Square.Height,
           &Square.Color);
}

VOID
MovingSquareTestInit (
  IN UMT_CONTEXT *Ctx
  )
{
  GRAPHICS_CONTEXT *Graphics = Ctx->Graphics;

  Square.Width  = Graphics->Width / 5;
  Square.Height = Graphics->Height / 5;
  Square.x      = Graphics->Width / 2 - Square.Width / 2;
  Square.y      = Graphics->Height / 2 - Square.Height / 2;

  PutRect (Graphics, 0, 0, Graphics->Width, Graphics->Height, &BackgroundColor);

  PutRect (Graphics,
           Square.x,
           Square.y,
           Square.x + Square.Width,
           Square.y + Square.Height,
           &Square.Color);

  if (Ctx->ShowTip) {
    gResponseTimeTest.Init (Ctx);
  }
}

VOID
MovingSquareTestDoit (
  IN UMT_CONTEXT *Ctx
  )
{
  gBS->CreateEvent (EVT_TIMER,
                    TPL_CALLBACK,
                    (EFI_EVENT_NOTIFY) NULL,
                    (VOID*) NULL,
                    &FrameTimer);

  gBS->SetTimer (FrameTimer, TimerRelative, (10 * MILLI));

  UINTN ind;
  EFI_STATUS Status;

  Status = gBS->WaitForEvent(1, &FrameTimer, &ind);
  if (!EFI_ERROR (Status)) {
    DrawFrame (Ctx->Graphics);
  }
  gBS->CloseEvent (FrameTimer);
  if (Ctx->ShowTip == TRUE) {
    gResponseTimeTest.Tip (Ctx);
  }
}

VOID
MovingSquareTestTip (
  IN UMT_CONTEXT *Ctx
  )
{
  if (Ctx->ShowTip == FALSE) {
    PutRect (Ctx->Graphics, 0, 0, Ctx->Graphics->Width, Ctx->Graphics->Height, &BackgroundColor);
    return;
  }

  GRAPHICS_CONTEXT *Graphics = Ctx->Graphics;

  DrawRectWithBorder (Graphics,
                      15,
                      Graphics->Height - 15 - 234,
                      535, Graphics->Height - 15,
                      3,
                      &gUmtColors[UMT_COLOR_WHITE].Color,
                      &gUmtColors[UMT_COLOR_NAVY].Color);

  DrawHiiStringF (Graphics,
                  25,
                  Graphics->Height - 15 - 224,
                  &gUmtColors[UMT_COLOR_NAVY].Color,
                  STRING_TOKEN (STR_MOVING_SQUARE_TITLE), gUmtHiiHandle);

  DrawHiiStringF (Graphics,
                  25,
                  Graphics->Height - 15 - 204,
                  &gUmtColors[UMT_COLOR_NAVY].Color,
                  STRING_TOKEN (STR_MOVING_SQUARE_MSG), gUmtHiiHandle,
                  (CurrentParam == UMT_RESPONSE_TIME_PARAM_SQUARE_RED) ? L'*' : L' ',
                  mColors[UMT_RESPONSE_TIME_PARAM_SQUARE_RED],
                  (CurrentParam == UMT_RESPONSE_TIME_PARAM_SQUARE_GREEN) ? L'*' : L' ',
                  mColors[UMT_RESPONSE_TIME_PARAM_SQUARE_GREEN],
                  (CurrentParam == UMT_RESPONSE_TIME_PARAM_SQUARE_BLUE) ? L'*' : L' ',
                  mColors[UMT_RESPONSE_TIME_PARAM_SQUARE_BLUE],
                  (CurrentParam == UMT_RESPONSE_TIME_PARAM_BACKGROUND_RED) ? L'*' : L' ',
                  mColors[UMT_RESPONSE_TIME_PARAM_BACKGROUND_RED],
                  (CurrentParam == UMT_RESPONSE_TIME_PARAM_BACKGROUND_GREEN) ? L'*' : L' ',
                  mColors[UMT_RESPONSE_TIME_PARAM_BACKGROUND_GREEN],
                  (CurrentParam == UMT_RESPONSE_TIME_PARAM_BACKGROUND_BLUE) ? L'*' : L' ',
                  mColors[UMT_RESPONSE_TIME_PARAM_BACKGROUND_BLUE]);
}

VOID
MovingSquareTestChangeParam (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        ParamStep
  )
{
  if (CurrentParam == 0 && ParamStep < 0) {
    CurrentParam = UMT_RESPONSE_TIME_PARAM_BACKGROUND_BLUE;
  } else {
    CurrentParam += ParamStep;
    if (CurrentParam > UMT_RESPONSE_TIME_PARAM_BACKGROUND_BLUE) {
      CurrentParam = UMT_RESPONSE_TIME_PARAM_SQUARE_RED;
    }
  }
}

VOID
MovingSquareTestChangeValue (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        ValueStep
  )
{
  mColors[CurrentParam] = MAX(0, MIN(255, mColors[CurrentParam] + ValueStep));
}

CONST UI_ENTRY gResponseTimeTest = {
  .Init         = MovingSquareTestInit,
  .Doit         = MovingSquareTestDoit,
  .Tip          = MovingSquareTestTip,
  .ChangeParam  = MovingSquareTestChangeParam,
  .ChangeValue  = MovingSquareTestChangeValue,
};
