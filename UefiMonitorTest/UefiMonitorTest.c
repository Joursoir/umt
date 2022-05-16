#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Library/HiiLib.h>
#include <Library/UefiHiiServicesLib.h>

#include "UefiMonitorTest.h"
#include "MainMenu.h"
#include "SettingsMenu.h"
#include "tests/SolidColors.h"
#include <tests/Grayscale.h>
#include "tests/Gradients.h"
#include "tests/ChessBoard.h"
<<<<<<< HEAD
=======
#include "tests/MovingSquare.h"
>>>>>>> 803c28f (add Response Time test)

STATIC CONST UMT_STATE_ACTIONS mStateActions[UMT_STATE_END] = {
  { MainMenuInit, MainMenuDoit, MainMenuTip, MainMenuChangeParam, MainMenuChangeValue },
  { SettingsMenuInit, SettingsMenuDoit, SettingsMenuTip, SettingsChangeParam, SettingsMenuChangeValue },
  { SolidColorsTestInit, SolidColorsTestDoit, SolidColorsTestTip, SolidColorsTestChangeParam, SolidColorsTestChangeValue },
  { GrayscaleTestInit, GrayscaleTestDoit, GrayscaleTestTip, GrayscaleTestChangeParam, GrayscaleTestChangeValue },
  { GradientsTestInit, GradientsTestDoit, GradientsTestTip, GradientsTestChangeParam, GradientsTestChangeValue },
<<<<<<< HEAD
  { ChessBoardTestInit, ChessBoardTestDoit, ChessBoardTestTip, ChessBoardTestChangeParam, ChessBoardTestChangeValue }
=======
  { ChessBoardTestInit, ChessBoardTestDoit, ChessBoardTestTip, ChessBoardTestChangeParam, ChessBoardTestChangeValue },
  { MovingSquareTestInit, MovingSquareTestDoit, MovingSquareTestTip, MovingSquareTestChangeParam, MovingSquareTestChangeValue }
>>>>>>> 803c28f (add Response Time test)
};

EFI_HII_HANDLE gUmtHiiHandle = NULL;

STATIC
EFI_STATUS
RegisterHiiPackage (
  IN  EFI_HANDLE      ImageHandle,
  OUT EFI_HII_HANDLE  *HiiHandle
  )
{
  EFI_STATUS                  Status;
  EFI_HII_PACKAGE_LIST_HEADER *PackageList;

  // Retrieve HII package list from ImageHandle
  Status = gBS->OpenProtocol (
                  ImageHandle,
                  &gEfiHiiPackageListProtocolGuid,
                  (VOID **)&PackageList,
                  ImageHandle,
                  NULL,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to open EFI_HII_PACKAGE_LIST_PROTOCOL\n"));
    return Status;
  }

  Status = gHiiDatabase->NewPackageList(gHiiDatabase, PackageList, NULL, HiiHandle);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to add HII Package list to HII database: %r\n", Status));
    return Status;
  }

  return Status;
}

STATIC
EFI_STATUS
UnregisterHiiPackage (
  IN  EFI_HII_HANDLE  HiiHandle
  )
{
  EFI_STATUS  Status;

  Status = gHiiDatabase->RemovePackageList(gHiiDatabase, HiiHandle);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_WARN, "Failed to remove HII Package list from HII database: %r\n", Status));
  }

  return Status;
}

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
VOID
ChangeCtxState (
  IN  UMT_CONTEXT     *Ctx,
  IN  enum UMT_STATE  State
  )
{
  if (State >= UMT_STATE_END)
    return;

  Ctx->State   = State;
  Ctx->ShowTip = FALSE;
  Ctx->Actions = &mStateActions[State];
  Ctx->Actions->Init (Ctx);
}

/**
  Handle user key input. Will route it to other components handle function

  @retval VOID
**/
STATIC
VOID
HandleInput (
  IN UMT_CONTEXT *Ctx
  )
{
  EFI_KEY_DATA  KeyData;
  EFI_STATUS    Status;

  // TODO: check mouse activity

  // Check keyboard activity
  Status = gBS->CheckEvent (Ctx->Graphics->TextInputEx->WaitForKeyEx);
  if (EFI_ERROR (Status))
    return;

  Status = Ctx->Graphics->TextInputEx->ReadKeyStrokeEx (Ctx->Graphics->TextInputEx, &KeyData);
  if (EFI_ERROR (Status))
    return;

  switch (KeyData.Key.ScanCode) {
    case SCAN_UP:
      Ctx->Actions->ChangeParam (Ctx, -1);
      break;

    case SCAN_DOWN:
      Ctx->Actions->ChangeParam (Ctx, +1);
      break;

    case SCAN_RIGHT:
      Ctx->Actions->ChangeValue (Ctx, +1);
      break;

    case SCAN_LEFT:
      Ctx->Actions->ChangeValue (Ctx, -1);
      break;

    case SCAN_F1...SCAN_F11:
      ChangeCtxState (Ctx, KeyData.Key.ScanCode - SCAN_F1 + UMT_STATE_SOLID_COLORS_TEST);
      break;

    case SCAN_F12:
      ChangeCtxState (Ctx, UMT_STATE_SETTINGS);
      break;

    case SCAN_ESC:
      if (Ctx->State != UMT_STATE_MAIN_MENU) {
        ChangeCtxState (Ctx, UMT_STATE_MAIN_MENU);
        break;
      }

      Ctx->Running = FALSE;
      break;

    default:
      break;
  }

  if (KeyData.Key.ScanCode == SCAN_NULL && KeyData.Key.UnicodeChar == L' ') {
    Ctx->ShowTip = !Ctx->ShowTip;
    Ctx->Actions->Tip (Ctx);
  }
}

STATIC
EFI_STATUS
Run (
  IN GRAPHICS_CONTEXT *Graphics
  )
{
  UMT_CONTEXT Ctx;

  Ctx.Running   = TRUE;
  Ctx.Graphics  = Graphics;
  ChangeCtxState (&Ctx, UMT_STATE_MAIN_MENU);

  while (Ctx.Running == TRUE)
  {
    HandleInput (&Ctx);

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

  Status = RegisterHiiPackage (ImageHandle, &gUmtHiiHandle);
  if (EFI_ERROR(Status)) {
    Print (L"Error: Failed to register HII Package list\n");
    return Status;
  }

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
  gST->ConOut->ClearScreen(gST->ConOut);

  UnregisterHiiPackage (gUmtHiiHandle);

  return Status;
}
