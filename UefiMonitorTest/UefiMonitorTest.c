#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

#include <Protocol/GraphicsOutput.h>
#include <Protocol/SimplePointer.h>

#include "UefiMonitorTest.h"
#include "fonts/System-8x16.h"

#define UMT_MAXPRINT_BUFFER_SIZE 6400 // TODO: Use Pcd

#define SWAP(A, B, C)                                               \
    C = A;                                                          \
    A = B;                                                          \
    B = C

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
EFI_STATUS
PrepareGraphicsInfo (
  IN GRAPHICS_CONTEXT             *Graphics,
  IN EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop
  )
{
  EFI_STATUS              Status;
  CONST EFI_PIXEL_BITMASK *BitMask;
  UINT32                  PixelWidth;
  INT8                    PixelShl[4];
  INT8                    PixelShr[4];
  EFI_HANDLE              *HandleBuffer;
  UINTN                   HandleCount;
  UINTN                   Index;

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
      return RETURN_UNSUPPORTED;

    default:
      ASSERT (FALSE);
      return RETURN_INVALID_PARAMETER;
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
  if (Graphics->BackBuffer == NULL)
    return EFI_OUT_OF_RESOURCES;
  Graphics->Width       = Gop->Mode->Info->HorizontalResolution;
  Graphics->Height      = Gop->Mode->Info->VerticalResolution;
  CopyMem (&Graphics->PixelMasks, BitMask, sizeof (*BitMask));
  CopyMem (Graphics->PixelShl, PixelShl, sizeof (PixelShl));
  CopyMem (Graphics->PixelShr, PixelShr, sizeof (PixelShr));
  Graphics->PixelWidth  = PixelWidth;
  Graphics->Pitch       = Gop->Mode->Info->PixelsPerScanLine;

  // Find TextInEx in System Table ConsoleInHandle
  Status = gBS->HandleProtocol (
                  gST->ConsoleInHandle,
                  &gEfiSimpleTextInputExProtocolGuid,
                  (VOID **)&Graphics->TextInputEx
                  );
  if (EFI_ERROR (Status)) {
    FreePool (Graphics->BackBuffer);
    return Status;
  }

  // Find mouse in System Table ConsoleInHandle
  Status = gBS->HandleProtocol (
                  gST->ConsoleInHandle,
                  &gEfiSimplePointerProtocolGuid,
                  (VOID **)&Graphics->MouseInterface
                  );
  if (EFI_ERROR (Status))
  {
    HandleBuffer               = NULL;
    Graphics->MouseInterface   = NULL;
    Status                     = gBS->LocateHandleBuffer (
                                        ByProtocol,
                                        &gEfiSimplePointerProtocolGuid,
                                        NULL,
                                        &HandleCount,
                                        &HandleBuffer
                                        );
    if (!EFI_ERROR (Status)) {
      for (Index = 0; Index < HandleCount; Index++) {
        Status = gBS->HandleProtocol (
                        HandleBuffer[Index],
                        &gEfiSimplePointerProtocolGuid,
                        (VOID **)&Graphics->MouseInterface
                        );
        if (!EFI_ERROR (Status)) {
          break;
        }
      }
    }

    if (HandleBuffer != NULL) {
      FreePool (HandleBuffer);
    }
  }

  if (!EFI_ERROR (Status) && (Graphics->MouseInterface != NULL)) {
    Graphics->MouseSupported    = TRUE;
  }

  DEBUG ((DEBUG_INFO, "Mouse support: %s\n\n", Graphics->MouseSupported ? L"Yes" : L"No"));
  return EFI_SUCCESS;
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

/**
  Draw a line using Bresenham's algorithm

  @retval  VOID
**/
STATIC
VOID
DrawLine (
  IN GRAPHICS_CONTEXT   *Graphics,
  IN UINTN              X0,
  IN UINTN              Y0,
  IN UINTN              X1,
  IN UINTN              Y1,
  GRAPHICS_PIXEL_COLOR  *Color
  )
{
  INTN    DeltaX;
  INTN    DeltaY;
  INTN    AbsDeltaX;
  INTN    AbsDeltaY;
  INTN    Correction;
  INT8    Direction;
  UINTN   X, Y, Z;
  BOOLEAN Reverse;
  UINT32  Ucolor;
  UINT32  Icolor;

  ASSERT (X0 >= 0 && X0 < Graphics->Width);
  ASSERT (Y0 >= 0 && Y0 < Graphics->Height);
  ASSERT (X1 >= 0 && X1 < Graphics->Width);
  ASSERT (Y1 >= 0 && Y1 < Graphics->Height);

  DeltaX = X1 - X0;
  DeltaY = Y1 - Y0;
  AbsDeltaX = ABS (DeltaX);
  AbsDeltaY = ABS (DeltaY);

  Reverse = FALSE;
  if (AbsDeltaX < AbsDeltaY) {
    SWAP (X0, Y0, Z);
    SWAP (X1, Y1, Z);
    Reverse = TRUE;
  }

  if (X0 > X1) {
    SWAP (X0, X1, Z);
    SWAP (Y0, Y1, Z);
  }

  DeltaX = X1 - X0;
  DeltaY = Y1 - Y0;
  AbsDeltaY = ABS (DeltaY) * 2;
  Correction = 0;
  Direction = (Y1 > Y0) ? 1 : -1;
  Ucolor = *(UINT32 *)Color;
  Icolor = GET_ICOLOR (Graphics, Ucolor);

  Y = Y0;
  for (X = X0; X <= X1; X++) {
    if (Reverse) {
      PUT_PUXEL (Graphics, Y, X, Icolor);
    } else {
      PUT_PUXEL (Graphics, X, Y, Icolor);
    }
    Correction += AbsDeltaY;

    if (Correction > DeltaX) {
      Y += Direction;
      Correction -= DeltaX * 2;
    }
  }
}

STATIC
VOID
PutRect (
  IN GRAPHICS_CONTEXT   *Graphics,
  IN UINTN              X0,
  IN UINTN              Y0,
  IN UINTN              X1,
  IN UINTN              Y1,
  GRAPHICS_PIXEL_COLOR  *Color
  )
{
  UINT32 *Buffer;
  UINT32 Ucolor;
  UINT32 Icolor;
  UINT32 I, J;

  ASSERT (X0 >= 0 && X0 <= Graphics->Width);
  ASSERT (Y0 >= 0 && Y0 <= Graphics->Height);
  ASSERT (X1 >= 0 && X1 <= Graphics->Width && X1 >= X0);
  ASSERT (Y1 >= 0 && Y1 <= Graphics->Height && Y1 >= Y0);

  Buffer = Graphics->BackBuffer + Y0 * Graphics->Pitch;
  Ucolor = *(UINT32 *)Color;
  Icolor = GET_ICOLOR(Graphics, Ucolor);

  for (J = Y0; J < Y1; J++) {
    for (I = X0; I < X1; I++) {
      Buffer[I] = Icolor;
    }
    Buffer += Graphics->Pitch;
  }
}

STATIC
VOID
DrawCircle (
  IN GRAPHICS_CONTEXT   *Graphics,
  IN UINTN              X0,
  IN UINTN              Y0,
  IN UINTN              R,
  GRAPHICS_PIXEL_COLOR  *Color
)
{
  UINT32 *Buffer;
  UINT32 Ucolor;
  UINT32 Icolor;
  UINT32 I, J;

  ASSERT (X0 >= 0 && X0 < Graphics->Width);
  ASSERT (Y0 >= 0 && Y0 < Graphics->Height);
  ASSERT (R > 0);
  ASSERT ((X0 + R) < Graphics->Width && X0 >= R);
  ASSERT ((Y0 + R) < Graphics->Height && Y0 >= R);

  Ucolor = *(UINT32 *)Color;
  Icolor = GET_ICOLOR(Graphics, Ucolor);
  Buffer = Graphics->BackBuffer + (Y0 - R) * Graphics->Pitch;

  for (J = (Y0 - R); J <= (Y0 + R); J++) {
    for (I = (X0 - R); I <= (X0 + R); I++) {
      if ((J - Y0) * (J - Y0) + (I - X0) * (I - X0) <= (R * R)) {
        Buffer[I] = Icolor;
      }
    }
    Buffer += Graphics->Pitch;
  }
}

/**
  Draws a character to the screen

  @retval  VOID
**/
STATIC
VOID
DrawChar (
  IN GRAPHICS_CONTEXT *Graphics,
  IN UINTN            X,
  IN UINTN            Y,
  IN UINT32           Icolor,
  IN CHAR16           Char
  )
{
  UINTN Index;
  UINTN l, c;

  Index = Char * (SYSTEM8X16_FONT_WIDTH * SYSTEM8X16_FONT_HEIGHT) - (SYSTEM8X16_FONT_WIDTH * SYSTEM8X16_FONT_HEIGHT);
  for (l = 0; l < SYSTEM8X16_FONT_HEIGHT; l++) {
    for (c = 0; c < SYSTEM8X16_FONT_WIDTH; c++) {
      if (gFontSystem8x16[Index] == 1) {
          PUT_PUXEL (Graphics, (X + c), (Y + l), Icolor);
      }
      Index++;
    }
  }
}

/**
  Draws output based on a null-terminated Unicode format string
  and a arguments list to the screen

  @retval The number of Unicode characters drawn
**/
STATIC
UINTN
EFIAPI
DrawStringVF (
  IN GRAPHICS_CONTEXT     *Graphics,
  IN UINTN                X,
  IN UINTN                Y,
  IN GRAPHICS_PIXEL_COLOR *Color,
  IN CONST CHAR16         *FormatString,
  IN VA_LIST              Marker
  )
{
  UINTN   WalkerSize;
  CHAR16  *FormatWalker;
  UINTN   Length;
  UINTN   Index;
  UINTN   OldX;
  UINT32  Ucolor;
  UINT32  Icolor;

  ASSERT(FormatString != NULL);

  WalkerSize = (UMT_MAXPRINT_BUFFER_SIZE + 1) * sizeof(CHAR16);
  FormatWalker = AllocateZeroPool(WalkerSize);
  if (FormatWalker == NULL)
    return 0;

  Length = UnicodeVSPrint(FormatWalker, WalkerSize, FormatString, Marker);
  Index  = 0;
  OldX   = X;
  Ucolor = *(UINT32 *)Color;
  Icolor = GET_ICOLOR (Graphics, Ucolor);

  while (FormatWalker[Index] != '\0' && Index < Length) {
    switch (FormatWalker[Index]) {
    case L'\n':
      X = OldX;
      Y += SYSTEM8X16_FONT_HEIGHT;
      break;
    case L'\t':
      X += SYSTEM8X16_FONT_WIDTH * 4;
      break;
    default:
      DrawChar (Graphics, X, Y, Icolor, FormatWalker[Index]);
      X += SYSTEM8X16_FONT_WIDTH;
      if (X > Graphics->Width)
        break;
    }
    Index++;
  }

  FreePool(FormatWalker);
  return Index;
}

/**
  Draws a null-terminated formatted Unicode string to the screen

  @retval The number of Unicode characters drawn
**/
STATIC
UINTN
EFIAPI
DrawStringF (
  IN GRAPHICS_CONTEXT     *Graphics,
  IN UINTN                X,
  IN UINTN                Y,
  IN GRAPHICS_PIXEL_COLOR *Color,
  IN CONST CHAR16         *FormatString,
  ...
  )
{
  VA_LIST Marker;
  UINTN   NumberOfPrinted;

  VA_START (Marker, FormatString);
  NumberOfPrinted = DrawStringVF(Graphics, X, Y, Color, FormatString, Marker);
  VA_END (Marker);
  return NumberOfPrinted;
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
