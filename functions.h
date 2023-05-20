#pragma once
#include "framework.h"
#include <stdbool.h>
typedef struct Color {
	int red;
	int green;
	int blue;
} Color;

typedef struct Button {
	bool push;
	POINT position;
	int color[3];
	int HighlightColor[3];
	char* name;
	struct Button* prev;
	struct Button* next;
} Button;

typedef struct Window {
	POINT position;
	int color[3];
	struct Window* prev;
	struct Window* next;
} Window;

void SystemInitialise(char* fileName, Button* newButton, Window* newWindow, HWND hWnd);
void DrawWindows(Window* windowHeader, HDC hdc);
void DrawButtons(Button* buttonHeader, HDC hdc);
void Update(int position, Button* head, HDC hdc, HWND hWnd);