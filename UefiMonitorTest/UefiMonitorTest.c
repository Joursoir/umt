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

#define GET_ICOLOR(Graphics, Ucolor)                                \
    (UINT32)(                                                       \
    (((Ucolor << Graphics->PixelShl[0]) >> Graphics->PixelShr[0]) & \
     Graphics->PixelMasks.RedMask) |                                \
    (((Ucolor << Graphics->PixelShl[1]) >> Graphics->PixelShr[1]) & \
     Graphics->PixelMasks.GreenMask) |                              \
    (((Ucolor << Graphics->PixelShl[2]) >> Graphics->PixelShr[2]) & \
     Graphics->PixelMasks.BlueMask)                                 \
    )

#define PUT_PUXEL(Graphics, X, Y, Icolor) \
    Graphics->BackBuffer[X + (Y * Graphics->Pitch)] = Icolor

CONST EFI_PIXEL_BITMASK mRgbPixelMasks = {
  0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000
};

CONST EFI_PIXEL_BITMASK mBgrPixelMasks = {
  0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000
};

STATIC
VOID
ParseGraphicsPixelFormat (
  IN CONST EFI_PIXEL_BITMASK  *BitMask,
  OUT UINT32                  *PixelWidth,
  OUT INT8                    *PixelShl,
  OUT INT8                    *PixelShr
  )
{
  UINT8   Index;
  UINT32  *Masks;
  UINT32  MergedMasks;

  MergedMasks = 0;
  Masks       = (UINT32 *)BitMask;
  for (Index = 0; Index < 3; Index++) {
    ASSERT ((MergedMasks & Masks[Index]) == 0);

    PixelShl[Index] = (INT8)HighBitSet32 (Masks[Index]) - 23 + (Index * 8);
    if (PixelShl[Index] < 0) {
      PixelShr[Index] = -PixelShl[Index];
      PixelShl[Index] = 0;
    } else {
      PixelShr[Index] = 0;
    }

    DEBUG ((
      DEBUG_INFO,
      "Index %d: shl:%d shr:%d mask:%08x\n",
      Index,
      PixelShl[Index],
      PixelShr[Index],
      Masks[Index]
      ));

    MergedMasks = (UINT32)(MergedMasks | Masks[Index]);
  }

  MergedMasks = (UINT32)(MergedMasks | Masks[3]);

  ASSERT (MergedMasks != 0);
  *PixelWidth = (UINT32)((HighBitSet32 (MergedMasks) + 7) / 8);
  DEBUG ((DEBUG_INFO, "Bytes per pixel: %d\n", *PixelWidth));
}

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
  ASSERT (PixelWidth == 4);

  Graphics->Gop         = Gop;
  Graphics->FrontBuffer = (UINT32 *)Gop->Mode->FrameBufferBase;
  Graphics->BufferSize  = Gop->Mode->FrameBufferSize;
  Graphics->BackBuffer  = AllocateCopyPool (Graphics->BufferSize, Graphics->FrontBuffer);
  ASSERT (Graphics->BackBuffer != NULL);
  Graphics->Width       = Gop->Mode->Info->HorizontalResolution;
  Graphics->Height      = Gop->Mode->Info->VerticalResolution;
  CopyMem (&Graphics->PixelMasks, BitMask, sizeof (*BitMask));
  CopyMem (Graphics->PixelShl, PixelShl, sizeof (PixelShl));
  CopyMem (Graphics->PixelShr, PixelShr, sizeof (PixelShr));
  Graphics->PixelWidth  = PixelWidth;
  Graphics->Pitch       = Gop->Mode->Info->PixelsPerScanLine;
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

STATIC
VOID
DrawLine (
  IN GRAPHICS_CONTEXT   *Graphics,
  IN UINTN              X,
  IN UINTN              Y,
  IN UINTN              X2,
  IN UINTN              Y2,
  GRAPHICS_PIXEL_COLOR  *Color
  )
{
  UINT32 Ucolor;
  UINT32 dX;
  UINT32 dY;
  UINT32 Index;
  UINT32 ColorB;

  ASSERT (X >= 0 && X <= Graphics->Width);
  ASSERT (Y >= 0 && Y <= Graphics->Height);
  ASSERT (X2 >= 0 && X2 <= Graphics->Width);
  ASSERT (Y2 >= 0 && Y2 <= Graphics->Height);

  Ucolor  = *(UINT32 *)Color;
  ColorB = GET_ICOLOR(Graphics,Ucolor);
  dX=X2-X;
  dY=Y2-Y;
  if (dX>dY) {
    for (Index = X; Index <= X2; Index++) {
      PUT_PUXEL(Graphics, Index, (Y + ( (Index - X) * dY ) / dX), ColorB);
    }
  }
  else {
    for (Index = Y; Index <= Y2; Index++) {
      PUT_PUXEL(Graphics, (X + ( (Index - Y) * dX ) / dY), Index, ColorB);
    }
  }
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

  PrepareGraphicsInfo (&Graphics, Gop);

  Status = Run (&Graphics);

  ForgetGraphicsInfo (&Graphics);

  return Status;
}
