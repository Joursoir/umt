#ifndef UEFI_MONITOR_TEST_H
#define UEFI_MONITOR_TEST_H

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
  /// The size of pixel color in bytes.
  ///
  UINT32                                  PixelWidth;
  ///
  /// Amount of bytes you should skip to go one pixel down.
  ///
  UINT32                                  Pitch;
} GRAPHICS_CONTEXT;

#endif /* UEFI_MONITOR_TEST_H */
