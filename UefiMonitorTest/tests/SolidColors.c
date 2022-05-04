#include <Library/DebugLib.h>
#include <Library/HiiLib.h>
#include <Library/MemoryAllocationLib.h>

#include "SolidColors.h"

STATIC enum UMT_COLORS CurrentColor = 0;

VOID
SolidColorsTestInit (
  IN UMT_CONTEXT *Ctx
  )
{
  GRAPHICS_CONTEXT *Graphics = Ctx->Graphics;

  PutRect (Graphics, 0, 0, Graphics->Width, Graphics->Height, &gUmtColors[CurrentColor]);
  if (Ctx->ShowTip)
    SolidColorsTestTip (Ctx);
} 

VOID
SolidColorsTestDoit (
  IN UMT_CONTEXT *Ctx
  )
{

}

VOID
SolidColorsTestTip (
  IN UMT_CONTEXT *Ctx
  )
{
  GRAPHICS_CONTEXT *Graphics;
  EFI_STRING_ID     TitleToken;
  EFI_STRING_ID     MsgToken;
  CHAR16            *Title;
  CHAR16            *Msg;

  Graphics = Ctx->Graphics;

  if (Ctx->ShowTip == FALSE) {
    // Restore
    PutRect (Graphics, 15, Graphics->Height - 15 - 104, 470, Graphics->Height - 15, &gUmtColors[CurrentColor]);
    return;
  }

  // TODO: or use CatSPrint()?
  switch (CurrentColor) {
    case UMT_COLOR_BLACK:
      TitleToken = STRING_TOKEN (STR_SOLID_COLORS_BLACK_TITLE);
      MsgToken = STRING_TOKEN (STR_SOLID_COLORS_BLACK_MSG);
      break;
    case UMT_COLOR_WHITE:
      TitleToken = STRING_TOKEN (STR_SOLID_COLORS_WHITE_TITLE);
      MsgToken = STRING_TOKEN (STR_SOLID_COLORS_WHITE_MSG);
      break;
    case UMT_COLOR_RED:
      TitleToken = STRING_TOKEN (STR_SOLID_COLORS_RED_TITLE);
      MsgToken = STRING_TOKEN (STR_SOLID_COLORS_RED_MSG);
      break;
    case UMT_COLOR_LIME:
      TitleToken = STRING_TOKEN (STR_SOLID_COLORS_GREEN_TITLE);
      MsgToken = STRING_TOKEN (STR_SOLID_COLORS_GREEN_MSG);
      break;
    case UMT_COLOR_BLUE:
      TitleToken = STRING_TOKEN (STR_SOLID_COLORS_BLUE_TITLE);
      MsgToken = STRING_TOKEN (STR_SOLID_COLORS_BLUE_MSG);
      break;
    default:
      ASSERT (FALSE);
      break;
  }

  Title = HiiGetString (gUmtHiiHandle, TitleToken, NULL);
  Msg = HiiGetString (gUmtHiiHandle, MsgToken, NULL);

  DrawRectWithBorder (Graphics, 15, Graphics->Height - 15 - 104, 470, Graphics->Height - 15,
    3, &gUmtColors[UMT_COLOR_WHITE], &gUmtColors[UMT_COLOR_NAVY]);
  DrawStringF (Graphics, 25, Graphics->Height - 15 - 94, &gUmtColors[UMT_COLOR_NAVY], Title);
  DrawStringF (Graphics, 25, Graphics->Height - 15 - 74, &gUmtColors[UMT_COLOR_BLACK], Msg);

  FreePool (Title);
  FreePool (Msg);
}

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
    if (CurrentColor >= UMT_COLOR_BLUE)
      CurrentColor = 0;
  }

  SolidColorsTestInit (Ctx);
}

VOID
SolidColorsTestChangeParam (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        ParamStep
  )
{
  
}

VOID
SolidColorsTestChangeVar (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        VariableStep // -1, +1
  )
{

}
