#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>

#include "UefiMonitorTest.h"

#define INDENT            15
#define RECT_HALF_WIDTH   160
#define RECT_HALF_HEIGHT  28

STATIC UINT32 mModeNumber = 0;

STATIC
EFI_STATUS
SetGraphicsMode (
  IN GRAPHICS_CONTEXT *Graphics,
  IN UINT32           ModeNumber
  )
{
  EFI_STATUS Status;

  ASSERT (Graphics != NULL);

  if (Graphics->Gop->Mode->Mode == ModeNumber)
    return EFI_SUCCESS;

  Status = Graphics->Gop->SetMode (Graphics->Gop, ModeNumber);
  if (!EFI_ERROR (Status)) {
    ForgetGraphicsInfo (Graphics);
    PrepareGraphicsInfo (Graphics, Graphics->Gop);
  }
  return Status;
}

STATIC
EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *
GetGraphicsModeInfo (
  IN  GRAPHICS_CONTEXT *Graphics,
  IN  UINT32           ModeNumber
  )
{
  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION  *ModeInfo;
  EFI_STATUS                            Status;
  UINTN                                 SizeOfInfo;

  if (ModeNumber >= Graphics->Gop->Mode->MaxMode)
    return NULL;

  Status = Graphics->Gop->QueryMode(Graphics->Gop, ModeNumber, &SizeOfInfo, &ModeInfo);
  if (EFI_ERROR(Status)) {
    ModeInfo = NULL;
  }
  return ModeInfo;
}

STATIC
VOID
SettingsMenuInit (
  IN UMT_CONTEXT *Ctx
  )
{
  GRAPHICS_CONTEXT                      *Graphics;
  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION  *ModeInfo;
  UINT32                                 Width;
  UINT32                                 Height;
  UINTN                                  FontHeight;
  UINTN                                  FontWidth;
  UINTN                                  Y;

  Graphics   = Ctx->Graphics;
  Width      = Graphics->Width;
  Height     = Graphics->Height;
  GetFontParameters (Graphics, &FontWidth, &FontHeight);

  // Background
  Y = Height / 2 - RECT_HALF_HEIGHT;
  PutRect (Graphics, 0, 0, Width, Height, &gUmtColors[UMT_COLOR_NAVY].Color);
  DrawHiiStringF (Graphics,
                  (Width - 13 * FontWidth) / 2,
                  Y - (INDENT * 2),
                  &gUmtColors[UMT_COLOR_WHITE].Color,
                  STRING_TOKEN (STR_SM_TITLE), gUmtHiiHandle);

  // Menu
  DrawRoundedRect (Graphics,
                  Width  / 2 - RECT_HALF_WIDTH,
                  Height / 2 - RECT_HALF_HEIGHT,
                  Width  / 2 + RECT_HALF_WIDTH,
                  Height / 2 + RECT_HALF_HEIGHT,
                  20,
                  &gUmtColors[UMT_COLOR_WHITE].Color
                  );

  ModeInfo = GetGraphicsModeInfo (Graphics, mModeNumber);
  ASSERT (ModeInfo != NULL);

  Y += INDENT + 5;
  DrawHiiStringF (Graphics,
                  Width / 2 - RECT_HALF_WIDTH + INDENT,
                  Y,
                  &gUmtColors[UMT_COLOR_BLACK].Color,
                  STRING_TOKEN (STR_SM_RESOLUTION_TITLE), gUmtHiiHandle);
  DrawHiiStringF (Graphics,
                  Width / 2 + INDENT,
                  Y,
                  &gUmtColors[UMT_COLOR_BLACK].Color,
                  STRING_TOKEN (STR_SM_RESOLUTION_OPTION), gUmtHiiHandle,
                  ModeInfo->HorizontalResolution,
                  ModeInfo->VerticalResolution,
                  (Graphics->Gop->Mode->Mode == mModeNumber) ? L'*' : L' ');

  FreePool (ModeInfo);
}

STATIC
VOID
SettingsMenuDoit (
  IN UMT_CONTEXT *Ctx
  )
{

}

STATIC
VOID
SettingsMenuTip (
  IN UMT_CONTEXT *Ctx
  )
{
  EFI_STATUS Status;

  Status = SetGraphicsMode (Ctx->Graphics, mModeNumber);
  if (EFI_ERROR(Status))
    return;

  gSettingsMenu.Init(Ctx);
}

STATIC
VOID
SettingsChangeParam (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        ParamStep
  )
{
  
}

STATIC
VOID
SettingsMenuChangeValue (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        ValueStep
  )
{
  if (mModeNumber == 0 && ValueStep < 0)
    mModeNumber = Ctx->Graphics->Gop->Mode->MaxMode - 1;
  else {
    mModeNumber += ValueStep;
    if (mModeNumber >= Ctx->Graphics->Gop->Mode->MaxMode)
      mModeNumber = 0;
  }

  gSettingsMenu.Init(Ctx);
}

CONST UMT_STATE_ACTIONS gSettingsMenu = {
  .Init         = SettingsMenuInit,
  .Doit         = SettingsMenuDoit,
  .Tip          = SettingsMenuTip,
  .ChangeParam  = SettingsChangeParam,
  .ChangeValue  = SettingsMenuChangeValue,
};
