#include <Library/HiiLib.h>

#include "MainMenu.h"

#define INDENT            15
#define CHAR_PER_COLUMN   24
#define RECT_HALF_WIDTH   300
#define RECT_HALF_HEIGHT  130

VOID
MainMenuInit (
  IN UMT_CONTEXT *Ctx
  )
{
  GRAPHICS_CONTEXT  *Graphics;
  UINT32            Width;
  UINT32            Height;
  UINTN             FontHeight;
  UINTN             FontWidth;
  UINTN             LeftColX;
  UINTN             MiddleColX;
  UINTN             RightColX;
  UINTN             Y;

  Graphics   = Ctx->Graphics;
  Width      = Graphics->Width;
  Height     = Graphics->Height;
  GetFontParameters (Graphics, &FontWidth, &FontHeight);
  LeftColX   = (Width - (CHAR_PER_COLUMN * 3) * FontWidth) / 2;
  MiddleColX = LeftColX + (CHAR_PER_COLUMN * FontWidth);
  RightColX  = MiddleColX + (CHAR_PER_COLUMN * FontWidth);

  // Background
  PutRect (Graphics, 0, 0, Width, Height, &gUmtColors[UMT_COLOR_NAVY].Color);
  DrawHiiStringF (Graphics,
                  (Width - 24 * FontWidth) / 2,
                  Height / 2 - RECT_HALF_HEIGHT - (INDENT * 2),
                  &gUmtColors[UMT_COLOR_WHITE].Color,
                  STRING_TOKEN (STR_MM_TITLE), gUmtHiiHandle, UMT_VERSION);

  // Menu
  DrawRoundedRect (Graphics,
                  Width  / 2 - RECT_HALF_WIDTH,
                  Height / 2 - RECT_HALF_HEIGHT,
                  Width  / 2 + RECT_HALF_WIDTH,
                  Height / 2 + RECT_HALF_HEIGHT,
                  20,
                  &gUmtColors[UMT_COLOR_WHITE].Color
                  );
  
  Y = Height / 2 - RECT_HALF_HEIGHT + INDENT + 5;
  DrawHiiStringF (Graphics, LeftColX, Y, &gUmtColors[UMT_COLOR_BLACK].Color,
                  STRING_TOKEN (STR_MM_SUBTITLE), gUmtHiiHandle);

  Y += FontHeight + INDENT;
  DrawHiiStringF (Graphics, LeftColX, Y, &gUmtColors[UMT_COLOR_BLACK].Color,
                  STRING_TOKEN (STR_MM_MENU), gUmtHiiHandle);

  Y += (FontHeight * 4) + (INDENT * 2);
  DrawHiiStringF (Graphics, (Width - 9 * FontWidth) / 2, Y, &gUmtColors[UMT_COLOR_BLACK].Color,
                  STRING_TOKEN (STR_MM_TIP_TITLE), gUmtHiiHandle);

  Y += FontHeight + INDENT;
  DrawHiiStringF (Graphics, LeftColX, Y, &gUmtColors[UMT_COLOR_BLACK].Color,
                  STRING_TOKEN (STR_MM_TIP_FIRST), gUmtHiiHandle);
  DrawHiiStringF (Graphics, MiddleColX, Y, &gUmtColors[UMT_COLOR_BLACK].Color,
                  STRING_TOKEN (STR_MM_TIP_SECOND), gUmtHiiHandle);
  DrawHiiStringF (Graphics, RightColX, Y, &gUmtColors[UMT_COLOR_BLACK].Color,
                  STRING_TOKEN (STR_MM_TIP_THIRD), gUmtHiiHandle);

}

VOID
MainMenuDoit (
  IN UMT_CONTEXT *Ctx
  )
{

}

VOID
MainMenuTip (
  IN UMT_CONTEXT *Ctx
  )
{
  
}

VOID
MainMenuChangeParam (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        ParamStep
  )
{
  
}

VOID
MainMenuChangeValue (
  IN  UMT_CONTEXT *Ctx,
  IN  INT8        ValueStep
  )
{
  
}
