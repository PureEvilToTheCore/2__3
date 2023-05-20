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
	SelectObject(hdc, hbrush);
	Rectangle(hdc, position.x, position.y, position.x + width, position.y + height);
	DeleteObject(hbrush);
	//InvalidateRect(hWnd, NULL, FALSE);
}

void DrawWindows(Window* windowHeader, HDC hdc, HWND hWnd) {
	while (windowHeader != NULL) {
		DrawRect(hdc, hWnd, windowHeader->position, windowHeader->color, WINDOW_WIDTH, WINDOW_WIDTH);
		windowHeader = windowHeader->next;
	}
}

void DrawButtons(Button* buttonHeader, HDC hdc, HWND hWnd) {
	Button* currentButton = buttonHeader;
	while (currentButton != NULL) {
		DrawRect(hdc, hWnd, currentButton->position, currentButton->color, BUTTON_WIDTH, BUTTON_WIDTH);
		currentButton = currentButton->next;
	}
}

void DraWButt(HDC hdc, Button* buttonHeader)
{
	COLORREF color = RGB(buttonHeader->HighlightColor[0], buttonHeader->HighlightColor[1], buttonHeader->HighlightColor[2]);
	HPEN pen = CreatePen(PS_SOLID, 10, color);
	HGDIOBJ oldPen = SelectObject(hdc, pen);
	Rectangle(hdc, buttonHeader->position.x, buttonHeader->position.y, buttonHeader->position.x + BUTTON_WIDTH, buttonHeader->position.y + BUTTON_WIDTH);
}
void Update(int position, Button* head, Window* wind, HDC hdc, HWND hWnd) {
	DrawWindows(windowHeader, hdc, hWnd);
	DrawButtons(buttonHeader, hdc, hWnd);
	while (head != NULL) {
		if (head->push == true)
		{
			if (position == 1 && head->prev != NULL) {
				//DrawRect(hdc, head->prev->position, *(head->highlightColor), BUTTON_WIDTH, BUTTON_WIDTH);
				DraWButt(hdc, head->prev);
				head->prev->push = true;
				head->push = false;
			}
			else if (position == -1 && head->next != NULL) {
				//DrawRect(hdc, head->next->position, *(head->highlightColor), BUTTON_WIDTH, BUTTON_WIDTH);
				DraWButt(hdc, head->next) ;
				head->next->push = true;
				head->push = false;
			}
			else if (position == 0) {
				OutputDebugStringA(head->name);
				OutputDebugStringA("\n");
			}
		}
		head = head->next;
	}
	//Draw(head, wind, hdc);
}

void SystemInitialise(char* fileName, Button* newButton, Window* newWindow, HDC hdc, int pos, HWND hWnd ) {
	FILE* file;
	char str[256];
	file = fopen(fileName, "r");
	int isButton = 0;
	while (fgets(str, 256, file)) {
		if (strcmp(str, "\tButtonBegin\n") == 0) {
			isButton = 1;
			newButton = malloc(sizeof(Button));
			//newButton->color = NULL;
			//newButton->highlightColor = malloc(sizeof(Color));
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
			//newWindow->color = malloc(sizeof(Color));
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
			else if (sscanf(str, "\t\tHighlightColor=(%d,%d,%d)\n", &newButton->HighlightColor[0], &newButton->HighlightColor[1], &newButton->HighlightColor[2]) == 3) {
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
	//Update(pos, buttonHeader, windowHeader, hdc);
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
