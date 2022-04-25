#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include "UefiMonitorTest.h"

#define UMT_COLOR_BLACK   0
#define UMT_COLOR_WHITE   1
#define UMT_COLOR_RED     2
#define UMT_COLOR_LIME    3
#define UMT_COLOR_BLUE    4
#define UMT_COLOR_YELLOW  5
#define UMT_COLOR_AQUA    6
#define UMT_COLOR_MAGENTA 7
#define UMT_COLOR_SILVER  8
#define UMT_COLOR_GRAY    9
#define UMT_COLOR_MAROON  10
#define UMT_COLOR_OLIVE   11
#define UMT_COLOR_GREEN   12
#define UMT_COLOR_PURPLE  13
#define UMT_COLOR_TEAL    14
#define UMT_COLOR_NAVY    15

STATIC GRAPHICS_PIXEL_COLOR mColors[16] = {
  { 0x00, 0x00, 0x00, 0x00 },
  { 0xFF, 0xFF, 0xFF, 0x00 },
  { 0x00, 0x00, 0xFF, 0x00 },
  { 0x00, 0xFF, 0x00, 0x00 },
  { 0xFF, 0x00, 0x00, 0x00 },
  { 0x00, 0xFF, 0xFF, 0x00 },
  { 0xFF, 0xFF, 0x00, 0x00 },
  { 0xFF, 0x00, 0xFF, 0x00 },
  { 0xC0, 0xC0, 0xC0, 0x00 },
  { 0x80, 0x80, 0x80, 0x00 },
  { 0x00, 0x00, 0x80, 0x00 },
  { 0x00, 0x80, 0x80, 0x00 },
  { 0x00, 0x80, 0x00, 0x00 },
  { 0x80, 0x00, 0x80, 0x00 },
  { 0x80, 0x80, 0x00, 0x00 },
  { 0x80, 0x00, 0x00, 0x00 }
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
  BOOLEAN Running;

  Running = TRUE;

  while (Running == TRUE)
  {
    // Buffer swap:
    CopyMem (Graphics->FrontBuffer, Graphics->BackBuffer, Graphics->BufferSize);
    Running = FALSE;
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
