#pragma once
#include"../Text/Text.h"
#include"../OutParams/OutParams.h"
#include"../Scroll/Scroll.h"

void LOProcChangeWSize(LPARAM lParam, OUTPARAM* op, TEXT* text);

void LOProcChangeMode(OUTPARAM* op, TEXT* text);

void HSOut(HDC hdc, PAINTSTRUCT ps, OUTPARAM op, TEXT text);

void LOScrollV(HWND hwnd, WPARAM wParam, OUTPARAM *op, TEXT* text);
