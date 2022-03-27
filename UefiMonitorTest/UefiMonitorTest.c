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

CONST EFI_PIXEL_BITMASK mRgbPixelMasks = {
  0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000
};

CONST EFI_PIXEL_BITMASK mBgrPixelMasks = {
  0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000
};

STATIC
VOID
PrepareGraphicsInfo (
  IN GRAPHICS_CONTEXT             *Graphics,
  IN EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop
  )
{
  CONST EFI_PIXEL_BITMASK  *BitMask;
  UINT32                   PixelWidth;
  INT8                     PixelShl[4];
  INT8                     PixelShr[4];

  ASSERT (Graphics != NULL);
  ASSERT (Gop != NULL);

  switch (Gop->Mode->Info->PixelFormat) {
    case PixelRedGreenBlueReserved8BitPerColor:
      BitMask = &mRgbPixelMasks;
      break;

    case PixelBlueGreenRedReserved8BitPerColor:
      BitMask = &mBgrPixelMasks;
      break;

    case PixelBitMask:
      BitMask = &Gop->Mode->Info->PixelInformation;
      break;

    case PixelBltOnly:
      ASSERT (FALSE);
      //return RETURN_UNSUPPORTED;

    default:
      ASSERT (FALSE);
      // return RETURN_INVALID_PARAMETER;
  }

  DEBUG ((
    DEBUG_INFO,
    "GOP information:\n"
    "Mode: %d\n"
    "Framebuffer address, size: %x, %d\n"
    "Screen width x height: %d x %d\n",
    Gop->Mode->Mode,
    Gop->Mode->FrameBufferBase, Gop->Mode->FrameBufferSize,
    Gop->Mode->Info->HorizontalResolution, Gop->Mode->Info->VerticalResolution
    ));

  ParseGraphicsPixelFormat (BitMask, &PixelWidth, PixelShl, PixelShr);

  Graphics->Gop         = Gop;
  Graphics->FrontBuffer = (UINT8 *)Gop->Mode->FrameBufferBase;
  Graphics->BufferSize  = Gop->Mode->FrameBufferSize;
  Graphics->BackBuffer  = AllocateCopyPool (Graphics->BufferSize, Graphics->FrontBuffer);
  ASSERT (Graphics->BackBuffer != NULL);
  Graphics->Width       = Gop->Mode->Info->HorizontalResolution;
  Graphics->Height      = Gop->Mode->Info->VerticalResolution;
  CopyMem (&Graphics->PixelMasks, BitMask, sizeof (*BitMask));
  CopyMem (Graphics->PixelShl, PixelShl, sizeof (PixelShl));
  CopyMem (Graphics->PixelShr, PixelShr, sizeof (PixelShr));
  Graphics->PixelWidth  = PixelWidth;
  Graphics->Pitch       = Graphics->PixelWidth * Gop->Mode->Info->PixelsPerScanLine;
}

STATIC
VOID
ForgetGraphicsInfo (
  IN GRAPHICS_CONTEXT *Graphics
  )
{
  ASSERT (Graphics != NULL);

  // Should we zero all the data structure?

  FreePool (Graphics->BackBuffer);
}

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

  

  ForgetGraphicsInfo (&Graphics);

  return EFI_SUCCESS;
}
