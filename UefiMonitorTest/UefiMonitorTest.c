#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include "UefiMonitorTest.h"
#include "MainMenu.h"

STATIC CONST UMT_STATE_ACTIONS mStateActions[UMT_STATE_END] = {
  { MainMenuInit, MainMenuDoit, MainMenuTip, MainMenuKeyRight, MainMenuKeyLeft }
};

STATIC
EFI_GRAPHICS_OUTPUT_PROTOCOL *
GetGraphicsOutputProtocol (
  VOID
  )
{
  EFI_STATUS                            Status;
  EFI_GRAPHICS_OUTPUT_PROTOCOL          *Gop;
  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION  *ModeInfo;
  UINTN                                 SizeOfInfo;

  Status = gBS->LocateProtocol (
                  &gEfiGraphicsOutputProtocolGuid,
                  NULL,
                  (VOID **)&Gop
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Unable to locate GOP\n"));
    return NULL;
  }

  Status = Gop->QueryMode (
                  Gop,
                  (Gop->Mode == NULL) ? 0 : Gop->Mode->Mode,
                  &SizeOfInfo,
                  &ModeInfo
                  );
  if (Status == EFI_NOT_STARTED) {
    Status = Gop->SetMode (Gop, 0);
  }
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Unable to get native mode\n"));
    return NULL;
  }

  // TODO: free ModeInfo
  return Gop;
}

STATIC
EFI_STATUS
Run (
  IN GRAPHICS_CONTEXT *Graphics
  )
{
  UMT_CONTEXT Ctx;

  Ctx.State     = UMT_STATE_MAIN_MENU;
  Ctx.Running   = TRUE;
  Ctx.ShowTip   = FALSE;
  Ctx.Actions   = &mStateActions[Ctx.State];
  Ctx.Graphics  = Graphics;
  Ctx.Actions->Init (&Ctx);

  while (Ctx.Running == TRUE)
  {
    Ctx.Actions->Doit (&Ctx);

    // Buffer swap:
    CopyMem (Graphics->FrontBuffer, Graphics->BackBuffer, Graphics->BufferSize);
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_GRAPHICS_OUTPUT_PROTOCOL  *Gop;
  GRAPHICS_CONTEXT              Graphics;
  EFI_STATUS                    Status;

  Status = EFI_SUCCESS;

  Gop = GetGraphicsOutputProtocol ();
  if (Gop == NULL) {
    Print (L"Error: Getting a Graphical Output Protocol is failed\n");
    return EFI_NOT_FOUND;
  }

  Status = PrepareGraphicsInfo (&Graphics, Gop);
  if (EFI_ERROR(Status)) {
    Print (L"Error: Preparing graphics information is failed. %r\n", Status);
    return EFI_NOT_FOUND;
  }

  Status = Run (&Graphics);

  ForgetGraphicsInfo (&Graphics);

  return Status;
}
