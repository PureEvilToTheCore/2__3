#define _CRT_SECURE_NO_WARNINGS
#include "functions.h"
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "windowsx.h"

#define WINDOW_WIDTH 200
#define BUTTON_WIDTH 50

Button* newButton = NULL;
Window* newWindow = NULL;
Button* buttonHeader = NULL;
Window* windowHeader = NULL;
HBRUSH hbrush;
HDC curHDC;
int buttonCount = 0;


void DrawRect(HDC hdc,HWND hWnd , POINT position, int color[3], int width, int height) {
	HBRUSH hbrush = CreateSolidBrush(RGB(color[0], color[1], color[2]));
	HPEN pen = CreatePen(PS_SOLID, 0, RGB(color[0], color[1], color[2]));
	SelectObject(hdc, hbrush);
	SelectObject(hdc, pen);
	Rectangle(hdc, position.x, position.y, position.x + width, position.y + height);
	DeleteObject(hbrush);
}

void DrawWindows(Window* windowHeader, HDC hdc, HWND hWnd) {
	Window* currentWindow = windowHeader;
	while (currentWindow != NULL) {
		DrawRect(hdc, hWnd, currentWindow->position, currentWindow->color, WINDOW_WIDTH, WINDOW_WIDTH);
		currentWindow = currentWindow->next;
	}
}

void DrawButtons(Button* buttonHeader, HDC hdc, HWND hWnd) {
	Button* currentButton = buttonHeader;
	while (currentButton != NULL) {
		DrawRect(hdc, hWnd, currentButton->position, currentButton->color, BUTTON_WIDTH, BUTTON_WIDTH);
		currentButton = currentButton->next;
	}
}

void DraWButt(HDC hdc, Button* header)
{
	COLORREF color = RGB(header->color[0], header->color[1], header->color[2]);
	COLORREF colorH = RGB(header->HighlightColor[0], header->HighlightColor[1], header->HighlightColor[2]);
	HPEN pen = CreatePen(PS_SOLID, 10, colorH);
	HBRUSH brush = CreateSolidBrush(color);
	SelectObject(hdc, pen);
	SelectObject(hdc, brush);
	Rectangle(hdc, header->position.x, header->position.y, header->position.x + BUTTON_WIDTH, header->position.y + BUTTON_WIDTH);
	DeleteObject(pen);
}
void Update(int position, HDC hdc, HWND hWnd) {
	DrawWindows(windowHeader, hdc, hWnd);
	DrawButtons(buttonHeader, hdc, hWnd);
	Button* curHeader = buttonHeader;
	if (position == 1 && buttonHeader->push) {
		DraWButt(hdc, buttonHeader);
		return;
	}
	while (curHeader != NULL) {
		if (curHeader->push == true)
		{
			if (position == 1 && curHeader->prev != NULL) {
				DraWButt(hdc, curHeader->prev);
				curHeader->prev->push = true;
				curHeader->push = false;
				return;
			}
			else if (position == -1 && curHeader->next != NULL) {
				DraWButt(hdc, curHeader->next) ;
				curHeader->next->push = true;
				curHeader->push = false;
				return;
			}
			else if (position == 0) {
				OutputDebugStringA(curHeader->name);
				OutputDebugStringA("\n");
			}
		}
		curHeader = curHeader->next;
	}
}

void SystemInitialise(char* fileName, HDC hdc, HWND hWnd ) {
	FILE* file;
	char str[256];
	file = fopen(fileName, "r");
	int isButton = 0;
	while (fgets(str, 256, file)) {
		if (strcmp(str, "\tButtonBegin\n") == 0) {
			isButton = 1;
			newButton = malloc(sizeof(Button));
			newButton->name = malloc(256 * sizeof(char));
			newButton->prev = NULL;
			newButton->next = buttonHeader;
			if (buttonHeader != NULL) {
				buttonHeader->prev = newButton;
			}
			buttonHeader = newButton;
		}
		else if (strcmp(str, "\tWindowBegin\n") == 0) {
			isButton = 0;
			newWindow = malloc(sizeof(Window));
			newWindow->prev = NULL;
			newWindow->next = windowHeader;
			if (windowHeader != NULL) {
				windowHeader->prev = newWindow;
			}
			windowHeader = newWindow;
		}
		else if (isButton == 1 && newButton != NULL) {
			if (sscanf(str, "\t\tPosition=(%d,%d)\n", &newButton->position.x, &newButton->position.y) == 2) {
			}
			else if (sscanf(str, "\t\tColor=(%d,%d,%d)\n", &newButton->color[0], &newButton->color[1], &newButton->color[2]) == 3) {
				// Color line
			}
			else if (sscanf(str, "\t\HiglightColor=(%d,%d,%d)\n", &newButton->HighlightColor[0], &newButton->HighlightColor[1], &newButton->HighlightColor[2]) == 3) {
				// HighlightColor line
			}
			else if (sscanf(str, "\t\tName=%s\n", newButton->name) == 1) {
				// Name line
			}
		}
		else if (isButton == 0 && newWindow != NULL) {
			if (sscanf(str, "\t\tPosition=(%d,%d)\n", &newWindow->position.x, &newWindow->position.y) == 2) {
				// Position line
			}
			else if (sscanf(str, "\t\tColor=(%d,%d,%d)\n", &newWindow->color[0], &newWindow->color[1], &newWindow->color[2]) == 3) {
				// Color line
			}
		}
	}
	buttonHeader->push = true;
	fclose(file);
	DrawWindows(windowHeader, hdc, hWnd);
	DrawButtons(buttonHeader, hdc, hWnd);
	Update(1, curHDC, hWnd);
}

void SystemShutdown() {
	Button* currentButton = buttonHeader;
	while (currentButton != NULL) {
		Button* nextButton = currentButton->next;
		free(currentButton->color);
		free(currentButton->HighlightColor);
		free(currentButton->name);
		free(currentButton);
		currentButton = nextButton;
	}
	Window* currentWindow = windowHeader;
	while (currentWindow != NULL) {
		Window* nextWindow = currentWindow->next;
		free(currentWindow->color);
		free(currentWindow);
		currentWindow = nextWindow;
	}
}
void SystemOpen(HDC hdc) {
	curHDC = hdc;
}
void SystemClose(HDC hdc) {
	free(hbrush);
	curHDC = NULL;
}
