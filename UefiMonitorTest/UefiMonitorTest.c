#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

#include <Protocol/GraphicsOutput.h>

#include "UefiMonitorTest.h"

STATIC
VOID
PrepareGraphicsInfo (
  IN GRAPHICS_CONTEXT             *Graphics,
  IN EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop
  )
{
  ASSERT (Graphics != NULL);
  ASSERT (Gop != NULL);

  Graphics->Gop         = Gop;
  Graphics->Base        = (UINT8 *)Gop->Mode->FrameBufferBase;
  Graphics->Width       = Gop->Mode->Info->HorizontalResolution;
  Graphics->Height      = Gop->Mode->Info->VerticalResolution;
  Graphics->PixelWidth  = 4; // A pixel is 32-bits
  Graphics->Pitch       = Graphics->PixelWidth * Gop->Mode->Info->PixelsPerScanLine;

  DEBUG ((
    DEBUG_INFO,
    "GOP information:\n"
    "Mode: %d\n"
    "Support a physical frame buffer: %s\n"
    "Framebuffer address, size: %x, %d\n"
    "Screen width x height: %d x %d\n",
    Gop->Mode->Mode,
    (Gop->Mode->Info->PixelFormat == PixelBltOnly) ? L"NO" : L"YES",
    Gop->Mode->FrameBufferBase, Gop->Mode->FrameBufferSize,
    Gop->Mode->Info->HorizontalResolution, Gop->Mode->Info->VerticalResolution
    ));
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

EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_GRAPHICS_OUTPUT_PROTOCOL  *Gop;
  GRAPHICS_CONTEXT              Graphics;

  Gop = GetGraphicsOutputProtocol ();
  if (Gop == NULL) {
    Print (L"Error: Getting a Graphical Output Protocol is failed\n");
    return EFI_NOT_FOUND;
  }

  PrepareGraphicsInfo (&Graphics, Gop);

  return EFI_SUCCESS;
}
