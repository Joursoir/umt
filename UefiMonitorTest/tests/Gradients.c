#include <Library/DebugLib.h>
#include <Library/HiiLib.h>
#include <Library/MemoryAllocationLib.h>

#include "Gradients.h"

enum UMT_STEPS {
  UMT_STEP_256 = 0,
  UMT_STEP_128,
  UMT_STEP_64,
  UMT_STEP_32,
  UMT_STEP_16,
  UMT_STEP_8,
  UMT_STEP_END
};

enum UMT_PARAM {
  UMT_PARAM_COLOR = 0,
  UMT_PARAM_STEP
};

STATIC enum UMT_COLORS CurrentColor = 1;
STATIC enum UMT_STEPS CurrentStep = 0;
STATIC enum UMT_PARAM CurrentParam = UMT_PARAM_COLOR;

typedef struct {
  EFI_STRING_ID  StringId;
  UINT32         Step;
} UMT_STEPS;

CONST UMT_STEPS gUmtSteps[UMT_STEP_END] = {
  { STRING_TOKEN (STR_GRADIENTS_STEP_256),   256 },
  { STRING_TOKEN (STR_GRADIENTS_STEP_128),   128 },
  { STRING_TOKEN (STR_GRADIENTS_STEP_64),    64  },
  { STRING_TOKEN (STR_GRADIENTS_STEP_32),    32  },
  { STRING_TOKEN (STR_GRADIENTS_STEP_16),    16  },
  { STRING_TOKEN (STR_GRADIENTS_STEP_8),     8   }
};

VOID
GradientsTestInit (
  IN UMT_CONTEXT *Ctx
  )
{
  GRAPHICS_CONTEXT *Graphics = Ctx->Graphics;

  UINT32 Index;
  GRAPHICS_PIXEL_COLOR ColorOutput = { 0x00 };
  GRAPHICS_PIXEL_COLOR ColorInc = { 0x00 };

  ColorInc.Red   = ((gUmtColors[CurrentColor].Color.Red) / (gUmtSteps[CurrentStep].Step-1));
  ColorInc.Green = ((gUmtColors[CurrentColor].Color.Green) / (gUmtSteps[CurrentStep].Step-1));
  ColorInc.Blue  = ((gUmtColors[CurrentColor].Color.Blue) / (gUmtSteps[CurrentStep].Step-1));

  for (Index = 0; Index < gUmtSteps[CurrentStep].Step; Index++)
  {
    PutRect (Graphics, (Graphics->Width * Index) / gUmtSteps[CurrentStep].Step,
                        0,
                        ((Graphics->Width * (Index + 1)) / gUmtSteps[CurrentStep].Step),
                        Graphics->Height,
                        &ColorOutput);

    ColorOutput.Red   += ColorInc.Red;
    ColorOutput.Green += ColorInc.Green;
    ColorOutput.Blue  += ColorInc.Blue;
  }
  if (Ctx->ShowTip)
    GradientsTestTip (Ctx);
}

VOID
GradientsTestDoit (
  IN UMT_CONTEXT *Ctx
  )
{

}

VOID
GradientsTestTip (
  IN UMT_CONTEXT *Ctx
  )
{
  GRAPHICS_CONTEXT *Graphics;
  EFI_STRING_ID     TitleToken;
  EFI_STRING_ID     MsgToken;
  CHAR16            *Title;
  CHAR16            *Msg;
  CHAR16 	    *ColorMsg;
  CHAR16            *StepMsg;
  Graphics = Ctx->Graphics;

  if (Ctx->ShowTip == FALSE) {
    // Restore
    GradientsTestInit (Ctx);
    return;
  }

  MsgToken = STRING_TOKEN (STR_GRADIENTS_MSG);
  TitleToken = STRING_TOKEN (STR_GRADIENTS_TITLE);

  Title    = HiiGetString (gUmtHiiHandle, TitleToken, NULL);
  Msg      = HiiGetString (gUmtHiiHandle, MsgToken, NULL);
  ColorMsg = HiiGetString (gUmtHiiHandle, gUmtColors[CurrentColor].StringId, NULL);
  StepMsg  = HiiGetString (gUmtHiiHandle, gUmtSteps[CurrentStep].StringId, NULL);

  DrawRectWithBorder (Graphics,
                      15,
                      Graphics->Height - 15 - 174,
                      510, Graphics->Height - 15,
                      3,
                      &gUmtColors[UMT_COLOR_WHITE].Color,
                      &gUmtColors[UMT_COLOR_NAVY].Color);

  DrawStringF (Graphics,
               25,
               Graphics->Height - 15 - 164,
               &gUmtColors[UMT_COLOR_NAVY].Color, Title);

  DrawStringF (Graphics,
               25,
               Graphics->Height - 15 - 144,
               &gUmtColors[UMT_COLOR_BLACK].Color,
               Msg);

  DrawStringF (Graphics,
               25,
               Graphics->Height - 15 - 34,
               &gUmtColors[UMT_COLOR_BLUE].Color,
               ColorMsg);

  DrawStringF (Graphics,
               25,
               Graphics->Height - 15 - 20,
               &gUmtColors[UMT_COLOR_BLUE].Color,
               StepMsg);

  FreePool (Title);
  FreePool (Msg);
  FreePool (ColorMsg);
  FreePool (StepMsg);
}

VOID
GradientsTestChangeValue (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        ValueStep
  )
{
  switch (CurrentParam) {
  case UMT_PARAM_COLOR:
    if (CurrentColor == 1 && ValueStep < 0) {
      CurrentColor = UMT_COLOR_MAGENTA;
    } else {
      CurrentColor += ValueStep;
      if (CurrentColor > UMT_COLOR_MAGENTA) {
        CurrentColor = 1;
      }
    }
    break;

  case UMT_PARAM_STEP:
    if (CurrentStep == 0 && ValueStep < 0) {
      CurrentStep = UMT_STEP_8;
    } else {
      CurrentStep += ValueStep;
      if (CurrentStep > UMT_STEP_8)
      {
        CurrentStep = 0;
      }
    }
    break;

  default:
    ASSERT(FALSE);
    break;
  }
  GradientsTestInit (Ctx);
}

VOID
GradientsTestChangeParam (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        ParamStep
  )
{
  if (CurrentParam == 0 && ParamStep < 0) {
    CurrentParam = UMT_PARAM_STEP;
  } else {
    CurrentParam += ParamStep;
    if (CurrentParam > UMT_PARAM_STEP)
    {
      CurrentParam = UMT_PARAM_COLOR;
    }
  }
}

VOID
GradientsTestChangeVar (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        VariableStep
  )
{

}
