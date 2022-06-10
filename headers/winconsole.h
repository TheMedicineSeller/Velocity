#pragma once

/* ----------------------- A CONSOLE INPUT/OUTPUT BUFFER HANDLING LIBRARY FOR WINDOWS -----------------------*/

#include <Windows.h>
#include <stdint.h>

#define startCoord {0, 0}

/*--------WARNING: THIS CONSTANT MIGHT BE SYSTEM MONITOR SPECIFIC*/
#define MAX_SCREEN_HEIGHT 50
#define EMPTIER " "

/*---------------CONSOLE OUTPUT COLOUR SETTERS---------------*/
inline void SetBgColorGreen (HANDLE opHandle) {
	SetConsoleTextAttribute(opHandle, 0 | BACKGROUND_GREEN);
}
inline void SetBgColorRed (HANDLE opHandle) {
    SetConsoleTextAttribute(opHandle, 0 | BACKGROUND_RED);
}
inline void setBgColorBlack (HANDLE opHandle) {
    SetConsoleTextAttribute(opHandle, 0 | 0x0007);
}

inline void SetColorBlue (HANDLE opHandle) {
    SetConsoleTextAttribute(opHandle, 0 | 0x0001);
}
inline void SetColorAqua (HANDLE opHandle) {
    SetConsoleTextAttribute(opHandle, 0 | 0x0003);
}
inline void SetColorPurple (HANDLE opHandle) {
    SetConsoleTextAttribute(opHandle, 0 | 0x0005);
}
inline void SetColorGold (HANDLE opHandle) {
    SetConsoleTextAttribute(opHandle, 0 | 0x0006);
}
inline void SetColorWhite (HANDLE opHandle) {
    SetConsoleTextAttribute(opHandle, 0 | 0x0007);
}
inline void SetColorGrey (HANDLE opHandle) {
    SetConsoleTextAttribute(opHandle, 0 | 0x0008);
}

/* ---------------CONSOLE POSITIONING / CLEARING FUNCTIONS---------------*/
COORD getCursorPosition (HANDLE opHandle) {
    CONSOLE_SCREEN_BUFFER_INFO screenbuffer;
    GetConsoleScreenBufferInfo(opHandle, &screenbuffer);
    return screenbuffer.dwCursorPosition;
}

uint8_t getScreenWidth (HANDLE opHandle) {
	CONSOLE_SCREEN_BUFFER_INFO screenbuffer;
	GetConsoleScreenBufferInfo(opHandle, &screenbuffer);
	return screenbuffer.srWindow.Right - screenbuffer.srWindow.Left + 1;
}
/*
uint8_t getScreenHeight (HANDLE opHandle) {
	CONSOLE_SCREEN_BUFFER_INFO screenbuffer;
	GetConsoleScreenBufferInfo(opHandle, &screenbuffer);
	return screenbuffer.srWindow.Bottom - screenbuffer.srWindow.Top + 1;
}*/

/*A Windows console api implementation of gotoxy() from conio library */
void gotoxy(HANDLE opHandle, int x, int y) {
	COORD coord;
	coord.X = x; coord.Y = y;
	SetConsoleCursorPosition(opHandle, coord);
}

/*A Windows console api implementation of clrscr() from conio library */
void clrscr(HANDLE opHandle) {
	DWORD dummy;
	CONSOLE_SCREEN_BUFFER_INFO screenBuffer;
	setBgColorBlack(opHandle);
	FillConsoleOutputCharacter(opHandle,
                               ' ', 
							   MAX_SCREEN_HEIGHT * getScreenWidth(opHandle),
							   startCoord,
							   &dummy);
	gotoxy(opHandle, 0, 0);
}

/*A more versatile (but slower) screen flusher that also removes background colours from the screen*/
void clearBG (HANDLE opHandle) {
	DWORD dummy;
	setBgColorBlack(opHandle);
	gotoxy(opHandle, 0, 0);
	for (int screenptr = 0; screenptr < MAX_SCREEN_HEIGHT * getScreenWidth(opHandle); screenptr ++)
		WriteFile(opHandle,
				  EMPTIER, 1, 
				  &dummy, NULL);
	gotoxy(opHandle, 0, 0);
}

/*A Windows console api implementation of getch() from conio library */
TCHAR getch(HANDLE ipHandle) {
  	DWORD con_mode, byteswritten;
  	GetConsoleMode(ipHandle, &con_mode);
  	SetConsoleMode(ipHandle, con_mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT));
  	TCHAR ch = 0;
  	ReadConsole(ipHandle, 
	  			&ch, 1, 
				&byteswritten, NULL);
  	SetConsoleMode(ipHandle, con_mode);
  	return ch;
}