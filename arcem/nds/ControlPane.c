/* Redraw and other services for the control pane */
/* (c) David Alan Gilbert 1995 - see Readme file for copying info */


#include "armdefs.h"
#include "archio.h"
#include "armarc.h"
#include "ControlPane.h"
#include "../arch/keyboard.h"

#include <stdarg.h>
#include <stdio.h>

#include <nds.h>

bool hasKeyboard = false;

void ControlPane_Init(ARMul_State *state)
{
	videoSetModeSub(MODE_0_2D | DISPLAY_BG0_ACTIVE);
	vramSetBankH(VRAM_H_SUB_BG);

#ifndef NDEBUG
	scanKeys();

	int held = keysHeld();
	if (held & KEY_SELECT) {
		consoleInit(NULL, 0, BgType_Text4bpp, BgSize_T_256x256, 15, 0, false, true);
	} else
#endif
	{
		Keyboard *keyboard = keyboardInit(NULL, 0, BgType_Text4bpp, BgSize_T_256x512, 15, 0, false, true);
		keyboard->OnKeyPressed = OnKeyPressed;
		keyboard->OnKeyReleased = OnKeyReleased;
		keyboardShow();
		hasKeyboard = true;
	}
}

void ControlPane_Error(int code,const char *fmt,...)
{
	va_list args;
	va_start(args,fmt);

	/* Log it */
	vprintf(fmt,args);

	while(1) {
		swiWaitForVBlank();
		scanKeys();
		if (keysDown() & KEY_START)
			break;
	}

	/* Quit */
	exit(code);
}
