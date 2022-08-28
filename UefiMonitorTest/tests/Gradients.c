#include <Library/DebugLib.h>
#include <Library/HiiLib.h>
#include <Library/MemoryAllocationLib.h>

#include "UefiMonitorTest.h"

enum UMT_GRADIENTS_PARAM {
  UMT_GRADIENTS_PARAM_COLOR = 0,
  UMT_GRADIENTS_PARAM_STEP
};

enum UMT_GRADIENTS_STEPS {
  UMT_GRADIENTS_STEP_8 = 0,
  UMT_GRADIENTS_STEP_16,
  UMT_GRADIENTS_STEP_32,
  UMT_GRADIENTS_STEP_64,
  UMT_GRADIENTS_STEP_128,
  UMT_GRADIENTS_STEP_256,
  UMT_GRADIENTS_STEP_END
};

STATIC enum UMT_COLORS CurrentColor           = 1;
STATIC enum UMT_GRADIENTS_STEPS CurrentStep   = UMT_GRADIENTS_STEP_8;
STATIC enum UMT_GRADIENTS_PARAM CurrentParam  = UMT_GRADIENTS_PARAM_COLOR;

CONST UINT32 mGradientsSteps[UMT_GRADIENTS_STEP_END] = {
  8, 16, 32, 64, 128, 256
};

STATIC
VOID
GradientsTestInit (
  IN UMT_CONTEXT *Ctx
  )
{
  UINT32 Index;
  GRAPHICS_PIXEL_COLOR ColorOutput = { 0x00 };
  GRAPHICS_PIXEL_COLOR ColorInc    = { 0x00 };
  GRAPHICS_CONTEXT *Graphics = Ctx->Graphics;

  ColorInc.Red   = ((gUmtColors[CurrentColor].Color.Red) / (mGradientsSteps[CurrentStep]-1));
  ColorInc.Green = ((gUmtColors[CurrentColor].Color.Green) / (mGradientsSteps[CurrentStep]-1));
  ColorInc.Blue  = ((gUmtColors[CurrentColor].Color.Blue) / (mGradientsSteps[CurrentStep]-1));

  for (Index = 0; Index < mGradientsSteps[CurrentStep]; Index++)
  {
    PutRect (Graphics, (Graphics->Width * Index) / mGradientsSteps[CurrentStep],
                        0,
                        (Graphics->Width * (Index + 1)) / mGradientsSteps[CurrentStep],
                        Graphics->Height,
                        &ColorOutput);

    ColorOutput.Red   += ColorInc.Red;
    ColorOutput.Green += ColorInc.Green;
    ColorOutput.Blue  += ColorInc.Blue;
  }

  if (Ctx->ShowTip) {
    gGradientsTest.Tip(Ctx);
  }
}

STATIC
VOID
GradientsTestDoit (
  IN UMT_CONTEXT *Ctx
  )
{

}

STATIC
VOID
GradientsTestTip (
  IN UMT_CONTEXT *Ctx
  )
{
  GRAPHICS_CONTEXT *Graphics;
  CHAR16           *ColorMsg;

  Graphics = Ctx->Graphics;

  if (Ctx->ShowTip == FALSE) {
    // Restore
    gGradientsTest.Init(Ctx);
    return;
  }

  ColorMsg = HiiGetString (gUmtHiiHandle, gUmtColors[CurrentColor].StringId, NULL);

  DrawRectWithBorder (Graphics,
                      15,
                      Graphics->Height - 15 - 174,
                      510, Graphics->Height - 15,
                      3,
                      &gUmtColors[UMT_COLOR_WHITE].Color,
                      &gUmtColors[UMT_COLOR_NAVY].Color);

  DrawHiiStringF (Graphics,
                  25,
                  Graphics->Height - 15 - 164,
                  &gUmtColors[UMT_COLOR_NAVY].Color,
                  STRING_TOKEN (STR_GRADIENTS_TITLE), gUmtHiiHandle);

  DrawHiiStringF (Graphics,
                  25,
                  Graphics->Height - 15 - 144,
                  &gUmtColors[UMT_COLOR_BLACK].Color,
                  STRING_TOKEN (STR_GRADIENTS_MSG), gUmtHiiHandle,
                  (CurrentParam == UMT_GRADIENTS_PARAM_COLOR) ? L'*' : L' ',
                  ColorMsg,
                  (CurrentParam == UMT_GRADIENTS_PARAM_STEP) ? L'*' : L' ',
                  mGradientsSteps[CurrentStep]);

  FreePool (ColorMsg);
}

STATIC
VOID
GradientsTestChangeValue (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        ValueStep
  )
{
  switch (CurrentParam) {
  case UMT_GRADIENTS_PARAM_COLOR:
    if (CurrentColor == UMT_COLOR_WHITE && ValueStep < 0) {
      CurrentColor = UMT_COLOR_MAGENTA;
    } else {
      CurrentColor += ValueStep;
      if (CurrentColor > UMT_COLOR_MAGENTA) {
        CurrentColor = UMT_COLOR_WHITE;
      }
    }
    break;

  case UMT_GRADIENTS_PARAM_STEP:
    if (CurrentStep == 0 && ValueStep < 0) {
      return;
    } else {
      CurrentStep += ValueStep;
      if (CurrentStep > UMT_GRADIENTS_STEP_256)
      {
        CurrentStep = UMT_GRADIENTS_STEP_256;
        return;
      }
    }
    break;

  default:
    ASSERT(FALSE);
    return;
  }

  GradientsTestInit (Ctx);
}

STATIC
VOID
GradientsTestChangeParam (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        ParamStep
  )
{
  if (CurrentParam == 0 && ParamStep < 0) {
    CurrentParam = UMT_GRADIENTS_PARAM_STEP;
  } else {
    CurrentParam += ParamStep;
    if (CurrentParam > UMT_GRADIENTS_PARAM_STEP)
    {
      CurrentParam = UMT_GRADIENTS_PARAM_COLOR;
    }
  }

  gGradientsTest.Init(Ctx);
}

CONST UMT_STATE_ACTIONS gGradientsTest = {
  .Init         = GradientsTestInit,
  .Doit         = GradientsTestDoit,
  .Tip          = GradientsTestTip,
  .ChangeParam  = GradientsTestChangeParam,
  .ChangeValue  = GradientsTestChangeValue,
};
