# Coding Style

This is a short document describing the preferred coding style for the UEFI Monitor Test. It is in many ways exactly the same as the EDKII coding style.

### 1. Indentation

Indentation are 2 spaces. Having 2-character indentations makes the code don't move too far to the right, and makes it easy to read on a 80-character terminal screen.

Never use tab characters. Set editor to insert spaces rather than a tab character. 

Correct:

```
if (TRUE) {
  Print (L"Hello, world!\n");
}
```

Incorrect:

```
if (TRUE) {
    Print (L"Hello, world!\n");
}
```

### 2. Breaking long lines and strings

The preferred limit on the length of a single line is 80 characters. If statements longer than 80 columns, then you should break it into sensible chunks

These same rules are applied to function headers with a long argument list.

### 3. Placing braces and spaces

The preferred way is to put the opening brace last on the line, and put the closing brace first:

```
if (TRUE) {
  Print (L"Hello, world!\n");
}
```

This applies to all non-function statement blocks (if, switch, for, while, do). Always use braces, even when there is only one statement:

```
for (Index = 0; Index < MaxVal; Index++) {
  FreePool (Buffer[Index]);
}
```

However, there is one special case, namely functions. The opening brace should always appear separately on the a new line:

```
VOID
SomeFunction (
  VOID
  )
{
  ...
}
```

### 4. Naming

[CamelCase](https://en.wikipedia.org/wiki/Camel_case) used for variables, functions and file names:

```
#include "FooFileName.h"
...
VOID SuperFunction (
  ...
  UINTN ALocalVariable;
  UINTN UefiVersion;
```

UPPERCASE used for types and macros:

```
#define BOOT_MANAGER_FORMSET_GUID \
  { \
  0x847bc3fe, 0xb974, 0x446d, {0x94, 0x49, 0x5a, 0xd5, 0x41, 0x2e, 0x99, 0x3b} \
  }

#define BOOT_MANAGER_FORM_ID  0x1000

#define BOOT_MANAGER_CALLBACK_DATA_SIGNATURE  SIGNATURE_32 ('B', 'M', 'C', 'B')
```

### 5. Use UEFI types

Never use C types. UEFI has macros for common data types. They are in the file `MdePkg/Include/_Arch_/ProcessorBind.h`.

Correct:

```
INTN   ALocalVariable;
UINTN  UefiVersion;
VOID   *Ptr;
```

Incorrect: 

```
int           ALocalVariable;
unsigned int  UefiVersion;
void          *Ptr;
```


## Examples

### C file

```
/**
  Brief and Detailed Descriptions.

  @param[in]      Arg1 Description of Arg1.
  @param[in]      Arg2 Description of Arg2, which is optional.
  @param[out]     Arg3 Description of Arg3.
  @param[in, out] Arg4 Description of Arg4.

  @retval EFI_SUCCESS   Description of what EFI_SUCCESS means.
  @retval !EFI_SUCCESS  Failure.

**/
EFI_STATUS
EFIAPI
FooName (
  IN     UINTN  Arg1,
  IN     UINTN  Arg2 OPTIONAL,
     OUT UINTN  *Arg3,
  IN OUT UINTN  *Arg4
  )
{
  UINTN Local;
  UINTN AnotherLocal;

  ...

  for (Local = 0; Local < 5; Local++) {
    if (Local == 2) {
      Print (L"Local: %d (yes! 2)\n", Local);
    } else {
      Print (L"Local: %d\n", Local);
    }
  }

  ...
}
```

### H file

```
#define FOO_MACRO(a, b) ((a) * (b))

#define FOO_CONSTANT  0xcafe

/**
  Brief and Detailed Descriptions.

  @param[in]      Arg1 Description of Arg1.
  @param[in]      Arg2 Description of Arg2, which is optional.
  @param[out]     Arg3 Description of Arg3.
  @param[in, out] Arg4 Description of Arg4.

  @retval EFI_SUCCESS   Description of what EFI_SUCCESS means.
  @retval !EFI_SUCCESS  Failure.

**/
EFI_STATUS
EFIAPI
FooName (
  IN     UINTN  Arg1,
  IN     UINTN  Arg2 OPTIONAL,
     OUT UINTN  *Arg3,
  IN OUT UINTN  *Arg4
  );

...

#endif
```
