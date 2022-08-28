#include <Library/DebugLib.h>
#include <Library/HiiLib.h>
#include <Library/MemoryAllocationLib.h>

#include "UefiMonitorTest.h"

STATIC enum UMT_COLORS CurrentColor = 0;

STATIC
VOID
SolidColorsTestInit (
  IN UMT_CONTEXT *Ctx
  )
{
  GRAPHICS_CONTEXT *Graphics = Ctx->Graphics;

  PutRect (Graphics, 0, 0, Graphics->Width, Graphics->Height, &gUmtColors[CurrentColor].Color);
  if (Ctx->ShowTip)
    gSolidColorsTest.Tip(Ctx);
} 

STATIC
VOID
SolidColorsTestDoit (
  IN UMT_CONTEXT *Ctx
  )
{

}

STATIC
VOID
SolidColorsTestTip (
  IN UMT_CONTEXT *Ctx
  )
{
  GRAPHICS_CONTEXT *Graphics;
  EFI_STRING_ID     MsgToken;
  CHAR16            *ColorName;
  CHAR16            *Msg;

  Graphics = Ctx->Graphics;

  if (Ctx->ShowTip == FALSE) {
    // Restore
    PutRect (Graphics, 15, Graphics->Height - 15 - 104, 470, Graphics->Height - 15, &gUmtColors[CurrentColor].Color);
    return;
  }

  switch (CurrentColor) {
    case UMT_COLOR_BLACK:
      MsgToken = STRING_TOKEN (STR_SOLID_COLORS_BLACK_MSG);
      break;
    case UMT_COLOR_WHITE:
      MsgToken = STRING_TOKEN (STR_SOLID_COLORS_WHITE_MSG);
      break;
    case UMT_COLOR_RED:
      MsgToken = STRING_TOKEN (STR_SOLID_COLORS_RED_MSG);
      break;
    case UMT_COLOR_LIME:
      MsgToken = STRING_TOKEN (STR_SOLID_COLORS_GREEN_MSG);
      break;
    case UMT_COLOR_BLUE:
      MsgToken = STRING_TOKEN (STR_SOLID_COLORS_BLUE_MSG);
      break;
    default:
      ASSERT (FALSE);
      break;
  }

  ColorName = HiiGetString (gUmtHiiHandle, gUmtColors[CurrentColor].StringId, NULL);
  Msg       = HiiGetString (gUmtHiiHandle, MsgToken, NULL);

  DrawRectWithBorder (Graphics, 15, Graphics->Height - 15 - 104, 470, Graphics->Height - 15,
    3, &gUmtColors[UMT_COLOR_WHITE].Color, &gUmtColors[UMT_COLOR_NAVY].Color);
  
  DrawHiiStringF (Graphics, 
                  25,
                  Graphics->Height - 15 - 94, 
                  &gUmtColors[UMT_COLOR_NAVY].Color,
                  STRING_TOKEN (STR_SOLID_COLORS_TITLE), gUmtHiiHandle,
                  ColorName);
  DrawStringF (Graphics, 25, Graphics->Height - 15 - 74, &gUmtColors[UMT_COLOR_BLACK].Color, Msg);

  FreePool (ColorName);
  FreePool (Msg);
}

STATIC
VOID
SolidColorsTestChangeValue (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        ValueStep
  )
{
  if (CurrentColor == 0 && ValueStep < 0)
    CurrentColor = UMT_COLOR_BLUE;
  else {
    CurrentColor += ValueStep;
    if (CurrentColor > UMT_COLOR_BLUE)
      CurrentColor = 0;
  }

  gSolidColorsTest.Init(Ctx);
}

STATIC
VOID
SolidColorsTestChangeParam (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        ParamStep
  )
{
  
}

CONST UMT_STATE_ACTIONS gSolidColorsTest = {
  .Init         = SolidColorsTestInit,
  .Doit         = SolidColorsTestDoit,
  .Tip          = SolidColorsTestTip,
  .ChangeParam  = SolidColorsTestChangeParam,
  .ChangeValue  = SolidColorsTestChangeValue,
};
