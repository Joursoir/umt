#ifndef UEFI_MONITOR_TEST_H
#define UEFI_MONITOR_TEST_H


#include "Graphics.h"

#define UMT_VERSION L"0.1.0"

typedef struct _UMT_CONTEXT UMT_CONTEXT;

typedef struct {
  VOID (*Init)         (UMT_CONTEXT *Ctx);
  VOID (*Doit)         (UMT_CONTEXT *Ctx);
  VOID (*Tip)          (UMT_CONTEXT *Ctx);
  VOID (*ChangeParam)  (UMT_CONTEXT *Ctx, INT8 ParamStep);
  VOID (*ChangeValue)  (UMT_CONTEXT *Ctx, INT8 ValueStep);
} UMT_STATE_ACTIONS;

enum UMT_STATE {
  UMT_STATE_MAIN_MENU = 0,
  UMT_STATE_SETTINGS,
  UMT_STATE_SOLID_COLORS_TEST,
  UMT_STATE_GRAYSCALE_TEST,
  UMT_STATE_GRADIENTS_TEST,
  UMT_STATE_CHESSBOARD_TEST,
  UMT_STATE_COLOR_DISTANCES_TEST,
  UMT_STATE_END
};

struct _UMT_CONTEXT {
  enum UMT_STATE          State;
  BOOLEAN                 Running;
  BOOLEAN                 ShowTip;
  CONST UMT_STATE_ACTIONS *Actions;
  GRAPHICS_CONTEXT        *Graphics;
};

extern EFI_HII_HANDLE gUmtHiiHandle;

extern CONST UMT_STATE_ACTIONS gMainMenu;
extern CONST UMT_STATE_ACTIONS gSettingsMenu;
extern CONST UMT_STATE_ACTIONS gSolidColorsTest;
extern CONST UMT_STATE_ACTIONS gGrayscaleTest;
extern CONST UMT_STATE_ACTIONS gGradientsTest;
extern CONST UMT_STATE_ACTIONS gChessBoardTest;
extern CONST UMT_STATE_ACTIONS gColorDistancesTest;

#endif /* UEFI_MONITOR_TEST_H */
