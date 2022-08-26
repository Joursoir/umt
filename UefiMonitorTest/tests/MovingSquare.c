#include "MovingSquare.h"
#include <Uefi.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>

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

typedef struct
{
  EFI_EVENT PeriodicTimer;
  UMT_CONTEXT *Ctx;
} UMT_FRAME;

STATIC UMT_SQUARE Square = {0, 0, 0, 0, 2, 1, {238, 181, 71}};
STATIC GRAPHICS_PIXEL_COLOR BackgroundColor = {0, 0, 0, 0};
STATIC EFI_EVENT FrameTimer;

VOID
DrawFrame (
  IN GRAPHICS_CONTEXT *Context
);

VOID
MovingSquareTestInit (
  IN UMT_CONTEXT *Ctx
  )
{
  GRAPHICS_CONTEXT *Graphics = Ctx->Graphics;

  Square.Width = Graphics->Width / 5;
  Square.Height = Graphics->Height / 5;
  Square.x = Graphics->Width / 2 - Square.Width / 2;
  Square.y = Graphics->Height / 2 - Square.Height / 2;

  PutRect (Graphics, 0, 0, Graphics->Width, Graphics->Height, &BackgroundColor);

  PutRect (Graphics,
           Square.x,
           Square.y,
           Square.x + Square.Width,
           Square.y + Square.Height,
           &Square.Color);

  if (Ctx->ShowTip) {
    MovingSquareTestInit (Ctx);
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

  gBS->SetTimer(FrameTimer, TimerRelative, (10 * 1000));

  UINTN ind;
  EFI_STATUS Status;

  Status = gBS->WaitForEvent(1, &FrameTimer, &ind);
  if (!EFI_ERROR (Status)) {
    DrawFrame(Ctx->Graphics);
  }
  gBS->CloseEvent(FrameTimer);
  if (Ctx->ShowTip == TRUE) {
    MovingSquareTestTip(Ctx);
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
                      Graphics->Height - 15 - 84,
                      535, Graphics->Height - 15,
                      3,
                      &gUmtColors[UMT_COLOR_WHITE].Color,
                      &gUmtColors[UMT_COLOR_NAVY].Color);

  DrawHiiStringF (Graphics,
                  25,
                  Graphics->Height - 15 - 74,
                  &gUmtColors[UMT_COLOR_NAVY].Color,
                  STRING_TOKEN (STR_MOVING_SQUARE_TITLE), gUmtHiiHandle);

  DrawHiiStringF (Graphics,
                  25,
                  Graphics->Height - 15 - 54,
                  &gUmtColors[UMT_COLOR_NAVY].Color,
                  STRING_TOKEN (STR_MOVING_SQUARE_MSG), gUmtHiiHandle);
}

VOID
MovingSquareTestChangeParam (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        ParamStep
  )
{

}

VOID
MovingSquareTestChangeValue (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        ValueStep
  ) {}

VOID
DrawFrame (
  IN GRAPHICS_CONTEXT *Context
)
{
  GRAPHICS_CONTEXT *Graphics = Context;

  PutRect (Graphics,
           Square.x,
           Square.y,
           Square.x + Square.Width,
           Square.y + Square.Height,
           &BackgroundColor);

  UINT32 NextPosX = Square.x + Square.Width + Square.dx;
  UINT32 NextPosY = Square.y + Square.Height + Square.dy;

  Square.dx = (NextPosX > Graphics->Width) || (Square.x + Square.dx < 0) ? -Square.dx : Square.dx;
  Square.dy = (NextPosY > Graphics->Height) || (Square.y + Square.dy < 0) ? -Square.dy : Square.dy;
  Square.x += Square.dx;
  Square.y += Square.dy;

  PutRect (Graphics,
           Square.x,
           Square.y,
           Square.x + Square.Width,
           Square.y + Square.Height,
           &Square.Color);
}
