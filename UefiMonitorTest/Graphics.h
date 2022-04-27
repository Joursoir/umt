#ifndef UMT_GRAPHICS_H
#define UMT_GRAPHICS_H

#include <Uefi.h>

#include <Protocol/GraphicsOutput.h>
#include <Protocol/SimpleTextIn.h>
#include <Protocol/SimplePointer.h>

#define PUT_PUXEL(Graphics, X, Y, Icolor) \
    Graphics->BackBuffer[X + (Y * Graphics->Pitch)] = Icolor

typedef struct {
  UINT8 Blue;
  UINT8 Green;
  UINT8 Red;
  UINT8 Reserved;
} GRAPHICS_PIXEL_COLOR;

typedef struct {
  ///
  /// The EFI_GRAPHICS_OUTPUT_PROTOCOL instance.
  ///
  EFI_GRAPHICS_OUTPUT_PROTOCOL            *Gop;
  ///
  /// Base address of graphics linear frame buffer.
  /// Starts from the upper left pixel.
  ///
  UINT32                                  *FrontBuffer;
  ///
  /// Pointer to allocated memory. Secondary linear buffer.
  ///
  UINT32                                  *BackBuffer;
  ///
  /// Amount of frame buffer needed to support the active mode in bytes.
  ///
  UINTN                                   BufferSize;
  ///
  /// The size of video screen in pixels in the X dimension.
  ///
  UINT32                                  Width;
  ///
  /// The size of video screen in pixels in the Y dimension.
  ///
  UINT32                                  Height;
  ///
  /// Bit-mask defines what bits are used for different colors.
  ///
  EFI_PIXEL_BITMASK                       PixelMasks;
  ///
  /// Amount of bits to shift left.
  /// R-G-B-Rsvd
  ///
  INT8                                    PixelShl[4];
  ///
  /// Amount of bits to shift right.
  /// R-G-B-Rsvd
  ///
  INT8                                    PixelShr[4];
  ///
  /// The size of pixel color in bytes.
  ///
  UINT32                                  PixelWidth;
  ///
  /// Amount of bytes you should skip to go one pixel down.
  ///
  UINT32                                  Pitch;
  //
  // The EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL instance.
  //
  EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL       *TextInputEx;
  //
  // A mouse support.
  //
  BOOLEAN                                 MouseSupported;
  //
  // The MouseInterface instance.
  //
  EFI_SIMPLE_POINTER_PROTOCOL             *MouseInterface;
} GRAPHICS_CONTEXT;

enum UMT_COLORS {
  UMT_COLOR_BLACK = 0,
  UMT_COLOR_WHITE,
  UMT_COLOR_RED,
  UMT_COLOR_LIME,
  UMT_COLOR_BLUE,
  UMT_COLOR_YELLOW,
  UMT_COLOR_AQUA,
  UMT_COLOR_MAGENTA,
  UMT_COLOR_SILVER,
  UMT_COLOR_GRAY,
  UMT_COLOR_MAROON,
  UMT_COLOR_OLIVE,
  UMT_COLOR_GREEN,
  UMT_COLOR_PURPLE,
  UMT_COLOR_TEAL,
  UMT_COLOR_NAVY,
  UMT_COLOR_END
};

extern CONST GRAPHICS_PIXEL_COLOR gUmtColors[];

EFI_STATUS
PrepareGraphicsInfo (
  IN GRAPHICS_CONTEXT             *Graphics,
  IN EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop
  );

VOID
ForgetGraphicsInfo (
  IN GRAPHICS_CONTEXT *Graphics
  );

/**
  Draw a line using Bresenham's algorithm

  @retval  VOID
**/
VOID
DrawLine (
  IN GRAPHICS_CONTEXT           *Graphics,
  IN UINTN                      X0,
  IN UINTN                      Y0,
  IN UINTN                      X1,
  IN UINTN                      Y1,
  IN CONST GRAPHICS_PIXEL_COLOR *Color
  );

VOID
PutRect (
  IN GRAPHICS_CONTEXT           *Graphics,
  IN UINTN                      X0,
  IN UINTN                      Y0,
  IN UINTN                      X1,
  IN UINTN                      Y1,
  IN CONST GRAPHICS_PIXEL_COLOR *Color
  );

VOID
DrawRectWithBorder (
  IN GRAPHICS_CONTEXT           *Graphics,
  IN UINTN                      X0,
  IN UINTN                      Y0,
  IN UINTN                      X1,
  IN UINTN                      Y1,
  IN UINTN                      BorderSize,
  IN CONST GRAPHICS_PIXEL_COLOR *MainColor,
  IN CONST GRAPHICS_PIXEL_COLOR *BorderColor
  );

VOID
DrawRoundedRect (
  IN GRAPHICS_CONTEXT           *Graphics,
  IN UINTN                      X0,
  IN UINTN                      Y0,
  IN UINTN                      X1,
  IN UINTN                      Y1,
  IN UINTN                      CornerRadius,
  IN CONST GRAPHICS_PIXEL_COLOR *Color
  );

VOID
DrawCircle (
  IN GRAPHICS_CONTEXT           *Graphics,
  IN UINTN                      X0,
  IN UINTN                      Y0,
  IN UINTN                      R,
  IN CONST GRAPHICS_PIXEL_COLOR *Color
  );

/**
  Draws a character to the screen

  @retval  VOID
**/
VOID
DrawChar (
  IN GRAPHICS_CONTEXT *Graphics,
  IN UINTN            X,
  IN UINTN            Y,
  IN UINT32           Icolor,
  IN CHAR16           Char
  );

/**
  Draws output based on a null-terminated Unicode format string
  and a arguments list to the screen

  @retval The number of Unicode characters drawn
**/
UINTN
EFIAPI
DrawStringVF (
  IN GRAPHICS_CONTEXT           *Graphics,
  IN UINTN                      X,
  IN UINTN                      Y,
  IN CONST GRAPHICS_PIXEL_COLOR *Color,
  IN CONST CHAR16               *FormatString,
  IN VA_LIST                    Marker
  );

UINTN
EFIAPI
DrawStringF (
  IN GRAPHICS_CONTEXT           *Graphics,
  IN UINTN                      X,
  IN UINTN                      Y,
  IN CONST GRAPHICS_PIXEL_COLOR *Color,
  IN CONST CHAR16               *FormatString,
  ...
  );

#endif /* UMT_GRAPHICS_H */
