#include <Library/DebugLib.h>
#include <Library/HiiLib.h>
#include <Library/MemoryAllocationLib.h>

#include "Grayscale.h"

STATIC UINT32 CurrentGrayTone = 50;

VOID
GrayscaleTestInit (
  IN UMT_CONTEXT *Ctx
  )
{
  GRAPHICS_PIXEL_COLOR ColorOutput = { 0x00 };
  GRAPHICS_CONTEXT *Graphics = Ctx->Graphics;

  ColorOutput.Red   = (CurrentGrayTone * 255) / 100;
  ColorOutput.Green = (CurrentGrayTone * 255) / 100;
  ColorOutput.Blue  = (CurrentGrayTone * 255) / 100;

  PutRect (Graphics,
           0,
           0,
           Graphics->Width,
           Graphics->Height,
           &ColorOutput);

  if (Ctx->ShowTip) {
    GrayscaleTestTip (Ctx);
  }
}

VOID
GrayscaleTestDoit (
  IN UMT_CONTEXT *Ctx
  )
{

}

VOID
GrayscaleTestTip (
  IN UMT_CONTEXT *Ctx
  )
{
  GRAPHICS_CONTEXT *Graphics;

  Graphics = Ctx->Graphics;

  if (Ctx->ShowTip == FALSE) {
    // Restore
    GrayscaleTestInit (Ctx);
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
                  STRING_TOKEN (STR_GRAYSCALE_TITLE), gUmtHiiHandle);

  DrawHiiStringF (Graphics,
                  25,
                  Graphics->Height - 15 - 104,
                  &gUmtColors[UMT_COLOR_BLACK].Color,
                  STRING_TOKEN (STR_GRAYSCALE_MSG), gUmtHiiHandle,
                  CurrentGrayTone);
}

VOID
GrayscaleTestChangeParam (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        ParamStep
)
{

}

VOID
GrayscaleTestChangeValue (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        ValueStep
  )
{
  if (CurrentGrayTone == 5 && ValueStep < 0) {
    return;
  } else {
    CurrentGrayTone += ValueStep * 5;
    if (CurrentGrayTone > 95)
    {
      CurrentGrayTone = 95;
      return;
    }
  }

  GrayscaleTestInit (Ctx);
}
