#ifndef _SHELL_H_
#define _SHELL_H_

#include "../printf/printf.h"
#include "../IDT/irq/irq.h"
#include "../PS2/keyboard/keyboard.h"
#include "../kmain/constants.h"
#include "../memory/heap/heap.h"
#include "../string/string.h"
void getUserInput();
void initShell();

#endif