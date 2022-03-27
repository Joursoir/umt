#ifndef UEFI_MONITOR_TEST_H
#define UEFI_MONITOR_TEST_H

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
  UINT8                                   *FrontBuffer;
  ///
  /// Pointer to allocated memory. Secondary linear buffer.
  ///
  UINT8                                   *BackBuffer;
  ///
  /// Amount of frame buffer needed to support the active mode.
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
} GRAPHICS_CONTEXT;

#endif /* UEFI_MONITOR_TEST_H */
