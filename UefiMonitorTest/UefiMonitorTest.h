#ifndef UEFI_MONITOR_TEST_H
#define UEFI_MONITOR_TEST_H


#include "Graphics.h"

typedef struct _UMT_CONTEXT UMT_CONTEXT;

typedef struct {
  VOID (*Init)(UMT_CONTEXT *);
  VOID (*Doit)(UMT_CONTEXT *);
  VOID (*Tip)(UMT_CONTEXT *);

  VOID (*KeyRight)(UMT_CONTEXT *);
  VOID (*KeyLeft)(UMT_CONTEXT *);
} UMT_STATE_ACTIONS;

enum UMT_STATE {
  UMT_STATE_MAIN_MENU = 0,
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

#endif /* UEFI_MONITOR_TEST_H */
