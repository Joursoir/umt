#include <Library/DebugLib.h>
#include <Library/HiiLib.h>
#include <Library/MemoryAllocationLib.h>

#include "ColoredLines.h"

STATIC enum UMT_COLORS CurrentColor = 1;
STATIC enum UMT_STEPS CurrentStep = 0;

CONST UINT32 gUmtSteps[UMT_STEP_END] = {
  256,
  128,
  64,
  32,
  16,
  8,
};

VOID
ColoredLinesTestInit (
  IN UMT_CONTEXT *Ctx
  )
{
  GRAPHICS_CONTEXT *Graphics = Ctx->Graphics;

  //STATIC UINT32 X = 8;
  STATIC UINT32 Index = 0;
  STATIC GRAPHICS_PIXEL_COLOR ColorOutput = {0x00, 0x00, 0x00, 0x00};
  STATIC GRAPHICS_PIXEL_COLOR ColorInc = {0x00, 0x00, 0x00, 0x00};

  ColorOutput.Red = 0;
  ColorOutput.Green = 0;
  ColorOutput.Blue = 0;

  ColorInc.Red = ((gUmtColors[CurrentColor].Red) / (gUmtSteps[CurrentStep]-1));
  ColorInc.Green = ((gUmtColors[CurrentColor].Green) / (gUmtSteps[CurrentStep]-1));
  ColorInc.Blue = ((gUmtColors[CurrentColor].Blue) / (gUmtSteps[CurrentStep]-1));

  for (Index = 0; Index < gUmtSteps[CurrentStep]; Index++)
  {
    PutRect(Graphics, (Graphics->Width * Index) / gUmtSteps[CurrentStep], 0, ((Graphics->Width * (Index + 1)) / gUmtSteps[CurrentStep]), Graphics->Height, &ColorOutput);
    ColorOutput.Red += ColorInc.Red;
    ColorOutput.Green += ColorInc.Green;
    ColorOutput.Blue += ColorInc.Blue;
  }
  if (Ctx->ShowTip)
    ColoredLinesTestTip (Ctx);
}

VOID
ColoredLinesTestDoit (
  IN UMT_CONTEXT *Ctx
  )
{

}

VOID
ColoredLinesTestTip (
  IN UMT_CONTEXT *Ctx
  )
{
  GRAPHICS_CONTEXT *Graphics;
  EFI_STRING_ID     TitleToken;
  EFI_STRING_ID     MsgToken;
  EFI_STRING_ID     ColorToken;
  EFI_STRING_ID     StepToken;
  CHAR16            *Title;
  CHAR16            *Msg;
  CHAR16 	    *ColorMsg;
  CHAR16            *StepMsg;
  Graphics = Ctx->Graphics;

  if (Ctx->ShowTip == FALSE) {
    // Restore
    ColoredLinesTestInit (Ctx);
    return;
  }

  // TODO: or use CatSPrint()?
  switch (CurrentColor) {
    case UMT_COLOR_WHITE:
      ColorToken = STRING_TOKEN (STR_COLORED_LINES_WHITE);
      break;
    case UMT_COLOR_RED:
      ColorToken = STRING_TOKEN (STR_COLORED_LINES_RED);
      break;
    case UMT_COLOR_LIME:
      ColorToken = STRING_TOKEN (STR_COLORED_LINES_GREEN);
      break;
    case UMT_COLOR_BLUE:
      ColorToken = STRING_TOKEN (STR_COLORED_LINES_BLUE);
      break;
    case UMT_COLOR_YELLOW:
      ColorToken = STRING_TOKEN (STR_COLORED_LINES_YELLOW);
      break;
    case UMT_COLOR_AQUA:
      ColorToken = STRING_TOKEN (STR_COLORED_LINES_AQUA);
      break;
    case UMT_COLOR_MAGENTA:
      ColorToken = STRING_TOKEN (STR_COLORED_LINES_MAGENTA);
      break;
    default:
      ASSERT (FALSE);
      break;
  }

  switch (CurrentStep) {
  case UMT_STEP_256:
    StepToken = STRING_TOKEN (STR_COLORED_LINES_STEP_256);
    break;
  case UMT_STEP_128:
    StepToken = STRING_TOKEN (STR_COLORED_LINES_STEP_128);
    break;
  case UMT_STEP_64:
    StepToken = STRING_TOKEN (STR_COLORED_LINES_STEP_64);
    break;
  case UMT_STEP_32:
    StepToken = STRING_TOKEN (STR_COLORED_LINES_STEP_32);
    break;
  case UMT_STEP_16:
    StepToken = STRING_TOKEN (STR_COLORED_LINES_STEP_16);
    break;
  case UMT_STEP_8:
    StepToken = STRING_TOKEN (STR_COLORED_LINES_STEP_8);
    break;
  default:
    ASSERT(FALSE);
    break;
  }

  MsgToken = STRING_TOKEN (STR_COLORED_LINES_MSG);
  TitleToken = STRING_TOKEN (STR_COLORED_LINES_TITLE);

  Title = HiiGetString (gUmtHiiHandle, TitleToken, NULL);
  Msg = HiiGetString (gUmtHiiHandle, MsgToken, NULL);
  ColorMsg = HiiGetString (gUmtHiiHandle, ColorToken, NULL);
  StepMsg = HiiGetString (gUmtHiiHandle, StepToken, NULL);

  DrawRectWithBorder (Graphics, 15, Graphics->Height - 15 - 174, 510, Graphics->Height - 15,
    3, &gUmtColors[UMT_COLOR_WHITE], &gUmtColors[UMT_COLOR_NAVY]);
  DrawStringF (Graphics, 25, Graphics->Height - 15 - 164, &gUmtColors[UMT_COLOR_NAVY], Title);
  DrawStringF (Graphics, 25, Graphics->Height - 15 - 144, &gUmtColors[UMT_COLOR_BLACK], Msg);
  DrawStringF (Graphics, 25, Graphics->Height - 15 - 34, &gUmtColors[UMT_COLOR_BLUE], ColorMsg);
  DrawStringF (Graphics, 25, Graphics->Height - 15 - 20, &gUmtColors[UMT_COLOR_BLUE],StepMsg);

  FreePool (Title);
  FreePool (Msg);
  FreePool (ColorMsg);
  FreePool (StepMsg);
}

VOID
ColoredLinesTestChangeValue (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        ValueStep
  )
{
  if (CurrentColor == 1 && ValueStep < 0)
    CurrentColor = UMT_COLOR_MAGENTA;
  else {
    CurrentColor += ValueStep;
    if (CurrentColor > UMT_COLOR_MAGENTA)
      CurrentColor = 1;
  }

  ColoredLinesTestInit (Ctx);
}

VOID
ColoredLinesTestChangeParam (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        ParamStep
  )
{
  if (CurrentStep == 0 && ParamStep < 0)
    CurrentStep = UMT_STEP_8;
  else {
    CurrentStep += ParamStep;
    if (CurrentStep > UMT_STEP_8)
      CurrentStep = 0;
  }

  ColoredLinesTestInit (Ctx);

}

VOID
ColoredLinesTestChangeVar (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        VariableStep // -1, +1
  )
{

}
